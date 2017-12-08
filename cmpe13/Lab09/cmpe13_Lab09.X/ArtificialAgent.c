
#include "Agent.h"
#include "Oled.h"
#include "OledDriver.h"
#include "BOARD.h"
#include "Field.h"
#include "Protocol.h"
#include "FieldOled.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

//#define RAND_MAX	0x7fffffff
//  %50, will give u range from 0-49.
#define OUTPUT_BUFFER_LENGTH 256
#define BOAT_DIRECTIONS 4

static AgentState agentState;
static Field myField, opField;
static NegotiationData myNegData, opNegData;
static GuessData myGuessData, opGuessData;
static char inBuffer[PROTOCOL_MAX_MESSAGE_LEN];
static FieldOledTurn turn;

// Function prototypes:
static AgentEvent ParseInputChar(char c);
static void ClearInputBuffer(void);
static void ClearOutputBuffer(char *outBuffer);
static void GenerateGuess(void);

/**
 * The Init() function for an Agent sets up everything necessary for an agent before the game
 * starts. This can include things like initialization of the field, placement of the boats,
 * etc. The agent can assume that stdlib's rand() function has been seeded properly in order to
 * use it safely within.
 */
void AgentInit(void)
{
    agentState = AGENT_STATE_GENERATE_NEG_DATA;
    //what is needed: placement of the boats means need location, type of boats.
    FieldInit(&myField, FIELD_POSITION_EMPTY); //p1 field is empty, p2 is technically unknown to p1
    FieldInit(&opField, FIELD_POSITION_UNKNOWN); //don't know the enemy's field.
    // Each of these while loops will continue to attempt to add a boat until it has success:
    while (FieldAddBoat(&myField, rand() % FIELD_ROWS, rand() % FIELD_COLS,
            rand() % BOAT_DIRECTIONS, FIELD_BOAT_SMALL) == FALSE);
    while (FieldAddBoat(&myField, rand() % FIELD_ROWS, rand() % FIELD_COLS,
            rand() % BOAT_DIRECTIONS, FIELD_BOAT_MEDIUM) == FALSE);
    while (FieldAddBoat(&myField, rand() % FIELD_ROWS, rand() % FIELD_COLS,
            rand() % BOAT_DIRECTIONS, FIELD_BOAT_LARGE) == FALSE);
    while (FieldAddBoat(&myField, rand() % FIELD_ROWS, rand() % FIELD_COLS,
            rand() % BOAT_DIRECTIONS, FIELD_BOAT_HUGE) == FALSE);
    turn = FIELD_OLED_TURN_NONE;
}

/**
 * The Run() function for an Agent takes in a single character. It then waits until enough
 * data is read that it can decode it as a full sentence via the Protocol interface. This data
 * is processed with any output returned via 'outBuffer', which is guaranteed to be 255
 * characters in length to allow for any valid NMEA0183 messages. The return value should be
 * the number of characters stored into 'outBuffer': so a 0 is both a perfectly valid output and
 * means a successful run.
 * @param in The next character in the incoming message stream.
 * @param outBuffer A string that should be transmit to the other agent. NULL if there is no
 *                  data.
 * @return The length of the string pointed to by outBuffer (excludes \0 character).
 */
