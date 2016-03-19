#ifndef PLAYERSPRITE_H
#define PLAYERSPRITE_H

#include "BaseCreatureEntity.h"
#include "ItemEntity.h"
#include "Constants.h"

class FairyEntity;

struct castSpellStruct
{
  enumCastSpell spell;
  float delay;
  float delayMax;
  int frame;
};

struct divinityStruct
{
  int divinity;
  int piety;
  int level;
  int interventions;
  float percentsToNextLevels;
};

const int MAX_SLOT_CONSUMABLES = 4;

/*! \class PlayerEntity
* \brief Class for the player
*
*  It contains the game logic of the player and the rendering.
*/
class PlayerEntity : public BaseCreatureEntity
{
  public:
    /*!
     *  \brief Constructor
     *
     *  Constructor of the PlayerEntity class.
     *
     *  \param x : x position of the player
     *  \param y : y position of the player
     */
    PlayerEntity(float x, float y);

    /*!
     *  \brief updates the player
     *
     *  Updates the player.
     *  Called in the game loop.
     *
     *  \param delay : elapsed time since the last call
     */
    virtual void animate(float delay);

     /*!
     *  \brief render the player
     *
     *  Render the player.
     *  Called in the game loop.
     *
     *  \param app : Rendering target
     */
    virtual void render(sf::RenderTarget* app);

    /*!
     *  \brief Moves the player to another place
     *
     *  Moves the player to another place.
     *  Called when changing room, the "familiers" will move too.
     *
     *  \param newX : target x position of the player
     *  \param newY : target y position of the player
     */
    void moveTo(float newX, float newY);

    /*!
     *  \brief returns the direction the player is facing
     *
     *  Return the direction the player is facing.
     *  Use to know in which direction he has to fire with the "one button" gameplay.
     *
     *  \return : the facing direction. 4 = left, 8 = north, 6 = right, 2 = south
     */
    int getFacingDirection();

    void setFacingDirection(int facingDirection);

     /*!
     *  \brief update the bounding box
     *
     *  Update the bounding box of the player.
     *  Used before testing collision.
     */
    virtual void calculateBB();

    virtual bool canCollide();

    /*!
     *  \brief Moves the player in the given direction
     *
     *  Moves the player in the given direction.
     *
     *  \param direction : direction of the new map. Numeric pad, diagonals included : 4 = left, 8 = north, 7 = north-west...
     */
    void move(int direction);

    /*!
     *  \brief Fires in the given direction
     *
     *  Fires the player in the given direction.
     *
     *  \param direction : direction of the new map. 4 = left, 8 = north, 6 = right, 2 = south
     */
    void fire(int direction);

    /*!
     *  \brief reset the fire direction of the player
     */
    void resetFireDirection();

    /*!
     *  \brief accessor on the fire direction of the player
     */
    int getFireDirection();

    /*!
     *  \brief returns if the player is moving or not
     *
     *  Returns if the player is moving or not.
     *
     *  \return : True if the player is moving
     */
    bool isMoving();

     /*!
     *  \brief returns if an item is equipped or not
     *
     *  Returns if an item is equipped or not.
     *
     *  \param eq : the equip item ID
     *  \return : True if the item is in possession of the player
     */
    bool isEquiped(int eq);

    bool* getEquipment();

/*!
     *  \brief returns if the player is poisoned or not
     *
     *  \return : True if the player is poisoned
     */
    bool isPoisoned();

    /*!
     *  \brief updates the equipment of the player
     *
     *  Updates the equipment of the player.
     *
     *  \param item : the equip item ID
     *  \param toggleEquipped : True if the item has to be equipped
     */
    void setEquipped(int item, bool toggleEquipped, bool isFairyPlayer = false);

    divinityStruct getDivinity();
    int getPiety();
    bool getFairyTransmuted();

    /*!
     *  \brief updates the entering status of the player
     *
     *  Updates the entering status of the player.
     *  Used when the player is entering in a not yet cleared room.
     */
    void setEntering();

