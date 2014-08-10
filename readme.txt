    WITCH BLAST
============================
Author / code - art - music: Seby
Email: sebygames@gmail.com

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
- different shot types,
- monsters,
- chest and keys,
- merchant,
- candy eye effects.

Thanks
--------
Thanks to
Sideous (for the main character design and graphics)
Billx (who suggested me the title "Witch Blast")
Ruth (for the english corrections)
Mika Attila (for his contribution concerning the Linux version)

Credits
-------
The sound used in the game come from:
- http://www.freesound.org/
- http://www.freesfx.co.uk/

Changes
-------

Next version
- ADD New level with a new boss
- ADD New items
- ADD New familiars (fire and target fairies)
- ADD New monsters variants (rats with helmet, violet slime)
- ADD New monster (bubble)
- ADD New shot type (fire)
- ADD Challenge Room
- ADD Explosions
- ADD Chest's trap
- ADD New tiles : holes
- ADD New decorative tiles
- ADD Display of health items on the minimap
- ADD Display of the damage in the game area
- ADD New graphics for Rat King
- MOD Health items drop's rate decreases
- MOD Rat King berserk's mode is easier now
- MOD Cyclops' falling stones bounce (one time)
- MOD Cyclops has protection before stones are falling
- MOD Butcher drops a silver coin now
- FIX Sprite center and Z-order
- FIX Giant Slime bug when falling on the wall 
- FIX Freeze when opening a chest while having all the items (but not the fairies)
- FIX Player does not bounce while being repulsed

v0.2.5
- ADD New mini-boss
- ADD New first level - easier
- ADD Time control (2x faster)
- ADD Display of HP/HPmax on the life bar
- ADD New health items: apple, bread, cheese (no more potions)
- ADD Monsters can drop health item
- MOD Shot distance is longer
- MOD Acquiring stance can be broken after the half of the time
- MOD Changes in item' level
- MOD Blood Snake damage's bonus decreases 
- MOD Merchant sells cheese an bread now
- FIX Lost item when leaving the game while acquiring the item
- FIX Monsters can leave the game zone while lagging or windows moving
- FIX Player can walk through south wall at the door
- FIX Player's death display artefacts

v0.2.0
- ADD New familiar (Ice Fairy)
- ADD New items (4 rings)
- ADD New monster and variants
- ADD New level
- ADD New boss
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
- MOD Tutorial's text (first room) removed
- FIX Switching windows while player is dead
- FIX Player can fire while getting an item
- FIX Player does not drop fairies anymore when dying
- FIX Giant Slime life bar corrected
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