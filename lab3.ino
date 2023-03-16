#include "DHT.h"
#include <MKRWAN.h>

#define DHTPIN 2     // Digital pin connected to the DHT sensor
#define DHTTYPE DHT11
#define PACKET_INTERVAL 10000


DHT dht(DHTPIN, DHTTYPE);
LoRaModem modem;

String appEui = "0000000000000000";
String appKey = "BC3525DA457BDB58C49E5854F4C6D77F";


void setup() {

  // Démarrage du serial
  Serial.begin(115200);
  while(!Serial);
  Serial.println("Serial Started");
  
  if(!modem.begin(US915)) {
    Serial.println("Failed to start module");
    exit(1);
  };
  
  int connected;
  appKey.trim();
  appEui.trim();

  connected = modem.joinOTAA(appEui, appKey);

  if (!connected) {
    Serial.println("An error occur while try to connect");
    exit(1);
  } else {
    Serial.println("Connected to the gateway");
  }

  dht.begin();
}

int sendPacket(float temp, float hum){
  modem.beginPacket();
  modem.print(temp);
  modem.print(hum);
  int err = modem.endPacket(true);
  if(err > 0){
    Serial.print("<-- Sent Packet with data :"); 
    Serial.print("{ ");
    Serial.print(temp);
    Serial.print(" , ");
    Serial.print(hum);
    Serial.println(" }");
  } else {
    Serial.println("/!\\ An error occur, a packet WASN'T sent");
  }
  return err;
}

void loop() {
  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }

  Serial.println("(i) Read data :");
  Serial.print("\tTemperature : ");
  Serial.println(t);
  Serial.print("\tHumidity : ");
  Serial.println(h);

  sendPacket(t, h);
  delay(PACKET_INTERVAL);
}
