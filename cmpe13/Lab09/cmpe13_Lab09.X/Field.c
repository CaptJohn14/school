// Name: Johnson Le
// Partner: Joe Loughboro

#include "Board.h"
#include "Field.h"
#include <stdio.h>
#include <stdbool.h>

// Function prototypes defined here:
static uint8_t CanAddBoat(Field *f, uint8_t row, uint8_t col, BoatDirection dir, BoatType type);
static void DoAddBoat(Field *f, uint8_t row, uint8_t col, BoatDirection dir, BoatType type);

/**
 * FieldInit() will fill the passed field array with the data specified in positionData. Also the
 * lives for each boat are filled according to the `BoatLives` enum.
 * @param f The field to initialize.
 * @param p The data to initialize the entire field to, should be a member of enum
 *                     FieldPosition.
 */
void FieldInit(Field *f, FieldPosition p)
{
    int x, y;
    for (x = 0; x < FIELD_ROWS; x++) {
        for (y = 0; y < FIELD_COLS; y++) {
            f->field[x][y] = p;
        }
    }
    f->smallBoatLives = FIELD_BOAT_LIVES_SMALL;
    f->mediumBoatLives = FIELD_BOAT_LIVES_MEDIUM;
    f->largeBoatLives = FIELD_POSITION_LARGE_BOAT;
    f->hugeBoatLives = FIELD_POSITION_HUGE_BOAT;
}

/**
 * Retrieves the value at the specified field position.
 * @param f The Field being referenced
 * @param row The row-component of the location to retrieve
 * @param col The column-component of the location to retrieve
 * @return
 */
FieldPosition FieldAt(const Field *f, uint8_t row, uint8_t col)
{
    return f->field[row][col]; //grabs field data and returns? think thats it.
}

/**
 * This function provides an interface for setting individual locations within a Field struct. This
 * is useful when FieldAddBoat() doesn't do exactly what you need. For example, if you'd like to use
 * FIELD_POSITION_CURSOR, this is the function to use.
 *
 * @param f The Field to modify.
 * @param row The row-component of the location to modify
 * @param col The column-component of the location to modify
 * @param p The new value of the field location
 * @return The old value at that field location
 */
FieldPosition FieldSetLocation(Field *f, uint8_t row, uint8_t col, FieldPosition p)
{
    //    f->field[row][col] = p;
    //    return FieldAt(f, row, col);
    //save old position, move and ret the old position.
    FieldPosition temp = f->field[row][col]; //old position
    f->field[row][col] = p; //new position.
    return temp; //return old posision.
}

/**
 * FieldAddBoat() places a single ship on the player's field based on arguments 2-5. Arguments 2, 3
 * represent the x, y coordinates of the pivot point of the ship.  Argument 4 represents the
 * direction of the ship, and argument 5 is the length of the ship being placed. All spaces that
 * the boat would occupy are checked to be clear before the field is modified so that if the boat
 * can fit in the desired position, the field is modified as SUCCESS is returned. Otherwise the
 * field is unmodified and STANDARD_ERROR is returned. There is no hard-coded limit to how many
 * times a boat can be added to a field within this function.
 *
 * So this is valid test code:
 * {
 *   Field myField;
 *   FieldInit(&myField,FIELD_POSITION_EMPTY);
 *   FieldAddBoat(&myField, 0, 0, FIELD_BOAT_DIRECTION_EAST, FIELD_BOAT_SMALL);
 *   FieldAddBoat(&myField, 1, 0, FIELD_BOAT_DIRECTION_EAST, FIELD_BOAT_MEDIUM);
 *   FieldAddBoat(&myField, 1, 0, FIELD_BOAT_DIRECTION_EAST, FIELD_BOAT_HUGE);
 *   FieldAddBoat(&myField, 0, 6, FIELD_BOAT_DIRECTION_SOUTH, FIELD_BOAT_SMALL);
 * }
 *
 * should result in a field like:
 *  _ _ _ _ _ _ _ _
 * [ 3 3 3       3 ]
 * [ 4 4 4 4     3 ]
 * [             3 ]
 *  . . . . . . . .
 *
 * @param f The field to grab data from.
 * @param row The row that the boat will start from, valid range is from 0 and to FIELD_ROWS - 1.
 * @param col The column that the boat will start from, valid range is from 0 and to FIELD_COLS - 1.
 * @param dir The direction that the boat will face once places, from the BoatDirection enum.
 * @param boatType The type of boat to place. Relies on the FIELD_POSITION_*_BOAT values from the
 * FieldPosition enum.
 * @return TRUE for success, FALSE for failure
 */
uint8_t FieldAddBoat(Field *f, uint8_t row, uint8_t col, BoatDirection dir, BoatType type)
{
    if (CanAddBoat(f, row, col, dir, type)) {
        DoAddBoat(f, row, col, dir, type);
        return TRUE;
    }
    return FALSE;
}

