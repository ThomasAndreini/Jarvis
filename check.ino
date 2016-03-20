void check(void) {
  Serial.println(F("check"));
  if (autostatus == 1 && temperature < (tempset)) { // se e' attivo temporizzatore e la temperatura e' inferiore alla temperatura impostata
    if (starthour  < endhour) {// se tempo di avvio e' minore tempo stop
      if (startmin + (starthour * 60) <= minuti + (ore * 60) && endmin + (endhour * 60) > minuti + (ore * 60) ) manualstatus = 1; // se l'orario impostato e' minore o uguale di quello corrente e quello di stop e' maggiore di quello corrente attiva il rele
      else {
        manualstatus = 0; //altrimenti lo disattiva
        if (fast == 1) {// se e' attivo termofast disattiva auto per evitare la ripetizione
          startmin = laststartmin;// restituisce la vecchia programmazione
          starthour = laststarthour;
          endmin = lastendmin;
          endhour = lastendhour;
          fast = 0;
          autostatus = lastautostatus;
        }
      }
    }
    if (starthour > endhour) { //se il tempo di avvio e' maggiore del tempo di stop
      if (startmin + (starthour * 60) > minuti + (ore * 60) && endmin + (endhour * 60) <= minuti + (ore * 60) ) {
        manualstatus = 0; // se l'orario impostato e' maggiore di quello corrente e quello di stop e' minore o uguale di quello corrente disattiva il rele
        if (fast == 1) {
          startmin = laststartmin;
          starthour = laststarthour;
          endmin = lastendmin;
          endhour = lastendhour;
          fast = 0;
          autostatus = lastautostatus;
        }
      }
      else manualstatus = 1; //altrimenti lo attiva
    }
  }
  if (manualstatus == 1 && temperature > (tempset + 1)) manualstatus = 0;// se e' attivo termostato e la temperatura e' superiore alla temperatura impostata + 1, serve a evitare attacca stacca continuo, spegni termostato
  if (manualstatus == 1 ) digitalWrite(RELE_PIN, LOW); // mette il pin basso e attiva il rele se manual status e' 1 e se temperatura e' inferiore a temperatura impostata
  else digitalWrite(RELE_PIN, HIGH); // mette il pin alto e disattiva il rele
}

