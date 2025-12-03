// Assets.ino

// ----------------------------------------------------------------------
// PRIVATE Hilfsfunktion: Zeichnet den Pfeil nach oben (5x6 Pixel, zeigt nach OBEN)
// ----------------------------------------------------------------------
void drawUpArrow(int x, int y, uint16_t color) {
auto& lcd = M5Cardputer.Lcd;
// Der Pfeil zeigt nach OBEN (Punkt oben, Basis unten)
// Zeile 0: --#--
lcd.drawPixel(x + 2, y + 0, color);
// Zeile 1: -###-
lcd.drawPixel(x + 1, y + 1, color);
lcd.drawPixel(x + 2, y + 1, color);
lcd.drawPixel(x + 3, y + 1, color);
// Zeile 2: ##### (Basis)
lcd.drawPixel(x + 0, y + 2, color);
lcd.drawPixel(x + 1, y + 2, color);
lcd.drawPixel(x + 2, y + 2, color);
lcd.drawPixel(x + 3, y + 2, color);
lcd.drawPixel(x + 4, y + 2, color);
// Zeile 3: --#--
lcd.drawPixel(x + 2, y + 3, color);
// Zeile 4: --#--
lcd.drawPixel(x + 2, y + 4, color);
// Zeile 5: --#-- (Punkt)
lcd.drawPixel(x + 2, y + 5, color);
}

// ----------------------------------------------------------------------
// PRIVATE Hilfsfunktion: Zeichnet den Pfeil nach unten (5x6 Pixel, zeigt nach UNTEN)
// ----------------------------------------------------------------------
void drawDownArrow(int x, int y, uint16_t color) {
auto& lcd = M5Cardputer.Lcd;
// Der Pfeil zeigt nach UNTEN (Basis oben, Punkt unten)
// Zeile 5: --#-- (Punkt)
lcd.drawPixel(x + 2, y + 0, color);
// Zeile 4: --#--
lcd.drawPixel(x + 2, y + 1, color);
// Zeile 3: --#--
lcd.drawPixel(x + 2, y + 2, color);
// Zeile 2: ##### (Basis)
lcd.drawPixel(x + 0, y + 3, color);
lcd.drawPixel(x + 1, y + 3, color);
lcd.drawPixel(x + 2, y + 3, color);
lcd.drawPixel(x + 3, y + 3, color);
lcd.drawPixel(x + 4, y + 3, color);
// Zeile 1: -###-
lcd.drawPixel(x + 1, y + 4, color);
lcd.drawPixel(x + 2, y + 4, color);
lcd.drawPixel(x + 3, y + 4, color);
// Zeile 0: --#--
lcd.drawPixel(x + 2, y + 5, color);
}

void drawHUD(int x, int y, uint16_t color) {
auto& lcd = M5Cardputer.Lcd;
//   -4 -3 -2 -1  0  1  2  3  4
// -4 -  -  -  -  #  -  -  -  -
// -3 -  -  -  -  #  -  -  -  -
// -2 -  -  -  -  #  -  -  -  - 
// -1 -  -  -  -  -  -  -  -  -
//  0 #  #  #  -  -  -  #  #  #
//  1 -  -  -  -  -  -  -  -  -
//  2 -  -  -  -  #  -  -  -  -
//  3 -  -  -  -  #  -  -  -  -
//  4 -  -  -  -  #  -  -  -  -

lcd.drawPixel(x +0, y -4, color);
lcd.drawPixel(x +0, y -3, color);
lcd.drawPixel(x +0, y -2, color);
lcd.drawPixel(x -4, y +0, color);
lcd.drawPixel(x -3, y +0, color);
lcd.drawPixel(x -2, y +0, color);
lcd.drawPixel(x +2, y +0, color);
lcd.drawPixel(x +3, y +0, color);
lcd.drawPixel(x +4, y +0, color);
lcd.drawPixel(x +0, y +2, color);
lcd.drawPixel(x +0, y +3, color);
lcd.drawPixel(x +0, y +4, color);

}

