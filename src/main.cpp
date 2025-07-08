#include <Arduino.h>
#include <TFT_eSPI.h>

TFT_eSPI tft = TFT_eSPI();

void setup() {
  Serial.begin(115200);
  Serial.println("Starting TFT test...");
  
  // Инициализация дисплея
  tft.init();
  
  // Поворот экрана (если нужно)
  tft.setRotation(0);
  
  // Очистка экрана черным цветом
  tft.fillScreen(TFT_BLACK);
  
  // Установка цвета текста
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  
  // Установка размера шрифта
  tft.setTextSize(2);
  
  // Вывод текста
  tft.setCursor(10, 10);
  tft.println("Hello World!");
  
  tft.setCursor(10, 40);
  tft.println("ESP32-S3");
  
  tft.setCursor(10, 70);
  tft.println("TFT Test");
  
  // Рисование простых фигур
  tft.drawRect(10, 100, 100, 50, TFT_RED);
  tft.fillCircle(180, 125, 25, TFT_GREEN);
  
  Serial.println("TFT initialized and text displayed");
}

void loop() {
  // Простая индикация работы через Serial
  Serial.println("Loop running...");
  delay(2000);
}