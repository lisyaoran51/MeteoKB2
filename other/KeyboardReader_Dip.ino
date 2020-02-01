#include <LinkedList.h>
#include <Wire.h>
#include <math.h>

#include "IRremote.h"
/*-----( Declare Constants )-----*/
int receiver = 10; // pin 1 of IR receiver to Arduino digital pin 3
/*-----( Declare objects )-----*/
IRrecv irrecv(receiver);           // create instance of 'irrecv'
decode_results results;            // create instance of 'decode_results'
bool irSent = false;
bool ff = false;
bool rw = false;
/*-----( Declare Variables )-----*/

#define SLAVE_ADDRESS 0x07

struct PressedKey
{
   int Key;
   unsigned long PressedTime;
};

struct KeyToOutput
{
   int Key;
   int Velocity;
   int PressTimeSpan;
};

int keyMap[48];
bool pressedMap[48];
bool pressedDownMap[48];
int pressNoBounceCount[96]; //按下以後連續按著沒有彈跳的次數
int upNoBounceCount[48];    //放開以後連續放開沒有彈跳的次數，只看上半橡膠紐，不看下半橡膠鈕

bool mutex = false;
bool isChanged = false;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  Wire.begin(SLAVE_ADDRESS);    // join I2C bus as a slave with address 1
  Wire.onRequest(requestEvent); //Prepare to send data
 // Wire.onReceive(receiveEvent); //Prepare to recieve data
   
  pinMode(2, INPUT);
  pinMode(3, INPUT);
  pinMode(4, INPUT);
  pinMode(5, INPUT);
  pinMode(6, INPUT);
  pinMode(7, INPUT);
  pinMode(8, INPUT);
  pinMode(9, INPUT);
  pinMode(A0, OUTPUT);
  pinMode(A1, OUTPUT);
  pinMode(A2, OUTPUT);
  pinMode(A3, OUTPUT);
  digitalWrite(A0,LOW);
  digitalWrite(A1,LOW);
  digitalWrite(A2,LOW);
  digitalWrite(A3,LOW);
  for(int i = 0; i < 48; i++){
    keyMap[i] = 174 - i;
    pressedMap[i] = false;
    pressedDownMap[i] = false;
    pressNoBounceCount[i] = 0;
    pressNoBounceCount[i+48] = 0;
    upNoBounceCount[i] = 0;
  }

  irrecv.enableIRIn(); // Start the receiver
}

LinkedList<struct PressedKey*> pressedKeys = LinkedList<struct PressedKey*>();
LinkedList<struct KeyToOutput*> keysToOutput = LinkedList<struct KeyToOutput*>();


