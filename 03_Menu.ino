// Menu.ino

#include "02_Menu.h"

// Globale Instanz der Klasse (Implementierung)
CardputerMenu Menu;

// Die aktuelle laufende App (Implementierung)
AppConfig* currentRunningApp = nullptr;

extern bool isMenuOpen; 

// ----------------------------------------------------------------------
// Hilfsfunktion: Gibt den eindeutigen Build-Zeitstempel zurück
// ----------------------------------------------------------------------
const char* getBuildVersion() {
    // __DATE__ und __TIME__ sind Compiler-Makros.
    
    // 1. Definiere einen statischen Puffer, um den kombinierten String zu speichern.
    // "Build: " (7 Zeichen) + Leerzeichen (1) + Datum/Uhrzeit (20 Zeichen) + Null-Terminator (1)
    // 7 + 1 + 20 + 1 = 29 Zeichen Puffergröße ist sicher.
    static char versionBuffer[30]; 
    
    // 2. Verwende snprintf, um die Zeichenketten sicher zu kombinieren.
    // Das Ergebnis wird in versionBuffer geschrieben.
    // %s = String (konstante Zeichenkette)
    // %s = Datum und Uhrzeit
    snprintf(
        versionBuffer, 
        sizeof(versionBuffer), 
        "Build: %s %s", 
        __DATE__, 
        __TIME__
    );
    
    // 3. Gib den Zeiger auf den statisch gespeicherten String zurück.
    return versionBuffer;
}

String read_Settings(){
    char tempstring;
    String output_String;
    int i = 0;
    // Nur die ersten 100 Bytes lesen, da der String nicht länger sein sollte
    for(int i = 0; i < 100; i++){
        tempstring = EEPROM.read(i);
        if (tempstring == '\0') break; // Stoppe, wenn Null-Terminator erreicht
        output_String += String(tempstring);
    }
  return(output_String);
}

//// Defaults speichern" 
void save_defaults(){
    //Serial.print("Save Factory Defaults:");

    // Speichergröße initialisieren (EEPROM.begin(100) ist im setup notwendig!)
    // Hier nur speichern, da die Initialisierung in setup() erfolgen muss
    
    // String in EEPROM schreiben (mit Null-Terminator)
    for (int i = 0; i < Default_Settings.length(); i++){
      EEPROM.write(i, Default_Settings[i]);
    }
    EEPROM.write(Default_Settings.length(), '\0'); // Null-Terminator
    EEPROM.commit(); // Schreibvorgang abschließen
    
    Settings_String = Default_Settings;
    //Serial.println("DONE!");
    //Serial.println(Settings_String);
}

void save_settings(){
    //Serial.print("Save Settings:");
    // HINWEIS: Global_BACKGROUND_COLOR und Global_ACCENT_COLOR sind jetzt uint16_t Variablen.
    // Wir müssen die 0x-Präfixe für die Speicherung wieder hinzufügen, 
    // damit das Parsen beim Laden wieder funktioniert.
    
    String bgHex = String(Global_BACKGROUND_COLOR, HEX);
    while (bgHex.length() < 4) bgHex = "0" + bgHex; // Fülle mit Nullen auf, falls nötig
    
    String acHex = String(Global_ACCENT_COLOR, HEX);
    while (acHex.length() < 4) acHex = "0" + acHex; // Fülle mit Nullen auf, falls nötig
    
    // ACHTUNG: Wenn weitere Einstellungen hinzukommen, müssen sie hier angehängt werden!
    String NewSettings = String(Global_Brightnes) + ";" + "0x" + bgHex + ";" + "0x" + acHex;
    
    // String in EEPROM schreiben
    for (int i = 0; i < NewSettings.length(); i++){
      EEPROM.write(i, NewSettings[i]);
    }
    EEPROM.write(NewSettings.length(), '\0'); // Null-Terminator
    EEPROM.commit(); // Schreibvorgang abschließen
    //Serial.println(NewSettings);
    //Serial.println("DONE!");
}

