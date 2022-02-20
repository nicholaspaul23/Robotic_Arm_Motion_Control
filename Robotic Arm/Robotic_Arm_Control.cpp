#include "Wire.h" // This library allows you to communicate with I2C devices.

const int MPU_ADDR = 0x68; // I2C address of the MPU-6050. If AD0 pin is set to HIGH, the I2C address will be 0x69.
int16_t accelerometer_x, accelerometer_y, accelerometer_z; // variables for accelerometer raw data
int16_t gyro_x, gyro_y, gyro_z; // variables for gyro raw data
int16_t temperature; // variables for temperature data
char tmp_str[7]; // temporary variable used in convert function

const int lifterH1 = 4; //input from motor 1 of Hbridge
const int lifterH2 = 5; //lift arm up and down
const int gripperH1 = 6; //open and close robot hand
const int gripperH2 = 7;
const int turnerH1 = 8; //turn arm left and right
const int turnerH2 = 9;
const int reacherH1 = 10; //extend arm forward and back
const int reacherH2 = 11;
const int fsr = 0; //force sensitive resistor to control gripper
int fsrReading;
bool gripped = false; //state variable to tell if we have initiated gripper via fsr

char* convert_int16_to_str(int16_t i) { // converts int16 to string. Moreover, resulting strings will have the same length in the debug monitor.
  sprintf(tmp_str, "%6d", i);
  return tmp_str;
}


void setup() {
  Serial.begin(9600);

  pinMode(gripperH1, OUTPUT);
  pinMode(gripperH2, OUTPUT);
  pinMode(lifterH1, OUTPUT);
  pinMode(lifterH2, OUTPUT);
  pinMode(turnerH1, OUTPUT);
  pinMode(turnerH2, OUTPUT);
  pinMode(reacherH1, OUTPUT);
  pinMode(reacherH2, OUTPUT);

  digitalWrite(gripperH1, LOW);
  digitalWrite(gripperH2, LOW);
  digitalWrite(lifterH1, LOW);
  digitalWrite(lifterH2, LOW);
  digitalWrite(turnerH1, LOW);
  digitalWrite(turnerH2, LOW);
  digitalWrite(reacherH1, LOW);
  digitalWrite(reacherH2, LOW);
  
  Wire.begin();
  Wire.beginTransmission(MPU_ADDR); // Begins a transmission to the I2C slave (GY-521 board)
  Wire.write(0x6B); // PWR_MGMT_1 register
  Wire.write(0); // set to zero (wakes up the MPU-6050)
  Wire.endTransmission(true);
}
void loop() {
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x3B); // starting with register 0x3B (ACCEL_XOUT_H) [MPU-6000 and MPU-6050 Register Map and Descriptions Revision 4.2, p.40]
  Wire.endTransmission(false); // the parameter indicates that the Arduino will send a restart. As a result, the connection is kept active.
  Wire.requestFrom(MPU_ADDR, 7*2, true); // request a total of 7*2=14 registers
  
  // "Wire.read()<<8 | Wire.read();" means two registers are read and stored in the same variable
  accelerometer_x = Wire.read()<<8 | Wire.read(); // reading registers: 0x3B (ACCEL_XOUT_H) and 0x3C (ACCEL_XOUT_L)
  accelerometer_y = Wire.read()<<8 | Wire.read(); // reading registers: 0x3D (ACCEL_YOUT_H) and 0x3E (ACCEL_YOUT_L)
  accelerometer_z = Wire.read()<<8 | Wire.read(); // reading registers: 0x3F (ACCEL_ZOUT_H) and 0x40 (ACCEL_ZOUT_L)
  temperature = Wire.read()<<8 | Wire.read(); // reading registers: 0x41 (TEMP_OUT_H) and 0x42 (TEMP_OUT_L)
  gyro_x = Wire.read()<<8 | Wire.read(); // reading registers: 0x43 (GYRO_XOUT_H) and 0x44 (GYRO_XOUT_L)
  gyro_y = Wire.read()<<8 | Wire.read(); // reading registers: 0x45 (GYRO_YOUT_H) and 0x46 (GYRO_YOUT_L)
  gyro_z = Wire.read()<<8 | Wire.read(); // reading registers: 0x47 (GYRO_ZOUT_H) and 0x48 (GYRO_ZOUT_L)

  //read force sensitive resistor
  fsrReading = analogRead(fsr);
  
  // print out data
  Serial.print("aX = "); Serial.print(convert_int16_to_str(accelerometer_x));
  Serial.print(" | aY = "); Serial.print(convert_int16_to_str(accelerometer_y));
  Serial.print(" | aZ = "); Serial.print(convert_int16_to_str(accelerometer_z));
  // the following equation was taken from the documentation [MPU-6000/MPU-6050 Register Map and Description, p.30]
  Serial.print(" | tmp = "); Serial.print(temperature/340.00+36.53);
  Serial.print(" | gX = "); Serial.print(convert_int16_to_str(gyro_x));
  Serial.print(" | gY = "); Serial.print(convert_int16_to_str(gyro_y));
  Serial.print(" | gZ = "); Serial.print(convert_int16_to_str(gyro_z));
  Serial.println();


  if(fsrReading > 500 && gripped == false){
    gripped = true;
    Serial.println("Grip");
    digitalWrite(gripperH1, LOW);
    digitalWrite(gripperH2, HIGH);
    delay(750);
    digitalWrite(gripperH1, LOW);
    digitalWrite(gripperH2, LOW);
    
  }
  if(fsrReading < 500 && gripped == true){
    gripped = false;
    Serial.println("Release");
    digitalWrite(gripperH1, HIGH);
    digitalWrite(gripperH2, LOW);
    delay(750);
    digitalWrite(gripperH1, LOW);
    digitalWrite(gripperH2, LOW);
  }
  
  if(gyro_x > 30000){
    Serial.println("Go right");
    digitalWrite(turnerH1, LOW);
    digitalWrite(turnerH2, HIGH);
    delay(750);
    digitalWrite(turnerH1, LOW);
    digitalWrite(turnerH2, LOW);
  }
  if(gyro_x < -30000){
    Serial.println("Go left");
    digitalWrite(turnerH1, HIGH);
    digitalWrite(turnerH2, LOW);
    delay(750);
    digitalWrite(turnerH1, LOW);
    digitalWrite(turnerH2, LOW);
  }
  if(gyro_z > 30000){
    Serial.println("Lift up");
    digitalWrite(lifterH1, HIGH);
    digitalWrite(lifterH2, LOW);
    delay(750);
    digitalWrite(lifterH1, LOW);
    digitalWrite(lifterH2, LOW);
  }
  if(gyro_z < -30000){
    Serial.println("Drop down");
    digitalWrite(lifterH1, LOW);
    digitalWrite(lifterH2, HIGH);
    delay(750);
    digitalWrite(lifterH1, LOW);
    digitalWrite(lifterH2, LOW);
  }
  if(accelerometer_y > 30000){
    Serial.println("Move forward");
    digitalWrite(reacherH1, LOW);
    digitalWrite(reacherH2, HIGH);
    delay(750);
    digitalWrite(reacherH1, LOW);
    digitalWrite(reacherH2, LOW);
  }
  if(accelerometer_y < -30000){
    Serial.println("Move backward");
    digitalWrite(reacherH1, HIGH);
    digitalWrite(reacherH2, LOW);
    delay(750);
    digitalWrite(reacherH1, LOW);
    digitalWrite(reacherH2, LOW);
  }
  // delay
  delay(50);
}