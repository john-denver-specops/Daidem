/* Project: Diadem Helmet
 *  
 * Created: 12-10-2020, Monday
 * 
 * Edits: #1  Added code for NRF24
 *            Added helmet button
 *        #2  Added MQ3 alcohol sensor code
 *            
 *            
 * Pinout:
 * D13  = SCK NRF24
 * D12  = MISO NRF24
 * D11  = MOSI NRF24
 * D8   = CSN NRF24
 * D7   = CE NRF24
 * D6   = Dout MQ3
 */

#include <SPI.h>                                //libraries for NRF24
#include <nRF24L01.h>
#include <RF24.h>


RF24 radio(7, 8);                               // CE, CSN
const byte address[6] = "00001";                // RF address
int button=6;

const int DOUTpin=5;//the DOUT pin of the alcohol sensor goes into digital pin D8 of the arduino
int limit;

void setup() {

  pinMode(button, INPUT_PULLUP); 
  pinMode(DOUTpin, INPUT);//sets the pin as an input to the arduino
  
  
  radio.begin();                                //RF transmitter setup
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MIN);
  radio.stopListening();
  
  Serial.begin(9600);
  
}

void loop() {

  if(digitalRead(button)==HIGH)
  {
  Serial.println("Helmet Removed");
  const char text[] = "helmet_0";                 //message
  radio.write(&text, sizeof(text));                     //sending code
  }
  else
  {
  Serial.println("Helmet Fixed");
  const char text[] = "helmet_1";                 //message
  radio.write(&text, sizeof(text));                     //sending code
  }
  limit= digitalRead(DOUTpin);//reads the digital value from the alcohol sensor's DOUT pin
  
  
  if (limit == HIGH){
    Serial.println("No alcohol");
  }
  else{
    const char text[] = "alcohol_1";                 //message
    radio.write(&text, sizeof(text));                     //sending code
    Serial.println("Alcohol Detected!");
  }
  delay(1000);                                          //delay time
}
