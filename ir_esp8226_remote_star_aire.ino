#include <FirebaseArduino.h>
#include <ESP8266WiFi.h>


#include <Arduino.h>
#include <IRremoteESP8266.h>
#include <IRsend.h>


#define WIFI_SSID "@CodeCamp_2.4G"
#define WIFI_PASSWORD "dcode1234"
#define FIREBASE_HOST "codecampirr.firebaseio.com"
#define FIREBASE_AUTH "7jF8kGHfkr7O3R39HcE3yre84ld0ZBQyQjRbaxz5"

#define led D1



const uint16_t kIrLed = D2;  // ESP8266 GPIO pin to use. Recommended: 4 (D2).

IRsend irsend(kIrLed);  // Set the GPIO to be used to sending the message.

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("Connect Wifi...");
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(250);
    Serial.print(".");
  }

  Serial.println("WiFi connected");
  Serial.println("IP Adress: ");
  Serial.println(WiFi.localIP());

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);

  irsend.begin();
#if ESP8266
  Serial.begin(115200, SERIAL_8N1, SERIAL_TX_ONLY);
#else  // ESP8266
  Serial.begin(115200, SERIAL_8N1);
#endif  // ESP8266

  pinMode(led, OUTPUT);

}

int isOn = false;

void loop() {


  String path = "/airConference";

  FirebaseObject obj = Firebase.get(path);
  String statusString = obj.getString("status");

  String rawDataString = obj.getString(statusString);

  const size_t bufferSize = 91;
  String cod = rawDataString;
  char *str = (char*) cod.c_str();
  uint16_t arr[bufferSize];
  char *p = strtok(str, ",");
  size_t index = 0;

  while (p !=  nullptr && index < bufferSize) {
    arr[index++] = atoi(p);
    p = strtok(NULL, ",");
  }

  for (size_t i = 0; i < index; i++) {
    Serial.println(arr[i]);
  }


  //    Serial.println(rawDataString);

  irsend.sendRaw(arr, 91, 38);

  if (!statusString.equals("none") && !statusString.equals("off")) {
    isOn = true;
    digitalWrite(led, true);
      Firebase.set("airConference/status", "none");


  }
  if (statusString.equals("off")) {
    isOn = false;
    digitalWrite(led, false);
      Firebase.set("airConference/status", "none");

  }
  

  Serial.println(statusString);




}
