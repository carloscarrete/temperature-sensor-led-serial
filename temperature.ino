const int sensorPin = A4;
int red_light_pin = 11;
int green_light_pin = 10;
int blue_light_pin = 9;
String color = "";
int periodo = 1000;
unsigned long TiempoAhora = 0;
unsigned long timeToBlink = millis();
unsigned long previousMillis = 0;        // will store last time LED was updated

// constants won't change:
const long interval = 500;           // interval at which to blink (milliseconds)
void setup()
{
  Serial.begin(9600);
  pinMode(red_light_pin, OUTPUT);
  pinMode(green_light_pin, OUTPUT);
  pinMode(blue_light_pin, OUTPUT);

  pinMode(9, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(5, OUTPUT);

}

void loop()
{
  if (Serial.available() > 0) {
    //Se crea una variable que servirá como buffer
    String bufferString = "";
    while (Serial.available() > 0) {
      bufferString += (char)Serial.read();
    }
    //Se imprime el contenido del buffer
    Serial.println(bufferString);
    color = bufferString;
      colorInicial(color);
  }
    setColor(0, 255, 0); // Green

  float  val = analogRead(sensorPin);
  float mv = ( val / 1024.0) * 5000;
  float cel = mv / 10;

  //int value = analogRead(sensorPin);
  //float millivolts = (value / 1023.0) * 5000;
  //float celsius = millivolts / 10;
  Serial.print(cel);
  Serial.println(" C");
  delay(1000);

  while (cel >= 30) {
    while (cel >= 40) {
      val = analogRead(sensorPin);
      mv = ( val / 1024.0) * 5000;
      cel = mv / 10;
  RGB_color(0, 255, 255); // Cyan
      Serial.println("Ya llego a 40");
      Serial.println(cel);
      delay(1000);
    }
    val = analogRead(sensorPin);
    mv = ( val / 1024.0) * 5000;
    cel = mv / 10;
    RGB_color(255, 0, 0); // Red
    Serial.println("Ya llego a 30");
    Serial.println(cel);
    delay(1000);
  }
}

/*
  void loop() {
  RGB_color(255, 0, 0); // Red
  delay(1000);
  RGB_color(0, 255, 0); // Green
  delay(1000);
  RGB_color(0, 0, 255); // Blue
  delay(1000);
  RGB_color(255, 255, 125); // Raspberry
  delay(1000);
  RGB_color(0, 255, 255); // Cyan
  delay(1000);
  RGB_color(255, 0, 255); // Magenta
  delay(1000);
  RGB_color(255, 255, 0); // Yellow
  delay(1000);
  RGB_color(255, 255, 255); // White
  delay(1000);
  }
*/
void RGB_color(int red_light_value, int green_light_value, int blue_light_value)
{
  analogWrite(red_light_pin, red_light_value);
  analogWrite(green_light_pin, green_light_value);
  analogWrite(blue_light_pin, blue_light_value);
}

void colorInicial(String color2){
    setColor(0, 255, 0); // Green

  if(color2.equals("xdxd")){
//    Serial.println("HER");
// Escritura de PWM del color Rojo
//analogWrite(red_light_pin, 19);
// Escritura de PWM del color Verde
//analogWrite(green_light_pin, 222);
// Escritura de PWM del color Azul
//analogWrite(blue_light_pin, 240);
bool flag = true;
  while(flag){
      if (Serial.available() > 0) {
    //Se crea una variable que servirá como buffer
    char input = Serial.read();
    if(input=='1'){
      setupMexico();
      flag = false;
    }
  }
  }


}
}

void setupMexico(){


      setColor(0, 0, 0); // Green

  bool selection = true;
  while(selection){
    unsigned long currentMillis = millis();
    Serial.println(currentMillis-previousMillis);
      if (currentMillis - previousMillis >= interval) {
    // save the last time you blinked the LED
    previousMillis = currentMillis;

    // if the LED is off turn it on and vice-versa:
   // if (digitalRead(9==HIGH)&&digitalRead(10==HIGH)&&digitalRead(11==HIGH)) {
      digitalWrite(13,!digitalRead(13));
            digitalWrite(12,!digitalRead(12));

      digitalWrite(11,!digitalRead(11));

   // }if(digitalRead(9==LOW)&&digitalRead(10==LOW)&&digitalRead(11==LOW)){
      //Serial.println("AHAHAHAHAHAH");
    //  setColor(255,255,255);    
   // }

    // set the LED with the ledState of the variable:
   // digitalWrite(ledPin, ledState);
  }

   if (Serial.available()>0){
 
   int    input=Serial.read();
 
    if (input=='1'){
 
      Serial.println("QUE CHINGUE A SU REPUTISISISISISISISISISISISISISISISISISISISIMA MADRE RV");
 
    }
 
    else
 
    {
 
      Serial.println("QUE CHINGUE A SU REPUTISISISISISISISISISISISISISISISISISISISIMA MADRE EL AMÉRICA");
 
    }
  }
  
  }


 
 /* 
  while(selection){
    auxiliar = millis();
    while(TiempoAhora<=500){
    TiempoAhora = millis()-auxiliar;
    setColor(255,255,255);
    }
    TiempoAhora = 0;
    auxiliar=millis();
    while(TiempoAhora<=500){
    TiempoAhora = millis()-auxiliar;
    setColor(0,0,0);
    Serial.println(TiempoAhora + "  OFF");
    }
  }*/
}


void setColor(int red, int green, int blue)
{
  #ifdef COMMON_ANODE
    red = 255 - red;
    green = 255 - green;
    blue = 255 - blue;
  #endif
  analogWrite(red_light_pin, red);
  analogWrite(green_light_pin, green);
  analogWrite(blue_light_pin, blue);  
}
