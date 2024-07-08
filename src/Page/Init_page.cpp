#include <page/init_page.h>
#include <string>
#include <config_wifi/ConfigWifi.h>

ConfigWifi configWifi;
lv_obj_t *scr;
String ip = "192.198.1.4"; // 假设ip是一个全局变量

void init(lv_timer_t *timer)
{
  lv_obj_t *qr = lv_obj_get_child(scr, 0);
  lv_obj_t *label = lv_obj_get_child(scr, 1);
  String labelText = "连接名为 " + ip + " 的WIFI\n并访问此IP进行配网\n网页加载较慢 耐心等待";
  lv_label_set_text(label, labelText.c_str());
  lv_obj_align_to(label, qr, LV_ALIGN_OUT_BOTTOM_MID, 0, 25);
}

void loop(lv_timer_t *timer)
{

  if (configWifi.useWeb)
  {
    configWifi.loop();
  }
  else
  {
    lv_timer_del(timer);
  }
}

lv_obj_t *createInitPage()
{

  scr = lv_obj_create(NULL);

  lv_obj_t *qr = lv_qrcode_create(scr);
  lv_qrcode_set_size(qr, 150);
  String data = "http://" + ip;
  lv_qrcode_update(qr, data.c_str(), data.length());
  lv_obj_align(qr, LV_ALIGN_CENTER, 0, -40);

  lv_obj_t *label = lv_label_create(scr);
  lv_obj_set_style_border_width(label, 2, 0);
  lv_obj_set_style_border_color(label, lv_color_black(), 0);
  lv_label_set_text(label, "Initializing...");
  lv_obj_align_to(label, qr, LV_ALIGN_OUT_BOTTOM_MID, 0, 30);
  lv_obj_set_style_pad_left(label, 10, 0);
  lv_obj_set_style_pad_right(label, 10, 0);
  lv_obj_set_style_pad_top(label, 10, 0);
  lv_obj_set_style_pad_bottom(label, 10, 0);

  configWifi.AP_NAME = ip;
  configWifi.begin();

  lv_timer_create(init, 300, NULL);
  lv_timer_create(loop, 600, NULL);
  return scr;
}
