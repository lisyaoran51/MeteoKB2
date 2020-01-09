#include <LinkedList.h>

struct PressedKey
{
   int Key;
   unsigned long PressedTime;
};

struct KeyToOutput
{
   int Key;
   int Velocity;
};

int keyMap[48];
bool pressedMap[48];
bool pressedDownMap[48];
int pressNoBounceCount[96]; //按下以後連續按著沒有彈跳的次數
int upNoBounceCount[48];    //放開以後連續放開沒有彈跳的次數，只看上半橡膠紐，不看下半橡膠鈕

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
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
    keyMap[i] = 71 - i;
    pressedMap[i] = false;
    pressedDownMap[i] = false;
    pressNoBounceCount[i] = 0;
    pressNoBounceCount[i+48] = 0;
    upNoBounceCount[i] = 0;
  }
}

LinkedList<struct PressedKey*> pressedKeys = LinkedList<struct PressedKey*>();
LinkedList<struct KeyToOutput*> keysToOutput = LinkedList<struct KeyToOutput*>();


void loop() {
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
            int pressedDownKey = keyMap[i / 2 * 8 + j]; // 由右往左每8個鍵輪一次
            unsigned long nowTime = millis();
            for(int k = 0; k < pressedKeys.size(); k++){
              if(pressedKeys.get(k)->Key == pressedDownKey && !pressedDownMap[i / 2 * 8 + j]){
                pressedDownMap[i / 2 * 8 + j] = true;
                
                unsigned long velocity = nowTime - pressedKeys.get(k)->PressedTime;
                struct KeyToOutput* keyToOutput = new struct KeyToOutput();
                keyToOutput->Key = pressedDownKey;
                keyToOutput->Velocity = velocity;
                keysToOutput.add(keyToOutput);
                struct PressedKey* pressedKey = pressedKeys.remove(k);
                delete pressedKey;
                //Serial.println(pressedDownKey);
              }
            }
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
              keysToOutput.add(keyToOutput);
            }
          }
        }
      }
    }
  }
  
  for(int i = 0; i < keysToOutput.size(); i++){
    Serial.print(keysToOutput.get(i)->Key);
    Serial.print(":");
    Serial.println(keysToOutput.get(i)->Velocity);
    delete keysToOutput.get(i);
  }
  keysToOutput.clear();
  
}
