// state_machine_v1.ino
// This is the state machine which will control the inflatable. All of the hardware interactions occur through this state machine.
// Author: Archisha Sinha

#include "inflation.h"
#include "checking.h"
#include "FallDetection.h"
#include "BluetoothSerial.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ALL VARIABLES FOR STATE MACHINE DECLARED BELOW
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Set up microcontroller to use bluetooth
#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

BluetoothSerial SerialBT;

// Names of HSM superstates
enum Superstate {
  CHECK_USABILITY,
  DETECT_MOVEMENT,
  INFLATE_WEARABLE
} super;


// Names of HSM substates
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
  DEFLATE_FULLY       // End of superstate 3
} sub;

#define IMU_ERROR_THRESHOLD 10

char input; //Variable stores the reading from the serial monitor
char command; //Variable stores the command given by tester
char error; //Variable stores input from tester about if "no User" or "low Battery". Defaults to '\n'
unsigned long startTime; //Variable stores the start time for all timers in the inflation process of the state machine
int imuErrorCount;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// BELOW IS THE MAIN CODE FOR THE STATE MACHINE
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Inflation inflation;
Checking checks;
FallDetection detection;

// This is where all initial varibales are set before the main
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  // Initialize bluetooth Serial
  SerialBT.begin("ESP32test"); //Bluetooth device name
  SerialBT.println("The device started, now you can pair it with bluetooth!");

  // Initialize the IMU
  // If IMU works, start SM at the beginning
  // If IMU doesn't work, SM jumps straight to an IMU error
  if(detection.initIMU() == ERROR){
    while (1){
      SerialBT.println("Unable to communicate with MPU-9250");
      SerialBT.println("Check connections, and try again.");
      SerialBT.println();
      delay(5000);
      super = DETECT_MOVEMENT;
      sub = IMU_ERROR;
    }
  }
  else{
    super = CHECK_USABILITY;
    sub = START;
    imuErrorCount = 0;
    SerialBT.println("Ready");
  }
}

// This is the main
void loop() {
  delay(1000);
  switch (super) {
    // Superstate 1
    case CHECK_USABILITY:
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
  }
}

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

      if (checks.checkForUser() == SUCCESS) {       // If touch sensor detects user
        sub = CHECK_BATTERY_LEVEL;
      } else {
        sub = CHECK_FOR_USER;
      }

      // For testing, get user input
      // Serial.println("Is user ready (Type Y or N): ");
      // wait_for_command();
      //
      // if (command == 'Y') {       // If touch sensor detects user
      //   sub = CHECK_BATTERY_LEVEL;
      // } else {
      //   sub = CHECK_FOR_USER;
      // }
      break;

    case CHECK_BATTERY_LEVEL:
      Serial.println("checking Battery");

      if (checks.getBatteryLevel() == SUCCESS) {     // If battery is in usage range
        super = DETECT_MOVEMENT;
        sub = READ_IMU;
      } else {
        sub = LOW_BATTERY;
      }
      checks.setBatteryLights();

      // For testing, get user input
      // Serial.println("Is battery ready (Type Y or N): ");
      // wait_for_command();
      //
      // if (command == 'Y') {     // If battery is in usage range
      //   super = DETECT_FALLS;
      //   sub = READ_IMU;
      // } else {
      //   sub = LOW_BATTERY;
      // }
      break;

    case LOW_BATTERY:
      Serial.println("low battery (User takes off jacket)");

      // // For testing, get user input
      // Serial.println("Is battery charged (Type Y or N): ");
      // wait_for_command();

      // Charge the battery here
      if (checks.checkBatteryCharging() == SUCCESS) {      // If battery is fully charged
        sub = START;
      } else {
        sub = LOW_BATTERY;
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
      // Serial.println("Is IMU initialized (Type Y or N): ");
      // wait_for_command();

      if(checks.checkForUser() == ERROR){           // If user takes of jacket
        super = CHECK_USABILITY;
        sub = START;
      }
      else if(checks.getBatteryLevel() == ERROR){      // If battery is low
        super = CHECK_USABILITY;
        sub = LOW_BATTERY;
      }
      else if(detection.getIMUData() == ERROR){
        imuErrorCount = imuErrorCount + 1;

        /*
         * IMU_ERROR_THRESHOLD reasoning: 
         * The IMU is sampled at 200Hz which is every 5ms. Therefore, the interrupt should be ready with data every 5ms
         * Based on literature, a fall algorthim needs to be sampled at least at 100Hz for any amount of accuracy (every 10ms)
         * If the interrupt is not ready for 10 calls, 5ms apart, data has not been ready for 50ms
         * This is more time than the minimum frequency, with 50ms intervals resulting in a frequency of 20Hz which is too slow to ensure accurate fall detection
         */
        if(imuErrorCount >= IMU_ERROR_THRESHOLD){
          sub = IMU_ERROR;  
        }
      }
      else{
        imuErrorCount = 0;
        sub = DETECT_FALLS;
      }
      break;

    case IMU_ERROR:
      Serial.println("detecting IMU ERROR");
      Serial.println("STOPPING SYSTEM (Reset ESP to continue testing)");
      break;

    case DETECT_FALLS:
      Serial.println("detecting Falls");

      // For testing, get user input
      // Serial.println("Is fall detected (Type Y or N): ");
      // wait_for_command();

      if(checks.checkForUser() == ERROR){           // If user takes of jacket
        super = CHECK_USABILITY;
        sub = START;
      } else if(checks.getBatteryLevel() == ERROR){      // If battery is low
          super = CHECK_USABILITY;
          sub = LOW_BATTERY;
      } else if(detection.detectFalls() != 0){    // If fall is detected
          super = INFLATE_WEARABLE;
          sub = INFLATE_TO_100;
      } else{
          sub = DETECT_FALLS;
          startTime = millis();
      }
      break;
  }
}


