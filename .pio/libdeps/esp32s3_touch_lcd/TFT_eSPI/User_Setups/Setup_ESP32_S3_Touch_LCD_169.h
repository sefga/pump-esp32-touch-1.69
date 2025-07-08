// Setup for ESP32-S3 Touch-LCD-1.69 (240x280 ST7789)
#define USER_SETUP_ID 999
#define USER_SETUP_INFO "ESP32_S3_Touch_LCD_169"

// Define to disable all #warnings in library
#define DISABLE_ALL_LIBRARY_WARNINGS

// Driver selection
#define ST7789_DRIVER

// Display size (explicitly defined for 240x280 pixels)
#define TFT_WIDTH  240
#define TFT_HEIGHT 280

// Color order (try both if colors are wrong)
#define TFT_RGB_ORDER TFT_RGB  // Red-Green-Blue
// #define TFT_RGB_ORDER TFT_BGR  // Blue-Green-Red

// Inversion (try both if display is inverted)
// #define TFT_INVERSION_ON
// #define TFT_INVERSION_OFF

// Pin definitions for ESP32-S3 Touch-LCD-1.69 (from README.md)
#define TFT_MOSI 11  // SPI Data Out
#define TFT_SCLK 12  // SPI Clock
#define TFT_CS   10  // Chip Select
#define TFT_DC   9   // Data/Command
#define TFT_RST  13  // Reset
#define TFT_BL   38  // Backlight Control

// Backlight control
#define TFT_BACKLIGHT_ON HIGH

// SPI port selection
#define USE_HSPI_PORT

// Font loading
#define LOAD_GLCD   // Font 1. Original Adafruit 8 pixel font
#define LOAD_FONT2  // Font 2. Small 16 pixel high font
#define LOAD_FONT4  // Font 4. Medium 26 pixel high font
#define LOAD_FONT6  // Font 6. Large 48 pixel font
#define LOAD_FONT7  // Font 7. 7 segment 48 pixel font
#define LOAD_FONT8  // Font 8. Large 75 pixel font
#define LOAD_GFXFF  // FreeFonts

// Smooth fonts
#define SMOOTH_FONT

// SPI frequency
#define SPI_FREQUENCY  80000000  // 80MHz
#define SPI_READ_FREQUENCY  20000000
#define SPI_TOUCH_FREQUENCY  2500000

// Enable transactions
#define SUPPORT_TRANSACTIONS