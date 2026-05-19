// ce fichier de configuration rassemble au même endroit toutes les constantes

// ces lignes permettent d'éviter les inclusions multiples
#ifndef CONFIG_H
#define CONFIG_H 



#include <Arduino.h>
//#include <StateMachine.h>   --------------------------------------------
//#include <Button.h>           ------------------------------------------


// ----------------------------------------------------------------
// Infrarouge
// protocole NEC uniquement pour ce projet
#define DECODE_NEC 
#define EXCLUDE_EXOTIC_PROTOCOLS
#define RAW_BUFFER_LENGTH  750
// ----------------------------------------------------------------

// le mot clé #define permet de définir des constantes, les valeurs seront effectivement remplacées
// dans le code avant la compilation.
// C'est une bonne pratique de mettre les constantes en majuscules.

/* Définition des broches
#define PIN_LED 13
#define PIN_BOUTON 2
#define IR_RECEIVE_PIN 7
*/


// Définition des délais
#define DELAI_ATTENTE 3000  // 3 secondes
#define DELAI_ACTION 5000   // 5 secondes
#define DELAI_BOUCLE_MS 10 // 10 millisecondes par boucle
#define BLINK_HALF_TIME 500 // 500 ms pour un clignotement rapide 1Hz

// Déclaration des variables globales (définies dans main.cpp)
//extern StateMachine machine; ------------------------------------------------------
//extern Button bouton; ---------------------------------------------------------
extern unsigned long tempsDebut;

//----------------------------------------------------
//Capteurs Ultra son HC-SR04

//définition des broches
#define PIN_TRIG_g 26
#define PIN_ECHO_g 27

#define PIN_TRIG_d 29
#define PIN_ECHO_d 30

//Configuration des constantes
#define SOUND_SPEED 0.0343 // [cm/µs]
#define US_TIMEOUT 30000


//----------------------------------------------------
//Stepper 17HS15 Nema17

//Attribution des broches (en réalité, ce sont les broches du driver)
#define M1_STEP_PIN 32 //broche "STEP" du driver 1
#define M1_DIR_PIN 33 //broche "DIR" du driver 1
#define M1_ENABLE_PIN 34 //broche "enable" du driver 1

#define M2_STEP_PIN 35
#define M2_DIR_PIN 36
#define M2_ENABLE_PIN 37

#define M3_STEP_PIN 38
#define M3_DIR_PIN 39
#define M3_ENABLE_PIN 40

#define M4_STEP_PIN 41
#define M4_DIR_PIN 42
#define M4_ENABLE_PIN 43

#define PAS_ROTATION_360 942
#define PAS_ROTATION_90  236


//---------------------------------------------------
//gyroscope MPU6050

//Attribution des broches
#define I2C_SDA 21
#define I2C_SCL 22

#define MPU_ADDR 0x68

//---------------------------------------------------
//servos FS5106B

//Attribution des broches
#define PIN_SERVO_3 4
#define PIN_SERVO_4 5
#define PIN_SERVO_1 6
#define PIN_SERVO_2 7

//----------------------------------------------
//NRF24L01
#define NRF_cePin  48
#define NRF_csnPin 53  
 
#define NRF_CHANNEL 104                 // Canal 104
#define NRF_SPEED    RF24_250KBPS       // Débit 250 kbps
const uint64_t NRF_ADDRESS = 0xE8E8F0F0A2LL;
 
#define NRF_PAYLOAD_SIZE 32 //32 octets max


//------------------------------------------------
// ==========================
// I2C PINS ARDUINO MEGA
// ==========================
#define I2C_SDA_PIN 20
#define I2C_SCL_PIN 21

#endif

