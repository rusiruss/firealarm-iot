//project : Fire alram System With IOT 
// Date :25/04/2021
// Rusiru Shashimal Madhuwantha

//Firstly, libraries are included.
#include <Wire.h> 


#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <CTBot.h>


//the LED, sensor, and buzzer pins are defined.
/*-----------------------------DHT------------------------*/
int dhtPin= D4;
int dhtType = DHT11;
DHT dht(dhtPin, dhtType);

/*--------------------------------------------------------*/
int MQ2=A0;
int SensorThres =100;

int buzzer = 16;

int RedLed = D5;
int BlueLed = D7;

//Secondly, we need to enter WIFI connection details and Telegram bot details. For that,we  use WIFI connection details and Telegram bot details.
//Enter WIFI connection details.

String ssid  = "add your Router SSH"    ; 
String password  = "Add your Router password"; 

//Enter  Telegram bot token as we obtained earlier.

#define BOTtoken "Add your telegram chat bot token"

//Enter your Telegram chat ID as we obtained earlier.

#define CHAT_ID "add your telegram chat bot id"

CTBot myBot;
TBMessage msg;

// creates objects for the I2C library and telegram library.

LiquidCrystal_I2C  lcd(0x3F ,20,4);
X509List cert(TELEGRAM_CERTIFICATE_ROOT);
WiFiClientSecure client;
UniversalTelegramBot bot(BOTtoken, client);

void setup(){
  //setup function, the serial monitor, LCD, wire library, and WIFI library are started
  Serial.begin(115200);
  lcd.begin(20,4);
  lcd.init();
  lcd.backlight();
  Wire.begin(D2, D1);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  myBot.setTelegramToken(BOTtoken);
   
  
  //configuration time and root certificate for api.telegram.org are included. Also, the LEDs, buzzer and sensor PIN are set as output.
  
  configTime(0, 0, "pool.ntp.org");   
  client.setTrustAnchors(&cert); 
  
  dht.begin();
  pinMode(MQ2,INPUT);
  pinMode(buzzer,OUTPUT);
  pinMode(RedLed,OUTPUT);
  pinMode(BlueLed,OUTPUT);
  pinMode(buzzer, OUTPUT);

  
  
 //Then, the Nodemcu board connects to the Internet, and “WIFI Connected” and the local IP address are displayed on the LCD.
  int a = 0;
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    lcd.setCursor(a, 0);
    lcd.print(".");
    delay(500);
    a++;
  }
  Serial.println("");
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("WiFi connected");
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  lcd.setCursor(0, 1);
  lcd.print(WiFi.localIP());
  Serial.println(WiFi.localIP());
  delay(500);
  
 //this code sends a message to indicate that the bot has started. Also displayed as “System startup” in the Telegram app and LCD.
  bot.sendMessage(CHAT_ID, "System started", "");
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("System startup");
  delay(1000);
  lcd.clear();
}
/*-----------------------------------LOOP-------------------------------------------------*/
void loop(){ 
  int analogSensor = analogRead(MQ2);   
  lcd.setCursor(1, 0);
  lcd.print("Fire  Alarm System");
  
  if(analogSensor>SensorThres ){
  lcd.setCursor(0, 1);
  lcd.print("DANGEROUS SITUATION");
  digitalWrite(RedLed, HIGH);
  digitalWrite(BlueLed, LOW);
  tone(buzzer, 1000); // Send 1KHz sound signal...
  bot.sendMessage(CHAT_ID, "GAS LEAK OR SMOKE Detected!!", "");
  lcd.clear();
  
 }
 else{
  lcd.setCursor(0, 1);
  lcd.print("NO GAS LEAK OR SMOKE");
  digitalWrite(BlueLed, HIGH);
  digitalWrite(RedLed, LOW);
  noTone(buzzer);     // Stop sound...
  delay(1000);
 }

 float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  float f = dht.readTemperature(true);
  lcd.setCursor(0, 2);
  lcd.print("Humidity:");
  lcd.setCursor(9, 2);
  lcd.print(h);
  lcd.setCursor(0, 3);
  lcd.print("Temp:");
  lcd.setCursor(5, 3);
  lcd.print(t);
  delay(1000);


  String humid = "Humidity: " + String(h) + " % \n";
  String temp = "Temperature: " + String(t) + "°C, " + String(f) + "°F \n";

  if (myBot.getNewMessage(msg)) {

    if (msg.text.equalsIgnoreCase("Humidity")) {    
     myBot.sendMessage(msg.sender.id, humid);
    }
  if (msg.text.equalsIgnoreCase("Temperature")) {    
      myBot.sendMessage(msg.sender.id, temp); 
    }

  }
}
