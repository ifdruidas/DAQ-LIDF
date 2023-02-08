#include <HX711.h>
#include <OneWire.h>
#include <DallasTemperature.h>
const int potenciometro = A0; // pino de entrada do potenciômetro de deslocamento
int valor = 0;
int band1;
int band2;
int porta_cal1 = D0;
HX711 scale;
#define ONE_WIRE_BUS D2
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
#include <ESP8266WiFi.h>
WiFiClientSecure client;//Cria um cliente seguro (para ter acesso ao HTTPS)
String textFix = "GET /forms/d/e/1FAIpQLScSi5AcmySqlLp9rzk1-esIr6MARNsNPPyFiSs1iwm3TZUoaA/formResponse?ifq";

void setup() {
  Serial.begin(9600);
  sensors.begin();
  WiFi.mode(WIFI_STA);//Habilita o modo estaçao
  WiFi.begin("Wano 2.4G", "Gdasdea1998");//Conecta na rede
  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println();

  Serial.print("Connected, IP address: ");
  Serial.println(WiFi.localIP());
  pinMode(porta_cal1, OUTPUT);
  digitalWrite(porta_cal1, LOW);
  scale.begin(D5, D6);
}
void loop() {
  SendHtml();
}