void drawEmptyFrame(int x, int y, uint16_t color) {
auto& lcd = M5Cardputer.Lcd;
//      -3 -2 -1  0  1  2  3
// -7 -  -  #  #  #  #  #  -
// -6 -  #  #  #  -  #  #  #
// -5 -  #  #  -  -  -  #  #
// -4 -  #  #  -  -  -  #  #
// -3 -  #  #  -  -  -  #  #
// -2 -  #  #  -  -  -  #  #
// -1 -  #  #  -  -  -  #  #
//  0 -  #  #  -  -  -  #  #
//  1 -  #  #  -  -  -  #  #
//  2 -  #  #  -  -  -  #  #
//  3 -  #  #  -  -  -  #  #
//  4 -  #  #  -  -  -  #  #
//  5 -  #  #  -  -  -  #  #
//  6 -  #  #  #  -  #  #  #
//  7 -  -  #  #  #  #  #  -


// -7 -  -  #  #  #  #  #  -
lcd.drawPixel(x -2, y -7, color);
lcd.drawPixel(x -1, y -7, color);
lcd.drawPixel(x +0, y -7, color);
lcd.drawPixel(x +1, y -7, color);
lcd.drawPixel(x +2, y -7, color);
// -6 -  #  #  #  -  #  #  #
lcd.drawPixel(x -3, y -6, color);
lcd.drawPixel(x -2, y -6, color);
lcd.drawPixel(x -1, y -6, color);
lcd.drawPixel(x +1, y -6, color);
lcd.drawPixel(x +2, y -6, color);
lcd.drawPixel(x +3, y -6, color);
// -5 -  #  #  -  -  -  #  #
lcd.drawPixel(x -3, y -5, color);
lcd.drawPixel(x -2, y -5, color);
lcd.drawPixel(x +2, y -5, color);
lcd.drawPixel(x +3, y -5, color);
// -4 -  #  #  -  -  -  #  #
lcd.drawPixel(x -3, y -4, color);
lcd.drawPixel(x -2, y -4, color);
lcd.drawPixel(x +2, y -4, color);
lcd.drawPixel(x +3, y -4, color);
// -3 -  #  #  -  -  -  #  #
lcd.drawPixel(x -3, y -3, color);
lcd.drawPixel(x -2, y -3, color);
lcd.drawPixel(x +2, y -3, color);
lcd.drawPixel(x +3, y -3, color);
// -2 -  #  #  -  -  -  #  #
lcd.drawPixel(x -3, y -2, color);
lcd.drawPixel(x -2, y -2, color);
lcd.drawPixel(x +2, y -2, color);
lcd.drawPixel(x +3, y -2, color);
// -1 -  #  #  -  -  -  #  #
lcd.drawPixel(x -3, y -1, color);
lcd.drawPixel(x -2, y -1, color);
lcd.drawPixel(x +2, y -1, color);
lcd.drawPixel(x +3, y -1, color);
//  0 -  #  #  -  -  -  #  #
lcd.drawPixel(x -3, y -0, color);
lcd.drawPixel(x -2, y -0, color);
lcd.drawPixel(x +2, y -0, color);
lcd.drawPixel(x +3, y -0, color);
//  1 -  #  #  -  -  -  #  #
lcd.drawPixel(x -3, y +1, color);
lcd.drawPixel(x -2, y +1, color);
lcd.drawPixel(x +2, y +1, color);
lcd.drawPixel(x +3, y +1, color);
//  2 -  #  #  -  -  -  #  #
lcd.drawPixel(x -3, y +2, color);
lcd.drawPixel(x -2, y +2, color);
lcd.drawPixel(x +2, y +2, color);
lcd.drawPixel(x +3, y +2, color);
//  3 -  #  #  -  -  -  #  #
lcd.drawPixel(x -3, y +3, color);
lcd.drawPixel(x -2, y +3, color);
lcd.drawPixel(x +2, y +3, color);
lcd.drawPixel(x +3, y +3, color);
//  4 -  #  #  -  -  -  #  #
lcd.drawPixel(x -3, y +4, color);
lcd.drawPixel(x -2, y +4, color);
lcd.drawPixel(x +2, y +4, color);
lcd.drawPixel(x +3, y +4, color);
//  5 -  #  #  -  -  -  #  #
lcd.drawPixel(x -3, y +5, color);
lcd.drawPixel(x -2, y +5, color);
lcd.drawPixel(x +2, y +5, color);
lcd.drawPixel(x +3, y +5, color);
//  6 -  #  #  #  -  #  #  #
lcd.drawPixel(x -3, y +6, color);
lcd.drawPixel(x -2, y +6, color);
lcd.drawPixel(x -1, y +6, color);
lcd.drawPixel(x +1, y +6, color);
lcd.drawPixel(x +2, y +6, color);
lcd.drawPixel(x +3, y +6, color);
//  7 -  -  #  #  #  #  #  -
lcd.drawPixel(x -2, y +7, color);
lcd.drawPixel(x -1, y +7, color);
lcd.drawPixel(x +0, y +7, color);
lcd.drawPixel(x +1, y +7, color);
lcd.drawPixel(x +2, y +7, color);
}

