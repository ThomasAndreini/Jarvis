void transmission2() {
  Serial.println(F("transmission2"));
    if (blynkisconnected == 1) { // se Blink e' connesso
      // Blynk.virtualWrite(V4, temperatureext);//temperatura esterna
      //Blynk.virtualWrite(V5, humidityext);//umidita' esterna
      Blynk.virtualWrite(V0, manualstatus);// switch attiva risc
      if (manualstatus == 0)ledterm.off(); //led widget
      else ledterm.on();
      // Blynk.virtualWrite(V6, millis() / 1000); //led irrigazione attiva
      //Blynk.virtualWrite(V7, millis() / 1000);// switch attiva irrigazione
    }
  }
//necessario dividere la trasmissione a blink in 2 per evitare flood e disconnessione frequente o a uso