static uint8_t CanAddBoat(Field *f, uint8_t row, uint8_t col, BoatDirection dir, BoatType type)
{
    int i, rowIndexingSign, colIndexingSign;
    switch (dir) {
    case FIELD_BOAT_DIRECTION_NORTH:
        rowIndexingSign = -1;
        colIndexingSign = 0;
        break;
    case FIELD_BOAT_DIRECTION_EAST:
        rowIndexingSign = 0;
        colIndexingSign = 1;
        break;
    case FIELD_BOAT_DIRECTION_SOUTH:
        rowIndexingSign = 1;
        colIndexingSign = 0;
        break;
    case FIELD_BOAT_DIRECTION_WEST:
        rowIndexingSign = 0;
        colIndexingSign = -1;
        break;
    default:
        return FALSE; // Invalid direction.
    }
    BoatLives boatLives;
    switch (type) {
    case FIELD_BOAT_SMALL:
        boatLives = FIELD_BOAT_LIVES_SMALL;
        break;
    case FIELD_BOAT_MEDIUM:
        boatLives = FIELD_BOAT_LIVES_MEDIUM;
        break;
    case FIELD_BOAT_LARGE:
        boatLives = FIELD_BOAT_LIVES_LARGE;
        break;
    case FIELD_BOAT_HUGE:
        boatLives = FIELD_BOAT_LIVES_HUGE;
        break;
    default:
        return FALSE; // invalid BoatType.
    }
    for (i = 0; i < boatLives; i++) {
        if ((row + (i * rowIndexingSign) >= FIELD_ROWS) || (row + (i * rowIndexingSign) < 0) ||
                (col + (i * colIndexingSign) >= FIELD_COLS) || (col + (i * colIndexingSign) < 0)) {
            return FALSE; // If we're out of the array range, boat placement has failed.
        }
        if (f->field[row + (i * rowIndexingSign)][col + (i * colIndexingSign)] !=
                FIELD_POSITION_EMPTY) {
            return FALSE; // If there's something already there, boat placement has failed.
        }
    }
    return TRUE;
}

static void DoAddBoat(Field *f, uint8_t row, uint8_t col, BoatDirection dir, BoatType type)
{
    int i, rowIndexingSign, colIndexingSign;
    switch (dir) {
    case FIELD_BOAT_DIRECTION_NORTH:
        rowIndexingSign = -1;
        colIndexingSign = 0;
        break;
    case FIELD_BOAT_DIRECTION_EAST:
        rowIndexingSign = 0;
        colIndexingSign = 1;
        break;
    case FIELD_BOAT_DIRECTION_SOUTH:
        rowIndexingSign = 1;
        colIndexingSign = 0;
        break;
    case FIELD_BOAT_DIRECTION_WEST:
        rowIndexingSign = 0;
        colIndexingSign = -1;
        break;
    default:
        break; // Invalid direction ???
    }
    BoatLives boatLives;
    FieldPosition p;
    switch (type) {
    case FIELD_BOAT_SMALL:
        p = FIELD_POSITION_SMALL_BOAT;
        break;
    case FIELD_BOAT_MEDIUM:
        p = FIELD_POSITION_MEDIUM_BOAT;
        break;
    case FIELD_BOAT_LARGE:
        p = FIELD_POSITION_LARGE_BOAT;
        break;
    case FIELD_BOAT_HUGE:
        p = FIELD_POSITION_HUGE_BOAT;
        break;
    default:
        break; // invalid BoatType ???
    }
    for (i = 0; i < boatLives; i++) {
        f->field[row + (i * rowIndexingSign)][col + (i * colIndexingSign)] = p;
    }
    switch (type) {
    case FIELD_BOAT_SMALL:
        f->smallBoatLives += FIELD_BOAT_LIVES_SMALL;
        break;
    case FIELD_BOAT_MEDIUM:
        f->mediumBoatLives += FIELD_BOAT_LIVES_MEDIUM;
        break;
    case FIELD_BOAT_LARGE:
        f->largeBoatLives += FIELD_BOAT_LIVES_LARGE;
        break;
    case FIELD_BOAT_HUGE:
        f->hugeBoatLives += FIELD_BOAT_LIVES_HUGE;
        break;
    default:
        break; // invalid BoatType ???
    }
}

/**
 * This function registers an attack at the gData coordinates on the provided field. This means that
 * 'f' is updated with a FIELD_POSITION_HIT or FIELD_POSITION_MISS depending on what was at the
 * coordinates indicated in 'gData'. 'gData' is also updated with the proper HitStatus value
 * depending on what happened AND the value of that field position BEFORE it was attacked. Finally
 * this function also reduces the lives for any boat that was hit from this attack.
 * @param f The field to check against and update.
 * @param gData The coordinates that were guessed. The HIT result is stored in gData->hit as an
 *               output.
 * @return The data that was stored at the field position indicated by gData before this attack.
 */
