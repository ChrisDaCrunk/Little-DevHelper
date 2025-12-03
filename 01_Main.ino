// 01_Main.ino

#include "02_Menu.h"
#include <M5Cardputer.h> // Hinzugefügt, falls nicht bereits implizit vorhanden
#include <M5Unified.h>

// --- FORWARD DECLARATION FÜR APPS ---
void runSettingsApp();
//void runDefaultApp();     
void runKeyboardMonitor(); 
void runFontsApp(); 
void runCordinatesApp(); 
void runIOPortWatcherApp();
void runFontSizeApp();

void setup() {
    // Initialisierung der seriellen Schnittstelle für Debugging
    //Serial.begin(115200);
    M5.begin();
    // EEPROM initialisieren (wichtig für ESP32)
    EEPROM.begin(100); 
    M5Cardputer.begin(); // Initialisiert den Cardputer (Display, Tastatur)

    // 1. Initialisierung des Menüs (mit Hintergrund- und Akzentfarbe)
    Menu.initialize(0x880F, 0xFC1D, 144);

    // 2. Registrierung der Apps
    Menu.addApp("Settings", 0, runSettingsApp);
    Menu.addApp("Keyboard Monitor", 1, runKeyboardMonitor);
    Menu.addApp("Fonts", 2, runFontsApp);
    Menu.addApp("Font Size", 2, runFontSizeApp);
    Menu.addApp("Get Cordinates", 3, runCordinatesApp);
    Menu.addApp("I/O Watcher", 4, runIOPortWatcherApp);
    //Menu.addApp("DefaultAPP", 5, runDefaultApp);


    // 3. Menü einmal neu zeichnen, NACHDEM alle Apps registriert wurden.
    Menu.redrawMenu();
}

void loop() {
    M5Cardputer.update(); 
    
    // Überprüfe, ob das Menü aktiv ist.
    if (isMenuOpen) {
        // --- ZENTRALE TASTATURVERARBEITUNG FÜR MENÜ ---
        if (M5Cardputer.Keyboard.isChange() && M5Cardputer.Keyboard.isPressed()) {
            Keyboard_Class::KeysState status = M5Cardputer.Keyboard.keysState();

            // 1. NAVIGATION (Pfeiltasten / Zeichen-Mapping)
            if (std::find(status.word.begin(), status.word.end(), ';') != status.word.end()) {
                Menu.prevApp();
            } 
            else if (std::find(status.word.begin(), status.word.end(), '.') != status.word.end()) {
                Menu.nextApp();
            }

            // 2. APP STARTEN (ENTER-Taste)
            if (status.enter) {
                Menu.startSelectedApp();
            }
        }
        
        Menu.run();
    } else {
        // App läuft
    }
}