#include <U8g2lib.h>
#include <Wire.h>
#include <ADS1115_WE.h>
#include "MotorControl.h"

MotorControl motor1(2, 8, 9);
MotorControl motor2(3, 10, 11);
MotorControl motor3(4, 12, 13);
MotorControl motor4(5, 14, 15);
MotorControl motor5(6, 19, 18);
MotorControl motor6(7, 17, 16);

MotorControl& selected_motor(int motor_num) {
  switch (motor_num) {
    case 1: return motor1;
    case 2: return motor2;
    case 3: return motor3;
    case 4: return motor4;
    case 5: return motor5;
    case 6: return motor6;
    default: return motor1;
  }
}

char oled_voltage[30];
char oled_v_result[20];

#define I2C_ADDRESS 0x48
ADS1115_WE adc = ADS1115_WE(&Wire, I2C_ADDRESS);

#define SPEED 4

#define BTN0 28
#define BTN1 26
#define BTN2 27

U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R1, /* reset=*/U8X8_PIN_NONE, /* clock=*/1, /* data=*/0);
const unsigned char headimg[518] = {0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X78,0X00,0X00,0X0F,0X00,0X00,
0X00,0X00,0XF0,0X1F,0X00,0X00,0X00,0X00,0X00,0X00,0XF0,0X0F,0X00,0X00,0X00,0X00,
0X00,0X00,0XF0,0X7F,0X00,0X08,0X00,0X00,0X00,0X00,0XF8,0X3F,0X00,0X88,0X00,0X00,
0X00,0X80,0XF9,0X3F,0X00,0X0C,0X10,0X00,0X00,0X00,0XE8,0X3F,0X00,0X04,0X30,0X00,
0X80,0X00,0XE0,0X3F,0X00,0X00,0XFC,0X00,0X80,0X07,0XE0,0X3F,0X00,0X60,0X78,0X00,
0XC0,0X03,0XE0,0X1F,0X00,0X02,0X38,0X00,0XE0,0X03,0XE0,0X3F,0X00,0X02,0X28,0X03,
0X80,0X27,0XE0,0X1F,0X00,0X02,0X00,0X00,0X80,0X60,0XE0,0X3F,0X00,0X01,0X00,0X00,
0X00,0X00,0XA0,0X3F,0XC0,0X00,0X00,0X00,0X00,0X00,0X40,0X02,0X00,0X00,0X80,0X00,
0X00,0X40,0X71,0X01,0X00,0XE0,0X01,0X00,0X00,0X10,0XA4,0X01,0X00,0X02,0X04,0X00,
0X00,0X00,0XAE,0X00,0X00,0X3A,0X00,0X00,0X00,0X84,0X9E,0X00,0X00,0X3E,0X10,0X00,
0X08,0XE4,0X7B,0X01,0X00,0XEE,0X01,0X00,0X20,0XE2,0X43,0X1E,0X78,0XE3,0X01,0X00,
0X00,0XE2,0X09,0X02,0X60,0XE8,0X13,0X00,0X00,0XF2,0X60,0XF0,0X43,0XC6,0X13,0X00,
0X00,0XE4,0X40,0XF7,0X73,0X81,0X13,0X00,0X00,0XD8,0X00,0XE0,0X0F,0X80,0X09,0X01,
0X00,0X90,0X06,0X0B,0X04,0XD0,0X07,0X00,0X00,0X40,0XF9,0X03,0XFC,0X9B,0X08,0X00,
0X00,0X40,0XFD,0XD9,0XF8,0X01,0X00,0X00,0X00,0X40,0XFE,0X50,0XF9,0X70,0X00,0X00,
0X00,0X41,0X7A,0X34,0X7C,0X78,0X20,0X00,0X00,0X43,0X38,0X2D,0X3A,0XF5,0X01,0X00,
0X00,0X21,0X9C,0X07,0X9C,0X77,0X03,0X00,0X00,0X60,0XD0,0X1F,0XF4,0XCF,0X42,0X00,
0X00,0X20,0XE0,0X2D,0XF2,0XCD,0X03,0X00,0X00,0X60,0XD1,0X02,0XF4,0X82,0X03,0X00,
0X00,0X60,0X00,0XFA,0X07,0X80,0X03,0X00,0X00,0X40,0X00,0XE0,0X01,0XC0,0X83,0X00,
0X80,0X41,0X00,0XE2,0X01,0XE0,0X83,0X00,0X00,0X40,0X01,0X02,0X00,0XE0,0X01,0X00,
0X00,0X80,0X01,0X02,0X00,0X60,0X81,0X00,0X00,0X80,0X03,0XC4,0X00,0X78,0X81,0X00,
0X00,0X00,0X05,0X04,0X00,0X38,0X80,0X00,0X00,0X00,0X0F,0X08,0X04,0X0F,0X80,0X00,
0X00,0X00,0XFC,0XF8,0X87,0X1F,0XC0,0X00,0X20,0X81,0XFE,0XE8,0XEF,0X1F,0X60,0X00,
0X00,0X40,0XF8,0XFF,0XFC,0X0F,0X40,0X00,0X00,0X40,0XBE,0XFF,0XDF,0X07,0X40,0X00,
0X00,0XA0,0X00,0XE8,0X55,0X17,0X60,0X00,0X00,0X00,0X00,0XF0,0X45,0X17,0X61,0X00,
0X00,0X10,0X31,0X40,0X01,0X6E,0X38,0X00,0X00,0X10,0X00,0X00,0X00,0X1F,0X30,0X00,
0X00,0X08,0X00,0X00,0X00,0XFE,0X1F,0X00,0X00,0X68,0X00,0X00,0X01,0XFE,0X0F,0X00,
0X00,0X08,0X00,0X00,0X00,0XFE,0X07,0X00,0X00,0X38,0X33,0X40,0X03,0X7E,0X01,0X00,
0X00,0XB8,0X12,0X10,0X03,0X0E,0X00,0X00,0X00,0XE5,0XFF,0X02,0X00,0X00,0X00,0X00,
0X00,0X00,0XFA,0X93,0X01,0X00,0X00,0X00,0X00,0X00,0X00,0XF6,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X14,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
};

