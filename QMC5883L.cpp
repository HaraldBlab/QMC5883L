#include "QMC5883L.h"

//I2C address of the QMC5883L
#define ADDR  0x0d

//values for the QMC5883 control register 1
//operating mode
#define Mode_Standby    0b00000000
#define Mode_Continuous 0b00000001
//Output data rate
#define ODR_10Hz        0b00000000
#define ODR_50Hz        0b00000100
#define ODR_100Hz       0b00001000
#define ODR_200Hz       0b00001100
//Measure range
#define RNG_2G          0b00000000
#define RNG_8G          0b00010000
//Over sampling rate
#define OSR_512         0b00000000
#define OSR_256         0b01000000
#define OSR_128         0b10000000
#define OSR_64          0b11000000

void QMC5883L::begin()
{
  softReset();
  setCtrlRegister(OSR_128,RNG_2G,ODR_100Hz,Mode_Continuous);  
}
//function to write data into a register on QMC5883L
void QMC5883L::writeRegister(uint8_t reg, uint8_t val){
  Wire.beginTransmission(ADDR); //start talking
  Wire.write(reg); 
  Wire.write(val);
  Wire.endTransmission();
}

/*
 * Output Data Register
 *
 * Registers 00H ~ 05H store the measurement data from each axis magnetic sensor in continuous-measurement. 
 * In the continuous measurement mode, the output data is refreshed periodically based on the data update rate ODR setup in control registers 1. 
 * The data stays the same, regardless of reading status through I²C, until new data replaces them. 
 * Each axis has 16 bit data width in 2’s complement, i.e., MSB of 01H/03H/05H indicates the sign of each axis. 
 * The output data of each channel saturates at -32768 and 32767.
 * 
 * Addr. 7 6 5 4 3 2 1 0
 * 00H Data Output X LSB Register XOUT[7:0]
 * 01H Data Output X MSB Register XOUT[15:8]
 * 02H Data Output Y LSB Register YOUT[7:0]
 * 03H Data Output Y MSB Register YOUT[15:8]
 * 04H Data Output Z LSB Register ZOUT[7:0]
 * 05H Data Output Z MSB Register ZOUT[15:8] 
 */
void QMC5883L::readData() {
  Wire.beginTransmission(ADDR);
  Wire.write(0x00);
  Wire.endTransmission();
  Wire.requestFrom(ADDR, 6);
  uint16_t val = Wire.read() | (Wire.read() << 8); 
  x = ~val + 1;    //2's compliment
  val = Wire.read() | (Wire.read() << 8); 
  y = ~val + 1;    //2's compliment
  val = Wire.read() | (Wire.read() << 8); 
  z = ~val + 1;    //2's compliment
}

/*
 * Temperature Data Registers
 * 
 * Registers 07H-08H store temperature sensor output data. 
 * 16 bits temperature sensor output is in Registers 2’s complement. 
 * Temperature sensor gain is factory-calibrated, but its offset has not been compensated, only relative temperature value is accurate. 
 * The temperature coefficient is about 100LSB/°C
 * 
 * Addr. 7 6 5 4 3 2 1 0
 * 07H TOUT[7:0]
 * 08H TOUT[15:8] 
 */
uint16_t QMC5883L::temperature() {
  Wire.beginTransmission(ADDR);
  Wire.write(0x07);
  Wire.endTransmission();
  Wire.requestFrom(ADDR, 2);
  uint16_t val = Wire.read() | Wire.read() << 8; 
  val = ~val + 1;    //2's compliment
  return val;
}

/*
 * Returns the heading of the compass in [0..360] °
 * N = 0, E = 90, S = 180, W = 270
 * posotive X axis points to north.
 */
int QMC5883L::heading() {
  //  change sign to make it cockwise
  float azimut = -atan2(y,x) * 180.0/PI;
  // add 180 degree to get only positive values
  int ret = azimut + 180.0;
  // to get things clockwise
  ret = 360 - ret;

  return ret;
}

//function to set the control register 1 on QMC5883L
void QMC5883L::setCtrlRegister(uint8_t overSampling, uint8_t range, uint8_t dataRate, uint8_t mode) {
  writeRegister(0x09,overSampling | range | dataRate | mode);
}

//function to reset QMC5883L
void QMC5883L::softReset() {
  writeRegister(0x0a,0x80);
  writeRegister(0x0b,0x01);
}
