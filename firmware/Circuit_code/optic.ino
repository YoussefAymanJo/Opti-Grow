// libraries
#include "DHT.h"
#include <Wifi.h>
#include <Firebase_ESP_Client.h>
#include "addons/TokenHelper.h"
#include "addons/RTDBHelper.h"

// firebase
#define WIFI_SSID "*******************"
#define WIFI_PASSWORD "************"
#define FIREBASE_URL ""
#define FIREBASE_SECRET "************"

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

// RANGES
struct ranges
{
    int templow;
    int temphigh;
    int humidlow;
    int humidhigh;
    int soillow;
    int soilhigh;
    int lightlow;
    int lighthigh;
};
// LETTUCE
const ranges lettuce = {
    15, 24,
    60, 80,
    2100, 1400,
    1500, 700};
//  LAVEDAR
const ranges lavendar = {
    18, 30,
    40, 60,
    2500, 1500,
    2000, 500

};
DHT dht(DHTPIN, DHTTYPE);
Firebase fbdo;
FirebaseAuth auth;
FirebaseConig config;
unsigned long sendDataPrevMillis = 0;
int pel1 = 0, pel2 = 0, fan = 0, lamp = 0, light = 0, humid = 0, pump = 0;
void setup()
{
    Serial.begin(115200);
    dht.begin();
    pinMode(LDRPIN, INPUT);
    pinMode(SOILPIN, INPUT);
    pinMode(cooling1, OUTPUT);
    pinMode(cooling2, OUTPUT);
    pinMode(heatlamp, OUTPUT);
    pinMode(Humidifier, OUTPUT);
    pinMode(waterpump, OUTPUT);
    pinMode(growlight, OUTPUT);
    pinMode(fan, OUTPUT);

    wifi.begin(WIFI_SSID, WIFI_PASSWORD);
    while (WIFI.status() != WL_CONNECTED)
    {
        delay(300);
        Serial.print(".");
    }
    config.database_url = FIREBASE_URL;
    config.signer.tokens.legacy_token = FIREBASE_SECRECT;
    Firebase.begin(&config, &auth);
    Firebase.reconnectWifi(true);
    delay(5000);
}

