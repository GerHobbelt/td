//
// Copyright Aliaksei Levin (levlam@telegram.org), Arseny Smirnov (arseny30@gmail.com) 2014-2021
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#pragma once

#include "td/telegram/td_api.h"
#include "td/telegram/telegram_api.h"
#include "td/telegram/UserId.h"

#include "td/utils/common.h"
#include "td/utils/tl_helpers.h"

namespace td {

class Td;

class BotCommand {
  string command_;
  string description_;

 public:
  BotCommand() = default;
  BotCommand(telegram_api::object_ptr<telegram_api::botCommand> &&bot_command);

  td_api::object_ptr<td_api::botCommand> get_bot_command_object() const;

  template <class StorerT>
  void store(StorerT &storer) const {
    td::store(command_, storer);
    td::store(description_, storer);
  }

  template <class ParserT>
  void parse(ParserT &parser) {
    td::parse(command_, parser);
    td::parse(description_, parser);
  }
};

class BotCommands {
  UserId bot_user_id_;
  vector<BotCommand> commands_;

 public:
  BotCommands() = default;
  BotCommands(UserId bot_user_id, vector<telegram_api::object_ptr<telegram_api::botCommand>> &&bot_commands);

  td_api::object_ptr<td_api::botCommands> get_bot_commands_object(Td *td) const;

  template <class StorerT>
  void store(StorerT &storer) const {
    td::store(bot_user_id_, storer);
    td::store(commands_, storer);
  }

  template <class ParserT>
  void parse(ParserT &parser) {
    td::parse(bot_user_id_, parser);
    td::parse(commands_, parser);
  }
};

}  // namespace td
