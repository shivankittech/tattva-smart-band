#include <Adafruit_MPU6050.h>
#include <Adafruit_SSD1306.h>
#include <Wire.h>
#include <MPU6050_tockn.h>
#include <ArduinoJson.h>

#include <PubSubClient.h>
Adafruit_MPU6050 mpu;
#include<WiFi.h>
Adafruit_SSD1306 display = Adafruit_SSD1306(128, 64, &Wire);
MPU6050 mpu6050(Wire);

unsigned long startMillis;
unsigned long currentMillis;
const unsigned long period = 3000;



WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;
long timer = 0;

  int steps=0;

  float distanceinonestep=71; //change it according to your distance between your legs in cm

  float distance;


// Connect to WiFi network

 
const char* ssid = "LUC9-2.4G";
const char* password =  "aarshLuc9";
const char* mqttServer = "dev.tattva.cloud";
const int mqttPort = 1883;
//const char* mqttUser = "yourMQTTuser";
//const char* mqttPassword = "yourMQTTpassword";
char data[80];




 
void wifisetup() {
 
  //Serial.begin(115200);
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi..");
  }
 
  Serial.println("Connected to the WiFi network");
 
  client.setServer(mqttServer, mqttPort);
 
  while (!client.connected()) {
    Serial.println("Connecting to MQTT...");
 
    if (client.connect("ESP32Client" )) {
 
      Serial.println("connected");
 
    } else {
 
      Serial.print("failed with state ");
      Serial.print(client.state());
      delay(2000);
 
    }
  }
 
  
 
}
 





  
void setup() {
  
  Serial.begin(115200);
  // while (!Serial);
  Serial.println("MPU6050 OLED demo");

  if (!mpu.begin()) {
    Serial.println("Sensor init failed");
    while (1)
      yield();
  }
  Serial.println("Found a MPU-6050 sensor");

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3C for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for (;;)
      ; // Don't proceed, loop forever
  }
  display.display();
  delay(500); // Pause for 2 seconds
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setRotation(0);
//  calibrate();
startMillis=millis();
wifisetup();
}





void loop() {
  currentMillis = millis();
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  display.clearDisplay();
  display.setCursor(0, 0);


    


   mpu6050.update();



  if(millis() - timer > 1000){

    



    Serial.print("\taccY : ");Serial.print(mpu6050.getAccY());   

  }
 

  

if(mpu6050.getAccY()>1)

{

  steps+=1;

  delay(350);

}






  display.println("steps");

  display.println(steps);

    Serial.println("steps");

  Serial.println(steps);

 



   distance = steps*distanceinonestep/100;

       display.println("distance");

     display.println(distance);

  display.display();      

 delay(100);

   display.clearDisplay();
  
  
DynamicJsonDocument payload(1024);
String strPayload;

payload["deviceId"] = WiFi.macAddress();
payload["steps"] = steps;

serializeJson(payload, strPayload);

   Serial.print(strPayload);
   strPayload.toCharArray(data, (strPayload.length() + 1));
   if (currentMillis-startMillis>=5*period)
 {client.publish("esp/test",data);
 startMillis=currentMillis;}

 
}
