// CordinatesApp.ino - Die interaktive Koordinaten-App

#include "02_Menu.h"

extern bool isMenuOpen; 
extern AppConfig* currentRunningApp;
//BackGround Color from Settings = Global_BACKGROUND_COLOR;
//Accent/Text Color from Settings = Global_ACCENT_COLOR;

// HINWEIS: drawHUD ist in 04_DrawFunctions.cpp definiert und wird hier verwendet.
// Das HUD ist 9x9 Pixel groß und wird um seinen Mittelpunkt (hudX, hudY) gezeichnet.

void runCordinatesApp() {
    bool appIsRunning = true; 
    
    // --- LOKALE ZUSTANDSVARIABLEN (Position des HUD) ---
    // Startpunkt des HUD ist zentriert (LCD ist 240x135)
    int hudX = (M5Cardputer.Lcd.width() / 2); // Startpunkt: X = 120
    int hudY = (M5Cardputer.Lcd.height() / 2); // Startpunkt: Y = 67
    
    
    // Y-Grenzen: MIN_Y muss sicherstellen, dass hudY - 4 > MENU_START_Y ist.
    const int MIN_Y = MENU_START_Y; 
    // MAX_Y: 135 - 5 (Ende des Displays - halbe HUD-Größe - 1)
    const int MAX_Y = M5Cardputer.Lcd.height(); 
    
    // X-Grenzen: 0 + 4
    const int MIN_X = 0;
    // MAX_X: 240 - 5
    const int MAX_X = M5Cardputer.Lcd.width();
    
    // --- INITIALISIERUNG ---
    M5Cardputer.Lcd.fillScreen(Global_BACKGROUND_COLOR);
    
    // Header einmal zeichnen
    Menu.drawHeader(currentRunningApp->name,"ESC to Exit (W/A Up/Down to Move)"); 

    M5Cardputer.Lcd.setFont(&fonts::Font2);
    M5Cardputer.Lcd.setTextColor(Global_ACCENT_COLOR, Global_BACKGROUND_COLOR);
    
    // --- HILFSFUNKTION: ZEICHNEN DES GESAMTEN SCREENS ---
    auto drawScreen = [&]() {

        // 1. Zeichenfläche leeren (außer Header-Bereich)
        M5Cardputer.Lcd.fillRect(0, MENU_START_Y, M5Cardputer.Lcd.width(), M5Cardputer.Lcd.height() - MENU_START_Y, Global_BACKGROUND_COLOR);
        
        // 2. HUD zeichnen. Farbe ist TFT_RED, wie gewünscht.
        drawHUD(hudX, hudY, TFT_RED);

        // 3. Koordinaten in der Mitte ausgeben
        M5Cardputer.Lcd.setTextDatum(MC_DATUM); // Zentriert
        M5Cardputer.Lcd.setFont(&fonts::Font4);
        
        // Gebe die Zentrumskoordinaten aus
        String coords = "X: " + String(hudX) + " Y: " + String(hudY); 
        M5Cardputer.Lcd.drawString(coords.c_str(), M5Cardputer.Lcd.width() / 2, 85); // Auf Y=85 positioniert

        M5Cardputer.Lcd.setTextDatum(TL_DATUM); // Zurücksetzen auf Top-Left
        M5Cardputer.Lcd.setFont(&fonts::Font2);

        // 2. HUD zeichnen. Farbe ist TFT_RED, wie gewünscht.
        drawHUD(hudX, hudY, TFT_RED);
    };

    drawScreen(); // Erstes Zeichnen
    
    // --- INTERNE APP-SCHLEIFE ---
    
    // Statischer Zähler, um die Dauer des Tastendrucks zu verfolgen (wird nur einmal initialisiert)
    static uint32_t moveCounter = 0; 
    const uint32_t HOLD_FRAME_THRESHOLD = 10; // Nach 10 Frames (ca. 500ms) wird auf 4px erhöht.
    
    while (appIsRunning) {
        M5Cardputer.update(); 
        
        int oldX = hudX;
        int oldY = hudY;

        // Bestimme, ob eine der Bewegungstasten gedrückt ist
        bool movementKeyIsDown = M5Cardputer.Keyboard.isKeyPressed('w') || 
                                 M5Cardputer.Keyboard.isKeyPressed('a') || 
                                 M5Cardputer.Keyboard.isKeyPressed(',') || 
                                 M5Cardputer.Keyboard.isKeyPressed('/');

        // 1. BEENDEN-LOGIK Backtick (`)
        if (M5Cardputer.Keyboard.isKeyPressed('`')) {
            appIsRunning = false;
            break;
        }

        // 2. GESCHWINDIGKEITSLOGIK
        int moveStep = 0;
        
        if (movementKeyIsDown) {
            moveCounter++;
            
            // Wenn die Taste länger als der Schwellenwert gehalten wird (HOLD)
            if (moveCounter > HOLD_FRAME_THRESHOLD) { 
                moveStep = 4; // Schnelle Bewegung
            } else {
                moveStep = 1; // Langsame Bewegung (TAP-Phase)
            }
        } else {
            moveCounter = 0; // Zähler zurücksetzen, wenn keine Bewegungstaste gedrückt ist
        }

        // 3. STEUERUNG (Bewegung anwenden)
        if (M5Cardputer.Keyboard.isKeyPressed('w')) {
            hudY -= moveStep;
        } 
        else if (M5Cardputer.Keyboard.isKeyPressed('a')) {
            hudY += moveStep;
        } 
        else if (M5Cardputer.Keyboard.isKeyPressed(',')) {
            hudX -= moveStep;
        } 
        else if (M5Cardputer.Keyboard.isKeyPressed('/')) {
            hudX += moveStep;
        }
        
        
        // 4. RANDPRÜFUNG
        // Begrenze Y-Position nach oben (Header) und unten (Display-Ende)
        hudY = std::min(MAX_Y, std::max(MIN_Y, hudY)); 
        // Begrenze X-Position nach links und rechts
        hudX = std::min(MAX_X, std::max(MIN_X, hudX));

        // 5. NEUZEICHNEN, WENN SICH DIE POSITION GEÄNDERT HAT
        if (oldX != hudX || oldY != hudY) {
            drawScreen();
            
            // Header neu zeichnen, um Akku-Anzeige zu aktualisieren und 
            // Überreste des HUDs am oberen Rand zu beseitigen.
            Menu.drawHeader(currentRunningApp->name,"ESC to Exit (W/A Up/Down to Move)");
        }

        delay(Global_RefreshDelay); // Kurze Verzögerung für eine flüssige Steuerung
    }
    // --- ENDE INTERNE APP-SCHLEIFE ---


    // Nach dem Beenden der App:
    isMenuOpen = true; 
    Menu.redrawMenu(); 
}