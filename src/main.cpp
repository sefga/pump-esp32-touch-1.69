#include <Arduino.h>
#include <lvgl.h>

#define LGFX_USE_V1
#include <LovyanGFX.hpp>

// Глобальные переменные для счетчика
static int button_click_count = 0;
static lv_obj_t *counter_label = nullptr;

// Определения пинов для ESP32-S3-Touch-LCD-1.69
#define SCREEN_WIDTH 240
#define SCREEN_HEIGHT 280
#define OFFSET_X 0
#define OFFSET_Y 20
#define RGB_ORDER true

// Touch pins
#define I2C_SDA 11
#define I2C_SCL 10
#define TP_INT 14
#define TP_RST 13

// Display pins
#define SPI_HOST SPI2_HOST
#define SCLK 6
#define MOSI 7
#define MISO -1
#define DC 4
#define CS 5
#define RST 8
#define BL 15

class LGFX : public lgfx::LGFX_Device
{
  lgfx::Panel_ST7789 _panel_instance;
  lgfx::Light_PWM _light_instance;
  lgfx::Bus_SPI _bus_instance;
  lgfx::Touch_CST816S _touch_instance;

public:
  LGFX(void)
  {
    {
      auto cfg = _bus_instance.config();
      cfg.spi_host = SPI_HOST;
      cfg.spi_mode = 0;
      cfg.freq_write = 80000000;
      cfg.freq_read = 20000000;
      cfg.use_lock = true;
      cfg.dma_channel = SPI_DMA_CH_AUTO;
      cfg.pin_sclk = SCLK;
      cfg.pin_mosi = MOSI;
      cfg.pin_miso = MISO;
      cfg.pin_dc = DC;
      _bus_instance.config(cfg);
      _panel_instance.setBus(&_bus_instance);
    }

    {
      auto cfg = _panel_instance.config();
      cfg.pin_cs = CS;
      cfg.pin_rst = RST;
      cfg.pin_busy = -1;
      cfg.memory_width = SCREEN_WIDTH;
      cfg.memory_height = SCREEN_HEIGHT;
      cfg.panel_width = SCREEN_WIDTH;
      cfg.panel_height = SCREEN_HEIGHT;
      cfg.offset_x = OFFSET_X;
      cfg.offset_y = OFFSET_Y;
      cfg.offset_rotation = 0;
      cfg.dummy_read_pixel = 8;
      cfg.dummy_read_bits = 1;
      cfg.readable = false;
      cfg.invert = true;
      cfg.rgb_order = RGB_ORDER;
      cfg.dlen_16bit = false;
      cfg.bus_shared = false;
      _panel_instance.config(cfg);
    }

    {
      auto cfg = _light_instance.config();
      cfg.pin_bl = BL;
      cfg.invert = false;
      cfg.freq = 44100;
      cfg.pwm_channel = 1;
      _light_instance.config(cfg);
      _panel_instance.setLight(&_light_instance);
    }

    {
      auto cfg = _touch_instance.config();
      cfg.x_min = 0;
      cfg.x_max = SCREEN_WIDTH;
      cfg.y_min = 0;
      cfg.y_max = SCREEN_HEIGHT;
      cfg.pin_int = TP_INT;
      cfg.pin_rst = TP_RST;
      cfg.bus_shared = true;
      cfg.offset_rotation = 0;
      cfg.i2c_port = 0;
      cfg.i2c_addr = 0x15;
      cfg.pin_sda = I2C_SDA;
      cfg.pin_scl = I2C_SCL;
      cfg.freq = 400000;
      _touch_instance.config(cfg);
      _panel_instance.setTouch(&_touch_instance);
    }

    setPanel(&_panel_instance);
  }
};

LGFX tft;

// LVGL display buffer
static const uint32_t lvBufferSize = SCREEN_WIDTH * 10;
static uint8_t lvBuffer[2][lvBufferSize];

