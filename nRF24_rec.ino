
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Servo.h>
#include "Filter.h"

// Com 7 
const uint8_t VescOutputPin = 5;
ExponentialFilter<float> FilteredRaw(20, 0);
const uint8_t PotentiometerPin = A0;

int x = 0;
Servo esc;

RF24 radio(9, 10);   // nRF24L01 (CE, CSN)
const byte address[6] = "00001";
unsigned long lastReceiveTime = 0;
unsigned long currentTime = 0;
// Max size of this struct is 32 bytes - NRF24L01 buffer limit
struct Data_Package {
  byte j1PotX;

};
Data_Package data; //Create a variable with the above structure
void setup() {
  Serial.begin(9600);
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setAutoAck(false);
  radio.setDataRate(RF24_250KBPS);
  radio.setPALevel(RF24_PA_LOW);
  radio.startListening(); //  Set the module as receiver
  resetData();
  esc.attach(VescOutputPin);

  esc.writeMicroseconds(1500);
}
void loop() {
  // Check whether there is data to be received
  if (radio.available()) {
    radio.read(&data, sizeof(Data_Package)); // Read the whole data and store it into the 'data' structure
    lastReceiveTime = millis(); // At this moment we have received the data
  }
  // Check whether we keep receving data, or we have a connection between the two modules
  currentTime = millis();
  if ( currentTime - lastReceiveTime > 1000 ) { // If current time is more then 1 second since we have recived the last data, that means we have lost connection
    resetData(); // If connection is lost, reset the data. It prevents unwanted behavior, for example if a drone has a throttle up and we lose connection, it can keep flying unless we reset the values
  }
  // Print the data in the Serial Monitor
  esc.writeMicroseconds(map(data.j1PotX, 0 , 255, 1000, 2000));
  Serial.print("j1PotX: ");
  Serial.println(data.j1PotX);

}
void resetData() {
  // Reset the values when there is no radio connection - Set initial default values
  data.j1PotX = 127;

}
int joy_read()
{
  
  delay(4); 
  float Raw = analogRead(data.j1PotX);
  FilteredRaw.Filter(Raw);
  int j = FilteredRaw.Current();
  return j;
  }