// ----------------------------------------------------------------------
// Rainbow für Farbauswahl
// ----------------------------------------------------------------------
void convertRGB565toRGB888(uint16_t color565, uint8_t &r, uint8_t &g, uint8_t &b) {
    // 5-Bit Rot (R5) auf 8-Bit (R8) skalieren: R8 = (R5 << 3) | (R5 >> 2)
    r = (uint8_t)(((color565 & 0xF800) >> 11) << 3);
    // 6-Bit Grün (G6) auf 8-Bit (G8) skalieren: G8 = (G6 << 2) | (G6 >> 4)
    g = (uint8_t)(((color565 & 0x07E0) >> 5) << 2);
    // 5-Bit Blau (B5) auf 8-Bit (B8) skalieren: B8 = (B5 << 3) | (B5 >> 2)
    b = (uint8_t)((color565 & 0x001F) << 3);
}

void convertRGB888toHSV(uint8_t r8, uint8_t g8, uint8_t b8, float &h, float &s, float &v) {
    float r = r8 / 255.0f;
    float g = g8 / 255.0f;
    float b = b8 / 255.0f;

    float cmax = max(r, max(g, b));
    float cmin = min(r, min(g, b));
    float delta = cmax - cmin;

    v = cmax; // Value (Helligkeit) ist der maximale Wert

    if (cmax == 0.0f) {
        s = 0.0f; // Sättigung 0, wenn kein Farbpixel da ist
        h = 0.0f; // Farbton ist irrelevant
        return;
    }

    s = delta / cmax; // Saturation

    if (delta == 0.0f) {
        h = 0.0f; // Farbton 0, wenn keine Sättigung
    } else {
        if (cmax == r) {
            h = 60.0f * fmodf(((g - b) / delta), 6.0f);
        } else if (cmax == g) {
            h = 60.0f * (((b - r) / delta) + 2.0f);
        } else { // cmax == b
            h = 60.0f * (((r - g) / delta) + 4.0f);
        }
    }

    if (h < 0.0f) {
        h += 360.0f;
    }
}

uint16_t convertHSVtoRGB565(float h, float s, float v) {
    // Die M5Cardputer.Lcd wird hier nicht direkt verwendet, 
    // aber wir nehmen an, dass die Umwandlung R, G, B (0-255) -> RGB565 
    // später mit einer entsprechenden Funktion der Bibliothek erfolgt.
    
    float r = 0.0f;
    float g = 0.0f;
    float b = 0.0f;

    if (s == 0.0f) {
        r = g = b = v; // Graustufen
    } else {
        int i;
        float f, p, q, t;

        if (h == 360.0f)
            h = 0.0f;
        
        h /= 60.0f;
        i = (int)floor(h);
        f = h - i;
        p = v * (1.0f - s);
        q = v * (1.0f - (s * f));
        t = v * (1.0f - (s * (1.0f - f)));

        switch (i) {
            case 0: r = v; g = t; b = p; break;
            case 1: r = q; g = v; b = p; break;
            case 2: r = p; g = v; b = t; break;
            case 3: r = p; g = q; b = v; break;
            case 4: r = t; g = p; b = v; break;
            case 5: r = v; g = p; b = q; break;
            default: r = 0; g = 0; b = 0; break;
        }
    }
    
    // Konvertiere float-Werte (0.0 bis 1.0) zu 8-Bit-Integer (0 bis 255)
    uint8_t r8 = (uint8_t)(r * 255.0f);
    uint8_t g8 = (uint8_t)(g * 255.0f);
    uint8_t b8 = (uint8_t)(b * 255.0f);

    // Nutzen Sie nun die Funktion der Bibliothek, um 8-Bit RGB in 16-Bit 565 zu konvertieren.
    // Da wir keinen direkten Zugriff auf das 'lcd'-Objekt haben, 
    // verwenden wir die statische Konvertierungslogik der TFT_eSPI/M5 Bibliothek.
    // Dies ist die Standard-RGB565-Makro-Konvertierung:
    return ((r8 & 0xF8) << 8) | ((g8 & 0xFC) << 3) | (b8 >> 3);
}

