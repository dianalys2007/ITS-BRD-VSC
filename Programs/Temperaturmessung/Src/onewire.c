#include "onewire.h"
#include "crc.h"
#include "ow_delay.h"
#include "stm32f4xx_hal.h"


#define OW_PORT GPIOD
#define OW_PIN  0
#define OW_VCC  1


unsigned char ROM_NO[8]; // aktuelle 64-Bit ROM-Adresse
int LastDiscrepancy = 0; // Position des letzten Unterscheidungsbits
int LastDeviceFlag = 0; // Flag, das anzeigt, ob das letzte Gerät gefunden wurde
int LastFamilyDiscrepancy = 0; // Position des letzten Unterscheidungsbits in der Gerätefamilie (für zukünftige Erweiterungen)


static void ow_low(void) //leitung auf low ziehen
{
    OW_PORT->BSRR = (1 << (OW_PIN + 16));
}

static void ow_high(void) //leitung freigeben (pull-up)
{
    OW_PORT->BSRR = (1 << OW_PIN);
}

static void ow_strong_high(void) //leitung auf high ziehen (push-pull)
{
   OW_PORT->OTYPER &= ~(1 << OW_PIN); // push-pull
   OW_PORT->BSRR = (1 << OW_PIN); // leitung auf high ziehen
}

static void ow_release(void) //leitung freigeben (open-drain)
{
    OW_PORT->OTYPER |= (1 << OW_PIN); // open-drain-modus wiederherstellen
}



void ow_init(void) //initialisierung der OneWire-Schnittstelle
{
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN; // GPIO D aktivieren

    //PD0 als Ausgang
    OW_PORT->MODER &= ~(3 << (OW_PIN * 2)); 
    OW_PORT->MODER |=  (1 << (OW_PIN * 2));

    //Open-Drain aktivieren
    OW_PORT->OTYPER |= (1 << OW_PIN);

    //Pull-Up aktivieren
    OW_PORT->PUPDR &= ~(3 << (OW_PIN * 2)); 
    OW_PORT->PUPDR |=  (1 << (OW_PIN * 2));

    //PD1 als Ausgang
    OW_PORT->MODER &= ~(3 << (OW_VCC * 2));
    OW_PORT->MODER |=  (1 << (OW_VCC * 2));

    //Versorgung einschalten
    OW_PORT->BSRR = (1 << OW_VCC);

    //in der Ruhephase soll die Leitung auf high sein
    ow_high();
}


//Reset-Puls senden und Presence-Puls prüfen
int OWReset(void)
{
    ow_low(); //ziehe die 1-wire-leitung auf low 
    delay_us(480); //halte sie für mindestens 480µs auf low
    ow_high(); //gib die Leitung wieder frei (pull-up)
    delay_us(70); //warte auf den Presence-Puls (70µs nach dem Loslassen der Leitung)

    //sensor antwortet mit LOW, wenn mind. ein gerät vorhanden ist 
    int presence = ((OW_PORT->IDR & (1 << OW_PIN)) == 0);

    delay_us(410); //warte bis zum Ende des Reset-Zyklus (410µs nach dem Presence-Puls)
    return presence; //1=Sensor antwortet, 0=kein Sensor gefunden
}


//ein einzelnes Bit senden
void OWWriteBit(uint8_t bit)
{
    if (bit)
    {
        ow_low(); //starte das Senden eines '1'-Bits, indem die Leitung kurz auf low gezogen wird
        delay_us(6); //nur kurzer LOW-Puls für logische '1'
        ow_high(); //gib die Leitung sofort wieder frei (pull-up)
        delay_us(64); //rest des Zeitslots warten 
    }
    else
    {
        ow_low(); //starte das Senden eines '0'-Bits, indem die Leitung auf low gezogen wird
        delay_us(60); //lange low-phase für logische '0'
        ow_high(); //leitung freigeben (pull-up)
        delay_us(10); //kurz warten, bevor der nächste Zeitslot beginnt
    }
}

