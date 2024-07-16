#include <page/init_page.h>
#include <string>
#include <config_wifi/ConfigWifi.h>
#include <page/speed_page.h>

ConfigWifi configWifi;
lv_obj_t *init_page_scr;
String ip = "192.198.4.1"; // 假设ip是一个全局变量

lv_obj_t *qr;
lv_obj_t *wifi_tips;

// 样式声明
static lv_style_t style_wifi_tips;
static lv_style_t style_qr;

void init_page_styles()
{
  lv_style_init(&style_wifi_tips);
  lv_style_set_text_color(&style_wifi_tips, lv_color_hex(MAIN_COLOR));
  lv_style_set_border_width(&style_wifi_tips, 2);
  lv_style_set_border_color(&style_wifi_tips, lv_color_hex(MAIN_COLOR));
  lv_style_set_text_align(&style_wifi_tips, LV_TEXT_ALIGN_CENTER);
  lv_style_set_pad_left(&style_wifi_tips, 10);
  lv_style_set_pad_right(&style_wifi_tips, 10);
  lv_style_set_pad_top(&style_wifi_tips, 10);
  lv_style_set_pad_bottom(&style_wifi_tips, 10);

  lv_style_init(&style_qr);
  lv_style_set_bg_color(&style_qr, lv_color_hex(MAIN_COLOR)); // 示例样式，可根据需要修改
}

void init_page_loop(lv_timer_t *timer)
{
  if (configWifi.useWeb)
  {
    configWifi.loop();
  }
  else
  {
    lv_obj_clean(init_page_scr);
    lv_obj_t *speedPage = create_speed_page();
    lv_screen_load_anim(speedPage, LV_SCR_LOAD_ANIM_NONE, 0, 0, true);
    // lv_screen_load(speedPage);
    lv_timer_del(timer);
  }
}

void init_page_init(lv_timer_t *timer)
{
  configWifi.AP_NAME = ip;
  configWifi.begin();
  String labelText = "连接名为 " + ip + " 的WIFI\n并访问此IP进行配网\n网页加载较慢 耐心等待";
  lv_label_set_text(wifi_tips, labelText.c_str());
  lv_obj_align_to(wifi_tips, qr, LV_ALIGN_OUT_BOTTOM_MID, 0, 25);
  lv_timer_create(init_page_loop, 300, NULL);
  lv_timer_del(timer);
}

lv_obj_t *create_init_page()
{
  init_page_scr = lv_obj_create(NULL);

  // 初始化样式
  init_page_styles();

  // 创建二维码对象并应用样式
  qr = lv_qrcode_create(init_page_scr);
  lv_qrcode_set_size(qr, 150);
  lv_qrcode_set_dark_color(qr, lv_color_hex(MAIN_COLOR));
  lv_obj_add_style(qr, &style_qr, 0); // 应用QR码样式
  String data = "http://" + ip;
  lv_qrcode_update(qr, data.c_str(), data.length());
  lv_obj_align(qr, LV_ALIGN_CENTER, 0, -40);

  // 创建标签对象并应用样式
  wifi_tips = lv_label_create(init_page_scr);
  lv_label_set_text(wifi_tips, "Initializing...");
  lv_obj_add_style(wifi_tips, &style_wifi_tips, 0); // 应用标签样式
  lv_obj_align_to(wifi_tips, qr, LV_ALIGN_OUT_BOTTOM_MID, 0, 30);

  // 创建定时器
  lv_timer_create(init_page_init, 1500, NULL);

  return init_page_scr;
}