void drawColorGradient(int startX, int startY, int breite, int hoehe) {
    auto& lcd = M5Cardputer.Lcd;
    
    // NEU: Definiere den VOLLEN HSV-Bereich (von Rot bis fast Rot)
    const float START_HUE = 0.0f;     // Rot
    const float END_HUE = 360.0f;     // Rot (wir gehen bis kurz davor)
    const float HUE_SPAN = END_HUE;   // 360 Grad Spanne

    // Sättigung und Helligkeit bleiben maximal
    const float SATURATION = 1.0f;
    const float VALUE = 1.0f;      

    if (breite < 1) {
        return;
    }

    // Die Schleife durchläuft jeden Pixel in X-Richtung
    for (int x = 0; x < breite; x++) {
        // 1. Berechnung des Farbtons (Hue) für den aktuellen Pixel
        // Der Farbton geht von 0.0f bis 359.99...f.
        float currentHue = START_HUE + ( (float)x / (breite - 1) * HUE_SPAN );
        
        // Wichtig: Da 360° gleich 0° ist, sorgen wir dafür, dass der letzte Pixel
        // nicht genau 360° erreicht, um den Übergang weich zu halten, falls 
        // die Implementierung 360° = 0° behandelt.
        if (x == breite - 1) {
            currentHue = 359.9f; 
        }

        // 2. Konvertierung von HSV nach 16-Bit RGB
        uint16_t color16Bit = convertHSVtoRGB565(currentHue, SATURATION, VALUE);
        
        // 3. Den gesamten vertikalen Streifen zeichnen
        for (int y = 0; y < hoehe; y++) {
            lcd.drawPixel(startX + x, startY + y, color16Bit);
        }
    }
}

void drawColorGradientFrom(int startX, int startY, int breite, int hoehe, uint16_t targetColor565) {
    auto& lcd = M5Cardputer.Lcd;

    uint8_t r8, g8, b8;
    float targetColorHue, currentSaturation, currentValue;

    // SCHRITT 1: 5-6-5 Farrcode in HSV umwandeln, um den Farbton (H) zu isolieren.
    convertRGB565toRGB888(targetColor565, r8, g8, b8);
    convertRGB888toHSV(r8, g8, b8, targetColorHue, currentSaturation, currentValue); 
    // ACHTUNG: Die Sättigung (S) und Helligkeit (V) der Zielfarbe werden ignoriert, 
    // da wir für den Verlauf immer S=1.0 und V=1.0 an der Mitte verwenden.

    const float HUE = targetColorHue;

    if (breite < 1) {
        return;
    }
    
    // Die Schleife durchläuft jeden Pixel in X-Richtung
    for (int x = 0; x < breite; x++) {
        float normalizedX = (float)x / (breite - 1);

        if (normalizedX <= 0.5f) {
            // Linke Hälfte (Schwarz zu Zielfarbe): V geht von 0.0 -> 1.0
            float t = normalizedX * 2.0f; 
            currentSaturation = 1.0f;
            currentValue = t;
        } else {
            // Rechte Hälfte (Zielfarbe zu Weiß): S geht von 1.0 -> 0.0
            float t = (normalizedX - 0.5f) * 2.0f;
            currentSaturation = 1.0f - t;
            currentValue = 1.0f;
        }

        // 3. Konvertierung von HSV nach 16-Bit RGB
        uint16_t color16Bit = convertHSVtoRGB565(HUE, currentSaturation, currentValue);
        
        // 4. Den gesamten vertikalen Streifen zeichnen
        for (int y = 0; y < hoehe; y++) {
            lcd.drawPixel(startX + x, startY + y, color16Bit);
        }
    }
}