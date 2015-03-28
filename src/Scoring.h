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

#ifndef SCORING_H_INCLUDED
#define SCORING_H_INCLUDED

int getMonsterScore(enemyTypeEnum monster)
{
  int score = 0;
  switch (monster)
  {
    case EnemyTypeBat:            score = 2; break;
    case EnemyTypeRat:            score = 2; break;
    case EnemyTypeEvilFlower:     score = 2; break;
    case EnemyTypeRatBlack:       score = 2; break;

    case EnemyTypeSnake:          score = 5; break;
    case EnemyTypeRatHelmet:      score = 5; break;
    case EnemyTypeRatBlackHelmet: score = 5; break;
    case EnemyTypeSlime:          score = 5; break;

    case EnemyTypeImpBlue:        score = 8; break;
    case EnemyTypeImpRed:         score = 8; break;
    case EnemyTypeEvilFlowerIce:  score = 8; break;
    case EnemyTypeEvilFlowerFire: score = 8; break;
    case EnemyTypeSpiderEgg:      score = 8; break;
    case EnemyTypeSpiderLittle:   score = 8; break;
    case EnemyTypeBatSkeleton:    score = 8; break;

    case EnemyTypeSnakeBlood:     score = 10; break;
    case EnemyTypeSlimeRed:       score = 10; break;
    case EnemyTypeSlimeBlue:      score = 10; break;
    case EnemyTypeSlimeViolet:    score = 10; break;

    case EnemyTypePumpkin:        score = 15; break;

    case EnemyTypeWitch:          score = 25; break;
    case EnemyTypeWitchRed:       score = 25; break;
    case EnemyTypeCauldron:       score = 25; break;
    case EnemyTypeGhost:          score = 25; break;
    case EnemyTypeZombie:         score = 25; break;
    case EnemyTypeZombieDark:     score = 25; break;

    case EnemyTypeBubble:         score = 25; break;
    case EnemyTypeBubbleIce:      score = 30; break;
    case EnemyTypeBubbleGreater:  score = 35; break;

    case EnemyTypeSlimeLarge:     score = 30; break;
    case EnemyTypeSlimeBlueLarge: score = 45; break;
    case EnemyTypeSlimeRedLarge:  score = 45; break;
    case EnemyTypeSlimeVioletLarge:  score = 45; break;

    case EnemyTypeButcher:        score = 30; break;
    case EnemyTypeSlimeBoss:      score = 60; break;
    case EnemyTypeCyclops:        score = 120; break;
    case EnemyTypeRatKing:        score = 180; break;
    case EnemyTypeSpiderGiant:    score = 240; break;
    case EnemyTypeFranckyHead:    score = 300; break;
    case EnemyTypeVampire:        score = 350; break;

    case EnemyTypeBat_invocated:
    case EnemyTypeBatSkeleton_invocated:
    case EnemyTypeRat_invocated:
    case EnemyTypeRatGreen:
    case EnemyTypeRatHelmet_invocated:
    case EnemyTypeSnake_invocated:
    case EnemyTypeSnakeBlood_invocated:
    case EnemyTypeSlime_invocated:
    case EnemyTypeSlimeBlue_invocated:
    case EnemyTypeSlimeRed_invocated:
    case EnemyTypeSlimeViolet_invocated:
    case EnemyTypePumpkin_invocated:
    case EnemyTypeRockFalling:
    case EnemyTypeRockMissile:
    case EnemyTypeSpiderEgg_invocated:
    case EnemyTypeSpiderLittle_invocated:
    case EnemyTypeSpiderWeb:
    case EnemyTypeZombie_invocated:
    case EnemyTypeFrancky:
    case EnemyTypeFranckyFoot:
    case EnemyTypeFranckyHand:
    case EnemyTypeVampireDead:
      score = 0; break;

    case EnemyTypeNone:
    case NB_ENEMY:
      score = 0; break;
  }
  return score;
}

// 100 points / completed challenge
int getChallengeScore(int challengeLevel)
{
  return 100 * (challengeLevel - 1);
}

// 1 point = 1 gold
int getGoldScore(int gold)
{
  return gold;
}

// points = half price value
int getItemScore(item_equip_enum item)
{
  int points = items[(int) item + FirstEquipItem].price / 2;
  return points;
}

#endif // SCORING_H_INCLUDED
