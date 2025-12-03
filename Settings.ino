
// Settings.ino - App: Einstellungen

#include "02_Menu.h"

extern bool isMenuOpen; 
extern AppConfig* currentRunningApp;
//BackGround Color from Settings = Global_BACKGROUND_COLOR;
//Accent/Text Color from Settings = Global_ACCENT_COLOR;

// HINWEIS: Global_Brightnes muss als extern deklariert werden, da es in 01_Main.ino definiert ist.
extern int Global_Brightnes;


// ----------------------------------------------------------------------
// LOKALE EINSTELLUNGEN KONSTANTEN UND STRUKTUREN
// ----------------------------------------------------------------------

struct SettingItem {
const char* name;
// Hier könnten später Funktionen für die Einstellung (z.B. toggle, ändern) stehen.
};

// Liste der verfügbaren Einstellungen
const SettingItem settingsList[] = {
{"Set UI Color"},
{"Set Brightness"},
//{"System Info anzeigen"},
};

const int NUM_SETTINGS = sizeof(settingsList) / sizeof(settingsList[0]);
int selectedSettingIndex = 0; // Lokaler Index für das Einstellungsmenü

// ----------------------------------------------------------------------
// LOKALE HILFSFUNKTION: Zeichnet das Einstellungs-Untermenü
// ----------------------------------------------------------------------
void drawSettingsSubMenu(uint16_t bgColor, uint16_t uiColor) {
M5Cardputer.Lcd.fillScreen(bgColor);

// Titelleiste zeichnen
Menu.drawHeader(currentRunningApp->name,"Press ESC to exit"); 

for (int i = 0; i < NUM_SETTINGS; i++) {
    int yPos = MENU_START_Y + i * LINE_SPACING;
    
    if (i == selectedSettingIndex) {
        // Hervorhebung für die ausgewählte Einstellung
        M5Cardputer.Lcd.setTextColor(bgColor, uiColor); 
        M5Cardputer.Lcd.fillRect(0, yPos, M5Cardputer.Lcd.width(), LINE_SPACING, uiColor);
        M5Cardputer.Lcd.drawString(settingsList[i].name, 10, yPos + 2);
    } else {
        // Normale Ansicht
        M5Cardputer.Lcd.setTextColor(uiColor, bgColor);
        M5Cardputer.Lcd.drawString(settingsList[i].name, 10, yPos + 2);
    }
}
}


