/*
* Jarvis
* An IoT system made by Thomas Andreini
* https://github.com/ThomasAndreini/Jarvis
*/
//#define BLYNK_DEBUG
//#define BLYNK_PRINT Serial    // Comment this out to disable prints and save space
#define DHT11_PIN 4
#define RELE_PIN 5
#define EspSerial Serial1
#define ESP8266_BAUD 115200 //Baud Esp 8266
#define SSID "" //Your wifi SSID
#define PASS "" //Your wifi password
#define DST_IP "23.222.152.140" //ip of api.wunderground.com
#define LOCATIONID "" // location id get from wunderground.com

#include <ESP8266_Lib.h>
#include <BlynkSimpleShieldEsp8266.h>
#include <ArduinoJson.h>
#include <RTClib.h>
#include <Wire.h>
#include <Nextion.h>
#include <looper.h>
#include <dht11.h>
#include <TimeLib.h>
#include <WidgetRTC.h>

NexPage presultok    = NexPage(9, 0, "resultokpage");//dichiarazione elementi del display nextion
NexPage presultko    = NexPage(10, 0, "resultkopage");
NexDSButton bprgstat = NexDSButton(1, 8, "bprgstat");
NexButton b1ora = NexButton(1, 3, "b1ora");
NexButton b2ore = NexButton(1, 4, "b2ore");
NexButton b3ore = NexButton(1, 5, "b3ore");
NexButton b4ore = NexButton(1, 6, "b4ore");
NexButton bbackmeteo = NexButton(4, 3, "bbackmeteo");
NexButton brecwifi = NexButton(2, 8, "brecwifi");
NexButton brecgard = NexButton(2, 9, "brecgard");
NexButton bsaveter = NexButton(6, 17, "bsaveter");
NexButton bsavegard = NexButton(7, 17, "bsavegard");
NexButton bsavetime = NexButton(8, 15, "bsavetime");
NexButton bsavetemp = NexButton(12, 2, "bsavetemp");
NexButton boffter = NexButton(11, 2, "boffter");
NexButton btermos = NexButton(0, 3, "btermos");
NexButton bset = NexButton(0, 4, "bset");
NexButton bbackter = NexButton(1, 1, "bback");
NexButton bhomeimp1 = NexButton(3, 4, "bhome");
NexButton bnextimp = NexButton(2, 9, "bnext");
NexButton bprevimp = NexButton(3, 3, "bprev");
NexButton bhomeimp = NexButton(2, 1, "bhome");
NexNumber ntemp = NexNumber(0, 12, "ntemp");
NexNumber ntempexth = NexNumber(0, 13, "ntempext");
NexNumber nhumidh = NexNumber(0, 14, "numid");
NexNumber nhumidexth = NexNumber(0, 15, "numidext");
NexNumber nhourh = NexNumber(0, 16, "nhour");
NexNumber nminutesh = NexNumber(0, 17, "nminutes");
NexNumber ndayh = NexNumber(0, 19, "nday");
NexNumber nmonthh = NexNumber(0, 21, "nmonth");
NexNumber nyearh = NexNumber(0, 23, "nyear");
NexNumber nhours = NexNumber(6, 2, "nhours");
NexNumber nminutess = NexNumber(6, 3, "nminutess");
NexNumber nhoures = NexNumber(6, 4, "nhoures");
NexNumber nminutees = NexNumber(6, 5, "nminutees");
NexNumber noraset = NexNumber(8, 1, "noraset");
NexNumber nminset = NexNumber(8, 2, "nminset");
NexNumber ndayset = NexNumber(8, 3, "ndayset");
NexNumber nmonthset = NexNumber(8, 4, "nmonthset");
NexNumber nyearset = NexNumber(8, 16, "nyearset");
NexNumber nmanualenable = NexNumber(0, 26, "nmanualenable");
NexNumber ntempset = NexNumber(12, 3, "ntempset");
NexNumber nt0max = NexNumber(4, 13, "nt0max");
NexNumber nt0min = NexNumber(4, 22, "nt0min");
NexNumber nw0 = NexNumber(4, 25, "nw0");
NexNumber nt1max = NexNumber(4, 14, "nt1max");
NexNumber nt1min = NexNumber(4, 15, "nt1min");
NexNumber nt2max = NexNumber(4, 16, "nt2max");
NexNumber nt2min = NexNumber(4, 17, "nt2min");
NexNumber nt3max = NexNumber(4, 18, "nt3max");
NexNumber nt3min = NexNumber(4, 19, "nt3min");
NexNumber nlastsynch = NexNumber(0, 29, "nlastsynch");//debug per vedere se aggiorna meteo correttamente, provvisoria
NexText tdatah = NexText(0, 2, "tdayweek");
NexText terror = NexText(10, 2, "terror");
NexPicture pmeteohome = NexPicture(0, 11, "pmeteo");
NexPicture p0meteo = NexPicture(4, 9, "p0meteo");
NexPicture p1meteo = NexPicture(4, 10, "p1meteo");
NexPicture p2meteo = NexPicture(4, 11, "p2meteo");
NexPicture p3meteo = NexPicture(4, 12, "p3meteo");

