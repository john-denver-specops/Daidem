/* Project: Diadem Vehicle
 *  
 * Created: 12-10-2020, Monday
 * 
 * Edits: #1  Added code for NRF24
 *            Added alchl_1 function
 *            Added relay
 *            Added LED 
 *            Added buzzer
 *            Added LCD I2C code
 *            
 *            
 * Pinout:
 * 
 * 
 *
 * 
 * 
 */

#include <SPI.h>                                //header for SPI communication protocol
#include <nRF24L01.h>                           //NRF24 header 
#include <RF24.h>                               //NRF24 header

#include <Wire.h>                               //header for I2C communciation protocol
#include <LiquidCrystal_I2C.h>                  //header for LCD 16X2 with I2C

RF24 radio(7, 8); // CE, CSN              
LiquidCrystal_I2C lcd(0x27, 16, 2);             // Set the LCD address to 0x27 for a 16 chars and 2 line display
 
const byte address[6] = "00001";

int count=0;    
int mode=0;

int relay=6;          //relay conencted to D6
int led=5;            //led connected to D5
int buzzer=9;         //buzzer connected to D9



//=============================== ALCHL_1 fucntion==============

void alchl_1(){
  Serial.println("Alcohol detected! Engine Deactivated");
  lcd.clear();
  lcd.print("Alcohol Present!");
  lcd.setCursor(0,1);
  lcd.print("Engine Off!");
  digitalWrite(relay,HIGH);          //relay switch off
  mode=1;
  for(int i=2;i>0;i++)              //infinte loop, must be restarted 
  {
      tone(buzzer,2000);            //buzzer
      digitalWrite(led,HIGH);       //led blink
      delay(500);
      digitalWrite(led,LOW);
      noTone(buzzer);
      delay(1500);
  }
}


//================= SETUP function =======================

void setup() {
  Serial.begin(9600);                          //Serial baud rate set to 9600
  radio.begin();                               // nrf24 
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MIN);
  radio.startListening();

  lcd.begin();                                  //initialize the LCD
  lcd.backlight();                              // Turn on the blacklight and print a message.
  lcd.print("Drive Safe!");                     //lcd dsiplay code
  lcd.setCursor(0,1);
  lcd.print("Connecting...");
  lcd.setCursor(0,0);
  Serial.println("LCD set");                    //CHECK for lcd working

  
  pinMode(relay,OUTPUT);                      //declaring realy pin as output
  digitalWrite(relay,LOW);                   //relay switch on

    

  
  pinMode(led,OUTPUT);                        //declaring led as output
  digitalWrite(led,HIGH);                     //led on
  delay(500);
  digitalWrite(led,LOW);                      //led off

  pinMode(buzzer,OUTPUT);                     //delcaring buzzer as output
  tone(buzzer,2000);                          //buzzer beep
  delay(500);
  noTone(buzzer);

  //lcd.clear();
  Serial.println("Device Setup Successful");
}


//===================== LOOP FUNCTION ==============

void loop() {
  if (radio.available()) {                          //checking if connected
    char text[32] = ""; 
    radio.read(&text, sizeof(text));                //reading incoming message

    
    String tert = text;                             //storing message in string tert
    Serial.println(text);                           
    if(tert.equals("alcohol_1"))                      //checking if alcohol is detected
  {
    alchl_1();    //calls alchl_1 fucntion  
    mode=1;
  }
  if(mode==0)
  {
    if(tert.equals("helmet_0"))                     //checking if the helmet is removed
  {
    count++;
    if(count>=5)                                    //giving a delay of 5 seconds
    {
      Serial.println("Helmet Removed");
      
      lcd.clear();                                  //warning displayed on LCD
      lcd.print("Helmet Removed");
      lcd.setCursor(0,1);
      lcd.print("Wear Helmet!");
      lcd.setCursor(0,0);

      tone(buzzer,2000);            //buzzer
      digitalWrite(led,HIGH);       //led blink
      delay(500);
      digitalWrite(led,LOW);
      noTone(buzzer);
    }
  }
    else
  {
    Serial.println("Helmet Fastened!");     //message 
    lcd.clear();
    lcd.print("Helmet Worn");               //message on LCD
    lcd.setCursor(0,1);
    lcd.print("Drive Safe!");
    lcd.setCursor(0,0);
    count=0;
  }   
  }
  }
}
