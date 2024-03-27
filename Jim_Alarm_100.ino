/*Suzuki Jimny JB74 on-board mini computer 
Temperature of the engine and automatic transmission, pressure in the wheel TPMS
Automatic lighting according to the temperature control of the engine 100C

 MCP2515 SPI MISO-D12
             MOSI-D11
             SDK -D13
             CS  -D8

 LCD1602 I2C SDA-A4
             SCL-A5            
 
  Written By:Eugene Belov 27 March 2024
*/

#include <SPI.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <mcp2515.h>
LiquidCrystal_I2C _lcd1(0x27, 16, 2);
int _dispTempLength1 = 0;
boolean _isNeedClearDisp1;
MCP2515 mcp2515_8(8);  // Set CS to pin 8
struct can_frame MCP2515_Read_1_CanMsg;
int _disp3oldLength = 0;
byte MCP2515_Read_Mess_4_out2 = 0;
byte MCP2515_Read_Mess_4_out3 = 0;
byte MCP2515_Read_Mess_4_out4 = 0;
byte MCP2515_Read_Mess_4_out5 = 0;
byte MCP2515_Read_Mess_1_out5 = 0;
int _disp2oldLength = 0;
int _disp4oldLength = 0;
byte MCP2515_Read_Mess_2_out4 = 0;
bool MCP2515_Read_1_HND = 0;
int _disp6oldLength = 0;
int _disp7oldLength = 0;
int _disp1oldLength = 0;

