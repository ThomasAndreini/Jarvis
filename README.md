# Jarvis

/ITALIANO/

ATTENZIONE QUESTA VERSIONE NON VERRA' PIU' AGGIORNATA SI CONSIGLIA DI PASSARE A JARVIS NEXT

Jarvis è un sistema IoT che include un unità  principale, un unità esterna e un server opzionale.
L'unità principale è un termostato smart in grado di comunicare con il server e l'unità esterna, è in grado di scaricare le previsioni meteo dal sito wunderground.com  ed è controllabile tramite Blynk per Android e Ios.
L'unità esterna avrà in dotazione un sensore di temperatura, uno di umidità e un elettrovalvola per controllare l'irrigazione del giardino.

Il progetto è così composto:

Unità principale:

1x Arduino Mega
1x Smart Display Itead Nextion 3.2" touch
1x Esp8266
1x RTC 1307 I2C
1x DHT 11 (temperatura-umidità)
1x Scheda Relay 220v 10 ampere , 5v comando, GND per ON, +5v per OFF
1x alimentatore 220v AC ---> 5v DC 2 ampere
1x regolatore di tensione da 5v a 3,3v minimo 500 mah


Unità esterna: (da fare)

1x esp8266 esp 12E


Server:(opzionale)*

1x Raspberry PI (il mio è il 2 rev B+, ma puoi usarne uno qualunque)
1x Alimentatore 220v AC ---> 5v 2 ampere DC


*Se non hai un Raspberry PI e non vuoi procurartelo è possibile usare i server cloud di Blynk, questo però diminuirà la sicurezza del sistema, in quanto Arduino e Esp8266 comunicano in chiaro con il server.

Caratteristiche unità principale :
Calendario completo di gestione dell'ora solare / legale
Sincronizzazione data / ora automatica tramite internet o impostazione manuale.
Download previsioni meteo per 4 giorni
Restituzione previsioni temperature massime e minime e vento primo giorno
Icone meteo cambiano secondo giorno / notte
Impostazione timer attivazione / spegnimento
Impostazione avvio rapido
Controllo touch
Restituzione temperatura e umidità interna e esterna(quando pronto il modulo esterno)
Visualizzazione e impostazione unità esterna.
Controllabile e monitorabile tramite app Blynk disponibile per Android e IOS (scansionsionare l'immagine blynkbarcode.png con l'app Blynk per scaricare il progetto)



/ENGLISH/

CAUTION THIS VERSION WILL NOT BE MORE UPDATE IS ADVISED TO MOVE TO JARVIS NEXT

Jarvis is an IoT system that includes a main unit, an outdoor unit and an optional server.
The main unit is a smart thermostat can communicate with the server and the external unit, it can download weather forecasts from wunderground.com site and is controllable via Blynk for Android and iOS.
The outdoor unit will have supplied a temperature sensor, a humidity and a solenoid valve to control irrigation of the garden.

The project consists of:

Main unit:

1x Arduino Mega
1x Itead Nextion Smart Display 3.2 "touch
1x Esp8266
1x I2C RTC 1307
1x DHT 11 (temperature-humidity)
1x PCB Relay 220V 10 amperes, 5v control, ON for GND, + 5v to OFF
1x 220v AC power supply ---> 5v DC 2 Amps
1x voltage regulator from 5V to 3.3V minimum 500 mah


Outdoor unit: (to do)

1x esp8266 esp 12E


Server: (Optional) *

1x Raspberry PI (mine is 2 Rev. B +, but you can use one any)
1x 220v AC power supply ---> 5v 2 amperes DC


* If you have a Raspberry PI and do not want buy it you can use the Blynk cloud server, but this will decrease the security of the system, as Arduino and Esp8266 communicate clearly with the server.

Main Unit Features:
Calendar Full Time / Daylight Management
Synchronizing date / time automatically via the internet or manual setting.
Download weather forecast for 4 days
Returning maximum and minimum temperatures and wind forecast first day
weather icons change according to the day / night
Setting timer on / off
Quick Start setting
touch control
internal and external temperature and humidity return (when ready, the external module)
Viewing and outdoor unit setting.
Operated and monitored via Blynk app available for Android and IOS (scansionsionare the blynkbarcode.png image with Blynk app to download the project)