looper Scheduler;
dht11 DHT;
RTC_DS1307 RTC;
ESP8266 wifi(&EspSerial);
WidgetLED ledterm(1);
WidgetLED ledirr(7);
WidgetRTC rtcblynk;

BLYNK_ATTACH_WIDGET(rtcblynk, V10)//rtc remoto assegnato al pin virtuale 10

char auth[] = ""; //Auth ID get from api developer registration on wunderground.com
char daysOfTheWeek[7][12] = { "Domenica", "Lunedi'", "Martedi'", "Mercoledi'", "Giovedi'", "Venerdi'", "Sabato"};//lista giorni della settimana
byte dayoftheweek = 0;//giorno della settimana attuale
int temperature = 0;//temperatura
int temperatureext = 0;//temperatura esterna
byte humidity = 0;//umidita'
byte humidityext = 0;//umidita' esterna
byte ore = 0;
byte minuti = 0;
byte secondi = 0;
byte giorni = 1;
byte mesi = 1;
unsigned int anni = 2000;
byte fast = 0; //indice se fast termo e' attivo
byte autostatus = 0; // indice se auto mode attivato
byte manualstatus = 0;//se 1 attiva il rele'
byte startmin = 0; // variabili di start e stop termostato in automode
byte starthour = 0;
byte endmin = 0;
byte endhour = 0;
byte lastautostatus = 0;
byte laststartmin = 0;
byte laststarthour = 0; // variabili di store per non perdere la programmazione in termofast
byte lastendmin = 0;
byte lastendhour = 0;
byte tempset = 25; // temperatura impostata in termostato
byte nextpageid = 0;// segnala quale pagina e' attiva su display nextion
byte blynkisconnected = 0; //status Blink.connected()
byte idbchk = 0;// blinkcheck index
byte idacq = 0;// acquisition index, is used for give time to wifi for connecting
byte timeout = 0;// timeout getnparseforecastntp()
const int buffer = 400; //dimensione buffer JSON
const char* iconName;//nome icone meteo
int num_elements_fr = 14; // numero degli elementi prelevati nel json delle previsioni
char* forecast[] = {"\"year\":", "\"celsius\":", "\"celsius\":", "\"icon\":", "\"kph\":", "\"celsius\":", "\"celsius\":", "\"icon\":", "\"celsius\":", "\"celsius\":", "\"icon\":", "\"celsius\":", "\"celsius\":", "\"icon\":"};
char* forecast_rename[] = { "\"year\":", "\"celsius_h_0\":", "\"celsius_l_0\":", "\"icon_0\":", "\"windkm_0\":", "\"celsius_h_1\":", "\"celsius_l_1\":", "\"icon_1\":", "\"celsius_h_2\":", "\"celsius_l_2\":", "\"icon_2\":", "\"celsius_h_3\":", "\"celsius_l_3\":", "\"icon_3\":"}; //rinomina per evitare doppi nomi
char close_brace = '}';
char comma = ',';
int t0max = 0;//variabili contententi temperature e numero icona meteo
int t0min = 0;
byte ws0 = 0;
int t1max = 0;
int t1min = 0;
int t2max = 0;
int t2min = 0;
int t3max = 0;
int t3min = 0;
byte meteo0 = 0;
byte meteo1 = 0;
byte meteo2 = 0;
byte meteo3 = 0;
byte lastsynch = 0; // ora dell'ultima sincronizzazione serve a avviare getnparseforecastntp una volta
byte firstrun = 1;//necessaria in getnparseforecast per non disconnettere blynk al primo avvio, visto che blynk non e' connesso
byte indx = 0;//indice per timeout reset esp8266
byte firstsynchrtc = 1; //esegue la sincronizzazione con l'rtc remoto solo la prima volta e prima di aggiornare il meteo
byte oralegale = 0; //tiene conto dell'ora legale/solare
byte meteorun = 0; //segnala se getnparseforecast e' attivo

