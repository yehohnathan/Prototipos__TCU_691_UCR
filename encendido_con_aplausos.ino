/*
  Universidad de Costa Rica
  TC-691: Tropicalización de la tecnología
  Grupo: KidsTechCamp2024
  Proyecto: Encendido y apagado de un switch (120V) con aplausos 
  Estudiante: Yehohnathan Miranda Vigil - C04879

  Componentes utilizados:
    * Sensor de sonido (KY-038 fue el utilizado).
    * Modulo relé (preferible de 1 canal) con conexión a NO (normalmente
      abierto) a COM.
    * Una extensión sin tierra (puede ser casera o comercial).
    * Módulo Bluetooth (HC-05 fue el utilizado)
    * 1 Buzzer pasivo

  Precauciones:
    * No conectar la extensión sin verificar que funcione el sensor de sonido
      junto con el relé.
    * Una vez la entensión y el relé esten conectados. Al conectar la extensión
      al toma de la casa, no tocar los contactos del relé.
    * El relé no supera cargas superiores a 15A.

  Finalidad y uso:
    Se ha creado un interruptor inteligente utilizando tres componentes clave: un
    sensor de sonido, un módulo Bluetooth y un relé. Este interruptor es una extensión
    casera.

    El sensor de sonido permite que el interruptor se active o desactive con aplausos.
    El módulo Bluetooth ofrece otra forma de controlar el interruptor, enviando un comando
    a través de Bluetooth para activar o desactivarlo.

    El relé es el componente que recibe las señales del sensor de sonido o del módulo
    Bluetooth. Según la señal que reciba, el relé permitirá o interrumpirá el flujo de
    energía de la extensión casera, permitiendo que el interruptor inteligente funcione.

    Además, una pequeña bocina emite un sonido cada vez que el interruptor se enciende o
    se apaga. Esto proporciona una confirmación auditiva de que el interruptor está 
    funcionando correctamente.

*/

/*----------------------------- Inclusión de librerías ----------------------------*/
#include <SoftwareSerial.h> // Permite la creación de puertos seria virtuales en
                            // cualquier par de pines del arduino.

#include <SPI.h>            // Proporciona funciones para utilizar la interfaz de
                            // bus SPI. Ideal para comunicarse con SD.

#include <SD.h>             // Permite la comunicación con tarjetas SD,
                            // permitiendo  la lectura y escritura de las mismas.

SoftwareSerial BTSerial(10, 11);  // Pin 10 Rx y Pin 11 Tx
/*------------------------- Intanciación de las variables -------------------------*/
// Se asigna el pin en donde se colocará la señal del sensor de sonido
int sensor_sonido = 12;

// Se asigna el pin en donde se colocará la señal de control del relé
int rele_control = 5;

// Contador de aplausos para cambiar de estado al relé
int rele_estado = 0;

// Pin para conectar el buzzer pasivo
int pinBuzzer = 2;

// Es una bandera que verifica si uno una señal (plauso)
int signal_sensor = 0;

// Tiempo de retardo entre aplausos
int const delay_aplauso = 90;

// Variable para enviar comandos al BT
char comando[4]; 

/*--------------------------- Inicializador del programa --------------------------*/
void setup() {
  // Velocidad de transmisión de datos de  BT en baudios
  Serial.begin(9600);
  BTSerial.begin(9600);

  // Se asigna el pin del sensor de sonido como entrada, ya que desea que envie una
  // señal al arduino cuando se aplude
  pinMode(sensor_sonido, INPUT);

  // Se asigna el pin del relé como salida, ya que cuando se aplude hay que enviarle
  // una señal para que cambie de estado
  pinMode(rele_control, OUTPUT);
  digitalWrite(rele_control, HIGH);

  // Configurar el pin del buzzer como salida
  pinMode(pinBuzzer, OUTPUT);
}

/*
  ========================================================================
  Función que pregunta si se apludio, para luego cambiar de estado al relé.
  ========================================================================
*/
void control_encendido_apagado(){
  // Guarda en la variable la señal actual del sensor de sonido
  signal_sensor = digitalRead(sensor_sonido);

  // Si la señal esta en HIGH significa que hubo un aplauso
  if (signal_sensor == HIGH) {
    // Tono de encendido
    if (rele_estado == 0){tono_encendido();}

    // Se le manda una señal al relé para que se encienda
    digitalWrite(rele_control, LOW);

    // Como hubo una señal, se cambia el valor de sonido para cambiar el estado
    // del relé
    rele_estado += 1;

    // Se añade un leve retardo para que no se pueda apagar o encender el relé
    // repetidamente de forma muy rápida. Incluso para evitar el eco.
    delay(delay_aplauso);
  }

  // Como rele_estado esta verdadero, significa que hubo un aplauso
  if (rele_estado == 2) {
    // Tono de apagado
    tono_apagado();

    // Se le manda una señal al relé para que se apague
    digitalWrite(rele_control, HIGH);

    // Luego se cambia el valor al inicial de 0 aplausos
    rele_estado = 0;
  }
}