void load_settings() {
    //################## CHECK SETTINGS #####################################################
    Settings_String = read_Settings();

    //############################ Defaults Speichern, falls EEPROM leer ist #######################################
    // Überprüfen Sie auf einen gültigen String (mindestens Brightness + 2 Separatoren und Werte)
    if(Settings_String.length() < 5 || Settings_String.indexOf(';') == -1){
        //Serial.println("Settings not found or invalid. Saving defaults.");
        save_defaults();
    }
   
    //############################ hier die settings übernehmen in variable#######################################
    if(Settings_String.length() > 0) {
        
        //Serial.println("Starting dynamic parsing...");
        
        int tokenIndex = 0;
        int current = 0;
        int next = -1;

        // Solange Trennzeichen gefunden werden
        while (current < Settings_String.length()) {
            
            // Finde das nächste Trennzeichen ';'
            next = Settings_String.indexOf(';', current);

            // Wenn kein Semikolon mehr gefunden wird, nimm den Rest des Strings
            if (next == -1) {
                next = Settings_String.length();
            }

            // Extrahiere den Token-String
            String token = Settings_String.substring(current, next);
            token.trim(); // Entferne Leerzeichen falls vorhanden

            if (token.length() > 0) {
                switch (tokenIndex) {
                    case 0: // Helligkeit
                        Global_Brightnes = token.toInt();
                        // Sicherstellen, dass die Helligkeit im gültigen Bereich liegt
                        Global_Brightnes = std::min(255, std::max(7, Global_Brightnes)); 
                        break;
                    
                    case 1: // Hintergrundfarbe
                        // strtoul konvertiert Hex-Strings (z.B. "0x0000") korrekt
                        Global_BACKGROUND_COLOR = (uint16_t)strtoul(token.c_str(), NULL, 16); 
                        break;

                    case 2: // Akzentfarbe
                        Global_ACCENT_COLOR = (uint16_t)strtoul(token.c_str(), NULL, 16);
                        break;
                    
                    default:
                        // Ignoriere alle zusätzlichen Tokens ab Index 3
                        //Serial.printf("Ignoring unknown setting at index %d: %s\n", tokenIndex, token.c_str());
                        break;
                }
                tokenIndex++;
            }
            
            // Bereite den nächsten Durchlauf vor (überspringe das Trennzeichen)
            current = next + 1;
            
            // Wenn wir das Ende des Strings erreicht haben, beende die Schleife
            if (next == Settings_String.length()) {
                break;
            }
        }
        
        //Serial.printf("Loaded Settings: Brightness=%d, BG=0x%X, AC=0x%X\n", Global_Brightnes, Global_BACKGROUND_COLOR, Global_ACCENT_COLOR);
    }
}

// ----------------------------------------------------------------------
// Hilfsfunktion: Zeichnet die Titelleiste
// ----------------------------------------------------------------------
void CardputerMenu::drawHeader(const char* title, const char* subtitle) {
    bool isCharging = M5Cardputer.Power.isCharging();
    int batteryLevel = M5Cardputer.Power.getBatteryLevel();
    String batteryLevel_string = String(batteryLevel) + " %";      // 0 - 100 %
    int batteryVoltage = M5Cardputer.Power.getBatteryVoltage();  // unit: mV

    M5Cardputer.Lcd.fillRect(0, 0, M5Cardputer.Lcd.width(), MENU_START_Y, Global_BACKGROUND_COLOR);
    
    // Linie zur Abtrennung bei Pixelhöhe MENU_HEADER_Hight - 1 (17)
    M5Cardputer.Lcd.drawLine(0, MENU_HEADER_Hight - 1, M5Cardputer.Lcd.width(), MENU_HEADER_Hight - 1, Global_ACCENT_COLOR);
    M5Cardputer.Lcd.drawLine(0, MENU_START_Y -1, M5Cardputer.Lcd.width(), MENU_START_Y - 1, Global_ACCENT_COLOR);
    // Titel zentriert zeichnen
    M5Cardputer.Lcd.setTextDatum(MC_DATUM); // Middle Center
    M5Cardputer.Lcd.setTextColor(Global_ACCENT_COLOR, Global_BACKGROUND_COLOR);
    // Verwenden von Font2 für den Header
    M5Cardputer.Lcd.setFont(&fonts::Font2); 
    M5Cardputer.Lcd.drawString(title, M5Cardputer.Lcd.width() / 2, 8); // 8 ist vertikal zentriert bei Höhe 17
    
    M5Cardputer.Lcd.setFont(&fonts::Font0); 
    M5Cardputer.Lcd.drawString(subtitle, M5Cardputer.Lcd.width() / 2, 24); // 8 ist vertikal zentriert bei Höhe 17

    M5Cardputer.Lcd.setTextDatum(CL_DATUM); // Zurücksetzen auf Top Left
        // Einrichtung des Lcd-Canvas für Scroll-Ausgabe
    M5Cardputer.Lcd.setFont(&fonts::Font2);
    M5Cardputer.Lcd.drawString(batteryLevel_string, 2, 8);
    
    M5Cardputer.Lcd.setFont(&fonts::Font2);
    M5Cardputer.Lcd.setTextDatum(TL_DATUM); // Zurücksetzen auf Top Left
}

