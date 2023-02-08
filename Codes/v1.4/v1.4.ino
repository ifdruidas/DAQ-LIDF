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
// dados de wifi
String LocalWifiSSID = "";
String LocalWifiPass = "";
//Nome do sensor
String SensorName1;
String SensorName2;
String SensorName3;
String SensorName4;
//dados em IR
String IRVal1;
String IRVal2;
String IRVal3;
String IRVal4;
String LastIRCode = "000000001";
//bandeiras
int ButtonState = -1;
int SetupMode = 0;
int InSetupMode = 0;
int FilesystemOK = 1;
int valor = 0;
int band1;
int band2;
int porta_cal1 = D0;
int SetupOperationalMode = 1;
int OperationalMode = 0;
void setup() {
  Serial.begin(115200);
  sensors.begin();
  pinMode(porta_cal1, OUTPUT);
  digitalWrite(porta_cal1, LOW);
  scale.begin(D5, D6);
}
void loop() {
  if (SetupMode == 0) {
    WaitForSetupMode();
    SetupMode = 1;
  }
   if (SetupOperationalMode == 1 && InSetupMode == 0 ) {
    StartOperationalMode();
  }
    if (InSetupMode == 1)
  {
    server.handleClient();
  }
  if (OperationalMode == 1) {
    SendHtml();
  }
}
