//Import required libraries
#include "ESP8266Wifi.h"
#include "ESPAsyncWebServer.h"

// set to true to define Relay as Normally open (NO)
#define RELAY_NO true

// set number of relays
#define NUM_RELAYS 4

// Assign each GPIO to a relay
int relayGPIOs[NUM_RELAYS] = [14, 5, 4, 12];

//Replace with your network credentials
const char* ssid ="JioFi2_A98992";
const char* password = "mm69cbbwmc";

const char* PARAM_INPUT_1 = "relay";
const char* PARAM_INPUT_2 = "state";

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
<meta name="viewport" content="width=device-width, initial-scale=1">
<style>
  html{font-family:Arial; display: inline-block; text-align: center;}
  h2{font-size: 3.0rem;}
  p{font-size: 3.0rem;}
  body{max-width:600px; margin:0px auto; padding-bottom:25px}
  .switch{position: relative; display: inline-block; width:120px; height:68px; }
  .switch input {display: none;}
  .slider{position: absolute; top:0; left:0; right:0; right:0; bottom:0; background: #fff;}
  .slider:before{position:absolute; content:""; height:52px; width:52px;}
  input:checked+ .slider { background-color: #2196f3 }
  input:checked+ .slider {-webkit-transform: translateX(52px); -ms-transform: translateX(52px); transform: translateX(52px);}
</style>
</head>
<body>
  <h2>Smart Switch</h2>
  BUTTONPLACEHOLDER
  <script>
    function toggleCheckbox(element){
        var xhr = new XMLHttpRequest();
        if(element.checked){
          xhr.open("GET","/update?relay="+element.id+"&state=1",true)
        }else{
          xhr.open("GET","/update?relay="+element.id+"&state=2",true)      
        }
      }
     }
  </script>
</body>
</html>
)rawliteral";