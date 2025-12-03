# Little DevHelper for M5Stack Cardputer

## Overview

The **Little DevHelper** is a collection of useful utilities (Apps) for the M5Stack Cardputer, specifically designed for **developers and hardware testers**. The tool allows users to quickly test basic hardware functions, calibrate UI elements, and customize device configuration without needing to re-flash the code.

## Features (Apps)

The DevHelper includes the following apps, accessed via a central main menu:

| App | Purpose |
| :--- | :--- |
| **Settings** | Permanently configures UI colors (Background/Accent) and display brightness in EEPROM. |
| **Keyboard Monitor** | Displays the real-time state of the keyboard: pressed character, hex code, and all modifiers/flags (SHIFT, CTRL, ALT, FN, etc.). |
| **Fonts** | Demonstrates all available standard fonts on the display for legibility checks. |
| **Get Coordinates** | An interactive crosshair (HUD) for determining precise pixel coordinates on the display. |
| **I/O Watcher** | Monitors the digital and analog states of the Cardputer's GPIO pins in real-time. |

## Configuration and Operation

### Global Settings (EEPROM)

Brightness and UI colors are saved in **Settings** and loaded automatically upon startup.

* **Brightness:** Adjustable from 7 (min) to 255 (max).
* **UI Color / BG Color:** Customized via the integrated color and brightness picker (Hue/Value).

### Controls

* **Menu Navigation:** Arrow Up (`;`) and Arrow Down (`.`)
* **Select/Start:** `ENTER`
* **Exit App:** Backtick (`` ` ``) or `ESC` (check the respective app header)

***

## Explanation: Implemented Changes

### 1. Internationalization (i18n)

**Change:** All user-facing texts displayed on the screen (menu items, headers, error messages, instruction texts) were translated from German to **English**.

**Reasoning:** Switching to English ensures broader usability and aligns with the standard practice within the global developer community.

### 2. UI Optimization (UX & Consistency)

**Change:**
* **Header Adjustment:** The header area was visually separated from the main app content using two distinct horizontal lines for improved structure.
* **Keyboard Monitor:** The flags output was precisely aligned to X=10 using `M5Cardputer.Lcd.setCursor(X, Y)`, and the vertical spacing was increased.
* **Settings State Management:** A critical bug was fixed by resetting the local menu index (`selectedSettingIndex = 0;`) upon exiting the `SettingsApp`.

**Reasoning:** These adjustments ensure a **consistent and professional User Experience**. Resetting the local state fixes an inconsistency where the menu would not start at the first entry when reopened.

### 3. Debugging Security

**Change:** All occurrences of `Serial.print`, `Serial.println`, and `Serial.printf` were commented out across the entire project.

**Reasoning:** Serial output is unnecessary in the final firmware and can cause unnecessary CPU load. Commenting them out ensures a **more efficient and "silent"** end product.