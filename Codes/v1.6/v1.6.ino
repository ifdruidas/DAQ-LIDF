//biblioteca
#define ONE_WIRE_BUS D2
#include <FS.h>
#include <HX711.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <WiFiClient.h>
HX711 scale;
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
WiFiClientSecure client;//Cria um cliente seguro (para ter acesso ao HTTPS)
ESP8266WebServer server(80);
const int potenciometro = A0; // pino de entrada do potenciômetro de deslocamento
String textFix = "GET /forms/d/e/1FAIpQLScSi5AcmySqlLp9rzk1-esIr6MARNsNPPyFiSs1iwm3TZUoaA/formResponse?ifq";
String LocalWifiSSID = "";
String LocalWifiPass = "";
String Valor[4];
String Resp[4];
String LastIRCode = "000000001";
int ButtonState = -1;
int SetupMode = 0;
int InSetupMode = 0;
int FilesystemOK = 1;
int valor = 0;
int band1;
int band2;
int porta_cal1 = D0;
int SetupOperationalMode = 1;
int i;
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
  if (SetupOperationalMode == 0) {
    SendHtml();
  }
}
