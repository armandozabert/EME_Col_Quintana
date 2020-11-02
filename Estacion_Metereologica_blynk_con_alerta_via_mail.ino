#define BLYNK_PRINT Serial
#define BLYNK_MAX_SENDBYTES 128
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <Adafruit_Sensor.h>
#define DHTPIN 4 //D2       
#define DHTTYPE DHT11     // DHT 11
#include <DHT_U.h>
 
char auth[] = "XXXXXXXXXXXXXXXXXXXXXXXXX";//auto token recibido por mail
char ssid[] = "XXXXXXXXXXXX";//nombre de usuario de su WiFi
char pass[] = "XXXXXXXXXXXXX";//contraseña de su WiFi

DHT dht(DHTPIN, DHTTYPE);

BlynkTimer timer;

float h;
float t; 
int R = 5;//D1

void setup(){
  
  Serial.begin(115200); 
  
  Blynk.begin(auth, ssid, pass);
  
  dht.begin();
  
  pinMode (R, OUTPUT);
  
  digitalWrite (R, LOW);
  
  Blynk.email("nombre@micorreo.com","ALERTA POR LLUVIA NO REGAR!!!",
  "La Estacion Metereologica del Colegio Quintana informa que llueve copiosamente,
  NO ENCENDER BOMBA PARA REGAR.");
    
  timer.setInterval(1000L, sendSensor);
 }   

void loop()
{
  int lluvia = analogRead (A0); 
  Alarma();
  Blynk.run();
  timer.run();
}

void sendSensor(){
  
  h = dht.readHumidity();
  t = dht.readTemperature(); 
      
  if (isnan(h) || isnan(t)) {
    Serial.println("Error de lectura del sensor DHT11!!!");
    return;
  }
  Blynk.virtualWrite(V5, h);
  Blynk.virtualWrite(V6, t);
  }
  void Alarma(){
  int lluvia = analogRead(0); 
  if (lluvia <= 800) 
  {
     Blynk.email("nombre@micorreo.com","ALERTA POR LLUVIA NO REGAR!!!",
  "La Estacion Metereologica del Colegio Quintana informa que llueve copiosamente,
  NO ENCENDER BOMBA PARA REGAR.");
    delay(1800);//avisar cada 30 minutos
  }
}
