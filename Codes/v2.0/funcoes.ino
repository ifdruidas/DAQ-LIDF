//inicio do modo de configuração
void WaitForSetupMode() {
  for (int i = 0; i <= 300; i++) {
    ButtonState =  digitalRead(0);
    if (ButtonState == 0) {
      StartSetupMode();
      break;
    }
    delay(10);
  }
}

void StartSetupMode() {
  const char* ssid = "Node_EspAlexa";
  const char* password = "";
  //idicar led para mostra que esta em modo de configurações
  for (int i = 0; i <= 10; i++) {
    digitalWrite(2, LOW);
    delay(50);
    digitalWrite(2, HIGH);
    delay(50);
  }
  digitalWrite(2, LOW);
  Serial.println();
  Serial.print("Conectado a ");
  Serial.println(ssid);
  WiFi.softAP(ssid, password);
  IPAddress myIP = WiFi.softAPIP();
  Serial.println("");
  Serial.println("Iniciando modo de configurações...");
  Serial.println("Wifi iniciado com sucesso");
  FlashLEDSlow();
  Serial.println("Numero de IP: " + myIP);
  //Indica que o node está no modo de configuração para lidar com as solicitações da web.
  InSetupMode = 1;
  //Iniciando Arquivos
  SPIFFS.begin();
  delay(1000);
  //verficação de arquivos essenciais
  Serial.println("Checking if essensial GUI files exist....");
  CheckGui("index.html.gz");
  CheckGui("setup.html.gz");
  CheckGui("savesetup.html.gz");
  CheckGui("bootstrap.min.css.gz");
  Serial.println("Listando Arquivos....");
  String str = "";
  Dir dir = SPIFFS.openDir("/");
  while (dir.next()) {
    str += dir.fileName();
    str += "/";
    str += dir.fileSize();
    str += "\r\n";
  }
  Serial.println(str);
  Serial.println("Arquivos Listados....");
  if (FilesystemOK == 0) {
    Serial.println("Sistema de arquivos quebrados: Falha na inicialização!");
  } else {
    Serial.println("Arquivos de sistemas carregados");
  }//Iniciando servidor Web
  server.begin();
  server.on("/", fileindex);
  server.on("/index.html", fileindex);
  server.on("/setup.html", filesetup);
  server.on("/savesetup.html", savesetup);
  server.on("/reboot.html", reboot);
  server.on("/bootstrap.min.css", bootstrap);
  server.on("/bootstrap.min.css", bootstrap);
  server.on("/fSensorName1.conf", fSensorName1);
  server.on("/fSensorName2.conf", fSensorName2);
  server.on("/fSensorName3.conf", fSensorName3);
  server.on("/fSensorName4.conf", fSensorName4);
  server.on("/fSensorIR1.conf", fSensorIR1);
  server.on("/fSensorIR2.conf", fSensorIR2);
  server.on("/fSensorIR3.conf", fSensorIR3);
  server.on("/fSensorIR4.conf", fSensorIR4);
  server.on("/Lastbalance", Lastbalance);
  server.on("/Lastdistance", Lastdistance);
  server.on("/fSSID.conf", fSSID);
  server.on("/fPassword.conf", fPassword);
}

void CheckGui(String FileNameToRead) {
  FileNameToRead.trim();
  File FileObjR = SPIFFS.open("/" + FileNameToRead , "r");
  if (!FileObjR) {
    Serial.println("FS Panic: Nenhuma GUI encontrada. Você DEVE fazer o upload da GUI, caso contrário, este dispositivo não funcionará!");
    Serial.print(FileNameToRead); Serial.println("não encontrado, certifique-se de enviá-lo!");
    FilesystemOK = 0;
  } else {
    Serial.print(FileNameToRead);
    Serial.println(" OK");
  }
  FileObjR.close();
}

void fileindex() {
  if (FilesystemOK == 0) {
    server.send(200, "text/html", "<h1><b>No GUI found!</b></h1><h3> Please use the ESP tool to upload the GUI.<br></h3>");
  } else {
    Serial.println("carregando index.html");
    File file = SPIFFS.open("/index.html.gz", "r");
    size_t sent = server.streamFile(file, "text/html");
  }
}

void filesetup() {
  Serial.println("carregando setup.html");
  File file = SPIFFS.open("/setup.html.gz", "r");
  size_t sent = server.streamFile(file, "text/html");
}

