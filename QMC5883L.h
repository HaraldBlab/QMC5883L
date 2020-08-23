#ifndef QMC5883L_h
#define QMC5883L_h

#include <Arduino.h>
#include <Wire.h> //I2C Arduino Library

/* Support of the QMC5883L device
 *  Brough to me wuth the GY-271 breakout module
 *  Source: https://www.az-delivery.de/blogs/azdelivery-blog-fur-arduino-und-raspberry-pi/kompass-mit-gy-271-und-arduino-nano 
 */
class QMC5883L
{
public:
    int x,y,z;        // triple axis data
  /* Intializes the device.
   * Mode: continuation mode.
   * Samplinf rate: 128
   * Measure range: 2G
   */
  void begin();
private:
  //function to write data into a register on QMC5883L
    void writeRegister(uint8_t reg, uint8_t val);
public:
  // function to read results from QMC5883L to x,y,z
  void readData();
  //function to read temperature from QMC5883L
  uint16_t temperature();
  //function to read heading from QMC5883L with currennt x,y, z value
  int heading();
private:
  //function to set the control register 1 on QMC5883L
  void setCtrlRegister(uint8_t overSampling, uint8_t range, uint8_t dataRate, uint8_t mode);

  //function to reset QMC5883L
  void softReset(); 
};
#endif
