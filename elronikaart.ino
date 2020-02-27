/*
 *  Elronikaart
 */

#define ARDUINOJSON_DECODE_UNICODE 1

// WIFI AUTOCONNECT
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <AutoConnect.h>

ESP8266WebServer Server;
AutoConnectConfig Config;       // Enable autoReconnect supported on v0.9.4
AutoConnect       Portal(Server);


// #include <WiFiClient.h> 
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>

#include <FastLED.h>
#define NUM_LEDS 40
#define DATA_PIN D7
CRGB leds[NUM_LEDS];

#define BUTTON_PIN D3



int display_mode = 0;

int start = 1;

// const char* wifiName = "";
// const char* wifiPass = "";

//Web Server address to read/write from 
const char *host = "http://elron.ee/api/v1/map";

  JsonArray rongid_eelmine;

void setup() {


  
  Serial.begin(115200);
  delay(10);
  Serial.println();

  pinMode(BUTTON_PIN, INPUT);

  FastLED.addLeds<WS2812, DATA_PIN, RGB>(leds, NUM_LEDS);
  loop_rgb();

  
//  Serial.print("Connecting to ");
//  Serial.println(wifiName);

 // WiFi.begin(wifiName, wifiPass);

  Config.staip = IPAddress(192,168,1,10);
  Config.staGateway = IPAddress(192,168,1,1);
  Config.staNetmask = IPAddress(255,255,255,0);
  Config.dns1 = IPAddress(192,168,1,1);
  Config.hostName = "elronikaart";
  Config.autoReconnect = true;
  
  Config.apid = "elronikaart";
  Config.psk  = "rongrong";
  Portal.config(Config);


  /*
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  */

   // Establish a connection with an autoReconnect option.
  if (Portal.begin()) {
    Serial.println("WiFi connected: " + WiFi.localIP().toString());
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());   //You can get IP address assigned to ESP


}

void loop() {
  
  Portal.handleClient();


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


      // Otsime olulised muutujad
      const int reis = doc["data"][i]["reis"].as<int>();
      const char* peatus = doc["data"][i]["viimane_peatus"];
      const int kiirus = doc["data"][i]["kiirus"].as<int>();
      const char* staatus = doc["data"][i]["reisi_staatus"];

      
      // Otsime eelmise kiiruse
      int kiirus_eelmine = 0;
      
      /*
       * Ei tööta, crashib
      if(start==0){
       for(int j = 0; j < rongid_eelmine.size();j++){
        int reis_eelmine = rongid_eelmine[j]["reis"].as<int>();
        if(reis_eelmine==reis){
            kiirus_eelmine = rongid_eelmine[j]["kiirus"].as<int>();
        }
        //Serial.println( rongid_eelmine[j]["reis"].as<int>());
       } // for
      } else {
       start = 0;
      }
    */

      // Prindime muutjad
      Serial.print("Reis:");
      Serial.print(reis);
      Serial.print(" / Viimane peatus: ");
      Serial.print(peatus);     
      Serial.print(" / Kiirus:");
      Serial.print(kiirus);
      Serial.print(" / Kiirus eelmine:");
      Serial.print(kiirus_eelmine);
      Serial.print(" / Staatus:");
      Serial.println(staatus);


        //int led_color;
        CRGB led_color;

        if(digitalRead(D3)==LOW){
          display_mode = 1;
        } else {
          display_mode = 0;
        }


        if(display_mode==1){

          if(strcmp(staatus, "plaaniline") == 0){
             led_color.red =   0;
             led_color.green = 0;
             led_color.blue =  100;
          } else {
             led_color.red =   100;
             led_color.green = 0;
             led_color.blue =  0;
          }
         

        } else if(display_mode==0){

           if(kiirus==0){
             led_color.red =    0;
             led_color.green = 100;
             led_color.blue =  0;
          } else if(kiirus > 100){
             led_color.red =  255;
             led_color.green = 0;
             led_color.blue =  0;
          } else {
             led_color.red =   map(kiirus,0,130,0,255);
             led_color.green = map(kiirus,0,130,0,255);
             led_color.blue =  map(kiirus,0,130,0,255);       
          }

        
        }



       

      
  

  /*
         if(kiirus > 120){
          led_color = CRGB::SlateBlue;
        } else if(kiirus > 110){
          led_color = CRGB::DarkCyan;
        } else if(kiirus > 100){
          led_color = CRGB::SandyBrown;
        } else if(kiirus > 90){
          led_color = CRGB::Aquamarine;
        } else if(kiirus > 80){
          led_color = CRGB::Amethyst;
        } else if(kiirus > 70){
          led_color = CRGB::CadetBlue;
        } else if(kiirus > 60){
          led_color = CRGB::Indigo;
        } else if(kiirus > 50){
          led_color = CRGB::Coral;
        } else if(kiirus > 40){
          led_color = CRGB::DarkRed;
        } else if(kiirus > 30){
          led_color = CRGB::DarkOrange;
        } else if(kiirus > 20){
          led_color = CRGB::MediumPurple;
        } else if(kiirus > 10){
          led_color = CRGB::Blue;
        } else {
          led_color = CRGB::Green;
        } 
        
*/
    peatus_color(peatus, led_color);


  }

      FastLED.show();
      
     // Crashib
     // rongid_eelmine = rongid;

  } else {
    Serial.println("Error in response");
  }

  http.end();  //Close connection
  
  delay(5000);  //GET Data at every 5 seconds
 // loop_rgb();
}


void peatus_color(const char* peatus, CRGB led_color){

           
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

      if(strcmp(peatus, "Tallinn") == 0){
        leds[28] = led_color;
      }

      if(strcmp(peatus, "Tallinn-Väike") == 0){
        leds[29] = led_color;
      }

      if(strcmp(peatus, "Liiva") == 0){
        leds[30] = led_color;
      }
      
      if(strcmp(peatus, "Valdeku") == 0){
        leds[31] = led_color;
      }

      if(strcmp(peatus, "Männiku") == 0){
        leds[32] = led_color;
      }

            if(strcmp(peatus, "Saku") == 0){
        leds[33] = led_color;
      }

            if(strcmp(peatus, "Kasemetsa") == 0){
        leds[34] = led_color;
      }

            if(strcmp(peatus, "Kiisa") == 0){
        leds[35] = led_color;
      }
      
            if(strcmp(peatus, "Roobuka") == 0){
        leds[36] = led_color;
      }

                  if(strcmp(peatus, "Vilivere") == 0){
        leds[37] = led_color;
      }

                  if(strcmp(peatus, "Kohila") == 0){
        leds[38] = led_color;
      }

  
}


void loop_rgb() { 
  
  FastLED.setBrightness(  30 );

  for(int i=0;i<NUM_LEDS;i++){

    leds[i] = CRGB::Red;
    delay(10);
    FastLED.show();

    
  }

    for(int i=0;i<NUM_LEDS;i++){
      
      leds[i] = CRGB::Black;
      FastLED.show();

    }
    //delay(1000);
  
}
