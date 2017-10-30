#include <ArduinoJson.h>

#define FASTLED_ALLOW_INTERRUPTS 0
#include <FastLED.h>

#include <LEDMatrix.h>
#include <LEDSprites.h>
#include <ESP8266WiFi.h>
#include <time.h>
#include <Timer.h>

#include <ESP8266HTTPClient.h>

Timer t;

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
int dropHeight = 3;
const uint8_t WaterDropData[] = 
{

  //:
  B8_2BIT(10000000),
  B8_2BIT(20000000),
  B8_2BIT(30000000),
};

const uint8_t WaterDropMask[] = 
{

  //:
  B8_1BIT(10000000),
  B8_1BIT(10000000),
  B8_1BIT(10000000),
};

int snowWidth = 1;
int snowHeight = 1;
const uint8_t SnowDropData[] = 
{
  B8_1BIT(10000000),
};

const uint8_t SnowDropMask[] = 
{
  B8_1BIT(10000000),
};

int sunHeight = 13;
int sunWidth = 13;

const uint8_t SunData[] = 
{
  B8_1BIT(10000010),B8_1BIT(00001000),
  B8_1BIT(01000010),B8_1BIT(00010000),
  B8_1BIT(00000000),B8_1BIT(00000000),
  B8_1BIT(00011111),B8_1BIT(11000000),
  B8_1BIT(00011111),B8_1BIT(11000000),
  B8_1BIT(00011111),B8_1BIT(11000000),
  B8_1BIT(11011111),B8_1BIT(11011000),
  B8_1BIT(00011111),B8_1BIT(11000000),
  B8_1BIT(00011111),B8_1BIT(11000000),
  B8_1BIT(00011111),B8_1BIT(11000000),
  B8_1BIT(00000000),B8_1BIT(00000000),
  B8_1BIT(01000010),B8_1BIT(00010000),
  B8_1BIT(10000010),B8_1BIT(00001000)
};

const uint8_t BlankRowData[] = 
{
    B8_1BIT(11111111),B8_1BIT(11111111),B8_1BIT(10000000),
    B8_1BIT(11111111),B8_1BIT(11111111),B8_1BIT(10000000),
    B8_1BIT(11111111),B8_1BIT(11111111),B8_1BIT(10000000),
    B8_1BIT(11111111),B8_1BIT(11111111),B8_1BIT(10000000),
    B8_1BIT(11111111),B8_1BIT(11111111),B8_1BIT(10000000),
};

int shortMistsTotal = 4;
cSprite ShortMists[4];

int MistShortLocations[][3] = {
{1,1},
{14,1},
{15,10},
{17,13},
};

const uint8_t MistShort[] = 
{
  B8_1BIT(11000000),
};

int midMistsTotal = 7;
cSprite MidMists[7];

int MistMidLocations[][6] = {
{3,10},
{6,1},
{7,13},
{10,11},
{10,3},
{16,2},
{11,9},
};

const uint8_t MistMid[] = 
{
  B8_1BIT(11100000),
};

int longMistsTotal = 4;
cSprite LongMists[4];

int MistLongLocations[][3] = {
{-4,3},
{-1,12},
{8,2},
{12,12},
};