    /*!
     *  \brief updates the status of the player to going up
     *
     *  Updates the status of the player to going up.
     *  Used when the player is leaving a level.
     */
    void setLeavingLevel();

    float getFireRate();

     /*!
     *  \brief returns if the player can move or not
     *
     *  Returns if the player can move or not.
     *
     *  \return : True if the player can move
     */
    bool canMove();

    /*!
     *  \brief called when the player is dying
     *
     *  Called when the player is dying (HP <= 0).
     */
    virtual void dying();

    /*!
     *  \brief hurts the player
     *
     *  Hurts the player.
     * Calld when the player is hurt, usually when colliding with a monster or a missile.
     *
     *  \param damages : the inflicted damages
     *  \param hurtingType : damages type
     *  \param level : damages level
     *  \return : True if the player has been hurt
     */
    virtual int hurt(StructHurt hurtParam) override;

    /*!
     *  \brief returns if the player is dead or not
     *
     *  Returns if the player is dead or not.
     *
     *  \return : True if the player is dead
     */
    bool isDead();

    /*!
     *  \brief returns the fire delay percentage
     *
     *  Returns the fire delay percentage.
     *  Used (currently) when displaying the blue bar in the HUD.
     *
     *  \return : the percentage (between 0.0f for empty to 1.0 for full)
     */
    float getPercentFireDelay();

    /*!
     *  \brief returns the light cone percentage
     *
     *  \return : the percentage (between 0.0f for empty to 1.0 for full) or -1.0f if no light cone
     */
    float getFadingDivinity(bool showCone);

    /*!
     *  \brief called when the player get an item
     *
     *  Called when the player get an item.
     *  When the item is an equip item, he starts an "acquiring stance".
     *
     *  \param type : item ID
     */
    void acquireItem(enumItemType type);

    /*!
     *  \brief called when the player get an item and ends the stance
     *
     */
    void acquireItemAfterStance();

     /*!
     *  \brief makes the player drop an item
     *
     *  Makes the player drop an item.
     *  Called when the player dies.
     *
     *  \param itemType : item ID
     *  \param isEquip : True if it's an equip item (not the same texture)
     */
    void loseItem(enumItemType itemType, bool isEquip);

     /*!
     *  \brief starts the acquire stance
     *
     *  Starts the acquire stance.
     *  Called when the player get an equip object. The item is "highlighted" and a description is displayed.
     *
     *  \param type : item ID
     */
    void acquireStance(enumItemType type);

    /*!
     *  \brief starts the opening stance
     *
     *  Starts the boss door opening animation.
     *  Remove the key from the inventory.
     */
    void useBossKey();

    int getLostHp(int level);
    void setLostHp(int level, int n);

    /*!
     *  \brief accessor on the gold
     *
     *  Accessor on the gold.
     *
     *  \return : the gold
     */
    int getGold() {return gold; }

     /*!
     *  \brief mutator on the gold
     *
     *  Mutator on the gold.
     *
     *  \param gold : the new gold value
     */
    void setGold(int gold) { this->gold = gold; }

    /*!
     *  \brief accessor on the donation
     *
     *  Accessor on the donation.
     *
     *  \return : the donation
     */
    int getDonation() {return donation; }

     /*!
     *  \brief mutator on the donation
     *
     *  Mutator on the donation.
     *
     *  \param gold : the new donation value
     */
    void setDonation(int gold) { this->donation = gold; }

    /*!
     *  \brief pay some gold
     *
     *  Pay some gold. Usually in shops.
     *
     *  \param gold : the price to pay
     */
    void pay(int price);

    void displayAcquiredGold(int n);

    /** Player status enum
     *  The different player states.
     */
    enum playerStatusEnum
    {
      playerStatusPlaying,    /**< Player is playing "normally" */
      playerStatusEntering,   /**< Player is entering a not yet cleared room (walking is forced) */
      playerStatusAcquire,    /**< Player is under acquiring stance */
      playerStatusUnlocking,  /**< Player is under unlocking stance */
      playerStatusPraying,    /**< Player is under unlocking stance */
      playerStatusStairs,     /**< Player walk the stairs */
      playerStatusGoingNext,  /**< Player goes to next level */
      playerStatusDead,       /**< Player RIP */
      playerStatusVictorious  /**< Game won */
    };

