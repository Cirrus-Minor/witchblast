/**  This file is part of Witch Blast.
  *
  *  Witch Blast is free software: you can redistribute it and/or modify
  *  it under the terms of the GNU General Public License as published by
  *  the Free Software Foundation, either version 3 of the License, or
  *  (at your option) any later version.
  *

  *  Witch Blast is distributed in the hope that it will be useful,
  *  but WITHOUT ANY WARRANTY; without even the implied warranty of
  *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  *  GNU General Public License for more details.
  *
  *  You should have received a copy of the GNU General Public License
  *  along with Witch Blast.  If not, see <http://www.gnu.org/licenses/>.
  */

#ifndef MESSAGEGENERATOR_H_INCLUDED
#define MESSAGEGENERATOR_H_INCLUDED

#include "WitchBlastGame.h"
#include "TextMapper.h"

#include <sstream>

static void loadMessageData(std::string msg_array[NB_MSG_LINES], std::string key, int nbLines)
{
  for (int i = 0; i < nbLines; i++)
  {
    std::stringstream ss;
    ss << key << "_" << i;
    msg_array[i] = tools::getLabel(ss.str());
  }
}

static messageStruct getMessage(EnumMessages type)
{
  messageStruct msg;

  // init
  msg.icon = -1;
  msg.messageType = MessageTypeInfo;
  msg.type = type;
  msg.timer = 6.0f;
  msg.message[0] = "";
  msg.message[1] = "";
  msg.message[2] = "";

  // treatment
  switch (type)
  {
  case MsgIntro:
    loadMessageData(msg.message, "msg_intro", 3);
    break;

  case MsgInfoRatsBats:
    loadMessageData(msg.message, "msg_rats_bats", 3);
    break;

  case MsgInfoGold:
    loadMessageData(msg.message, "msg_gold", 3);
    break;

  case MsgInfoButcher:
    loadMessageData(msg.message, "msg_butcher", 3);
    break;

  case MsgInfoLevel2:
    loadMessageData(msg.message, "msg_level2", 3);
    break;

  case MsgTutoIntro:
    loadMessageData(msg.message, "msg_tuto_intro", 3);
    msg.messageType = MessageTypeTutorial;
    break;

  case MsgTutoHeal:
    loadMessageData(msg.message, "msg_tuto_heal", 3);
    msg.messageType = MessageTypeTutorial;
    break;
  };

  // finalize
  msg.timerMax = msg.timer;
  if (msg.icon < 0)
  {
    switch (msg.messageType)
    {
      case MessageTypeInfo: msg.icon = 1; break;
      case MessageTypeTutorial: msg.icon = 0; break;
    }
  }

  return msg;
}

#endif // MESSAGEGENERATOR_H_INCLUDED
