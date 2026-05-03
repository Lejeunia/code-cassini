#include <Arduino.h>
#include <Wire.h>
#include <vl53l4cd_class.h>

/*// put function declarations here:
int myFunction(int, int);

void setup() {
  // put your setup code here, to run once:
  int result = myFunction(2, 3);
}

void loop() {
  // put your main code here, to run repeatedly:
}

// put function definitions here:
int myFunction(int x, int y) {
  return x + y;
}
  */



// Initialisation du capteur via I2C (Wire)
// Le pin "XSHUT" est optionnel, ici on met -1 si non utilisé
VL53L4CD sensor(&Wire, -1);

void setup() {
  // Initialisation du port série pour l'affichage
  Serial.begin(115200);
  while (!Serial); // Attendre l'ouverture du moniteur

  // Initialisation du bus I2C
  Wire.begin();

  Serial.println("Initialisation du capteur VL53L4CD...");

  // Initialisation du capteur
  if (sensor.begin() != 0) {
    Serial.println("Erreur: Capteur non détecté ! Vérifiez le câblage.");
    while (1);
  }

  // Configuration du capteur
  sensor.VL53L4CD_Off(); // Arrêt par précaution
  sensor.VL53L4CD_StartMeasurement();
  
  Serial.println("Capteur prêt !");
}

void loop() {
  VL53L4CD_Result_t results;
  uint8_t status;

  // Vérifier si une nouvelle donnée est disponible
  status = sensor.VL53L4CD_GetMeasurementDataReady(&status);

  if (status == 0) {
    // Lire la distance
    sensor.VL53L4CD_GetResult(&results);

    if (results.range_status == 0) {
      Serial.print("Distance: ");
      Serial.print(results.distance_mm);
      Serial.println(" mm");
    } else {
      Serial.println("Erreur de mesure (signal trop faible ou trop loin)");
    }

    // Effacer l'interruption pour la prochaine mesure
    sensor.VL53L4CD_ClearInterrupt();
  }

  delay(100); // Petite pause pour ne pas saturer le moniteur
}