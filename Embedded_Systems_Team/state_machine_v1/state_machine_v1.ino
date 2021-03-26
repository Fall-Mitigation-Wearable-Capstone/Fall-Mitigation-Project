enum Superstate {
  CHECK_USABILITY,
  DETECT_MOVEMENT,
  INFLATE_WEARABLE
} super;

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
  FULLY_INFLATED,
  DEFLATE_BY_20,
  INFLATED_80,
  INFLATION_ERROR,
  DEFLATE_FULLY
} sub;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  super = CHECK_USABILITY;
  sub = START;
  Serial.println("Ready");
}

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
      break;
  }
}

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
}