// ----------------------------------------------------------------------
// Methode: INITIALIZE
// ----------------------------------------------------------------------
void CardputerMenu::initialize(uint16_t bgColor, uint16_t uiColor, int bright) {
    M5Cardputer.begin();

    String HexoutputBG = String(bgColor, HEX);
    String HexoutputUI = String(uiColor, HEX);   
    HexoutputBG.toUpperCase();
    HexoutputUI.toUpperCase();

    Default_Settings = String(bright) + ";" + "0x" + HexoutputBG + ";" + "0x" + HexoutputUI;
    load_settings();
    
    // M5Cardputer.Lcd.setBrightness(.  aus string.  );
    M5Cardputer.Lcd.setBrightness(Global_Brightnes);
    M5Cardputer.Lcd.fillScreen(Global_BACKGROUND_COLOR);
    M5Cardputer.Lcd.setTextColor(Global_ACCENT_COLOR, Global_BACKGROUND_COLOR);
    M5Cardputer.Lcd.setFont(&fonts::Font2); 
    
    // Beim Initialisieren muss das Menü einmal gezeichnet werden.
    drawMenu();
}

// ----------------------------------------------------------------------
// Methode: ADD APP (Signatur ohne Logo)
// ----------------------------------------------------------------------
void CardputerMenu::addApp(const char* name, int position, void (*runFunction)(void)) {
    AppConfig newApp = {
        name,
        position,
        runFunction
    };
    apps.push_back(newApp);

    // Sortiere den Vektor basierend auf der 'position'
    std::sort(apps.begin(), apps.end(), [](const AppConfig& a, const AppConfig& b) {
        return a.position < b.position;
    });
}

// ----------------------------------------------------------------------
// Methode: nextApp (Steuerung mit Scrolling)
// ----------------------------------------------------------------------
void CardputerMenu::nextApp() {
    if (apps.empty()) return;
    
    // Gehe zum nächsten Eintrag
    selectedIndex = (selectedIndex + 1); 

    // Wickeln, falls das Ende erreicht ist (Gehe zur ersten App)
    if (selectedIndex >= apps.size()) {
        selectedIndex = 0;
        currentTopIndex = 0; // Scrolle zurück zum Anfang
    }
    
    // Wenn die Auswahl das Ende des sichtbaren Bereichs erreicht, scrolle nach unten
    if (selectedIndex >= currentTopIndex + MAX_VISIBLE_APPS) {
        currentTopIndex++;
    }

    drawMenu();
}

// ----------------------------------------------------------------------
// Methode: prevApp (Steuerung mit Scrolling)
// ----------------------------------------------------------------------
void CardputerMenu::prevApp() {
    if (apps.empty()) return;

    // Gehe zum vorherigen Eintrag
    selectedIndex = (selectedIndex - 1); 
    
    // Wickeln, falls der Anfang erreicht ist (Gehe zur letzten App)
    if (selectedIndex < 0) {
        selectedIndex = apps.size() - 1;
        // Scrolle, sodass die letzte App sichtbar ist
        currentTopIndex = std::max(0, (int)apps.size() - MAX_VISIBLE_APPS); 
    }

    // Wenn die Auswahl den Anfang des sichtbaren Bereichs erreicht, scrolle nach oben
    if (selectedIndex < currentTopIndex) {
        currentTopIndex--;
    }
    
    drawMenu();
}

