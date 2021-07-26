
// DEVICE ID
int device_id = 1;

//
char deviceIDRef = "xyz";

// 
int roomNumber = 1;


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

    void setup()
{
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

        timeA = 0;
        timeB = 0;
      }else{
        // detecting moving in

          // reading ledState
          if(ledState==LOw){
            digitalWrite(ledState, HIGH);
            
          }else{ 
          }

          // sending to API: device_id,devIDREF, ledState, roomNumber,lastUSER:OCCUPANT, OCCUPIED:true

          timeA = 0;
          timeB = 0;
      }

      
 // setting up wi-fi module

 // streaming to our API

 // LED state
        
}

//  **************** FUNCTIONS ***************



// debouncing function