// ----------------------------------------------------------------------
// EINSTELLUNGEN APP (Hauptfunktion)
// ----------------------------------------------------------------------
void runSettingsApp() {
bool appIsRunning = true; 
bool SubMenuIsRunning = false; 

// Initiales Zeichnen des Untermenüs
drawSettingsSubMenu(Global_BACKGROUND_COLOR, Global_ACCENT_COLOR);

// setze den lokalen Index zurück
selectedSettingIndex = 0;

// --- INTERNE MENÜ-SCHLEIFE ---
while (appIsRunning) {
    M5Cardputer.update(); 
    if (M5Cardputer.Keyboard.isKeyPressed('`')) {
        appIsRunning = false;
        selectedSettingIndex = 0;
    }
    // Navigation und Auswahl nur bei gedrückter Taste verarbeiten
    if (M5Cardputer.Keyboard.isChange() && M5Cardputer.Keyboard.isPressed()) {
        Keyboard_Class::KeysState status = M5Cardputer.Keyboard.keysState();
        bool shouldRedraw = false;
    // Backtick (`) oder 'q' als universeller Notausstieg (direkt zum Hauptmenü)

        // 1. NAVIGATION (Pfeiltasten / Zeichen-Mapping)
        if (std::find(status.word.begin(), status.word.end(), ';') != status.word.end()) {
            // Pfeil HOCH (;)
            selectedSettingIndex = (selectedSettingIndex - 1 + NUM_SETTINGS) % NUM_SETTINGS;
            shouldRedraw = true;
        }else if (std::find(status.word.begin(), status.word.end(), '.') != status.word.end()) {
            // Pfeil RUNTER (.)
            selectedSettingIndex = (selectedSettingIndex + 1) % NUM_SETTINGS;
            shouldRedraw = true;
        }

        // 2. AUSWAHL (ENTER-Taste)
        if (status.enter) {
            // Die letzte Option ist "Zurück zum Hauptmenü"
            if (settingsList[selectedSettingIndex].name == "Zurueck zum Hauptmenue") {
                appIsRunning = false; // Beendet die App
            } else if (settingsList[selectedSettingIndex].name == "Set Brightness"){
                int LCD_Brighnes = 0;
                int newLCD_Brightnes = Global_Brightnes;
                int originalLCD_Brightnes = Global_Brightnes;
                SubMenuIsRunning = true; 
                int bMax = 255;
                int bMin = 7;
                M5Cardputer.Lcd.setFont(&fonts::Font2);
                M5Cardputer.Lcd.setTextColor(Global_ACCENT_COLOR, Global_BACKGROUND_COLOR);
                M5Cardputer.Lcd.fillRect(0, MENU_START_Y, M5Cardputer.Lcd.width(), M5Cardputer.Lcd.height() - MENU_START_Y, Global_BACKGROUND_COLOR);
                while (SubMenuIsRunning) {
                    M5Cardputer.update(); 

                    if (M5Cardputer.Keyboard.isKeyPressed(',')) {
                        newLCD_Brightnes = newLCD_Brightnes -1;
                    }else if (M5Cardputer.Keyboard.isKeyPressed('/')) {
                        newLCD_Brightnes = newLCD_Brightnes +1;
                    }
                    
                    if(newLCD_Brightnes > bMax){newLCD_Brightnes = bMax;}
                    if(newLCD_Brightnes < bMin){newLCD_Brightnes = bMin;}

                    if (M5Cardputer.Keyboard.isChange() && M5Cardputer.Keyboard.isPressed()) {
                        Keyboard_Class::KeysState status = M5Cardputer.Keyboard.keysState();
                        if (status.enter){
                            Global_Brightnes = newLCD_Brightnes; // Zuweisung ist nun möglich und Semikolon ist da!
                            save_settings();
                            SubMenuIsRunning = false;
                            shouldRedraw = true; 
                        }else if (std::find(status.word.begin(), status.word.end(), '`') != status.word.end()) {
                            SubMenuIsRunning = false;
                            shouldRedraw = true; 
                            M5Cardputer.Lcd.setBrightness(originalLCD_Brightnes);
                        }
                    }

                    if (LCD_Brighnes != newLCD_Brightnes) {
                        M5Cardputer.Lcd.setBrightness(newLCD_Brightnes);
                        String bVal = "Val: " + String(newLCD_Brightnes) + "  ";
                        M5Cardputer.Lcd.drawString(bVal, 20, 50);
                        M5Cardputer.Lcd.drawString("Use Left and Right so set Value", 20, 70);
                        M5Cardputer.Lcd.drawString("Press Enter to Save to EEPROM", 20, 90);
                        LCD_Brighnes = newLCD_Brightnes;
                    }
                    delay(Global_RefreshDelay);
                }
            }else if (settingsList[selectedSettingIndex].name == "Set UI Color"){
                M5Cardputer.Lcd.setFont(&fonts::Font2);

                
                SubMenuIsRunning = true; 
                int GradientHight = 15;
                int GradianWidth = 200;
                int GradiantXStart = 20;
                int colorpickerY = 90;
                int brightpickerY = 110;
                int colorpickerFrameY = colorpickerY+7;
                int brightpickerFrameY = brightpickerY+7;
                int SelectMax = (GradiantXStart + GradianWidth)-4; //halbe frame breite abzug
                int SelectMin = GradiantXStart+3; //halbe frame breite dazu
                int selectedColorPX = 0;
                int selectedBrightPX = 0;
                int selectedNewColorPX = 190;
                int selectedNewBrightPX = 170;
                int ModeColor = 1; //1 uiColor; 2 BG Color
                int OldModeColor = 0;
                uint16_t ColorPicker_Color;
                uint16_t ZIEL_FARBE;
                uint16_t Old_GBC = Global_BACKGROUND_COLOR;
                uint16_t Old_UIC = Global_ACCENT_COLOR;
                uint16_t FrameColor = 0xFFFF;

                while (SubMenuIsRunning) {
                    M5Cardputer.update(); 
                    


                    if (M5Cardputer.Keyboard.isKeyPressed(',')) { //inks
                        selectedNewColorPX = selectedNewColorPX -2;
                    }else if (M5Cardputer.Keyboard.isKeyPressed('/')) { //rechts
                        selectedNewColorPX = selectedNewColorPX +2;
                    }else if (M5Cardputer.Keyboard.isKeyPressed('c')) {
                        selectedNewBrightPX = selectedNewBrightPX +2;
                    }else if (M5Cardputer.Keyboard.isKeyPressed('z')) {
                        selectedNewBrightPX = selectedNewBrightPX -2;
                    }else if (M5Cardputer.Keyboard.isKeyPressed('1')) { // hoch
                        ModeColor = 1;
                    }else if (M5Cardputer.Keyboard.isKeyPressed('2')) { //runter
                      if(ModeColor == 1){
                    selectedNewBrightPX = (selectedNewBrightPX -GradiantXStart) /2;

                      }
                      
   
                        ModeColor = 2;
                    }
                    
                    if(selectedNewColorPX > SelectMax){selectedNewColorPX = SelectMax;}
                    if(selectedNewColorPX < SelectMin){selectedNewColorPX = SelectMin;}
                    if(selectedNewBrightPX > SelectMax){selectedNewBrightPX = SelectMax;}
                    if(selectedNewBrightPX < SelectMin){selectedNewBrightPX = SelectMin;}

                    if (M5Cardputer.Keyboard.isChange() && M5Cardputer.Keyboard.isPressed()) {
                        Keyboard_Class::KeysState status = M5Cardputer.Keyboard.keysState();
                        if (status.enter){
                            
                            save_settings();
                            SubMenuIsRunning = false;
                            shouldRedraw = true; 


                        }else if (std::find(status.word.begin(), status.word.end(), '`') != status.word.end()) {
                            SubMenuIsRunning = false;
                            shouldRedraw = true; 
                            Global_BACKGROUND_COLOR = Old_GBC;
                            Global_ACCENT_COLOR = Old_UIC;
                        }
                    }

                    if (selectedColorPX != selectedNewColorPX || selectedBrightPX != selectedNewBrightPX || OldModeColor != ModeColor) {
                        M5Cardputer.Lcd.fillRect(0, MENU_START_Y, M5Cardputer.Lcd.width(), M5Cardputer.Lcd.height() - MENU_START_Y, Global_BACKGROUND_COLOR);
                        drawColorGradient(GradiantXStart, colorpickerY, GradianWidth, GradientHight);
                        drawEmptyFrame(selectedNewColorPX, colorpickerFrameY, FrameColor);
                        
                        ColorPicker_Color = M5Cardputer.Lcd.readPixel(selectedNewColorPX, colorpickerFrameY);

                        drawColorGradientFrom(GradiantXStart, brightpickerY, GradianWidth, GradientHight, ColorPicker_Color);
                        drawEmptyFrame(selectedNewBrightPX, brightpickerFrameY, FrameColor);

                        ZIEL_FARBE = M5Cardputer.Lcd.readPixel(selectedNewBrightPX, brightpickerFrameY);
                        
                        if(ModeColor == 1){

                            Global_ACCENT_COLOR = ZIEL_FARBE;
                            M5Cardputer.Lcd.setTextColor(Global_ACCENT_COLOR, Global_BACKGROUND_COLOR);
                            M5Cardputer.Lcd.drawString("1/2 = Mode > UI Color >", 10, MENU_START_Y+30);
                        }else if(ModeColor == 2){

                            Global_BACKGROUND_COLOR = ZIEL_FARBE;
                            M5Cardputer.Lcd.setTextColor(Global_ACCENT_COLOR, Global_BACKGROUND_COLOR);
                            M5Cardputer.Lcd.drawString("1/2 = Mode > BG Color >", 10, MENU_START_Y+30);
                        }
                        
                        Menu.drawHeader(currentRunningApp->name,"ESC = exit    ENTER = save to EEPROM");
                        M5Cardputer.Lcd.drawString("Left/Right = Color, Z/C = Brightness", 10, MENU_START_Y+10);

                        String Hexoutpu = String(ZIEL_FARBE, HEX);
                        Hexoutpu.toUpperCase();
                        M5Cardputer.Lcd.drawString("0x" + Hexoutpu, 170, MENU_START_Y+30);

                        OldModeColor = ModeColor;
                        selectedColorPX = selectedNewColorPX;
                        selectedBrightPX = selectedNewBrightPX;
                    }

                    //if (LCD_Brighnes != newLCD_Brightnes) {}
                    delay(Global_RefreshDelay);
                }
            }else {
                // *** HIER KOMMT DIE LOGIK FÜR DIE GEWÄHLTE EINSTELLUNG ***
                // Wir tun im Moment nur so, als ob wir etwas tun würden.
                M5Cardputer.Lcd.fillScreen(Global_BACKGROUND_COLOR);
                M5Cardputer.Lcd.setFont(&fonts::Font4);
                M5Cardputer.Lcd.drawString("Auswahl...", 20, 20);
                M5Cardputer.Lcd.setFont(&fonts::Font2);
                M5Cardputer.Lcd.drawString(settingsList[selectedSettingIndex].name, 20, 50);
                M5Cardputer.Lcd.drawString("Waiting (2s)...", 20, 70);
                delay(2000); 
                
                // Nach der Bearbeitung das Menü neu zeichnen
                shouldRedraw = true; 
            }
        }

        // 3. Wenn eine Taste gedrückt wurde, die Navigation oder Auswahl auslöst
        if (shouldRedraw) {
            drawSettingsSubMenu(Global_BACKGROUND_COLOR, Global_ACCENT_COLOR);
        }
    }
    delay(Global_RefreshDelay); // Entlastung für den Prozesso
}
// --- ENDE INTERNE MENÜ-SCHLEIFE ---


// Nach dem Beenden der App:
isMenuOpen = true; // Setze den State zurück auf Menü
Menu.redrawMenu(); // Erzwinge die Neuzeichnung des Hauptmenüs
}