NexTouch * nex_listen_list[] =//lista elementi display che inviano un comando da tenere conto
{
  &bprgstat, &b1ora, &b2ore, &b3ore, &b4ore, &brecwifi,
  &brecgard, &bsaveter, &bsavegard, &bsavetime, &boffter,
  &bsavetemp, &btermos, &bset, &bhomeimp1, &bprevimp, &bnextimp,
  &bhomeimp, &ntemp, &pmeteohome, &bbackmeteo,
  NULL
};

void setup() {
  Serial.begin(115200);// imposta i baudrate
  EspSerial.begin(115200);
  Serial.println(F("setup"));//avisa che si e' in setup
  Wire.begin();
  RTC.begin();
  if (! RTC.isrunning()) {//se l'rtc non e' attivo, segnalalo e impostalo secondo i valori sul display
    Serial.println(F("RTCnrun"));
    RTC.adjust(DateTime(anni, mesi, giorni, ore, minuti, secondi));
  }
  pinMode(RELE_PIN, OUTPUT);//inizializzo pin rele'
  digitalWrite(RELE_PIN, HIGH);//imposta il rele' come contatto aperto
  Scheduler.addTask(acquisition, 1000); //aggiunge il task allo scheduler, esegue la funzione acquisition ogni secondo
  Scheduler.addTask(check, 800); //aggiunge il task allo scheduler, esegue la funzione check ogni 0,8 secondo
  Scheduler.addTask(transmission1, 1000); //aggiunge il task allo scheduler, esegue la funzione transmission1 ogni secondo
  Scheduler.addTask(transmission2, 1300); //aggiunge il task allo scheduler, esegue la funzione transmission2 ogni 1,3 secondo
  Scheduler.addTask(blinkcheck, 10000); //aggiunge il task allo scheduler, esegue la funzione blinkcheck ogni 10 secondi
  bprgstat.attachPop(bprgstatPopCallback, &bprgstat);//funzioni di attach su gli elementi premibili sul display touch
  b1ora.attachPop(b1oraPopCallback, &b1ora);
  b2ore.attachPop(b2orePopCallback, &b2ore);
  b3ore.attachPop(b3orePopCallback, &b3ore);
  b4ore.attachPop(b4orePopCallback, &b4ore);
  brecwifi.attachPop(brecwifiPopCallback, &brecwifi);
  brecgard.attachPop(brecgardPopCallback, &brecgard);
  bsaveter.attachPop(bsaveterPopCallback, &bsaveter);
  bsavegard.attachPop(bsavegardPopCallback, &bsavegard);
  bsavetime.attachPop(bsavetimePopCallback, &bsavetime);
  boffter.attachPop(boffterPopCallback, &boffter);
  bsavetemp.attachPop(bsavetempPopCallback, &bsavetemp);
  btermos.attachPop(btermosPopCallback, &btermos);
  bset.attachPop(bsetPopCallback, &bset);
  bhomeimp.attachPop(bhomeimpPopCallback, &bhomeimp);
  bhomeimp1.attachPop(bhomeimp1PopCallback, &bhomeimp1);
  bprevimp.attachPop(bprevimpPopCallback, &bprevimp);
  bnextimp.attachPop(bnextimpPopCallback, &bnextimp);
  bbackter.attachPop(bbackterPopCallback, &bbackter);
  bbackmeteo.attachPop(bbackmeteoPopCallback, &bbackmeteo);
  bsavetemp.attachPop(bsavetempPopCallback, &bsavetemp);
  pmeteohome.attachPop(pmeteohomePopCallback, &pmeteohome);
  nexInit();
}

void loop() {
  Serial.println(F("loop"));
  if (firstrun == 1)meteorun = 1; //recupera le previsioni meteo se e' il primo avvio
  if (meteorun == 1) getnparseforecast();
  else {
    Scheduler.scheduler(); // funzione di esecuzione dello scheduler(looper)
    nexLoop(nex_listen_list);//controlla se qualche elemento dl display viene premuto e esegue le funzioni corrispondenti
    if (blynkisconnected == 1) Blynk.run(); //blynk e' connesso, esegui il refresh di Blynk
  }
}

