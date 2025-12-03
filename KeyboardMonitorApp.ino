// KeyboardMonitorApp.ino - App: KeyboardMonitor

#include "02_Menu.h"

// Externe Deklaration des Flags
extern bool isMenuOpen; 
extern AppConfig* currentRunningApp;
//BackGround Color from Settings = Global_BACKGROUND_COLOR;
//Accent/Text Color from Settings = Global_ACCENT_COLOR;

// ----------------------------------------------------------------------
// KEYBOARD MONITOR APP
// ----------------------------------------------------------------------
void runKeyboardMonitor() {
    bool appIsRunning = true;
    M5Cardputer.Lcd.fillScreen(Global_BACKGROUND_COLOR);

    // Titelleiste zeichnen
    Menu.drawHeader(currentRunningApp->name,"Press 'Ctrl' + 'q' to exit."); 
    M5Cardputer.Lcd.drawString("Press any Keyboard Key", 20, MENU_START_Y +2);

    while (appIsRunning) {
        M5Cardputer.update();

        // Prüfen, ob eine Zustandsänderung vorliegt
        if (M5Cardputer.Keyboard.isChange()) {
            if (M5Cardputer.Keyboard.isPressed()) {
                Keyboard_Class::KeysState status = M5Cardputer.Keyboard.keysState();

                // 1. BEENDEN-LOGIK: Ctrl + q oder Backtick (`)
                if ((status.ctrl && M5Cardputer.Keyboard.isKeyPressed('Q'))) {
                    appIsRunning = false;
                    continue; // Springe aus dem Loop
                }

                // 2. TASTENAUSGABE
                // Lösche die Ausgabezeile
                M5Cardputer.Lcd.fillRect(0, 45, M5Cardputer.Lcd.width(), 100, Global_BACKGROUND_COLOR);
                M5Cardputer.Lcd.setCursor(10, 70); // Start der Char-Ausgabe

                // Ausgabe der gedrückten Zeichen
                for (char c : status.word) {
                    M5Cardputer.Lcd.printf("Char: '%c' (0x%02X) ", c, c);
                }

                // 3. Ausgabe der Flags (Anpassung der Ausrichtung)
                M5Cardputer.Lcd.setCursor(10, 85); // <-- ANGEPASST: X=10, Y=65
                M5Cardputer.Lcd.print("Flags: ");
                
                if (status.shift) M5Cardputer.Lcd.print("SHIFT ");
                if (status.ctrl) M5Cardputer.Lcd.print("CTRL ");
                if (status.alt) M5Cardputer.Lcd.print("ALT ");
                if (status.enter) M5Cardputer.Lcd.print("ENTER ");
                if (status.del) M5Cardputer.Lcd.print("DEL ");
                if (status.tab) M5Cardputer.Lcd.print("TAB ");
                if (status.fn) M5Cardputer.Lcd.print("FN ");
                if (status.opt) M5Cardputer.Lcd.print("OPT ");
                if (status.space) M5Cardputer.Lcd.print("SPACE ");
                
                // Wenn nichts gedrückt wurde, aber eine Änderung vorliegt (z.B. nur Flags)
                if (status.word.empty() && !status.shift && !status.ctrl && !status.alt && !status.enter && !status.del && !status.tab && !status.fn && !status.opt && !status.space) {
                    M5Cardputer.Lcd.print("No Char/Flag");
                }
            }
        }
        
        delay(Global_RefreshDelay);
    }

    // Nach dem Beenden der App:
    isMenuOpen = true; 
    Menu.redrawMenu(); // NEU: Erzwinge die Neuzeichnung des Menüs
}