FieldPosition FieldRegisterEnemyAttack(Field *f, GuessData *gData)
{
    //    switch (f->field[gData->row][gData->col]) {
    //
    //    }
    //is guess correct? have to match to boat to see if hit or miss.
    switch (FieldAt(f, gData->row, gData->col)) { //checks if maps. yes = a hit
    case FIELD_POSITION_SMALL_BOAT:
        f->smallBoatLives--;
        if (f->smallBoatLives == 0) {
            gData->hit = HIT_SUNK_SMALL_BOAT;
        } else {
            gData->hit = HIT_HIT;
        }
        FieldSetLocation(f, gData->row, gData->col, FIELD_POSITION_HIT);
        break;
    case FIELD_POSITION_MEDIUM_BOAT:
        f->mediumBoatLives--;
        if (f->mediumBoatLives == 0) {
            gData->hit = HIT_SUNK_MEDIUM_BOAT;
        } else {
            gData->hit = HIT_HIT;
        }
        break;
        FieldSetLocation(f, gData->row, gData->col, FIELD_POSITION_HIT);
    case FIELD_POSITION_LARGE_BOAT:
        f->largeBoatLives--;
        if (f->largeBoatLives == 0) {
            gData->hit = HIT_SUNK_LARGE_BOAT;
        } else {
            gData->hit = HIT_HIT;
        }
        FieldSetLocation(f, gData->row, gData->col, FIELD_POSITION_HIT);
        break;
    case FIELD_POSITION_HUGE_BOAT:
        f->hugeBoatLives--;
        if (f->hugeBoatLives == 0) {
            gData->hit = HIT_SUNK_HUGE_BOAT;
        } else {
            gData->hit = HIT_HIT;
        }
        FieldSetLocation(f, gData->row, gData->col, FIELD_POSITION_HIT);
        break;
    case FIELD_POSITION_EMPTY: //if not, then it miss.
    default:
        gData->hit = HIT_MISS;
        FieldSetLocation(f, gData->row, gData->col, FIELD_POSITION_MISS);
        break;
    }
    return FieldAt(f, gData->row, gData->col);

    //can condense: (if HIT..... FIELDPOSITIONHIT)
}

/**
 * This function updates the FieldState representing the opponent's game board with whether the
 * guess indicated within gData was a hit or not. If it was a hit, then the field is updated with a
 * FIELD_POSITION_HIT at that position. If it was a miss, display a FIELD_POSITION_EMPTY instead, as
 * it is now known that there was no boat there. The FieldState struct also contains data on how
 * many lives each ship has. Each hit only reports if it was a hit on any boat or if a specific boat
 * was sunk, this function also clears a boats lives if it detects that the hit was a
 * HIT_SUNK_*_BOAT.
 * @param f The field to grab data from.
 * @param gData The coordinates that were guessed along with their HitStatus.
 * @return The previous value of that coordinate position in the field before the hit/miss was
 * registered.
 */
FieldPosition FieldUpdateKnowledge(Field *f, const GuessData *gData)
{
    FieldPosition previousvalue = f->field[gData->row][gData->col]; //storing prev coordinate pos
    if (gData->hit == HIT_HIT) { //if hit
        FieldSetLocation(f, gData->row, gData->col, FIELD_POSITION_HIT); //update location as hit.
        switch (gData->hit) {
        case HIT_SUNK_SMALL_BOAT:
            f->smallBoatLives = 0;
            break;
        case HIT_SUNK_MEDIUM_BOAT:
            f->mediumBoatLives = 0;
            break;
        case HIT_SUNK_LARGE_BOAT:
            f->largeBoatLives = 0;
            break;
        case HIT_SUNK_HUGE_BOAT:
            f->hugeBoatLives = 0;
            break;
        }
    } else {
        FieldSetLocation(f, gData->row, gData->col, FIELD_POSITION_EMPTY);
    }
    return previousvalue;
}

/**
 * This function returns the alive states of all 4 boats as a 4-bit bitfield (stored as a uint8).
 * The boats are ordered from smallest to largest starting at the least-significant bit. So that:
 * 0b00001010 indicates that the small boat and large boat are sunk, while the medium and huge boat
 * are still alive. See the BoatStatus enum for the bit arrangement.
 * @param f The field to grab data from.
 * @return A 4-bit value with each bit corresponding to whether each ship is alive or not.
 */
uint8_t FieldGetBoatStates(const Field *f)
{
    uint8_t boatstate = 0x0; //will make zero. start at all alive?
    if (f->smallBoatLives) { //if alive so not zero
        boatstate = boatstate | FIELD_BOAT_STATUS_SMALL; //0b0000 or with 0b0001. so alive.
    }
    if (f->mediumBoatLives) {
        boatstate = boatstate | FIELD_BOAT_STATUS_MEDIUM;
    }
    if (f->largeBoatLives) {
        boatstate = boatstate | FIELD_BOAT_STATUS_LARGE;
    }
    if (f->hugeBoatLives) {
        boatstate = boatstate | FIELD_BOAT_STATUS_HUGE;
    }
    return boatstate;
}