void setup() {
  pinMode(4, OUTPUT);  // Set Out active buzzer pin 4
  digitalWrite(4, 0);
  Wire.begin();
  delay(10);
  _lcd1.init();
  _lcd1.backlight();
  SPI.begin();
  mcp2515_8.reset();
  mcp2515_8.setBitrate(CAN_500KBPS, MCP_8MHZ);
  mcp2515_8.setConfigMode();
  mcp2515_8.setFilterMask(MCP2515::MASK0, false, 0x03FF);
  mcp2515_8.setFilter(MCP2515::RXF0, false, 0x380);
  mcp2515_8.setFilter(MCP2515::RXF1, false, 0x3F6);
  mcp2515_8.setFilterMask(MCP2515::MASK1, false, 0x03FF);
  mcp2515_8.setFilter(MCP2515::RXF2, false, 0x3E2);
  mcp2515_8.setFilter(MCP2515::RXF3, false, 0x3E2);
  mcp2515_8.setFilter(MCP2515::RXF4, false, 0x3E2);
  mcp2515_8.setFilter(MCP2515::RXF5, false, 0x3E2);
  mcp2515_8.setListenOnlyMode();
}
void loop() {
  if (_isNeedClearDisp1) {
    _lcd1.clear();
    _isNeedClearDisp1 = 0;
  }

  if (MCP2515_Read_1_HND) {
    MCP2515_Read_1_HND = 0;
  } else {
    if (mcp2515_8.readMessage(&MCP2515_Read_1_CanMsg) == MCP2515::ERROR_OK) {
      MCP2515_Read_1_HND = 1;
    }
  }
  if (MCP2515_Read_1_CanMsg.can_id == 1014) {
    MCP2515_Read_Mess_2_out4 = MCP2515_Read_1_CanMsg.data[4];
  }
  digitalWrite(4, ((MCP2515_Read_Mess_2_out4) - (40)) == (100));  // Set 100C Alarm buzzer for engine
  if (!(0)) {
    _dispTempLength1 = (((String((MCP2515_Read_Mess_2_out4) - (40), DEC)))).length();
    if (_disp7oldLength > _dispTempLength1) {
      _isNeedClearDisp1 = 1;
    }
    _disp7oldLength = _dispTempLength1;
    _lcd1.setCursor(1, 0);
    _lcd1.print(((String((MCP2515_Read_Mess_2_out4) - (40), DEC))));
  } else {
    if (_disp7oldLength > 0) {
      _isNeedClearDisp1 = 1;
      _disp7oldLength = 0;
    }
  }
  if (MCP2515_Read_1_CanMsg.can_id == 896) {
    MCP2515_Read_Mess_1_out5 = MCP2515_Read_1_CanMsg.data[5];
  }
  if (!(0)) {
    _dispTempLength1 = (((String((MCP2515_Read_Mess_1_out5) - (40), DEC)))).length();
    if (_disp6oldLength > _dispTempLength1) {
      _isNeedClearDisp1 = 1;
    }
    _disp6oldLength = _dispTempLength1;
    _lcd1.setCursor(1, 1);
    _lcd1.print(((String((MCP2515_Read_Mess_1_out5) - (40), DEC))));
  } else {
    if (_disp6oldLength > 0) {
      _isNeedClearDisp1 = 1;
      _disp6oldLength = 0;
    }
  }
  if (MCP2515_Read_1_CanMsg.can_id == 994) {
    MCP2515_Read_Mess_4_out2 = MCP2515_Read_1_CanMsg.data[2];
    MCP2515_Read_Mess_4_out3 = MCP2515_Read_1_CanMsg.data[3];
    MCP2515_Read_Mess_4_out4 = MCP2515_Read_1_CanMsg.data[4];
    MCP2515_Read_Mess_4_out5 = MCP2515_Read_1_CanMsg.data[5];
  }
  if (!(0)) {
    _dispTempLength1 = (((_floatToStringWitRaz((MCP2515_Read_Mess_4_out5) * (0.014), 2)))).length();
    if (_disp4oldLength > _dispTempLength1) {
      _isNeedClearDisp1 = 1;
    }
    _disp4oldLength = _dispTempLength1;
    _lcd1.setCursor(6, 1);
    _lcd1.print(((_floatToStringWitRaz((MCP2515_Read_Mess_4_out5) * (0.014), 2))));
  } else {
    if (_disp4oldLength > 0) {
      _isNeedClearDisp1 = 1;
      _disp4oldLength = 0;
    }
  }
  if (!(0)) {
    _dispTempLength1 = (((_floatToStringWitRaz((MCP2515_Read_Mess_4_out4) * (0.014), 2)))).length();
    if (_disp2oldLength > _dispTempLength1) {
      _isNeedClearDisp1 = 1;
    }
    _disp2oldLength = _dispTempLength1;
    _lcd1.setCursor(12, 1);
    _lcd1.print(((_floatToStringWitRaz((MCP2515_Read_Mess_4_out4) * (0.014), 2))));
  } else {
    if (_disp2oldLength > 0) {
      _isNeedClearDisp1 = 1;
      _disp2oldLength = 0;
    }
  }
  if (!(0)) {
    _dispTempLength1 = (((_floatToStringWitRaz((MCP2515_Read_Mess_4_out3) * (0.014), 2)))).length();
    if (_disp3oldLength > _dispTempLength1) {
      _isNeedClearDisp1 = 1;
    }
    _disp3oldLength = _dispTempLength1;
    _lcd1.setCursor(6, 0);
    _lcd1.print(((_floatToStringWitRaz((MCP2515_Read_Mess_4_out3) * (0.014), 2))));
  } else {
    if (_disp3oldLength > 0) {
      _isNeedClearDisp1 = 1;
      _disp3oldLength = 0;
    }
  }
  if (!(0)) {
    _dispTempLength1 = (((_floatToStringWitRaz((MCP2515_Read_Mess_4_out2) * (0.014), 2)))).length();
    if (_disp1oldLength > _dispTempLength1) {
      _isNeedClearDisp1 = 1;
    }
    _disp1oldLength = _dispTempLength1;
    _lcd1.setCursor(12, 0);
    _lcd1.print(((_floatToStringWitRaz((MCP2515_Read_Mess_4_out2) * (0.014), 2))));
  } else {
    if (_disp1oldLength > 0) {
      _isNeedClearDisp1 = 1;
      _disp1oldLength = 0;
    }
  }
}
String _floatToStringWitRaz(float value, int raz) {
  return String(value, raz);
}
