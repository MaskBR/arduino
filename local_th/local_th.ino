// Ver1.0，根据网络版2.2程序修改为本地1.0版本
#include <LedControl.h>
#include<Wire.h>
#include <ArduinoJson.h>
#include <U8g2lib.h>

#define SDA_PIN 4               // SDA引脚，gpio4(D2)
#define SCL_PIN 5            // SCL引脚，默认gpio5(D1)
#define SHT30_ADDR 0x44

typedef struct
{
  float temp;
  float hum;
}t_h_struct;

t_h_struct sht3x_data;
t_h_struct get_sht3x_data();

String PATH_NAME;
float sht3x_temp;
float sht3x_hum;

const char* software_version="1.00";

char oled_tem[40];
char oled_hum[40];
char oled_time[50];
char oled_sw[30];

U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2(U8G2_R2, /* clock=*/ 5, /* data=*/ 4, /* reset=*/ U8X8_PIN_NONE);

void setup() {
  Serial.begin(9600);//开启串口监视器
  delay(10);
  Wire.begin(SDA_PIN, SCL_PIN);
  u8g2.begin();
  u8g2.enableUTF8Print();    // enable UTF8 support for the Arduino print() function
  u8g2.setFontDirection(0);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN,HIGH);
  strcat(oled_sw,"SW:");
  strcat(oled_sw,software_version);
  show_server_oled("Current Verison:","",oled_sw);
}

void loop() {
  Serial.println("Version: "+String(software_version));
  sht3x_data=get_sht3x_data();
  sht3x_temp=sht3x_data.temp;
  sht3x_hum=sht3x_data.hum;
  //显示屏显示代码
  build_th_oled();
  delay(5000);
}

void build_th_oled(){
  //oled显示部分代码
  char oled_t_result[20],oled_h_result[20];
  dtostrf(sht3x_temp,4,2,oled_t_result);
  dtostrf(sht3x_hum,4,2,oled_h_result);
  strcat(oled_tem,"T:");
  strcat(oled_tem,oled_t_result);
  strcat(oled_hum,"H:");
  strcat(oled_hum,oled_h_result);
  strcat(oled_hum,"%");
  show_th_oled(oled_tem,oled_hum);
  oled_tem[0]=0;
  oled_hum[0]=0;
  
  Serial.print("sht3x temperature is :");
  Serial.println(oled_t_result);
  Serial.print("sht3x humidity is :");
  Serial.println(oled_h_result);
}

t_h_struct get_sht3x_data(){
   unsigned int data[6]; //存储获取到的六个数据
   float temp, hum;
  //开始IIC
  //写地址
  Wire.beginTransmission(SHT30_ADDR);
  //发送测量命令 0x2C06,但是因为IIC一次只能发一个8位数据，所以得发两次
  Wire.write(0x2C);
  Wire.write(0x06);
  //停止IIC
  Wire.endTransmission();
  //等待500ms是等待SHT30器件测量数据，实际上这个时间可以很短
  delay(500);

  //请求获取6字节的数据，然后会存到8266的内存里
  Wire.requestFrom(SHT30_ADDR, 6);

  //读取6字节的数据
  //这六个字节分别为：温度8位高数据，温度8位低数据，温度8位CRC校验数据
  //               湿度8位高数据，湿度8位低数据，湿度8位CRC校验数据
  if (Wire.available() == 6)
  {
    data[0] = Wire.read();
    data[1] = Wire.read();
    data[2] = Wire.read();
    data[3] = Wire.read();
    data[4] = Wire.read();
    data[5] = Wire.read();
  }
  //然后计算得到的数据，要转化为摄氏度、华氏度、相对湿度
  temp = ((((data[0] * 256.0) + data[1]) * 175) / 65535.0) - 45;
  hum = ((((data[3] * 256.0) + data[4]) * 100) / 65535.0);
  if(temp<200){
    sht3x_data.temp=temp;
    sht3x_data.hum=hum;
  }else{
    sht3x_data.temp=0;
    sht3x_data.hum=0;
    Serial.println("SHT3X read error!");
  }
  return sht3x_data;
}

void show_th_oled(char* line1,char* line2){
  u8g2.setFont(u8g2_font_inb16_mr); 
  u8g2.clearBuffer();
  u8g2.drawStr(0, 20,line1);
  u8g2.drawCircle(100,3,2,U8G2_DRAW_ALL);
  u8g2.drawStr(105, 20,"C");
  u8g2.drawStr(0, 50,line2);
  u8g2.sendBuffer(); 
}

void show_server_oled(const char* line1,const char* line2,const char* line3){
  u8g2.setFont(u8g2_font_unifont_t_chinese1); 
  u8g2.clearBuffer();
  u8g2.drawStr(0, 20,line1);
  u8g2.drawStr(0, 40,line2);
  u8g2.drawStr(0, 60,line3);
  u8g2.sendBuffer(); 
}