uint8_t disappear_step = 1;
int motor_speed = 0;
int motor_speed_pid = 0;

uint8_t* buf_ptr;
uint16_t buf_len;

uint8_t x;
int16_t y, y_trg;
uint8_t line_y, line_y_trg;
uint8_t box_width, box_width_trg;
int16_t box_y, box_y_trg;
int8_t ui_select;

uint8_t ui_index, ui_state;

enum {
  M_SELECT,
  M_MOTOR1,
  M_MOTOR2,
  M_MOTOR3,
  M_MOTOR4,
  M_MOTOR5,
  M_MOTOR6,
  M_SPEED_ADJUST,
  M_ABOUT,
};

enum {
  S_NONE,
  S_DISAPPEAR,
  S_SWITCH,
  S_MENU_TO_MENU,
  S_MENU_TO_PIC,
  S_PIC_TO_MENU,
};

typedef struct {
  const char* select;
} SELECT_LIST;

SELECT_LIST list[] = {
  { "-1# Motor" },
  { "-2# Motor" },
  { "-3# Motor" },
  { "-4# Motor" },
  { "-5# Motor" },
  { "-6# Motor" },
  { "-Speed" },
  { "{ About }" },
};

uint8_t list_num = sizeof(list) / sizeof(SELECT_LIST);
uint8_t single_line_length = 127 / list_num;
uint8_t total_line_length = single_line_length * list_num + 1;

typedef struct {
  bool val;
  bool last_val;
} KEY;

KEY key[3] = { false };

typedef struct {
  uint8_t id;
  bool pressed;
} KEY_MSG;

KEY_MSG key_msg = { 0 };

bool get_key_val(uint8_t ch) {
  switch (ch) {
    case 0: return digitalRead(BTN0);
    case 1: return digitalRead(BTN1);
    case 2: return digitalRead(BTN2);
    default: return false;
  }
}

void key_init() {
  for (uint8_t i = 0; i < (sizeof(key) / sizeof(KEY)); ++i) {
    key[i].val = key[i].last_val = get_key_val(i);
  }
}

void key_scan() {
  for (uint8_t i = 0; i < (sizeof(key) / sizeof(KEY)); ++i) {
    key[i].val = get_key_val(i);
    if (key[i].last_val != key[i].val) {
      key[i].last_val = key[i].val;
      if (key[i].val == LOW) {
        key_msg.id = i;
        key_msg.pressed = true;
      }
    }
  }
}

