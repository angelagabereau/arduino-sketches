/*

 Climobot Sense Base v1
 Angela Gabereau
 2015-03-28
 Santa Elena, Colombia
 
 Uses the following sensors:
 -- Adafruit GPS modules using MTK3329/MTK3339 driver, http://www.adafruit.com/products/746
 
 */

#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP085_U.h>
#include <Adafruit_GPS.h>
#include <SoftwareSerial.h>
#include "DHT.h"

// Enable Pins
#define ENABLE_GPS true
#define ENABLE_BMP true
#define ENABLE_DHT true
#define ENABLE_SOIL true
#define ENABLE_SOUND true
#define ENABLE_LIGHT true
#define ENABLE_GAS true

// Sensor Pins
#define PIN_LIGHT 0 // Analog
#define PIN_GAS 1 // Analog
#define PIN_SOIL 2 // Analog
#define PIN_SOUND 3 // Analog
#define PIN_DHT 2 // Digital
#define PIN_GPS_TX 8// Digital, Software Serial
#define PIN_GPS_RX 9// Digital, Software Serial

// DHT DEFINES
#define DHTTYPE DHT22

// Config GPS
// Set GPSECHO to 'false' to turn off echoing the GPS data to the Serial console
// Set to 'true' if you want to debug and listen to the raw GPS sentences. 
#define GPSECHO  false

// Configure Interrupt for GPS
boolean usingInterrupt = false;
void useInterrupt(boolean); // Func prototype keeps Arduino 0023 happy

// Initialize the GPS module
// GPS communicates over software serial
SoftwareSerial gpsSerial(PIN_GPS_RX, PIN_GPS_TX); // RX, TX
Adafruit_GPS GPS(&gpsSerial);

// Initialize the barmetric pressure sensor
Adafruit_BMP085_Unified bmp = Adafruit_BMP085_Unified(10085);

// Initialize DHT sensor for normal 16mhz Arduino
DHT dht(PIN_DHT, DHTTYPE);


void setup()  
{
  // connect at 115200 so we can read the GPS fast enough and echo without dropping chars
  // also spit it out
  Serial.begin(115200);
  Serial.println("Climobot v1");

  if(ENABLE_GPS){
    setupGPS();
  }
  if(ENABLE_BMP){
    setupBMP();
  }
  if(ENABLE_DHT){
    setupDHT(); 
  }
}

uint32_t timer = millis();

void loop()                     // run over and over again
{
  // Serial.println("-----------------------------------");
  // Serial.println("-----------------------------------");
  if(ENABLE_GPS){
    loopGPS();
  }
  if(ENABLE_BMP){
    loopBMP();
  }
  if(ENABLE_DHT){
    loopDHT();
  }
  if(ENABLE_SOUND){
    loopSoundSensor();
  }
  if(ENABLE_SOIL){
    loopSoilSensor();
  }
  if(ENABLE_LIGHT){
    loopLightSensor();
  }
  if(ENABLE_GAS){
    loopGasSensor();
  }

}

/*
  Interrupts
 */

// Interrupt is called once a millisecond, looks for any new GPS data, and stores it
SIGNAL(TIMER0_COMPA_vect) {
  char c = GPS.read();
  // if you want to debug, this is a good time to do it!
#ifdef UDR0
  if (GPSECHO)
    if (c) UDR0 = c;  
  // writing direct to UDR0 is much much faster than Serial.print 
  // but only one character can be written at a time. 
#endif
}

void useInterrupt(boolean v) {
  if (v) {
    // Timer0 is already used for millis() - we'll just interrupt somewhere
    // in the middle and call the "Compare A" function above
    OCR0A = 0xAF;
    TIMSK0 |= _BV(OCIE0A);
    usingInterrupt = true;
  } 
  else {
    // do not call the interrupt function COMPA anymore
    TIMSK0 &= ~_BV(OCIE0A);
    usingInterrupt = false;
  }
}

void loopSoundSensor(){
  int val;
  val=analogRead(PIN_SOUND);   //connect mic sensor to Analog Sound Pin
  Serial.println();
  Serial.print("Analog Sound Sensor: ");
  Serial.println(val,DEC);//print the sound value to serial
  Serial.println();      
  delay(100);
}

