#include "config.h"
#include "IR.h"

void setupIR() {
    IrReceiver.begin(IR_RECEIVE_PIN, DISABLE_LED_FEEDBACK);
    Serial.println(F("Réception infrafouge prête, protocole : "));
    printActiveIRProtocols(&Serial);
    Serial.println();
}
    
void printReceivedIR() {
    if (IrReceiver.decode()) {
        Serial.println();
        IrReceiver.printIRResultShort(&Serial);
        IrReceiver.resume();  // Enable receiving of the next value
    }
}