void loop() {
  if (irrecv.decode(&results)) // have we received an IR signal?
  {
    translateIR();
    irrecv.resume();
  }
  
  // put your main code here, to run repeatedly:
  for(int i = 0; i < 12; i++){
    switch(i){
      case 0:
        digitalWrite(A0,LOW);
        digitalWrite(A1,LOW);
        digitalWrite(A2,LOW);
        digitalWrite(A3,LOW); //切換到第一個74hc138
        break;
      case 1:
        digitalWrite(A0,HIGH);
        digitalWrite(A1,LOW);
        digitalWrite(A2,LOW);
        break;
      case 2:
        digitalWrite(A0,LOW);
        digitalWrite(A1,HIGH);
        digitalWrite(A2,LOW);
        break;
      case 3:
        digitalWrite(A0,HIGH);
        digitalWrite(A1,HIGH);
        digitalWrite(A2,LOW);
        break;  
      case 4:
        digitalWrite(A0,LOW);
        digitalWrite(A1,LOW);
        digitalWrite(A2,HIGH);
        break;  
      case 5:
        digitalWrite(A0,HIGH);
        digitalWrite(A1,LOW);
        digitalWrite(A2,HIGH);
        break;  
      case 6:
        digitalWrite(A0,LOW);
        digitalWrite(A1,HIGH);
        digitalWrite(A2,HIGH);
        break;  
      case 7:
        digitalWrite(A0,HIGH);
        digitalWrite(A1,HIGH);
        digitalWrite(A2,HIGH);
        break; 
      case 8:
        digitalWrite(A0,LOW);
        digitalWrite(A1,LOW);
        digitalWrite(A2,LOW);
        digitalWrite(A3,HIGH); //切換到第二個74hc138
        break; 
      case 9:
        digitalWrite(A0,HIGH);
        digitalWrite(A1,LOW);
        digitalWrite(A2,LOW);
        break; 
      case 10:
        digitalWrite(A0,LOW);
        digitalWrite(A1,HIGH);
        digitalWrite(A2,LOW);
        break; 
      case 11:
        digitalWrite(A0,HIGH);
        digitalWrite(A1,HIGH);
        digitalWrite(A2,LOW);
        break; 
    }
    

    for(int j = 0; j < 8; j++){
      if(digitalRead(j+2) == 0){
        if(i % 2 == 0){ //ACE是上半橡膠鍵
          upNoBounceCount[i / 2 * 8 + j] = 0;
          
          if(++pressNoBounceCount[i * 8 + j] >= 10){
            int key = keyMap[i / 2 * 8 + j]; // 由右往左每8個鍵輪一次
            bool notPressedYet = true;
            
            //Serial.println(key);
            for(int k = 0; k < pressedKeys.size(); k++){
              
              if(pressedKeys.get(k)->Key == key)
                notPressedYet = false;
            }
            
            if(notPressedYet && !pressedMap[i / 2 * 8 + j]){ 
              struct PressedKey* newPressedKey = new struct PressedKey();
              newPressedKey->Key = key; // 由右往左每8個鍵輪一次
              newPressedKey->PressedTime = millis();
              
              pressedKeys.add(newPressedKey);
              pressedMap[i / 2 * 8 + j] = true;
            }
          }
        }
        else{
          if(++pressNoBounceCount[i * 8 + j] >= 10){

            
            mutex = true;
            
            int pressedDownKey = keyMap[i / 2 * 8 + j]; // 由右往左每8個鍵輪一次
            unsigned long nowTime = millis();
            for(int k = 0; k < pressedKeys.size(); k++){
              if(pressedKeys.get(k)->Key == pressedDownKey && !pressedDownMap[i / 2 * 8 + j]){
                pressedDownMap[i / 2 * 8 + j] = true;
                
                unsigned long pressTimeSpan = nowTime - pressedKeys.get(k)->PressedTime;
                struct KeyToOutput* keyToOutput = new struct KeyToOutput();
                keyToOutput->Key = pressedDownKey;
                keyToOutput->PressTimeSpan = pressTimeSpan;
                //Serial.println(pressTimeSpan);
                keyToOutput->Velocity = pow(1.000554672, pow(102 - pressTimeSpan, 2));// 1.057018041是256的100根，102是因為最小壓下時間只能到2
                
                if(keyToOutput->Velocity > 0)
                  keysToOutput.add(keyToOutput);
                  
                struct PressedKey* pressedKey = pressedKeys.remove(k);
                delete pressedKey;
                //Serial.println(pressedDownKey);
              }
            }
            
            mutex = false;
          }
        }
      }
      else{
        pressNoBounceCount[i * 8 + j] = 0;
        
        if( i % 2 == 0 && pressedMap[i / 2 * 8 + j]){
          if(++upNoBounceCount[i / 2 * 8 + j] > 10){
            pressedMap[i / 2 * 8 + j] = false;
            upNoBounceCount[i / 2 * 8 + j] = 0;
            for(int k = 0; k < pressedKeys.size(); k++){
              if(pressedKeys.get(k)->Key == keyMap[i / 2 * 8 + j]){
                struct PressedKey* toDelete = pressedKeys.remove(k);
                delete toDelete;
              }
            }
            if(pressedDownMap[i / 2 * 8 + j]){
              
              pressedDownMap[i / 2 * 8 + j] = false;
              struct KeyToOutput* keyToOutput = new struct KeyToOutput();
              keyToOutput->Key = keyMap[i / 2 * 8 + j]; // 由右往左每8個鍵輪一次
              keyToOutput->Velocity = -1;
              
              mutex = true;
              keysToOutput.add(keyToOutput);
              mutex = false;
            }
          }
        }
      }
    }
  }



  return;
  for(int i = 0; i < keysToOutput.size(); i++){
    Serial.print(keysToOutput.get(i)->Key);
    Serial.print(":");
    Serial.println(keysToOutput.get(i)->Velocity);
    delete keysToOutput.get(i);
  }
  keysToOutput.clear();
}

bool isRequesting = false;

void requestEvent()
{
  if(isRequesting)
  return;
  isRequesting = true;
  
  if(mutex){
    isRequesting = false;
    return;
  }
  
  for(int i = 0; i < keysToOutput.size(); i++){

    unsigned char char_ar[9] = "        ";
    char_ar[0] = 48;
    char_ar[1] = keysToOutput.get(i)->Key / 100 + 48;
    char_ar[2] = keysToOutput.get(i)->Key % 100 / 10 + 48;
    char_ar[3] = keysToOutput.get(i)->Key % 10 + 48;
    char_ar[4] = 44;
    if(keysToOutput.get(i)->Velocity > 0){
      char_ar[5] = keysToOutput.get(i)->Velocity / 100 + 48;
      char_ar[6] = keysToOutput.get(i)->Velocity % 100 / 10 + 48;
      char_ar[7] = keysToOutput.get(i)->Velocity % 10 + 48;
    }
    else{
      char_ar[5] = 45;
      char_ar[6] = 49;
    }

    Wire.write(char_ar,9);
    delete keysToOutput.get(i);
    keysToOutput.remove(i);
    isRequesting = false;
    return;
  }
  keysToOutput.clear();
  isRequesting = false;

  if(irSent){
    if(ff){
      unsigned char char_ar[9] = "1021,1  ";
      Wire.write(char_ar,9);
      ff = false;
    }
    else if(rw){
      unsigned char char_ar[9] = "1021,-1 ";
      Wire.write(char_ar,9);
      rw = false;
    }
    else{
      unsigned char char_ar[9] = "1012,1  ";
      Wire.write(char_ar,9);
    }
    
    irSent = false;
  }
}

int couuu = 0;
void receiveEvent(int numBytes){
  if(couuu++ % 2 == 0)
    Serial.println("Received Number"); 
  
}

void translateIR() // takes action based on IR code received
// describing KEYES Remote IR codes
{
  switch(results.value)
  {
  case 0xFF629D: Serial.println(" FORWARD"); break;
  case 0xFF22DD: Serial.println("RW");rw = true;    break;
  case 0xFF02FD: Serial.println("FF");ff = true;    break;
  case 0xFFFFFFFF: Serial.println(" REPEAT");break;
  default:
    Serial.println(results.value);
  }// End Case
  irSent = true;
  
}