bool move(int16_t* a, int16_t* a_trg) {
  if (*a < *a_trg) {
    *a += SPEED;
    if (*a > *a_trg) *a = *a_trg;
  } else if (*a > *a_trg) {
    *a -= SPEED;
    if (*a < *a_trg) *a = *a_trg;
  } else {
    return true;
  }
  return false;
}

bool move_width(uint8_t* a, uint8_t* a_trg, uint8_t select, uint8_t id) {
  if (*a < *a_trg) {
    uint8_t step = 16 / SPEED;
    uint8_t len;
    if (ui_index == M_SELECT) {
      len = abs(u8g2.getStrWidth(list[select].select) - u8g2.getStrWidth(list[id == 0 ? select + 1 : select - 1].select));
    }
    uint8_t width_speed = ((len % step) == 0 ? (len / step) : (len / step + 1));
    *a += width_speed;
    if (*a > *a_trg) *a = *a_trg;
  } else if (*a > *a_trg) {
    uint8_t step = 16 / SPEED;
    uint8_t len;
    if (ui_index == M_SELECT) {
      len = abs(u8g2.getStrWidth(list[select].select) - u8g2.getStrWidth(list[id == 0 ? select + 1 : select - 1].select));
    }
    uint8_t width_speed = ((len % step) == 0 ? (len / step) : (len / step + 1));
    *a -= width_speed;
    if (*a < *a_trg) *a = *a_trg;
  } else {
    return true;
  }
  return false;
}

bool move_bar(uint8_t* a, uint8_t* a_trg) {
  if (*a < *a_trg) {
    uint8_t step = 16 / SPEED;
    uint8_t width_speed = ((single_line_length % step) == 0 ? (single_line_length / step) : (single_line_length / step + 1));
    *a += width_speed;
    if (*a > *a_trg) *a = *a_trg;
  } else if (*a > *a_trg) {
    uint8_t step = 16 / SPEED;
    uint8_t width_speed = ((single_line_length % step) == 0 ? (single_line_length / step) : (single_line_length / step + 1));
    *a -= width_speed;
    if (*a < *a_trg) *a = *a_trg;
  } else {
    return true;
  }
  return false;
}

void disappear() {
  switch (disappear_step) {
    case 1:
      for (uint16_t i = 0; i < buf_len; ++i) {
        if (i % 2 == 0) buf_ptr[i] = buf_ptr[i] & 0x55;
      }
      break;
    case 2:
      for (uint16_t i = 0; i < buf_len; ++i) {
        if (i % 2 != 0) buf_ptr[i] = buf_ptr[i] & 0xAA;
      }
      break;
    case 3:
      for (uint16_t i = 0; i < buf_len; ++i) {
        if (i % 2 == 0) buf_ptr[i] = buf_ptr[i] & 0x00;
      }
      break;
    case 4:
      for (uint16_t i = 0; i < buf_len; ++i) {
        if (i % 2 != 0) buf_ptr[i] = buf_ptr[i] & 0x00;
      }
      break;
    default:
      ui_state = S_NONE;
      disappear_step = 0;
      break;
  }
  disappear_step++;
}

void select_ui_show() {
  u8g2.setFont(u8g2_font_wqy12_t_chinese1);
  move_bar(&line_y, &line_y_trg);
  move(&y, &y_trg);
  move(&box_y, &box_y_trg);
  move_width(&box_width, &box_width_trg, ui_select, key_msg.id);
  u8g2.drawVLine(62, 0, total_line_length);
  u8g2.drawPixel(61, 0);
  u8g2.drawPixel(63, 0);
  for (uint8_t i = 0; i < list_num; ++i) {
    u8g2.drawStr(x, 16 * i + y + 12, list[i].select);
    u8g2.drawPixel(61, single_line_length * (i + 1));
    u8g2.drawPixel(63, single_line_length * (i + 1));
  }
  u8g2.drawVLine(61, line_y, single_line_length - 1);
  u8g2.drawVLine(63, line_y, single_line_length - 1);
  u8g2.setDrawColor(2);
  u8g2.drawRBox(0, box_y, box_width, 16, 1);
  u8g2.setDrawColor(1);
}

float readChannel(ADS1115_MUX channel) {
  float voltage = 0.0;
  adc.setCompareChannels(channel);
  adc.startSingleMeasurement();
  while (adc.isBusy()) {}
  voltage = adc.getResult_V();
  return voltage;
}

void motor_reset() {
  motor1.reset();
  motor2.reset();
  motor3.reset();
  motor4.reset();
  motor5.reset();
  motor6.reset();
}

