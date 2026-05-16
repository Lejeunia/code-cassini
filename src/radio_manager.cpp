#include "radio_manager.h"
#include "config.h"
#include "servos.h"
#include "stepper.h"
#include <SPI.h>
#include <RF24.h>
#include "string.h"

RF24 radio(48, 49); // pin CE et CSN

void setupRadio() {
    if (!radio.begin()) {
        Serial.println("Erreur de communication avec le module nRF24L01");
        while (1); //bloqué si le module n'est pas détecté
    }
    radio.setChannel(RF_CHANNEL);
    radio.setDataRate(RF_SPEED);
    radio.setPALevel(RF24_PA_MAX); // Puissance maximale pour les tests en labo

    //désactivation de l'auto-ack
    radio.setAutoAck(false);

    // Ouverture du pipe de transmission
    radio.openReadingPipe(1, RF_ADDRESS);

    //Début audition
    radio.startListening();

    Serial.print("Module nRF24L01 initialisé sur le canal ");
    Serial.print(RF_CHANNEL);
}


void interpreterCommandes(char* message) {
    // Cette fonction est implémentée pour interpréter les commandes reçues

    //1. Nettoyage 
    for (int i = 0; message[i] != '\0'; i++) {
        if (message[i] == '\n' || message[i] == '\r') {
            message[i] = '\0'; // Terminer la chaîne à la première occurrence de \n ou \r
            break;
        }
        
    }
    //2. Extraction de la commande 
    char* commande = strtok(message, " "); // La commande est le premier mot

    if (commande == NULL) {
        Serial.println("Message vide ou mal formé");
        return;
    }

    //3. Comparaison et action 
    if (strcmp(commande, "MOVE") == 0) {
        char*arg = strtok(NULL, " "); //on récupère l'argument suivant
        if (arg != NULL) {
            int valeur = atoi(arg); //transformation en entier
            Serial.print("ORDRE : AVANCER de");
            Serial.println(valeur);

            avancerMoteurs(valeur); //fonction à implémenter pour faire avancer les moteurs
        }
    }
    else if (strcmp(commande, "TURN") == 0) {
        char*arg = strtok(NULL, " ");
        if (arg != NULL) {
            int angle = atoi(arg);
            Serial.print("ORDRE : TOURNER de ");
            Serial.println(angle);
            
            ecrireAngleServo(1, angle); 
            ecrireAngleServo(2, angle);
            ecrireAngleServo(3, -angle);
            ecrireAngleServo(4, -angle);
        }
    }
    else if(strcmp(commande, "STOP") == 0){
        Serial.println("ORDRE : STOP");
        setMotorsEnabled(false); 
    }
}

//interpréter plusieurs commandes dans un seul paquet 
void interpreterMultipleCommandes(char* paquet) {
    char*rest; //pointeur interne pour strtok_r
    char* ligne = strtok_r(paquet, "\n", &rest); //séparer les commandes par ligne, "r" pour ré-entrant
    while (ligne!= NULL){
        interpreterCommandes(ligne); //interpréter chaque ligne comme une commande
        ligne = strtok_r(NULL, "\n", &rest); //passer à la ligne suivante
    }
 
}

void checkRadio() {
    if (radio.available()) {
        char receivedData[PAYLOAD_SIZE + 1]; // +1 pour le caractère de fin de chaîne

        //lire le paquet de 32 octets
        radio.read(&receivedData, PAYLOAD_SIZE);

        //assurer que la chaîne est bien terminée
        receivedData[PAYLOAD_SIZE] = '\0'; 
        
        Serial.print("Données reçues : ");
        Serial.println(receivedData);

        interpreterMultipleCommandes(receivedData); //fonction à implémenter pour interpréter les commandes
    }
}