// LVGL display flush callback
void my_disp_flush(lv_display_t *display, const lv_area_t *area, unsigned char *data)
{
  uint32_t w = lv_area_get_width(area);
  uint32_t h = lv_area_get_height(area);
  
  // Swap RGB565 bytes for correct color display
  lv_draw_sw_rgb565_swap(data, w * h);
  
  tft.pushImageDMA(area->x1, area->y1, w, h, (uint16_t*)data);
  lv_display_flush_ready(display);
}

// LVGL input device callback
void my_touchpad_read(lv_indev_t *indev, lv_indev_data_t *data)
{
    static lv_coord_t last_x = 0;
    static lv_coord_t last_y = 0;

    uint16_t touchX, touchY;
    bool touched = tft.getTouch(&touchX, &touchY);

    if (touched) {
        last_x = touchX;
        last_y = touchY;
        data->state = LV_INDEV_STATE_PRESSED;
    } else {
        data->state = LV_INDEV_STATE_RELEASED;
    }

    data->point.x = last_x;
    data->point.y = last_y;
}

// Button event handler
static void button_event_cb(lv_event_t *e)
{
  lv_event_code_t code = lv_event_get_code(e);
  
  if (code == LV_EVENT_CLICKED) {
    button_click_count++;
    
    // Update counter text
    char buffer[64];
    snprintf(buffer, sizeof(buffer), "Button clicked! Count: %d", button_click_count);
    lv_label_set_text(counter_label, buffer);
    
    Serial.printf("Button clicked! Count: %d\n", button_click_count);
  }
}

void setup()
{
  Serial.begin(115200);
  Serial.println("ESP32-S3-Touch-LCD-1.69 LVGL Demo");
  
  // Initialize display
  tft.init();
  tft.setRotation(0);
  tft.setBrightness(128);
  
  Serial.println("Display initialized");
  
  // Initialize LVGL
  lv_init();
  Serial.println("LVGL initialized");
  
  // Create display
  lv_display_t *disp = lv_display_create(SCREEN_WIDTH, SCREEN_HEIGHT);
  lv_display_set_flush_cb(disp, my_disp_flush);
  lv_display_set_buffers(disp, lvBuffer[0], lvBuffer[1], lvBufferSize, LV_DISPLAY_RENDER_MODE_PARTIAL);
  
  Serial.println("LVGL display created");
  
  // Create input device
  lv_indev_t *indev = lv_indev_create();
  lv_indev_set_type(indev, LV_INDEV_TYPE_POINTER);
  lv_indev_set_read_cb(indev, my_touchpad_read);
  
  Serial.println("LVGL input device created");
  
  // Create a simple UI
  lv_obj_t *scr = lv_screen_active();
  lv_obj_set_style_bg_color(scr, lv_color_hex(0x000000), LV_PART_MAIN);
  
  // Create a label with counter
  counter_label = lv_label_create(scr);
  lv_label_set_text(counter_label, "Button clicked! Count: 0");
  lv_obj_set_style_text_color(counter_label, lv_color_hex(0xFFFFFF), LV_PART_MAIN);
  lv_obj_set_style_text_align(counter_label, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN);
  lv_obj_align(counter_label, LV_ALIGN_CENTER, 0, -50);
  
  // Create a button
  lv_obj_t *btn = lv_button_create(scr);
  lv_obj_set_size(btn, 160, 60);
  lv_obj_align(btn, LV_ALIGN_CENTER, 0, 50);
  lv_obj_set_style_bg_color(btn, lv_color_hex(0x0080FF), LV_PART_MAIN);
  
  // Add event handler to the button
  lv_obj_add_event_cb(btn, button_event_cb, LV_EVENT_CLICKED, NULL);
  
  lv_obj_t *btn_label = lv_label_create(btn);
  lv_label_set_text(btn_label, "Click me!");
  lv_obj_center(btn_label);
  
  Serial.println("UI created successfully");
  Serial.println("Setup complete!");
}

void loop()
{
  lv_tick_inc(5);
  lv_timer_handler();
  delay(5);
}