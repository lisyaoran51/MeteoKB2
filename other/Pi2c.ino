#include <Wire.h>


int count = 0;
bool button[6];
int pin[6];
unsigned char charToPass[6][9];
int toPassCommandNumber = -1;

void setup() {
  Serial.begin(9600);// open the serial port at 9600 bps:
  Wire.begin(0x07); //Set Arduino up as an I2C slave at address 0x07
  Wire.onRequest(requestEvent); //Prepare to send data
  Wire.onReceive(receiveEvent); //Prepare to recieve data

  for(int i = 0; i < 6; i++){
    button[i] = false;
    pin[i] = i+5;
  }
  strncpy(charToPass[0], "151,99  ", sizeof(charToPass[0]));
  strncpy(charToPass[1], "1020,1  ", sizeof(charToPass[1]));
  strncpy(charToPass[2], "1020,-1 ", sizeof(charToPass[2]));
  strncpy(charToPass[3], "1012,0  ", sizeof(charToPass[3]));
  strncpy(charToPass[4], "1010,0  ", sizeof(charToPass[4]));
  strncpy(charToPass[5], "1011,0  ", sizeof(charToPass[5]));
  
}

void loop() {

  for(int i = 0; i < 6; i++){
    if(!button[i]){
      if(digitalRead(pin[i]) == HIGH){
        button[i] = true;
        toPassCommandNumber = i;
        Serial.print("press ");
        Serial.println(i);
      }
    }
    else{
      if(digitalRead(pin[i]) == LOW){
        button[i] = false;
      }
    }
    
  }
  if(count++ % 100 == 1)
    Serial.println(count);
  delay(1);
}

void requestEvent()
{
  count++;
  if(count == 3000){
    unsigned char char_ar[9] = "151,99  "; //Create String
    Wire.write(char_ar,9); //Write String to Pi.
    return;
  }
  if(toPassCommandNumber != -1){
    Wire.write(charToPass[toPassCommandNumber],9);
    toPassCommandNumber = -1;
  }
  unsigned char char_ar[9] = "        "; //Create String
  Wire.write(char_ar,9); //Write String to Pi.
}

void receiveEvent(int numBytes){
  //Set Up Vars
  int receive_int=0;
  int count=0;

  //We'll recieve one byte at a time. Stop when none left
  while(Wire.available())
  {
    char c = Wire.read();    // receive a byte as character
    //Create Int from the Byte Array
    receive_int = c << (8 * count) | receive_int;
    count++;
  }
  //Print the Int out.
  Serial.print("Received Number: "); 
  Serial.println(receive_int);
}