void speed_adjust_ui_show() {
  char buf[20];
  float voltage = readChannel(ADS1115_COMP_0_GND);
  Serial.println(voltage);
  dtostrf(voltage, 4, 2, oled_v_result);
  strcat(oled_voltage, oled_v_result);
  sprintf(buf, "%d# Speed", 1);
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_wqy15_t_gb2312);
  u8g2.drawStr(0, 15, buf);
  u8g2.setCursor(0, 35);
  u8g2.print("速度：");
  u8g2.setCursor(0, 55);
  u8g2.print(motor_speed_pid);
  u8g2.setCursor(0, 75);
  u8g2.print("电压：");
  u8g2.setCursor(0, 95);
  u8g2.print(oled_voltage);
  u8g2.sendBuffer();
  oled_voltage[0] = '\0';
  oled_v_result[0] = '\0';
}

void motor_show() {
  char buf[20];
  float voltage = readChannel(ADS1115_COMP_0_GND);
  sprintf(buf, "Motor %d", ui_select + 1);
  Serial.println(voltage);
  dtostrf(voltage, 4, 2, oled_v_result);
  strcat(oled_voltage, oled_v_result);
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_wqy15_t_gb2312);
  u8g2.drawStr(0, 15, buf);
  u8g2.setCursor(0, 35);
  u8g2.print("电压：");
  u8g2.setFont(u8g2_font_inb16_mr);
  u8g2.setCursor(0, 72);
  u8g2.print(oled_voltage);
  u8g2.sendBuffer();
  oled_voltage[0] = '\0';
  oled_v_result[0] = '\0';
}

void about_ui_show() {
  u8g2.drawXBMP(0, 0, 64, 64, headimg);
  u8g2.setCursor(10, 85);
  u8g2.print("HW:2.00");
  u8g2.setCursor(10, 105);
  u8g2.print("SW:2.00_pr");
}

void select_proc() {
  motor_reset();
  if (key_msg.pressed) {
    key_msg.pressed = false;
    switch (key_msg.id) {
      case 0:
        if (ui_select < 1) break;
        ui_select -= 1;
        line_y_trg -= single_line_length;
        if (ui_select < -(y / 16)) {
          y_trg += 16;
        } else {
          box_y_trg -= 16;
        }
        break;
      case 1:
        if ((ui_select + 2) > (sizeof(list) / sizeof(SELECT_LIST))) break;
        ui_select += 1;
        line_y_trg += single_line_length;
        if ((ui_select + 1) > (8 - y / 16)) {
          y_trg -= 16;
        } else {
          box_y_trg += 16;
        }
        break;
      case 2:
        switch (ui_select) {
          case 0:
            ui_state = S_DISAPPEAR;
            ui_index = M_MOTOR1;
            break;
          case 1:
            ui_state = S_DISAPPEAR;
            ui_index = M_MOTOR2;
            break;
          case 2:
            ui_state = S_DISAPPEAR;
            ui_index = M_MOTOR3;
            break;
          case 3:
            ui_state = S_DISAPPEAR;
            ui_index = M_MOTOR4;
            break;
          case 4:
            ui_state = S_DISAPPEAR;
            ui_index = M_MOTOR5;
            break;
          case 5:
            ui_state = S_DISAPPEAR;
            ui_index = M_MOTOR6;
            break;
          case 6:
            ui_state = S_DISAPPEAR;
            ui_index = M_SPEED_ADJUST;
            break;
          case 7:
            ui_state = S_DISAPPEAR;
            ui_index = M_ABOUT;
            break;
          default: break;
        }
        break;
      default: break;
    }
    box_width_trg = u8g2.getStrWidth(list[ui_select].select) + x * 2;
  }
  select_ui_show();
}

bool button0_pressed = false;
bool button1_pressed = false;

void motor_proc() {
  selected_motor(ui_select + 1).stop();
  if (digitalRead(BTN0)) {
    button0_pressed = true;
    selected_motor(ui_select + 1).reverse();
  } else if (button0_pressed) {
    button0_pressed = false;
    selected_motor(ui_select + 1).stop();
  }

  if (digitalRead(BTN1)) {
    button1_pressed = true;
    selected_motor(ui_select + 1).forward();
  } else if (button1_pressed) {
    button1_pressed = false;
    selected_motor(ui_select + 1).stop();
  }

  if (key_msg.pressed) {
    key_msg.pressed = false;
    if (key_msg.id == 2) {
      ui_state = S_DISAPPEAR;
      ui_index = M_SELECT;
    }
  }
  motor_show();
}

