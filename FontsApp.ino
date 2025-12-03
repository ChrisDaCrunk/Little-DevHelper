// FontsApp.ino - App: Fonts

#include "02_Menu.h"
#include <algorithm> // Fuer std::min und std::max (fuer Scrolling-Logik)
#include <cmath> // Fuer std::max (falls <algorithm> nicht ausreicht)

// Externe Deklaration des Flags
extern bool isMenuOpen; 
extern AppConfig* currentRunningApp;
//BackGround Color from Settings = Global_BACKGROUND_COLOR;
//Accent/Text Color from Settings = Global_ACCENT_COLOR;

// ----------------------------------------------------------------------
// LOKALE FONT KONSTANTEN UND STRUKTUREN
// ----------------------------------------------------------------------

struct FontsItem {
    const char* name;       // Name der Schriftart für die Anzeige
    const GFXfont* fontPtr; // Pointer zur eigentlichen Schriftart-Definition
};

// Liste der verfügbaren Schriftarten
// WICHTIG: Expliziter Cast (const GFXfont*) für die nicht-Adafruit-GFX-kompatiblen Fonts
// ist notwendig, um den Konvertierungsfehler zu beheben.
const FontsItem FontsList[] = {
    {"Font2", (const GFXfont*)&fonts::Font2},           // FIX: Expliziter Cast hinzugefügt
    {"AsciiFont8x16", (const GFXfont*)&fonts::AsciiFont8x16}, // FIX: Expliziter Cast hinzugefügt
    {"TomThumb", &fonts::TomThumb},
    {"FreeMono9pt7b", &fonts::FreeMono9pt7b},
    {"FreeMonoBold9pt7b", &fonts::FreeMonoBold9pt7b},
    {"FreeMonoOblique9pt7b", &fonts::FreeMonoOblique9pt7b},
    {"FreeMonoBoldOblique9pt7b", &fonts::FreeMonoBoldOblique9pt7b},
    {"FreeSans9pt7b", &fonts::FreeSans9pt7b},
    {"FreeSansBold9pt7b", &fonts::FreeSansBold9pt7b},
    {"FreeSansOblique9pt7b", &fonts::FreeSansOblique9pt7b},
    {"FreeSansBoldOblique9pt7b", &fonts::FreeSansBoldOblique9pt7b},
    {"FreeSerif9pt7b", &fonts::FreeSerif9pt7b},
    {"FreeSerifItalic9pt7b", &fonts::FreeSerifItalic9pt7b},
    {"FreeSerifBold9pt7b", &fonts::FreeSerifBold9pt7b},
    {"FreeSerifBoldItalic9pt7b", &fonts::FreeSerifBoldItalic9pt7b},
    // Achtung: Große Fonts brauchen mehr Platz und können zu Überlappungen führen!
    {"Orbitron_Light_24", &fonts::Orbitron_Light_24}, 
    {"Roboto_Thin_24", &fonts::Roboto_Thin_24},
    {"Satisfy_24", &fonts::Satisfy_24},
    {"Yellowtail_32", &fonts::Yellowtail_32},
    {"DejaVu9", &fonts::DejaVu9}
};

const int NUM_FONTS = sizeof(FontsList) / sizeof(FontsList[0]);
int selectedFontsIndex = 0; // Lokaler Index für das Menü
int currentTopFontIndex = 0; // Index der obersten sichtbaren Schriftart (für Scrolling)
const int MAX_VISIBLE_FONTS = 5; // Mehr Platz für größere Fonts lassen

