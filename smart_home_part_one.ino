
#include <Servo.h>
#include <DHT.h>
#include <LiquidCrystal.h>

Servo servo_11;
Servo servo_10;
DHT dht5(5, 11);
volatile int Button_Red;
volatile int Button_Green;
volatile int Red_num_time;
String password;
LiquidCrystal mylcd(13,12,0,1,2,3);
volatile int water_level;
volatile int carch;
volatile int pass;
volatile int gas;
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

void open_door() {
  Button_Green = digitalRead(8);
  Button_Red = digitalRead(4);
  if (0 != Button_Green && 0 == Button_Red) {
    delay(100);
    Button_Green = digitalRead(8);
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
          servo_11.write(180);
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
          tone(9,165);
          delay(125);
          delay(500);
          noTone(9);
          servo_11.write(0);
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
        servo_11.write(90);
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
  if (Brightness > 500) {
    fan_motor_pwm(6, 255);
    digitalWrite(7,HIGH);

  } else {
    digitalWrite(6,LOW);
    digitalWrite(7,LOW);

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
  temperature = dht5.readTemperature();
  humidity = dht5.readHumidity();
  flame = digitalRead(A4);
  if (500 > flame || 31 < temperature) {
    digitalWrite(7,HIGH);
    tone(9,784);
    delay(125);

  } else {
    digitalWrite(7,LOW);
    noTone(9);

  }
}

void Watering_warning() {
  for (int i = 1; i <= 2; i = i + (1)) {
    digitalWrite(7,HIGH);
    tone(9,532);
    delay(125);
    delay(50);
    digitalWrite(7,LOW);
    noTone(9);
    delay(50);
  }
}

void gasDetector() {
  gas = analogRead(A5);
  if (0 == gas) {
    digitalWrite(7,HIGH);
    tone(9,294);
    delay(125);

  } else {
    digitalWrite(7,LOW);
    noTone(9);

  }
}

void key_voice() {
  tone(9,165);
  delay(125);
  delay(100);
  noTone(9);
  delay(100);
}

void setup(){
  pinMode(8, INPUT);
  pinMode(4, INPUT);
  servo_11.attach(11);
  pinMode(9, OUTPUT);
  pinMode(A2, INPUT);
  servo_10.attach(10);
  pinMode(A1, INPUT);
  pinMode(6, OUTPUT);
  digitalWrite(6, LOW);
  pinMode(7, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(A3, INPUT);
   dht5.begin();
  pinMode(A4, INPUT);
  Button_Red = 1;
  Button_Green = 1;
  Red_num_time = 0;
  password = "";
  mylcd.begin(16,2);
  water_level = 0;
  carch = 0;
  pass = 0;
  gas = 0;
  Brightness = 0;
  Soil_moisture = 0;
  temperature = 0;
  humidity = 0;
  flame = 0;
  door_flag = 0;
  relay_close_led_flag = 0;
  relay_close_led_count = 0;
  key_door_flag = false;
  last_soil = 55;
  soil_low_count = 0;
  digitalWrite(7,LOW);
  noTone(9);
  servo_10.write(0);
  delay(0);
  servo_11.write(0);
  delay(0);
}

void loop(){
  while (true) {
    mylcd.setCursor(1-1, 0-1);
    mylcd.print("REYMOND");
    mylcd.setCursor(1-1, 1-1);
    mylcd.print("SMART HOUSE");
    Sound_Photosensitive_Control_LED();
    Rain_Controlled_Window();
    open_door();
    Falme__Temp_Fan();
    gasDetector();
    Plant_Watering_Warning();
  }

}