    /*!
     *  \brief accessor on the player status
     *
     *  Accessor on the player status.
     *
     *  \return : the player status
     */
    playerStatusEnum getPlayerStatus();

     /*!
     *  \brief mutator on the player status
     *
     *  Mutator on the player status.
     *
     *  \param player status : the new player status value
     */
    void setPlayerStatus(playerStatusEnum playerStatus);

    /*!
     *  \brief accessor on the colliding direction
     *
     *  Accessor on the colliding direction.
     *
     *  \return : the colliding direction
     */
    int getCollidingDirection();

    /*!
     *  \brief compute all the player data (stats / items)
     */
    void computePlayer();

    /*!
     *  \brief register a new special shot and select it
     *
     *  \param item : Item which provides the shot
     */
    void registerSpecialShot(int item);

    /*!
     *  \brief accessor on current shot type
     *
     *  \return : the current shot type
     */
    enumShotType getShotType();

    /*!
     *  \brief accessor on current shot level
     *
     *  \return : the current shot level
     */
    unsigned int getShotLevel();

    /*!
     *  \brief accessor a shot level
     *  \param index : the index of the shot
     *  \return : the shot level
     */
    unsigned int getShotLevel(int index);

    /*!
     *  \brief accessor on current shot index
     *
     *  Accessor on the current shot index.
     *
     *  \return : the current shot index
     */
    int getShotIndex();

    /*!
     *  \brief mutator on the shot index
     *
     *  Mutator on the shot index.
     *
     *  \param index : the new shot index
     */
    void setShotIndex(int index);

    /*!
     *  \brief accessor on current shot type of a slot
     *
     *  \param slot : The slot number
     *  \return : the current shot type
     */
    enumShotType getShotType(int slot);

    /*!
     *  \brief mutator on a shot slot
     *
     *  Mutator on a shot slot.
     *
     *  \param index : the shot index
     *  \param shotType : the shot type to set
     */
    void setShotType(int slot, enumShotType shotType);

    /*!
     *  \brief select the next shot type
     *
     *  Select the next shot type.
     */
    void selectNextShotType();
    void selectShotType(int n);

    /*!
     *  \brief accessor on the fairies number
     */
    int getFairieNumber();

    /*!
     *  \brief accessor on a fairy
     *  \param n : index of the fairy in the vector
     */
    FairyEntity* getFairy(unsigned int n);

    /*!
     *  \brief checks if the player can get a new shot type
     *
     *  \param advancedShot : true if the shot type is "advanced"
     *  \return : true if there is a free slot
     */
    bool canGetNewShot(bool advancedShot);

    float getEndAge();
    void setEndAge(float endAge);

    virtual void setMap(GameMap* map, int tileWidth, int tileHeight, int offsetX, int offsetY) override;

    /*!
     *  \brief casts a spell (if possible)
     */
    void castSpell();

    void onClearRoom();
    void resetFloorItem();

    void interact(EnumInteractionType interaction, int id);
    void worship(enumDivinityType divinity);
    void donate(int n);
    void offerMonster(enemyTypeEnum monster, enumShotType hurtingType);
    void offerHealth(int lostHp);
    void offerChallenge();
    void offerSecret();
    void addPiety(int n);
    void pietyLevelUp();
    void loadDivinity(int id, int piety, int level, int interventions);
    bool triggerIllusionTeleport();
    bool triggerDivinityBefore();
    void triggerDivinityAfter();
    void divineFury();
    void divineDestroyUndead();
    void divineIce();
    void divineRepulse();
    void divineFire();
    void divineProtection(float duration, float armorBonus);
    void divineHeal(int hpHealed);

