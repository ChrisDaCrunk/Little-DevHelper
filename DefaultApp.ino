// DefaultApp.ino - Die Standard-Platzhalter App

// currentRunningApp:
//      Nützlich, um den Namen der aktuell laufenden App abzurufen (z.B. für eine Titelleiste)
//      String currentAppName = currentRunningApp->name;

// load_settings():
//      Lädt und parst Einstellungen neu, setzt Global_Brightnes, BACKGROUND_COLOR, ACCENT_COLOR
//      load_settings(); 

// save_settings():
//      Speichert die aktuellen Global_Brightnes, BACKGROUND_COLOR, ACCENT_COLOR ins EEPROM
//      save_settings(); 

// save_defaults():
//      Setzt EEPROM auf Default_Settings zurück
//      save_defaults(); 

// read_Settings():
//      Liest den reinen Einstellungs-String aus dem EEPROM
//      String rawData = read_Settings();

// Variablenn zur feien verwendung, beeinflussen globales vehalten
// MAX_VISIBLE_APPS 5   //Max AppZeilen die auf eine seite passen
// MENU_HEADER_Hight 18 // Gesamthöhe des Headers (Titel + Trennlinie, ohne subtite(esc to exit))
// MENU_START_Y 30      // Y-Startposition der ersten Menüzeile (unter dem Header)
// LINE_SPACING 20      // Durchschnittliche Zeilenhöhe
// Global_Brightnes     // Liest die aktuelle helligkeit aus
// Global_RefreshDelay 60 //delay to stop LCD flicker and save cpu power
// Global_BACKGROUND_COLOR
// Global_ACCENT_COLOR

#include "02_Menu.h"

extern bool isMenuOpen; 
extern AppConfig* currentRunningApp;

//BackGround Color from Settings = Global_BACKGROUND_COLOR;
//Accent/Text Color from Settings = Global_ACCENT_COLOR;


void runDefaultApp() {
    bool appIsRunning = true; 
    
    M5Cardputer.Lcd.fillScreen(Global_BACKGROUND_COLOR);
    
    Menu.drawHeader(currentRunningApp->name,"Press ESC to exit"); 

    M5Cardputer.Lcd.setTextColor(Global_ACCENT_COLOR, Global_BACKGROUND_COLOR);
    M5Cardputer.Lcd.setFont(&fonts::Font2);
    M5Cardputer.Lcd.drawString("Nothing happend here....", 20, 50);
    M5Cardputer.Lcd.drawString("Press ESC to exit.", 20, 70);

    while (appIsRunning) {
        M5Cardputer.update(); 
        
        if (M5Cardputer.Keyboard.isKeyPressed('`')) {
            appIsRunning = false;
        }

        delay(Global_RefreshDelay); 
    }

    // Nach dem Beenden der App:
    isMenuOpen = true; 
    Menu.redrawMenu(); // NEU: Erzwinge die Neuzeichnung des Menüs
}