int AgentRun(char in, char *outBuffer)
{
    static AgentEvent agentEvent;
    agentEvent = ParseInputChar(in);
    if (agentEvent == AGENT_EVENT_MESSAGE_PARSING_FAILED) {
        OledClear(OLED_COLOR_BLACK);
        OledDrawString(AGENT_ERROR_STRING_PARSING);
        OledUpdate();
        agentState = AGENT_STATE_INVALID;
    }
    ClearOutputBuffer(outBuffer);
    switch (agentState) {
    case AGENT_STATE_GENERATE_NEG_DATA:
        ProtocolGenerateNegotiationData(&myNegData);
        agentState = AGENT_STATE_SEND_CHALLENGE_DATA;
        return ProtocolEncodeChaMessage(outBuffer, &myNegData);
    case AGENT_STATE_SEND_CHALLENGE_DATA:
        if (agentEvent == AGENT_EVENT_RECEIVED_CHA_MESSAGE){
            return ProtocolEncodeDetMessage(outBuffer, &myNegData);
        }
        return 0;
    case AGENT_STATE_DETERMINE_TURN_ORDER:
        if (agentEvent == AGENT_EVENT_RECEIVED_DET_MESSAGE){
            if (ProtocolValidateNegotiationData(&opNegData)) {
                TurnOrder order = ProtocolGetTurnOrder(&myNegData, &opNegData);
                if (order == TURN_ORDER_START) {
                    turn = FIELD_OLED_TURN_MINE;
                    FieldOledDrawScreen(&myField, &opField, turn);
                    agentState = AGENT_STATE_SEND_GUESS;
                } else if (order == TURN_ORDER_DEFER) {
                    turn = FIELD_OLED_TURN_THEIRS;
                    FieldOledDrawScreen(&myField, &opField, turn);
                    agentState = AGENT_STATE_WAIT_FOR_GUESS;
                } else {
                    OledClear(OLED_COLOR_BLACK);
                    OledDrawString(AGENT_ERROR_STRING_ORDERING);
                    OledUpdate();
                    agentState = AGENT_STATE_INVALID;
                }
            } else {
                OledClear(OLED_COLOR_BLACK);
                OledDrawString(AGENT_ERROR_STRING_NEG_DATA);
                OledUpdate();
                agentState = AGENT_STATE_INVALID;
            }
        }
        return 0;
    case AGENT_STATE_SEND_GUESS:
        GenerateGuess();
        agentState = AGENT_STATE_WAIT_FOR_HIT;
        return ProtocolEncodeCooMessage(outBuffer, &myGuessData);
    case AGENT_STATE_WAIT_FOR_HIT:
        if (agentEvent == AGENT_EVENT_RECEIVED_HIT_MESSAGE) {
            if (FieldGetBoatStates(&opField)) { // If enemy boats left
                FieldUpdateKnowledge(&opField, &opGuessData);
                turn = FIELD_OLED_TURN_THEIRS;
                agentState = AGENT_STATE_WAIT_FOR_GUESS;
            } else {
                turn = FIELD_OLED_TURN_NONE;
                agentState = AGENT_STATE_WON; 
            }
            FieldOledDrawScreen(&myField, &opField, turn);
        }
        return 0;
    case AGENT_STATE_WAIT_FOR_GUESS:
        if (agentEvent == AGENT_EVENT_RECEIVED_COO_MESSAGE) {
            FieldRegisterEnemyAttack(&myField, &opGuessData);
            if (FieldGetBoatStates(&myField)) {
                turn = FIELD_OLED_TURN_MINE;
                agentState = AGENT_STATE_SEND_GUESS;
            } else {
                turn = FIELD_OLED_TURN_NONE;
                agentState = AGENT_STATE_LOST;
            }
            FieldOledDrawScreen(&myField, &opField, turn);
            return ProtocolEncodeCooMessage(outBuffer, &opGuessData);
        }
        return 0;
    case AGENT_STATE_LOST: //blank
        OledClear(OLED_COLOR_BLACK);
        OledDrawString("You LOSE!\n");
        OledUpdate();
        FieldOledDrawScreen(&myField, &opField, turn);
        break;
    case AGENT_STATE_WON: //blank
        OledClear(OLED_COLOR_BLACK);
        OledDrawString("You WIN!\n");
        OledUpdate();
        FieldOledDrawScreen(&myField, &opField, turn);
        break;
    case AGENT_STATE_INVALID:
    default:
        break;
    }
    return 0;
}

/**
 * StateCheck() returns a 4-bit number indicating the status of that agent's ships. The smallest
 * ship, the 3-length one, is indicated by the 0th bit, the medium-length ship (4 tiles) is the
 * 1st bit, etc. until the 3rd bit is the biggest (6-tile) ship. This function is used within
 * main() to update the LEDs displaying each agents' ship status. This function is similar to
 * Field::FieldGetBoatStates().
 * @return A bitfield indicating the sunk/unsunk status of each ship under this agent's control.
 *
 * @see Field.h:FieldGetBoatStates()
 * @see Field.h:BoatStatus
 */
