#include <Arduino.h>
#include <lvgl.h>
#include <TFT_eSPI.h>
#include <page/init_page.h>

#define TFT_HOR_RES 240
#define TFT_VER_RES 320
#define DRAW_BUF_SIZE (TFT_HOR_RES * TFT_VER_RES / 10 * (LV_COLOR_DEPTH / 8))
uint32_t draw_buf[DRAW_BUF_SIZE / 4];

void setup()
{
    Serial.begin(115200);
    lv_init();
    LV_FONT_DECLARE(misans_bold_14);

    lv_display_t *display = lv_tft_espi_create(TFT_HOR_RES, TFT_VER_RES, draw_buf, sizeof(draw_buf));
    lv_theme_t *th = lv_theme_default_init(display, lv_color_hex(0x00BFFF), lv_color_hex(0x87CEEB), false, &misans_bold_14);
    lv_disp_set_theme(display, th);
    lv_obj_t *initPage = create_init_page();
    lv_screen_load(initPage);
}

void loop()
{
    lv_task_handler(); // Ensure this is called
    delay(5);
    lv_tick_inc(5);
}