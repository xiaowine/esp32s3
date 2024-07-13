#include <page/speed_page.h>

lv_obj_t *speed_speed_scr;

lv_obj_t *scale_line;
lv_obj_t *needle_line;
lv_obj_t *speed_text;
lv_style_t line_style;

// 样式声明
static lv_style_t section_minor_tick_style;
static lv_style_t section_label_style;
static lv_style_t section_main_line_style;
static lv_style_t main_color_style;

void speed_page_styles()
{
  // 初始化指针线条样式
  lv_style_init(&line_style);

  // 初始化分段样式
  lv_style_init(&section_label_style);
  lv_style_set_text_color(&section_label_style, lv_palette_darken(LV_PALETTE_RED, 4));
  lv_style_set_line_color(&section_label_style, lv_palette_darken(LV_PALETTE_RED, 4));
  lv_style_set_line_width(&section_label_style, 5U);

  lv_style_init(&section_minor_tick_style);
  lv_style_set_line_color(&section_minor_tick_style, lv_palette_lighten(LV_PALETTE_RED, 2));
  lv_style_set_line_width(&section_minor_tick_style, 3U);

  lv_style_init(&section_main_line_style);
  lv_style_set_line_color(&section_main_line_style, lv_palette_darken(LV_PALETTE_RED, 4));
  lv_style_set_line_width(&section_main_line_style, 3U);
  lv_style_set_arc_color(&section_main_line_style, lv_palette_darken(LV_PALETTE_RED, 3));
  lv_style_set_arc_width(&section_main_line_style, 4U);

  // 初始化 MAIN_COLOR 样式
  lv_style_init(&main_color_style);
  lv_style_set_text_color(&main_color_style, lv_color_hex(MAIN_COLOR));
  lv_style_set_line_color(&main_color_style, lv_color_hex(MAIN_COLOR));
  lv_style_set_line_width(&main_color_style, 3U);
  lv_style_set_arc_color(&main_color_style, lv_color_hex(MAIN_COLOR));
  lv_style_set_arc_width(&main_color_style, 4U);
}

static void set_needle_line_value(void *obj, int32_t v)
{
  if (v < 260)
  {
    lv_style_set_line_color(&line_style, lv_color_hex(MAIN_COLOR));
  }
  else
  {
    lv_style_set_line_color(&line_style, lv_color_hex(0xFF0000));
  }
  lv_scale_set_line_needle_value(scale_line, needle_line, 70, v);
}

lv_obj_t *create_speed_page()
{
  speed_speed_scr = lv_obj_create(NULL);

  // 初始化样式
  speed_page_styles();

  // 创建刻度盘对象
  scale_line = lv_scale_create(speed_speed_scr);
  lv_obj_set_size(scale_line, 250, 250);
  lv_scale_set_mode(scale_line, LV_SCALE_MODE_ROUND_INNER);
  lv_obj_align(scale_line, LV_ALIGN_LEFT_MID, 0, 0);
  lv_obj_set_x(scale_line, -75);
  lv_obj_set_y(scale_line, -30);

  lv_scale_set_label_show(scale_line, true);
  lv_scale_set_total_tick_count(scale_line, 81);
  lv_scale_set_major_tick_every(scale_line, 10);
  lv_obj_set_style_length(scale_line, 10, LV_PART_ITEMS);
  lv_obj_set_style_length(scale_line, 18, LV_PART_INDICATOR);
  lv_scale_set_range(scale_line, 0, 400);
  lv_scale_set_rotation(scale_line, 250);
  lv_scale_set_angle_range(scale_line, 220);

  // 为 0-260 范围添加样式
  lv_scale_section_t *main_color_section = lv_scale_add_section(scale_line);
  lv_scale_section_set_range(main_color_section, 0, 260);
  lv_scale_section_set_style(main_color_section, LV_PART_INDICATOR, &main_color_style);
  lv_scale_section_set_style(main_color_section, LV_PART_ITEMS, &main_color_style);
  lv_scale_section_set_style(main_color_section, LV_PART_MAIN, &main_color_style);

  // 为 260-400 范围添加样式
  lv_scale_section_t *section = lv_scale_add_section(scale_line);
  lv_scale_section_set_range(section, 260, 400);
  lv_scale_section_set_style(section, LV_PART_INDICATOR, &section_label_style);
  lv_scale_section_set_style(section, LV_PART_ITEMS, &section_minor_tick_style);
  lv_scale_section_set_style(section, LV_PART_MAIN, &section_main_line_style);

  // 创建指针对象
  needle_line = lv_line_create(scale_line);
  lv_obj_set_style_line_width(needle_line, 5, LV_PART_MAIN);
  lv_obj_set_style_line_rounded(needle_line, true, LV_PART_MAIN);
  lv_obj_add_style(needle_line, &line_style, LV_PART_MAIN);

  // 创建动画
  lv_anim_t anim_scale_line;
  lv_anim_init(&anim_scale_line);
  lv_anim_set_var(&anim_scale_line, scale_line);
  lv_anim_set_exec_cb(&anim_scale_line, set_needle_line_value);
  lv_anim_set_duration(&anim_scale_line, 700);
  lv_anim_set_repeat_count(&anim_scale_line, LV_ANIM_REPEAT_INFINITE);
  lv_anim_set_playback_duration(&anim_scale_line, 700);
  lv_anim_set_values(&anim_scale_line, 0, 360);
  lv_anim_start(&anim_scale_line);

  // speed_text = lv_label_create(speed_speed_scr);
  // lv_label_set_text(speed_text, "aaa");

  return speed_speed_scr;
}
