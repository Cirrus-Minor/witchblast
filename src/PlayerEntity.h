#ifndef PLAYERSPRITE_H
#define PLAYERSPRITE_H

#include "BaseCreatureEntity.h"
#include "ItemEntity.h"
#include "Constants.h"

class FairyEntity;

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

     /*!
     *  \brief update the bounding box
     *
     *  Update the bounding box of the player.
     *  Used before testing collision.
     */
    virtual void calculateBB();

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
    void resestFireDirection();

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

    /*!
     *  \brief updates the equipment of the player
     *
     *  Updates the equipment of the player.
     *
     *  \param item : the equip item ID
     *  \param toggleEquipped : True if the item has to be equipped
     */
    void setEquiped(int item, bool toggleEquipped);

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
    virtual bool hurt(int damages, enumShotType hurtingType, int level);

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
     *  \brief called when the player get an item
     *
     *  Called when the player get an item.
     *  When the item is an equip item, he starts an "acquiring stance".
     *
     *  \param type : item ID
     */
    void acquireItem(enumItemType type);

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
     *  \brief pay some gold
     *
     *  Pay some gold. Usually in shops.
     *
     *  \param gold : the price to pay
     */
    void pay(int price);

    void setSprinting(bool sprinting);
    bool isSprinting();

    /** Player status enum
     *  The different player states.
     */
    enum playerStatusEnum
    {
      playerStatusPlaying,    /**< Player is playing "normally" */
      playerStatusEntering,   /**< Player is entering a not yet cleared room (walking is forced) */
      playerStatusAcquire,    /**< Player is under acquiring stance */
      playerStatusUnlocking,  /**< Player is under unlocking stance */
      playerStatusGoingUp,    /**< Player goes to next level */
      playerStatusDead        /**< Player RIP */
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
     *  Accessor on the current shot type of a slot.
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

    /*!
     *  \brief accessor on the fairies number
     */
    int getFairieNumber();

    /*!
     *  \brief accessor on a fairy
     *  \param n : index of the fairy in the vector
     */
    FairyEntity* getFairy(unsigned int n);

  protected:
    virtual void readCollidingEntity(CollidingSpriteEntity* entity);
    void generateBolt(float velx, float vely);

    virtual void collideMapRight();
    virtual void collideMapLeft();
    virtual void collideMapTop();
    virtual void collideMapBottom();

  private:
    int fireDamages;
    float fireVelocity;
    float fireDelay;
    float currentFireDelay;
    float boltLifeTime;
    int gold;

    bool canFirePlayer;
    playerStatusEnum playerStatus;
    float acquireDelay;
    enumItemType acquiredItem;
    bool sprinting;

    bool equip[NUMBER_EQUIP_ITEMS];


    float specialBoltTimer;
    enumShotType specialShots[SPECIAL_SHOT_SLOTS];
    unsigned int specialShotLevel[SPECIAL_SHOT_SLOTS];
    int specialShotIndex;
    bool needInitShotType;

    int collidingDirection;           /*!< Colliding direction (4, 8, 6, 2) to detect collision with closed doors */

    int firingDirection;

    std::vector<FairyEntity*> fairies;

    int spriteDx;
    void renderHead(sf::RenderTarget* app);
    void renderBody(sf::RenderTarget* app);
    void renderHands(sf::RenderTarget* app);
    void renderFeet(sf::RenderTarget* app);
    void renderStaff(sf::RenderTarget* app);

    /*!
     *  \brief init the current shot type.
     *
     *  Init the current shot type.
     *  Called when the player get a new shot, or after a switch.
     */
    void initShotType();
};

#endif // PLAYERSPRITE_H
