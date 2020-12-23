/**************************************************************************
 * Project: Internet Clock
 * Version: 0.1
 * Arthor: Suwat Saisema
 * Email: suwat.sai@hotmail.com
 **************************************************************************/

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <ESP8266WiFi.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <Time.h>
#include <TimeLib.h>
#include <Timezone.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     1 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// SSID
const char *ssid     = "naisema_2.4G";
const char *password = "0866145241";

// NTP
const char *ntp_server  = "2.th.pool.ntp.org";
const int ntp_interval  = 60000; // miliseconds
const long ntp_offset = 25200; // Bangkok GMT+7 = 7 * 60 * 60 = 25200

// Date and Time
String date;
String tm;
String m; // minute
const char *daysOfTheWeek[]   = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
const char *monthsOfTheYear[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"} ;
const char *ampm[]            = {"AM", "PM"};

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, ntp_server, ntp_offset, ntp_interval);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  Wire.begin(0, 2); //(SDA = GPIO0, SCL = GPIO2)

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  // Show initial display buffer contents on the screen --
  // the library initializes this with an Adafruit splash screen.
  display.display();
  delay(2000); // Pause for 2 seconds

  // Clear the buffer
  display.clearDisplay();

  // Draw a single pixel in white
  display.drawPixel(10, 10, SSD1306_WHITE);
  
  // Show the display buffer on the screen. You MUST call display() after
  // drawing commands to make them visible on screen!
  display.display();
  delay(2000);

//  showTime();

  // WIFI Connected
  WiFi.begin(ssid, password);

  while ( WiFi.status() != WL_CONNECTED ) {
    delay ( 500 );
    Serial.print ( "." );
  }
  
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0,10);
  display.println(F("WiFi Connected..."));
  display.setCursor(0,20);
  display.print(F("IP: "));
  display.println(WiFi.localIP());
  display.display();
  delay(2000);

  timeClient.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
  // Clear String
  date  = "";
  tm    = "";
  m     = "";

  timeClient.update();
  unsigned long epochTime =  timeClient.getEpochTime(); 

  time_t local, utc;
  utc = epochTime;

//  TimeChangeRule usEDT = {"EDT", Second, Sun, Mar, 2, -300};  //UTC - 5 hours - change this as needed
//  TimeChangeRule usEST = {"EST", First, Sun, Nov, 2, -360};   //UTC - 6 hours - change this as needed
  TimeChangeRule utcRule = {"UTC", Last, Sun, Mar, 1, 0};
  Timezone UTC(utcRule);
//  Timezone usEastern(usEDT, usEST);
//  local = usEastern.toLocal(utc);
  local = UTC.toLocal(utc);

//  date += daysOfTheWeek[weekday(local)-1];
//  date += ", ";
//  date += monthsOfTheYear[month(local)-1];
//  date += " ";
//  date += day(local);
//  date += ", ";
//  date += year(local);
//  tm += ampm[isPM(local)];
//  tm += " ";
//  tm += hourFormat12(local);
//  tm += ":";
//  if(minute(local) < 10)  // add a zero if minute is under 10
//    tm += "0";
//  tm += minute(local);
  if(minute(local) < 10)
    m +=  "0";
  m += minute(local);
  
  // Display the date and time
  Serial.println("");
  Serial.print("Local date: ");
  Serial.print(date);
  Serial.println("");
  Serial.print("Local time: ");
  Serial.print(tm);
  
  // time
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0,5);
  display.print(ampm[isPM(local)]);
  display.setTextSize(3);
  display.setCursor(0,15);
  display.print(hourFormat12(local));
  display.print(":");
  display.println(m);

  // date
  display.setTextSize(1);
  display.setCursor(0,47);
  display.print(daysOfTheWeek[weekday(local)-1]);
  display.setCursor(0,57);
  display.print(monthsOfTheYear[month(local)-1]);
  display.print(" ");
  display.print(day(local));
  display.print(", ");
  display.print(year(local));
  display.display();

//  showTime();
//  display.clearDisplay();
//  display.setTextColor(SSD1306_WHITE);
//  display.setCursor(10,10);
//  display.print(timeClient.getHours());
//  display.print(F(":"));
//  display.print(timeClient.getMinutes());
//  display.print(F(":"));
//  display.println(timeClient.getSeconds());
//  display.display();
  
  delay(1000);
}

//void getTime() {
//  timeClient.update();
//
//  Serial.print(daysOfTheWeek[timeClient.getDay()]);
//  Serial.print(", ");
//  Serial.print(timeClient.getHours());
//  Serial.print(":");
//  Serial.print(timeClient.getMinutes());
//  Serial.print(":");
//  Serial.println(timeClient.getSeconds());
//
//  delay(1000);
//}

void showTime(void) {
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(10,10);
  display.print(timeClient.getHours());
  display.print(F(":"));
  display.print(timeClient.getMinutes());
  display.print(F(":"));
  display.println(timeClient.getSeconds());
  display.display();
}