const uint8_t MistLong[] = 
{
  B8_1BIT(11110000),
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

struct CRGB NumberColor[1] = { CRGB(255, 0, 0) };

struct CRGB SunColor[1] = { CRGB(255, 255, 0) };

struct CRGB ShortMistColor[1] = { CRGB(230, 230, 230) };
struct CRGB MidMistColor[1] = { CRGB(198, 198, 198) };
struct CRGB LongMistColor[1] = { CRGB(  115, 115, 115) };

struct CRGB DotNumberColor[1] = { CRGB(255, 0, 0) };

struct CRGB BlankColor[1] = { CRGB(0, 0, 0) };

struct CRGB WaterDropColor[3] = { CRGB(0, 0, 100), CRGB(0, 0, 200), CRGB(0, 0, 255) };

struct CRGB SnowDropColor[1] = { CRGB(255, 255, 255) };


cLEDSprites Sprites(&leds);
cSprite Char1(numWidth, numHeight, NumberData, 9, _1BIT, NumberColor, NumberData);
cSprite Char2(numWidth, numHeight, NumberData, 9, _1BIT, NumberColor, NumberData);
cSprite Char3(numWidth, numHeight, NumberData, 9, _1BIT, NumberColor, NumberData);
cSprite Char4(numWidth, numHeight, NumberData, 9, _1BIT, NumberColor, NumberData);

cSprite BlankRow(17, 5, BlankRowData, 9, _1BIT, BlankColor, BlankRowData);

cSprite Sun(sunWidth, sunHeight, SunData, 0, _1BIT, SunColor, SunData);

int totalDrops = 28;
cSprite Droplet[28];
cSprite Snow[28];
cSprite FastDrops[28];
cSprite SlowDroplet[28];

int DropLocations[][28] = {
  {0,0},
  {0,7},
  {0,14},
  {0,21},
  {3,4},
  {3,11},
  {3,18},
  {3,25},
  {6,0},
  {6,7},
  {6,14},
  {6,21},
  {9,4},
  {9,11},
  {9,18},
  {9,25},
  {12,0},
  {12,7},
  {12,14},
  {12,21},
  {15,4},
  {15,11},
  {15,18},
  {15,25},
  {18,0},
  {18,7},
  {18,14},
  {18,21},
};

//int totalDrops = 36;
//cSprite Droplet[36];
//cSprite Snow[36];
//cSprite FastDrops[36];

//int DropLocations[][36] = {
//{15,0},
//{1,2},
//{4,4},
//{13,4},
//{16,6},
//{5,8},
//{10,8},
//{2,9},
//{9,10},
//{16,12},
//{15,14},
//{3,14},
//{11,16},
//{1,17},
//{7,17},
//{7,20},
//{15,22},
//{1,23},
//{8,25},
//{4,26},
//{13,28},
//{0,30},
//{10,32},
//{5,34},
//{12,35},
//{18,35},
//{5,38},
//{12,39},
//{3,41},
//{9,42},
//{13,44},
//{6,46},
//{17,48},
//{13,49},
//{6,51},
//{16,52},
//{2,53},
//{17,5},
//{17,28},
//{17,17},
//{17,10},
//{16,23},
//{16,43},
//};

int weatherCode = 0;
                    

cSprite Dots(3, 5, DotData, 1, _1BIT, DotNumberColor, DotData);

void getWeather() {
  if (WiFi.status() == WL_CONNECTED) { //Check WiFi connection status
 
    HTTPClient http;  //Declare an object of class HTTPClient
 
    http.begin("http://api.apixu.com/v1/forecast.json?key=KEYKEYKEYKEY&q=PR2&days=1");
    int httpCode = http.GET();                                                                  //Send the request

    if (httpCode > 0) { //Check the returning code
      String payload = http.getString();   //Get the request response payload
      Serial.println(payload);                     //Print the response payload

      const size_t bufferSize = JSON_ARRAY_SIZE(1) + JSON_ARRAY_SIZE(24) + JSON_OBJECT_SIZE(0) + 53*JSON_OBJECT_SIZE(1) + 2*JSON_OBJECT_SIZE(3) + JSON_OBJECT_SIZE(8) + 790;
      DynamicJsonBuffer jsonBuffer(bufferSize);

      JsonObject& root = jsonBuffer.parseObject(payload);
      String weatherInfo = "";
      weatherInfo = (char)root["forecast"]["forecastday"][0]["day"]["condition"]["code"];
      weatherCode = weatherInfo.toInt();
    }
 
    http.end();   //Close connection
 
  }
}

int maxMist = 0;

void setup() {

  maxMist = _max(shortMistsTotal, midMistsTotal);
  maxMist = _max(maxMist, longMistsTotal);


  ESP.wdtDisable();
  ESP.wdtEnable(WDTO_8S);
  ESP.getFreeHeap();
  Serial.println(ESP.getFreeHeap());
    
  Serial.begin(115200);
  Serial.setDebugOutput(true);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("\nConnecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
  }

  configTime(0, 0, "uk.pool.ntp.org", "ntp2a.mcc.ac.uk");
  Serial.println("\nWaiting for time");
  while (!time(nullptr)) {
    Serial.print(".");
    delay(1000);
  }
  Serial.println("Weather time");

  getWeather();
//  t.every(3600000, getWeather);
  t.every(100000, getWeather);
  
  FastLED.addLeds<CHIPSET, LED_PIN, COLOR_ORDER>(leds[0], leds.Size());
  FastLED.setBrightness(5);
  FastLED.clear(true);
  delay(500);
  FastLED.showColor(CRGB::Red);
  delay(5000);
  FastLED.show();

  Char1.SetPositionFrameMotionOptions(1/*X*/, 5/*Y*/, 0/*Frame*/, 0/*FrameRate*/, 0/*XChange*/, 0/*XRate*/, 0/*YChange*/, 0/*YRate*/, 0);
  Char2.SetPositionFrameMotionOptions(5/*X*/, 5/*Y*/, 0/*Frame*/, 0/*FrameRate*/, 0/*XChange*/, 0/*XRate*/, 0/*YChange*/, 0/*YRate*/, 0);
  Dots.SetPositionFrameMotionOptions(8/*X*/, 5/*Y*/, 0/*Frame*/, 0/*FrameRate*/, 0/*XChange*/, 0/*XRate*/, 0/*YChange*/, 0/*YRate*/, 0);
  Char3.SetPositionFrameMotionOptions(11/*X*/, 5/*Y*/, 0/*Frame*/, 0/*FrameRate*/, 0/*XChange*/, 0/*XRate*/, 0/*YChange*/, 0/*YRate*/, 0);
  Char4.SetPositionFrameMotionOptions(15/*X*/, 5/*Y*/, 0/*Frame*/, 0/*FrameRate*/, 0/*XChange*/, 0/*XRate*/, 0/*YChange*/, 0/*YRate*/, 0);

  BlankRow.SetPositionFrameMotionOptions(1/*X*/, 5/*Y*/, 0/*Frame*/, 0/*FrameRate*/, 0/*XChange*/, 0/*XRate*/, 0/*YChange*/, 0/*YRate*/, 0);

  for (int i=0; i<shortMistsTotal; i++) {
    ShortMists[i].Setup(2, 1, MistShort, 0, _1BIT, ShortMistColor, MistShort);
    ShortMists[i].SetPositionFrameMotionOptions(MistShortLocations[i][0], MistShortLocations[i][1], 0, 0, 1, 3, 0, 0, 0);
  }

  for (int i=0; i<midMistsTotal; i++) {
    MidMists[i].Setup(3, 1, MistMid, 0, _1BIT, MidMistColor, MistMid);
    MidMists[i].SetPositionFrameMotionOptions(MistMidLocations[i][0], MistMidLocations[i][1], 0, 0, 1, 4, 0, 0, 0);
  }

  for (int i=0; i<longMistsTotal; i++) {
    LongMists[i].Setup(4, 1, MistLong, 0, _1BIT, LongMistColor, MistLong);
    LongMists[i].SetPositionFrameMotionOptions(MistLongLocations[i][0], MistLongLocations[i][1], 0, 0, 1, 5, 0, 0, 0);
  }

  Sun.SetPositionFrameMotionOptions(3/*X*/, 0/*Y*/, 0/*Frame*/, 0/*FrameRate*/, 0/*XChange*/, 0/*XRate*/, 0/*YChange*/, 0/*YRate*/, 0);

  for (int i=0; i<totalDrops; i++) {
    SlowDroplet[i].Setup(dropWidth, dropHeight, WaterDropData, 0, _2BIT, WaterDropColor, WaterDropMask);
    SlowDroplet[i].SetPositionFrameMotionOptions(DropLocations[i][0], DropLocations[i][1], 0, 0, 0, 0, -1, 3, 0);
    
    Droplet[i].Setup(dropWidth, dropHeight, WaterDropData, 0, _2BIT, WaterDropColor, WaterDropMask);
    Droplet[i].SetPositionFrameMotionOptions(DropLocations[i][0], DropLocations[i][1], 0, 0, 0, 0, -1, 3, 0);

    FastDrops[i].Setup(dropWidth, dropHeight, WaterDropData, 0, _2BIT, WaterDropColor, WaterDropMask);
    FastDrops[i].SetPositionFrameMotionOptions(DropLocations[i][0], DropLocations[i][1], 0, 0, 0, 0, -1, 2, 0);

    Snow[i].Setup(snowWidth, snowHeight, SnowDropData, 0, _1BIT, SnowDropColor, SnowDropMask);
    Snow[i].SetPositionFrameMotionOptions(DropLocations[i][0], DropLocations[i][1], 0, 0, 0, 0, -1, 5, 0);
  }
 
  
}

int totalFrames = 0;
int rainType = 0;

void loop()
{
  time_t now;
  t.update();
  struct tm * timeinfo;
  time(&now);
  timeinfo = localtime(&now);  
  
  int hours = timeinfo->tm_hour;
  if (hours >= 10) {
    int hour2 = hours % 10;
    int hour1 = (hours / 10) % 10;
    Char1.SetFrame(hour1, 0);
    Char2.SetFrame(hour2, 0);    
  } else {
    Char1.SetFrame(0, 0);
    Char2.SetFrame(hours, 0);
  }

  int minutes = timeinfo->tm_min;
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

  Sprites.ChangePriority(&BlankRow, SPR_FRONT);
  Sprites.ChangePriority(&Char1, SPR_FRONT);
  Sprites.ChangePriority(&Char2, SPR_FRONT);
  Sprites.ChangePriority(&Char3, SPR_FRONT);
  Sprites.ChangePriority(&Char4, SPR_FRONT);
  Sprites.ChangePriority(&Dots, SPR_FRONT);

  for (int i=0; i<shortMistsTotal; i++)
  {
     if (ShortMists[i].m_X > MATRIX_WIDTH) {
       ShortMists[i].m_X = -2;
     }
  }

  for (int i=0; i<midMistsTotal; i++)
  {
     if (MidMists[i].m_X > MATRIX_WIDTH) {
       MidMists[i].m_X = -3;
     }
  }

    for (int i=0; i<longMistsTotal; i++)
  {
     if (LongMists[i].m_X > MATRIX_WIDTH) {
       LongMists[i].m_X = -4;
     }
  }


  for (int i=0; i<totalDrops; i++)
  {
     if (Droplet[i].m_Y == -3) {
       Droplet[i].m_Y = MATRIX_HEIGHT+4;
     }

    if (SlowDroplet[i].m_Y == -3) {
       SlowDroplet[i].m_Y = MATRIX_HEIGHT+4;
     }
     
    if (Snow[i].m_Y < -3) {
       Snow[i].m_Y = MATRIX_HEIGHT+4;
     }
       
    if (FastDrops[i].m_Y < -3) {
       FastDrops[i].m_Y = MATRIX_HEIGHT+4;
     }
       
//    if (Droplet[i].m_Y = -3) {
//       Droplet[i].m_Y = MATRIX_HEIGHT;
//     }
//     
//    if (Snow[i].m_Y = -3) {
//         Snow[i].m_Y = MATRIX_HEIGHT;
//       }
//       
//      if (FastDrops[i].m_Y = -3) {
//         FastDrops[i].m_Y = MATRIX_HEIGHT;
//       }
  }

  if (totalFrames % 100 == 0) {
      Sprites.RemoveAllSprites();
      Sprites.AddSprite(&BlankRow);
      Sprites.AddSprite(&Char1);
      Sprites.AddSprite(&Char2);
      Sprites.AddSprite(&Dots);
      Sprites.AddSprite(&Char3);
      Sprites.AddSprite(&Char4);
  }

  switch (totalFrames) {
    case 0:
      for (int i=0; i<totalDrops; i++) {
        Sprites.AddSprite(&Droplet[i]);
      }
    break;
    
    case 100:
      for (int i=0; i<totalDrops; i++) {
        Sprites.AddSprite(&FastDrops[i]);
      }
    break;
    
    case 200:
      for (int i=0; i<totalDrops; i++) {
        Sprites.AddSprite(&Snow[i]);
      }
    break;

    case 300:
      for (int i=0; i<totalDrops; i++) {
        Sprites.AddSprite(&SlowDroplet[i]);
      }
    break;
    
    case 400:
      for (int i=0; i<shortMistsTotal; i++) {
        Sprites.AddSprite(&ShortMists[i]);
      }
    
      for (int i=0; i<midMistsTotal; i++) {
        Sprites.AddSprite(&MidMists[i]);
      }
    
      for (int i=0; i<longMistsTotal; i++) {
        Sprites.AddSprite(&LongMists[i]);
      }     
    break;
    
    case 500:
      totalFrames = -1;
    break;
  };

  Sprites.RenderSprites();
  FastLED.show();
  delay(30);
  totalFrames++;
  
}
