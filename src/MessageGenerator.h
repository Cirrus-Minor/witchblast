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
#include <map>

struct messageBuilderStruct
{
  EnumMessagesType type;
  int icon;
  std::string key;
  int nbLines;
};

const float MESSAGE_DURATION = 6.0f;

std::map<EnumMessages, messageBuilderStruct> msgMap =
{
  { MsgIntro,         { MessageTypeInfo, 1, "msg_intro",            3} },
  { MsgInfoRatsBats,  { MessageTypeInfo, 1, "msg_rats_bats",        3} },
  { MsgInfoGold,      { MessageTypeInfo, 1, "msg_gold",             3} },
  { MsgInfoButcher,   { MessageTypeInfo, 1, "msg_butcher",          3} },
  { MsgInfoLevel2,    { MessageTypeInfo, 1, "msg_level2",           3} },

  { MsgTutoIntro,     { MessageTypeTutorial, 0, "msg_tuto_intro",   3} },
  { MsgTutoHeal,      { MessageTypeTutorial, 0, "msg_tuto_heal",    3} },
  { MsgTutoShots,     { MessageTypeTutorial, 0, "msg_tuto_shots",   3} },
  { MsgTutoSpell,     { MessageTypeTutorial, 0, "msg_tuto_spell",   3} },
};

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
  messageBuilderStruct msgBuilder = msgMap[type];
  // init
  msg.icon = msgBuilder.icon; //-1;
  msg.messageType = msgBuilder.type; //MessageTypeInfo;
  msg.type = type;
  msg.timer = MESSAGE_DURATION;
  msg.timerMax = msg.timer;
  msg.message[0] = "";
  msg.message[1] = "";
  msg.message[2] = "";
  loadMessageData(msg.message, msgBuilder.key, msgBuilder.nbLines);

  return msg;
}

#endif // MESSAGEGENERATOR_H_INCLUDED
