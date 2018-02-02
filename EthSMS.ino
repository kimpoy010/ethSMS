//INCLUDE LIBRARIES
#include <GPRS_Shield_Arduino.h>
#include <SoftwareSerial.h>
#include <Wire.h>
#include <Ethernet.h>

//SERIAL PIN ASSIGNMENT, BAUDRATE, PHONE NUMBER, MESSAGE
#define PIN_TX    3
#define PIN_RX    2
#define BAUDRATE  9600
#define PHONE_NUMBER "+639056600152"
#define MESSAGE  "Smoke was detected in your apartment unit"

String response = String(100); 
byte mac[6] = {0x54, 0x55, 0x58, 0x10, 0x00, 0x24};    
EthernetClient client;
char server[] = "192.168.2.100"; // change this to your server IP

GPRS GSMTEST(PIN_TX,PIN_RX,BAUDRATE);//RX,TX,BAUDRATE

void setup() {
  Serial.begin(9600);
  while(!GSMTEST.init()) {
      delay(1000);
      Serial.print("INIT ERROR\r\n");
  }  
  Serial.println("GSM INIT SUCCESS");
  Ethernet.begin(mac);
  Serial.print("IP Address        : ");
  Serial.println(Ethernet.localIP());
}

void loop() {
    if(client.connect(server, 80)){
            Serial.println("-> Connected");
            
            // Make a HTTP request:
            client.print( "GET /apartment/getnum.php");
            client.println( " HTTP/1.1");
            client.print( "Host: " );
            client.println(server);
            client.println( "Connection: close" );
            client.println();
            client.println();

           while (client.connected()) {
              if (client.available()) {
                char c = client.read();
                //Serial.print(c);
                response= response + c;
              }
           }
           
           
           //GSMTEST.sendSMS(response,MESSAGE);

           int contentBodyIndex = response.lastIndexOf('\n');
           if (contentBodyIndex > 0) {

              String str = response.substring(contentBodyIndex);
              int respLen = str.length() + 1;
              char charArr[respLen];
              str.toCharArray(charArr, respLen);
              Serial.print(str);
              //PHONE_NUMBER = str;
              GSMTEST.sendSMS(str,MESSAGE); //DEFINE PHONE NUMBER AND TEXT
           }
            delay(5000); 
            client.stop();
               
          }else{
            Serial.println("--> connection failed/n");
          }
}