BLYNK_CONNECTED() {//se blynk e' connesso
  rtcblynk.begin();//inizializza la sincronizzazione con l'rtc remoto
  blynkisconnected = 1;//segnala che blynk e' connesso
  Serial.println(F("Blynkconn"));
}

void bprgstatPopCallback(void *ptr)
{
  uint32_t dual_state;

  /* Get the state value of dual state button component . */
  bprgstat.getValue(&dual_state);
  if (dual_state)
  {
    autostatus = 1;
  }
  else
  {
    autostatus = 0;
    manualstatus = 0;
  }
}
void b1oraPopCallback(void *ptr)
{
  dbSerialPrintln("push done");
  laststartmin = startmin; // salva la programmazione per non perderla quando imposta autostatus
  laststarthour = starthour;
  lastendmin = endmin;
  lastendhour = endhour;
  startmin = minuti;
  starthour = ore;
  endmin = minuti;
  endhour = ore + 1;
  if (endhour > 23) endhour = endhour - 24;//evita che aggiungendo ore si superi le 24 ore
  fast = 1;
  lastautostatus = autostatus;
  autostatus = 1;
  presultok.show();
  nextpageid = 0;
}
void b2orePopCallback(void *ptr)
{
  laststartmin = startmin; // salva la programmazione per non perderla quando imposta autostatus
  laststarthour = starthour;
  lastendmin = endmin;
  lastendhour = endhour;
  startmin = minuti;
  starthour = ore;
  endmin = minuti;
  endhour = ore + 2;
  if (endhour > 23) endhour = endhour - 24;//evita che aggiungendo ore si superi le 24 ore
  fast = 1;
  lastautostatus = autostatus;
  autostatus = 1;
  presultok.show();
  nextpageid = 0;
}
void b3orePopCallback(void *ptr)
{
  laststartmin = startmin; // salva la programmazione per non perderla quando imposta autostatus
  laststarthour = starthour;
  lastendmin = endmin;
  lastendhour = endhour;
  startmin = minuti;
  starthour = ore;
  endmin = minuti;
  endhour = ore + 3;
  if (endhour > 23) endhour = endhour - 24;//evita che aggiungendo ore si superi le 24 ore
  fast = 1;
  lastautostatus = autostatus;
  autostatus = 1;
  presultok.show();
  nextpageid = 0;
}
void b4orePopCallback(void *ptr)
{
  laststartmin = startmin; // salva la programmazione per non perderla quando imposta autostatus
  laststarthour = starthour;
  lastendmin = endmin;
  lastendhour = endhour;
  startmin = minuti;
  starthour = ore;
  endmin = minuti;
  endhour = ore + 4;
  if (endhour > 23) endhour = endhour - 24;//evita che aggiungendo ore si superi le 24 ore
  fast = 1;
  lastautostatus = autostatus;
  autostatus = 1;
  presultok.show();
  nextpageid = 0;
}
void brecwifiPopCallback(void *ptr)
{

}
void brecgardPopCallback(void *ptr)
{

}
void bsaveterPopCallback(void *ptr)
{
  uint32_t sht;
  uint32_t smt;
  uint32_t shet;
  uint32_t smet;
  nhours.getValue(&sht);
  nminutess.getValue(&smt);
  nhoures.getValue(&shet);
  nminutees.getValue(&smet);
  starthour = byte(sht);
  startmin = byte(smt);
  endhour = byte(shet);
  endmin = byte(smet);
  autostatus = 1;
  presultok.show();
  nextpageid = 0;
}
void bsavegardPopCallback(void *ptr)
{

}
void bsavetimePopCallback(void *ptr)
{
  uint32_t ht;
  uint32_t mt;
  uint32_t gt;
  uint32_t mot;
  uint32_t at;
  noraset.getValue(&ht);
  nminset.getValue(&mt);
  ndayset.getValue(&gt);
  nmonthset.getValue(&mot);
  nyearset.getValue(&at);
  RTC.adjust(DateTime(at, mot, gt, ht, mt, 0));
  presultok.show();
  nextpageid = 0;
}
void bsavetempPopCallback(void *ptr)
{
  uint32_t temp;
  ntempset.getValue(&temp);
  tempset = byte(temp);
  presultok.show();
  nextpageid = 0;
}

