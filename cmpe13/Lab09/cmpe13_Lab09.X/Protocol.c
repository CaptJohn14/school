// Name: Joe Loughboro
// Partner: Johnson Le

#include "Protocol.h"
#include "BOARD.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ILLEGAL_HEX_VAL 127
#define MESSAGE_ID_LENGTH 3
#define COMMA_CHAR_LENGTH 1
#define NULL_CHAR_LENGTH 1
#define PARSABLE_MESSAGE_LENGTH (PROTOCOL_MAX_PAYLOAD_LEN - (MESSAGE_ID_LENGTH + COMMA_CHAR_LENGTH))

typedef enum {
    WAITING,
    RECORDING,
    FIRST_CHECKSUM_HALF,
    SECOND_CHECKSUM_HALF,
    NEWLINE
} DecodingState;

typedef struct {
    char sentence[PROTOCOL_MAX_MESSAGE_LEN];
    int index;
    DecodingState decodingState;
    uint8_t checksum;
} StateData;

// Array sizes need "+ 1" for '\0' at the end to make a valid C-string:
static char payloadData[PROTOCOL_MAX_PAYLOAD_LEN + NULL_CHAR_LENGTH];
static char dataToParse[PARSABLE_MESSAGE_LENGTH + NULL_CHAR_LENGTH];
static StateData stateData;
static uint8_t checksum;
static uint8_t myChecksum;
static int payloadLength;

static void ProtocolInit(void);
static uint8_t GenerateChecksum(char *string);
static uint8_t AsciiHexToUint8(char c); // 
static ProtocolParserStatus ParseMessage(const char *string,
        NegotiationData *nData,
        GuessData *gData);
static ProtocolParserStatus ParseMessageId(const char *string);
static uint8_t ParseHitMessage(GuessData *gData);
static uint8_t ParseCooMessage(GuessData *gData);
static uint8_t ParseChaMessage(NegotiationData *nData);
static uint8_t ParseDetMessage(NegotiationData *nData);

/**
 * Encodes the coordinate data for a guess into the string `message`. This string must be big
 * enough to contain all of the necessary data. The format is specified in PAYLOAD_TEMPLATE_COO,
 * which is then wrapped within the message as defined by MESSAGE_TEMPLATE. The final length of this
 * message is then returned. There is no failure mode for this function as there is no checking
 * for NULL pointers.
 * @param message The character array used for storing the output. Must be long enough to store the
 *                entire string, see PROTOCOL_MAX_MESSAGE_LEN.
 * @param data The data struct that holds the data to be encoded into `message`.
 * @return The length of the string stored into `message`.
 */
int ProtocolEncodeCooMessage(char *message, const GuessData *data)
{
    sprintf(payloadData, PAYLOAD_TEMPLATE_COO, data->row, data->col);
    checksum = GenerateChecksum(payloadData);
    return sprintf(message, MESSAGE_TEMPLATE, payloadData, checksum);
}

/**
 * Follows from ProtocolEncodeCooMessage above.
 */
int ProtocolEncodeHitMessage(char *message, const GuessData *data)
{
    sprintf(payloadData, PAYLOAD_TEMPLATE_HIT, data->row, data->col, data->hit);
    checksum = GenerateChecksum(payloadData);
    return sprintf(message, MESSAGE_TEMPLATE, payloadData, checksum);
}

/**
 * Follows from ProtocolEncodeCooMessage above.
 */
int ProtocolEncodeChaMessage(char *message, const NegotiationData *data)
{
    sprintf(payloadData, PAYLOAD_TEMPLATE_CHA, data->encryptedGuess, data->hash);
    checksum = GenerateChecksum(payloadData);
    return sprintf(message, MESSAGE_TEMPLATE, payloadData, checksum);
}

/**
 * Follows from ProtocolEncodeCooMessage above.
 */
int ProtocolEncodeDetMessage(char *message, const NegotiationData *data)
{
    sprintf(payloadData, PAYLOAD_TEMPLATE_DET, data->guess, data->encryptionKey);
    checksum = GenerateChecksum(payloadData);
    return sprintf(message, MESSAGE_TEMPLATE, payloadData, checksum);
}

