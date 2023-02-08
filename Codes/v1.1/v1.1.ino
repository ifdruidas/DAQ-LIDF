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
  if (client.connect("docs.google.com", 443) == 1)//Tenta se conectar ao servidor do Google docs na porta 443 (HTTPS)
  {
    String toSend = textFix;//Atribuimos a String auxiliar na nova String que sera enviada
    toSend += "&entry.601296763=";//resposta 1
    scale.set_gain(128);
    band1=scale.read();
    Serial.print(band1);
    toSend += band1;//balanca
    toSend += "&entry.93145873=";//resposta 2
    scale.set_gain(32);
    band2=scale.read();
    Serial.print(band2);
    toSend += band2;//Adicionamos um valor aleatorio
    toSend += "&entry.1080741398=";//resposta 3
    toSend += sensors.getTempCByIndex(0);//Adicionamos um valor aleatorio
    toSend += "&entry.649379059=";//resposta 4
    toSend += analogRead(potenciometro);//Adicionamos um valor aleatorio
    toSend += "&submit=Submit HTTP/1.1";//Completamos o metodo GET para nosso formulario.
    client.println(toSend);//Enviamos o GET ao servidor-
    client.println("Host: docs.google.com");//-
    client.println();//-
    client.stop();//Encerramos a conexao com o servidor
    Serial.println("Dados enviados.");//Mostra no monitor que foi enviado
  }
  else {
    Serial.println("Erro ao se conectar");//Se nao for possivel conectar no servidor, ira avisar no monitor.
  }
}
