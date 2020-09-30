#include <SPI.h>
#include <MFRC522.h>
#include <RTClib.h>  
#include <SoftwareSerial.h>
SoftwareSerial myserial(10, 11); // RX: 10, TX:11
MFRC522 mfrc522(8, 9);
#include <ArduinoJson.h>
StaticJsonBuffer<200> jsonBuffer; 
char t[32];// String to store time.
char  Device_ID[32] // String to store UID of tag.
RTC_DS3231 rtc;
void setup()
{
  myserial.begin(9600);        // the GPRS baud rate
  Serial.begin(9600);
  Serial.println("Initializing..........");
  DynamicJsonBuffer jsonBuffer;
  SPI.begin();      // Init SPI bus
  mfrc522.PCD_Init();   // Init MFRC522
  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }
  rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  //rtc.adjust(DateTime(2020, 02, 29, 17, 50, 40));
  delay(5000);

}
 
void loop()
{
    if (!rfid.PICC_IsNewCardPresent() || !rfid.PICC_ReadCardSerial())
    {
    return;
    }

    // Serial.print(F("PICC type: "));
    MFRC522::PICC_Type piccType = rfid.PICC_GetType(rfid.uid.sak);
    // Serial.println(rfid.PICC_GetTypeName(piccType));

    // Check is the PICC of Classic MIFARE type
    if (piccType != MFRC522::PICC_TYPE_MIFARE_MINI &&
        piccType != MFRC522::PICC_TYPE_MIFARE_1K &&
        piccType != MFRC522::PICC_TYPE_MIFARE_4K) 
    {
              Serial.println(F("Your tag is not of type MIFARE Classic."));
              return;
    }

    String strID = "";
    for (byte i = 0; i < 4; i++) 
    {
          strID +=  (rfid.uid.uidByte[i] < 0x10 ? "0" : "") + String(rfid.uid.uidByte[i], HEX) +(i!=3 ? ":" : "");
    }
    strID.toUpperCase();

    rfid.PICC_HaltA();
    DateTime now = rtc.now(); // To get the current date and time.
 
    sprintf(t, "%02d:%02d:%02d %02d/%02d/%02d",  now.hour(), now.minute(), now.second(), now.day(), now.month(), now.year());
 /********************GSM Communication Starts********************/
 
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
  JsonObject& object = jsonBuffer.createObject(); // create a JSON Object.
  
  object.set("tag_ID",strID);
  object.set("timedate",t);
  
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
 
  /********************GSM Communication Stops********************/
 
}
 
 
void ShowSerialData()
{
  while (myserial.available() != 0)
    Serial.write(myserial.read());
  delay(1000);
 
}
