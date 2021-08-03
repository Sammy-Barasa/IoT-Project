#include <ArduinoHttpClient.h>
#include <WiFi101.h>
#include <ArduinoJson.h>
#include "arduino_secrets.h"

//#include <SPI.h>
/////// Wifi Settings ///////
char ssid[] = SECRET_SSID;
char pass[] = SECRET_PASS;
char serverAddress[] = "192.168.0.3";  // server address
int port = 8080;

WiFiClient wifi;
HttpClient client = HttpClient(wifi, serverAddress, port);
int status = WL_IDLE_STATUS;
// DEVICE ID
int device_id = 1;

//
String deviceIDRef = "xyz";

// 
int roomNumber = 1;

char OCCUPANT[] = "OCCUPANT";
// sensor A
int sA = 2;
    // sensor 2
int sB= 4;
    // LED
int LED = 12;

int buttonPin = 7;

boolean currentButton = LOW; // the current reading from the input pin
boolean lastButton = LOW; // the previous reading from the input pin
// LED State
bool ledState;
unsigned long timeA;
unsigned long timeB;
DynamicJsonDocument dataTS(1024);




void setup(){
  Serial.begin(9600);
  while ( status != WL_CONNECTED) {
    Serial.print("Attempting to connect to Network named: ");
    Serial.println(ssid);                   // print the network name (SSID);

    // Connect to WPA/WPA2 network:
    status = WiFi.begin(ssid, pass);
  }

  // print the SSID of the network you're attached to:
  
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.println(ip);

      // put your setup code here, to run once:
  pinMode(sA, INPUT);
  pinMode(sB, INPUT);
  pinMode(LED, OUTPUT);
 
}




void loop() {

  // put your main code here, to run repeatedly:
 
 ledState = digitalRead(LED);

//  setting up time

if(digitalRead(sA) == LOW){
timeA = millis();
}

if(digitalRead(sB) == LOW){
timeB = millis();
}

 // setting up detecting direction

      
      if (timeA<timeB){
        // detecting moving out

          // reading ledState

        // sending to API: device_id,devIDREF, ledState, roomNumber,lastUSER:OCCUPANT, OCCUPIED:false
        
        dataTS["ledState"] = ledState;
        dataTS["roomNumber"] = roomNumber;
        dataTS["lastUser"] = OCCUPANT;
        dataTS["occupied"] = false;
        String url = "https://iot-project-ic.herokuapp.com/api/update/"+String(device_id)+"/"+deviceIDRef;
        Serial.println(url);
        httpSend(url, dataTS);

        // flush time
        timeA = 0;
        timeB = 0;

        dataTS.clear();
      }else{
        // detecting moving in

          // reading ledState
          if(ledState==LOW){
            digitalWrite(ledState, HIGH); 
          }

          // sending to API: device_id,devIDREF, ledState, roomNumber,lastUSER:OCCUPANT, OCCUPIED:true

          dataTS["ledState"] = ledState;
          dataTS["roomNumber"] = roomNumber;
          dataTS["lastUser"] = OCCUPANT;
          dataTS["occupied"] = true;
          String url = "https://iot-project-ic.herokuapp.com/api/update/"+String(device_id)+"/"+deviceIDRef;
          httpSend(url,dataTS);

          // flush time
          timeA = 0;
          timeB = 0;

          dataTS.clear();
      }
      currentButton = debounce(lastButton);           //read deboucned state
      if (lastButton == LOW && currentButton == HIGH) //if it was pressed...
      {
        ledState = !ledState; //toggle the LED value

        lastButton = currentButton; //reset button value

        digitalWrite(LED, ledState); //change the LED state

        dataTS["ledState"] = ledState;
        dataTS["roomNumber"] = roomNumber;
        dataTS["lastUser"] = OCCUPANT;
        dataTS["occupied"] = true;
        String url = "https://iot-project-ic.herokuapp.com/api/update/" + String(device_id) + "/" + deviceIDRef;
        Serial.println(url);
        httpSend(url, dataTS);
        dataTS.clear();
      }
}


// setting up wi-fi module

// streaming to our API

// LED state
        


//  ******************************** FUNCTIONS **********************************************
//  ******************************** FUNCTIONS **********************************************
//  ******************************** FUNCTIONS **********************************************


// *******************************debouncing function*****************************************
boolean debounce(boolean last){
  boolean current = digitalRead(buttonPin); //Read the button state
  if (last != current)                   //if it's different…
  {
    delay(5);                      //wait 5ms
    current = digitalRead(buttonPin); //read it again

    return current; //return the current value
  }
}

//******************************** httpSend function *****************************************

void httpSend(String url,DynamicJsonDocument postData){
  String jsn;
  serializeJson(postData, jsn);
  client.beginRequest();
  String contentType = "application/json";
  client.put(url,contentType,jsn);
//  client.sendHeader("Content-Type", "application/json");
//  client.sendHeader("X-Custom-Header", "custom-header-value");
//  client.beginBody();
  client.endRequest();

  // response
  // read the status code and body of the response
  int statusCode = client.responseStatusCode();
  String response = client.responseBody();
//  Serial.print("Status code: ");
//  Serial.println(statusCode);
//  Serial.print("Response: ");
//  Serial.println(response);
};
