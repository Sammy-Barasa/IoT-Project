#include <ArduinoHttpClient.h>
#include <WiFi101.h>
#include <ArduinoJson.h>

#include "arduino_secrets.h"

// setting up httpclien

///////please enter your sensitive data in the Secret tab/arduino_secrets.h
/////// Wifi Settings ///////
char ssid[] = SECRET_SSID;
char pass[] = SECRET_PASS;


char serverAddress[] = "192.168.0.3"; // server address
int port = 8080;

WiFiClient wifi;
HttpClient client = HttpClient(wifi, serverAddress, port);
int status = WL_IDLE_STATUS;


// end 

// DEVICE ID
int device_id = 1;

//
char deviceIDRef = "xyz";

// 
int roomNumber = 1;

char OCCUPANT = "OCCUPANT"
// sensor A
int sensorA = 2;
    // sensor 2
int sensorB= 4;
    // LED
int LED = 12;


// LED State
bool ledState;
bool opA;
bool opB;
unsigned long timeA;
unsigned long timeB;
DynamicJsonDocument dataTS(1024);
DynamicJsonDocument resp;

void setup()
{

  //  setting up http
  Serial.begin(9600);
  while (status != WL_CONNECTED)
  {
    Serial.print("Attempting to connect to Network named: ");
    Serial.println(ssid); // print the network name (SSID);

    // Connect to WPA/WPA2 network:
    status = WiFi.begin(ssid, pass);
  }

  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip)


      // put your setup code here, to run once:
  pinMode(sensorA, INPUT);
  pinMode(sensorB, INPUT);
  pinMode(LED, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
 opA = digitalRead(sensorA);
 opB = digitalRead(sensorB);
 ledState = digitalRead(LED);

//  setting up time

if(opA == LOW){
timeA = millis();
}

if(opB == LOW){
timeB = millis();
}

 // setting up detecting direction

      
      if (timeA<timeB){
        // detecting moving out

          // reading ledState

        // sending to API: device_id,devIDREF, ledState, roomNumber,lastUSER:OCCUPANT, OCCUPIED:false
        dataTS["device_id"] = device_id;
        dataTS["deviceIDRef"] = deviceIDRef;
        dataTS["ledState"] = ledState;
        dataTS["roomNumber"] = roomNumber;
        dataTS["lastUser"] = OCCUPANT;
        dataTS["occupied"] = false;
        serializeJson(dataTS, Serial);
        char url = "https://iot-project-ic.herokuapp.com/api/update/"+device_id+"/"+deviceIDRef;
        httpSend(url, dataTS);

        // flush time
        timeA = 0;
        timeB = 0;

        dataTS.clear()
      }else{
        // detecting moving in

          // reading ledState
          if(ledState==LOw){
            digitalWrite(ledState, HIGH);
            
          }

          // sending to API: device_id,devIDREF, ledState, roomNumber,lastUSER:OCCUPANT, OCCUPIED:true

          dataTS["device_id"] = device_id;
          dataTS["deviceIDRef"] = deviceIDRef;
          dataTS["ledState"] = ledState;
          dataTS["roomNumber"] = roomNumber;
          dataTS["lastUser"] = OCCUPANT;
          dataTS["occupied"] = true;
          serializeJson(dataTS, Serial);
          char url = "https://iot-project-ic.herokuapp.com/api/update/" + device_id + "/" + deviceIDRef;
          httpSend(url, dataTS);

          // flush time
          timeA = 0;
          timeB = 0;

          dataTS.clear()
      }

      
 // setting up wi-fi module

 // streaming to our API

 // LED state
        
}

//  **************** FUNCTIONS ***************



// debouncing function

// httpSend function

httpSend(url,postData){
  client.beginRequest();
  client.post(url);
  client.sendHeader("Content-Type", "application/json");
  client.sendHeader("Content-Length", postData.length());
  client.sendHeader("X-Custom-Header", "custom-header-value");
  client.beginBody();
  client.print(postData);
  client.endRequest();

  // response
  // read the status code and body of the response
  int statusCode = client.responseStatusCode();
  String response = client.responseBody();
  Serial.print("Status code: ");
  Serial.println(statusCode);
  Serial.print("Response: ");
  Serial.println(response);
}