//ein einzelnes Bit lesen
uint8_t OWReadBit(void) 
{
    uint8_t bit; //variable zum Speichern des gelesenen Bits

    ow_low(); //erzeuge den Startimpuls des Lesevorgangs, indem die Leitung kurz auf low gezogen wird
    delay_us(6); //low für mindestens 6µs halten, um den Sensor zum Senden des Bits zu veranlassen
    ow_high(); //leitung freigeben (pull-up), damit der Sensor das Bit senden kann
    delay_us(9); //warte 9µs, damit der Sensor das Bit stabil auf die Leitung legt

    //lese den aktuellen Pegel der 1-Wire-Leitung 
    bit = (OW_PORT->IDR & (1 << OW_PIN)) != 0; 

    delay_us(55); //warte bis zum Ende des Zeitslots (insgesamt 60µs nach dem Startimpuls)
    return bit; //rückgabe des gelesenen Bits (1 oder 0)
}

//ein Bytes auf den Bus schreiben (LSB zuerst)
void OWWriteByte(uint8_t val) 
{
    //sende alle 8 Bits eines Bytes, beginnend mit dem LSB
    for (int i = 0; i < 8; i++)
    {
        OWWriteBit(val & 1); //sende das niederwertiges Bit
        val >>= 1; //nächstes Bit an Position 0 schieben
    }
}

//ein Byte vom Bus lesen (LSB zuerst)
uint8_t OWReadByte(void)
{
    uint8_t val = 0; //Variable zum Speichern des gelesenen Bytes

    for (int i = 0; i < 8; i++) 
    {
        if (OWReadBit()) //wenn das gelesene Bit 1 ist
            val |= (1 << i); //setze das entsprechende Bit in val
        //wenn das gelesene Bit 0 ist, bleibt das entsprechende Bit in val auf 0, daher keine Aktion erforderlich
    }

    return val;//Rückgabe des vollständig gelesenen Bytes
}

//Suche nach dem ersten Sensor 
int OWFirst(void)
{
    LastDiscrepancy = 0; //Position des letzten Unterscheidungsbits zurücksetzen
    LastDeviceFlag = 0; //Flag zurücksetzen, das anzeigt, ob das letzte Gerät gefunden wurde
    LastFamilyDiscrepancy = 0; //Position des letzten Unterscheidungsbits in der Gerätefamilie zurücksetzen (für zukünftige Erweiterungen)

    //ROM-Puffer löschen, damit er für die Suche bereit ist
    for (int i = 0; i < 8; i++)
        ROM_NO[i] = 0;

    return OWSearch(); //Starte die Suche nach dem ersten Sensor
}

//Suche nach dem nächsten Sensor
int OWNext(void)
{
    //falls das letzte Gerät bereits gefunden wurde, gibt es kein weiteres Gerät mehr zu suchen
    if (LastDeviceFlag)
        return 0;

    return OWSearch(); //Starte die Suche nach dem nächsten Sensor
}

