/* ************************************************************************** */
/* Fall Injury Mitigation Wearable Team
 * UCSC ECE Senior Capstone 2020-21
 
 * File name: projectSM_main.c
 
 * File description: This is the main loop for the wearable. This file houses 
 * the main state machine which controls the wearable. All of the subsystems are
 * combined through software in this state machine.
 
 * Author: Archisha Sinha
 */
/* ************************************************************************** */

/* ************************************************************************** */
/* Section: Included Files                                                    */
/* ************************************************************************** */


/* ************************************************************************** */
/* Section: Function Prototypes                                               */
/* ************************************************************************** */
void checkUsability(void);
void detectMovement(void);
void inflateWearable(void);

/* ************************************************************************** */
/* Section: File Scope or Global Data                                         */

/* ************************************************************************** */
enum Superstate {
    CHECK_USABILITY,
    DETECT_MOVEMENT,
    INFLATE_WEARABLE
};

enum Superstate super;

enum Substate {
    START,
    CHECK_FOR_USER,
    CHECK_BATTERY_LEVEL,
    LOW_BATTERY,
    READ_IMU,
    IMU_ERROR,
    DETECT_FALLS,
    INFLATE_TO_100,
    MAINTAIN_FULL_INFLATION,
    INFLATION_ERROR,
    DEFLATE_FULLY
};

enum Substate sub;

/* ************************************************************************** */
/* Section: Main Loop                                                         */

/* ************************************************************************** */
int main(void) {
    switch (super) {
        case CHECK_USABILITY:
            break;

        case DETECT_MOVEMENT:
            break;

        case INFLATE_WEARABLE:
            break;
    }
    return 1;
}


/* ************************************************************************** */
/* Section: Function Definitions                                              */

/* ************************************************************************** */
void checkUsability(void) {
    switch(sub) {
        case START:
//            Serial.println("checking Start");
//            sub = CHECK_FOR_USER;
            break;

        case CHECK_FOR_USER:
//            Serial.println("checking User");

//            if (checks.checkForUser() == SUCCESS) { // If touch sensor detects user
//                sub = CHECK_BATTERY_LEVEL;
//            } else {
//                sub = CHECK_FOR_USER;
//            }
            break;

        case CHECK_BATTERY_LEVEL:
//            Serial.println("checking Battery");

//            if (checks.getBatteryLevel() == SUCCESS) { // If battery is in usage range
//                super = DETECT_MOVEMENT;
//                sub = READ_IMU;
//            } else {
//                sub = LOW_BATTERY;
//            }
//            checks.setBatteryLights();
            break;

        case LOW_BATTERY:
//            Serial.println("low battery (User takes off jacket)");

//            if (checks.checkBatteryCharging() == SUCCESS) { // If battery is fully charged
//                sub = START;
//            } else {
//                sub = LOW_BATTERY;
//            }
            break;
    }
}
void detectMovement(void);
void inflateWearable(void);