void loopSoilSensor(){
  int val;
  val=analogRead(PIN_SOIL);   //connect mic sensor to Analog Sound Pin
  //Serial.println();
  Serial.print("Soil Humidity Sensor : ");
  Serial.println(val,DEC);//print the moisture value to serial
  //Serial.println();      
  delay(100);
}

void loopLightSensor(){
  int val;
  val=analogRead(PIN_LIGHT);   //connect mic sensor to Analog Sound Pin
  //Serial.println();
  Serial.print("Light Sensor : ");
  Serial.println(val,DEC);//print the moisture value to serial
  //Serial.println();      
  delay(100);
}

void loopGasSensor(){
  int val;
  val=analogRead(PIN_GAS);   //connect mic sensor to Analog Sound Pin
  //Serial.println();
  Serial.print("Gas Sensor : ");
  Serial.println(val,DEC);//print the moisture value to serial
  //Serial.println();      
  delay(100);
}

/*
  DHT Humidity Temperature Sensor
 */
void setupDHT(){
  dht.begin();  
}

void loopDHT(){
  // Wait a few seconds between measurements.
  delay(2000);

  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  // Read temperature as Celsius
  float t = dht.readTemperature();
  // Read temperature as Fahrenheit
  float f = dht.readTemperature(true);

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  // Compute heat index
  // Must send in temp in Fahrenheit!
  float hi = dht.computeHeatIndex(f, h);

  Serial.print("Humidity: "); 
  Serial.print(h);
  Serial.print(" %\t");
  Serial.print("Temperature: "); 
  Serial.print(t);
  Serial.print(" *C ");
  Serial.print(f);
  Serial.print(" *F\t");
  Serial.print("Heat index: ");
  Serial.print(hi);
  Serial.println(" *F");
}


/*
 BMP Barometric Pressure Sensor
 */
void setupBMP(){
  /* Initialise the sensor */
  if(!bmp.begin())
  {
    /* There was a problem detecting the BMP085 ... check your connections */
    Serial.print("Ooops, no BMP085 detected ... Check your wiring or I2C ADDR!");
    while(1);
  }

  /* Display some basic information on this sensor */
  displaySensorDetails();
}

void loopBMP(){
  /* Get a new sensor event */
  sensors_event_t event;
  bmp.getEvent(&event);

  /* Display the results (barometric pressure is measure in hPa) */
  if (event.pressure)
  {
    /* Display atmospheric pressue in hPa */
    Serial.print("Pressure:    ");
    Serial.print(event.pressure);
    Serial.println(" hPa");

    /* Calculating altitude with reasonable accuracy requires pressure    *
     * sea level pressure for your position at the moment the data is     *
     * converted, as well as the ambient temperature in degress           *
     * celcius.  If you don't have these values, a 'generic' value of     *
     * 1013.25 hPa can be used (defined as SENSORS_PRESSURE_SEALEVELHPA   *
     * in sensors.h), but this isn't ideal and will give variable         *
     * results from one day to the next.                                  *
     *                                                                    *
     * You can usually find the current SLP value by looking at weather   *
     * websites or from environmental information centers near any major  *
     * airport.                                                           *
     *                                                                    *
     * For example, for Paris, France you can check the current mean      *
     * pressure and sea level at: http://bit.ly/16Au8ol                   */

    /* First we get the current temperature from the BMP085 */
    float temperature;
    bmp.getTemperature(&temperature);
    Serial.print("Temperature: ");
    Serial.print(temperature);
    Serial.println(" C");

    /* Then convert the atmospheric pressure, and SLP to altitude         */
    /* Update this next line with the current SLP for better results      */
    float seaLevelPressure = SENSORS_PRESSURE_SEALEVELHPA;
    Serial.print("Altitude:    "); 
    Serial.print(bmp.pressureToAltitude(seaLevelPressure,
    event.pressure)); 
    Serial.println(" m");
    Serial.println("");
  }
  else
  {
    Serial.println("Sensor error");
  }
  delay(1000);
}
void setupGPS(){

  // 9600 NMEA is the default baud rate for Adafruit MTK GPS's- some use 4800
  GPS.begin(9600);

  // uncomment this line to turn on RMC (recommended minimum) and GGA (fix data) including altitude
  GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);
  // uncomment this line to turn on only the "minimum recommended" data
  //GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCONLY);
  // For parsing data, we don't suggest using anything but either RMC only or RMC+GGA since
  // the parser doesn't care about other sentences at this time

  // Set the update rate
  GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ);   // 1 Hz update rate
  // For the parsing code to work nicely and have time to sort thru the data, and
  // print it out we don't suggest using anything higher than 1 Hz

  // Request updates on antenna status, comment out to keep quiet
  GPS.sendCommand(PGCMD_ANTENNA);

  // the nice thing about this code is you can have a timer0 interrupt go off
  // every 1 millisecond, and read data from the GPS for you. that makes the
  // loop code a heck of a lot easier!
  useInterrupt(true);

  delay(1000);
  // Ask for firmware version
  gpsSerial.println(PMTK_Q_RELEASE);
}