// ----------------------------------------------------------------------
// LOKALE HILFSFUNKTION: Zeichnet das Font-Menü
// ----------------------------------------------------------------------
void drawFonts(uint16_t bgColor, uint16_t uiColor) {
    M5Cardputer.Lcd.fillScreen(bgColor);
    
    // Titelleiste zeichnen (nimmt den App-Namen automatisch)
    Menu.drawHeader(currentRunningApp->name,"Press ESC to exit"); 
    
    // Wir nutzen einen Standard-Font für die Anzeige der Menüliste
    M5Cardputer.Lcd.setFont((const GFXfont*)&fonts::Font2); // FIX: Cast hier auch
    
    int startY = MENU_START_Y;
    int spacing = 20;

    // Bestimme den sichtbaren Bereich für Scrolling
    size_t endIndex = std::min((size_t)NUM_FONTS, (size_t)(currentTopFontIndex + MAX_VISIBLE_FONTS));

    for (size_t i = currentTopFontIndex; i < endIndex; i++) {
        // Berechne den Index im sichtbaren Fenster
        int visibleIndex = i - currentTopFontIndex; 
        int yPos = startY + visibleIndex * spacing;
        
        // --- Anzeigefeld des Eintrags ---
        if (i == selectedFontsIndex) {
            // Hervorhebung: Hintergrundfarbe (Schwarz) auf Akzentfarbe (Weiß)
            M5Cardputer.Lcd.setTextColor(bgColor, uiColor); 
            M5Cardputer.Lcd.fillRect(0, yPos, M5Cardputer.Lcd.width(), spacing, uiColor);
            M5Cardputer.Lcd.setFont(FontsList[i].fontPtr);
            // Text zeichnen (Font-Name)
            M5Cardputer.Lcd.drawString(FontsList[i].name, 5, yPos + 2);

        } else {
            M5Cardputer.Lcd.setFont(FontsList[i].fontPtr);
            // Normale Ansicht: Akzentfarbe (Weiß) auf Hintergrundfarbe (Schwarz)
            M5Cardputer.Lcd.setTextColor(uiColor, bgColor);
            M5Cardputer.Lcd.fillRect(0, yPos, M5Cardputer.Lcd.width(), spacing, bgColor);
            
            // Text zeichnen
            M5Cardputer.Lcd.drawString(FontsList[i].name, 5, yPos + 2);
        }
    }
    
    // Rücksetzen auf den Standard-Font für Debug-Meldungen/UI
   // M5Cardputer.Lcd.setFont((const GFXfont*)&fonts::Font2); // FIX: Cast hier auch
    //M5Cardputer.Lcd.setTextColor(uiColor, bgColor);
   // M5Cardputer.Lcd.drawString("Enter zum Anwenden", 5, M5Cardputer.Lcd.height() - 20); // Infozeile
}


// ----------------------------------------------------------------------
// FONT APP (Hauptfunktion)
// ----------------------------------------------------------------------
void runFontsApp() {
    bool appIsRunning = true; 
    
    uint16_t bgColor = Global_BACKGROUND_COLOR;
    uint16_t uiColor = Global_ACCENT_COLOR;

    // Initiales Zeichnen des Untermenüs
    drawFonts(bgColor, uiColor);
    
    // setze den lokalen Index zurück
    selectedFontsIndex = 0;
    currentTopFontIndex = 0;

    // --- INTERNE MENÜ-SCHLEIFE ---
    while (appIsRunning) {
        M5Cardputer.update(); 
        
        // Navigation und Auswahl nur bei gedrückter Taste verarbeiten
        if (M5Cardputer.Keyboard.isChange() && M5Cardputer.Keyboard.isPressed()) {
            Keyboard_Class::KeysState status = M5Cardputer.Keyboard.keysState();
            bool shouldRedraw = false;

            // 1. NAVIGATION (Pfeiltasten / Zeichen-Mapping)
            if (std::find(status.word.begin(), status.word.end(), ';') != status.word.end()) {
                // Pfeil HOCH (;)
                selectedFontsIndex = (selectedFontsIndex - 1 + NUM_FONTS) % NUM_FONTS;
                
                // Scrolling nach oben
                if (selectedFontsIndex < currentTopFontIndex) {
                    currentTopFontIndex = selectedFontsIndex;
                } else if (currentTopFontIndex == NUM_FONTS - MAX_VISIBLE_FONTS && selectedFontsIndex == NUM_FONTS - 1) {
                    // Von oben nach ganz unten gesprungen (Wrap-Around-Fall)
                    currentTopFontIndex = std::max(0, NUM_FONTS - MAX_VISIBLE_FONTS);
                }
                shouldRedraw = true;
            } 
            else if (std::find(status.word.begin(), status.word.end(), '.') != status.word.end()) {
                // Pfeil RUNTER (.)
                selectedFontsIndex = (selectedFontsIndex + 1) % NUM_FONTS;
                
                // Scrolling nach unten
                if (selectedFontsIndex >= currentTopFontIndex + MAX_VISIBLE_FONTS) {
                    currentTopFontIndex++;
                } else if (currentTopFontIndex != 0 && selectedFontsIndex == 0) {
                     // Von unten nach ganz oben gesprungen (Wrap-Around-Fall)
                    currentTopFontIndex = 0;
                }
                shouldRedraw = true;
            }

            if (shouldRedraw) {
                drawFonts(bgColor, uiColor);
            }
        }
        
        // Backtick (`) oder 'q' als universeller Notausstieg (direkt zum Hauptmenü)
        if (M5Cardputer.Keyboard.isKeyPressed('`')) {
            appIsRunning = false;
        }


        delay(Global_RefreshDelay); // Entlastung für den Prozessor
    }
    // --- ENDE INTERNE MENÜ-SCHLEIFE ---


    // Nach dem Beenden der App:
    isMenuOpen = true; // Setze den State zurück auf Menü
    Menu.redrawMenu(); // Erzwinge die Neuzeichnung des Hauptmenüs
}