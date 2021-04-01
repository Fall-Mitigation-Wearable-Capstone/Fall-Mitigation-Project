<<<<<<< HEAD
// state_machine_v1.ino
// This is the state machine which will control the inflatable. All of the hardware interactions occur through this state machine.
// Author: Archisha Sinha

#include "inflation.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ALL VARIABLES FOR STATE MACHINE DECLARED BELOW
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Names of HSM superstates
=======
>>>>>>> 21b021ebe49b8db3730a4607af462cb4ac4e90a3
enum Superstate {
  CHECK_USABILITY,
  DETECT_MOVEMENT,
  INFLATE_WEARABLE
} super;

<<<<<<< HEAD
// Names of HSM substates
enum Substate {
  START,                // Beginning of superstate 1
  CHECK_FOR_USER,
  CHECK_BATTERY_LEVEL,
  LOW_BATTERY,
  CHECK_GAS_LEVEL,      
  LOW_GAS,              // End of superstate 1
  READ_IMU,             // Beginning of superstate 2
  IMU_ERROR,
  CALCULATE_EULERS,
  DETECT_FALLS,         // End of superstate 2
  INFLATE_TO_100,       // Beginning of superstate 3
=======
enum Substate {
  START,
  CHECK_FOR_USER,
  CHECK_BATTERY_LEVEL,
  LOW_BATTERY,
  CHECK_GAS_LEVEL,
  LOW_GAS,
  READ_IMU,
  IMU_ERROR,
  CALCULATE_EULERS,
  DETECT_FALLS,
  INFLATE_TO_100,
>>>>>>> 21b021ebe49b8db3730a4607af462cb4ac4e90a3
  FULLY_INFLATED,
  DEFLATE_BY_20,
  INFLATED_80,
  INFLATION_ERROR,
<<<<<<< HEAD
  DEFLATE_FULLY,
  FULLY_DEFLATED        // End of superstate 3
} sub;


char input; //Variable stores the reading from the serial monitor
char command; //Variable stores the command given by tester
char error; //Variable stores input from tester about if "no User" or "low Battery". Defaults to '\n'
int tester; 
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// BELOW IS THE MAIN CODE FOR THE STATE MACHINE
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Inflation inf(3);
// This is where all initial varibales are set before the main
void setup() {
=======
  DEFLATE_FULLY
} sub;

void setup() {
  // put your setup code here, to run once:
>>>>>>> 21b021ebe49b8db3730a4607af462cb4ac4e90a3
  Serial.begin(115200);
  super = CHECK_USABILITY;
  sub = START;
  Serial.println("Ready");
}

<<<<<<< HEAD
// This is the main
void loop() {
  delay(1000);
  switch (super) {
    // Superstate 1
    case CHECK_USABILITY:
      tester = inf.get_input(5);
      Serial.print("test: ");
      Serial.println(tester);      
      Serial.println("checking");
      check_usability();
      delay(1000);
      break;

    // Superstate 2
    case DETECT_MOVEMENT:
      Serial.println("detecting");
      detect_movement();
      delay(1000);
      break;

    // Superstate 3
    case INFLATE_WEARABLE:
      Serial.println("inflating");
      inflate_wearable();
      delay(1000);
=======
void loop() {
  // put your main code here, to run repeatedly:
  delay(1000);
  switch (super) {
    case CHECK_USABILITY:
      Serial.println("checking");
      checkUsability(1);
      delay(1000);
//      super = DETECT_MOVEMENT;
      break;
    case DETECT_MOVEMENT:
      Serial.println("detecting");
      detectMovement(2);
      delay(1000);
//      super = INFLATE_WEARABLE;
      break;
    case INFLATE_WEARABLE:
      Serial.println("inflating");
      inflateWearable(3);
      delay(1000);
//      super = CHECK_USABILITY;
>>>>>>> 21b021ebe49b8db3730a4607af462cb4ac4e90a3
      break;
  }
}

<<<<<<< HEAD
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ALL FUNCTIONS DEFINITIONS BELOW... FUNCTIONS IN THIS FILE ARE SPECIFIC TO THE STATE MACHINE AND STATE MACHINE TESTING
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Superstate 1
void check_usability(void) {
  switch (sub) {
    case START:
      Serial.println("checking Start");
      sub = CHECK_FOR_USER;
      break;

    case CHECK_FOR_USER:
      Serial.println("checking User");
      
      // For testing, get user input
      Serial.println("Is user ready (Type Y or N): ");
      wait_for_command();
      
      if (command == 'Y') {       // If touch sensor detects user
        sub = CHECK_BATTERY_LEVEL;
      } else {              
        sub = CHECK_FOR_USER;
      }
      break;

    case CHECK_BATTERY_LEVEL:
      Serial.println("checking Battery");

      // For testing, get user input
      Serial.println("Is battery ready (Type Y or N): ");
      wait_for_command();
      
      if (command == 'Y') {     // If battery is in usage range
        sub = CHECK_GAS_LEVEL;
      } else {
        sub = LOW_BATTERY;
      }
      break;

    case LOW_BATTERY:
      Serial.println("low battery (User takes off jacket)");

      // For testing, get user input
      Serial.println("Is battery charged (Type Y or N): ");
      wait_for_command();

      // Charge the battery here
      if (command == 'Y') {      // If battery is fully charged
        sub = START;
      } else {
        sub = LOW_BATTERY;
      }
      break;

    case CHECK_GAS_LEVEL:
      Serial.println("checking Gas");
      
      // For testing, get user input
      Serial.println("Is gas full (Type Y or N): ");
      wait_for_command();

      if (command == 'Y') {       // If gas level is ok
        super = DETECT_MOVEMENT;
        sub = READ_IMU;
      } else {
        sub = LOW_GAS;
      }
      break;

    case LOW_GAS:
      Serial.println("low gas (User takes off jacket)");
      
      // For testing, get user input
      Serial.println("Is gas refilled (Type Y or N): ");
      wait_for_command();

      // Change gas canister here
      if (command == 'Y') {     // If gas level is ok 
        sub = START;
      } else {
        sub = LOW_GAS;
      }
      break;
  }
}

// Superstate 2
void detect_movement(void) {
  switch (sub) {
    case READ_IMU:
      Serial.println("detecting Read");
      
      // For testing, get user input
      Serial.println("Is IMU initialized (Type Y or N): ");
      wait_for_command();

      if(error == 'U'){           // If user takes of jacket
        error = '\n';
        super = CHECK_USABILITY;
        sub = START;
      }
      else if(error == 'B'){      // If battery is low
        error = '\n';
        super = CHECK_USABILITY;
        sub = LOW_BATTERY;
      }
      else if(command == 'Y'){    // If data read correctly
        sub = CALCULATE_EULERS;  
      } else{
        sub = IMU_ERROR;
      }
      break;
      
    case IMU_ERROR:
      Serial.println("detecting IMU ERROR");
      Serial.println("STOPPING SYSTEM (Reset ESP to continue testing)");
      break;
      
    case CALCULATE_EULERS:
      Serial.println("detecting Eulers");
      delay(200);

      // For testing, get user input
      Serial.println("Is user/battery ok still (Type U or B): ");
      wait_for_command();
            
      if(error == 'U'){           // If user takes of jacket
        super = CHECK_USABILITY;
        sub = START;
      }
      else if(error == 'B'){      // If battery is low
        super = CHECK_USABILITY;
        sub = LOW_BATTERY;
      }
      else{
        sub = DETECT_FALLS;  
      }
      break;
      
    case DETECT_FALLS:
      Serial.println("detecting Falls");
      
      // For testing, get user input
      Serial.println("Is fall detected (Type Y or N): ");
      wait_for_command();

      if(error == 'U'){           // If user takes of jacket
        error = '\n';
        super = CHECK_USABILITY;
        sub = START;
      }
      else if(error == 'B'){      // If battery is low
        error = '\n';
        super = CHECK_USABILITY;
        sub = LOW_BATTERY;
      }
      else if(command == 'Y'){    // If fall is detected
        super = INFLATE_WEARABLE;
        sub = INFLATE_TO_100;  
      } else{
        sub = READ_IMU;
      }
      break;
  }
}

// Superstate 3
void inflate_wearable(void) {
  switch (sub) {
    case INFLATE_TO_100:
      Serial.println("inflating 100");
      delay(200);
      sub = FULLY_INFLATED;
      break;
      
    case FULLY_INFLATED:
      Serial.println("inflating Full");

      // For testing, get user input
      Serial.println("Is it fully inflated (Type Y or N): ");
      wait_for_command();

      if(command == 'Y'){     // If pressure is ok
        sub = DEFLATE_BY_20;  
      } else{
        sub = INFLATION_ERROR;
      }
      break;
      
    case DEFLATE_BY_20:
      Serial.println("deflating by 20");
      delay(200);
      sub = INFLATED_80;
      break;
      
    case INFLATED_80:
      Serial.println("inflating 80");

      // For testing, get user input
      Serial.println("Is it partially inflated (Type Y or N): ");
      wait_for_command();

      if(command == 'Y'){     // If pressure is ok
        sub = DEFLATE_FULLY;  
      } else{
        sub = INFLATION_ERROR;
      }
      break;
      
    case INFLATION_ERROR:
      Serial.println("inflating INFLATION ERROR");
      Serial.println("STOPPING SYSTEM (Reset ESP to continue testing)");
      break;
      
    case DEFLATE_FULLY:                   
      Serial.println("deflating Fully");
      delay(200);

      // For testing, get user input
      Serial.println("Is it fully inflated (Type Y or N): ");
      wait_for_command();
      
      if(command == 'Y'){       // If pressure is ok
        sub = FULLY_DEFLATED;  
      } else{
        sub = INFLATION_ERROR;
      }
      break;

    case FULLY_DEFLATED:
      Serial.println("deflating done");
      super = CHECK_USABILITY;
      sub = CHECK_GAS_LEVEL;
  }
}


//Function only used for testing
//Will make system wait for user input to proceed
//Used for testing SM without hardware
void wait_for_command(void) {
  while (Serial.available() < 1) {
    delay(100);
  }
  while (Serial.available() > 0) {
    input = Serial.read();
    if (input != '\n' && input != 'U' && input != 'B') {
      command = input;
    } else if (input == 'U') {
      error = input;
    } else if (input == 'B'){
      error = input;
    }
  }
=======
void checkUsability(int x) {
  switch(sub){
    case START:
      Serial.println("checking Start");
      super = CHECK_USABILITY;
      sub = CHECK_FOR_USER;
    break;
    case CHECK_FOR_USER:
      Serial.println("checking User");
      super = DETECT_MOVEMENT;
      sub = READ_IMU;
    break;
    case CHECK_BATTERY_LEVEL:
    break;
    case LOW_BATTERY:
    break;
    case CHECK_GAS_LEVEL:
    break;
    case LOW_GAS:
    break;
  }
//  return DETECT_MOVEMENT;
}

void detectMovement(int x) {
  switch(sub){
    case READ_IMU:
      Serial.println("detecting Read");
      super = DETECT_MOVEMENT;
      sub = CALCULATE_EULERS;
    break;
    case IMU_ERROR:
    break;
    case CALCULATE_EULERS:
      Serial.println("detecting Eulers");
      super = INFLATE_WEARABLE;
      sub = INFLATE_TO_100;
    break;
    case DETECT_FALLS:
    break;
  }
//  return INFLATE_WEARABLE;
}

void inflateWearable(int x) {
  switch(sub){
    case INFLATE_TO_100:
      Serial.println("inflating 100");
      super = INFLATE_WEARABLE;
      sub = FULLY_INFLATED;
    break;
    case FULLY_INFLATED:
      Serial.println("inflating Full");
      super = CHECK_USABILITY;
      sub = START;
    break;
    case DEFLATE_BY_20:
    break;
    case INFLATED_80:
    break;
    case INFLATION_ERROR:
    break;
    case DEFLATE_FULLY:
    break;
  }
//  return CHECK_USABILITY;
>>>>>>> 21b021ebe49b8db3730a4607af462cb4ac4e90a3
}
