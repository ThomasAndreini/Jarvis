void getnparseforecast() {
  Serial.println(F("getparse"));
  idbchk = 0;//disattiva la riconnessione a blynk(vedi blynkcheck)
  if (firstrun == 0 && timeout == 0) { //se non e' il primo avvio e timeout e'uguale a zero, evita che si ripeta durante il timeout se riceve errori
    blynkisconnected = 0;//segnala che blynk e' disconnesso
    Blynk.disconnect();
    delay(500);
    Serial.begin(115200);//riconnetti le seriali, perche' Blynk.disconnect disconnette la serial e serial1
    EspSerial.begin(115200);
    delay(4000);//attendo che l'esp8266 sia pronto
  }
  if (firstrun == 1) firstrun = 0;//se e' il primo avvio azzera la variabile
  indx = 0;
  while (indx < 6) {//tenta il reset dell'esp per 5 volte
    delay(2000);
    Serial1.println(F("AT+RST")); // reset e controlla se l'ep8266 e' pronto
    delay(4000);
    if (Serial1.find("OK")) {//se il reset ha avuto successo
      //Serial.println(F("WiFi - Module is ready"));
      indx = 6;//imposta indx a 6 per uscire dal loop senza uscire dalla funzione
    } else {
      //Serial.println(F("Module doesn't respond."));
    }
    if (indx == 5)return; //se indx e' arrivata a 5 tentativi non riusciti esci dal loop e dalla funzione
    indx++;
  }

  if (timeout > 5) {//se dopo 6 tentativi non ha successo esce
    timeout = 0;//azzera il contatore del timeout
    indx = 0;//inizializza la variabile
    while (indx < 6) {
      delay(2000);
      Serial1.println(F("AT+RST")); // reset e controlla se l'ep8266 e' pronto
      delay(4000);
      if (Serial1.find("OK")) {
        //Serial.println(F("WiFi - Module is ready"));
        indx = 5;//imposta indx a 5 per uscire dal loop
      } //else {
      //Serial.println(F("Module doesn't respond."));
      //}
      indx++;
    }
    idbchk = 6;//attiva la riconnessione a blynk
    //Serial.println(F("timeout, esco"));
    lastsynch = ore;//se dopo 10 tentativi non riesce, imposta lastsynch e riprova alla prossima
    meteorun = 0;
    return;
  }
  timeout++;
  //Serial.print(F("timeout "));
  //Serial.println(timeout);
  String cmd = "AT+CWJAP=\"";
  cmd += SSID;
  cmd += "\",\"";
  cmd += PASS;
  cmd += "\"";
  //Serial.println(cmd);
  Serial1.println(cmd);
  delay(4000);
  if (Serial1.find("ERROR")) {
    //Serial.println(F("Can not connect to the WiFi."));
    return;
  }
  //Serial.println(F("wifi connect"));
  Serial1.println(F("AT+CIPMUX=0")); // imposta la connessione in modalita' singola
  //Serial.println(F("Connection Mode Single"));
  delay(1000);
  cmd = "AT+CIPSTART=\"TCP\",\"";
  cmd += DST_IP;
  cmd += "\",80";
  Serial1.println(cmd);//imposta la connessione TCP
  //Serial.println(cmd);
  delay(500);
  if (Serial1.find("ERROR")) {
    //Serial.println(F("Errore connessione con server"));
    return;
  }
  cmd = "GET ";
  cmd += "http://api.wunderground.com/api/4126cdff2c89e5af/forecast/q/";
  cmd += LOCATIONID;
  cmd += ".json HTTP/1.0\r\n\r\n\r\n";

  //Serial.println(cmd.length());//invia la lunghezza della stringa di richiesta
  Serial1.print(F("AT+CIPSEND="));
  Serial1.println(cmd.length());
  delay(500);
  if (Serial1.find(">")) {
    //Serial.println(F("invio"));
    //Serial.println(cmd);
    Serial1.print(cmd);//invia la stringa GET
  }
  else {
    //Serial.println(F("Errore, simbolo > non trovato"));
    firstrun = 1;
    return;
  } //ricezione json
  unsigned int i = 0; //contatore timeout
  char json[buffer] = "{"; // array per il parsing Json
  int n = 1;          // contatore di caratteri del json
  //Serial.println(F("while in"));
  while (!Serial1.find("\"simpleforecast\": {")) {}//attende la stringa indicata per iniziare l'acquisizione
  //Serial.println(F("while out"));
  unsigned long timeoutforecast = millis();
  for (int j = 0; j < num_elements_fr; j++) {
    while (!Serial1.find(forecast[j])) {
      if (timeoutforecast > millis()) timeoutforecast = millis();// se timeoutforecast e' maggiore di millis allora il contatore si e' riavviato, quindi aggiorna la variabile
      if (timeoutforecast + 20000 < millis())return; // se sono passati piu' di 20 secondi esci
    }

    String Str1 = forecast_rename[j];
    //Serial.println(Str1);

    for (int l = 0; l < (Str1.length()); l++)
    { json[n] = Str1[l];
      n++;
    }
    //Serial.println(F("fine for"));
    while (i < 60000) {
      if (Serial1.available()) {
        char c = Serial1.read();
        if (c == ',') {
          break;
        }
        if (c == '}') {
          n--;
          n--;
          n--;
          break;
        }

        json[n] = c;
        n++;
        i = 0;
      }
      i++;
    }
    //Serial.println(F("fine lettura buffer"));
    //non inserire qui comandi gravosi o sballa tutto
    if (j == num_elements_fr - 1)
    {
      json[n] = close_brace;
    }
    else
    {
      json[n] = comma;
    }
    n++;
  }
  //Serial.print(F("inizio parse "));
  //Serial.println(json);
  parseForecastJSON(json);
  //Serial.print(F("t0max "));
  //Serial.println(t0max);
  lastsynch = ore;
  nlastsynch.setValue(lastsynch);
  //Serial.print(F("lstsync "));
  //Serial.println(lastsynch);
  indx = 0;
  while (indx < 6) {
    delay(2000);
    Serial1.println(F("AT+RST")); // reset e controlla se l'ep8266 e' pronto
    delay(3000);
    if (Serial1.find("OK")) {
      //Serial.println(F("WiFi - Module is ready"));
      indx = 6;//imposta indx a 6 per uscire dal loop
    } else {
      //Serial.println(F("Module doesn't respond."));
    }
    indx++;
  }
  timeout = 0; //restta la variabile di timeout
  idbchk = 6;//attiva la riconnessione a blynk(vedi blynkcheck)
  nlastsynch.setValue(lastsynch);
  meteorun = 0;
}
