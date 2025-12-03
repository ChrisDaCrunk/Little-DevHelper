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


void runFontSizeApp() {
    bool appIsRunning = true; 
    float FontMaxSize = 7.5;
    float FontMinSize = 1;
    float FontNowSize = 2;
    int screenOffset = 10;
    float Fontsize;

    M5Cardputer.Lcd.fillScreen(Global_BACKGROUND_COLOR);
    
    Menu.drawHeader(currentRunningApp->name,"ESC to exit  Left/Right ch. TextSize"); 

    M5Cardputer.Lcd.setTextColor(Global_ACCENT_COLOR, Global_BACKGROUND_COLOR);
    //M5Cardputer.Lcd.setFont(&fonts::Font2);
    //M5Cardputer.Lcd.drawString("Nothing happend here....", 20, 50);
    //M5Cardputer.Lcd.drawString("Press ESC to exit.", 20, 70);

    while (appIsRunning) {
        M5Cardputer.update(); 

        if(FontNowSize != Fontsize){
          M5Cardputer.Display.setCursor(0, MENU_START_Y -screenOffset);
          M5Cardputer.Display.setTextSize(FontNowSize);
          M5Cardputer.Lcd.fillRect(0, MENU_START_Y -screenOffset, M5Cardputer.Lcd.width(), M5Cardputer.Lcd.height(), Global_BACKGROUND_COLOR);

          if(FontNowSize == 1){
            M5Cardputer.Display.println("123456789-123456789-1234567890");
            M5Cardputer.Display.println("2");
            M5Cardputer.Display.println("3");
            M5Cardputer.Display.println("4");
            M5Cardputer.Display.println("5");
            M5Cardputer.Display.println("6");
            M5Cardputer.Display.println("7");
          }else if(FontNowSize == 1.5){
            M5Cardputer.Display.println("123456789-123456789-");
            M5Cardputer.Display.println("2");
            M5Cardputer.Display.println("3");
            M5Cardputer.Display.println("4");
            M5Cardputer.Display.println("5");
          }else if(FontNowSize == 2){
            M5Cardputer.Display.println("123456789-12345");
            M5Cardputer.Display.println("2");
            M5Cardputer.Display.println("3");
            M5Cardputer.Display.println("4");
          }else if(FontNowSize == 2.5){
            M5Cardputer.Display.println("123456789-12");
            M5Cardputer.Display.println("2");
            M5Cardputer.Display.println("3");
          }else if(FontNowSize == 3){
            M5Cardputer.Display.println("1234567890");
            M5Cardputer.Display.println("2");
            M5Cardputer.Display.println("3");
          }else if(FontNowSize == 3.5){
            M5Cardputer.Display.println("12345678");
            M5Cardputer.Display.println("2");
          }else if(FontNowSize == 4){
            M5Cardputer.Display.println("1234567");
            M5Cardputer.Display.println("2");
          }else if(FontNowSize == 4.5){
            M5Cardputer.Display.println("123456");
            M5Cardputer.Display.println("2");
          }else if(FontNowSize == 5){
            M5Cardputer.Display.println("123456");
            M5Cardputer.Display.println("2");
          }else if(FontNowSize == 5.5){
            M5Cardputer.Display.println("12345");
            M5Cardputer.Display.println("2");
          }else if(FontNowSize == 6){
            M5Cardputer.Display.println("12345");
          }else if(FontNowSize == 6.5){
            M5Cardputer.Display.println("1234");
          }else if(FontNowSize == 7){
            M5Cardputer.Display.println("1234");
          }else if(FontNowSize == 7.5){
            M5Cardputer.Display.println("1234");
          }


          M5Cardputer.Display.setTextSize(2);
          M5Cardputer.Lcd.drawString(String(FontNowSize), 110, 70);
          M5Cardputer.Display.setTextSize(1);
          M5Cardputer.Lcd.drawString("ESC to EXIT", 110, 100);
          M5Cardputer.Lcd.drawString("Left/Right set Size", 110, 115);

          Fontsize = FontNowSize;
        }


        if (M5Cardputer.Keyboard.isKeyPressed('`')) {
            appIsRunning = false;
        }else if (M5Cardputer.Keyboard.isKeyPressed(',')) { //links
            FontNowSize = FontNowSize -0.5;
        }else if (M5Cardputer.Keyboard.isKeyPressed('/')) { //rechts
            FontNowSize = FontNowSize +0.5;
        }

        if(FontNowSize < FontMinSize){
            FontNowSize = FontMinSize;
        }else if(FontNowSize > FontMaxSize){
            FontNowSize = FontMaxSize;
        }
        delay(Global_RefreshDelay); 
    }

    // Nach dem Beenden der App:
    isMenuOpen = true; 
    Menu.redrawMenu(); // NEU: Erzwinge die Neuzeichnung des Menüs
}