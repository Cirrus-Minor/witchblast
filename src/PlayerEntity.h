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
    virtual void render(sf::RenderWindow* app);

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
     *  \param eq : True if the item has to be equipped
     */
    void setEquiped(int item, bool eq);

    /*!
     *  \brief updates the entering status of the player
     *
     *  Updates the entering status of the player.
     *  Used when the player is entering in a not yet cleared room.
     */
    void setEntering();

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
     *  \return : True if the player has been hurt
     */
    virtual bool hurt(int damages, enumBoltType hurtingType);

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

    /*!
     *  \brief accessor on the colliding direction
     *
     *  Accessor on the colliding direction.
     *
     *  \return : the colliding direction
     */
    int getCollidingDirection();

    /** Player status enum
     *  The different player states.
     */
    enum playerStatusEnum
    {
      playerStatusPlaying,    /**< Player is playing "normally" */
      playerStatusEntering,   /**< Player is entering a not yet cleared room (walking is forced) */
      playerStatusAcquire,    /**< Player is under acquiring stance */
      playerStatusUnlocking,  /**< Player is under unlocking stance */
      playerStatusDead        /**< Player RIP */
    };
  protected:
    void computePlayer();
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

    bool equip[NUMBER_EQUIP_ITEMS];
    enumBoltType boltType;
    float specialBoltTimer;

    int collidingDirection;           /*!< Colliding direction (4, 8, 6, 2) to detect collision with closed doors */

    int facingDirection;
    int firingDirection;

    FairyEntity* fairy;
};

#endif // PLAYERSPRITE_H
