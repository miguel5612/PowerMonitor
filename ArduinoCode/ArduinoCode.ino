// 3950 THERMISTOR BASED SUPERVISION SYSTEM.
// Written by Miguel Angel Califa Urquiza
// Released under an MIT license.

#include <Wire.h>
#include <thermistor.h>
#include <DHT.h>
#include <SoftwareSerial.h> // Arduino IDE <1.6.6
#include <PZEM004T.h>

//Variables
String outStr = "";
double t,h,hic;
float i1,v1,p1,e1,i2,v2,p2,e2;

//Definitions

#define AmbientTerm A6   // Thermistor Base Ambient (1)
#define timeDelay 100
#define sep ','
#define dhtPin 9
#define dhtType DHT11
#define txMain 11 //Single pin for all Meters
#define rx1 10 //Meter 1
#define rx2 9  //Meter 2

PZEM004T pzem1(rx1, txMain); // RX,TX
IPAddress ip1(192, 168, 1, 1);

PZEM004T pzem2(rx2, txMain); // RX,TX
IPAddress ip2(192, 168, 1, 10);

DHT dht(dhtPin, dhtType); 

void setup() {
  // put your setup code here, to run once: 
  Serial.begin(9600); //initialize port serial at 9600 Bauds.
  pzem1.setAddress(ip1);
  pzem2.setAddress(ip2);
  Wire.begin(8);
  Wire.onRequest(requestEvent); // register event
  dht.begin();
}

void loop() {
  // All thermistor temperature is in degrade.
  getTemp();
  getPower();
  //Print temperature in port serial  
  delay(timeDelay); //wait 2000 mS for next measure
}
void requestEvent()
{
  outStr = getData();
  Serial.print("DATA: ");
  Serial.println(outStr);
  int str_len = outStr.length() + 1;  
  uint8_t Buffer[str_len];
  outStr.toCharArray(Buffer, str_len);
  Wire.write(Buffer,str_len);
}
void getTemp(){
  h = dht.readHumidity();
  t = dht.readTemperature();
  hic = dht.computeHeatIndex(t, h, false);
}
void getPower(){
  v1 =  pzem1.voltage(ip1);
  i1 = pzem1.current(ip1);
  p1 = pzem1.power(ip1);
  e1 = pzem1.energy(ip1);
  v2 =  pzem2.voltage(ip2);
  i2 = pzem2.current(ip2);
  p2 = pzem2.power(ip2);
  e2 = pzem2.energy(ip2);
}
String getData(){
  return String(i1) + sep + String(p1) + sep + String(e1) + sep + String(v1) + sep + String(i2) + sep + String(p2) + sep + String(e2) + sep + String(v2) + String(t) + sep + String(h) + sep + String(hic) ;
}
