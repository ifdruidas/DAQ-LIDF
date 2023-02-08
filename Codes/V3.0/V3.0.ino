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
String Valor[6];
String Resp[6];
String NewLastbalance = "000000";
String NewLastdistance = "000000";
String NewLastgauge = "000000";
int ButtonState = -1;
int SetupMode = 0;
int InSetupMode = 0;
int FilesystemOK = 1;
float valor;
float band1;
float band2;
float Peso;
float Distancia;
float Forca;
int porta_cal1 = D0;
int SetupOperationalMode = 1;
int i;
void setup() {
  Serial.begin(115200);
  sensors.begin();
  pinMode(porta_cal1, OUTPUT);
  pinMode(2, OUTPUT);
  pinMode(0, INPUT);
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
    //peso
    scale.set_gain(128);
    band1 = scale.read();
    Serial.println(band1/1000);
    NewLastbalance = String(band1/1000);
    WriteFile("Lastbalance", NewLastbalance);
    scale.set_gain(32);
    band2 = scale.read();
    Serial.println(band2/100);
    NewLastgauge = String(band2/100);
    WriteFile("Lastgauge", NewLastgauge);
    //distancia
    NewLastdistance = String(analogRead(A0));
    WriteFile("Lastdistance", NewLastdistance);

  }
  if (SetupOperationalMode == 0) {
    SendHtml();
  }
}
