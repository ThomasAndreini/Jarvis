void transmission1() {
  Serial.println(F("transmission1"));
  switch (nextpageid) {
    case 0:
      nlastsynch.setValue(lastsynch);
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
      pmeteohome.setPic(meteo0 + 8);
      if (manualstatus == 1)nmanualenable.setValue(1);
      if (manualstatus == 0)nmanualenable.setValue(0);
      break;
    case 1:
      if (autostatus == 1)bprgstat.setValue(1);
      if (autostatus == 0)bprgstat.setValue(0);
      break;
    case 3:
      stemp.addValue(0, temperature + 10); //permette di visualizzare valori negativi fino a - 10
      stemp.addValue(1, temperatureext + 10);
      shumid.addValue(0, humidity);
      shumid.addValue(1, humidityext);
      break;
    case 4:
    if (meteo0== 0 || meteo0 == 1) if (ore > 19 || ore < 5)meteo0 = meteo0 + 8; //dalle 20 alle 4 visualizza icone notturne sereno e sereno variabile
    if (meteo1== 0 || meteo1 == 1) if (ore > 19 || ore < 5)meteo1 = meteo1 + 8; 
    if (meteo2== 0 || meteo2 == 1) if (ore > 19 || ore < 5)meteo2 = meteo2 + 8; 
    if (meteo3== 0 || meteo3 == 1) if (ore > 19 || ore < 5)meteo3 = meteo3 + 8; 
    if (meteo0== 8 || meteo0 == 9) if (ore < 19 && ore > 5)meteo0 = meteo0 - 8; //dalle 5 alle 19 visualizza icone diurne sereno e sereno variabile
    if (meteo1== 8 || meteo1 == 9) if (ore < 19 && ore > 5)meteo1 = meteo1 - 8; 
    if (meteo2== 8 || meteo2 == 9) if (ore < 19 && ore > 5)meteo2 = meteo2 - 8; 
    if (meteo3== 8 || meteo3 == 9) if (ore < 19 && ore > 5)meteo3 = meteo3 - 8; 
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
  if (blynkisconnected == 1) { // se Blink e' connesso
    Blynk.virtualWrite(V2, temperature);//temperatura
    Blynk.virtualWrite(V3, humidity);// umidita'
    Blynk.virtualWrite(V8, temperature);//temperatura gauge
    Blynk.virtualWrite(V9, humidity);// umidita'gauge
  }
}
