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
#include <AT24Cxx.h>

#define i2c_address 0x50
 
#define SS_PIN 10
#define RST_PIN 9
MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.

// String ar[10] = {};
bool flag_write = false;
const int max_card =800;//save 200 card * 4 bytes for each card = 200
bool history[max_card/4] = {};
byte active_card[max_card] = {};
int address = 0;
AT24Cxx eep(i2c_address, 8);
byte master_write[4] = {67, 233, 147, 174};
byte master_clear[4] = {35, 226, 216, 27};

 
void setup() 
{
  Serial.begin(9600);   // Initiate a serial communication
  SPI.begin();      // Initiate  SPI bus
  mfrc522.PCD_Init();   // Initiate MFRC522
  // Serial.println("Dua The Lai Gan Dau Doc...");
  // Serial.println();
  delay(500);
  clear_history();
  read_from_rom();

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
  // String content= " ";
  byte current_UID[mfrc522.uid.size] = {}; // mfrc522.uid.size = 4

  //uid of the current 13Mhz card is 4 byte. e.g., 43 E9 93 AE. Each byte is 8 bit.
  for (byte i = 0; i < mfrc522.uid.size; i++) 
  {
    //  Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
    //  Serial.print(mfrc522.uid.uidByte[i], HEX);
    //  content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? "0" : ""));
    //  content.concat(String(mfrc522.uid.uidByte[i], HEX));
    current_UID[i] = mfrc522.uid.uidByte[i];
  }

  print_UID(current_UID);
  // content.toUpperCase();
  // Serial.println(content.substring(1));

  
  // if (compare_UID(current_UID, master_write)){
  //   Serial.println("This is the master_write");
  // }
  // Serial.println("------");

  // Check if the card is the Master_Write
  if (flag_write == true && compare_UID(current_UID, master_write)){
      Serial.println("Hoan thanh viec ghi the");
      read_from_rom();
      flag_write = false;      
  }else if (flag_write == false && compare_UID(current_UID, master_write)){
      Serial.println("Bat dau ghi the");
      flag_write = true;      
  }else if (flag_write){
    if (find_UID_rom(current_UID) != -1){
      Serial.println("The da duoc luu");
    }else{      
      int add_te = find_empty_mem();
      if (add_te != -1){ // if there is an available slot in memory to write       
        Serial.print("Ghi the ");
        print_UID(current_UID);
        Serial.println(" vao bo nho");
        write_card(add_te, current_UID);
      }else{
        Serial.println("bo nho day, xoa the roi ghi lai sau");
      }
      // Serial.println("debug-----address:");
      // Serial.println(address);
    }    
  }else if(compare_UID(current_UID, master_clear)){
    // Check if the card is the master_delete
    // print_history();

    for (int i = 0; i < max_card/4; i = i + 1) {
      // Serial.println(history[i]);
      if (history[i] == false){
        byte temp[4] = {NULL, NULL, NULL, NULL};
        // byte temp[4] = {35, 226, 216, 27};
        write_card(i*4, temp);
      }
    }
    Serial.println("hoan thanh viec xoa the khong hoat dong");
    read_from_rom();
  }


  else{
    int temp = find_UID_rom(current_UID);
    if (temp != -1){
      history[temp/4] = true;
      Serial.println("DUOC Vao");    
    }else{
      Serial.println("KHONG Duoc Vao");
    }
  }
  delay(500);
} 

void write_card(int add, byte in[]){
  // Serial.println("write_card()");
  eep.write(add, in[0]);	
  eep.write(add+1, in[1]);
  eep.write(add+2, in[2]);
  eep.write(add+3, in[3]);

  active_card[add]= in[0];
  active_card[add+1]= in[1];
  active_card[add+2]= in[2];
  active_card[add+3]= in[3];
  // Serial.println(add);
  add += 4;

  if (add >= eep.length()-10){
		add = 0;
	}

  // eep.write(8000, add);
}

void read_from_rom(){
  Serial.println("coppying rom to ram...");
  for (int i = 0; i < max_card; i = i + 1) {    
    active_card[i] = eep.read(i);
    // Serial.println(active_card[i]);
  } 
  // address = eep.read(8000);
  Serial.println("ready");
}



// bool find_UID(String input){
//   for (int i = 0; i < 10; i = i + 1) {
//     if (input == ar[i]){
//       return true;      
//     }
//   }
//   return false;
// }

void clear_history(){
  Serial.println("delete history");
  for (int i = 0; i < max_card/4; i = i + 1) {
    history[i] = false;
  }
}

// void print_history(){
//   Serial.println("In lich su the");
//   for (int i = 0; i < max_card/4; i = i + 1) {
//     Serial.println(history[i]);
//   }
// }

void print_UID(byte in[]){
  Serial.print(in[0],HEX);
  Serial.print(in[1],HEX);
  Serial.print(in[2],HEX);
  Serial.println(in[3],HEX);
}

int find_UID_rom(byte in[]){
  
  // Serial.println(input);
  for (int i = 0; i < max_card; i = i + 4) {
    // Serial.println(active_card[i]);
    byte temp[4] = {active_card[i], active_card[i+1], active_card[i+2], active_card[i+3]};
    if (compare_UID(in, temp)){
      Serial.println("The da luu");
      return i;      
    }    
  }
  Serial.println("The chua luu");
  return -1;
}

int find_empty_mem(){
  // Serial.println("Tim vi tri trong bo nho");
  for (int i = 0; i < max_card; i = i + 4) {
    // Serial.println(active_card[i]);   
    if (active_card[i]==NULL && active_card[i+1]==NULL && active_card[i+2]==NULL && active_card[i+3]==NULL){
      Serial.println("slot available");
      return i;
    }    
  }
  Serial.println("Bo nho day");
  return -1;
}


bool compare_UID(byte in1[], byte in2[]){  
  if (in1[0] == in2[0] && in1[1] == in2[1] && in1[2] == in2[2] && in1[3] == in2[3]){
    return true;      
  }else{
    return false;
  }
}