void savesetup() {
  File file = SPIFFS.open("/savesetup.html.gz", "r");
  size_t sent = server.streamFile(file, "text/html");
  //Get data from incomming POST request
  String Valor[4];
  String Resp[4];
  String fSSID;
  String fPassword;
  if (server.hasArg("fSensorName1")) {
    Serial.println("Salvando as configurações...");
    //Device Names
    for (i = 0; i <= 3; i++) {
      Valor[i] = server.arg("fSensorName" + (String)(i + 1)); Serial.println("Sensor Name" + (String)(i + 1) + ": " + Valor[i]);
    }
    //Device IR Values
    for (i = 0; i <= 3; i++) {
      Resp[i] = server.arg("fSensorIR" + (String)(i + 1)); Serial.println("Sensor Val IR" + (String)(i + 1) + ": " + Resp[i]);
    }
    //WIFi
    fSSID = server.arg("fSSID"); Serial.println("AP SSID: " + fSSID);
    fPassword = server.arg("fPassword"); Serial.println("AP Password: " + fPassword);
    //escrevendo as configuracoes nos arquvos
    //nome dos dipositivo
    for (i = 0; i <= 3; i++) {
      WriteFile("fSensorName" + (String)(i + 1) + ".conf", Valor[i]);
    }
    //IR Values
    for (i = 0; i <= 3; i++) {
      WriteFile("fSensorIR" + (String)(i + 1) + ".conf", Resp[i]);
    }
    //WIFI
    WriteFile("fSSID.conf", fSSID);
    WriteFile("fPassword.conf", fPassword);
    Serial.println("Dados salvo");
  }
}
void WriteFile(String FileName, String DataToWrite) {
  File FileObjW = SPIFFS.open("/" + FileName, "w");
  DataToWrite.trim();
  FileObjW.print(DataToWrite);
  FileObjW.close();
}
void bootstrap() {
  File file = SPIFFS.open("/bootstrap.min.css.gz", "r");
  size_t sent = server.streamFile(file, "text/css");
}
void fSensorName1() {
  File file = SPIFFS.open("/fSensorName1.conf", "r");
  size_t sent = server.streamFile(file, "text/html");
}
void fSensorName2() {
  File file = SPIFFS.open("/fSensorName2.conf", "r");
  size_t sent = server.streamFile(file, "text/html");
}
void fSensorName3() {
  File file = SPIFFS.open("/fSensorName3.conf", "r");
  size_t sent = server.streamFile(file, "text/html");
}
void fSensorName4() {
  File file = SPIFFS.open("/fSensorName4.conf", "r");
  size_t sent = server.streamFile(file, "text/html");
}
void fSensorIR1() {
  File file = SPIFFS.open("/fSensorIR1.conf", "r");
  size_t sent = server.streamFile(file, "text/html");
}
void fSensorIR2() {
  File file = SPIFFS.open("/fSensorIR2.conf", "r");
  size_t sent = server.streamFile(file, "text/html");
}
void fSensorIR3() {
  File file = SPIFFS.open("/fSensorIR3.conf", "r");
  size_t sent = server.streamFile(file, "text/html");
}
void fSensorIR4() {
  File file = SPIFFS.open("/fSensorIR4.conf", "r");
  size_t sent = server.streamFile(file, "text/html");
}
void Lastbalance() {
  File file = SPIFFS.open("/Lastbalance", "r");
  size_t sent = server.streamFile(file, "text/html");
}
void Lastdistance() {
  File file = SPIFFS.open("/Lastdistance", "r");
  size_t sent = server.streamFile(file, "text/html");
}
void fSSID() {
  File file = SPIFFS.open("/fSSID.conf", "r");
  size_t sent = server.streamFile(file, "text/html");
}
void fPassword() {
  File file = SPIFFS.open("/fPassword.conf", "r");
  size_t sent = server.streamFile(file, "text/html");
}
void reboot() {
  ESP.restart();
}

