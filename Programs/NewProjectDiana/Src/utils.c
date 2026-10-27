#include "utils.h"

void intToString(int value, char *buffer) { //wandelt eine ganzzahl in einen stringum 
    int i = 0; // index
    int isNegative = 0; //merkt sich, ob die zahl negativ ist

    if(value == 0) 
    {
        buffer[i++] = '0';
        buffer[i] = '\0'; //string-terminator
        return;
    }
    if(value < 0)  //falls negativ
    { 
        isNegative = 1; //vorzeichen merken
        value = -value; //in positiven wert umwandeln
    }

    while(value != 0) 
    {
        buffer[i++] = (value % 10) + '0'; //letzte ziffer in char umwandeln
        value /= 10; //letzte ziffer entfernen
    }
    if(isNegative) //falls nötig, einen minuszeichen hinzufügen
    {
        buffer[i++] = '-';
    }

    buffer[i] = '\0'; //string beenden 

    int start = 0;
    int end = i - 1;

    while(start < end) //string umdrehen, da der rückwärts aufgebaut ist
    {
        char temp = buffer[start];
        buffer[start] = buffer[end];
        buffer[end] = temp;
        start++;
        end--;
    }
}
