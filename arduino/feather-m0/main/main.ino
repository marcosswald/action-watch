#include <Adafruit_GFX.h>
#include <Adafruit_SharpMem.h>

// any pins can be used
#define SHARP_SCK  12
#define SHARP_MOSI 11
#define SHARP_SS   10

// Set the size of the display here, e.g. 144x168!
Adafruit_SharpMem display(SHARP_SCK, SHARP_MOSI, SHARP_SS, 144, 168);
// The currently-available SHARP Memory Display (144x168 pixels)
// requires > 4K of microcontroller RAM

#define BLACK 0
#define WHITE 1

#include <Adafruit_GPS.h>

// what's the name of the hardware serial port?
#define GPSSerial Serial1

// Connect to the GPS on the hardware port
Adafruit_GPS GPS(&GPSSerial);
     
// Set GPSECHO to 'false' to turn off echoing the GPS data to the Serial console
// Set to 'true' if you want to debug and listen to the raw GPS sentences
#define GPSECHO true

uint32_t timer = millis();

void setup() {
  Serial.begin(115200);
  Serial.println("Adafruit GPS library basic test!");
     
  // 9600 NMEA is the default baud rate for Adafruit MTK GPS's- some use 4800
  GPS.begin(9600);
  // uncomment this line to turn on RMC (recommended minimum) and GGA (fix data) including altitude
  GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);
  // uncomment this line to turn on only the "minimum recommended" data
  //GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCONLY);
  // For parsing data, we don't suggest using anything but either RMC only or RMC+GGA since
  // the parser doesn't care about other sentences at this time
  // Set the update rate
  GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ); // 1 Hz update rate
  // For the parsing code to work nicely and have time to sort thru the data, and
  // print it out we don't suggest using anything higher than 1 Hz
     
  // Request updates on antenna status, comment out to keep quiet
  GPS.sendCommand(PGCMD_ANTENNA);

  delay(1000);
  
  // Ask for firmware version
  GPSSerial.println(PMTK_Q_RELEASE);

  // start & clear the display
  display.begin();
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(BLACK);
  display.setCursor(0,0);
  display.println("ActionWatch 1.0");
  display.refresh();

  // led
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  // read data from the GPS in the 'main loop'
  char c = GPS.read();
  // if you want to debug, this is a good time to do it!
  if (GPSECHO)
    if (c) Serial.print(c);
  // if a sentence is received, we can check the checksum, parse it...
  if (GPS.newNMEAreceived()) {
    if (!GPS.parse(GPS.lastNMEA())) // this also sets the newNMEAreceived() flag to false
      return; // we can fail to parse a sentence in which case we should just wait for another
  }
  // if millis() or timer wraps around, we'll just reset it
  if (timer > millis()) timer = millis();
     
  // approximately every 2 seconds or so, print out the current stats
  if (millis() - timer > 1000) {
    timer = millis(); // reset the timer
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(BLACK);
    display.setCursor(0,0);
    display.print("\nTime: ");
    display.print(GPS.hour, DEC); display.print(':');
    display.print(GPS.minute, DEC); display.print(':');
    display.print(GPS.seconds, DEC); display.print('.');
    display.println(GPS.milliseconds);
    display.print("Date: ");
    display.print(GPS.day, DEC); display.print('/');
    display.print(GPS.month, DEC); display.print("/20");
    display.println(GPS.year, DEC);
    display.print("Fix: "); display.print((int)GPS.fix);
    display.print(" quality: "); display.println((int)GPS.fixquality);
    if (GPS.fix) {
      display.print("Location: ");
      display.print(GPS.latitude, 4); display.print(GPS.lat);
      display.print(", ");
      display.print(GPS.longitude, 4); display.println(GPS.lon);
      display.print("Speed (knots): "); display.println(GPS.speed);
      display.print("Angle: "); display.println(GPS.angle);
      display.print("Altitude: "); display.println(GPS.altitude);
      display.print("Satellites: "); display.println((int)GPS.satellites);
    }
    display.refresh();
  }
}
