#include <Arduino.h>
#include <lvgl.h>
#include <TFT_eSPI.h>
#include <forza_data_parser/ForzaDataParser.h>
#include <font/misans_bold_14.c>
#include <page/init_page.h>
#include <page/speed_page.h>

/* WiFi credentials */
const char *ssid = "Xiaomi_D6AB";
const char *password = "e42e2492";
#define TFT_HOR_RES 240
#define TFT_VER_RES 320
#define DRAW_BUF_SIZE (TFT_HOR_RES * TFT_VER_RES / 10 * (LV_COLOR_DEPTH / 8))
uint32_t draw_buf[DRAW_BUF_SIZE / 4];

ForzaDataParser parser(5300);
unsigned long lastPrintTime = 0;
const unsigned long printInterval = 500;

// ForzaData data;
// String a;

// void setup_wifi()
// {
//     lv_label_set_text(label, "Connecting to WiFi...");
//     WiFi.begin(ssid, password);
//     while (WiFi.status() != WL_CONNECTED)
//     {
//         delay(1000);
//         Serial.println("Connecting...");
//     }
//     Serial.println("WiFi connected");
//     String ipAddress = "IP address: \n" + WiFi.localIP().toString();
//     lv_label_set_text(label, ipAddress.c_str());
//     configTime(8 * 3600, 0, "pool.ntp.org");
// }

// void updateText(lv_timer_t *timer)
// {
//     if (!a.isEmpty())
//     {

//         lv_label_set_text(label, a.c_str());
//         if (data.EngineMaxRpm != 0)
//         {
//             int32_t value = (data.CurrentEngineRpm / data.EngineMaxRpm * 100);
//             lv_arc_set_value(arc, value);
//             lv_label_set_text_fmt(label1, "%" LV_PRId32 "%%", value);
//         }
//     }
// }
// void updateDate(lv_timer_t *timer)
// {
//     data = parser.getData();
//     if (data.IsRaceOn)
//     {
//         String output;
//         output += "Speed: " + String(data.Speed * 3.6) + "\n"; // Convert m/s to km/h
//         output += "RPM: " + String(data.CurrentEngineRpm) + "\n";
//         output += "RPM Ratio: " + String(data.CurrentEngineRpm / data.EngineMaxRpm) + "\n";
//         output += "Gear: " + String(data.Gear) + "\n";
//         output += "Car ID: " + String(data.CarOrdinal) + "\n";
//         output += "Car Class: " + String(data.CarClass) + "\n";
//         output += "Drivetrain: " + String(data.DrivetrainType) + "\n";
//         output += "Car Performance: " + String(data.CarPerformanceIndex) + "\n";
//         output += "Torque: " + String(data.Torque) + "\n";
//         output += "Throttle: " + String(data.Accel) + "\n";
//         output += "Brake: " + String(data.Brake) + "\n";
//         a = output;
//     }
// }

void setup()
{
    Serial.begin(115200);
    lv_init();
    LV_FONT_DECLARE(misans_bold_14);

    lv_display_t *display = lv_tft_espi_create(TFT_HOR_RES, TFT_VER_RES, draw_buf, sizeof(draw_buf));
    lv_theme_t *th = lv_theme_default_init(display, lv_color_hex(0x00BFFF), lv_color_hex(0x87CEEB), false, &misans_bold_14);
    lv_disp_set_theme(display, th);
    // lv_obj_t *initPage = create_init_page();
    lv_obj_t *speedPage = create_speed_page();
    lv_screen_load(speedPage);
}

void loop()
{
    lv_task_handler(); // Ensure this is called
    delay(5);
    lv_tick_inc(5);
}