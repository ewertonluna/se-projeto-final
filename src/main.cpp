#include <Arduino.h>

/**
 * Created by K. Suwatchai (Mobizt)
 *
 * Email: k_suwatchai@hotmail.com
 *
 * Github: https://github.com/mobizt/Firebase-ESP8266
 *
 * Copyright (c) 2022 mobizt
 *
 */

/** This example will show how to authenticate using
 * the legacy token or database secret with the new APIs (using config and auth data).
 */

#if defined(ESP32)
#include <WiFi.h>
#include <FirebaseESP32.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#include <FirebaseESP8266.h>
#endif

#include <Arduino.h>
#include <SoftwareSerial.h>
#include <addons/RTDBHelper.h>
#include <MQUnifiedsensor.h>
#define placa "Arduino UNO"
#define Voltage_Resolution 5
#define pin A0 //Analog input 4 of your arduino
#define type "MQ-2" //MQ2
#define ADC_Bit_Resolution 10 // For arduino UNO/MEGA/NANO
#define RatioMQ2CleanAir 4.4  //RS / R0 = 4.4 ppm 
#define Pin_ST_NUCLEO_RX    5  //Pino D1 da placa Node MCU
#define Pin_ST_NUCLEO_TX    4  //Pino D2 da placa Node MCU

#define WIFI_SSID "ewerton123"
#define WIFI_PASSWORD "12345678"

#define DATABASE_URL "https://boostrap-a9db8-default-rtdb.firebaseio.com/"
#define DATABASE_SECRET "upmptRlESgxHImqcPdUQKIKY6eIPMtvRb8H1Fq90"
SoftwareSerial SSerial(Pin_ST_NUCLEO_RX, Pin_ST_NUCLEO_TX);

/* 3. Define the Firebase Data object */
FirebaseData fbdo;

/* 4, Define the FirebaseAuth data for authentication data */
FirebaseAuth auth;

/* Define the FirebaseConfig data for config data */
FirebaseConfig config;

unsigned long dataMillis = 0;
int count = 0;

MQUnifiedsensor MQ2(placa, Voltage_Resolution, ADC_Bit_Resolution, pin, type);

void setup()
{

  // Open serial communications and wait for port to open:
  Serial.begin(115200);
  SSerial.begin(115200);
  Serial.println("Serial by hardware!");
  pinMode(LED_BUILTIN,OUTPUT);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED)
  {
  	Serial.print(".");
  	delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  Serial.printf("Firebase Client v%s\n\n", FIREBASE_CLIENT_VERSION);
  config.database_url = DATABASE_URL;
  config.signer.tokens.legacy_token = DATABASE_SECRET;

  Firebase.reconnectWiFi(true);

  Firebase.begin(&config, &auth);

  MQ2.setRegressionMethod(1); //_PPM =  a*ratio^b
  MQ2.setA(60000000000); MQ2.setB(-14.01); // Configure the equation to to calculate CH4 concentration
  MQ2.init();

  SSerial.println("Serial by software!");
  Serial.print("Calibrating please wait.");

  float calcR0 = 0;
  for(int i = 1; i<=10; i ++)
  {
    MQ2.update(); // Update data, the arduino will read the voltage from the analog pin
    calcR0 += MQ2.calibrate(RatioMQ2CleanAir);
    Serial.print(".");
  }
  MQ2.setR0(calcR0/10);
  Serial.println("done!");
  
  if(isinf(calcR0)) {Serial.println("Warning: Conection issue, R0 is infinite (Open circuit detected) please check your wiring and supply"); while(1);}
  if(calcR0 == 0){Serial.println("Warning: Conection issue found, R0 is zero (Analog pin shorts to ground) please check your wiring and supply"); while(1);}
  /*****************************  MQ CAlibration ********************************************/ 
  MQ2.serialDebug(true);
}

void loop()
{
  MQ2.update();
  float smokePPM = MQ2.readSensor(); // Sensor will read PPM concentration using the model, a and b values set previously or from the setup
  if(smokePPM > 120) {
	Serial.println("Warning: High concentrations of smoke detected");
  }
  MQ2.serialDebug(); // Will print the table on the serial port
  delay(400);  

  if (millis() - dataMillis > 1000) {
        dataMillis = millis();
        Serial.printf("Set int... %s\n", Firebase.setInt(fbdo, "/test/int", count++) ? "ok" : fbdo.errorReason().c_str());
        Firebase.setInt(fbdo, "/concentration", smokePPM); // 0 é o número da entrada
        Serial.println(fbdo.intData());
        digitalWrite(BUILTIN_LED,!fbdo.intData());
  }
}