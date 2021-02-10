#include <SoftwareSerial.h>
#include <TinyGPS.h>
#include <SPI.h>
#include <RH_RF95.h>
TinyGPS gps;
RH_RF95 rf95;
SoftwareSerial ss(3, 4); // Arduino RX, TX to conenct to GPS module.

float frequency = 868.0;
int txPower = 20;

long prevMillis = 0;

String deviceID = "LORD02,02";
String payload = "";
String dataSend = "";

void setup() {
  ss.begin(9600);
  Serial.begin(9600);
  // while (!Serial) ; // Wait for serial port to be available
  Serial.println("Start LoRa Client");
  if (!rf95.init())
    Serial.println("init failed");
  // Setup ISM frequency
  loraSetup();


}

void loop() {
  encodeGNSS();
  if (runEvery(6000)) {

    Serial.println(dataSend);
    rf95.send(dataSend.c_str(), dataSend.length());
  }

}
void encodeGNSS() {
  if (ss.available() > 0) {
    char _byte = ss.read();
    payload += _byte;
    if (_byte == '\n') {
      if (payload.indexOf("GPRMC") == 1) {
        dataSend = deviceID + payload.substring(payload.indexOf(","), payload.indexOf(",", 7)).substring(0, 7) + payload.substring(payload.indexOf(",", 7), payload.indexOf("*"));
      }

      payload = "";

    }

  }
}

void loraSetup() {
  rf95.setTxPower(txPower, false);
  rf95.setFrequency(frequency);
}
bool runEvery(long _interval) {
  if (millis() - prevMillis > _interval) {
    prevMillis = millis();
    return true;
  }
  return false;
}
