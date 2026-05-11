
#include "config.h"

// Pour inclure la librarie IRremote, il faut faire un premier include dans le fichier main (voir main.cpp). 
// Ensuite, il faut inclure le fichier IRremoteInt.h avec la ligne suivante dans le code propre à l'infrarouge.
#define USE_IRREMOTE_HPP_AS_PLAIN_INCLUDE
#include <IRremoteInt.h>


void setupIR() ;
void printReceivedIR() ;