    void setActiveSpell(enumCastSpell spell, bool fromSaveInFight);
    castSpellStruct getActiveSpell();
    float getPercentSpellDelay();
    bool canCastSpell();

    enemyTypeEnum getLastHurtingEnemy();
    sourceTypeEnum getLastHurtingSource();

    virtual void setSpecialState(enumSpecialState state, bool active, float timer, float param1, float param2, bool waitUnclear = false) override;

    void setItemToBuy(ItemEntity* item);
    ItemEntity* getItemToBuy();

    void castTeleport();
    virtual bool collideWithMap(int direction) override;

    bool canAquireConsumable(enumItemType type);
    int getConsumable(int n);
    void setConsumable(int n, int type);
    void dropConsumables(int n);
    void tryToConsume(int n);
    void consume(enumItemType item);
    int getDamage();
    int getDps();

    bool seeInvisible();

  protected:
    virtual void readCollidingEntity(CollidingSpriteEntity* entity);
    void generateBolt(float velx, float vely);
    void rageFire(float damage, bool full, float velMult);

    virtual void collideMapRight();
    virtual void collideMapLeft();
    virtual void collideMapTop();
    virtual void collideMapBottom();

    virtual void stuck() override;

  private:
    int fireDamage;
    int dps;
    float fireVelocity;
    float fireDelay;
    float fireAnimationDelay;
    float fireAnimationDelayMax;
    int fireAnimationDirection;
    bool canAnimateFire;
    float spellAnimationDelay;
    float spellAnimationDelayMax;
    float currentFireDelay;
    float randomFireDelay;
    float rageFireDelay;
    float hiccupDelay;
    float boltLifeTime;
    int gold;
    int donation;
    int criticalChance;
    float invincibleDelay;
    float divineInterventionDelay;
    bool showCone;
    bool isRegeneration;
    bool isFairyTransmuted;
    int lostHp[LAST_LEVEL];

    bool canFirePlayer;
    playerStatusEnum playerStatus;
    float statusTimer;
    enumItemType acquiredItem;

    bool equip[NUMBER_EQUIP_ITEMS];
    SpriteEntity* spriteItem;
    SpriteEntity* spriteItemStar;

    float specialBoltTimer;
    enumShotType specialShots[SPECIAL_SHOT_SLOTS];
    unsigned int specialShotLevel[SPECIAL_SHOT_SLOTS];
    int specialShotIndex;
    bool needInitShotType;

    int collidingDirection;           /*!< Colliding direction (4, 8, 6, 2) to detect collision with closed doors */

    int firingDirection;

    int keyDirection;

    bool willCollideWithMap(int dx, int dy, bool checkMiddle);

    std::vector<FairyEntity*> fairies;

    float idleAge;
    float endAge;
    enemyTypeEnum lastHurtingEnemy;
    sourceTypeEnum lastHurtingSource;

    int spriteDy;
    void renderPlayer(sf::RenderTarget* app);
    void renderHalo(sf::RenderTarget* app);

    divinityStruct divinity;
    bool shouldBeSavedFromDivinity;

    void fallRock();
    void initFallingGrid();
    bool fallingGrid[MAP_WIDTH][MAP_HEIGHT];

    /*!
     *  \brief init the current shot type.
     *
     *  Init the current shot type.
     *  Called when the player get a new shot, or after a switch.
     */
    void initShotType();
    castSpellStruct activeSpell;

    struct protectionStruct
    {
      bool active;
      float value;
      float timer;
    } protection;

    void castSummonsSlimeExplode();
    void castFireball();
    void castFreeze();
    void castEarthquake();
    void castProtection();
    void castWeb();
    void castSummonsFlower();
    void castTransmuteFairy();
    void castTimeStop();
    void castLightning();

    ItemEntity* itemToBuy;

    float getBolPositionY();
    void incrementDivInterventions();

    int consumable[MAX_SLOT_CONSUMABLES];
    void acquireConsumable(enumItemType type);
    void reveal();

    float lastTeleportSave;
};

#endif // PLAYERSPRITE_H
