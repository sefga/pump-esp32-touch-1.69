M3 # Полное руководство по запуску ESP32-S3 Touch-LCD-1.69

**Автор:** MiniMax Agent
**Дата:** 2025-07-08

## Введение

Это руководство представляет собой подробное исследование платы ESP32-S3 Touch-LCD-1.69 от Waveshare. Цель - предоставить разработчикам всю необходимую информацию для быстрого старта, от технических характеристик до решения распространенных проблем. Особое внимание уделено практическим аспектам, примерам кода и решению проблемы "черного экрана".

---

## 1. Технические характеристики платы

### 1.1. Спецификации ESP32-S3 Touch-LCD-1.69
- **Микроконтроллер:** ESP32-S3R8 (двухъядерный Xtensa® LX7 @ 240 МГц)
- **Память:** 512КБ SRAM, 8МБ PSRAM, 16МБ Flash
- **Беспроводная связь:** Wi-Fi 802.11 b/g/n, Bluetooth 5 (BLE)
- **Датчики:** QMI8658C (6-осевой IMU)
- **RTC:** PCF85063
- **Питание:** USB-C, разъем для Li-Po батареи с контроллером заряда ETA6098

### 1.2. Характеристики дисплея
- **Тип:** Емкостный сенсорный LCD
- **Размер:** 1.69 дюйма
- **Разрешение:** 240x280 пикселей
- **Драйвер:** ST7789V2
- **Интерфейс:** SPI

### 1.3. Сенсорный контроллер
- **Драйвер:** CST816T
- **Интерфейс:** I2C
- **Особенности:** Поддерживает базовые жесты.

### 1.4. Распиновка (Pinout)
| Функция | Пин GPIO | Примечание |
| :--- | :--- | :--- |
| **TFT_SCLK** | `GPIO12` | SPI Clock |
| **TFT_MOSI** | `GPIO11` | SPI Data Out |
| **TFT_CS** | `GPIO10` | Chip Select |
| **TFT_DC** | `GPIO9` | Data/Command |
| **TFT_RST** | `GPIO13` | Reset |
| **TFT_BL** | `GPIO38` | Backlight Control |
| **TP_SCL** | `GPIO7` | I2C Clock |
| **TP_SDA** | `GPIO6` | I2C Data |
| **TP_INT** | `GPIO1` | Touch Interrupt |
| **TP_RST** | `GPIO2` | Touch Reset |

### 1.5. Схема питания
Плата может питаться от USB-C или от литиевой батареи. Чип ETA6098 управляет зарядкой и переключением источников. Потребление энергии сильно зависит от режима работы (активность Wi-Fi/BT, яркость подсветки).

---

## 2. Настройка среды разработки

### 2.1. Arduino IDE
Это самый простой способ начать.

1.  **Установка ESP32 Core:**
    - В `Файл > Настройки` добавьте URL:
      `https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_dev_index.json`
    - В `Инструменты > Плата > Менеджер плат` найдите и установите `esp32` от Espressif Systems (версии 2.0.5+).
2.  **Настройки платы:**
    - **Плата:** "ESP32S3 Dev Module"
    - **USB CDC On Boot:** "Enabled"
    - **Flash Size:** "16MB"
    - **PSRAM:** "OPI PSRAM"
3.  **Необходимые библиотеки (установить через менеджер библиотек):**
    - `TFT_eSPI` от Bodmer
    - `CST816_TouchLib` от mjdonders
4.  **Конфигурация `TFT_eSPI`:**
    - В папке с библиотеками найдите `TFT_eSPI/User_Setup_Select.h`.
    - Закомментируйте `#include <User_Setup.h>` и раскомментируйте или добавьте `#include <User_Setups/Setup206_ESP32_S3_Waveshare_1.69_inch_LCD.h>`.
    - Создайте файл `Setup206...h` в папке `User_Setups` с содержимым из файла `docs/Chapter1_Hardware_and_Setup.md`.

### 2.2. PlatformIO
Более продвинутая среда на базе VS Code.

1.  **Создание проекта:** Создайте новый проект, выбрав в качестве платы `Waveshare ESP32-S3-Touch-LCD-1.69` (если доступна) или `Espressif ESP32-S3 Dev Module`.
2.  **Файл `platformio.ini`:** Это основной конфигурационный файл.

    ```ini
    [env:waveshare_esp32s3_lcd]
    platform = espressif32
    board = esp32s3_devkitc_1
    framework = arduino

    ; Настройки для монитора порта
    monitor_speed = 115200

    ; Подключаем необходимые библиотеки
    lib_deps = 
        bodmer/TFT_eSPI
        mjdonders/CST816_TouchLib

    ; Конфигурация TFT_eSPI через флаги сборки
    build_flags = 
        -D USER_SETUP_LOADED=1
        -D ST7789_DRIVER=1
        -D TFT_WIDTH=240
        -D TFT_HEIGHT=280
        -D TFT_MOSI=11
        -D TFT_SCLK=12
        -D TFT_CS=10
        -D TFT_DC=9
        -D TFT_RST=13
        -D TFT_BL=38
        -D TFT_BACKLIGHT_ON=HIGH
        -D USE_HSPI_PORT=1 ; Важно для избежания конфликтов с PSRAM
        -D LOAD_GFXFF=1
        -D SPI_FREQUENCY=80000000
    ```

