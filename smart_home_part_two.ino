
#include <Servo.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>

Servo servo_9;
Servo servo_10;
Servo servo_11;
volatile int Button_Red;
volatile int Button_Green;
volatile int Red_num_time;
String password;
LiquidCrystal_I2C mylcd(0x27,16,2);
volatile int water_level;
volatile int carch;
volatile int pass;
volatile int Environmental_sound;
volatile int Brightness;
volatile int Soil_moisture;
volatile int temperature;
volatile int humidity;
volatile int flame;
volatile int door_flag;
volatile int relay_close_led_flag;
volatile int relay_close_led_count;
volatile boolean key_door_flag;
volatile int last_soil;
volatile int soil_low_count;
DHT dht12(12, 11);

void open_door() {
  Button_Green = digitalRead(5);
  Button_Red = digitalRead(4);
  if (0 != Button_Green && 0 == Button_Red) {
    delay(100);
    Button_Green = digitalRead(5);
    while (0 != Button_Green && 0 == Button_Red) {
      Button_Red = digitalRead(4);
      Red_num_time = Red_num_time + 1;
      delay(100);
    }

  }
  if (1 < Red_num_time && 5 > Red_num_time) {
    key_voice();
    password = String(password) + String(".");
    mylcd.clear();
    mylcd.setCursor(1-1, 1-1);
    mylcd.print("ROLOF SMART HOME");
    mylcd.setCursor(1-1, 2-1);
    mylcd.print("Password:");
    mylcd.setCursor(11-1, 2-1);
    mylcd.print(password);

  }
  if (5 < Red_num_time) {
    key_voice();
    password = String(password) + String("-");
    mylcd.clear();
    mylcd.setCursor(1-1, 1-1);
    mylcd.print("ROLOF SMART HOME");
    mylcd.setCursor(1-1, 2-1);
    mylcd.print("Password:");
    mylcd.setCursor(11-1, 2-1);
    mylcd.print(password);

  }
  if (0 == Button_Green && 0 != Button_Red) {
    delay(100);
    Button_Red = digitalRead(4);
    if (0 == Button_Green && 0 != Button_Red) {
      if (false == key_door_flag) {
        if (password == ".--.") {
          mylcd.clear();
          mylcd.setCursor(1-1, 1-1);
          mylcd.print("ROLOF SMART HOME");
          mylcd.setCursor(1-1, 2-1);
          mylcd.print("Password:");
          mylcd.setCursor(11-1, 2-1);
          mylcd.print("Right");
          servo_9.write(180);
          delay(0);
          door_flag = 0;
          key_door_flag = true;

        } else {
          mylcd.clear();
          mylcd.setCursor(1-1, 1-1);
          mylcd.print("ROLOF SMART HOME");
          mylcd.setCursor(1-1, 2-1);
          mylcd.print("Password:");
          mylcd.setCursor(11-1, 2-1);
          mylcd.print("Error");
          tone(3,165);
          delay(125);
          delay(500);
          noTone(3);
          servo_9.write(0);
          delay(0);
          delay(200);
          mylcd.clear();
          mylcd.setCursor(1-1, 1-1);
          mylcd.print("ROLOF SMART HOME");
          mylcd.setCursor(11-1, 2-1);
          mylcd.print("Again ");
          mylcd.setCursor(1-1, 2-1);
          mylcd.print("Password:");
          key_voice();

        }

      } else if (true == key_door_flag) {
        key_door_flag = false;
        mylcd.clear();
        mylcd.setCursor(1-1, 1-1);
        mylcd.print("ROLOF SMART HOME");
        servo_9.write(90);
        delay(0);
      }
      password = "";

    }

  }
  Red_num_time = 0;
}

void Rain_Controlled_Window() {
  water_level = analogRead(A2);
  if (100 < water_level) {
    servo_10.write(90);
    delay(0);

  } else {
    servo_10.write(0);
    delay(0);

  }
}

void fan_motor_pwm(int speedpin, int speed)
{
  if (speed <= 0)
  {
    analogWrite(speedpin, 0);
  }
  else if (speed > 255)
  {
    analogWrite(speedpin, 255);
  }
  else
  {
    analogWrite(speedpin,speed);
  }
}

