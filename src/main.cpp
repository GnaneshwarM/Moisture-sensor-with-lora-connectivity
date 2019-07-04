#include<Arduino.h>
#include <Adafruit_GPS.h>
#include<SPI.h>
#include <LoRa.h>


#define GPSSerial Serial2
Adafruit_GPS GPS(&GPSSerial);
hw_timer_t * timer = NULL;

int counter = 0;


/*
 
 All the resources for this project:
 http://randomnerdtutorials.com/
 
*/

int rainPin = 12;

// you can adjust the threshold value
int thresholdValue = 800;

void setup(){
    Serial.begin(115200);
  GPS.begin(9600);
    GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCONLY);
      GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ);
SPI.begin(LORA_SCK,LORA_MISO,LORA_MOSI,LORA_CS);
LoRa.setSPI(SPI);
LoRa.setPins(LORA_CS, LORA_RST, LORA_IRQ);

  pinMode(rainPin, INPUT);
  Serial.begin(9600);
  while (!Serial);

  Serial.println("LoRa Sender");

  if (!LoRa.begin(915E6)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
}


void loop() {
  // read the input on analog pin 0:
  int sensorValue = analogRead(rainPin);
  Serial.print("Sending packet: ");
  Serial.println(counter);
  Serial.print(sensorValue);
    LoRa.beginPacket();

  if(sensorValue < thresholdValue){
    LoRa.println(" - Doesn't need watering");
    
  }
  else {
    LoRa.println(" - Time to water your plant");
    
  }
  delay(500);


if (GPSSerial.available()) {
    GPS.read();
    if (GPS.newNMEAreceived()) {
      GPS.parse(GPS.lastNMEA());
      
      LoRa.println(GPS.lastNMEA());
      LoRa.print("\nTime: ");
      LoRa.print(GPS.hour, DEC); LoRa.print(':');
      LoRa.print(GPS.minute, DEC); LoRa.print(':');
      LoRa.print(GPS.seconds, DEC); LoRa.print('.');
      LoRa.println(GPS.milliseconds);
      LoRa.print("Date: ");
      LoRa.print(GPS.day, DEC); LoRa.print('/');
      LoRa.print(GPS.month, DEC); LoRa.print("/20");
      LoRa.println(GPS.year, DEC);
      LoRa.print("Fix: "); LoRa.print((int)GPS.fix);
      LoRa.print(" quality: "); LoRa.println((int)GPS.fixquality);
      if (GPS.fix) {
        LoRa.print("Location: ");
        LoRa.print(GPS.latitude, 4); LoRa.print(GPS.lat);
        LoRa.print(", ");
        LoRa.print(GPS.longitude, 4); LoRa.println(GPS.lon);
        LoRa.print("Speed (knots): "); LoRa.println(GPS.speed);
        LoRa.print("Angle: "); LoRa.println(GPS.angle);
        LoRa.print("Altitude: "); LoRa.println(GPS.altitude);
        LoRa.print("Satellites: "); LoRa.println((int)GPS.satellites);
      }
      
    }
  }

  LoRa.print(counter);
  LoRa.endPacket();

  counter++;

  delay(5000);


}
