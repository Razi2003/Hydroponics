#include <WiFi.h>
#include <HTTPClient.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>

#define network "HACKATHON"
#define password "H@ck!2022t0n"
#define timeout 20000
#define sensorPower 35
#define sensorPin 34


LiquidCrystal_I2C lcd(0x27, 0, 4);
DHT dht(27, DHT11);

void connectWifi(){

  Serial.print("Connecting to Wifi");
  WiFi.mode(WIFI_STA);
  WiFi.begin(network, password);

  unsigned long startAttempt = millis();

  while(WiFi.status() != WL_CONNECTED && millis() - startAttempt < timeout){
    Serial.print(".");
    delay(100);
  }

  if(WiFi.status() != WL_CONNECTED){

    Serial.println("Failed.."); 
  }
  else{
    Serial.print("Connected:");
    Serial.println(WiFi.localIP());
  }
}

/* ENDED WIFI CONNECTION */

int button1 = 4, button2 = 15, button1_value, button2_value, button1_old = 0, button2_old = 0;
int water_pin = 25, wv, w_map, relay = 18;
int rice_time = 0, cotton_time = 0, barley_time = 0, t = 0;
float temperature, humidity;
char buffer[40], rice_buf[40], cotton_buf[40], barley_buf[40];

enum modes {
  RICE,
  COTTON,
  BARLEY,
  None 
};

modes st_now, st_main;

void rice_mode(){

  // initialise
  if (st_now != st_main){

    st_main = st_now;
    
    lcd.clear();
    delay(50);
    lcd.setCursor(0,0);
    lcd.print("Rice activated");
    delay(1500);
    
  }
  

  // apply state loop

  t = millis();

  if (t - rice_time >= 1500){

    sprintf(rice_buf, "Rice:%.1f,%.f", temperature, humidity);
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print(rice_buf);
   
    rice_time = t;
    Serial.println(rice_time);
  }  

    //  digitalWrite(sensorPower, HIGH);  // Turn the sensor ON
//  delay(10);              // Allow power to settle
  int val = analogRead(sensorPin);  // Read the analog value form sensor
//  digitalWrite(sensorPower, LOW);   // Turn the sensor OFF
  

  if (val > 3900){

    digitalWrite(relay, HIGH);
    delay(1000);
    digitalWrite(relay,LOW);
  }
  else{
    digitalWrite(relay, LOW);
  }


  // check exit state
  if (button1_value == 1 && button1_old == 0){ // This place will be rework

    st_now = COTTON;

    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Mode changing..");
    delay(1000);
    }

  // clean stuff
  if(st_now != st_main){ // This place will be considered
    
    // motor off
    digitalWrite(relay, LOW);

    lcd.clear();
  }
  
}

void cotton_mode(){

  // initialise
  if (st_now != st_main){

    st_main = st_now;
    
    lcd.clear();
    delay(50);
    lcd.setCursor(0,0);
    lcd.print("Cotton activated");
    delay(1500);
  }

  // apply state loop
  t = millis();

  if (t - cotton_time >= 1500){

    sprintf(cotton_buf, "Cotton:%.1f,%.f", temperature, humidity);
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print(cotton_buf);
   
    cotton_time = t;
    Serial.println(cotton_time);
  }  



  // check exit state
  if (button1_value == 1 && button1_old == 0){ // This place will be rework

    st_now = BARLEY;

    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Mode changing..");
    delay(1000);
    }

  // check previous state
  if (button2_value == 1 && button2_old == 0){ // This place will be rework

    st_now = RICE;

    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Mode changing..");
    delay(1000);
    }

  // clean stuff
  if (st_now != st_main){

    lcd.clear();
  }
}


void barley_mode(){

  // initialise
  if (st_now != st_main){

    st_main = st_now;
    
    lcd.clear();
    delay(50);
    lcd.setCursor(0,0);
    lcd.print("Barley activated");
    delay(1500);
  }

  // apply state loop
  t = millis();

  if (t - barley_time >= 1500){

    sprintf(barley_buf, "Barley:%.1f,%.f", temperature, humidity);
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print(barley_buf);
   
    barley_time = t;
    Serial.println(barley_time);
  }  


  // check previous state
  if (button2_value == 1 && button2_old == 0){ // This place will be rework

    st_now = COTTON;

    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Mode changing..");
    delay(1000);
    }

  // clean stuff
  if (st_now != st_main){

    lcd.clear();
  }
}

void setup() {

  Serial.begin(9600);
  connectWifi();

  lcd.init();
  lcd.backlight();

  pinMode(button1, INPUT);
  pinMode(button2, INPUT);
  pinMode(relay, OUTPUT);
  pinMode(sensorPower, OUTPUT);
  digitalWrite(relay, LOW);
  

  dht.begin();

  st_now = RICE;
  st_main = None;

}

void loop() {

  button1_value = digitalRead(button1);
  button2_value = digitalRead(button2);
//
//  wv = analogRead(water_pin);
//  w_map = map(wv, 0, 4095, 0, 100);

  temperature = dht.readTemperature();
  humidity = dht.readHumidity();

  switch(st_now){

    case RICE:
      rice_mode();
      break;

    case COTTON:
      cotton_mode();
      break;

    case BARLEY:
      barley_mode();
      break;
}
//
//  sprintf(buffer, "%d, %d", button1_value, button2_value);
//  Serial.println(buffer);
button1_old = button1_value;
button2_old = button2_value;



}
