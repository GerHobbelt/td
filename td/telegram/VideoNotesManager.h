//
// Copyright Aliaksei Levin (levlam@telegram.org), Arseny Smirnov (arseny30@gmail.com) 2014-2023
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#pragma once

#include "td/telegram/Dimensions.h"
#include "td/telegram/files/FileId.h"
#include "td/telegram/MessageFullId.h"
#include "td/telegram/PhotoSize.h"
#include "td/telegram/SecretInputMedia.h"
#include "td/telegram/td_api.h"
#include "td/telegram/telegram_api.h"
#include "td/telegram/TranscriptionInfo.h"

#include "td/actor/actor.h"

#include "td/utils/buffer.h"
#include "td/utils/common.h"
#include "td/utils/FlatHashMap.h"
#include "td/utils/FlatHashSet.h"
#include "td/utils/Promise.h"
#include "td/utils/Status.h"
#include "td/utils/WaitFreeHashMap.h"

namespace td {

class Td;

class VideoNotesManager final : public Actor {
 public:
  VideoNotesManager(Td *td, ActorShared<> parent);
  VideoNotesManager(const VideoNotesManager &) = delete;
  VideoNotesManager &operator=(const VideoNotesManager &) = delete;
  VideoNotesManager(VideoNotesManager &&) = delete;
  VideoNotesManager &operator=(VideoNotesManager &&) = delete;
  ~VideoNotesManager() final;

  void memory_stats(vector<string> &output);

  int32 get_video_note_duration(FileId file_id) const;

  tl_object_ptr<td_api::videoNote> get_video_note_object(FileId file_id) const;

  void create_video_note(FileId file_id, string minithumbnail, PhotoSize thumbnail, int32 duration,
                         Dimensions dimensions, string waveform, bool replace);

  void register_video_note(FileId video_note_file_id, MessageFullId message_full_id, const char *source);

  void unregister_video_note(FileId video_note_file_id, MessageFullId message_full_id, const char *source);

  void recognize_speech(MessageFullId message_full_id, Promise<Unit> &&promise);

  void rate_speech_recognition(MessageFullId message_full_id, bool is_good, Promise<Unit> &&promise);

  tl_object_ptr<telegram_api::InputMedia> get_input_media(FileId file_id,
                                                          tl_object_ptr<telegram_api::InputFile> input_file,
                                                          tl_object_ptr<telegram_api::InputFile> input_thumbnail) const;

  SecretInputMedia get_secret_input_media(FileId video_note_file_id,
                                          tl_object_ptr<telegram_api::InputEncryptedFile> input_file,
                                          BufferSlice thumbnail, int32 layer) const;

  FileId get_video_note_thumbnail_file_id(FileId file_id) const;

  void delete_video_note_thumbnail(FileId file_id);

  FileId dup_video_note(FileId new_id, FileId old_id);

  void merge_video_notes(FileId new_id, FileId old_id);

  template <class StorerT>
  void store_video_note(FileId file_id, StorerT &storer) const;

  template <class ParserT>
  FileId parse_video_note(ParserT &parser);

 private:
  class VideoNote {
   public:
    int32 duration = 0;
    Dimensions dimensions;
    string waveform;
    string minithumbnail;
    PhotoSize thumbnail;
    unique_ptr<TranscriptionInfo> transcription_info;

    FileId file_id;
  };

  VideoNote *get_video_note(FileId file_id);

  const VideoNote *get_video_note(FileId file_id) const;

  FileId on_get_video_note(unique_ptr<VideoNote> new_video_note, bool replace);

  void on_video_note_transcription_updated(FileId file_id);

  void on_video_note_transcription_completed(FileId file_id);

  void on_transcribed_audio_update(FileId file_id, bool is_initial,
                                   Result<telegram_api::object_ptr<telegram_api::updateTranscribedAudio>> r_update);

  void tear_down() final;

  Td *td_;
  ActorShared<> parent_;

  WaitFreeHashMap<FileId, unique_ptr<VideoNote>, FileIdHash> video_notes_;

  FlatHashMap<FileId, FlatHashSet<MessageFullId, MessageFullIdHash>, FileIdHash> video_note_messages_;
  FlatHashMap<MessageFullId, FileId, MessageFullIdHash> message_video_notes_;
};

}  // namespace td