uint8_t AgentGetStatus(void)
{
    return FieldGetBoatStates(&myField);
}

/**
 * This function returns the same data as `AgentCheckState()`, but for the enemy agent.
 * @return A bitfield indicating the sunk/unsunk status of each ship under the enemy agent's
 *         control.
 *
 * @see Field.h:FieldGetBoatStates()
 * @see Field.h:BoatStatus
 */
uint8_t AgentGetEnemyStatus(void)
{
    return FieldGetBoatStates(&opField);
}

static AgentEvent ParseInputChar(char in)
{
    static ProtocolParserStatus parserStatus = PROTOCOL_WAITING;
    static int inBufferIndex;
    if (in != '\0') {
        switch (parserStatus) {
        case PROTOCOL_WAITING:
            if (in == '$') {
                inBufferIndex = 0;
                inBuffer[inBufferIndex] = in;
                inBufferIndex++;
                parserStatus = PROTOCOL_PARSING_GOOD;
                return AGENT_EVENT_NONE;
            }
            parserStatus = PROTOCOL_PARSING_FAILURE;
            break;
        case PROTOCOL_PARSING_GOOD:
            if (inBufferIndex < (PROTOCOL_MAX_MESSAGE_LEN - 1)) {
                if (in != '\n') {
                    parserStatus = PROTOCOL_PARSING_FAILURE;
                } else {
                    inBuffer[inBufferIndex] = in;
                    for (inBufferIndex = 0; parserStatus < 2; inBufferIndex++) {
                        parserStatus = ProtocolDecode(inBuffer[inBufferIndex], &opNegData, &opGuessData);
                        if (parserStatus == PROTOCOL_PARSING_FAILURE) {
                            return AGENT_EVENT_MESSAGE_PARSING_FAILED;
                        }
                    }
                    return ParseInputChar('\n'); // parser status is set, now return the correct event.
                }
            } else {
                inBuffer[inBufferIndex] = in;
                inBufferIndex++;
                return AGENT_EVENT_NONE;
            }
            break;
        case PROTOCOL_PARSED_COO_MESSAGE:
            ClearInputBuffer();
            parserStatus = PROTOCOL_WAITING;
            return AGENT_EVENT_RECEIVED_COO_MESSAGE;
        case PROTOCOL_PARSED_HIT_MESSAGE:
            ClearInputBuffer();
            parserStatus = PROTOCOL_WAITING;
            return AGENT_EVENT_RECEIVED_HIT_MESSAGE;
        case PROTOCOL_PARSED_CHA_MESSAGE:
            ClearInputBuffer();
            parserStatus = PROTOCOL_WAITING;
            return AGENT_EVENT_RECEIVED_CHA_MESSAGE;
        case PROTOCOL_PARSED_DET_MESSAGE:
            ClearInputBuffer();
            parserStatus = PROTOCOL_WAITING;
            return AGENT_EVENT_RECEIVED_DET_MESSAGE;
        case PROTOCOL_PARSING_FAILURE:
        default:
            return AGENT_EVENT_MESSAGE_PARSING_FAILED;
        }
        return AGENT_EVENT_MESSAGE_PARSING_FAILED;
    }
    return AGENT_EVENT_NONE;
}

static void ClearInputBuffer(void) {
    int i;
    for (i = 0; i < PROTOCOL_MAX_MESSAGE_LEN; i++){
        inBuffer[i] = '\0';
    }
}

static void ClearOutputBuffer(char *outBuffer) {
    int i;
    for (i = 0; i < OUTPUT_BUFFER_LENGTH; i++){
        outBuffer[i] = '\0';
    }
}
static void GenerateGuess(void) {
    //can call like GuessAttack(rand() % FIELD_COLS, rand() % FIELD_ROWS)
    myGuessData.row = rand() % FIELD_ROWS;
    myGuessData.col = rand() % FIELD_COLS;
    
    if(opField.field[myGuessData.row][myGuessData.col] != FIELD_POSITION_UNKNOWN){
        GenerateGuess();
    }
}