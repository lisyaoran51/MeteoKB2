#include <Adafruit_NeoPixel.h>
#include <Wire.h>
#include <LinkedList.h>
 
#define PIN 12
#define MAX_LED 16
#define SLAVE_ADDRESS 0x07
#define SERIAL_BAUD 9600 
 

struct LightRing
{
   float targetTime;
   float targetTotalTime;
   float pedalTime;
   float lightRingTotalTime;
};

 
// Parameter 1 = ws2811级联数量
// Parameter 2 = arduino PIN
// Parameter 3 = pixel type flags, add together as needed:
// NEO_KHZ800 800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
// NEO_KHZ400 400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
// NEO_GRB Pixels are wired for GRB bitstream (most NeoPixel products)
// NEO_RGB Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
Adafruit_NeoPixel strip = Adafruit_NeoPixel( MAX_LED, PIN, NEO_RGB + NEO_KHZ800 );
uint32_t color = strip.Color(255, 255, 255);
uint32_t lowColor = strip.Color(20, 20, 20);
uint32_t noColor = strip.Color(0, 0, 0);


uint8_t lights[MAX_LED];
unsigned long nowTime, lastTime;
LinkedList<struct LightRing*> list = LinkedList<struct LightRing*>();

 
void setup()
{
  //Wire.begin(SLAVE_ADDRESS);    // join I2C bus as a slave with address 1
  //Wire.onReceive(receiveEvent); // register event

  //Serial.begin(SERIAL_BAUD);
  //Serial.println("I2C Slave started\n");
  
  strip.begin();
  strip.show();
  lastTime = millis();
}

int count = 0;
 
void loop()
{
  lastTime = nowTime;
  nowTime=millis();


  for(int i = 0; i < MAX_LED; i++)
    lights[i] = 0;

    
  int i = count++;
  lights[i] = 255;
  lights[i-1 < 0 ? i-1 + 16 : i-1] = 180;
  lights[i-2 < 0 ? i-2 + 16 : i-2] = 120;
  lights[i-3 < 0 ? i-3 + 16 : i-3] = 60;
    
  count %= 16;
  
  for(int i = 0; i < MAX_LED; i++){
    strip.setPixelColor(MAX_LED - i - 2 < 0 ? 15 : MAX_LED - i - 2, strip.Color(lights[i], lights[i], lights[i]));
  }


  
  strip.show();


  delay(100);
}


char buffer[32];
void receiveEvent(int numBytes){
  strncpy(buffer, "", sizeof(buffer));
  int count = 0;
  //We'll recieve one byte at a time. Stop when none left
  while(Wire.available())
  {
    buffer[count] = Wire.read();
    count++;
  }
  Serial.println(buffer);

  struct LightRing* lightRing = new struct LightRing();
  
  char *str;
  char *splitBuffer = buffer;
  //strncpy(splitBuffer, buffer, sizeof(buffer));
  // 名字
  str = strtok_r(splitBuffer, ",", &splitBuffer);
  //Serial.println(str);
  // 名字-end

  // 目標秒數
  str = strtok_r(splitBuffer, ",", &splitBuffer);
  //Serial.println(str);
  lightRing->targetTime = atof(str);
  lightRing->targetTotalTime = atof(str);
  // 目標秒數-end
  
  // 踏板秒數
  str = strtok_r(splitBuffer, ",", &splitBuffer);
  //Serial.println(str);
  // 踏板秒數-end

  // 踏板總秒數
  //Serial.println(splitBuffer);
  lightRing->pedalTime = atof(splitBuffer);
  lightRing->lightRingTotalTime = atof(splitBuffer);
  // 踏板總秒數-end

  list.add(lightRing);
}
