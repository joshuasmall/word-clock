#include <Wire.h>
#include <RTClib.h>

#include <Adafruit_NeoPixel.h>

#define PIN 4
#define LED_COUNT 130

RTC_DS1307 RTC;

const int hours[12][2] = {
    {65, 67}, // One
    {39, 41}, // Two
    {73, 77}, // Three
    {55, 58}, // Four
    {26, 29}, // Five
    {13, 15}, //Six
    {30, 34}, // Seven
    {46, 50}, // Eight
    {35, 38}, // Nine
    {23, 25}, // Ten
    {59, 64}, // Eleven
    {7, 12}, // Twelve
};

const int it[2] = {117, 118};
const int is[2] = {120, 121};

const int ten[2] = {123, 125};
const int half[2] = {126, 129};
const int quarter[2] = {110, 116};
const int twenty[2] = {104 , 109};
const int five[2] = {91, 94};
const int to[2] = {79, 80};
const int past[2] = {81, 84};
const int minutes[2] = {96, 102};
const int oclock[2] = {0, 5};

int oldHour = 0;

// Create an instance of the Adafruit_NeoPixel class called "leds".
// That'll be what we refer to from here on...
Adafruit_NeoPixel leds = Adafruit_NeoPixel(LED_COUNT, PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  Serial.begin(9600);
  
  turnOnOff(it, true);
  turnOnOff(is, true);
  
  leds.begin();
  leds.setBrightness(50);
  
  Wire.begin();
  RTC.begin();
  
  if (! RTC.isrunning())
  {
    Serial.println("RTC is NOT running!");
    // following line sets the RTC to the date & time this sketch was compiled
    RTC.adjust(DateTime(__DATE__, __TIME__));
  }
}

void loop() {
  
  DateTime now = RTC.now(); 
  Serial.print(now.day(), DEC);
  Serial.print('/');
  Serial.print(now.month(), DEC);
  Serial.print('/');
  Serial.print(now.year(), DEC);
  Serial.print(' ');
  Serial.print(now.hour(), DEC);
  Serial.print(':');
  Serial.print(now.minute(), DEC);
  Serial.print(':');
  Serial.print(now.second(), DEC);
  Serial.println();    
  Serial.println();    

  int minute = now.minute();
  int hour = now.hour();

  if (hour > 12)
    hour-= 12;

  if (hour == 0)
    hour = 12;

  // Minus so that the index matches up
  hour--;

  // Reset the words
  turnOnOff(to, false);
  turnOnOff(past, false);
  turnOnOff(five, false);
  turnOnOff(ten, false);
  turnOnOff(quarter, false);
  turnOnOff(twenty, false);
  turnOnOff(minutes, true);
  turnOnOff(oclock, false);
  
  if (minute >= 30)
  {
    // Show the next hour as it is not to
    hour++;
    turnOnOff(to, true);
  }
  else if (minute >= 5)
  {
    turnOnOff(past, true);
  }
  else
  {
    turnOnOff(minutes, false);
    turnOnOff(oclock, true);
  }
  
  if (minute >= 5 && minute < 10 || minute >= 55)
  {
      turnOnOff(five, true);
  }
  else if(minute >= 10 && minute < 15 || minute >= 50)
  {
      turnOnOff(ten, true);
  }
  else if (minute >= 15 && minute < 20 || minute >= 45)
  {
      turnOnOff(quarter, true);
      turnOnOff(minutes, false);
  }
  else if(minute >= 20 && minute < 25 || minute >= 40)
  {
  
      turnOnOff(twenty, true);
  }
  else if(minute >= 25 && minute < 30 || minute >= 35)
  {
      turnOnOff(twenty, true);
      turnOnOff(five, true);
  }
  
  if (hour != oldHour)
  {
    turnOnOff(hours[oldHour], false);
    turnOnOff(hours[hour], true);
    oldHour = hour;

    Serial.print('Hour: ');
    Serial.print(hour);
    Serial.println();
  }
  leds.show();
  delay(5000);
}

/**
 * Turns a set of leds on/off
 * 
 * @param int[] theWord Array containing the start and end index of the leds of the word to turn on/off
 * @param boolean on If we are turning the word on/off
 */
void turnOnOff(int theWord[], boolean on)
{   
  for (int i = theWord[0]; i <= theWord[1]; i++)
  leds.setPixelColor(i, on ? 0xFFFFFF : 0x000000); 
}

