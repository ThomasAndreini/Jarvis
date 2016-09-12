void acquisition() {
  Serial.println(F("acquisition"));
  Serial.print(F("ore "));
  Serial.println(ore);
  Serial.print(F("blynkisconn "));
  Serial.println(blynkisconnected);
  if ( DHT.read(DHT11_PIN) == DHTLIB_OK) { //se dht e' disponibile
    temperature = DHT.temperature;
    humidity = DHT.humidity;
  }
  if (firstsynchrtc == 1 && blynkisconnected == 1 && year() > 2015 && hour() < 22) { // se l'rtc remoto e' sincronizzato e e' il primo synch(e sono meno delle 22, altrimenti incasina data ora)
    RTC.adjust(DateTime(year(), month(), day(), hour(), minute(), second())); //sistema l'ora
    firstsynchrtc = 0; //azzera la variabile per evitare il loop
  }
  DateTime now = RTC.now();
  ore = now.hour();
  minuti = now.minute();
  secondi = now.second();
  giorni = now.day();
  mesi = now.month();
  anni = now.year();
  if (blynkisconnected == 1 && year() > 2015 && hour() < 22 && ore != hour())RTC.adjust(DateTime(year(), month(), day(), hour(), minute(), second())); //se vede una differenza tra l'rtc remoto e l'rtc con fuso, sincronizza l'rtc
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

