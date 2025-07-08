/*
 * I2C Scanner для ESP32-S3 Touch-LCD-1.69
 * 
 * Этот код сканирует I2C шину и выводит адреса найденных устройств.
 * Полезно для диагностики сенсорного контроллера CST816T.
 * 
 * Ожидаемый результат: устройство на адресе 0x15 (CST816T)
 */

#include <Wire.h>
#include <TFT_eSPI.h>

// Пины I2C для сенсорного контроллера
#define I2C_SDA 6
#define I2C_SCL 7

// Инициализация дисплея
TFT_eSPI tft = TFT_eSPI();

void setup() {
  // Инициализация Serial
  Serial.begin(115200);
  delay(1000);
  Serial.println("\n=== I2C Scanner для ESP32-S3 Touch-LCD-1.69 ===");
  
  // Инициализация дисплея
  tft.init();
  tft.setRotation(1);
  tft.fillScreen(TFT_BLACK);
  
  // Включение подсветки
  pinMode(TFT_BL, OUTPUT);
  digitalWrite(TFT_BL, HIGH);
  
  // Заголовок на дисплее
  tft.setCursor(10, 10, 2);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.println("I2C Scanner");
  
  // Инициализация I2C
  Wire.begin(I2C_SDA, I2C_SCL);
  Serial.println("I2C инициализирован");
  Serial.printf("SDA: GPIO%d, SCL: GPIO%d\n", I2C_SDA, I2C_SCL);
  
  tft.setCursor(10, 40, 1);
  tft.setTextColor(TFT_GREEN, TFT_BLACK);
  tft.printf("SDA:%d SCL:%d", I2C_SDA, I2C_SCL);
}

void loop() {
  byte error, address;
  int nDevices = 0;
  int yPos = 70;
  
  Serial.println("\nСканирование I2C устройств...");
  
  // Очистка области результатов на дисплее
  tft.fillRect(0, 60, 280, 180, TFT_BLACK);
  tft.setCursor(10, 60, 1);
  tft.setTextColor(TFT_YELLOW, TFT_BLACK);
  tft.println("Scanning...");
  
  // Сканирование адресов от 1 до 126
  for(address = 1; address < 127; address++) {
    Wire.beginTransmission(address);
    error = Wire.endTransmission();
    
    if (error == 0) {
      // Устройство найдено
      Serial.printf("I2C устройство найдено по адресу 0x%02X", address);
      
      // Определение типа устройства
      if (address == 0x15) {
        Serial.println(" (CST816T - Touch Controller)");
        tft.setCursor(10, yPos, 1);
        tft.setTextColor(TFT_GREEN, TFT_BLACK);
        tft.printf("0x%02X CST816T OK", address);
      } else if (address == 0x51) {
        Serial.println(" (PCF85063 - RTC)");
        tft.setCursor(10, yPos, 1);
        tft.setTextColor(TFT_CYAN, TFT_BLACK);
        tft.printf("0x%02X PCF85063 RTC", address);
      } else if (address == 0x6B) {
        Serial.println(" (QMI8658C - IMU)");
        tft.setCursor(10, yPos, 1);
        tft.setTextColor(TFT_MAGENTA, TFT_BLACK);
        tft.printf("0x%02X QMI8658C IMU", address);
      } else {
        Serial.println(" (Неизвестное устройство)");
        tft.setCursor(10, yPos, 1);
        tft.setTextColor(TFT_WHITE, TFT_BLACK);
        tft.printf("0x%02X Unknown", address);
      }
      
      nDevices++;
      yPos += 15;
      
    } else if (error == 4) {
      Serial.printf("Неизвестная ошибка по адресу 0x%02X\n", address);
    }
  }
  
  // Результаты сканирования
  if (nDevices == 0) {
    Serial.println("I2C устройства не найдены\n");
    tft.setCursor(10, yPos, 1);
    tft.setTextColor(TFT_RED, TFT_BLACK);
    tft.println("No devices found!");
    
    // Диагностическая информация
    tft.setCursor(10, yPos + 20, 1);
    tft.setTextColor(TFT_ORANGE, TFT_BLACK);
    tft.println("Check connections:");
    tft.setCursor(10, yPos + 35, 1);
    tft.println("- Power supply");
    tft.setCursor(10, yPos + 50, 1);
    tft.println("- I2C wiring");
    tft.setCursor(10, yPos + 65, 1);
    tft.println("- Pull-up resistors");
    
  } else {
    Serial.printf("Сканирование завершено. Найдено %d устройств\n", nDevices);
    tft.setCursor(10, yPos + 10, 1);
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.printf("Found: %d devices", nDevices);
  }
  
  // Информация о следующем сканировании
  tft.setCursor(10, 250, 1);
  tft.setTextColor(TFT_DARKGREY, TFT_BLACK);
  tft.println("Next scan in 5s...");
  
  delay(5000); // Пауза 5 секунд перед следующим сканированием
}

/*
 * Ожидаемые устройства на ESP32-S3 Touch-LCD-1.69:
 * 
 * 0x15 - CST816T (Сенсорный контроллер)
 * 0x51 - PCF85063 (RTC)
 * 0x6B - QMI8658C (6-осевой IMU)
 * 
 * Если устройства не найдены:
 * 1. Проверьте питание платы
 * 2. Убедитесь в правильности пинов I2C
 * 3. Проверьте качество пайки
 * 4. Убедитесь, что pull-up резисторы установлены
 */