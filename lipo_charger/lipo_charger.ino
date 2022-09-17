#include "U8glib.h"
#include<Wire.h>
U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NONE);  // I2C / TWI
unsigned long interval;
float now_volt ;
int times_str[2][3];    //= {{00, 0, 00, 0, 00}, {00, 0, 00, 0, 00}};
//二次元配列 times[0][]が表示する方,times[1][]が差分として使用する方
unsigned long times;
int sec;
int min_;
int hour;
//float Comparison_volt[]={3.5,3.6,3.65,3.7,3.75,3.8,3,85,3.9,}
//byte chargevolt[];
void setup() {
  Serial.begin(115200);
  Wire.begin();
  u8g.setColorIndex(1);
  pinMode(3, OUTPUT);
  pinMode(5, OUTPUT);
}

void loop() {
  times = millis() / 1000;
  times_str[0][2] = times % 60;
  if (times % 60 == 0) {
    if (times_str[1][2] - times_str[0][2] == 59) {
      min_++;
      times_str[0][1] = min_ % 60;
      if (times % 3600 == 0) {
        if (times_str[1][1] - times_str[0][1] == 59) {
          hour++;
          times_str[0][0] = hour;
        }
      }
    }
  }
  times_str[1][2] = times_str[0][2];
  times_str[1][1] = times_str[0][1];

  float volt_source = read_volt();

  now_volt = (volt_source * 5.00) / 1024.00;
  if ( 4.1 > now_volt) {
    analogWrite(3, 255 - (now_volt * 51.2) - 15.2); //0-255
  }
  if (4.2 <= now_volt) {
    analogWrite(3, 40);
  }
  Serial.print( volt_source);
  Serial.print("/t");
  Serial.print("充電電圧");
  Serial.print("/t");
  Serial.println(255 - (now_volt * 51.2) - 15.2);
  Serial.print("/t");
  Serial.println( (((now_volt * 51.2) - 15.2)/256)*5);

  if (millis() - interval > 100) {
    u8g.firstPage();
    do {
      u8g.setFont(u8g_font_unifont);
      byte x_volt = u8g.getStrWidth("voltage: ");
      u8g.drawStr( 0, 22, "voltage: ");
      u8g.setPrintPos(x_volt + 5, 20);
      u8g.print(now_volt);
      u8g.print("v");

      byte y_volt = u8g.getFontAscent();
      u8g.drawStr(0, 22 + y_volt + 5, "times: ");
      byte x_times = u8g.getStrWidth("times: ");
      u8g.setPrintPos(x_times + 5, 23 + y_volt + 5);
      for (int i = 0; i < 3; i++) {
        if (i == 1) {
          u8g.print(":");
        }

        if (i == 2) {
          u8g.print(".");
        }
        if (i > 0) {
          if (times_str[0][i] < 10) {
            u8g.print("0");
          }
        }
        u8g.print(times_str[0][i]);
      }

    } while ( u8g.nextPage() );
    interval = millis();
  }
}


float  read_volt() {
  long val = 0;
  float val2;
  for (int i = 0; i < 100; i++) {
    val = val + analogRead(A1);
  }
  val2 = val / 100;
  return val2;
}
