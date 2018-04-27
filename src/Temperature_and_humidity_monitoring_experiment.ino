/*
 * Author: SMRAZA KEEN
 * Date:2016/6/29
 * IDE V1.6.9
 * Email:TechnicSmraza@outlook.com
 * Function:
 */
#include <LiquidCrystal.h>
#include <dht11.h>    
#include <IRremote.h>

const int irReceiverPin = 7;
IRrecv irrecv(irReceiverPin);
decode_results results;              
dht11 DHT;                  //Note:DHT on behalf of the temperature and humidity sensor 
const int dht11_data = 6;     
int temp=0;
int hum=0;
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

//Celsius to Fahrenheit conversion
double Fahrenheit(double celsius)
{
  return 1.8 * celsius + 32;
}

// fast integer version with rounding
//int Celcius2Fahrenheit(int celcius)
//{
//  return (celsius * 18 + 5)/10 + 32;
//}


//Celsius to Kelvin conversion
double Kelvin(double celsius)
{
  return celsius + 273.15;
}

// dewPoint function NOAA
// reference (1) : http://wahiduddin.net/calc/density_algorithms.htm
// reference (2) : http://www.colorado.edu/geography/weather_station/Geog_site/about.htm
//
double dewPoint(double celsius, double humidity)
{
  // (1) Saturation Vapor Pressure = ESGG(T)
  double RATIO = 373.15 / (273.15 + celsius);
  double RHS = -7.90298 * (RATIO - 1);
  RHS += 5.02808 * log10(RATIO);
  RHS += -1.3816e-7 * (pow(10, (11.344 * (1 - 1/RATIO ))) - 1) ;
  RHS += 8.1328e-3 * (pow(10, (-3.49149 * (RATIO - 1))) - 1) ;
  RHS += log10(1013.246);

        // factor -3 is to adjust units - Vapor Pressure SVP * humidity
  double VP = pow(10, RHS - 3) * humidity;

        // (2) DEWPOINT = F(Vapor Pressure)
  double T = log(VP/0.61078);   // temp var
  return (241.88 * T) / (17.558 - T);
}

// delta max = 0.6544 wrt dewPoint()
// 6.9 x faster than dewPoint()
// reference: http://en.wikipedia.org/wiki/Dew_point
double dewPointFast(double celsius, double humidity)
{
  double a = 17.271;
  double b = 237.7;
  double temp = (a * celsius) / (b + celsius) + log(humidity*0.01);
  double Td = (b * temp) / (a - temp);
  return Td;
}


void setup()                                                                                     
{
  Serial.begin(9600);
  irrecv.enableIRIn();
  lcd.begin(16,2);
  lcd.print("Welcome to mini proj by");
  lcd.setCursor(0,1);
  lcd.print(" Tran Long Chau");
  delay(2000);
  lcd.clear();
}
void loop()
{ 
  if (irrecv.decode(&results))
  {
  Serial.print("IR_Code: ");
  Serial.print(results.value, HEX);
  Serial.print(", Bits: ");
  Serial.println(results.bits);
  irrecv.resume();
  }
  if (Serial.available() > 0) {
        // read the incoming byte:
        incomingByte = Serial.read();

        // say what you got:
        Serial.print("I received: ");
        Serial.println(incomingByte, DEC);
}
  DHT.read(dht11_data);
  temp=DHT.temperature;
  hum=DHT.humidity;
  lcd.clear();
  lcd.print(incomingByte); 
  delay(1000);
  lcd.clear();                   //clear display
  lcd.print("Hum=%");            //display "Hum=%"
  lcd.print(hum);
  lcd.setCursor(10,0) ;
  lcd.print("ihsara");         //display "Smraza"
  lcd.setCursor(0,1) ;           //Display position
  lcd.print("Temp=");            //display"Temp="
  lcd.print(temp);
  lcd.write(0xDF);              //Display custom characters '°'
  lcd.print("C");
  delay(500);                   //After 500ms ,the screen will be refreshed

  Serial.print("Humidity (%): ");
  Serial.println((float)DHT11.humidity, 2);

  Serial.print("Temperature (°C): ");
  Serial.println((float)DHT11.temperature, 2);

  Serial.print("Temperature (°F): ");
  Serial.println(Fahrenheit(DHT11.temperature), 2);

  Serial.print("Temperature (°K): ");
  Serial.println(Kelvin(DHT11.temperature), 2);

  Serial.print("Dew Point (°C): ");
  Serial.println(dewPoint(DHT11.temperature, DHT11.humidity));

  Serial.print("Dew PointFast (°C): ");
  Serial.println(dewPointFast(DHT11.temperature, DHT11.humidity));

  delay(2000);
}

