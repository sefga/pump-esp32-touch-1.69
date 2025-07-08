#include <TFT_eSPI.h>

TFT_eSPI tft = TFT_eSPI();

void setup() {
  // Инициализация Serial для отладки
  Serial.begin(115200);
  Serial.println("Запуск ESP32-S3 Touch-LCD-1.69...");
  
  // Инициализация дисплея
  tft.init();
  tft.setRotation(1);
  tft.fillScreen(TFT_BLACK);
  
  // Включение подсветки
  pinMode(TFT_BL, OUTPUT);
  digitalWrite(TFT_BL, HIGH);
  
  // Отображение текста
  tft.setCursor(10, 10, 4);
  tft.setTextColor(TFT_GREEN, TFT_BLACK);
  tft.println("Display OK");
  
  Serial.println("Инициализация завершена!");
}

void loop() {
  // Основной цикл пуст для простого примера
  delay(1000);
}