void boffterPopCallback(void *ptr)
{
  if (manualstatus == 1) {
    manualstatus = 0;
    if (fast == 1) {
      startmin = laststartmin;// restituisce la vecchia programmazione
      starthour = laststarthour;
      endmin = lastendmin;
      endhour = lastendhour;
      fast = 0;
      autostatus = lastautostatus;
    }
    else autostatus = 0;
    presultok.show();
  }
  else {
    presultko.show();
    terror.setText("Riscaldamento gia' spento");
  }
  nextpageid = 0;
}

void btermosPopCallback(void *ptr)
{
  nextpageid = 1;
}
void bsetPopCallback(void *ptr)
{
  nextpageid = 2;
}
void bhomeimp1PopCallback(void *ptr)
{
  nextpageid = 0;
}
void bhomeimpPopCallback(void *ptr)
{
  nextpageid = 0;
}
void bbackterPopCallback(void *ptr)
{
  nextpageid = 0;
}
void bbackimpPopCallback(void *ptr)
{
  nextpageid = 0;
}
void bbackmeteoPopCallback(void *ptr)
{
  nextpageid = 0;
}
void pmeteohomePopCallback(void *ptr)
{
  nextpageid = 4;
}
void bnextimpPopCallback(void *ptr)
{
  nextpageid = 3;
}
void bprevimpPopCallback(void *ptr)
{
  nextpageid = 2;
}
BLYNK_WRITE(V0)//se vengono scritti valori su pin virtuale 0
{
  int i = param.asInt();
  manualstatus = byte(i);
  if (manualstatus == 0) {
    autostatus = 0;
    if (fast == 1) fast = 0;
  }
}
BLYNK_WRITE(V2)
{
  tempset = param.asInt();
}
void parseForecastJSON(char json[400])
{
  // Serial.print(F("json "));
  //Serial.println(json);
  StaticJsonBuffer<buffer> jsonBuffer;
  JsonObject& root = jsonBuffer.parseObject(json);

  if (!root.success())
  {
    Serial.print(F("parseObject() failed"));
  }
  int chkyearntp = root["year"];// variabile di controllo
  t0max = root["celsius_h_0"];
  t0min = root["celsius_l_0"];
  const char* icon_0 = root["icon_0"];
  ws0 = root["windkm_0"];
  t1max = root["celsius_h_1"];
  t1min = root["celsius_l_1"];
  const char* icon_1 = root["icon_1"];
  t2max = root["celsius_h_2"];
  t2min = root["celsius_l_2"];
  const char* icon_2 = root["icon_2"];
  t3max = root["celsius_h_3"];
  t3min = root["celsius_l_3"];
  const char* icon_3 = root["icon_3"];
  meteo0 = icontonumber(icon_0);//conversione nomi icone in numeri per richiamare le immagini sul display
  meteo1 = icontonumber(icon_1);
  meteo2 = icontonumber(icon_2);
  meteo3 = icontonumber(icon_3);
  if (chkyearntp > 0)lastsynch = ore; //se l'anno e' maggiore di 0 quindi il parse e' riuscito, copia l'ora in lastsynch in modo da non ripetere l'operazione prima di 2 ore
}

byte icontonumber(String icon) {
  /*
  sereno = 0
  sereno variabile = 1
  nuvoloso = 2
  pioggia = 3
  grandine = 4
  neve = 5
  temporale fulmini = 6
  nebbia = 7
  sereno notte = 8
  sereno variabile notte = 9
  */
  byte dataout;
  // ci sono altre icone ma con indexof trova la parola chiave es. rain in chancerain, quindi copre tutte le icone
  if (icon.indexOf("clear") != -1)dataout = 0;
  if (icon.indexOf("sunny") != -1)dataout = 0;
  if (icon.indexOf("cloudy") != -1)dataout = 2;
  if (icon.indexOf("fog") != -1)dataout = 7;
  if (icon.indexOf("hazy") != -1)dataout = 7;
  if (icon.indexOf("rain") != -1)dataout = 3;
  if (icon.indexOf("sleet") != -1)dataout = 4;
  if (icon.indexOf("flurries") != -1)dataout = 5;
  if (icon.indexOf("snow") != -1)dataout = 5;
  if (icon.indexOf("storms") != -1)dataout = 6;
  if (icon.indexOf("partlycloudy") != -1)dataout = 1;// questi vanno per ultimi altrimenti vengono segnati come cloudy o sunny
  if (icon.indexOf("partlysunny") != -1)dataout = 1;

  return dataout;
}
