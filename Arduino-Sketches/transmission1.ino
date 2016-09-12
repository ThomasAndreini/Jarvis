void transmission1() {
  Serial.println(F("transmission1"));
  switch (nextpageid) {
    case 0:
      // nlastsynch.setValue(oralegale);
      nhourh.setValue(ore);
      nminutesh.setValue(minuti);
      ndayh.setValue(giorni);
      nmonthh.setValue(mesi);
      nyearh.setValue(anni);
      ntemp.setValue(temperature);
      ntempexth.setValue(temperatureext);
      nhumidh.setValue(humidity);
      nhumidexth.setValue(humidityext);
      tdatah.setText( daysOfTheWeek[dayoftheweek]);
      if (meteo0 == 0 || meteo0 == 1) if (ore > 19 || ore < 5)meteo0 = meteo0 + 8; //dalle 20 alle 4 visualizza icone notturne sereno e sereno variabile
      if (meteo0 == 8 || meteo0 == 9) if (ore < 19 && ore > 5)meteo0 = meteo0 - 8; //dalle 5 alle 19 visualizza icone diurne sereno e sereno variabile
      pmeteohome.setPic(meteo0 + 8);
      if (manualstatus == 1)nmanualenable.setValue(1);//sincronizza la variabile su nextion con manual status
      if (manualstatus == 0)nmanualenable.setValue(0);
      break;
    case 1:
      if (autostatus == 1)bprgstat.setValue(1);
      if (autostatus == 0)bprgstat.setValue(0);
      break;
    case 3:
      // seconda pagina impostazioni
      break;
    case 4:
      if (meteo0 == 0 || meteo0 == 1) if (ore > 19 || ore < 5)meteo0 = meteo0 + 8; //dalle 20 alle 4 visualizza icone notturne sereno e sereno variabile
      if (meteo1 == 0 || meteo1 == 1) if (ore > 19 || ore < 5)meteo1 = meteo1 + 8;
      if (meteo2 == 0 || meteo2 == 1) if (ore > 19 || ore < 5)meteo2 = meteo2 + 8;
      if (meteo3 == 0 || meteo3 == 1) if (ore > 19 || ore < 5)meteo3 = meteo3 + 8;
      if (meteo0 == 8 || meteo0 == 9) if (ore < 19 && ore > 5)meteo0 = meteo0 - 8; //dalle 5 alle 19 visualizza icone diurne sereno e sereno variabile
      if (meteo1 == 8 || meteo1 == 9) if (ore < 19 && ore > 5)meteo1 = meteo1 - 8;
      if (meteo2 == 8 || meteo2 == 9) if (ore < 19 && ore > 5)meteo2 = meteo2 - 8;
      if (meteo3 == 8 || meteo3 == 9) if (ore < 19 && ore > 5)meteo3 = meteo3 - 8;
      p0meteo.setPic(meteo0 + 8);//allinea i numeri ricevuti con i numeri delle icone sul display
      p1meteo.setPic(meteo1 + 18);
      p2meteo.setPic(meteo2 + 18);
      p3meteo.setPic(meteo3 + 18);
      nt0max.setValue(t0max);
      nt0min.setValue(t0min);
      nt1max.setValue(t1max);
      nw0.setValue(ws0);
      nt1min.setValue(t1min);
      nt2max.setValue(t2max);
      nt2min.setValue(t2min);
      nt3max.setValue(t3max);
      nt3min.setValue(t3min);
      break;
  }

  if (blynkisconnected == 1)
    if (offset > 59) { //serve a fare l'upload al server blynk ogni circa 60 secondi per alcuni valori come temp e umid
      offset = 0;
      if (blynkisconnected == 1) { // se Blink e' connesso
        Blynk.virtualWrite(V8, temperature);//temperatura interna gauge
        Blynk.virtualWrite(V9, humidity);// umidita'interna gauge
        // Blynk.virtualWrite(V4, temperatureext);//temperatura esterna
        //Blynk.virtualWrite(V5, humidityext);//umidita' esterna
      }
    } else offset++;
}
