void blinkcheck() {
  Serial.println(F("blinkcheck"));
  if (blynkisconnected == 0) idbchk = idbchk + 1; //if Blink is disconnected add 1 to idbchk
  else idbchk = 0;
  if (idbchk > 5) { // if idbchk is equal to 6 so 60 seconds
    wdt_disable();
    Blynk.begin(auth, wifi, SSID, PASS, "server ip"); // Insert here Blyk server ip
    delay(4000);
    Blynk.connect();//try to connect
    idacq = 0;//set to 0 the acquisition id for give time to connect
    idbchk = 0; // set to 0 the blinkcheck id, next connection try, if blynk is disconnected, is after one minutes
    wdt_enable(WDTO_8S);
  }
}

