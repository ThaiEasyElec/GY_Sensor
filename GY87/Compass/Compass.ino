#include <Wire.h> //I2C Arduino Library
#include "MPU6050.h"
#define address 0x1E //0011110b, I2C 7bit address of HMC5883
MPU6050 accelgyro;
int aX, aY, aZ;

void setup() {
  //Initialize Serial and I2C communications
  Serial.begin(9600);
  Wire.begin();
  //Set Bypass-Mode for HMC5883L I2C
  accelgyro.setI2CMasterModeEnabled(false);
  accelgyro.setI2CBypassEnabled(true) ;
  accelgyro.setSleepEnabled(false);


  //Put the HMC5883 IC into the correct operating mode
  Wire.beginTransmission(address); //open communication with HMC5883
  Wire.write(0x02); //select mode register
  Wire.write(0x00); //continuous measurement mode
  Wire.endTransmission();
}

void loop() {

  int x, y, z; //triple axis data

  //Tell the HMC5883 where to begin reading data
  Wire.beginTransmission(address);
  Wire.write(0x03); //select register 3, X MSB register
  Wire.endTransmission();


  //Read data from each axis, 2 registers per axis
  Wire.requestFrom(address, 6);
  if (6 <= Wire.available()) {
    x = Wire.read() << 8; //X msb
    x |= Wire.read(); //X lsb
    z = Wire.read() << 8; //Z msb
    z |= Wire.read(); //Z lsb
    y = Wire.read() << 8; //Y msb
    y |= Wire.read(); //Y lsb
  }

  aX = (aX + x) / 2;
  aY = (aY + y) / 2;
  aZ = (aZ + z) / 2;

  //Print out values of each axis
  Serial.print("x: ");
  Serial.print(aX);
  Serial.print("  y: ");
  Serial.print(aY);
  Serial.print("  z: ");
  Serial.println(aZ);

  delay(250);
}