// Superstate 3
void inflate_wearable(void) {
  switch (sub) {
    case INFLATE_TO_100:
      Serial.println("inflating 100");
      inflation.fullInflate();
      
      if(millis() - startTime >= 80){
        sub = MAINTAIN_FULL_INFLATION;
        startTime = millis();
      } else{
          sub = INFLATE_TO_100;
      }
      break;

    case MAINTAIN_FULL_INFLATION:
      Serial.println("inflating Full");
      inflation.fullInflate();

      if(inflation.pressureCheck() == ERROR){     // If pressure is ok
        sub = INFLATION_ERROR;
      } else if(millis() - startTime >= 1000){
        sub = DEFLATE_FULLY; 
      } else{
        sub = MAINTAIN_FULL_INFLATION;
      }
      break;

    case INFLATION_ERROR:
      Serial.println("inflating INFLATION ERROR");
      Serial.println("STOPPING SYSTEM (Reset ESP to continue testing)");
      break;

    case DEFLATE_FULLY:
      Serial.println("deflating Fully");
      inflation.fullDeflate();

      // For testing, get user input
      super = DETECT_MOVEMENT;
      sub = READ_IMU;
      break;
  }
}


void printIMUData(void)
{
    //Print time
  SerialBT.print(millis());
  SerialBT.print(",\t");

  //Print raw data
  SerialBT.print(detection.gyroX);
  SerialBT.print(",\t");
  SerialBT.print(detection.gyroY);
  SerialBT.print(",\t");
  SerialBT.print(detection.gyroZ);
  SerialBT.print(",\t");
//  SerialBT.print(accelX);
//  SerialBT.print(",\t");
//  SerialBT.print(accelY);
//  SerialBT.print(",\t");
//  SerialBT.print(accelZ);
//  SerialBT.print(",\t");

  //Print Eulers
  SerialBT.print(detection.yaw);
  SerialBT.print(",\t");
  SerialBT.print(detection.pitch);
  SerialBT.print(",\t");
  SerialBT.print(detection.roll);
  SerialBT.print(",\t");
}



////Function only used for testing
////Will make system wait for user input to proceed
////Used for testing SM without hardware
//void wait_for_command(void) {
//  while (Serial.available() < 1) {
//    delay(100);
//  }
//  while (Serial.available() > 0) {
//    input = Serial.read();
//    if (input != '\n' && input != 'U' && input != 'B') {
//      command = input;
//    } else if (input == 'U') {
//      error = input;
//    } else if (input == 'B'){
//      error = input;
//    }
//  }
//}
