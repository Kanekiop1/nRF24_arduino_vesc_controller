
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Wire.h>

//Wysyłanie com 9 oldBootloader
// Define the digital inputs
#define jB1 1  // Joystick button 1
#define jB2 0  // Joystick button 2



float elapsedTime, currentTime, previousTime;
int c = 0;


RF24 radio(9, 10);   // nRF24L01 (CE, CSN)
const byte address[6] = "00001"; // Address

// Max size of this struct is 32 bytes - NRF24L01 buffer limit
struct Data_Package {
  byte j1PotX;

};

Data_Package data; //Create a variable with the above structure

void setup() {
  Serial.begin(9600);
  
  // Call this function if you need to get the IMU error values for your module
  //calculate_IMU_error();
  
  // Define the radio communication
  radio.begin();
  radio.openWritingPipe(address);
  radio.setAutoAck(false);
  radio.setDataRate(RF24_250KBPS);
  radio.setPALevel(RF24_PA_LOW);
  
  // Activate the Arduino internal pull-up resistors
  pinMode(jB1, INPUT_PULLUP);
  pinMode(jB2, INPUT_PULLUP);

  
  // Set initial default values
  data.j1PotX = 127; // Values from 0 to 255. When Joystick is in resting position, the value is in the middle, or 127. We actually map the pot value from 0 to 1023 to 0 to 255 because that's one BYTE value


}
void loop() {
  // Read all analog inputs and map them to one Byte value
  data.j1PotX = map(analogRead(A0), 0, 1023, 0, 255); // Convert the analog read value from 0 to 1023 into a BYTE value from 0 to 255


  // Send the whole data from the structure to the receiver
  radio.write(&data, sizeof(Data_Package));
  Serial.println(analogRead(A0));
}