### 2.3. ESP-IDF
Официальный фреймворк от Espressif для профессиональной разработки.

1.  **Настройка ESP-IDF:** Следуйте официальной документации Espressif для установки фреймворка.
2.  **Драйвер дисплея:** Используйте встроенный компонент `esp_lcd`.
    - В `menuconfig` (`Component config > Display > LCD Drivers`) выберите поддержку `ST7789`.
    - Настройте пины SPI и параметры дисплея через `menuconfig` или напрямую в коде.
3.  **Драйвер сенсора:** Интегрируйте сторонний компонент для `CST816` или напишите свою реализацию с использованием I2C-драйвера ESP-IDF.

---

## 3. Инициализация и программирование

Код инициализации и примеры доступны в файле `docs/Chapter2_Code_and_Troubleshooting.md`.

---

## 4. Проблемы черного экрана при запуске

Подробная диагностика и пошаговые решения описаны в `docs/Chapter2_Code_and_Troubleshooting.md`.

**Краткий чек-лист:**
1.  **Проверьте Serial Monitor:** Есть ли какие-либо сообщения? Это ваш главный инструмент диагностики.
2.  **Конфигурация `TFT_eSPI`:** Правильно ли указаны ВСЕ пины? Используется ли `HSPI`?
3.  **Питание:** Качественный ли USB-кабель? Попробуйте другой порт.
4.  **Подсветка:** Управляется ли пин `TFT_BL`?

---

## 5. Типичные ошибки и их решения

| Ошибка | Решение |
| :--- | :--- |
| **Ошибки компиляции** | Убедитесь, что все библиотеки установлены и `TFT_eSPI` правильно сконфигурирован. |
| **Проблемы с загрузкой** | Используйте комбинацию кнопок BOOT/RST для входа в режим загрузки. Проверьте кабель/порт. |
| **Сенсор не работает** | Проверьте пины I2C. Запустите I2C-сканер для проверки наличия устройства на шине. |

---

## 6. Практические примеры

### 6.1. Минимальный рабочий код (Arduino)
```cpp
// Код из файла docs/Chapter2_Code_and_Troubleshooting.md
#include <TFT_eSPI.h>

TFT_eSPI tft = TFT_eSPI();

void setup() {
  tft.init();
  tft.setRotation(1);
  tft.fillScreen(TFT_BLACK);
  pinMode(TFT_BL, OUTPUT);
  digitalWrite(TFT_BL, HIGH);
  tft.setCursor(10, 10, 4);
  tft.setTextColor(TFT_GREEN, TFT_BLACK);
  tft.println("Display OK");
}

void loop() {}
```

### 6.2. Отладочный скрипт: I2C Сканер (Arduino)
Этот скрипт поможет проверить, виден ли сенсорный контроллер CST816 на шине I2C.
```cpp
#include <Wire.h>

#define I2C_SDA 6
#define I2C_SCL 7

void setup() {
  Wire.begin(I2C_SDA, I2C_SCL);
  Serial.begin(115200);
  Serial.println("\nI2C Scanner");
}

void loop() {
  byte error, address;
  int nDevices;
  Serial.println("Scanning...");
  nDevices = 0;
  for(address = 1; address < 127; address++ ) {
    Wire.beginTransmission(address);
    error = Wire.endTransmission();
    if (error == 0) {
      Serial.print("I2C device found at address 0x");
      if (address<16) Serial.print("0");
      Serial.print(address, HEX);
      Serial.println("  !");
      nDevices++;
    }
    else if (error==4) {
      Serial.print("Unknown error at address 0x");
      if (address<16) Serial.print("0");
      Serial.println(address, HEX);
    }
  }
  if (nDevices == 0)
    Serial.println("No I2C devices found\n");
  else
    Serial.println("done\n");
  delay(5000);
}
```

## Заключение

Плата ESP32-S3 Touch-LCD-1.69 является мощным и универсальным инструментом для разработки проектов с графическим интерфейсом. Несмотря на некоторые сложности с первоначальной настройкой, правильная конфигурация библиотек и понимание аппаратных особенностей позволяют полностью раскрыть ее потенциал. Ключ к успеху — внимательная настройка пинов и использование правильного SPI порта (`HSPI`) для избежания конфликтов.