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
Seby: code and music 
Vetea: 2D art
R0d: particles system and multilingual support coding

Music
Our Ship To Candletown from Michael Ghelfi - https://www.youtube.com/user/MichaelGhelfi
Music Is His Only Friend from Michael Ghelfi
Marching United from JappeJ - http://jappejj.bandcamp.com/
Fanatsy- Theme of Elvarim from SteveSyz - http://stevesyz.newgrounds.com/
PubStomp Deluxe from CinTer - http://cinter.newgrounds.com/
The Spider Machine from cazok - http://cazok.newgrounds.com/
Under Siege from ET16 - http://et16.newgrounds.com/

The sound used in the game come from:
- http://www.freesound.org/
- http://www.freesfx.co.uk/
- http://www.universal-soundbank.com/

Licenses
--------
Code: GPLv3
Art (2D and music): CC BY-NC-SA

Thanks
--------
Thanks to

Sideous and Portoss01 (they provide me 2D art in earlier versions)
Billx (who suggested me the title "Witch Blast")
Mika Attila (for his "coding" contribution during the development)
Ruth (for the english corrections)
Geheim (for the German translation)
Achpile (for the Russian version and his help in the code)
AFS (for the Spanish translation)

Changes
-------
v0.4.1
- ADD Scores and hi-score screen
- ADD Blood is spraying now
- ADD Sound and music volume in config menu
- ADD Options to enable/disable zoom, vssync, blood spraying, in config file
- ADD Auto-save at the beginning of a fight (game is not wasted in case of crash or freeze)
- MOD Level 5 and level 6 are easier now
- MOD Sorted inventory
- MOD Optimizations (use of vertex arrays)
- MOD Don't distort game screen when resizing the window
- MOD Corpses don't stay on walls
- FIX Credit screen crash while pressing arrow

v0.4.0
- ADD New level
- ADD New monsters
- ADD New items
- ADD New spell
- ADD New art for sprites and tiles
- ADD New room surfaces
- ADD New intro and menu
- ADD Levels have different tilesets now 
- ADD In-game menu (pause)
- ADD Inventory display in the in-game menu
- ADD Chest for completed challenges
- ADD Tutorial state save and reset
- ADD Cauldron now explodes while dying
- ADD Sound for imp's teleportation
- ADD "Jump" animation for items you can get
- ADD Zoom effect when starting / dying
- ADD In-fight save game
- ADD Mouse right click support (spell)
- ADD Show selected key in key configuration
- ADD Credits screen
- MOD Game is faster now
- MOD Standard Rat IA changes (rats now move in 4 directions)
- MOD Frozen dividing bubbles generate frozen "children" bubbles
- MOD Fireball (spell) now goes through monsters (if enough HP)
- MOD Lightning bolt now goes through monsters (if enough HP)
- MOD Cyclops' damage decreases
- MOD Giant Spider is more difficult to kill now
- MOD Shop's items too expensive displayed in red
- MOD One cauldron max in witches rooms in level 5
- MOD New death screen
- MOD Exploding fire shot replaced with sulfur item
- MOD Exploding corpse with timer now
- MOD Snake AI changes (no more "group attack)
- MOD Witches and Flowers use ray casting to fire or not
- MOD Cauldron + Witches only on rooms with walls
- MOD Bats and little slime HP-
- MOD "Elemental" monsters resistance or weakness increase
- MOD Double and triple quick shots don't do so much damage when cumulated with damage items
- FIX Cyclops firing in the wall when too close to the north wall
- FIX Items can fly a long time on the walls
- FIX Clean message queue while starting a new game

v0.3.7
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
- MOD Giant Slime HP decreased
- MOD Cyclops melee and missile damage decreased, now repulse
- MOD A merchant on each floor > 1 now
- MOD Force Challenge room for each floor > 2 if no spell is equipped
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