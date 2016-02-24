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

const float MESSAGE_DURATION = 8.0f;
const float MESSAGE_DURATION_SHORT = 4.0f;

std::map<EnumMessages, messageBuilderStruct> msgMap =
{
  { MsgInfoLevel1,    { MessageTypeInfo, 1, "msg_level1",           3} },
  { MsgInfoLevel2,    { MessageTypeInfo, 1, "msg_level2",           3} },
  { MsgInfoLevel3,    { MessageTypeInfo, 1, "msg_level3",           3} },
  { MsgInfoLevel4,    { MessageTypeInfo, 1, "msg_level4",           3} },
  { MsgInfoLevel5,    { MessageTypeInfo, 1, "msg_level5",           3} },
  { MsgInfoLevel6,    { MessageTypeInfo, 1, "msg_level6",           3} },
  { MsgInfoLevel7,    { MessageTypeInfo, 1, "msg_level7",           3} },

  { MsgInfoRatsBats,  { MessageTypeInfo, 1, "msg_rats_bats",        3} },
  { MsgInfoSnakes,    { MessageTypeInfo, 1, "msg_snakes",           2} },
  { MsgInfoWitches,   { MessageTypeInfo, 1, "msg_witches",          2} },
  { MsgInfoGold,      { MessageTypeInfo, 1, "msg_gold",             3} },
  { MsgInfoFamiliar,  { MessageTypeInfo, 1, "msg_familiar",         3} },

  { MsgInfoButcher,           { MessageTypeInfo, 1, "msg_butcher",              3} },
  { MsgInfoGiantSlime,        { MessageTypeInfo, 1, "msg_giant_slime",          3} },
  { MsgInfoCyclops,           { MessageTypeInfo, 1, "msg_cyclops",              3} },
  { MsgInfoWererat,           { MessageTypeInfo, 1, "msg_wererat",              2} },
  { MsgInfoGiantSpiderBefore, { MessageTypeInfo, 1, "msg_giant_spider_before",  2} },
  { MsgInfoGiantSpiderAfter,  { MessageTypeInfo, 1, "msg_giant_spider_after",   2} },
  { MsgInfoFranky,            { MessageTypeInfo, 1, "msg_francky",              2} },
  { MsgInfoVampire,           { MessageTypeInfo, 1, "msg_vampire",              2} },

  { MsgInfoDivHealer,         { MessageTypeInfo, 0, "divinity_healer",            3} },
  { MsgInfoDivFighter,        { MessageTypeInfo, 0, "divinity_fighter",           3} },
  { MsgInfoDivIce,            { MessageTypeInfo, 0, "divinity_ice",               3} },
  { MsgInfoDivStone,          { MessageTypeInfo, 0, "divinity_stone",             3} },
  { MsgInfoDivAir,            { MessageTypeInfo, 0, "divinity_air",               3} },
  { MsgInfoDivIllusion,       { MessageTypeInfo, 0, "divinity_illusion",          3} },
  { MsgInfoDivFire,           { MessageTypeInfo, 0, "divinity_fire",              3} },
  { MsgInfoDivPoison,         { MessageTypeInfo, 0, "divinity_poison",            3} },

  { MsgInfoDivIntervention,   { MessageTypeInfo, 0, "divinity_intervention",      2} },
  { MsgInfoDivGift,           { MessageTypeInfo, 0, "divinity_gift",              2} },
  { MsgInfoDivLevel,          { MessageTypeInfo, 0, "divinity_leveling",          2} },

  { MsgInfoPotionId,          { MessageTypeInfo, 0, "msg_potion_id",              2} },

  { MsgTutoBasics,            { MessageTypeTutorial, 0, "msg_tuto_intro",           3} },
  { MsgTutoTips,              { MessageTypeTutorial, 0, "msg_tuto_tips",            3} },
  { MsgTutoItems,             { MessageTypeTutorial, 0, "msg_tuto_items",           3} },
  { MsgTutoShops,             { MessageTypeTutorial, 0, "msg_tuto_shops",           3} },
  { MsgTutoBossDoor,          { MessageTypeTutorial, 0, "msg_tuto_boss_door",       3} },
  { MsgTutoChallengeDoor,     { MessageTypeTutorial, 0, "msg_tuto_challenge_door",  3} },
  { MsgTutoTemple,            { MessageTypeTutorial, 0, "msg_tuto_temple",          3} },
  { MsgTutoHeal,              { MessageTypeTutorial, 0, "msg_tuto_heal",            3} },
  { MsgTutoShots,             { MessageTypeTutorial, 0, "msg_tuto_shots",           3} },
  { MsgTutoSpell,             { MessageTypeTutorial, 0, "msg_tuto_spell",           3} },
  { MsgTutoAchievements,      { MessageTypeTutorial, 0, "msg_tuto_achiev",          3} },
  { MsgTutoConsumables,       { MessageTypeTutorial, 0, "msg_tuto_consumables",     3} },
  { MsgTutoPotions,           { MessageTypeTutorial, 0, "msg_tuto_potions",         3} },
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
  msg.messageType = msgBuilder.type; //MessageTypeInfo;
  msg.type = type;
  msg.timer = msgBuilder.nbLines == 3 ? MESSAGE_DURATION : MESSAGE_DURATION_SHORT;
  msg.timerMax = msg.timer;
  msg.message[0] = "";
  msg.message[1] = "";
  msg.message[2] = "";
  loadMessageData(msg.message, msgBuilder.key, msgBuilder.nbLines);

  return msg;
}

#endif // MESSAGEGENERATOR_H_INCLUDED
