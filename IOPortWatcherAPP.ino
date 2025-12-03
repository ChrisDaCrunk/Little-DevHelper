// IOPortWatcherAPP.ino - App: IO Port Monitor (basierend auf der Watcher-Logik)

#include "02_Menu.h"
#include <algorithm>

extern bool isMenuOpen; 
extern AppConfig* currentRunningApp;

// ----------------------------------------------------------------------
// KONSTANTEN UND VARIABLEN
// ----------------------------------------------------------------------

struct PortNum {
    const int Number;
};

// Liste der zu überwachenden Pins, wie vom Benutzer bereitgestellt.
// ACHTUNG: Die Pins 1 und 2 sind intern für die Tastatur (ENTER) bzw. die Status-LED reserviert.
// Analoge Lesevorgänge auf diesen Pins führen oft zu dem "adc1 already in use" Fehler 
// auf dem M5Cardputer (ESP32-S3) oder zu inkorrekten Werten.
const PortNum PortList[] = {{0},{1},{2},{3},{4},{5},{6},{8},{9},{12},{13},{14},{15},{39},{40}};
const int NUM_Ports = sizeof(PortList) / sizeof(PortList[0]);

int ColumSpace = 80;
int LineHeight = 15;
int MaxLines = 5; 

// ----------------------------------------------------------------------
// LOKALE HILFSFUNKTIONEN
// ----------------------------------------------------------------------

/**
 * Ruft analogRead() nur für bekannte, sichere ADC-Pins auf, die keine 
 * internen Board-Funktionen stören.
 * @return Analoger Wert (0-4095) oder -1, wenn der Pin nicht ADC-fähig ist oder intern belegt.
 */
int safeAnalogRead(int pin) {
    // Liste der sicheren ADC1-Pins auf dem M5Cardputer (GPIOs 12-15)
    // Andere Pins wie 1 und 2 werden oft intern genutzt (Taste/LED) und verursachen Fehler.
    if (pin >= 12 && pin <= 15) {
        return analogRead(pin);
    }
    
    // Für alle anderen Pins -1 zurückgeben, um den ADC-Konflikt zu vermeiden.
    // Insbesondere für Pin 1 (ENTER) und Pin 2 (LED)
    return -1; 
}


void drawPorts(){
    int actualLine = 1;
    int startY = 50;
    int NowX; // Tatsächliche X-Position der Spalte
    int startX = 10;
    
    // UI-Setup: Farben werden direkt über Menu.get*Color() abgerufen.
    M5Cardputer.Lcd.setTextColor(Global_ACCENT_COLOR, Global_BACKGROUND_COLOR);
M5Cardputer.Lcd.setTextDatum(TL_DATUM);
    for (int i = 0; i < NUM_Ports; i++) {
        
        // Berechnet die X-Position der Spalte (NowX)
        if(actualLine <= MaxLines){
          NowX = startX;
        }else if (actualLine <= (MaxLines *2)){
          NowX = startX + ColumSpace;
        }else if (actualLine <= (MaxLines *3)){
          NowX = startX + (ColumSpace *2);
        } else {
          // Begrenze auf 3 Spalten
          continue; 
        }

        int pin = PortList[i].Number;
        int PinDigitalState;
        String STRGPinDigitalState;
        int PinAnalogState;

        // Digitales Lesen: Funktioniert in der Regel für alle Pins
        //pinMode(pin, INPUT);
        PinDigitalState = digitalRead(pin);
        
        // Analoges Lesen (SICHER): Verwendet die ADC-sichere Funktion
        PinAnalogState = safeAnalogRead(pin);

        // Map nur, wenn ein gültiger Analogwert gelesen wurde
        String analogStr;
        if (PinAnalogState != -1) {
            PinAnalogState = map(PinAnalogState, 0, 4095, 0, 100);
            analogStr = String(PinAnalogState) + "%";
        } else {
            // Zeige nur digital
            analogStr = "DIGI"; 
        }

        if(PinDigitalState == 0){
            STRGPinDigitalState = "-L";
        }else if(PinDigitalState == 1){
            STRGPinDigitalState = "-H";
        }

        String PortString = String(pin) + STRGPinDigitalState + "-" + analogStr;
        
        // Zeichne den String auf den Bildschirm
        M5Cardputer.Lcd.drawString(PortString, NowX, startY);

        actualLine = actualLine +1;
        startY = startY + LineHeight;

        if(startY >= 120){ // Wenn die Y-Position für eine Zeile voll ist,
          startY = 50;     // setze Y zurück
        }
    }
}


String listenToPorts(){
    String RAWPortString = "";

    for (int i = 0; i < NUM_Ports; i++) {
        int pin = PortList[i].Number;
        int PinDigitalState = digitalRead(pin);
        int PinAnalogState = safeAnalogRead(pin);

        // Füge den Status zum Roh-String hinzu
        RAWPortString = RAWPortString + String(pin) + String(PinDigitalState) + String(PinAnalogState);
    }

  return(RAWPortString);
}

void runIOPortWatcherApp() {
    bool appIsRunning = true; 
    // Initialisiere die Pin-Modi hier, um die Wiederholung in listenToPorts/drawPorts zu vermeiden
    for (int i = 0; i < NUM_Ports; i++) {
        pinMode(PortList[i].Number, INPUT);
    }
    
    // UI-Setup
    Menu.drawHeader(currentRunningApp->name,"Press ESC to exit");
    M5Cardputer.Lcd.setTextDatum(TC_DATUM); 
    M5Cardputer.Lcd.drawString("Port - HIGH/LOW - Analog level", M5Cardputer.Lcd.width() / 2, MENU_START_Y +2);

    // Initiales Auslesen und Zeichnen
    String OldRawPortStrgg = "";
    String NewRawPortString = listenToPorts();
    
    while (appIsRunning) {
        M5Cardputer.update(); 
        
        NewRawPortString = listenToPorts();

        if (OldRawPortStrgg != NewRawPortString) {
          drawPorts(); // Neuzeichnen nur bei Veränderung der Werte
          OldRawPortStrgg = NewRawPortString;
        }

        if (M5Cardputer.Keyboard.isKeyPressed('`')) {
            appIsRunning = false;
        } 
        
        delay(Global_RefreshDelay);
    }

    // Nach dem Beenden der App:
    isMenuOpen = true; 
    Menu.redrawMenu(); 
}