#include "LCD_Driver.h"
#include "Touch_Driver.h"
#include "GUI_Paint.h"
#include "image.h"
#include "HX711.h"
//#include <Bounce2.h>

HX711 scale; // For weight
UBYTE flag = 0;
const int pumpRelayPin = 2;
bool relayState = LOW;
uint8_t dataPin  = 6;//for esp32
uint8_t clockPin = 5;//for esp32
//Bounce debouncer = Bounce();

void setup() {
  //Setting up DISPLAY
  Touch_1IN28_XY XY;
  XY.mode = 0;
  XY.x_point = 0;
  XY.y_point = 0;
  Config_Init();
  LCD_Init();
  attachInterrupt(1,Touch_INT_callback,LOW);

  if (Touch_1IN28_init(XY.mode) == true)
    Serial.println("Touch screen initialized.");
  else
    Serial.println("Touch screen initialization failed.");
     
  LCD_SetBacklight(1000);
  Paint_NewImage(LCD_WIDTH, LCD_HEIGHT, 0, BLACK);
  Paint_Clear(BLACK);
  pinMode(TP_INT_PIN, INPUT_PULLUP);

  Paint_DrawString_EN(45, 50, "Smart Canister", &Font16, BLACK, WHITE);
  Paint_DrawString_EN(35, 90, "Start - Tap", &Font16, BLACK, WHITE);
  Paint_DrawCircle( 20, 95, 5, GREEN, 1, 1);
   Paint_DrawString_EN(35, 110, "Pause - Double Tap", &Font16, BLACK, WHITE);
  Paint_DrawCircle( 20, 115, 5, RED, 1, 1);
    Paint_DrawString_EN(35, 150, "Weight: 0.00 g", &Font16, BLACK, WHITE);

  DEV_Delay_ms(500);

  pinMode(pumpRelayPin, OUTPUT);
  digitalWrite(pumpRelayPin, relayState);

 // debouncer.attach(TP_INT_PIN);
 // debouncer.interval(50);
  // Serial.begin(9600);

  //Measuring WEIGHT
  Serial.begin(115200);
  Serial.println(__FILE__);
  Serial.print("LIBRARY VERSION: ");
  Serial.println(HX711_LIB_VERSION);
  Serial.println();

  scale.begin(dataPin, clockPin);

//   Serial.print("UNITS: ");
//   Serial.println(scale.get_units(10));

//   Serial.println("\nEmpty the scale, press a key to continue");
//   while(!Serial.available());
//   while(Serial.available()) Serial.read();

//   scale.tare();
//   Serial.print("UNITS: ");
//   Serial.println(scale.get_units(10));


//   Serial.println("\nPut 1000 gram in the scale, press a key to continue");
//   while(!Serial.available());
//   while(Serial.available()) Serial.read();

//   scale.calibrate_scale(1000, 5);
//   Serial.print("UNITS: ");
//   Serial.println(scale.get_units(10));
// Serial.println(scale.get_units(" g"));


//   Serial.println("\nScale is calibrated, press a key to continue");
//   // Serial.println(scale.get_scale());
//   // Serial.println(scale.get_offset());
//   while(!Serial.available());
//   while(Serial.available()) Serial.read();

}

void loop() {
  //Serial.println(flag);
  //delay (100);
  //debouncer.update();
  relayState = !relayState;
  digitalWrite(pumpRelayPin, relayState);

  //Read Weights
  // Serial.print("UNITS: ");
  // Serial.print(scale.get_value());
  // Serial.println(" g");
  // delay(250);
    

  if (flag == TOUCH_IRQ) {
    XY = Touch_1IN28_Get_Point();
    TurnOnRelay();
    flag = TOUCH_DRAW;
  }

  if (flag == TOUCH_IRQ && XY.Gesture == DOUBLE_CLICK) {
    XY = Touch_1IN28_Get_Point();
    TurnOnRelay();
    flag = TOUCH_DRAW;
  }

  // // Check for a single click to toggle the pump
  // if (XY.Gesture == CLICK) {
  //   // Toggle the state of the relay and the pump
  //   relayState = !relayState;
  //   digitalWrite(pumpRelayPin, relayState);
  //   Serial.println("Pump activated");

  // }

  // Reset the flag
  flag = 0;
}
void TurnOnRelay()
{
  relayState = !relayState;
  digitalWrite(pumpRelayPin, relayState);
  delay(86400);
  Serial.println("relayCalled");
  relayState = !relayState;
  digitalWrite(pumpRelayPin, relayState);
}

void Touch_INT_callback() {
  flag = TOUCH_IRQ;
}