void Sound_Photosensitive_Control_LED() {
  Brightness = analogRead(A1);
  Environmental_sound = digitalRead(2);
  if (Brightness > 500) {
    if (Environmental_sound == 1) {
      fan_motor_pwm(6, 255);
      digitalWrite(7,HIGH);
      relay_close_led_flag = 1;

    } else {
      if (relay_close_led_flag == 1) {
        relay_close_led_count = relay_close_led_count + 1;
        if (relay_close_led_count >= 5000) {
          digitalWrite(6,LOW);
          digitalWrite(7,LOW);
          relay_close_led_count = 0;
          relay_close_led_flag = 0;

        }

      }

    }

  } else {
    digitalWrite(6,LOW);
    digitalWrite(7,LOW);

  }
}

void Coin_Parking() {
  pass = digitalRead(A0);
  if (pass == 0) {
    servo_11.write(180);
    delay(0);

  } else if (pass == 1) {
    servo_11.write(85);
    delay(0);
  }
}

void Plant_Watering_Warning() {
  Soil_moisture = analogRead(A3);
  if (50 > Soil_moisture) {
    if (50 < last_soil) {
      Watering_warning();

    } else if (12000 <= soil_low_count) {
      Watering_warning();
      soil_low_count = 0;
    } else {
      soil_low_count = soil_low_count + 1;

    }
    last_soil = Soil_moisture;

  } else {
    soil_low_count = 0;
    last_soil = Soil_moisture;

  }
}

void Falme__Temp_Fan() {
  temperature = dht12.readTemperature();
  humidity = dht12.readHumidity();
  flame = digitalRead(8);
  if (1 == flame || 31 < temperature) {
    digitalWrite(13,HIGH);
    tone(3,784);
    delay(125);

  } else {
    digitalWrite(13,LOW);
    noTone(3);

  }
}

void Watering_warning() {
  for (int i = 1; i <= 2; i = i + (1)) {
    digitalWrite(13,HIGH);
    tone(3,532);
    delay(125);
    delay(50);
    digitalWrite(13,LOW);
    noTone(3);
    delay(50);
  }
}

void key_voice() {
  tone(3,349);
  delay(125);
  delay(100);
  noTone(3);
  delay(100);
}

void setup(){
  pinMode(5, INPUT);
  pinMode(4, INPUT);
  servo_9.attach(9);
  pinMode(3, OUTPUT);
  pinMode(A2, INPUT);
  servo_10.attach(10);
  pinMode(A1, INPUT);
  pinMode(2, INPUT);
  pinMode(6, OUTPUT);
  digitalWrite(6, LOW);
  pinMode(7, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(A0, INPUT);
  servo_11.attach(11);
  Button_Red = 1;
  Button_Green = 1;
  Red_num_time = 0;
  password = "";
  mylcd.init();
  mylcd.backlight();
  water_level = 0;
  carch = 0;
  pass = 0;
  Environmental_sound = 0;
  Brightness = 0;
  Soil_moisture = 0;
  temperature = 0;
  humidity = 0;
  flame = 0;
  Serial.begin(9600);
  door_flag = 0;
  relay_close_led_flag = 0;
  relay_close_led_count = 0;
  key_door_flag = false;
  last_soil = 55;
  soil_low_count = 0;
  mylcd.setCursor(1-1, 1-1);
  mylcd.print("ROLOF SMART HOME");
  digitalWrite(7,LOW);
  servo_9.write(0);
  delay(0);
  servo_10.write(0);
  delay(0);
  servo_11.write(90);
  delay(0);
  pinMode(A3, INPUT);
   dht12.begin();
  pinMode(8, INPUT);
  pinMode(13, OUTPUT);
}

void loop(){
  while (true) {
    Sound_Photosensitive_Control_LED();
    Coin_Parking();
    Rain_Controlled_Window();
    open_door();
    Falme__Temp_Fan();
    Plant_Watering_Warning();
  }

}
