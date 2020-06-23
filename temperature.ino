const int sensorPin = A4;                         //Entrada analógica del sensor
int red_light_pin = 11;                           //Entrada digital del PIN ROJO
int green_light_pin = 10;                         //Entrada digital del PIN VERDE
int blue_light_pin = 9;                           //Entrada digital del PIN AZUL
String color = "";                                //String utilizada para el Modo de configuración
unsigned long previousMillis = 0;                 // Tiempo previo de Millis para el Blinkeo en Modo configuración, se inicializa en 0
unsigned long previoMillis = 0 ;                   //Estado normal para evitar el delay()
const long interval = 500;                        // Tiempo de blinkeo del LED cuando entre en Modo configuración
const long intervalo = 1000;                      //El tiempo del parpadeo
int edoNormalR = 0, edoNormalG = 255, edoNormalB = 0;                    //Variables para el Estado Normal, se inicializa en verde
int edoTempR = 255, edoTempG = 255, edoTempB = 0;                       ///Variables para el Estado Emergencia, se inicializa en amarillo
int edoTempFR = 255, edoTempFG = 0, edoTempFB = 0;                      //Variables para el Estado Fallo, se inicializa en verde
int limiteTemperaturaAdvertencia = 30, limiteTemperaturaFallo = 40;     //Variables para el control de la temperatura, valores 30 y 40
String modoconfig = "xdxdxd";                                             //Variable para el modo de configuración, viene por default xdxdxd
bool cambioColor = true;
float val = 0;
float mv = 0;
float cel = 0;
int contador = 0;
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
  //DetectarConfiguracion();                                             //Se manda a llamar la función DetectarConfiguración
  //setColor(edoNormalR, edoNormalG, edoNormalB);                        //Se inicializa el LED prendido en verde
  EstadoNormal();
  //Tiempo que tarda en aparecer los valores de la temperatura
  //En este ciclo While se accederá una vez que se sobrepasa la temperatura del Estado Normal
  while (cel >= limiteTemperaturaAdvertencia) {
    unsigned long currentMillis = millis();
    setColor(edoTempR, edoTempG, edoTempB); // Yellow
    if (currentMillis - previoMillis >= intervalo) {
      DetectarConfiguracion();
      previoMillis = currentMillis;
      val = analogRead(sensorPin);
      mv = ( val / 1024.0) * 5000;
      cel = mv / 10;
      Serial.print(cel);
      Serial.println(" C");
    }
    while (cel >= limiteTemperaturaFallo) {
      unsigned long currentMillis = millis();
      setColor(edoTempFR, edoTempFG, edoTempFB); // RED
      if (currentMillis - previoMillis >= intervalo) {
        DetectarConfiguracion();
        previoMillis = currentMillis;
        val = analogRead(sensorPin);
        mv = ( val / 1024.0) * 5000;
        cel = mv / 10;
        Serial.print(cel);
        Serial.println(" C  FAL");
      }
    }
  }
}

void colorInicial(String color2) {
  setColor(edoNormalR, edoNormalG, edoNormalB); // Green
  if (color2.equals(modoconfig)) {
    bool flag = true;
    while (flag) {
      setupMexico();
      flag = false;
    }


  }
}

