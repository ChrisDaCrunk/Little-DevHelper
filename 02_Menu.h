// 02_Menu.h

#ifndef MENU_H
#define MENU_H

#include <EEPROM.h>
#include <M5Cardputer.h>
#include <vector>
#include <algorithm>
#include <cmath> // Für std::min/max


// ----------------------------------------------------------------------
// Layout Konstanten
// ----------------------------------------------------------------------
#define MAX_VISIBLE_APPS 5
#define MENU_HEADER_Hight 18 // Gesamthöhe des Headers (Titel + Trennlinie)
#define MENU_START_Y 30      // Y-Startposition der ersten Menüzeile (unter dem Header)
#define LINE_SPACING 20      // Höhe jeder Menüzeile in Pixeln

int         Global_RefreshDelay = 100;
uint16_t    Global_BACKGROUND_COLOR; 
uint16_t    Global_ACCENT_COLOR; 
int         Global_Brightnes; // Global Brightness ist jetzt eine Variable (int), keine Konstante (#define) mehr
bool isMenuOpen = true; // Status-Flag, ob das Menü aktiv ist (wird von Apps auf false gesetzt)
String Default_Settings = "";//// String besteht aus folgenden einstellungen "Helligkeit;BackgroundColor;AccentColor" 
String Settings_String = "";

// ----------------------------------------------------------------------
// Struktur zur Konfiguration einer App
// ----------------------------------------------------------------------
struct AppConfig {
    const char* name;
    int position;
    void (*runFunction)(void);
};

// ----------------------------------------------------------------------
// Cardputer Menu Klasse
// ----------------------------------------------------------------------
class CardputerMenu {
public:
    // Initialisierung des M5Cardputer und der Farben
    void initialize(uint16_t bgColor, uint16_t uiColor, int bright);

    // Zeichnet die Titelleiste
    void drawHeader(const char* title, const char* subtitle);

    // Registriert eine neue App beim Menü
    void addApp(const char* name, int position, void (*runFunction)(void));

    // Die Haupt-Run-Methode (Menü-State-Maschine)
    void run();
    
    // NEU: Erzwingt das Neuzeichnen des Menüs (zum Aufruf nach App-Ende)
    void redrawMenu();

    // Navigation (zum Aufruf von 01_Main.ino)
    void nextApp();
    void prevApp();
    
    // Startet die aktuell ausgewählte App
    void startSelectedApp();

private:
    std::vector<AppConfig> apps;
    int selectedIndex = 0;       // Index der aktuell ausgewählten App
    int currentTopIndex = 0;     // Index der App, die ganz oben gezeichnet wird (für Scrolling)
    
    // Zeichnet das gesamte Menü (mit Scrolling und Indikatoren)
    void drawMenu();
};

extern CardputerMenu Menu;
extern AppConfig* currentRunningApp;

// Die Deklaration 'extern bool isMenuOpen;' wurde entfernt, da die Definition nun in 01_Main.ino liegt.

#endif // MENU_H