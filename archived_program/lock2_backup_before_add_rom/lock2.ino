/*
 * 
 * All the resources for this project: http://randomnerdtutorials.com/
 * Modified by Rui Santos
 * 
 * Created by FILIPEFLOP
 * 
 */
 
#include <SPI.h>
#include <MFRC522.h>
 
#define SS_PIN 10
#define RST_PIN 9
MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.

String ar[100] = {};
int idx = 0;
bool flag_write = false;

 
void setup() 
{
  Serial.begin(9600);   // Initiate a serial communication
  SPI.begin();      // Initiate  SPI bus
  mfrc522.PCD_Init();   // Initiate MFRC522
  Serial.println("Dua The Lai Gan Dau Doc...");
  Serial.println();
  // ar[0] = "23 E2 D8 1B";

}
void loop() 
{
  // Look for new cards
  if ( ! mfrc522.PICC_IsNewCardPresent()) 
  {
    return;
  }
  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) 
  {
    return;
  }
  //Show UID on serial monitor
  Serial.print("Ma UID:");
  String content= " ";
  // byte letter;
  for (byte i = 0; i < mfrc522.uid.size; i++) 
  {
    //  Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
    //  Serial.print(mfrc522.uid.uidByte[i], HEX);
     content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? "0" : ""));
     content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }


  content.toUpperCase();
  Serial.println(content.substring(1));

  // uint8_t decimal_answer = stoul("93AE", NULL, 16);
  // Serial.println(decimal_answer);


  // Check if the card is the Master_Write
  if (flag_write == true && content.substring(1) == "93AE"){
      Serial.println("hoan thanh viec ghi the");
      flag_write = false;      
  }else if (flag_write == false && content.substring(1) == "93AE"){
      Serial.println("Khoi dong viec ghi the");
      flag_write = true;      
  }else if (flag_write){
    if (check_UID(content.substring(1))){
      Serial.println("The da duoc luu");      
    }else{
      Serial.print("Ghi the ");
      Serial.print(content.substring(1));
      Serial.println(" vao bo nho");
      ar[idx] = content.substring(1);
      idx++;
    }    
  }

  // Check if the card is the master_delete

  else if (check_UID(content.substring(5))){
    Serial.println("Duoc Phep Vao");    
  }else{
    Serial.println("Khong Duoc Vao");
  }
  delay(500);
} 

bool check_UID(String input){
  // Serial.println("check_UID: input: ");
  // Serial.println(input);
  for (int i = 0; i < 100; i = i + 1) {
    if (input == ar[i]){
      return true;      
    }
  }
  return false;

}