//Suche nach einem Sensor auf dem Bus
int OWSearch(void)
{
    int id_bit_number = 1; //aktuelle bitnummer innerhalb der rom adresse (1-64)
    int last_zero = 0; //letzte position eines untescheidungsbits 
    int rom_byte = 0; //aktuelles byte der rom adresse (0-7)
    unsigned char mask = 1; //Bitmaske innerhalb des aktuellen Bytes

    int id_bit, cmp_bit; //gelesenes Bit und sein Komplement 
    unsigned char search_direction; //Richtung der Suche (0 oder 1)

    crc8 = 0; //CRC zurücksetzen, damit die ROM-Adresse korrekt überprüft werden kann

    //prüfen, ob mind. ein sensor antwortet
    if (!OWReset())
        return 0;

    OWWriteByte(0xF0); //Sende den Suchbefehl, um alle Sensoren auf dem Bus zu identifizieren
    //search rom

    //alle 64 Bits der ROM-Adresse durchlaufen
    while (rom_byte < 8)
    {
        id_bit = OWReadBit(); //lese datenbit (ob alle sensoren dieselbe Bitposition besitzen)
        cmp_bit = OWReadBit(); //lese komplementbit (ob es an deiser Stelle unterschiedliche Bits gibt, Konflikt)

        //beide Bits = 1, dann ist kein gerät auf dem Bus
        if (id_bit == 1 && cmp_bit == 1)
            return 0;

        //keine Kollision, Bitwert ist eindeutig     
        if (id_bit != cmp_bit)
        {
            search_direction = id_bit;
        }
        else
        //Kollision (sowohl 0 als auch 1 exisitieren)
        {
            // if (id_bit_number < LastDiscrepancy)
            // {
            //     //verwende dieselbe Richtung wie beim letzten Durchlauf
            //     search_direction = (ROM_NO[rom_byte] & mask) ? 1 : 0;
            // }
            // else if (id_bit_number == LastDiscrepancy)
            // {
            //     //an der letzten Kollision nun den anderen zweig wählen
            //     search_direction = 1;
            // }
            // else
            // {
            //     //standardmäßig zuerst den 0-Zweig wählen
            //     search_direction = 0;
            // }

            // //Position der aktuellen kollision merken
            // if (search_direction == 0)
            //     last_zero = id_bit_number;

            //Bei einer Kollision folgt der Algorithmus zunächst dem bisherigen Pfad. 
            // An der Position der letzten Kollision wird jedoch der andere Zweig gewählt. 
            // Dadurch werden nach und nach alle möglichen ROM-Pfade durchsucht und alle Sensoren auf dem OneWire-Bus gefunden


            if (id_bit_number < LastDiscrepancy)
                search_direction = (ROM_NO[rom_byte] & mask);
            else
                search_direction = (id_bit_number == LastDiscrepancy);

            if (search_direction == 0)
                last_zero = id_bit_number;
        }

        //gewähltes Bit in rom speichern
        if (search_direction)
            ROM_NO[rom_byte] |= mask;
        else
            ROM_NO[rom_byte] &= ~mask;

        OWWriteBit(search_direction); //gewähltes Bit an den Bus zurücksenden 

        id_bit_number++; //nächstes rom bit

        mask <<= 1; //nächstes Bit im Byte

        //Wenn ein komplettes Byte fertig ist 
        if (mask == 0)
        {
            docrc8(ROM_NO[rom_byte]); //crc aktualisieren
            rom_byte++; //zum nächsten Rom Byte wechseln
            mask = 1; //bitmaske zurücksetzen
        }
    }

    LastDiscrepancy = last_zero; //letzte Verzweigungsstelle speichern


    //keine weiteren Verzweigungen vorhanden 
    if (LastDiscrepancy == 0)
        LastDeviceFlag = 1;

    //Sicherheitsprüfung    
    if(rom_byte < 8)
        return 0;

    return 1;//erfolgreich gefunden
}

//Temperatur eines bestimmten sensor lesen
int read_temperature_by_rom(const uint8_t rom[8], float *out_c)
{
    //prüfen, ob Sensor antwortet
    if (!OWReset())
        return 0;

    OWWriteByte(0x55); //match rom, einen bestimmten sensor auswählen

    //gewünschte rom adresse senden
    for (int i = 0; i < 8; i++)
        OWWriteByte(rom[i]);

    OWWriteByte(0x44); //Temperaturmessung starten
    //convert t

    ow_strong_high(); 

    HAL_Delay(750);

    ow_release();

    //erneut reset
    if (!OWReset())
        return 0;


    OWWriteByte(0x55); //match rom erneut senden

    //erneut dieselbe rom adresse auswählen
    for (int i = 0; i < 8; i++)
        OWWriteByte(rom[i]);

    OWWriteByte(0xBE); //read scratchpad, messdaten auslesen

    uint8_t sp[9]; //scratchpad speicher

    //alle 9 bytes des scratchpads lesen 
    for (int i = 0; i < 9; i++)
        sp[i] = OWReadByte();

    crc8 = 0; //crc berexhnung zurücksetzen

    //crc der ersten 8 Bytes berechnen 
    for (int i = 0; i < 8; i++)
        crc8 = docrc8(sp[i]);

    //prüfen, ob die crc korrekt ist     
    if (crc8 != sp[8])
        return 0;

    //Temperatur Bytes zu einem 6 bit wert zusammensetzen    
    int16_t raw = (sp[1] << 8) | sp[0];

    //DS18B20 liefert die temperatur in Schritten von 1/16 grad C
    *out_c = raw / 16.0f;

    return 1; //Temperatur erfolgreich gelesen
}