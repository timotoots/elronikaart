/*
 *  Elronikaart
 */

#define ARDUINOJSON_DECODE_UNICODE 1


#include <ESP8266WiFi.h>
#include <WiFiClient.h> 
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>

#include <FastLED.h>
#define NUM_LEDS 40
#define DATA_PIN D7
CRGB leds[NUM_LEDS];


const char* wifiName = "";
const char* wifiPass = "";

//Web Server address to read/write from 
const char *host = "http://elron.ee/api/v1/map";

void setup() {
  
  Serial.begin(115200);
  delay(10);
  Serial.println();

  FastLED.addLeds<WS2812, DATA_PIN, RGB>(leds, NUM_LEDS);
  loop_rgb();

  
  Serial.print("Connecting to ");
  Serial.println(wifiName);

  WiFi.begin(wifiName, wifiPass);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());   //You can get IP address assigned to ESP
}

void loop() {
  HTTPClient http;    //Declare object of class HTTPClient

  Serial.print("Request Link:");
  Serial.println(host);
  
  http.begin(host);     //Specify request destination
  
  int httpCode = http.GET();            //Send the request
  String payload = http.getString();    //Get the response payload from server

  Serial.print("Response Code:"); //200 is OK
  Serial.println(httpCode);   //Print HTTP return code

  Serial.print("Returned data from Server:");
  Serial.println(payload);    //Print request response payload
  
  if(httpCode == 200)
  {


//    const size_t capacity = JSON_OBJECT_SIZE(3) + JSON_ARRAY_SIZE(2) + 60;


  DynamicJsonDocument doc(20000);
 // Deserialize the JSON document
  DeserializationError error = deserializeJson(doc, payload);
  // Test if parsing succeeds.
  if (error) {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.c_str());
    return;
  }

 //int ronge =  sizeof(doc["data"]) / sizeof(doc["data"][0]);

 JsonArray rongid = doc["data"];
 int ronge = rongid.size();
 
 Serial.print(ronge);
 Serial.println(" rongi");
 
    for(int i=0;i<NUM_LEDS;i++){
  
        leds[i] = CRGB::Black;
  
      }

      
  for(int i = 0; i<ronge;i++){
    
      const char* peatus = doc["data"][i]["viimane_peatus"];
      const int kiirus = doc["data"][i]["kiirus"].as<int>();

        int led_color;
  
        if(kiirus > 60){
          led_color = CRGB::Orange;
        } else if(kiirus > 40){
          led_color = CRGB::Blue;
        } else if(kiirus > 10){
          led_color = CRGB::Purple;
        } else if(kiirus==0){
          led_color = CRGB::Green;
        } else {
          led_color = CRGB::Red;
        }
    

      if(strcmp(peatus, "Tallinn") == 0){
        leds[28] = led_color;
        
      }
            
      if(strcmp(peatus, "Paldiski") == 0){
        leds[0] = led_color;
      }

      if(strcmp(peatus, "Laoküla") == 0){
        leds[1] = led_color;
      }

      if(strcmp(peatus, "Põllküla") == 0){
        leds[2] = led_color;
      }

      if(strcmp(peatus, "Klooga-Aedlinn") == 0){
        leds[3] = led_color;
      }

      if(strcmp(peatus, "Klooga") == 0){
        leds[4] = led_color;
      }

      if(strcmp(peatus, "Niitvälja") == 0){
        leds[5] = led_color;
      }

      if(strcmp(peatus, "Kloogaranna") == 0){
        leds[6] = led_color;
      }

      if(strcmp(peatus, "Turba") == 0){
        leds[7] = led_color;
      }

      if(strcmp(peatus, "Riisipere") == 0){
        leds[8] = led_color;
      }

      if(strcmp(peatus, "Jaanika") == 0){
        leds[9] = led_color;
      }

      if(strcmp(peatus, "Laitse") == 0){
        leds[10] = led_color;
      }

      if(strcmp(peatus, "Kibuna") == 0){
        leds[11] = led_color;
      }

      if(strcmp(peatus, "Vasalemma") == 0){
        leds[12] = led_color;
      }
      
      if(strcmp(peatus, "Kulna") == 0){
        leds[13] = led_color;
      }

      if(strcmp(peatus, "Keila") == 0){
        leds[14] = led_color;
      }

      if(strcmp(peatus, "Valingu") == 0){
        leds[15] = led_color;
      }

      if(strcmp(peatus, "Saue") == 0){
        leds[16] = led_color;
      }

      if(strcmp(peatus, "Padula") == 0){
        leds[17] = led_color;
      }

      if(strcmp(peatus, "Urda") == 0){
        leds[18] = led_color;
      }

      if(strcmp(peatus, "Laagri") == 0){
        leds[19] = led_color;
      }
      
      if(strcmp(peatus, "Pääsküla") == 0){
        leds[20] = led_color;
      }

      if(strcmp(peatus, "Kivimäe") == 0){
        leds[21] = led_color;
      }

      if(strcmp(peatus, "Hiiu") == 0){
        leds[22] = led_color;
      }

      if(strcmp(peatus, "Nõmme") == 0){
        leds[23] = led_color;
      }

      if(strcmp(peatus, "Rahumäe") == 0){
        leds[24] = led_color;
      }

      if(strcmp(peatus, "Järve") == 0){
        leds[25] = led_color;
      }

      if(strcmp(peatus, "Tondi") == 0){
        leds[26] = led_color;
      }

      if(strcmp(peatus, "Lilleküla") == 0){
        leds[27] = led_color;
      }

      if(strcmp(peatus, "Kivimäe") == 0){
        leds[21] = led_color;
      }
      
      //double longitude = doc["data"][i]["longitude"];
      //double latitude = doc["data"][i]["latitude"];
    
      // Print values.
      Serial.print(peatus);
      Serial.print(", kiirus:");
      Serial.println(kiirus);

  }

      FastLED.show();


  
    

  }
  else
  {
    Serial.println("Error in response");
  }

  http.end();  //Close connection
  
  delay(5000);  //GET Data at every 5 seconds
 // loop_rgb();
}



void loop_rgb() { 
  
  FastLED.setBrightness(  30 );

  for(int i=0;i<NUM_LEDS;i++){

    leds[i] = CRGB::Red;
    delay(100);
    FastLED.show();

    
  }

    for(int i=0;i<NUM_LEDS;i++){
      
      leds[i] = CRGB::Black;
      FastLED.show();

    }
    //delay(1000);
  
}