/*
  ========================================================================
  Función que pregunta si se mando un comando ON o OFF, para luego cambiar
  de estado al relé.
  ========================================================================
*/
void activador_BT(){
  if (BTSerial.available()) {
    BTSerial.readBytes(comando, 1); // El primer caracter (incluido el carácter nulo) en el buffer.
    comando[1] = '\0';              // Asegura que el array de caracteres tenga un carácter nulo al final.
                                    // De esta forma se leé el comando ingresado.
    Serial.println(comando);

    if (strcmp(comando, "O") == 0) {       // Muestra la lista de canciones
      // Se le manda una señal al relé para que se enciende
      if(digitalRead(rele_control) == HIGH){tono_encendido();}
      digitalWrite(rele_control, LOW);
      rele_estado = 1;
    }
    else if (strcmp(comando, "F") == 0) { 
      // Se le manda una señal al relé para que se apague
      if(digitalRead(rele_control) == LOW){tono_apagado();}
      digitalWrite(rele_control, HIGH);
      rele_estado = 0;
    }
    else if (strcmp(comando, "S") == 0) { 
      // Se le manda señal para que suene una canción
      sonido_escalado();
    }
  }
} 

/*
  ======================================================================
  Función que hace sonar un indicativo de encendido con el buzzer pasivo
  ======================================================================
*/
void tono_encendido(){
  // Reproducir el sonido de encendido
  for (int i = 1000; i <= 2000; i += 100) {
    tone(pinBuzzer, i); // Generar tono en el buzzer
    delay(50); // Duración del tono
  }
  
  // Detener el sonido
  noTone(pinBuzzer);
}

/*
  ====================================================================
  Función que hace sonar un indicativo de apagado con el buzzer pasivo
  ====================================================================
*/
void tono_apagado(){
  // Reproducir el sonido de encendido
  for (int i = 2000; i >= 1000; i -= 100) {
    tone(pinBuzzer, i); // Generar tono en el buzzer
    delay(50); // Duración del tono
  }
  
  // Detener el sonido
  noTone(pinBuzzer);
}

/*
  ==================================================================
  Función que hace sonar un indicativo de fallo con el buzzer pasivo
  ==================================================================
*/
void tono_fallo(){
  // Reproducir el sonido de fallo
  for (int i = 0; i < 3; i++) {
    // Reproducir tres tonos cortos y rápidos
    tone(pinBuzzer, 1000); // Tonos agudos
    delay(100);            // Duración del tono
    noTone(pinBuzzer);     // Detener el tono
    delay(50);             // Pausa entre tonos
  }
  
  // Reproducir un tono más largo para indicar el fallo
  tone(pinBuzzer, 400);    // Tono más bajo
  delay(500);              // Duración del tono
  noTone(pinBuzzer);       // Detener el tono
}

/*
  =======================================================================
  Función para indicar que se solicito una instrucción espacial con el BT
  =======================================================================
*/
void sonido_escalado(){
  int melody[] = {
    262, 294, 330, 349, 392, 440, 494,
    523, 587, 659, 698, 784, 880, 988,
    1047, 1175, 1319, 1397, 1568, 1760, 1976,
    2093, 2349, 2637, 2794, 3136, 3520, 3951
  };

  // Duración de cada nota en milisegundos
  int noteDuration = 250;

  for (int i = 0; i < sizeof(melody) / sizeof(melody[0]); i++) {
    tone(pinBuzzer, melody[i]); // Generar tono en el buzzer
    delay(noteDuration);        // Duración del tono
    noTone(pinBuzzer);          // Detener el tono
    delay(50);                  // Pausa entre notas
  }
}

/*
  ====================================================
  Función que repite funciones(tareas) indefinidamente
  ====================================================
*/

void loop() {
  // Función de control mediante aplausos
  control_encendido_apagado();

  // Función de control mediante comandos Bluetooth
  activador_BT();
}
