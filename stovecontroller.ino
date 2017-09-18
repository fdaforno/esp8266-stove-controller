#include <ESP8266WiFi.h>
#define TRIGGER 5
#define ECHO    4
long duration, distance; // Duration used to calculate distance
const char* ssid = "Raina";
const char* password = "2111795917";

//comandi stufa
char serialbuffer[4];  //buffer of stove response
byte tempH2O[] = {0x00, 0x03}; //ok
byte stovestatus[] = {0x00, 0x21}; //(0-off, 1-starting, 2-load pellet, 3-flame light, 4-work, 5-cleaning, 6-, 7-cleaning final)
byte curretpower[] = {0x00, 0x34}; // from 1 to 5
byte poweron[] = {0x80, 0x21, 0x01, 0xA2};
byte poweroff[] = {0x80, 0x21, 0x06, 0xA7};

//variabili stufa
int itempH2O;
int istovestatus;
int icurretpower;
// Create an instance of the server
// specify the port to listen on as an argument
WiFiServer server(80);


void setup() {
    //ultrasonic
      pinMode(TRIGGER, OUTPUT);
      pinMode(ECHO, INPUT);
      pinMode(BUILTIN_LED, OUTPUT);

    Serial.begin(1200, SERIAL_8N2);
    //Serial.begin(115200);
    delay(10);
    // Connect to WiFi network
    //Serial.print("Connecting to ");
    //Serial.println(ssid);
  
    WiFi.begin(ssid, password);
  
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      //Serial.print(".");
    }
    //Serial.println("");
    //Serial.println("WiFi connected");
  
  // Start the server
    server.begin();
  //  Serial.println("Server started");

  // Print the IP address
  //  Serial.println(WiFi.localIP());

}
void loop() {
    WiFiClient client = server.available();
    if (!client) {
      return;
    }
  
  // Wait until the client sends some data
    //Serial.println("new client");
    while(!client.available()){
     delay(1);
   }
   
    String req = client.readStringUntil('\r');
    //Serial.println(req);
    client.flush();
    // Match the request
  int val;


    if (req.indexOf("/geth2otemp") != -1)
       val = geth2otemp();
    else if (req.indexOf("/getstatus") != -1)
       val = getstatus();
    else if (req.indexOf("/getpower") != -1)
        val = getpower();
    else if (req.indexOf("/getpellet") != -1)
       val=getpellet();
    else if (req.indexOf("/poweron") != -1)
       val =2;
    else if (req.indexOf("/poweroff") != -1)
       val =2;
    else if (req.indexOf("/moto1on") != -1)
       val =2;
    else if (req.indexOf("/moto1off") != -1)
       val =2;
    else if (req.indexOf("/moto2on") != -1)
       val =2;
    else if (req.indexOf("/moto2off") != -1)
       val =2;
    else {
    //Serial.println("invalid request");
    client.stop();
    return;
  }

String s = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE HTML>\r\n<html> ";
  //s += (val)?"high":"low";
   s += val;


  // Send the response to the client
  client.print(s);
  delay(1);
 // Serial.println("Client disonnected");

    
}
int geth2otemp(){
  Serial.write(tempH2O, sizeof(tempH2O));
    while(Serial.available() < 4) {};
  Serial.readBytes(serialbuffer,4);
  itempH2O = serialbuffer[3];
  return itempH2O;
}

int getstatus(){
  Serial.write(stovestatus, sizeof(stovestatus));
    while(Serial.available() < 4) {};
  Serial.readBytes(serialbuffer,4);
  istovestatus = serialbuffer[3];
  return istovestatus;
}
int getpower(){
  Serial.write(curretpower, sizeof(curretpower));
    while(Serial.available() < 4) {};
  Serial.readBytes(serialbuffer,4);
  icurretpower = serialbuffer[3];
  return icurretpower;
}



long getpellet(){
/* The following trigPin/echoPin cycle is used to determine the
distance of the nearest object by bouncing soundwaves off of it. */
digitalWrite(TRIGGER, LOW);
delayMicroseconds(2);
digitalWrite(TRIGGER, HIGH);
delayMicroseconds(10);
digitalWrite(TRIGGER, LOW);
duration = pulseIn(ECHO, HIGH);
//Calculate the distance (in cm) based on the speed of sound.
distance = duration/58.2;
Serial.println(distance);
//Delay 50ms before next reading.
delay(50);
  return distance;
  
}




