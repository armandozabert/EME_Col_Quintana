#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include "ThingSpeak.h"
#include <DHT_U.h>

char auth[] = "zZrim6qXUPI25PtCjbEooF9dHhZMyDBX";
char ssid[] = "HITRON-5EA0";
char pass[] = "M6KHK3K8SFRC";

char thingSpeakAddress[]= "api.thingspeak.com";
unsigned long channelID = 1192896;
const char* writeAPIKey = "PB55XO5K6ELWKK0F";

const unsigned long postingInterval = 3*60*1000; //cada 5 minutos
long lastUpdateTime=0;

float h;
float t;
int R = 2;//D4
int lluvia = analogRead(A0);

WiFiClient client;

#define DHTPIN 4 //D2        
#define DHTTYPE DHT11 //DHT 11

DHT dht(DHTPIN, DHTTYPE);
BlynkTimer timer;

//poner en modo push en el celular

void sendSensor()
{
  float h = dht.readHumidity();
  float t = dht.readTemperature(); 

  if (isnan(h) || isnan(t)) {
    Serial.println("Fallo la lectura del Sensor DHT11 !!!");
    return;
  }
  
  Blynk.virtualWrite(V5, h);
  Blynk.virtualWrite(V6, t);
  //Blynk.virtualWrite(V6, R);
}

void setup()
{
  Serial.begin(115200);

  Blynk.begin(auth, ssid, pass);
 
  dht.begin();

  pinMode (R, OUTPUT);
  digitalWrite (R, LOW);
  
  WiFi.begin(ssid,pass);
  Serial.print("Conectando");
  
  while(WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(100); 
  }
  Serial.println("Conectado");
  Serial.print("DIRECCION IP:");
  Serial.println(WiFi.localIP());

  ThingSpeak.begin(client);
  
  timer.setInterval(1000L, sendSensor);
}

void loop()
{
  Blynk.run();
  timer.run();

  if ((millis()- lastUpdateTime)>= postingInterval){
    Serial.println("Se envian los datos");
  
  h = dht.readHumidity();// Leemos la humedad relativa
  t = dht.readTemperature();// Leemos la temperatura en grados centígrados (por defecto)
  int lluvia = analogRead(A0);
  
  ThingSpeak.setField(1,t);
  ThingSpeak.setField(2,h);
  ThingSpeak.setField(3,lluvia);
  
  ThingSpeak.setStatus("Actualiza los datos");
   
  int x = ThingSpeak.writeFields(channelID, writeAPIKey);
  if(x == 200) {
    Serial.println("Actualizacion correcta");
  } else {
    Serial.println("Se produjo un problema. HTTP Codigo de error"+String(x));
 }
}
}
