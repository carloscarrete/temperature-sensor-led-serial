const int sensorPin = A4;                         //Entrada analógica del sensor
int red_light_pin = 11;                           //Entrada digital del PIN ROJO
int green_light_pin = 10;                         //Entrada digital del PIN VERDE
int blue_light_pin = 9;                           //Entrada digital del PIN AZUL
String color = "";                                //String utilizada para el Modo de configuración
unsigned long previousMillis = 0;                 // Tiempo previo de Millis para el Blinkeo en Modo configuración, se inicializa en 0
const long interval = 500;                        // Tiempo de blinkeo del LED cuando entre en Modo configuración
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
    String bufferString = "";                     //Declaración de variable para buffer
    while (Serial.available() > 0) {
      bufferString += (char)Serial.read();
    }
    Serial.println(bufferString);                //Impresión del buffer de forma Serial
    color = bufferString;
    colorInicial(color);
  }
  setColor(0, 255, 0);                          //Se inicializa el LED prendido en verde

  float  val = analogRead(sensorPin);
  float mv = ( val / 1024.0) * 5000;
  float cel = mv / 10;
  Serial.print(cel);
  Serial.println(" C");
  delay(1000);                                //Tiempo que tarda en aparecer los valores de la temperatura

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

void RGB_color(int red_light_value, int green_light_value, int blue_light_value)
{
  analogWrite(red_light_pin, red_light_value);
  analogWrite(green_light_pin, green_light_value);
  analogWrite(blue_light_pin, blue_light_value);
}

void colorInicial(String color2) {
  setColor(0, 255, 0); // Green

  if (color2.equals("xdxd")) {
    bool flag = true;
    while (flag) {
      if (Serial.available() > 0) {
        //Se crea una variable que servirá como buffer
        char input = Serial.read();
        if (input == '1') {
          setupMexico();
          flag = false;
        }
      }
    }


  }
}

void setupMexico() {


  setColor(0, 0, 0); // Green

  bool selection = true;
  while (selection) {
    unsigned long currentMillis = millis();
    Serial.println(currentMillis - previousMillis);
    if (currentMillis - previousMillis >= interval) {
      // save the last time you blinked the LED
      previousMillis = currentMillis;

      digitalWrite(13, !digitalRead(13));
      digitalWrite(12, !digitalRead(12));
      digitalWrite(11, !digitalRead(11));
    }

    if (Serial.available() > 0) {

      int    input = Serial.read();

      if (input == '1') {

        Serial.println("ESTAMOS EN EL NÚMERO 1");

      }

      else

      {

        Serial.println("ESTAMOS EN EL NÚMERO 2");

      }
    }

  }

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
