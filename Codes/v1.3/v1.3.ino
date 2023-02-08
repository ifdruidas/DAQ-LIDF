//biblioteca
#include <FS.h>
//sensores
#include <HX711.h>
#include <OneWire.h>
#include <DallasTemperature.h>
HX711 scale;
#define ONE_WIRE_BUS D2
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
const int potenciometro = A0; // pino de entrada do potenciômetro de deslocamento
//internet
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <WiFiClient.h>
WiFiClientSecure client;//Cria um cliente seguro (para ter acesso ao HTTPS)
ESP8266WebServer server(80);
String textFix = "GET /forms/d/e/1FAIpQLScSi5AcmySqlLp9rzk1-esIr6MARNsNPPyFiSs1iwm3TZUoaA/formResponse?ifq";
//bandeiras
int ButtonState = -1;
int SetupMode = 0;
int InSetupMode = 0;
int FilesystemOK = 1;
int valor = 0;
int band1;
int band2;
int porta_cal1 = D0;
int OperationalMode = 0;
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
  if (SetupMode == 0) {
    WaitForSetupMode();
    SetupMode = 1;
  }
  if (OperationalMode == 1) {
    SendHtml();
  }
}
