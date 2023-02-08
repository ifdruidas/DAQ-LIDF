void SendHtml() {
  if (client.connect("docs.google.com", 443) == 1)//Tenta se conectar ao servidor do Google docs na porta 443 (HTTPS)
  {
    String toSend = textFix;//Atribuimos a String auxiliar na nova String que sera enviada
    toSend += "&entry.601296763=";//resposta 1
    scale.set_gain(128);
    band1 = scale.read();
    Serial.print(band1);
    toSend += band1;//balanca
    toSend += "&entry.93145873=";//resposta 2
    scale.set_gain(32);
    band2 = scale.read();
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
