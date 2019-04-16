
#ifndef Sub_ToIFZ_H 
#define Sub_ToIFZ_H 


/*==============================================================================
 * Libraries
 =============================================================================*/
#include "ES_Configure.h"   // defines ES_Event, INIT_EVENT, ENTRY_EVENT, and EXIT_EVENT

/*******************************************************************************
 * PUBLIC FUNCTION PROTOTYPES                                                  *
 ******************************************************************************/

uint8_t InitSubToIFZ(void);

ES_Event RunSubToIFZ(ES_Event ThisEvent);

#endif /* SUB_HSM_Template_H */

