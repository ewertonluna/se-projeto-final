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
//Pinos de comunicacao serial com a ST Núcleo
#define Pin_ST_NUCLEO_RX    5  //Pino D1 da placa Node MCU
#define Pin_ST_NUCLEO_TX    4  //Pino D2 da placa Node MCU
SoftwareSerial SSerial(Pin_ST_NUCLEO_RX, Pin_ST_NUCLEO_TX);


// Provide the RTDB payload printing info and other helper functions.
#include <addons/RTDBHelper.h>

/* 1. Define the WiFi credentials */
#define WIFI_SSID "ewerton123"
#define WIFI_PASSWORD "12345678"

/* 2. If work with RTDB, define the RTDB URL and database secret */
#define DATABASE_URL "https://boostrap-a9db8-default-rtdb.firebaseio.com/"
#define DATABASE_SECRET "upmptRlESgxHImqcPdUQKIKY6eIPMtvRb8H1Fq90"

/* 3. Define the Firebase Data object */
FirebaseData fbdo;

/* 4, Define the FirebaseAuth data for authentication data */
FirebaseAuth auth;

/* Define the FirebaseConfig data for config data */
FirebaseConfig config;

unsigned long dataMillis = 0;
int count = 0;

void setup()
{

     // Open serial communications and wait for port to open:
  Serial.begin(115200);
  SSerial.begin(115200);

  Serial.println("Serial by hardware!");

  // set the data rate for the SoftwareSerial port
  SSerial.println("Serial by software!");

}

void loop()
{
//IMPORTANTE: O codigo abaixo é apenas para demonstração. 
  // Este codigo precisará ser removidou ou modificado para o projeto final!
  if (SSerial.available()){
    Serial.write(SSerial.read());
    delay(1);
  }
  if (Serial.available()){
    SSerial.write(Serial.read());
    delay(1);
  }

}