/**
 * This function decodes a message into either the NegotiationData or GuessData structs depending
 * on what the type of message is. This function receives the message one byte at a time, where the
 * messages are in the format defined by MESSAGE_TEMPLATE, with payloads of the format defined by
 * the PAYLOAD_TEMPLATE_* macros. It returns the type of message that was decoded and also places
 * the decoded data into either the `nData` or `gData` structs depending on what the message held.
 * The onus is on the calling function to make sure the appropriate structs are available (blame the
 * lack of function overloading in C for this ugliness).
 *
 * PROTOCOL_PARSING_FAILURE is returned if there was an error of any kind (though this excludes
 * checking for NULL pointers), while
 * 
 * @param in The next character in the NMEA0183 message to be decoded.
 * @param nData A struct used for storing data if a message is decoded that stores NegotiationData.
 * @param gData A struct used for storing data if a message is decoded that stores GuessData.
 * @return A value from the UnpackageDataEnum enum.
 */
ProtocolParserStatus ProtocolDecode(char in, NegotiationData *nData, GuessData *gData)
{
    static uint8_t hexValue;
    static uint8_t isFirstPass = TRUE;
    if (isFirstPass) {
        isFirstPass = FALSE;
        ProtocolInit();
    }
    switch (stateData.decodingState) {
    case WAITING:
        if (in == '$') {
            stateData.index = 0;
            stateData.decodingState = RECORDING;
            printf("$\n");
            return PROTOCOL_PARSING_GOOD;
        }
        return PROTOCOL_WAITING;
    case RECORDING:
        if (in != '*') {
            stateData.sentence[stateData.index] = in;
            stateData.index++;
        } else {
            stateData.sentence[stateData.index] = '\0';
            stateData.decodingState = FIRST_CHECKSUM_HALF;
            printf("%s*\n", stateData.sentence);
        }
        return PROTOCOL_PARSING_GOOD;
    case FIRST_CHECKSUM_HALF:
        hexValue = AsciiHexToUint8(in);
        if (hexValue == ILLEGAL_HEX_VAL) {
            stateData.decodingState = WAITING;
            return PROTOCOL_PARSING_FAILURE;
        }
        stateData.checksum = hexValue << 4;
        stateData.decodingState = SECOND_CHECKSUM_HALF;
        return PROTOCOL_PARSING_GOOD;
    case SECOND_CHECKSUM_HALF:
        hexValue = AsciiHexToUint8(in);
        if (hexValue == ILLEGAL_HEX_VAL) {
            stateData.decodingState = WAITING;
            return PROTOCOL_PARSING_FAILURE;
        }
        stateData.checksum += hexValue;
        myChecksum = GenerateChecksum(stateData.sentence);
        printf("message checksum: %x\ncomputed checksum: %x\n", stateData.checksum, myChecksum);
        if (myChecksum != stateData.checksum) {
            stateData.decodingState = WAITING;
            return PROTOCOL_PARSING_FAILURE;
        }
        stateData.decodingState = NEWLINE;
        return PROTOCOL_PARSING_GOOD;
    case NEWLINE:
        stateData.decodingState = WAITING;
        if (in == '\n') {
            return ParseMessage(stateData.sentence, nData, gData);
        }
        return PROTOCOL_PARSING_FAILURE;
    default:
        stateData.decodingState = WAITING;
        return PROTOCOL_PARSING_FAILURE;
    }
}

/**
 * This function generates all of the data necessary for the negotiation process used to determine
 * the player that goes first. It relies on the pseudo-random functionality built into the standard
 * library. The output is stored in the passed NegotiationData struct. The negotiation data is
 * generated by creating two random 16-bit numbers, one for the actual guess and another for an
 * encryptionKey used for encrypting the data. The 'encryptedGuess' is generated with an
 * XOR(guess, encryptionKey). The hash is simply an 8-bit value that is the XOR() of all of the
 * bytes making up both the guess and the encryptionKey. There is no checking for NULL pointers
 * within this function.
 * @param data The struct used for both input and output of negotiation data.
 */
