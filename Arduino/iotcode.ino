#include <ArduinoHttpClient.h>
//#include <WiFiClient.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <ArduinoJson.h>

#include "arduino_secrets.h"


//#include <SPI.h>


// setting up httpclien

///////please enter your sensitive data in the Secret tab/arduino_secrets.h
/////// Wifi Settings ///////
char ssid[] = SECRET_SSID;
char pass[] = SECRET_PASS;


char serverAddress[] = "192.168.0.3"; // server address
int port = 8080;
ESP8266WebServer server(80);

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

int buttonPin = 8;

boolean currentButton = LOW; // the current reading from the input pin
bool lastButton = LOW; // the previous reading from the input pin
// LED State
bool ledState;
unsigned long timeA;
unsigned long timeB;
DynamicJsonDocument dataTS(1024);

//******************** Serving Rest Server To listen on Commands through post to our*************************************************
void setRest() {
    String postBody = server.arg("plain");
    Serial.println(postBody);
 
    DynamicJsonDocument doc(512);
    DeserializationError error = deserializeJson(doc, postBody);
    if (error) {
        // if the file didn't open, print an error:
        Serial.print(F("Error parsing JSON "));
        Serial.println(error.c_str());
 
        String msg = error.c_str();
 
        server.send(400, F("text/html"),
                "Error in parsin json body! <br>" + msg);
 
    } else {
        JsonObject postObj = doc.as<JsonObject>();
 
        Serial.print(F("HTTP Method: "));
        Serial.println(server.method());
 
        if (server.method() == HTTP_POST) {
            if (postObj.containsKey("ledState")) {
 
                // Here store data or doing operation
                ledState = postObj["ledState"];
 
                // Create the response
                // To get the status of the result you can get the http status so
                // this part can be unusefully
                DynamicJsonDocument doc(512);
                doc["status"] = "OK";
 
                Serial.print(F("Stream..."));
                String buf;
                serializeJson(doc, buf);
 
                server.send(200, F("application/json"), buf);
                Serial.print(F("done."));
 
            }else {
                DynamicJsonDocument doc(512);
                doc["status"] = "KO";
                doc["message"] = F("No data found, or incorrect!");
 
                Serial.print(F("Stream..."));
                String buf;
                serializeJson(doc, buf);
 
                server.send(400, F("application/json"), buf);
                Serial.print(F("done."));
            }
        }
    }
}


// Define routing
void restServerRouting() {
    server.on("/", HTTP_GET, []() {
        server.send(200, F("text/html"),
            F("Welcome to the REST Web Server"));
    });
    // handle post request
    server.on(F("/setRoom"), HTTP_POST, setRest);
}
// Manage not found URL
void handleNotFound() {
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
}


//****************************End Serving Rest Server To listen on Commands through post to our*************************************************

// end 


void setup(){
  Serial.begin(115200);
  //  setting up ESP8266Wifi
  Serial.begin(9600);
  while (status != WL_CONNECTED)
  {
//    Serial.print("Attempting to connect to Network named: ");
//    Serial.println(ssid); // print the network name (SSID);

    // Connect to WPA/WPA2 network:
    status = WiFi.begin(ssid, pass);
  }

  // print the SSID of the network you're attached to:

      // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);


      // put your setup code here, to run once:
  pinMode(sA, INPUT);
  pinMode(sB, INPUT);
  pinMode(LED, OUTPUT);

  // with local DNS hostmane esp8266.local
  if (MDNS.begin("esp8266")) {
    Serial.println("MDNS responder started");
  }
 
  // Set server routing
  restServerRouting();
  // Set not found response
  server.onNotFound(handleNotFound);
  // Start server
  server.begin();
 
}




void loop() {

  server.handleClient();
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
void debounce(bool last){
  boolean current = digitalRead(buttonPin); //Read the button state
  if (last != current)                   //if it's different…
  {
    delay(5);                      //wait 5ms
    current = digitalRead(BUTTON); //read it again

    return current; //return the current value
    
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



