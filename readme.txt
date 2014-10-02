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
Standard commands (can be changed in the game):

WASD (US, DE) or QSDZ (FR) to move in 8 directions
---
Arrows to shoot in 4 directions,
or
[Right CTRL] (can be hold) to shoot in the facing direction.
or
Mouse left-click
---
[Tab] to change shoot type (if you've found any).
or 
Mouse Wheel
---
[Space] to cast a spell (if you've found any).


Saving
--------

Game saves automatically when leaving the game in a cleared area.
Leaving the game while fighting destroys the current game.


Features
--------

- randomly generated dungeons,
- powerful items,
- different shot types,
- monsters,
- chest and keys,
- merchant,
- spells with cooldown,
- candy eye effects.

Credits
-------
Seby: code, music and 2D art
Sideous: the main character design and graphics
Portoss01: bats and snake graphics
R0d: particles system and multilingual support coding

The sound used in the game come from:
- http://www.freesound.org/
- http://www.freesfx.co.uk/
- http://www.universal-soundbank.com/

Thanks
--------
Thanks to

Billx (who suggested me the title "Witch Blast")
Mika Attila (for his "coding" contribution during the development)
Ruth (for the english corrections)
Geheim (for the German translation)
Achpile (for the Russian version and his help in the code)
AFS (for the Spanish translation)
Lord Archibald (who helps me writing the story)

Changes
-------
Next release
- ADD New monsters (snakes)
- ADD New items (spell orbs)
- ADD Poison
- ADD Cold bolt (generated with an unloaded ice shot)
- ADD Spells (5 different ones)
- ADD Translations: Spanish and Russian
- ADD New art for bats
- ADD Texts "widgets" for story and tutorial
- MOD Vibration Gloves are now Displacement Glove (player's velocity impacts bolt velocity)
- MOD Items presentation windows colour has changed
- MOD Repulsion improved
- FIX Texts are now in UTF-8
- FIX Monsters out of the game zone
- FIX Persistent particles on some machines/systems

v0.3.3
- ADD German version
- FIX Crash when getting the quartz ring

v0.3.2
- ADD Particles for player's missiles
- ADD Death Screen
- ADD Force shop generation if there is no one in the previous floor
- ADD Display gold when getting coins
- ADD New starting screen with language selection (English or French) and standard keyboard configuration
- MOD Invert Cyclop and Rat King bosses
- MOD Dual bolt angle is smaller
- MOD Damage's display size increases with damage
- MOD Rat with helmet are slower
- MOD Green rats' don't collide while spawning
- MOD Fairies don't stay stuck in wall/door 
- MOD Player's bounding box is now smaller
- MOD Rat and bat's dying sound's volume is lower now
- MOD Better Bubble physics
- MOD Target Fairy (Scarlett) firerate decreased
- FIX Wrong HP display (bar) when a boss is dying
- FIX Items are not dropped out of screen anymore

v0.3.0
- ADD New level with a new boss
- ADD New items
- ADD New familiars (fire and target fairies)
- ADD New monsters variants (rats with helmet, violet slime)
- ADD New monsters (bubble, witches, cauldron)
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
- MOD Evil Flower missile's damage decreased
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