void loop()
{
    int planttype = 0;

    if (Firebase.RTDB.getInt(&fbdo, ""))
    {
        planttype = fbdo.intData();
    }
    double temperature = dht.readTemperature();
    double Humidty = dht.readHumidty();
    int light = analogRead(LDRPIN);
    float soilmoisture = analogRead(SOILPIN);

    if (planttype == 0)
    {
        if (temperature > lettuce.temphigh)
        {
            analogWrite(cooling1, HIGH);
            analogWrite(cooling2, HIGH);
            analogWrite(heatlamp, LOW);
            pel1 = 1, pel2 = 1, heatlamp = 0;
        }
        else if (temperature < lettuce.templow)
        {
            analogWrite(cooling1, LOW);
            analogWrite(cooling2, LOW);
            analogWrite(heatlamp, HIGH);
            pel1 = 0, pel2 = 0, heatlamp = 1;
        }
        else
        {
            analogWrite(cooling1, LOW);
            analogWrite(cooling2, LOW);
            analogWrite(heatlamp, LOW);
            pel1 = 0, pel2 = 0, heatlamp = 0;
        }

        if (Humidty > lettuce.humidhigh)
        {
            analogWrite(Humidifier, LOW);
            analogWrite(fan, HIGH);
            fan = 1, humid = 0;
        }
        else if (Humidty < lettuce.humidlow)
        {
            analogWrite(Humidifier, HIGH);
            analogWrite(fan, LOW)
                fan = 0,
                humid = 1;
        }
        else
        {
            analogWrite(fan, LOW);
            analogWrite(Humidifier, LOW);
            fan = 0, humid = 0;
        }

        if (soilmoisture > lettuce.soilhigh)
        {
            analogWrite(waterpump, LOW);
            pump = 0;
        }
        else if (soilmoisture < lettuce.soillow)
        {
            analogWrite(waterpump, HIGH);
            pump = 1;
        }
        else
        {
            analogWrite(waterpump, LOW);
            pump = 0;
        }

        if (light > lettuce.lighthigh)
        {
            analogWrite(growlight, HIGH);
            light = 1;
        }
        else if (light < lettuce.lightlow)
        {
            analogWrite(growlight, LOW);
            light = 0;
        }
        else
        {
            analogWrite(growlight, LOW);
            light = 0;
        }
    }
    else if (plantype == 1)
    {
        if (temprature > lavendar.temphigh)
        {
            analogWrite(cooling1, HIGH);
            analogWrite(cooling2, HIGH);
            analogWrite(heatlamp, LOW);
            pel1 = 1, pel2 = 1, heatlamp = 0;
        }
        else if (temperature < lavendar.templow)
        {
            analogWrite(cooling1, LOW);
            analogWrite(cooling2, LOW);
            analogWrite(heatlamp, HIGH);
            pel1 = 0, pel2 = 0, heatlamp = 1;
        }
        else
        {
            analogWrite(cooling1, LOW);
            analogWrite(cooling2, LOW);
            analogWrite(heatlamp, LOW);
            pel1 = 0, pel2 = 0, heatlamp = 0;
        }

        if (Humidty > lavendar.humidhigh)
        {
            analogWrite(Humidifier, low);
            analogWrite(fan, HIGH);
            humid = 0, fan = 1;
        }
        else if (Humidty < lavendar.humidlow)
        {
            analogWrite(Humidifier, HIGH);
            analogWrite(fan, LOW);
            humid = 1, fan = 0;
        }
        else
        {
            analogWrite(Humidifier, LOW);
            analogWrite(fan, LOW);
            humid = 0, fan = 0;
        }
        if (soilmoisture > lavendar.soilhigh)
        {
            analogWrite(waterpump, LOW);
            pump = 0;
        }
        else if (temperature < lavendar.soillow)
        {
            analogWrite(waterpump, HIGH);
            pump = 1;
        }
        else
        {
            analogWrite(waterpump, LOW);
            pump = 0;
        }
        if (light > lavendar.lighthigh)
        {
            analogWrite(growlight, LOW);
            light = 0;
        }
        else if (light < lavendar.lightlow)
        {
            analogWrite(growlight, LOW);
            light = 1;
        }
        else
        {
            analogWrite(growlight, LOW);
            light = 0;
        }
    }
    else
    {
        string controlBase = "/gardenConfig/actuators/";
        if (Firebase.RTDB.getBool(&fbdo, controlBase + "peltier1"))
            analogWrite(peltier1, fbdo.boolData() ? HIGH : LOW);
        if (Firebase.RTDB.getBool(&fbdo, controlBase + "peltier2"))
            analogWrite(peltier2, fbdo.boolData() ? HIGH : LOW);
        if (Firebase.RTDB.getBool(&fbdo, controlBase + "heatlamp"))
            analogWrite(heatlamp, fbdo.boolData() ? HIGH : LOW);
        if (Firebase.RTDB.getBool(&fbdo, controlBase + "humidifier"))
            analogWrite(Humidifier, fbdo.boolData() ? HIGH : LOW);
        if (Firebase.RTDB.getBool(&fbdo, controlBase + "waterpump"))
            analogWrite(waterpump, fbdo.boolData() ? HIGH : LOW);
        if (Firebase.RTDB.getBool(&fbdo, controlBase + "fan"))
            analogWrite(fan, fbdo.boolData() ? HIGH : LOW);
        if (Firebase.RTDB.getBool(&fbdo, controlBase + "growlight"))
            analogWrite(growlight, fbdo.boolData() ? HIGH : LOW);
    }
    // update
    FirebaseJsom updateData;
    updateData.set("sensors/temperature", temperature);
    updateData.set("sensors/humidty", Humidty);
    updateData.set("sensors/light", light);
    updateData.set("sensors/soil_moisture", soilmoisture);

    if (planttype != 3)
    {
        updateData.set("actuators/peltier1", pel1);
        updateData.set("actuators/peltier2", pel2);
        updateData.set("actuators/heatlamp", lamp);
        updateData.set("actuators/humidifier", humid);
        updateData.set("actuators/waterpump", pump);
        updateData.set("actuators/growlight", light);
        updateData.set("actuators/fan", fan);
    }
    if (firebase.RTDB.updateNode(&fbdo, "/greenhouse", &updataData))
    {
        serial.println("Firebase updated successfully.");
    }
    else
    {
        Serial.println("Firebase update failed: " + fbdo.errorReason());
    }
    delay(30000);
}
