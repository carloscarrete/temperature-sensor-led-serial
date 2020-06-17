const int sensorPin = A4;                         //Entrada analógica del sensor
int red_light_pin = 11;                           //Entrada digital del PIN ROJO
int green_light_pin = 10;                         //Entrada digital del PIN VERDE
int blue_light_pin = 9;                           //Entrada digital del PIN AZUL
String color = "";                                //String utilizada para el Modo de configuración
unsigned long previousMillis = 0;                 // Tiempo previo de Millis para el Blinkeo en Modo configuración, se inicializa en 0
const long interval = 500;                        // Tiempo de blinkeo del LED cuando entre en Modo configuración
int edoNormalR = 0, edoNormalG = 255, edoNormalB = 0;
int edoTempR = 255, edoTempG = 255, edoTempB = 0;
int edoTempFR = 255, edoTempFG = 0, edoTempFB = 0;
int limiteTemperaturaAdvertencia = 30, limiteTemperaturaFallo = 40;
String modoconfig = "xdxd";
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
  setupMexico();
  if (Serial.available() > 0) {
    String bufferString = "";                     //Declaración de variable para buffer
    while (Serial.available() > 0) {
      bufferString += (char)Serial.read();
    }
    Serial.println(bufferString);                //Impresión del buffer de forma Serial
    color = bufferString;
    colorInicial(color);
  }
  setColor(edoNormalR, edoNormalG, edoNormalB);                        //Se inicializa el LED prendido en verde

  float  val = analogRead(sensorPin);
  float mv = ( val / 1024.0) * 5000;
  float cel = mv / 10;
  Serial.print(cel);
  Serial.println(" C");
  delay(1000);                                //Tiempo que tarda en aparecer los valores de la temperatura

  while (cel >= limiteTemperaturaAdvertencia) {
    while (cel >= limiteTemperaturaFallo) {
      val = analogRead(sensorPin);
      mv = ( val / 1024.0) * 5000;
      cel = mv / 10;
      setColor(edoTempFR, edoTempFG, edoTempFB); // Cyan
      Serial.println("Ya llego a 40");
      Serial.println(cel);
      delay(1000);
    }
    val = analogRead(sensorPin);
    mv = ( val / 1024.0) * 5000;
    cel = mv / 10;
    setColor(edoTempR, edoTempG, edoTempB); // Red
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

  if (color2.equals(modoconfig)) {
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
  Serial.println("Bienvenido a el modo de configuración");
  Serial.println("------------------------------------");
  Serial.println("1.- Cambiar el color de los diferentes estados");
  Serial.println("2.- Cambiar los valores limites de temperatura");
  Serial.println("3.- Cambiar la frase para entrar a el modo de configuración");


  bool selection = true;
  bool configuracion = true;
  bool internalConfiguracion = true;
  while (selection) {
    unsigned long currentMillis = millis();
    //Serial.println(currentMillis - previousMillis);
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

        Serial.println("CONFIGURACIÓN PARA CAMBIAR EL COLOR DE LOS ESTADOS");
        Serial.println("-------------------------------------");
        Serial.println("Seleccione el estado que desea configurar");
        Serial.println("1.-Estado Normal");
        Serial.println("2.-Estado de advertencia");
        Serial.println("3.-Estado de emergencia");
        while (configuracion) {
          if (Serial.available() > 0) {
            int entrada = Serial.read();
            if (entrada == '1') {
              mostrarColores();
              while (internalConfiguracion) {
                if (Serial.available() > 0) {
                  int entrada = Serial.read();
                  if (entrada == '1') {
                    edoNormalR = 0, edoNormalG = 255, edoNormalB = 255;
                    internalConfiguracion = false;
                    configuracion = false;
                    selection = false;
                  } else if (entrada == '2') {
                    edoNormalR = 255, edoNormalG = 255, edoNormalB = 255;
                    internalConfiguracion = false;
                    configuracion = false;
                    selection = false;
                  } else if (entrada == '3') {
                    edoNormalR = 255, edoNormalG = 0, edoNormalB = 255;
                    internalConfiguracion = false;
                    configuracion = false;
                    selection = false;
                  } else {
                    Serial.println("Seleccione un color válido, por favor");
                  }
                }
              }
              configuracion = false;
            } else if (entrada == '2') {
              mostrarColores();
              while (internalConfiguracion) {
                if (Serial.available() > 0) {
                  int entrada = Serial.read();
                  if (entrada == '1') {
                    edoTempR = 0, edoTempG = 255, edoTempB = 255;
                    internalConfiguracion = false;
                    configuracion = false;
                    selection = false;
                  } else if (entrada == '2') {
                    edoTempR = 255, edoTempG = 255, edoTempB = 255;
                    internalConfiguracion = false;
                    configuracion = false;
                    selection = false;
                  } else if (entrada == '3') {
                    edoTempR = 255, edoTempG = 0, edoTempB = 255;
                    internalConfiguracion = false;
                    configuracion = false;
                    selection = false;
                  } else {
                    Serial.println("Seleccione un color válido, por favor");
                  }
                }
              }
              configuracion = false;
            } else if (entrada == '3') {
              mostrarColores();
              while (internalConfiguracion) {
                if (Serial.available() > 0) {
                  int entrada = Serial.read();
                  if (entrada == '1') {
                    edoTempFR = 0, edoTempFG = 255, edoTempFB = 255;
                    internalConfiguracion = false;
                    configuracion = false;
                    selection = false;
                  } else if (entrada == '2') {
                    edoTempFR = 255, edoTempFG = 255, edoTempFB = 255;
                    internalConfiguracion = false;
                    configuracion = false;
                    selection = false;
                  } else if (entrada == '3') {
                    edoTempFR = 255, edoTempFG = 0, edoTempFB = 255;
                    internalConfiguracion = false;
                    configuracion = false;
                    selection = false;
                  } else {
                    Serial.println("Seleccione un color válido, por favor");
                  }
                }
              }
              configuracion = false;
            }
          }
        }

      }

      else if (input == '2') {
        int    input = Serial.read();
        Serial.println("CONFIGURACIÓN PARA CAMBIAR EL LÍMITE DE LAS TEMPERATURAS");
        Serial.println("-------------------------------------");
        Serial.println("Seleccione la temperatura que desea cambiar su límite de temperatura");
        Serial.println("1.-Temperatura de estado de emergencia");
        Serial.println("2.-Temperatura de estado de fallo");
        while (configuracion) {
          if (Serial.available() > 0) {
            int entrada = Serial.read();
            if (entrada == '1') {
              Serial.println("Introduzca el valor limite para el estado de advertencia");
              String bufferString = "";                     //Declaración de variable para buffer
              while (internalConfiguracion) {
                if (Serial.available() > 0) {
                  while (Serial.available() > 0) {
                    bufferString += (char)Serial.read();
                  }
                }
                if (bufferString.toInt() >= 30) {
                  internalConfiguracion = false;
                }
              }
              int valorFinal = bufferString.toInt();
              limiteTemperaturaAdvertencia = valorFinal;
              Serial.println(valorFinal);
              configuracion = false;
              internalConfiguracion = false;
              selection = false;
            } else if (entrada == '2') {
              Serial.println("Introduzca el valor limite para el estado de fallo");
              String bufferString = "";                     //Declaración de variable para buffer
              while (internalConfiguracion) {
                if (Serial.available() > 0) {
                  while (Serial.available() > 0) {
                    bufferString += (char)Serial.read();
                  }
                }
                if (bufferString.toInt() >= 30) {
                  internalConfiguracion = false;
                }
              }
              int valorFinal = bufferString.toInt();
              limiteTemperaturaFallo = valorFinal;
              Serial.println(valorFinal);
              configuracion = false;
              internalConfiguracion = false;
              selection = false;
            }
            configuracion = false;
          }
        }



      } else if (input == '3') {
        int    input = Serial.read();
        Serial.println(Serial.read());

        int contadorInterno = 0;
        Serial.println("CONFIGURACIÓN PARA CAMBIAR LA PALABRA PARA PODER ACCEDER AL MODO DE CONFIGURACIÓN");
        Serial.println("-------------------------------------");
        Serial.println("A continuación escriba la palabra que desea utilizar para acceder a el modo de configuración");
        String bufferString = "";
        char regres;
        while (internalConfiguracion) {
          if (Serial.available() > 0) {
            while (Serial.available() > 0) {
              bufferString += (char)Serial.read();
            //  regres = Serial.read();
                  String data = Serial.readStringUntil('\n');
                  

              Serial.println(bufferString+ data + " ES EL MEJOR");
            }
          } else {
            contadorInterno++;
         //   Serial.println(regres);
            if (regres == 13) {
         //   Serial.println(regres);
             internalConfiguracion = false;
            }
          }
        }
        Serial.println(bufferString);
        modoconfig = bufferString;
        configuracion = false;
        internalConfiguracion = false;
        selection = false;

      } else {

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


void mostrarColores() {
  Serial.println("-------------------------------");
  Serial.println("Seleccione el color que desea establecer para el estado del LED");
  Serial.println("1.-Cyan");
  Serial.println("2.-Blanco");
  Serial.println("3.-Magenta");

}