void ProtocolGenerateNegotiationData(NegotiationData *data)
{
    uint32_t mask = 0x000000FF;
    uint32_t randNum = rand();
    data->guess = randNum >> 16; // Left half is the guess.
    data->encryptionKey = (randNum << 16) >> 16; // Right half is the key.
    data->encryptedGuess = data->guess ^ data->encryptionKey;
    data->hash = 0;
    int i;
    for (i = 0; i < 4; i++) {
        data->hash ^= (randNum >> (i * 8)) & mask; // XOR is commutative so order doesn't matter.
    }
    randNum = 0; // So the enemy can't steal our secrets by looking where we stored randNum!
}

/**
 * Validates that the negotiation data within 'data' is correct according to the algorithm given in
 * GenerateNegotitateData(). Used for verifying another agent's supplied negotiation data. There is
 * no checking for NULL pointers within this function. Returns TRUE if the NegotiationData struct
 * is valid or FALSE on failure.
 * @param data A filled NegotiationData struct that will be validated.
 * @return TRUE if the NegotiationData struct is consistent and FALSE otherwise.
 */
uint8_t ProtocolValidateNegotiationData(const NegotiationData *data)
{
    // Encryption should work both ways, so we check it both ways:
    uint32_t result;
    result = data->encryptedGuess ^ data->encryptionKey;
    if (result != data->guess) {
        return FALSE;
    }
    result = data->guess ^ data->encryptionKey;
    if (result != data->encryptedGuess) {
        return FALSE;
    }
    // Compute the hash in a somewhat different way from ProtocolGenerate to double-check myself:
    result = 0;
    result ^= data->encryptionKey & 0x00FF;
    result ^= (data->encryptionKey & 0xFF00) >> 8;
    result ^= data->guess & 0x00FF;
    result ^= (data->guess & 0xFF00) >> 8;
    if (result != data->hash) {
        return FALSE;
    }
    return TRUE;
}

/**
 * This function returns a TurnOrder enum type representing which agent has won precedence for going
 * first. The value retur ies on the XOR() of the 'encryptionKey' used by both agents. The least-
 * significant bit of XOR(myData.encryptionKey, oppData.encryptionKey) is checked so that if it's a
 * 1 the player with the largest 'guess' goes first otherwise if it's a 0, the agent with the
 * smallest 'guess' goes first. The return value of TURN_ORDER_START indicates that 'myData' won,
 * TURN_ORDER_DEFER indicates that 'oppData' won, otherwise a tie is indicated with TURN_ORDER_TIE.
 * There is no checking for NULL pointers within this function.
 * @param myData The negotiation data representing the current agent.
 * @param oppData The negotiation data representing the opposing agent.
 * @return A value from the TurnOrdering enum representing which agent should go first.
 */
TurnOrder ProtocolGetTurnOrder(const NegotiationData *myData, const NegotiationData *oppData)
{
    uint8_t largerGoesFirst = (myData->guess & 1) ^ (oppData->guess & 1);
    if (largerGoesFirst) {
        if (myData->guess > oppData->guess) {
            return TURN_ORDER_START;
        }
        if (myData->guess < oppData->guess) {
            return TURN_ORDER_DEFER;
        }
    } else {
        if (myData->guess < oppData->guess) {
            return TURN_ORDER_START;
        }
        if (myData->guess > oppData->guess) {
            return TURN_ORDER_DEFER;
        }
    }
    return TURN_ORDER_TIE;
}

static void ProtocolInit(void)
{
    stateData.decodingState = WAITING;
}

static uint8_t GenerateChecksum(char *string)
{
    int i;
    uint8_t checksum = 0;
    for (i = 0; string[i] != '\0'; i++) {
        checksum ^= string[i];
    }
    return checksum;
}

static uint8_t AsciiHexToUint8(char c)
{
    switch (c) {
    case '0' ... '9':
        return ((uint8_t) c) - 48;
    case 'A'...'F':
        return ((uint8_t) c) - 55;
    case 'a'...'f':
        return ((uint8_t) c) - 87;
    default:
        return ILLEGAL_HEX_VAL;
    }
}