void setupMexico() {


  setColor(0, 0, 0); // Green
MainConfig:
  Serial.println("Bienvenido a el modo de configuración");
  Serial.println("-----------------------------------------------------");
  Serial.println("1.- Cambiar el color de los diferentes estados");
  Serial.println("2.- Cambiar los valores limites de temperatura");
  Serial.println("3.- Cambiar la frase para entrar a el modo de configuración");
  Serial.println("4.- Salir del modo de configuración");
  Serial.println("-----------------------------------------------------");
  bool selection = true;
  bool configuracion = true;
  bool internalConfiguracion = true;
  while (selection) {
    ParpadeoConfiguracion();                                                               //Se manda a llamar la función de parpadeo para el modo de configuración
    if (Serial.available() > 0) {
      int    input = Serial.read();
      if (input == '1') {
SeleccionDeEstados:
        Serial.println("CONFIGURACIÓN PARA CAMBIAR EL COLOR DE LOS ESTADOS");
        Serial.println("--------------------------------------------------");
        Serial.println("Seleccione el estado que desea configurar");
        Serial.println("1.-Estado Normal");
        Serial.println("2.-Estado de advertencia");
        Serial.println("3.-Estado de emergencia");
        Serial.println("4.-Salir de la configuración del LED");
        Serial.println("--------------------------------------------------");
        configuracion = true;
        while (configuracion) {
          ParpadeoConfiguracion();                                                      //Se manda a llamar la función de parpadeo para el modo de configuración
          if (Serial.available() > 0) {
            int entrada = Serial.read();
            if (entrada == '1') {
              mostrarColores();                                                         //Se manda llamar a la función de mostrar colores
              while (internalConfiguracion) {
                ParpadeoConfiguracion();                                                //Se manda a llamar la función de parpadeo para el modo de configuración
                if (Serial.available() > 0) {
                  int entrada = Serial.read();
                  if (entrada == '1') {
                    edoNormalR = 0, edoNormalG = 255, edoNormalB = 255;
                    goto SeleccionDeEstados;
                  } else if (entrada == '2') {
                    edoNormalR = 255, edoNormalG = 255, edoNormalB = 255;
                    goto SeleccionDeEstados;
                  } else if (entrada == '3') {
                    edoNormalR = 255, edoNormalG = 0, edoNormalB = 255;
                    goto SeleccionDeEstados;
                  }
                  else
                  {
                    Serial.println("Seleccione un color válido, por favor");
                  }
                }
              }
              configuracion = false;
            } else if (entrada == '2') {
              mostrarColores();                                                       //Se manda llamar a la función de mostrar colores
              while (internalConfiguracion) {
                ParpadeoConfiguracion();                                             //Se manda a llamar la función de parpadeo para el modo de configuración
                if (Serial.available() > 0) {
                  int entrada = Serial.read();
                  if (entrada == '1') {
                    edoTempR = 0, edoTempG = 255, edoTempB = 255;
                    goto SeleccionDeEstados;

                  } else if (entrada == '2') {
                    edoTempR = 255, edoTempG = 255, edoTempB = 255;
                    goto SeleccionDeEstados;
                  } else if (entrada == '3') {
                    edoTempR = 255, edoTempG = 0, edoTempB = 255;
                    goto SeleccionDeEstados;
                  } else {
                    Serial.println("Seleccione un color válido, por favor");
                  }
                }
              }
              configuracion = false;
            } else if (entrada == '3') {
              mostrarColores();                                                        //Se manda llamar a la función de mostrar colores
              while (internalConfiguracion) {
                ParpadeoConfiguracion();                                              //Se manda a llamar la función de parpadeo para el modo de configuración
                if (Serial.available() > 0) {
                  int entrada = Serial.read();
                  if (entrada == '1') {
                    edoTempFR = 0, edoTempFG = 255, edoTempFB = 255;
                    goto SeleccionDeEstados;
                  } else if (entrada == '2') {
                    edoTempFR = 255, edoTempFG = 255, edoTempFB = 255;
                    goto SeleccionDeEstados;
                  } else if (entrada == '3') {
                    edoTempFR = 255, edoTempFG = 0, edoTempFB = 255;
                    goto SeleccionDeEstados;
                  } else {
                    Serial.println("Seleccione un color válido, por favor");
                  }
                }
              }
            } else if (entrada == '4') {
              goto MainConfig;

            }
          }
        }

      }

      else if (input == '2') {
        int    input = Serial.read();
SeleccionDeTemperaturas:
        Serial.println("CONFIGURACIÓN PARA CAMBIAR EL LÍMITE DE LAS TEMPERATURAS");
        Serial.println("-------------------------------------------------------------------------");
        Serial.println("Seleccione la temperatura que desea cambiar su límite de temperatura");
        Serial.println("1.-Temperatura de estado de emergencia");
        Serial.println("2.-Temperatura de estado de fallo");
        Serial.println("3.-Salir de la configuración de temperaturas");
        Serial.println("-------------------------------------------------------------------------");
        internalConfiguracion = true;
        while (configuracion) {
          ParpadeoConfiguracion();                                                                      //Se manda a llamar la función de parpadeo para el modo de configuración
          if (Serial.available() > 0) {
            int entrada = Serial.read();
            if (entrada == '1') {
              Serial.println("Introduzca el valor limite para el estado de advertencia");
              String bufferString = "";                     //Declaración de variable para buffer
              while (internalConfiguracion) {
                ParpadeoConfiguracion();                                                               //Se manda a llamar la función de parpadeo para el modo de configuración
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
              Serial.println(valorFinal + "PUTITO");
              goto SeleccionDeTemperaturas;
            } else if (entrada == '2') {
              Serial.println("Introduzca el valor limite para el estado de fallo");
              String bufferString = "";                     //Declaración de variable para buffer
              while (internalConfiguracion) {
                ParpadeoConfiguracion();                                                              //Se manda a llamar la función de parpadeo para el modo de configuración
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
              goto SeleccionDeTemperaturas;
            } else if (entrada == '3') {
              goto MainConfig;
            } else {
              Serial.print("Digite una cantidad válida");
            }
            // configuracion = false;
          }
        }



      } else if (input == '3') {
        int    input = Serial.read();

        int contadorInterno = 0;
        Serial.println("CONFIGURACIÓN PARA CAMBIAR LA PALABRA PARA PODER ACCEDER AL MODO DE CONFIGURACIÓN");
        Serial.println("---------------------------------------------------------------------------------------------");
        Serial.println("A continuación escriba la palabra que desea utilizar para acceder a el modo de configuración");
        Serial.println("---------------------------------------------------------------------------------------------");
        String bufferString = "";
        char regres;
        while (internalConfiguracion) {
          ParpadeoConfiguracion();                                                            //Se manda a llamar la función de parpadeo para el modo de configuración
          if (Serial.available() > 0) {
            while (Serial.available() > 0) {
              bufferString += (char)Serial.read();
              //  regres = Serial.read();
              String data = Serial.readStringUntil('\n');
              bufferString += data;
              modoconfig = bufferString;
              internalConfiguracion = false;

            }
          } else {
            if (regres == 13) {
              //   Serial.println(regres);
            }
          }
        }
        configuracion = false;
        internalConfiguracion = false;
        selection = false;
        goto MainConfig;

      } else if (input = 4) {
        selection = false;
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


//Esta función lo que realiza es mostrar los posibles colores para configurar el LED
void mostrarColores() {
  Serial.println("-------------------------------");
  Serial.println("Seleccione el color que desea establecer para el estado del LED");
  Serial.println("1.-Cyan");
  Serial.println("2.-Blanco");
  Serial.println("3.-Magenta");

}
//Esta funcipon lo que realiza es ciclarse para estar siempre detectando de forma serial que se cumpla la cadena para poder acceder al modo de configuración
void DetectarConfiguracion() {
  if (Serial.available() > 0) {
    String bufferString = "";                     //Declaración de variable para buffer
    while (Serial.available() > 0) {
      bufferString += (char)Serial.read();
    }
    Serial.println(bufferString);                //Impresión del buffer de forma Serial
    color = bufferString;
    colorInicial(color);
  }
}
//Esta función lo que realiza es hacer que el parpade para avisarle al usuario de que esta en modo de configuración
void ParpadeoConfiguracion() {
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    cambioColor = !cambioColor;
    if (cambioColor == true) {
      setColor(255, 255, 255);
    } else {
      setColor(0, 0, 0);
    }
  }
}

void EstadoNormal() {
  unsigned long currentMillis = millis();
  //Serial.println("------");
  //Serial.println(contador);
  //Serial.println("-------");

  if (currentMillis - previoMillis >= intervalo) {
    DetectarConfiguracion();
    cambioColor = !cambioColor;
    //Serial.println(cambioColor);
    //  Serial.println(cambioColor);
    previoMillis = currentMillis;
    if (cambioColor == true) {
      setColor(edoNormalR, edoNormalG, edoNormalB);
    } else {
      setColor(0, 0, 0);
    }

    val = analogRead(sensorPin);
    mv = ( val / 1024.0) * 5000;
    cel = mv / 10;
    Serial.print(cel);
    Serial.println(" C");
  }


}
