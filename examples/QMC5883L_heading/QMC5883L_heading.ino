#include <QMC5883L.h>

QMC5883L compass;

//prepare hardware
void setup(){
  Serial.begin(115200);
  Wire.begin();
  
  Serial.println("Start");
  compass.begin();
  Serial.println("init done");
}

void loop(){
  compass.readData(); //read data from sensor

  // Show values on serial line
  Serial.print("X Value: ");
  Serial.println(compass.x);
  Serial.print("Y Value: ");
  Serial.println(compass.y);
  Serial.print("Z Value: ");
  Serial.println(compass.z);

  int heading = compass.heading();
  Serial.print("Heading: ");
  Serial.print(heading);
  Serial.println("°");

  // wait 1 second
  delay(1000);
}
