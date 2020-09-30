#include <SoftwareSerial.h>
SoftwareSerial myserial(10,9); // RX: 10, TX:9
StaticJsonBuffer<200> jsonBuffer; 
char message = "Haii..";//test message.
void setup() {
  myserial.begin(9600);   // The GPRS baud rate is set at 9600.
  Serial.begin(9600);
  Serial.println("Initializing..........");
  DynamicJsonBuffer jsonBuffer;
}

void loop() {
  
 if (myserial.available())
  Serial.write(myserial.read());
 
  myserial.println("AT");
  delay(3000);
 
  myserial.println("AT+SAPBR=3,1,\"Contype\",\"GPRS\"");
  delay(6000);
  ShowSerialData();
 
  myserial.println("AT+SAPBR=3,1,\"APN\",\"airtelgprs.com\"");//APN
  delay(6000);
  ShowSerialData();
 
  myserial.println("AT+SAPBR=1,1");
  delay(6000);
  ShowSerialData();
 
  myserial.println("AT+SAPBR=2,1");
  delay(6000);
  ShowSerialData();
 
 
  myserial.println("AT+HTTPINIT");
  delay(6000);
  ShowSerialData();
 
  myserial.println("AT+HTTPPARA=\"CID\",1");
  delay(6000);
  ShowSerialData();
 
  StaticJsonBuffer<200> jsonBuffer;
  JsonObject& object = jsonBuffer.createObject();
  
  object.set("message",message);
  
  object.printTo(Serial);
  Serial.println(" ");  
  String sendtoserver;
  object.prettyPrintTo(sendtoserver);
  delay(4000);
 
  myserial.println("AT+HTTPPARA=\"URL\",\"http://192.xxxxxxxxxxxxxxxxxxxxxxxx.php\""); //Server address
  delay(4000);
  ShowSerialData();
 
  myserial.println("AT+HTTPPARA=\"CONTENT\",\"application/json\"");
  delay(4000);
  ShowSerialData();
 
 
  myserial.println("AT+HTTPDATA=" + String(sendtoserver.length()) + ",100000");
  Serial.println(sendtoserver);
  delay(6000);
  ShowSerialData();
 
  myserial.println(sendtoserver);
  delay(6000);
  ShowSerialData;
 
  myserial.println("AT+HTTPACTION=1");
  delay(6000);
  ShowSerialData();
 
  myserial.println("AT+HTTPREAD");
  delay(6000);
  ShowSerialData();
 
  myserial.println("AT+HTTPTERM");
  delay(10000);
  ShowSerialData;
 
}
 
 
void ShowSerialData()
{
  while (myserial.available() != 0)
    Serial.write(myserial.read());
  delay(1000);
 
}