void loopGPS(){
  // in case you are not using the interrupt above, you'll
  // need to 'hand query' the GPS, not suggested :(
  if (! usingInterrupt) {
    // read data from the GPS in the 'main loop'
    char c = GPS.read();
    // if you want to debug, this is a good time to do it!
    if (GPSECHO)
      if (c) Serial.print(c);
  }

  // if a sentence is received, we can check the checksum, parse it...
  if (GPS.newNMEAreceived()) {
    // a tricky thing here is if we print the NMEA sentence, or data
    // we end up not listening and catching other sentences! 
    // so be very wary if using OUTPUT_ALLDATA and trytng to print out data
    //Serial.println(GPS.lastNMEA());   // this also sets the newNMEAreceived() flag to false

    if (!GPS.parse(GPS.lastNMEA()))   // this also sets the newNMEAreceived() flag to false
      return;  // we can fail to parse a sentence in which case we should just wait for another
  }

  // if millis() or timer wraps around, we'll just reset it
  if (timer > millis())  timer = millis();

  // approximately every 2 seconds or so, print out the current stats
  if (millis() - timer > 2000) { 
    timer = millis(); // reset the timer

    Serial.print("\nTime: ");
    Serial.print(GPS.hour, DEC); 
    Serial.print(':');
    Serial.print(GPS.minute, DEC); 
    Serial.print(':');
    Serial.print(GPS.seconds, DEC); 
    Serial.print('.');
    Serial.println(GPS.milliseconds);
    Serial.print("Date: ");
    Serial.print(GPS.day, DEC); 
    Serial.print('/');
    Serial.print(GPS.month, DEC); 
    Serial.print("/20");
    Serial.println(GPS.year, DEC);
    Serial.print("Fix: "); 
    Serial.print((int)GPS.fix);
    Serial.print(" quality: "); 
    Serial.println((int)GPS.fixquality); 
    if (GPS.fix) {
      Serial.print("Location: ");
      Serial.print(GPS.latitude, 4); 
      Serial.print(GPS.lat);
      Serial.print(", "); 
      Serial.print(GPS.longitude, 4); 
      Serial.println(GPS.lon);
      Serial.print("Location (in degrees, works with Google Maps): ");
      Serial.print(GPS.latitudeDegrees, 4);
      Serial.print(", "); 
      Serial.println(GPS.longitudeDegrees, 4);

      Serial.print("Speed (knots): "); 
      Serial.println(GPS.speed);
      Serial.print("Angle: "); 
      Serial.println(GPS.angle);
      Serial.print("Altitude: "); 
      Serial.println(GPS.altitude);
      Serial.print("Satellites: "); 
      Serial.println((int)GPS.satellites);
    }
  }
}

void displaySensorDetails(void)
{
  sensor_t sensor;
  bmp.getSensor(&sensor);
  Serial.println("------------------------------------");
  Serial.print  ("Sensor:       "); 
  Serial.println(sensor.name);
  Serial.print  ("Driver Ver:   "); 
  Serial.println(sensor.version);
  Serial.print  ("Unique ID:    "); 
  Serial.println(sensor.sensor_id);
  Serial.print  ("Max Value:    "); 
  Serial.print(sensor.max_value); 
  Serial.println(" hPa");
  Serial.print  ("Min Value:    "); 
  Serial.print(sensor.min_value); 
  Serial.println(" hPa");
  Serial.print  ("Resolution:   "); 
  Serial.print(sensor.resolution); 
  Serial.println(" hPa");  
  Serial.println("------------------------------------");
  Serial.println("");
  delay(500);
}


