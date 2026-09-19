#include "DHT.h"





// sensor pin variables
#define DHTPIN 5
#define DHTTYPE DHT22
#define LDRPIN 3
#define SOILPIN 1

// acuators pin variables
#define cooling1 6
#define cooling2 8
#define heatlamp 10
#define waterpump 14
#define Humidifier 12
#define fan 16
#define growlight 35

struct ranges 
{
    int templow;
    int temphigh;
    int humidlow;
    int humidhigh;
    int soillow ;
    int soilhigh;
    int lightlow;
    int lighthigh;
};

const ranges lettuce = {
    15,24,
    60,80,
    2100,1400,
    1500,700
};

const ranges lavendar = {
  18, 30,
  40,60,
  2500,1500,
  2000,500

};
DHT dht(DHTPIN, DHTTYPE);
void setup()
{
    Serial.begin(115200);
    dht.begin();
    pinMode(LDRPIN,INPUT);
    pinMode(SOILPIN, OUTPUT);
    pinMode(cooling1, OUTPUT);
    pinMode(cooling2,OUTPUT);
    pinMode (heatlamp, OUTPUT);
    pinMode(Humidifier,OUTPUT);
    pinMode(waterpump,OUTPUT);
    pinMode(growlight , OUTPUT);
    pinMode(fan , OUTPUT);
}

void loop()
{
double temperature = dht.readTemperature();
double Humidty = dht.readHumidty();
int light = analogRead(LDRPIN);
float soilmoisture = analogRead(SOILPIN);


if(planttype == 0)
{
   if (temperature > lettuce.temphigh)
   {
    analogWrite(cooling1, 255);
    analogWrite(cooling2, 255);
    analogWrite(heatlamp ,0);
   }else if(temperature < lettuce.templow)
   {
    analogWrite(cooling1 ,0 );
    analogWrite(cooling2 , 0);
    analogWrite(heatlamp , 255);
   }else 
   {
    analogWrite(cooling1 , 0);
    analogWrite(cooling2 , 0);
    analogWrite(heatlamp , 0);
   }

   if (Humidty > lettuce.humidhigh)
   {
    analogWrite(Humidifier , 0);
    analogWrite(fan , 255);
   }else if (Humidty < lettuce.humidlow)
   {
    analogWrite(Humidifier , 255);
    analogWrite(fan , 0)
   }else {
    
   }
}else if (plantype == 1)
{

}else {


}
}
