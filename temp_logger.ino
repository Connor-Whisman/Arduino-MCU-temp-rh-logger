#include <ArduinoJson.h>
#include <ESP8266HTTPClient.h>
#include <HttpClient.h>
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include "DHT.h"


#define DHTTYPE DHT22 //--> Defines the type of DHT sensor used (DHT11, DHT21, and DHT22), in this project the sensor used is DHT11.
#define ON_Board_LED 2  //--> Defining an On Board LED, used for indicators when the process of connecting to a wifi router
const int DHTPin1 = 5; //--> The pins used for the DHT22 sensors are Pin D1 & D2
const int DHTPin2 = 4;
DHT dht1(DHTPin1, DHTTYPE); //--> Initialize DHT sensor, DHT dht(Pin_used, Type_of_DHT_Sensor);
DHT dht2(DHTPin2, DHTTYPE);

//----------------------------------------SSID and Password of your WiFi router.
//const char* ssid = "Amped_RTA1750_2.4"; //--> Your wifi name or SSID.
//const char* password = "wireless"; //--> Your wifi password.
const char* ssid = "XFSETUP-38E9"; //--> Your wifi name or SSID.
const char* password = "bridge9252empty"; //--> Your wifi password.
//----------------------------------------

//----------------------------------------Host & httpsPort
const char* host = "script.google.com";
const int httpsPort = 443;
//----------------------------------------

WiFiClientSecure client; //--> Create a WiFiClientSecure object.


String GAS_ID = "AKfycbw70WYzYqBYR--45jPhcyR4SjdPmUJdveO5b_UrQ-GJjREik8uqsP04lc5VK2O1dwE"; //--> spreadsheet script ID
String url = "https://script.google.com/macros/s/" + GAS_ID + "/exec";
String Table_Code = "C";

