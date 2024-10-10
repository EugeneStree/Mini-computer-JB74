/*Suzuki Jimny JB74 on-board mini computer
  Temperature of the engine and automatic transmission, pressure in the wheel TPMS
  Automatic lighting according to the temperature control of the engine 100C

  MCP2515 SPI MISO-D12
             MOSI-D11
             SDK -D13
             CS  -D8

  LCD1602 I2C SDA-A4
             SCL-A5

  Written By: Eugene Belov 09/10/2024
*/

#include <SPI.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <mcp2515.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);
MCP2515 mcp2515_8(8);                       // Set CS to pin 8
struct can_frame canMsg;
byte Mess_3_out2 = 0;
byte Mess_3_out3 = 0;
byte Mess_3_out4 = 0;
byte Mess_3_out5 = 0;
byte Mess_2_out4 = 0;
byte Mess_1_out5 = 0;
String text_dis_1;
String text_dis_2;
String text_dis_3;
String text_dis_4;
String text_dis_5;
String text_dis_6;
uint16_t tmr;

void setup() {

  pinMode(4, OUTPUT);                        // Set Out active buzzer pin 4
  digitalWrite(4, 0);
  Wire.begin();
  lcd.init();
  lcd.backlight();
  SPI.begin();
  mcp2515_8.reset();
  mcp2515_8.setBitrate(CAN_500KBPS, MCP_8MHZ);
  mcp2515_8.setConfigMode();
  mcp2515_8.setFilterMask(MCP2515::MASK0, false, 0x07FF);
  mcp2515_8.setFilter(MCP2515::RXF0, false, 0x3F6);
  mcp2515_8.setFilter(MCP2515::RXF1, false, 0x380);
  mcp2515_8.setFilterMask(MCP2515::MASK1, false, 0x07FF);
  mcp2515_8.setFilter(MCP2515::RXF2, false, 0x3E2);
  mcp2515_8.setFilter(MCP2515::RXF3, false, 0x600);
  mcp2515_8.setFilter(MCP2515::RXF4, false, 0x600);
  mcp2515_8.setFilter(MCP2515::RXF5, false, 0x600);
  mcp2515_8.setListenOnlyMode();
  lcd.setCursor(3, 0);
  lcd.print("SUZUKI V2");
  lcd.setCursor(0, 1);
  lcd.print("GOOD TRAVEL BRO!");
  delay(2000);
  lcd.clear();
}
void loop() {

  if (mcp2515_8.readMessage(&canMsg) == MCP2515::ERROR_OK) {
    if (canMsg.can_id == 0x3F6) {                                 //Если есть ID 0x3F6
      Mess_2_out4 = canMsg.data[4];                               //то присваиваем значение четвертого байта переменной
    text_dis_2 = (String((Mess_2_out4) - (40), DEC));             //Высчитываем температуру двигателя и присваиваем переменной
    lcd.setCursor(1, 0);                                          //Выставляем курсор
    lcd.print(text_dis_2);                                        // Выводим на дисплей
    lcd.write(223);                                               // Выводим на дисплей знак градуса
  }
    if (canMsg.can_id == 0x380) {
      Mess_1_out5 = canMsg.data[5];
    text_dis_1 = (String((Mess_1_out5) - (40), DEC));
    lcd.setCursor(1, 1);
    lcd.print(text_dis_1);
    lcd.write(223); 
  }
    if (canMsg.can_id == 0x3E2) {
      Mess_3_out2 = canMsg.data[2];
      Mess_3_out3 = canMsg.data[3];
      Mess_3_out4 = canMsg.data[4];
      Mess_3_out5 = canMsg.data[5];
    text_dis_5 = (float (Mess_3_out5) * (0.014));
    lcd.setCursor(6, 1);
    lcd.print(text_dis_5);
    text_dis_4 = (float (Mess_3_out4) * (0.014));
    lcd.setCursor(12, 1);
    lcd.print(text_dis_4);
    text_dis_3 = (float (Mess_3_out3) * (0.014));
    lcd.setCursor(6, 0);
    lcd.print(text_dis_3);
    text_dis_6 = (float (Mess_3_out2) * (0.014));
    lcd.setCursor(12, 0);
    lcd.print(text_dis_6);
    }
    if ((Mess_2_out4) == 140) {                                     //Установка температуры Alarm 100С (140-40)
      digitalWrite(4, 0);                                           //Выход бузера  в 0
      uint16_t ms = millis();                                       //Функция задержки времени
      if (ms - tmr > 500) {                                         //Пауза пол секунды
        tmr = ms;
        digitalWrite(4, 1);                                         //Выход бузера в 1
      }
    }
  }
}