void StartOperationalMode() {
  SPIFFS.begin();
  SetupOperationalMode = 0;
  Serial.println("Iniciando modo de operação");
  Serial.println("lendo configurações do sistema...");
  //Load Config
  ReadConfig("fSSID.conf");
  ReadConfig("fPassword.conf");
  //Load Device Names
  for (i = 0; i <= 3; i++) {
    ReadConfig("fSensorName" + (String)(i + 1) + ".conf");
  }
  //Load IR Values
  for (i = 0; i <= 3; i++) {
    ReadConfig("fSensorIR" + (String)(i + 1) + ".conf");
  }
  Serial.println("Local Wifi SSID:" + LocalWifiSSID);
  Serial.println("Local Wifi PASS:" + LocalWifiPass);
  for (i = 0; i <= 3; i++) {
    Serial.println("SensorName" + (String)(i + 1) + ":" + Valor[i] + ":" + Resp[i] );
  }
  //Start Wifi
  Serial.println("Setting Up Wifi");
  LocalWifiSSID.trim();
  LocalWifiPass.trim();
  WiFi.mode(WIFI_STA);
  WiFi.begin(LocalWifiSSID.c_str(), LocalWifiPass.c_str());
  Serial.println("Connecting to " + LocalWifiSSID);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("WiFi connected - ");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  FlashLEDSlow();
}

void ReadConfig(String FileNameToRead)
{
  Serial.println("Reading:" + FileNameToRead);
  String FileContent = "";
  FileNameToRead.trim();
  File FileObjR = SPIFFS.open("/" + FileNameToRead , "r");
  if (!FileObjR) {
    Serial.println("/" + FileNameToRead + " File not found.");
  }
  while (FileObjR.available()) {
    FileContent = FileContent += char(FileObjR.read());
  }
  FileObjR.close();
  if (FileNameToRead == "fSSID.conf") {
    LocalWifiSSID = FileContent;
  }
  if (FileNameToRead == "fPassword.conf") {
    LocalWifiPass = FileContent;
  }
  if (FileNameToRead == "fSensorName1.conf") {
    Valor[0] = FileContent;
  }
  if (FileNameToRead == "fSensorName2.conf") {
    Valor[1] = FileContent;
  }
  if (FileNameToRead == "fSensorName3.conf") {
    Valor[2] = FileContent;
  }
  if (FileNameToRead == "fSensorName4.conf") {
    Valor[3] = FileContent;
  }
  if (FileNameToRead == "fSensorIR1.conf") {
    Resp[0] = FileContent;
  }
  if (FileNameToRead == "fSensorIR2.conf") {
    Resp[1] = FileContent;
  }
  if (FileNameToRead == "fSensorIR3.conf") {
    Resp[2] = FileContent;
  }
  if (FileNameToRead == "fSensorIR4.conf") {
    Resp[3] = FileContent;
  }
}

// envio de valores via HTML
void SendHtml() {
  if (client.connect("docs.google.com", 443) == 1)//Tenta se conectar ao servidor do Google docs na porta 443 (HTTPS)
  {
    String toSend = textFix;//Atribuimos a String auxiliar na nova String que sera enviada
    toSend += "&entry.601296763=";//resposta 1
    scale.set_gain(128);
    band1 = scale.read();
    //calculo para kg
    Peso = ((Valor[1].toFloat()-Valor[0].toFloat())/(Resp[1].toFloat()-Resp[0].toFloat())*(band1-Valor[0].toFloat()))/1000  ;
    Serial.println((String)band1 +" kg: " +(String)Peso);
    toSend += Peso;//balanca
    toSend += "&entry.93145873=";//resposta 2
    scale.set_gain(32);
    band2 = scale.read();
    Serial.println(band2);
    toSend += band2;
    toSend += "&entry.1080741398=";//resposta 3
    toSend += sensors.getTempCByIndex(0);
    toSend += "&entry.649379059=";//resposta 4
    valor = analogRead(potenciometro);
    Distancia = (Valor[3].toFloat()-Valor[2].toFloat())/(Resp[3].toFloat()-Resp[2].toFloat())*(valor-Valor[2].toFloat());
    toSend += Distancia;
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
//LED
void FlashLED() {
  for (int i = 0; i <= 3; i++) {
    digitalWrite(2, HIGH);
    delay(100);
    digitalWrite(2, LOW);
    delay(100);
    digitalWrite(2, HIGH);
  }
}
void FlashLEDSlow()
{
  for (int i = 0; i <= 2; i++) {
    digitalWrite(2, HIGH);
    delay(200);
    digitalWrite(2, LOW);
    delay(200);
    digitalWrite(2, HIGH);
  }
}
void FlashLEDOnOff() {
  digitalWrite(2, LOW);
  delay(100);
  digitalWrite(2, HIGH);
}