// ----------------------------------------------------------------------
// Methode: startSelectedApp (Startet die App)
// ----------------------------------------------------------------------
void CardputerMenu::startSelectedApp() {
    if (apps.empty()) return;

    // Starte die ausgewählte App
    currentRunningApp = &apps[selectedIndex];
    isMenuOpen = false;
    M5Cardputer.Lcd.fillScreen(Global_BACKGROUND_COLOR); // Bildschirm vor App-Start leeren
    currentRunningApp->runFunction(); // RUFT DIE APP AUF!
    // Hier wird die App-Funktion blockieren, bis sie zurückkehrt (z.B. bei 'B'-Taste)
}

// ----------------------------------------------------------------------
// NEU: Methode: redrawMenu (Erzwingt das Neuzeichnen)
// ----------------------------------------------------------------------
void CardputerMenu::redrawMenu() {
    drawMenu();
}



// ----------------------------------------------------------------------
// Hilfsfunktion: Zeichnet das Menü (mit Scrolling und Indikatoren)
// ----------------------------------------------------------------------
void CardputerMenu::drawMenu() {
    M5Cardputer.Lcd.fillScreen(Global_BACKGROUND_COLOR); // Ganzen Bildschirm löschen
    
    drawHeader("Little DevHelper",getBuildVersion()); 

    if (apps.empty()) {
        M5Cardputer.Lcd.setTextColor(TFT_RED);
        M5Cardputer.Lcd.drawString("Keine Apps registriert!", 10, MENU_START_Y);
        return;
    }

    // Bestimme, welche Apps gezeichnet werden sollen
    size_t endIndex = std::min(apps.size(), (size_t)(currentTopIndex + MAX_VISIBLE_APPS));

    // 1. App-Namen links zeichnen (Liste)
    for (size_t i = currentTopIndex; i < endIndex; ++i) {
        // Berechne den Index im sichtbaren Fenster (0 bis MAX_VISIBLE_APPS-1)
        int visibleIndex = i - currentTopIndex; 
        int yPos = MENU_START_Y + visibleIndex * LINE_SPACING;

        if (i == selectedIndex) {
            // Hervorhebung
            M5Cardputer.Lcd.setTextColor(Global_BACKGROUND_COLOR, Global_ACCENT_COLOR); 
            M5Cardputer.Lcd.fillRect(0, yPos, M5Cardputer.Lcd.width(), LINE_SPACING, Global_ACCENT_COLOR);
            M5Cardputer.Lcd.drawString(apps[i].name, 10, yPos + 2); // Leichte Y-Korrektur für bessere Zentrierung im Feld
        } else {
            // Normale Ansicht
            M5Cardputer.Lcd.setTextColor(Global_ACCENT_COLOR, Global_BACKGROUND_COLOR);
            M5Cardputer.Lcd.drawString(apps[i].name, 10, yPos + 2);
        }
    }
    
    // 2. SCROLL-INDIKATOREN (Pfeile) zeichnen
    int arrowX = M5Cardputer.Lcd.width() - 15; // Position X am rechten Rand
    
    // Oben-Pfeil (Wenn wir nicht die erste App sehen, d.h. currentTopIndex > 0)
    if (currentTopIndex > 0) {
        // Zeichne den Pfeil zentriert zwischen MENU_HEADER_Hight und MENU_START_Y
        int arrowY = MENU_START_Y + 3; // -3 um den Pfeil zu zentrieren
        drawUpArrow(arrowX, arrowY, Global_ACCENT_COLOR);
    }

    // Unten-Pfeil (Wenn die letzte App nicht sichtbar ist)
    if (apps.size() > (size_t)currentTopIndex + MAX_VISIBLE_APPS) {
        // Zeichne den Pfeil in der Mitte der letzten sichtbaren Zeile
        int arrowY = MENU_START_Y + (MAX_VISIBLE_APPS - 1) * LINE_SPACING + (LINE_SPACING / 2) - 3;
        drawDownArrow(arrowX, arrowY, Global_ACCENT_COLOR);
    }
}

// ----------------------------------------------------------------------
// Methode: RUN (Der State Machine Handler)
// ----------------------------------------------------------------------
void CardputerMenu::run() {
    if (isMenuOpen) {
        // Keine "initialDraw"-Logik mehr nötig.
        // Die initialen und rückkehrenden Draws werden explizit über
        // initialize() und redrawMenu() ausgelöst.
    } else {
        // App ist aktiv, das Menü wartet
    }
}

