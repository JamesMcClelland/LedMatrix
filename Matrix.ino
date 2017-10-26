#define FASTLED_ALLOW_INTERRUPTS 0
#include <FastLED.h>

#include <LEDMatrix.h>
#include <LEDSprites.h>
#include <ESP8266WiFi.h>
#include <time.h>

int numWidth = 3;
int numHeight = 5;
const uint8_t NumberData[] = 
{

  //0
  B8_1BIT(11100000),
  B8_1BIT(10100000),
  B8_1BIT(10100000),
  B8_1BIT(10100000),
  B8_1BIT(11100000),
  
  //1
  B8_1BIT(01000000),
  B8_1BIT(01000000),
  B8_1BIT(01000000),
  B8_1BIT(01000000),
  B8_1BIT(01000000),

//2
  B8_1BIT(11100000),
  B8_1BIT(00100000),
  B8_1BIT(11100000),
  B8_1BIT(10000000),
  B8_1BIT(11100000),

//3
  B8_1BIT(11100000),
  B8_1BIT(00100000),
  B8_1BIT(11100000),
  B8_1BIT(00100000),
  B8_1BIT(11100000),

//4
  B8_1BIT(10100000),
  B8_1BIT(10100000),
  B8_1BIT(11100000),
  B8_1BIT(00100000),
  B8_1BIT(00100000),

//5
  B8_1BIT(11100000),
  B8_1BIT(10000000),
  B8_1BIT(11100000),
  B8_1BIT(00100000),
  B8_1BIT(11100000),

//6
  B8_1BIT(11100000),
  B8_1BIT(10000000),
  B8_1BIT(11100000),
  B8_1BIT(10100000),
  B8_1BIT(11100000),

//7
  B8_1BIT(11100000),
  B8_1BIT(00100000),
  B8_1BIT(01000000),
  B8_1BIT(01000000),
  B8_1BIT(01000000),

//8
  B8_1BIT(11100000),
  B8_1BIT(10100000),
  B8_1BIT(11100000),
  B8_1BIT(10100000),
  B8_1BIT(11100000),

//9
  B8_1BIT(11100000),
  B8_1BIT(10100000),
  B8_1BIT(11100000),
  B8_1BIT(00100000),
  B8_1BIT(11100000),
  
};

const uint8_t DotData[] = 
{

  //:
  B8_1BIT(00000000),
  B8_1BIT(01000000),
  B8_1BIT(00000000),
  B8_1BIT(01000000),
  B8_1BIT(00000000),
};

int dropWidth = 1;
int dropHeight = 2;
const uint8_t WaterDropData[] = 
{

  //:
  B8_2BIT(10000000),
  B8_2BIT(20000000),
  B8_2BIT(30000000),
  B8_2BIT(00000000),
  B8_2BIT(00000000),
};

const uint8_t WaterDropMask[] = 
{

  //:
  B8_1BIT(10000000),
  B8_1BIT(10000000),
  B8_1BIT(10000000),
  B8_1BIT(00000000),
  B8_1BIT(00000000),
};


const char* ssid = "VM4534915";
const char* password = "7phfbdnQmtfw";

#define LED_PIN        D2
#define COLOR_ORDER    GRB
#define CHIPSET        WS2812B

#define MATRIX_WIDTH   19
#define MATRIX_HEIGHT  14
#define MATRIX_TYPE    HORIZONTAL_ZIGZAG_MATRIX

cLEDMatrix<MATRIX_WIDTH, MATRIX_HEIGHT, MATRIX_TYPE> leds;

struct CRGB NumberColor[1] = { CRGB(0, 51, 0) };

struct CRGB DotNumberColor[1] = { CRGB(0, 51, 0) };

struct CRGB WaterDropColor[3] = { CRGB(0, 0, 100), CRGB(0, 0, 200), CRGB(0, 0, 255) };

struct CRGB SnowDropColor[3] = { CRGB(255, 255, 255), CRGB(0, 0, 0), CRGB(0, 0, 0) };


cLEDSprites Sprites(&leds);
cSprite Char1(numWidth, numHeight, NumberData, 9, _1BIT, NumberColor, NumberData);
cSprite Char2(numWidth, numHeight, NumberData, 9, _1BIT, NumberColor, NumberData);
cSprite Char3(numWidth, numHeight, NumberData, 9, _1BIT, NumberColor, NumberData);
cSprite Char4(numWidth, numHeight, NumberData, 9, _1BIT, NumberColor, NumberData);

int totalDrops = 36;
cSprite Droplet[36];
cSprite Snow[36];
cSprite FastDrops[36];

int DropLocations[][36] = {
{15,0},
{1,2},
{4,4},
{13,4},
{16,6},
{5,8},
{10,8},
{2,9},
{9,10},
{16,12},
{15,14},
{3,14},
{11,16},
{1,17},
{7,17},
{7,20},
{15,22},
{1,23},
{8,25},
{4,26},
{13,28},
{0,30},
{10,32},
{5,34},
{12,35},
{18,35},
{5,38},
{12,39},
{3,41},
{9,42},
{13,44},
{6,46},
{17,48},
{13,49},
{6,51},
{16,52},
{2,53}
};
                    

cSprite Dots(3, 5, DotData, 1, _1BIT, DotNumberColor, DotData);


