const int buzzer = 15; //buzzer to arduino pin 9

void setup(){
  // pinMode(buzzer, OUTPUT); // Set buzzer - pin 9 as an output
  // digitalWrite(buzzer, HIGH);
//     tone( 8, 1000);   
// delay(500);
// // tone( 8, 20);
// // delay(500);
// noTone(8);
// digitalWrite(buzzer, HIGH);


  peep(1,1000);
  peep(1,1200);  
  peep(1,1500);
  stopSound();


}

void loop(){

  // peep(1,1000);
  // // // noTone(buzzer);
  // // // digitalWrite(buzzer, LOW);
  // delay(5000);
  // peep(1,1000);
  // // // digitalWrite(buzzer, LOW);
  // delay(5000);
  // peep(1,1000);


  // peep(1,1000);
  // peep(1,1200);  
  // peep(1,1500);

  // exit(0);
}

// void peep(int in,int hi){
//   for (int i = 0; i<in; i = i + 1){
//     tone(buzzer, hi);
//     delay(200);// ...for 1sec
//   }
//   digitalWrite(buzzer, HIGH);
// }

void peep(int in,int hi){
  for (int i = 0; i<in; i = i + 1){
    tone(buzzer, hi);
    delay(200);// ...for 1sec
  }
  // digitalWrite(buzzer, HIGH);
}
void stopSound(){
  noTone(buzzer);
  digitalWrite(buzzer, HIGH);
}