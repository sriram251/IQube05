#include "SPI.h"
#include "MFRC522.h"
MFRC522 rfid(10, 9);// Rx ,Tx
MFRC522::MIFARE_Key key;

void setup() {
  Serial.begin(9600);
  Serial.println("Initializing..........");
  SPI.begin();      // Init SPI bus
  mfrc522.PCD_Init();   // Init MFRC522
  delay(5000);
}

void loop() {
  
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
    Serial.println(strID);
    delay(1000);
}
