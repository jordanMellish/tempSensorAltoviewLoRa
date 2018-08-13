/*
   

   This is a script writen for the Arduino Uno and DHT22 temperature sensor for a small LoRa workshop and
   uses the Multitech mDOT LoRa module running the Australian compatable AT
   enabled firmware.

   Includes:
    - Altoview mDOT: https://github.com/Altoview/Altoview_MDot
    - AltSoftSerial: https://www.pjrc.com/teensy/td_libs_AltSoftSerial.html to send data to the mDot via a serial port other than the Hardware Serial


   This program,
    Joins the LoRa Network
    Waits for motion to be sensed
    Sends the following fields:
      Temperature: the current temperature in celsius
      Humidity: the percentage of humidity
*/

/*--------------------------------------------------------------------------------------
  Includes
  --------------------------------------------------------------------------------------*/
#include <AltoviewMDot.h>
#include <AltSoftSerial.h>

/*--------------------------------------------------------------------------------------
  Definitions
  --------------------------------------------------------------------------------------*/
/* library uses software serial to communicate with the mDot module */
AltSoftSerial mdotSerial;        // AltSoftSerial only uses ports 8, 9 for RX, TX
/* library uses hardware serial to print the debuggin information */
HardwareSerial& debugSerial = Serial;

/* creating an object of a type LoRaAT called mDot */
AltoviewMDot mdot(&mdotSerial, &debugSerial);

int count = 0;
int tempSensorPin = 2;                                 //attach the DATA pin of the DHT22 to pin 2 on the Uno
int responseCode;                              //Response code from the mdot
char msg[15];                                  //cmd = {'a', 'l', 'e', 'r', 't', ':', '#', ',',
//       'c', 'o', 'u', 'n', 't', ':', '#', '#', '#'}
unsigned long rise, fall, pulseLength;
unsigned long pulseAccumulator = 0;
boolean rising = false;

#define DHTPIN D4
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE)

/*--- setup() --------------------------------------------------------------------------
  Called by the Arduino framework once, before the main loop begins.

  In the setup() routine:
   - Opens serial communication with MDOT
  --------------------------------------------------------------------------------------*/
void setup() {
  Serial.begin(38400);
  dht.begin()
  
  int responseCode;
  /* begins a serial communication of a hardware serial */
  debugSerial.begin(38400);
  /* begins a serial communication of a software serial */
  mdotSerial.begin(38400);

  debugSerial.println("\n\nJoining Altoview...\n\n");
  mdot.begin();

  do {
    /* attempt to join to Altoview */
    responseCode = mdot.join();
    /* waiting for the join process to finish. */
    delay(1000);
    if (responseCode == -1) {
      /* To go around the Join Backoff if received from the LoRa Server */ 
      delay(120000);              
    }
  } while (responseCode != 0);
  
  /* Send a simple loop count to confirm that Altoview is set up correctly and receiving this sample data. */
  sprintf(msg, "count:%d", count);
  responseCode = mdot.sendPairs(msg);
  if (responseCode == 0) {
    debugSerial.println("Success");
  } else {
    debugSerial.println("Fail");
  }
}

/*--- loop() ---------------------------------------------------------------------------
  Main loop called by the Arduino framework
  --------------------------------------------------------------------------------------*/
void loop() {
  
  float h = dht.readHumidity();
  float t = dht.readTemperature();

    // Check if any reads failed and exit early (to try again).
    if (isnan(h) || isnan(t)) {
      Serial.println("Failed to read from DHT sensor!");
      return; 
    } else {
       String temperature = String(t);
       String humidity = String(h); 
        
    }
  responseCode = -1;
  sprintf(msg, "count:%d,accum:%d", count, accum);
  responseCode = mdot.sendPairs(msg);
  debugSerial.println(temperature);
  debugSerial.println(humidity);
  delay(30000);
}