//============================================================================== void setup
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  delay(500);

  dht1.begin();  //--> Start reading DHT11 sensors
  dht2.begin();
  delay(500);
  
  WiFi.begin(ssid, password); //--> Connect to your WiFi router
  Serial.println("");
    
  pinMode(ON_Board_LED,OUTPUT); //--> On Board LED port Direction output
  digitalWrite(ON_Board_LED, HIGH); //--> Turn off Led On Board

  //----------------------------------------Wait for connection
  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    //----------------------------------------Make the On Board Flashing LED on the process of connecting to the wifi router.
    digitalWrite(ON_Board_LED, LOW);
    delay(250);
    digitalWrite(ON_Board_LED, HIGH);
    delay(250);
    //----------------------------------------
  }
  //----------------------------------------
  digitalWrite(ON_Board_LED, HIGH); //--> Turn off the On Board LED when it is connected to the wifi router.
  //----------------------------------------If successfully connected to the wifi router, the IP Address that will be visited is displayed in the serial monitor
  Serial.println("");
  Serial.print("Successfully connected to : ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.println();
  //----------------------------------------

  client.setInsecure();
}
//==============================================================================
//============================================================================== void loop
void loop() {
  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  int h1 = dht1.readHumidity();
  delay(100);
  int h2 = dht2.readHumidity();
  delay(100);
  // Read temperature as Celsius (the default)
  float t1 = dht1.readTemperature();
  delay(100);
  float t2 = dht2.readTemperature();
  delay(100);
  // Check if any reads failed and exit early (to try again).
  if (isnan(h1) || isnan(t1) || isnan(h2) || isnan(t2) || t1 == -50 || t2 == -50 || h1 == 1 || h2 == 1) {
    Serial.println("Failed to read from DHT sensor !");
    Serial.println("t1 = " + String(t1));
    Serial.println("h1 = " + String(h1));
    Serial.println("t2 = " + String(t2));
    Serial.println("h2 = " + String(h2));
//    digitalWrite(ON_Board_LED, LOW);
//    delay(250);
//    digitalWrite(ON_Board_LED, HIGH);
//    delay(250);
    delay(3000); 
    return;
  }else {
    Serial.println("Running delay..");
    delay(58500);
  }
  String Temp1 = "Temperature 1: " + String(t1) + " °C";
  String Humi1 = "Humidity 1: " + String(h1) + " %";
  String Temp2 = "Temperature 2: " + String(t2) + " °C";
  String Humi2 = "Humidity 2: " + String(h2) + " %";
  Serial.println(Temp1);
  Serial.println(Humi1);
  Serial.println(Temp2);
  Serial.println(Humi2);
  
  sendData2(t1, h1, t2, h2); //--> Calls the sendData Subroutine
  
}
//==============================================================================
//============================================================================== void sendData
// Subroutine for sending data to Google Sheets
//void sendData(float tem1, int hum1, float tem2, int hum2) {
//  Serial.println("==========");
//  Serial.print("connecting to ");
//  Serial.println(host);
//  
//  //----------------------------------------Connect to Google host
//  if (!client.connect(host, httpsPort)) {
//    Serial.println("connection failed");
//    return;
//  }
//  //----------------------------------------
//
//  //----------------------------------------Processing data and sending data
//  String string_temperature1 =  String(tem1);
//  String string_temperature2 =  String(tem2);
//  String string_humidity1 =  String(hum1, DEC); 
//  String string_humidity2 =  String(hum2, DEC); 
////  String url = "/macros/s/" + GAS_ID + "/exec?table=A1&Temp1=" + string_temperature1 + "&RH1=" + string_humidity1 + "&Temp2=" + string_temperature2 + "&RH2=" + string_humidity2;
//  Serial.print("requesting URL: ");
//  Serial.println(url);
//
//  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
//         "Host: " + host + "\r\n" +
//         "User-Agent: BuildFailureDetectorESP8266\r\n" +
//         "Connection: close\r\n\r\n");
//
//  Serial.println("request sent");
//  //----------------------------------------
//
//  //----------------------------------------Checking whether the data was sent successfully or not
//  while (client.connected()) {
//    String line = client.readStringUntil('\n');
//    if (line == "\r") {
//      Serial.println("headers received");
//      break;
//    }
//  }
//  String line = client.readStringUntil('\n');
//  if (line.startsWith("{\"state\":\"success\"")) {
//    Serial.println("esp8266/Arduino CI successfull!");
//  } else {
//    Serial.println("esp8266/Arduino CI has failed");
//  }
//  Serial.print("reply was : ");
//  Serial.println(line);
//  Serial.println("closing connection");
//  Serial.println("==========");
//  Serial.println();
//  //----------------------------------------
//} 

void sendData2(float tem1, int hum1, float tem2, int hum2) {
  Serial.println("==========");
  Serial.print("connecting to ");
  Serial.println(url);
  String string_temperature1 =  String(tem1);
  String string_humidity1 =  String(hum1, DEC); 
  String string_temperature2 =  String(tem2);
  String string_humidity2 =  String(hum2, DEC);
  String data_string = "table=" + Table_Code + "&temp1=" + string_temperature1 + "&rh1=" + string_humidity1 + "&temp2=" + string_temperature2 + "&rh2=" + string_humidity2;
  DynamicJsonDocument data(1024);
  data["table"] = Table_Code;
  data["temp1"] = tem1;
  data["rh1"] = hum1;
  data["temp2"] = tem2;
  data["rh2"] = hum2;
  
  String json;
  serializeJson(data, json);
  Serial.print(json);
  
  HTTPClient http;
  http.begin(client, url); 
  http.addHeader("Content-Type", "Content-Type: application/json"); 

  int httpResponseCode = http.POST(json); //Send the actual POST request

  if(httpResponseCode>0){
    String response = http.getString();  //Get the response to the request
    Serial.println(httpResponseCode);   //Print return code
    Serial.println(response);           //Print request answer
  } else {
    Serial.print("Error on sending POST: ");
    Serial.println(httpResponseCode);

    http.end();
 }
}
//==============================================================================
