//
// Copyright Aliaksei Levin (levlam@telegram.org), Arseny Smirnov (arseny30@gmail.com) 2014-2023
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#pragma once

#include "td/telegram/DialogId.h"
#include "td/telegram/td_api.h"
#include "td/telegram/telegram_api.h"

#include "td/utils/algorithm.h"
#include "td/utils/common.h"
#include "td/utils/Slice.h"
#include "td/utils/StringBuilder.h"

namespace td {

class Td;

class DialogFilterInviteLink {
  string invite_link_;
  string title_;
  vector<DialogId> dialog_ids_;

  friend bool operator==(const DialogFilterInviteLink &lhs, const DialogFilterInviteLink &rhs);

  friend StringBuilder &operator<<(StringBuilder &string_builder, const DialogFilterInviteLink &invite_link);

 public:
  DialogFilterInviteLink() = default;

  DialogFilterInviteLink(Td *td, telegram_api::object_ptr<telegram_api::exportedChatlistInvite> exported_invite);

  td_api::object_ptr<td_api::chatFilterInviteLink> get_chat_filter_invite_link_object() const;

  bool is_valid() const {
    return !invite_link_.empty();
  }
};

bool operator==(const DialogFilterInviteLink &lhs, const DialogFilterInviteLink &rhs);

bool operator!=(const DialogFilterInviteLink &lhs, const DialogFilterInviteLink &rhs);

StringBuilder &operator<<(StringBuilder &string_builder, const DialogFilterInviteLink &invite_link);

}  // namespace td