void setup() {
  Serial.begin(115200);
  Serial.setDebugOutput(true);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("\nConnecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
  }

  configTime(3600, 0, "uk.pool.ntp.org", "ntp2a.mcc.ac.uk");
  Serial.println("\nWaiting for time");
  while (!time(nullptr)) {
    Serial.print(".");
    delay(1000);
  }
  Serial.println("..");
  
  FastLED.addLeds<CHIPSET, LED_PIN, COLOR_ORDER>(leds[0], leds.Size());
  FastLED.setBrightness(5);
  FastLED.clear(true);
  delay(500);
  FastLED.showColor(CRGB::Red);
  delay(1000);
  FastLED.show();

  Char1.SetPositionFrameMotionOptions(1/*X*/, 5/*Y*/, 0/*Frame*/, 0/*FrameRate*/, 0/*XChange*/, 0/*XRate*/, 0/*YChange*/, 0/*YRate*/, 0);
  Sprites.AddSprite(&Char1);

  Char2.SetPositionFrameMotionOptions(5/*X*/, 5/*Y*/, 0/*Frame*/, 0/*FrameRate*/, 0/*XChange*/, 0/*XRate*/, 0/*YChange*/, 0/*YRate*/, 0);
  Sprites.AddSprite(&Char2);

  Dots.SetPositionFrameMotionOptions(8/*X*/, 5/*Y*/, 0/*Frame*/, 0/*FrameRate*/, 0/*XChange*/, 0/*XRate*/, 0/*YChange*/, 0/*YRate*/, 0);
  Sprites.AddSprite(&Dots);
 
  Char3.SetPositionFrameMotionOptions(11/*X*/, 5/*Y*/, 0/*Frame*/, 0/*FrameRate*/, 0/*XChange*/, 0/*XRate*/, 0/*YChange*/, 0/*YRate*/, 0);
  Sprites.AddSprite(&Char3);

  Char4.SetPositionFrameMotionOptions(15/*X*/, 5/*Y*/, 0/*Frame*/, 0/*FrameRate*/, 0/*XChange*/, 0/*XRate*/, 0/*YChange*/, 0/*YRate*/, 0);
  Sprites.AddSprite(&Char4);
  
  for (int i=0; i<totalDrops; i++) {
    Droplet[i].Setup(dropWidth, dropHeight, WaterDropData, 0, _2BIT, WaterDropColor, WaterDropMask);
    Droplet[i].SetPositionFrameMotionOptions(DropLocations[i][0], DropLocations[i][1], 0, 0, 0, 3, -1, 2, 0);
    Sprites.AddSprite(&Droplet[i]);
  }
  
}

int totalFrames = 0;
int rainType = 0;

void loop()
{
  

  time_t now;
  struct tm * timeinfo;
  time(&now);
  timeinfo = localtime(&now);  
  Serial.println(timeinfo->tm_hour);
  
  int hours = timeinfo->tm_hour;
  Serial.println(hours);
  if (hours >= 10) {
    int hour2 = hours % 10;
    int hour1 = (hours / 10) % 10;
    Char1.SetFrame(hour1, 0);
    Char2.SetFrame(hour2, 0);    
  } else {
    Char1.SetFrame(0, 0);
    Char2.SetFrame(hours, 0);
  }

  int minutes = timeinfo->tm_min +1;
  Serial.println(minutes);
  if (minutes >= 10) {
    int minutes2 = minutes % 10;
    int minutes1 = (minutes / 10) % 10;
    Char3.SetFrame(minutes1, 0);
    Char4.SetFrame(minutes2, 0);    
  } else {
    Char3.SetFrame(0, 0);
    Char4.SetFrame(minutes, 0);
  }
   
  FastLED.clear();
  Sprites.UpdateSprites();
    
  Sprites.ChangePriority(&Char1, SPR_FRONT);
  Sprites.ChangePriority(&Char2, SPR_FRONT);
  Sprites.ChangePriority(&Char3, SPR_FRONT);
  Sprites.ChangePriority(&Char4, SPR_FRONT);

Serial.println(totalFrames);
   if (totalFrames == 100) {
    for (int i=0; i<totalDrops; i++) {
      FastDrops[i].Setup(dropWidth, dropHeight, WaterDropData, 0, _2BIT, WaterDropColor, WaterDropMask);
      FastDrops[i].SetPositionFrameMotionOptions(Droplet[i].m_X, Droplet[i].m_Y, 0, 0, 0, 1, -1, 1, 0);
      Sprites.RemoveSprite(&Droplet[i]);
      Sprites.AddSprite(&FastDrops[i]);
      rainType = 2;
    }
   } else if (totalFrames == 200) {
    for (int i=0; i<totalDrops; i++) {
      Snow[i].Setup(dropWidth, dropHeight, WaterDropData, 0, _2BIT, SnowDropColor, WaterDropMask);
      Snow[i].SetPositionFrameMotionOptions(FastDrops[i].m_X, FastDrops[i].m_Y, 0, 0, 0, 1, -1, 4, 0);
      Sprites.RemoveSprite(&FastDrops[i]);
      Sprites.AddSprite(&Snow[i]);
      rainType = 1;
    }
   }


  for (int i=0; i<totalDrops; i++)
  {
    if (rainType == 0) {
     if (Droplet[i].m_Y < -3) {
       Droplet[i].m_Y = MATRIX_HEIGHT+40;
     }
    } else if (rainType == 1) {
        if (Snow[i].m_Y < -3) {
         Snow[i].m_Y = MATRIX_HEIGHT+40;
       }
    } else {
      if (FastDrops[i].m_Y < -3) {
         FastDrops[i].m_Y = MATRIX_HEIGHT+40;
       }
    }
  }
  
  Sprites.RenderSprites();
  FastLED.show();
  delay(30);
  totalFrames++;
  
}
