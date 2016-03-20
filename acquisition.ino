void acquisition() {
  Serial.println(F("acquisition"));
  Serial.print(F("ore "));
  Serial.println(ore);
  Serial.print(F("lstsynch "));
  Serial.println(lastsynch);
  Serial.print(F("blynkisconn "));
  Serial.println(blynkisconnected);
  if (ore == 0)lastsynch = 0;//azzera lastsynch per evitare che dopo mezzanotte l'if sotto non funzioni
  if (ore == 1 || ore == 7 || ore == 13 || ore == 19) {
    if (ore >= lastsynch + 1) {
      if (blynkisconnected == 1) RTC.adjust(DateTime(year(), month(), day(), hour() + 1, minute(), second()));
      getnparseforecast(); //ogni 6 ore avvia aggiornamento meteo, se sono le ore 1,7,13,19 e se sono passate almeno 1 ora dall'ultima sincronizzaizone con successo
    }
  }
  if ( DHT.read(DHT11_PIN) == DHTLIB_OK) { //se dht e' disponibile
    temperature = DHT.temperature;
    humidity = DHT.humidity;
  }
  if (firstsynchrtc == 1 && blynkisconnected == 1 && year() > 2015) {
    if (hour() == 23)RTC.adjust(DateTime(year(), month(), day() + 1, 0, minute(), second()));
    else (RTC.adjust(DateTime(year(), month(), day(), hour() + 1, minute(), second())));
    firstsynchrtc = 0; //azzera la variabile per evitare il loop
  }
  DateTime now = RTC.now();
  ore = now.hour();
  minuti = now.minute();
  secondi = now.second();
  giorni = now.day();
  mesi = now.month();
  anni = now.year();
  Serial.print (F("ora "));
  Serial.println(ore);
  Serial.print (F("giorni "));
  Serial.println(giorni);
  dayoftheweek = now.dayOfTheWeek();
  idacq = idacq++; // first time and when idbchk is 6 execute this function after 4 second, after execute every 1 second
  if (idacq > 4 ) {
    blynkisconnected = Blynk.connected(); // after 5 secons, to increase or decrease, depend to connection delay
    Serial.print(F("blynkisconn "));
    Serial.println(blynkisconnected);
  }
  if (idacq > 200) idacq = 4; // evita che si saturi la variabile
}

