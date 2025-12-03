# DevREADME.md: Code Architecture for Little DevHelper

This document outlines the architecture, component responsibilities, and key design decisions made during the development of the **Little DevHelper**. It is intended for developers maintaining or extending the project.

---

## 1. Project Structure and Components

The project utilizes the Arduino/ESP32 platform and relies on the M5Stack libraries (`M5Cardputer.h`). The architecture is divided into modular `.ino` files, each with specific responsibilities.

| File | Responsibility | Key Function/Concept |
| :--- | :--- | :--- |
| **`01_Main.ino`** | Main loop (`loop`), Setup, App registration, central keyboard input for menu navigation. | `void loop()`, `Menu.addApp()`, `isMenuOpen` |
| **`02_Menu.h`** | Header definitions, global layout constants (`MENU_START_Y`), global variables (e.g., `Global_ACCENT_COLOR`), struct definitions (`AppConfig`). | `MENU_START_Y`, `AppConfig`, `CardputerMenu` class |
| **`Menu.ino`** | Core implementation of the `CardputerMenu` class, navigation logic, menu drawing, and **EEPROM management**. | `drawMenu()`, `load_settings()`, `save_settings()` |
| **`Settings.ino`** | Implementation of the configuration app, handling brightness, color picker logic, and triggering EEPROM writes. | `runSettingsApp()`, State reset logic |
| **`*App.ino`** | Specific application implementations (e.g., `KeyboardMonitorApp.ino`). | `run[App]App()` |
| **`Assets.ino`** | (Implizit) Contains functions for drawing UI elements like arrows, frames, and color gradients. | `drawHUD()`, `drawColorGradient()` |

---

## 2. State Management and Control Flow

The system is controlled by a simple boolean flag: **`bool isMenuOpen`**.

* **`true`**: The main `loop()` executes menu navigation logic.
* **`false`**: An application is currently running.

**App Exit Protocol**: Any running app must execute the following to safely return to the main menu: `isMenuOpen = true;` followed by `Menu.redrawMenu();`.

---

## 3. Global Variables and Function Reference

This section provides an explicit reference for all global state variables and functions within the project.

### 3.1 Global Variables (State & Configuration)

| Variable Name | Type | Defined In | Purpose |
| :--- | :--- | :--- | :--- |
| **`Menu`** | `CardputerMenu` | `Menu.ino` | The global instance of the menu class, managing app list and navigation. |
| **`isMenuOpen`** | `bool` | `01_Main.ino` | **Core State Flag:** `true` if the main menu is displayed; `false` if an app is running. |
| **`Global_Brightnes`** | `int` | `01_Main.ino` | Stores the current LCD backlight brightness (7 to 255). Persisted in EEPROM. |
| **`Global_BACKGROUND_COLOR`** | `uint16_t` | `01_Main.ino` | Stores the global background color (16-bit TFT code). |
| **`Global_ACCENT_COLOR`** | `uint16_t` | `01_Main.ino` | Stores the global accent color for text and highlights. |
| **`Global_RefreshDelay`** | `int` | `01_Main.ino` | Controls the delay (in milliseconds) at the end of `loop()` and app loops. |

### 3.2 Function Reference (API & Logic)

#### A. Globale Funktionen und App-Einstiegspunkte (`void`)

| Funktion/Void Signature | Defined In | Brief Explanation |
| :--- | :--- | :--- |
| **`void setup()`** | `01_Main.ino` | Initializes hardware, sets up the menu, registers all apps, and loads settings. |
| **`void loop()`** | `01_Main.ino` | Reads keyboard input and switches between menu navigation logic and app execution flow. |
| **`void save_defaults()`** | `Menu.ino` | Overwrites the EEPROM memory with the defined `Default_Settings`. |
| **`void save_settings()`** | `Menu.ino` | Serializes current global config (`Global_*`) into a string and writes it to EEPROM. |
| **`void load_settings()`** | `Menu.ino` | Parses the settings string from EEPROM to update all `Global_*` variables. |
| **`void run[App]App()`** | `*App.ino` | The main execution loop and logic for a specific application (e.g., `runSettingsApp`). |
| **`void drawHUD()`** | `Assets.ino` | Draws the crosshair/HUD symbol at the given coordinates. |
| **`void drawUpArrow()`** | `Assets.ino` | Draws the upward-pointing arrow (for menu scroll indicator). |
| **`void drawDownArrow()`** | `Assets.ino` | Draws the downward-pointing arrow (for menu scroll indicator). |
| **`void drawColorGradient()`** | `Assets.ino` | Draws a horizontal color gradient (e.g., Hue spectrum) for the color picker. |
| **`void drawEmptyFrame()`** | `Assets.ino` | Draws a small frame around the selected point in the color pickers. |

#### B. `CardputerMenu` Methoden (`void CardputerMenu::`)

| Funktion/Void Signature | Defined In | Brief Explanation |
| :--- | :--- | :--- |
| **`void initialize()`** | `Menu.ino` | Initializes the menu system, loads settings, and performs the first screen draw. |
| **`void drawHeader()`** | `Menu.ino` | Renders the top title bar, including title, subtitle, and battery status. |
| **`void addApp()`** | `Menu.ino` | Registers a new app (`AppConfig`) into the list and sorts it by position. |
| **`void run()`** | `Menu.ino` | State handler for the menu. |
| **`void redrawMenu()`** | `Menu.ino` | Forces a full redraw of the current menu screen. |
| **`void nextApp()`** | `Menu.ino` | Moves the selection down the app list, managing scrolling and wraparound. |
| **`void prevApp()`** | `Menu.ino` | Moves the selection up the app list, managing scrolling and wraparound. |
| **`void startSelectedApp()`** | `Menu.ino` | Executes the selected app's `runFunction` and sets `isMenuOpen = false`. |
| **`void drawMenu()`** | `Menu.ino` | Renders the complete menu list, including selected item highlight and scroll indicators. |