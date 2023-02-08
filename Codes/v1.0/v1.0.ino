// e-structure UNO V1.0 - Licença Creative Commons - Atribuição-NãoComercial 4.0 Internacional.
#include <HX711.h>
#include <OneWire.h>
#include <DallasTemperature.h>
const int potenciometro = A0; // pino de entrada do potenciômetro de deslocamento
int valor =0;
int porta_cal1 = D7;
HX711 scale; 

#define ONE_WIRE_BUS D8
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

void setup() {
  Serial.begin(9600);
  sensors.begin();
  pinMode(porta_cal1, OUTPUT); 
  digitalWrite(porta_cal1, LOW);
  scale.begin(D5, D6);
  }
void loop() {
  sensors.requestTemperatures();
  Serial.print("10X,"); // não alterar 
  scale.set_gain(128); 
  Serial.print(scale.read()); 
  Serial.print("A,");// não alterar 
  scale.set_gain(32); 
  Serial.print(scale.read()); 
  Serial.print("B,");// não alterar 
  Serial.print(sensors.getTempCByIndex(0));
  Serial.print("C,");// não alterar
  valor = analogRead(potenciometro);
  Serial.print(valor);
  Serial.println("D,");// não alterar  
       {
 if(Serial.available())
  {
    char numero = Serial.read();
    if (numero == 'L')
  {
  digitalWrite(porta_cal1, HIGH); 
  }
  else
      if(numero == 'D') 
    {
  digitalWrite(porta_cal1, LOW); 
    }        
}
   }
      }
