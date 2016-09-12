import requests
from urllib2 import Request, urlopen

r = requests.get("http://api.wunderground.com/api/4126cdff2c89e5af/forecast/q/pws:ICASTIGL11.json")
data = r.json()

Tweek = ["", "", "", ""]
Tcond = ["", "", "", ""]
Thigh = ["", "", "", ""]
Tlow = ["", "", "", ""]
Twind = ["", "", "", ""]
Tdirwind = ["", "", "", ""]
i = 0

for day in data['forecast']['simpleforecast']['forecastday']:
    Tweek[i] = day['date']['weekday']
    Tcond[i] = day['conditions']
    Thigh[i] = day['high']['celsius']
    Tlow[i] = day['low']['celsius']
    Twind[i] = day['maxwind']['kph']
    Tdirwind[i] = day['maxwind']['dir']
    i=i+1

tosend = "[" + "\"" + str(Tweek[0]) +"\"" +  "," + "\"" + str(Tcond[0]) + "\"" +  "," + "\"" + str(Thigh[0]) + "\"" +  "," + "\"" +  str(Tlow[0]) + "\"" +  "," + "\"" +  str(Twind[0]) + "\"" +  "," + "\"" +  str(Tdirwind[0]) + "\"" +  "," + "\"" +  str(Tweek[1]) + "\"" + "," +"\"" +  str(Tcond[1]) + "\"" + "," + "\"" + str(Thigh[1]) + "\"" + "," + "\"" + str(Tlow[1]) + "\"" + "," + "\"" + str(Twind[1]) + "\"" + "," + "\"" + str(Tdirwind[1]) + "\"" + "," + "\"" + str(Tweek[2]) + "\"" + "," + "\"" + str(Tcond[2]) + "\"" + "," + "\"" + str(Thigh[2]) + "\"" + "," + "\"" + str(Tlow[2]) + "\"" + "," + "\"" + str(Twind[2]) + "\"" + "," + "\"" + str(Tdirwind[2]) + "\"" + "," + "\"" + str(Tweek[3]) + "\"" + "," + "\"" + str(Tcond[3]) + "\"" + "," + "\"" + str(Thigh[3]) + "\"" + "," + "\"" + str(Tlow[3]) + "\"" + "," + "\"" + str(Twind[3]) + "\"" + "," + "\"" + str(Tdirwind[3]) + "\"" + "]"

headers = {
  'Content-Type': 'application/json'
}
request = Request('http://127.0.0.1:8080/5c07e4b691864f0ea28041b81f47ab33/pin/V11', data=tosend, headers=headers)
request.get_method = lambda: 'PUT'

response_body = urlopen(request).read()
print response_body