static ProtocolParserStatus ParseMessage(const char *string,
        NegotiationData *nData,
        GuessData *gData)
{
    switch (ParseMessageId(string)) {
    case PROTOCOL_PARSED_COO_MESSAGE:
        strcpy(dataToParse, &string[MESSAGE_ID_LENGTH + COMMA_CHAR_LENGTH]);
        if (ParseCooMessage(gData)) {
            return PROTOCOL_PARSED_COO_MESSAGE; // Return ID upon success, otherwise return FAILURE.
        }
        printf("Error parsing COO message.\n");
        break;
    case PROTOCOL_PARSED_HIT_MESSAGE:
        strcpy(dataToParse, &string[MESSAGE_ID_LENGTH + COMMA_CHAR_LENGTH]);
        if (ParseHitMessage(gData)) {
            return PROTOCOL_PARSED_HIT_MESSAGE;
        }
        printf("Error parsing HIT message.\n");
        break;
    case PROTOCOL_PARSED_CHA_MESSAGE:
        strcpy(dataToParse, &string[MESSAGE_ID_LENGTH + COMMA_CHAR_LENGTH]);
        if (ParseChaMessage(nData)) {
            return PROTOCOL_PARSED_CHA_MESSAGE;
        }
        printf("Error parsing CHA message.\n");
        break;
    case PROTOCOL_PARSED_DET_MESSAGE:
        strcpy(dataToParse, &string[MESSAGE_ID_LENGTH + COMMA_CHAR_LENGTH]);
        if (ParseDetMessage(nData)) {
            return PROTOCOL_PARSED_DET_MESSAGE;
        }
        printf("Error parsing DET message.\n");
        break;
    default:
        break; // Can't do anything if we didn't get one of those 4.
    }
    return PROTOCOL_PARSING_FAILURE;
}

static ProtocolParserStatus ParseMessageId(const char *string)
{
    if (string[0] == 'C' && string[1] == 'O' && string[2] == 'O') {
        return PROTOCOL_PARSED_COO_MESSAGE;
    }
    if (string[0] == 'H' && string[1] == 'I' && string[2] == 'T') {
        return PROTOCOL_PARSED_HIT_MESSAGE;
    }
    if (string[0] == 'C' && string[1] == 'H' && string[2] == 'A') {
        return PROTOCOL_PARSED_CHA_MESSAGE;
    }
    if (string[0] == 'D' && string[1] == 'E' && string[2] == 'T') {
        return PROTOCOL_PARSED_DET_MESSAGE;
    }
    return PROTOCOL_PARSING_FAILURE;
}

static uint8_t ParseCooMessage(GuessData *gData)
{
    char *token;
    token = strtok(dataToParse, ",");
    if (token == NULL) {
        return FALSE;
    }
    gData->row = atoi(token);
    token = strtok(NULL, ",");
    if (token == NULL) {
        return FALSE;
    }
    gData->col = atoi(token);
    token = strtok(NULL, ",");
    return (token == NULL);
}

static uint8_t ParseHitMessage(GuessData *gData)
{
    char *token;
    token = strtok(dataToParse, ",");
    if (token == NULL) {
        return FALSE;
    }
    gData->row = atoi(token);
    token = strtok(NULL, ",");
    if (token == NULL) {
        return FALSE;
    }
    gData->col = atoi(token);
    token = strtok(NULL, ",");
    if (token == NULL) {
        return FALSE;
    }
    gData->hit = atoi(token);
    token = strtok(NULL, ",");
    return (token == NULL);
}

static uint8_t ParseChaMessage(NegotiationData *nData)
{
    char *token;
    token = strtok(dataToParse, ",");
    if (token == NULL) {
        return FALSE;
    }
    nData->encryptedGuess = atoi(token);
    token = strtok(NULL, ",");
    if (token == NULL) {
        return FALSE;
    }
    nData->hash = atoi(token);
    token = strtok(NULL, ",");
    return (token == NULL);
}

static uint8_t ParseDetMessage(NegotiationData *nData)
{
    char *token;
    token = strtok(dataToParse, ",");
    if (token == NULL) {
        return FALSE;
    }
    nData->guess = atoi(token);
    token = strtok(NULL, ",");
    if (token == NULL) {
        return FALSE;
    }
    nData->encryptionKey = atoi(token);
    token = strtok(NULL, ",");
    return (token == NULL);
}