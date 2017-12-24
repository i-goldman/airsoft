#include <Wire.h>
#include <Keypad.h>
#include "Mfrc522.h"
#include <SPI.h>



int hours,minutes,seconds;
uint16_t displaybuffer[8];







  void writedata (int data)
{
  Wire.begin();
  Wire.beginTransmission(0x70);
  Wire.write(data);
  Wire.endTransmission();


}

void initialise_display()
{
  delay(100);
  writedata(0x21);
  writedata(0x80 | 0x01 | 0x00);
  writedata(0xE0 | 15);

      displaybuffer[2] = 0x2;
  Wire.beginTransmission(0x70);
  Wire.write((uint8_t)0x04);
  Wire.write(displaybuffer[2] & 0xFF);
  Wire.write(displaybuffer[2] >> 8);
  Wire.endTransmission();
  

}



void update_display ()
{
   Wire.beginTransmission(0x70);
        Wire.write((uint8_t)0x00); // start at address $00

        for (uint8_t i=0; i<8; i++) {
          Wire.write(displaybuffer[i] & 0xFF);    
          Wire.write(displaybuffer[i] >> 8);    
    }
    Wire.endTransmission();
}



const static uint8_t numbertable[] = {
  0x3F, /* 0 */
  0x06, /* 1 */
  0x5B, /* 2 */
  0x4F, /* 3 */
  0x66, /* 4 */
  0x6D, /* 5 */
  0x7D, /* 6 */
  0x07, /* 7 */
  0x7F, /* 8 */
  0x6F, /* 9 */
  0x77, /* a */
  0x7C, /* b */
  0x39, /* C */
  0x5E, /* d */
  0x79, /* E */
  0x71, /* F */
};






void countdown ()
{
  if (seconds != 0 || minutes != 0 || hours != 0)
  {
    seconds--;
    if (seconds < 0)
      {
        seconds = 59;
        if (minutes > 0)
          minutes--;
        else if (hours > 0)
        {
          hours--;
          minutes = 59;
        }
      }

    if (hours > 0)
      {
displaybuffer[0] = numbertable[(int) hours / 10];
displaybuffer[1] = numbertable[hours % 10];
displaybuffer[3] = numbertable[(int) minutes / 10];
displaybuffer[4] = numbertable[minutes % 10]; 
      }
    else
    {
displaybuffer[0] = numbertable[(int) minutes / 10];
displaybuffer[1] = numbertable[minutes % 10];
displaybuffer[3] = numbertable[(int) seconds / 10];
displaybuffer[4] = numbertable[seconds % 10]; 
    }
    update_display();      
}
  else //out of time
  {
    displaybuffer[0] = numbertable[15];
displaybuffer[1] = numbertable[15];
displaybuffer[3] = numbertable[15];
displaybuffer[4] = numbertable[15];
    update_display();   
  }
  
}


bool keypd = false;
bool keySW = false;
bool RF  = false;
bool mgnt = false;

void update_leds()
{
  pinMode(11,OUTPUT);
  pinMode(8,OUTPUT);
  pinMode(18,OUTPUT);
  pinMode(19,OUTPUT);

  digitalWrite(11,!keypd);
  digitalWrite(8,!keySW);
  digitalWrite(18,!RF);
  digitalWrite(19,!mgnt);
}

const byte ROWS = 4; //four rows
const byte COLS = 3; //three columns
char keys[ROWS][COLS] = {
    {'1','2','3'},
    {'4','5','6'},
    {'7','8','9'},
    {'*','0','#'}
};

byte rowPins[ROWS] = {34, 33, 32, 31}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {37, 36, 35}; //connect to the column pinouts of the keypad

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

char keypad_code [6] = {'1','2','3','4','5','6'};
char keypad_entered [6] = {'a','a','a','a','a','a'};

void check_keycode()
{
  if (keypad_code == keypad_entered)
  {
   keypd = true;
   return;
  }
  if (keypad_entered[6] == 'a')
    return;
  /*if ((keypad_code != keypad_entered) && (keypad_entered[6] != 'a'))
    {
      for (int i = 0 ; i < 6 ; i++)
        {
          keypad_entered [i] = 'a';
        }
      for (int i = 0 ; i < 30 ; i++)
        {
          countdown();
        }
    }*/
    
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  initialise_display();
  hours = 10;
  minutes = 0;
  seconds = 0;
  keypad.addEventListener(keypadEvent); 
  
}


void loop() {
  // put your main code here, to run repeatedly: 
countdown();
for (int i = 0 ; i < 100 ; i++)
  {
    update_leds();
    delay(10); 
    check_keycode();
}
}




void keypadEvent(KeypadEvent key){
    switch (keypad.getState()){
    case PRESSED:
                    Serial.println(key);
                    for (int i = 0 ; i < 6 ; i++)
                    {
                      if (keypad_entered[i] == 'a')
                        {
                        keypad_entered [i] = key;
                        break;
                        }
                    }
                    break;

    }
}




















