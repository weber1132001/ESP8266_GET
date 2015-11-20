#include <ESP8266WiFiMulti.h>
#include <WiFiUdp.h>
#include <WiFiServer.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>

#include <WiFiClient.h>
#include <WiFiUdp.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>

#define RELAY_ON  0
#define RELAY_OFF 1
#define Relay_1  D1
#define Relay_2  D2
#define Relay_3  D3
#define Relay_4  D4
IPAddress localIP;
String  localIPStr="";

int nCount=0;
const char* ssid     = "A514";
const char* password = "A514WIFI";
const char* host="192.168.1.10";

void setup() {
  pinMode(Relay_1, OUTPUT);  
  pinMode(Relay_2, OUTPUT);
  pinMode(Relay_3, OUTPUT);
  pinMode(Relay_4, OUTPUT);
  digitalWrite(Relay_1, RELAY_OFF);
  digitalWrite(Relay_2, RELAY_OFF);
  digitalWrite(Relay_3, RELAY_OFF);
  digitalWrite(Relay_4, RELAY_OFF);
  delay(3000);
  Serial.begin(9600);
  delay(1000);
  pinMode(Relay_1, OUTPUT);
  digitalWrite(Relay_1, RELAY_OFF);
  delay(3000);
  Serial.println();
  Serial.println();
  Serial.println("Connecting to ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
  delay(500);
  Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  localIP = WiFi.localIP();
byte first_octet = localIP[0];
byte second_octet = localIP[1];
byte third_octet = localIP[2];
byte fourth_octet = localIP[3];


char s[16];  
  sprintf(s, "%d.%d.%d.%d", (int)first_octet, (int)second_octet, (int)third_octet, (int)fourth_octet);
  localIPStr = s;
}

void loop() {
    nCount++;
    WiFiClient client;
    const int httpPort = 8080;
  
    if (!client.connect(host, httpPort)) {
      Serial.println("connection ");
      return;
    }
    
    delay(1000);
    String url = "/PowerConnection?";
    url += "mIP="+localIPStr+"&nCount="+nCount;

    Serial.print("Requesting URL: ");
    Serial.println(url);
    

    client.print(String("GET ") + url + " HTTP/1.1\r\n" +

                 "Host: " + host + "\r\n" + 
                 "Connection: close\r\n\r\n");

//  client.print ("GET "+ url +"\n\n");
  client.flush();

 String result = "";
 delay(1000);
 int lineCount=0;

 while(client.available()){
  /*
      Serial.println ("I am reading...");
      char c = client.read();
      Serial.print (c);
*/
//      char c[1];
  //    c[0]=client.read();
  //    Serial.print (c[0]);
      String line = client.readStringUntil('Y');

//      String line = c;
      result += line;

      lineCount++;      
//      delay(10);
//      Serial.print(line);
      
//      client.flush();
}
//     client.flush();
    delay(100);

    Serial.println();
    Serial.println("closing connection");
    int cc=0;
    int ccCount=0;
    
    for (cc=0;cc<result.length();cc++) {
    
      if (result.charAt(cc)=='\n') {
        ccCount++;
      }    
     
    }
   Serial.println ("ccCount:");
    Serial.println (ccCount);

    int xxCount = 0;
    String resultArray[ccCount];
    String singleLine = "";
    for (cc=0;cc<result.length();cc++) {
 /*
      if (result.charAt(cc)=='\n') {
        continue;
      }
      */
      if (result.charAt(cc)!='\n') {
        singleLine += result.charAt(cc);
      } else {
        resultArray[xxCount]= singleLine;
        singleLine = "";
        xxCount++;
      }
    }
    String mIPx = "";
    String nCountx="";
    String pStatex="";
    
    for (cc=0;cc<xxCount;cc++) {


      if (resultArray[cc].indexOf("mIP")!=-1) {
        mIPx = resultArray[cc];
      }
      if (resultArray[cc].indexOf("nCount")!=-1) {
         nCountx = resultArray[cc];
      }
      if (resultArray[cc].indexOf("pState")!=-1) {
        pStatex = resultArray[cc];
      }
      
    }
 
    String mIPStr = "";
    String nCountStr = "";
    String pStateStr = "";

    mIPStr = mIPx.substring(mIPx.indexOf("\t")+1);
    nCountStr=nCountx.substring(nCountx.indexOf("\t")+1);
    pStateStr=pStatex.substring(pStatex.indexOf("\t")+1);

    int rnCount = nCountStr.toInt();

    Serial.print ("mIPStr:");
    Serial.println (mIPStr);
    Serial.print ("nCountStr:");
    Serial.println (nCountStr);
    Serial.print ("pStateStr:");
    Serial.println (pStateStr);
    Serial.print ("nCount:");
    Serial.println (nCount);
    Serial.print ("rnCount:");
    Serial.println (rnCount);
    Serial.print ("localIP:");
    Serial.println (localIPStr);
    

//    int rnCount = nCountStr.toInt();

    boolean isNCount = false;
    boolean isMIP    = false;
    
    if (rnCount==nCount) {
      isNCount = true;
      Serial.println ("rnCount is equal to nCount");
    }
    if (localIPStr==mIPStr) {
      isMIP = true;
      Serial.println ("mIPStr is equal to mIP");
    }
    String powerState="x";
    if (isNCount && isMIP) {
//    if (isNCount && isMIP) {

      pStateStr = pStateStr.substring(0,pStateStr.indexOf(".")+1);
      Serial.println (pStateStr);
      powerState = pStateStr.substring(0,1);
      Serial.print ("powerState:");
      Serial.println (powerState);
      if (powerState=="1,3,5") powerOn();
      if (powerState=="0,2,4") powerOff();
      
    }     
    delay(1000);
}

void powerOff() {
 // if (powerState=="0,2,4");
  //{   
  Serial.println("Relay_1_OFF");
  digitalWrite(Relay_1, 1);
 // }
             /*digitalWrite(Relay_2, 1);
             digitalWrite(Relay_3, 1);
             digitalWrite(Relay_4, 1);*/
             
}
void powerOn() {
     Serial.println("Relay All ON");
             digitalWrite(Relay_1, 0);
             digitalWrite(Relay_2, 0);
             digitalWrite(Relay_3, 0);
             digitalWrite(Relay_4, 0);
}

