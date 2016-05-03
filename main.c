/*********************************************************************
 *
 *                  EE 4 Project - Code Template
 *
 *********************************************************************
 * Processor:       PIC18F2550
 * Compiler:        XC8 1.35+
 * Author:          Jeroen Van Aken
 * Updated:         17/02/2016
 ********************************************************************/

/** I N C L U D E S *************************************************/
#include "config.h"
/** D E F I N E S ***************************************************/

/** P R I V A T E   V A R I A B L E S *******************************/
static unsigned char seed_time; /* Value to seed */
static unsigned long long time_ms;

unsigned int counter;

//===----------------------------------------------------------------------===//
//  GLOBAL PUBLIC VARIABLES
//===----------------------------------------------------------------------===//

/* Global game variables */
unsigned char round;
bool game_ended;

/** P R I V A T E   P R O T O T Y P E S *****************************/
static void init(void);

char rl(char a)
{
    unsigned char temp = 0;
    
    temp = a;
    a = a >> 1;
    a |= (temp << 7) & 0x80;
    
    return a;
}

/********************************************************************/
/** P U B L I C   D E C L A R A T I O N S ***************************/
/********************************************************************
 * Function:        void main(void)
 * PreCondition:    None
 * Input:           None
 * Output:          None
 * Overview:        Main program entry point
 ******************************************s**************************/
void main(void) {
	init();						//initialize the system
    unsigned char byte = 0x00;
    
    //AUDIO_playSound(SOUND_SELECT);
    p1_tapping_fsm_init();
    feedback_fsm_init();
    
	while(timed_to_1ms()) {
        time_ms++;
        RAND_seed(seed_time++); /* Try to seed the RNG - seed_time can overflow, 
                                 * but we don't care #YOLO */
        
        /* Main game FSM */
        general_fsm();
        p1_tapping_fsm();
        feedback_fsm();
        
        if (time_ms % 2000) {
            FEEDBACK_giveFeedback(PLAYER_1);
        }
        
        if (!(seed_time % 128)) {
            
            PATTERN_setPattern(PLAYER_1, byte);
            PATTERN_setPattern(PLAYER_2, byte);
            
            LEDS_update();
            
            byte = (byte + 1) % 5;
        }
	}
}

/********************************************************************/
/** P R I V A T E   D E C L A R A T I O N S *************************/
/********************************************************************
 * Function:        void init(void)
 * PreCondition:    None
 * Input:           None
 * Output:          None
 * Overview:        Init is a centralize initialization routine.              
 *                  It will initialze all IO and variables, also the
 *                  hardware_init() for the hardware functions should
 *                  be called here.              
 ********************************************************************/
static void init(void) 
{
    hardware_init();
    
    general_fsm_init();
    mode3_fsm_init();
}

//EOF-------------------------------------------------------------------------