float Kp = 14;
float Ki = 0.9;
float Kd = 0.5;

float setpoint = 3.6;
float error = 0.0;
float last_error = 0.0;
float integral = 0.0;
float derivative = 0.0;

void speed_adjust_proc() {
  static bool button0_pressed = false;
  static bool button1_pressed = false;
  float voltage = readChannel(ADS1115_COMP_0_GND);

  error = setpoint - voltage;

  if (abs(error) <= 0.06) {
    motor_speed_pid = 0;
    selected_motor(1).stop();
  } else {
    integral += error;
    derivative = error - last_error;

    float pid_output = Kp * error + Ki * integral + Kd * derivative;

    if (pid_output > 0) {
      motor_speed_pid = constrain(pid_output, 130, 255);
    } else {
      motor_speed_pid = constrain(pid_output, -255, -130);
    }
    selected_motor(1).setSpeed(motor_speed_pid);
  }
  last_error = error;
  if (digitalRead(BTN0)) {  // 按键0被按下
    button0_pressed = true;
    motor_speed_pid += 51;                             // 增加速度
    if (motor_speed_pid > 255) motor_speed_pid = 255;  // 限制最大速度
    selected_motor(1).setSpeed(motor_speed_pid);
  } else if (button0_pressed) {  // 按键0被释放
    button0_pressed = false;
  }

  // 检查按键1的当前状态
  if (digitalRead(BTN1)) {  // 按键1被按下
    button1_pressed = true;
    motor_speed_pid -= 51;                               // 减少速度
    if (motor_speed_pid < -255) motor_speed_pid = -255;  // 限制最小速度
    selected_motor(1).setSpeed(motor_speed_pid);
  } else if (button1_pressed) {  // 按键1被释放
    button1_pressed = false;
  }

  // 处理退出按键
  if (key_msg.pressed) {
    key_msg.pressed = false;
    if (key_msg.id == 2) {  // 退出
      ui_state = S_DISAPPEAR;
      ui_index = M_SELECT;
    }
  }
  speed_adjust_ui_show();
}

void about_proc()  //about界面处理函数
{
  if (key_msg.pressed) {
    key_msg.pressed = false;
    ui_state = S_DISAPPEAR;
    ui_index = M_SELECT;
  }
  about_ui_show();
}
  void ui_proc()  //总的UI进程
{
  switch (ui_state) {
    case S_NONE:
      u8g2.clearBuffer();
      switch (ui_index) {
        case M_SELECT:
          select_proc();
          break;
        case M_MOTOR1:
        case M_MOTOR2:
        case M_MOTOR3:
        case M_MOTOR4:
        case M_MOTOR5:
        case M_MOTOR6:
          motor_proc();
          break;
        case M_SPEED_ADJUST:
          speed_adjust_proc();
          break;
        case M_ABOUT:
          about_proc();
          break;
        default:
          break;
      }
      break;
    case S_DISAPPEAR:
      disappear();
      break;
    default:
      break;
  }
  u8g2.sendBuffer();
}

void setup() {
  Serial.begin(9600);
  pinMode(BTN0, INPUT_PULLUP);
  pinMode(BTN1, INPUT_PULLUP);
  pinMode(BTN2, INPUT_PULLUP);
  key_init();
  Wire.setSCL(1);
  Wire.setSDA(0);
  u8g2.begin();
  u8g2.enableUTF8Print();
  u8g2.setFont(u8g2_font_wqy12_t_chinese1);
  if (adc.init()) {
    Serial.println("ADS1115 connected!");
    adc.setVoltageRange_mV(ADS1115_RANGE_6144);
  }
  buf_ptr = u8g2.getBufferPtr();  //拿到buffer首地址
  buf_len = 8 * u8g2.getBufferTileHeight() * u8g2.getBufferTileWidth();

  x = 4;
  y = y_trg = 0;
  line_y = line_y_trg = 1;
  ui_select = 0;

  box_width = box_width_trg = u8g2.getStrWidth(list[ui_select].select) + x * 2;  //两边各多2
  ui_index = M_SELECT;
  ui_state = S_NONE;
}

void loop() {
  key_scan();
  ui_proc();
}