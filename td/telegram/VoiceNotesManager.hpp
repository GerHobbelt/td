//
// Copyright Aliaksei Levin (levlam@telegram.org), Arseny Smirnov (arseny30@gmail.com) 2014-2022
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#pragma once

#include "td/telegram/VoiceNotesManager.h"

#include "td/telegram/files/FileId.hpp"
#include "td/telegram/Version.h"

#include "td/utils/common.h"
#include "td/utils/tl_helpers.h"

namespace td {

template <class StorerT>
void VoiceNotesManager::store_voice_note(FileId file_id, StorerT &storer) const {
  auto it = voice_notes_.find(file_id);
  CHECK(it != voice_notes_.end());
  const VoiceNote *voice_note = it->second.get();
  bool has_mime_type = !voice_note->mime_type.empty();
  bool has_duration = voice_note->duration != 0;
  bool has_waveform = !voice_note->waveform.empty();
  BEGIN_STORE_FLAGS();
  STORE_FLAG(has_mime_type);
  STORE_FLAG(has_duration);
  STORE_FLAG(has_waveform);
  STORE_FLAG(voice_note->is_transcribed);
  END_STORE_FLAGS();
  if (has_mime_type) {
    store(voice_note->mime_type, storer);
  }
  if (has_duration) {
    store(voice_note->duration, storer);
  }
  if (has_waveform) {
    store(voice_note->waveform, storer);
  }
  if (voice_note->is_transcribed) {
    store(voice_note->transcription_id, storer);
    store(voice_note->text, storer);
  }
  store(file_id, storer);
}

template <class ParserT>
FileId VoiceNotesManager::parse_voice_note(ParserT &parser) {
  auto voice_note = make_unique<VoiceNote>();
  bool has_mime_type;
  bool has_duration;
  bool has_waveform;
  if (parser.version() >= static_cast<int32>(Version::AddVoiceNoteFlags)) {
    BEGIN_PARSE_FLAGS();
    PARSE_FLAG(has_mime_type);
    PARSE_FLAG(has_duration);
    PARSE_FLAG(has_waveform);
    PARSE_FLAG(voice_note->is_transcribed);
    END_PARSE_FLAGS();
  } else {
    has_mime_type = true;
    has_duration = true;
    has_waveform = true;
    voice_note->is_transcribed = false;
  }
  if (has_mime_type) {
    parse(voice_note->mime_type, parser);
  }
  if (has_duration) {
    parse(voice_note->duration, parser);
  }
  if (has_waveform) {
    parse(voice_note->waveform, parser);
  }
  if (voice_note->is_transcribed) {
    parse(voice_note->transcription_id, parser);
    parse(voice_note->text, parser);
  }
  parse(voice_note->file_id, parser);
  if (parser.get_error() != nullptr || !voice_note->file_id.is_valid()) {
    return FileId();
  }
  return on_get_voice_note(std::move(voice_note), false);
}

}  // namespace td
