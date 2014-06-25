    WITCH BLAST
============================
Author / code: Seby
Email: sebygames@gmail.com

Art: Sideous
Email: bianchi.jonathan94@gmail.com
and Seby

Music: Seby

2014
============================

Introduction
------------

Witch Blast is a free roguelite dungeon crawl shooter heavily inspired from Binding Of Isaac. The player plays as a novice magician in a dungeon who is trying to get as far as he can, using various items he can find to defeat the inhabitants of the dungeon.

Web: https://github.com/Cirrus-Minor/witchblast


Install
-------

If there is no binary for your version, you have to compile the application. You will need the library SFML, minimal version 2.1.
A CMake file is available.


Commands
--------

WASD (US, DE) or QSDZ (FR) to move in 8 directions
---
Arrows to shoot in 4 directions,
or
[Space] (can be hold) to shoot in the facing direction.
or
Mouse left-click
---
[Tab] to change shoot type (if you've found any).
or 
Mouse Wheel


Saving
--------

Game saves automatically when leaving the game in a cleared area.
Leaving the game during a fight destroys the current game.


Features
--------

- randomly generated dungeons,
- powerful items,
- monsters,
- chest and keys,
- merchant,
- candy eye effects.

Thanks
--------
Thanks to
Billx (who suggested me the title "Witch Blast")
Ruth (for the english corrections)
Mika Attila (for his contribution concerning the Linux version)

Changes
-------

Next version
- ADD New familiar (Ice Fairy)
- ADD New items (4 rings)
- ADD New monster and variants
- ADD Lightning's shot effect and sound
- ADD Special shots levels
- ADD Menu screen
- ADD Keys configuration
- ADD Death animation and sound for bats and rats
- MOD Bolt's bounding box is bigger when testing collision with monsters
- MOD Player's invincibility delay after being shot increases
- MOD Player's diagonal velocity decreases
- MOD Bosses are now in the middle of the room, 2 seconds idle
- MOD Giant Slime's latency decreases
- MOD Resistance and status immunity new system (boss can be frozen now, although it's difficult)
- MOD Player's step sound removed
- MOD New game with [LShift]+[X] now (used to be [X] alone)
- FIX Switching windows while player is dead
- FIX Player can fire while getting an item
- FIX Giant Slime life bar
- FIX King Rat protection while berserk

v0.1.0
- ADD New enemies (slime and black rat)
- ADD New items
- ADD Item's rarity
- ADD Show player's impact when he's hurt
- ADD New music when the player dies
- ADD New music for boss' fight
- ADD Mouse support - Player can fire with left click, switch shot types with the wheel
- ADD Pause
- ADD Save / restore game
- ADD Slowing (creature state)
- ADD HUD for shot types
- ADD 4 shot types
- ADD Recoil
- ADD New room templates
- ADD Exit stairs
- ADD Collisions between monsters
- ADD Screen's shaking effect
- ADD Configuration file
- ADD New tile's graphics
- MOD Damages inflicted by the player have been modified
- MOD The player's bolt size depend of the damages
- MOD Only useful data displayed on the minimap
- MOD Reduced bounding box for the player's bolt
- MOD Vertical Synchronisation
- MOD 2 health potions in shops now
- FIX Fade in / fade out text bugs
- FIX Merchant z-ordering bug
- FIX It was possible to begin at the exit
- FIX Various bad references crashes
- FIX Double shot used to double drop