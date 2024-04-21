/*
  Universidad de Costa Rica
  TC-691: Tropicalización de la tecnología
  Grupo: KidsTechCamp2024
  Proyecto: Juego de luces
  Estudiante: Yehohnathan Miranda Vigil - C04879

  Componentes utilizados:
    * Arduino UNO
    * Botón genérico para corriente DC
    * 9 LEDs blancos
    * 9 resistencias de 82 Ohm
    * 1 pantalla LCD backlight 2x16
    * 1 buzzer pasivo
    * 1 sensor de sonido (opcional) (el juego con aplausos no funciona del todo bien)

  Dependencias:
    * Necesita de descargar una librería desde el repositorio de GitHub en el link:
    https://github.com/ELECTROALL/Codigos-arduino/blob/master/LiquidCrystal_I2C.zip

  Finalidad y uso:
    Se crea juego de iluminación interactiva utilizando LEDs blancos. Los LEDs se mueven
    en un patrón de zig-zag. Cuando el LED del centro se ilumina y el jugador presiona un
    botón, la velocidad de los LEDs aumenta. El juego continúa hasta que se alcanza el
    último nivel o el jugador se queda sin vidas. Se pierde una vida cada vez que el jugador
    no acierta al presionar el botón cuando el LED del centro está iluminado.

    Como premio, cada vez que el jugador avanza cinco niveles, se reproduce una canción, con
    un patrón de LEDs aleatorio asociado. Esto continúa hasta el nivel 18, que es el último
    nivel del juego.

    Además, si el jugador aplaude, se reproducirá una canción, sin importar en qué nivel se
    encuentre. Sin embargo, esto no sucede si el jugador está en la etapa final del juego,
    ya sea ganando o perdiendo.

*/

/*----------------------------- Inclusión de librerías ----------------------------*/
// Se incluye la librería para el uso de la LCD backlight con el protocolo i2c
#include <LiquidCrystal_I2C.h>

/*------------------------- Intanciación de las variables -------------------------*/
// Se asigna la dirección de la LCD para los 16 carácteres y 2 lineas
LiquidCrystal_I2C lcd(0x27,16,2);

// Se instancia una bandera de actualización del LCD para ahorrar recursos. True =  
// actualizar.
bool actualizar_LCD = true;

// Se instancia la variable para controlar el desplazamiento de los LEDs. Comienza
// en 2 porque corresponde al pin digital de inicio del arduino.
int LEDs_control = 2;

// Se instancia la cantidad de LEDs que serán utilizados
const int unidades_LEDS = 9;

// Límite inferior y límite superior de los LEDs. Estos números tienen que ver
// con los pines digitales del arduino
const int limite_superior_LED = 10;
const int limite_inferior_LED = 2;

// Pin que corresponderá al LED del centro
int const LED_central = 6;

// Tiempo de desplazamiento inicial de los LEDs
int time_delay = 550;

// Tiempo que se le restara a time_delay 500-25*(15niveles + 1)=150
int const time_sub = 25;
int const time_limit =100;

// Marca el modo de desplazamiento. True cuando va del pin 2 al 10, False de
// forma contraria
bool ascendencia_LEDS = true;

// Pin digital utilizado para el botón
const int i_boton = 13;

// Bandera de control, para indicar una pulsación
bool pulsacion_habilitada = true;

// Contador de vidas que tiene el usuario
int vidas = 10;

// Contador de nivel en el que esta el usuario
int nivel = 0;

// Se asigna el pin en donde se colocará la señal del sensor de sonido
int sensor_sonido = 11;

// Es una bandera que verifica si uno una señal (plauso)
int signal_sensor = 0;

// Tiempo de retardo entre aplausos
int const delay_aplauso = 40;

// Variable para controlar si se está esperando un aplauso
bool aplauso_en_curso = true;

// Variables para controlar el tiempo de los LEDS, boton y sensor, respectivamente.
unsigned long tiempo_anterior_LED = 0;
unsigned long tiempo_anterior_boton = 0;
const unsigned long intervalo_lectura_boton = 250; // Intervalo de lecturaen ms
unsigned long tiempo_inicio_aplauso = 0; // Variable para almacenar el tiempo de inicio del aplauso

// Define un arreglo de pines de LED para función de leds aleatorios
int leds[] = {2, 3, 4, 5, 6, 7, 8, 9, 10};

// Variable para habilitar canciones
bool canciones = false;

// Variable para habilitar una canción si se aplaudió
bool cancion_sensor_sonido = false;

// Nivel por canción
int const nivel_por_cancion = 5;  // Suena una canción si se llega a un multiplo de este valor

// variables para Música:
int pinBuzzer = 12;

int C__ =  261/4;
int Cs__=  277/4;
int D__ =  293/4 ;
int Ds__=  311/4;
int E__ =  329/4 ;
int F__ =  349/4 ;
int Fs__=  369/4;
int G__ =  391/4 ;
int Gs__=  415/4;
int A__ =  440/4 ;
int As__=  466/4;
int B__ =  493/4 ;
int C_ =  261/2;
int Cs_=  277/2;
int D_ =  293/2 ;
int Ds_=  311/2;
int E_ =  329/2 ;
int F_ =  349/2 ;
int Fs_=  369/2;
int G_ =  391/2 ;
int Gs_=  415/2;
int A_ =  440/2 ;
int As_=  466/2;
int B_ =  493/2 ;
int C =  261;
int Cs=  277;
int D =  293 ;
int Ds=  311;
int E =  329 ;
int F =  349 ;
int Fs=  369;
int G =  391 ;
int Gs=  415;
int A =  440 ;
int As=  466;
int B =  493 ;
int C2   =524;
int Cs2  =555;
int D2   =588;
int Ds2  =623;
int E2   =660;
int F2   =699;
int Fs2  =740;
int G2   =784;
int Gs2  =831;
int A_2   = 880;
int As2  =933;
int B2   =988;
int  C3   =524*2;
int Cs3   =555*2;
int  D3   =588*2;
int Ds3   =623*2;
int  E3   =660*2;
int  F3   =699*2;
int Fs3   =740*2;
int  G3   =784*2;
int Gs3   =831*2;
int A_3   =880*2;
int As3   =933*2;
int  B3   =988*2;
int  C4   =524*4;
int Cs4   =555*4;
int  D4   =588*4;
int Ds4   =623*4;
int  E4   =660*4;
int  F4   =699*4;
int Fs4   =740*4;
int  G4   =784*4;
int Gs4   =831*4;
int A_4   =880*4;
int As4   =933*4;
int  B4   =988*4;

#define C_2 65 // Do
#define D_2 73 // Re
#define E_2 82 // Mi
#define F_2 87 // Fa
#define G_2 98 // Sol
#define A_2 110 // La
#define B_2 123 // Si

int tempo = 112;  
int semi = 60000/( tempo * 4 *2);

int corch = 2*semi;
int negra = corch*2;
int np = corch*3;

int blanca = negra*2;
int redonda = blanca*2;
int rep = 3*negra;
int bnp = 3*negra+3*corch;

int b =blanca;
int r =2*b;
int n = negra;
int c = corch;
int s = semi;
int f = s/2;
int sf = f/2;

int cp = c+s;
int Sil = 10;

int retardo = 115;

int tttt= b+cp  - (c+s+c+c+f);

/*--------------------------- Inicializador del programa --------------------------*/
void setup() {
  // Se coloca como salida los pines que serán utilizados como salida para los
  // LEDs
  for(int i = 2; i < unidades_LEDS + 2; i++) {
    pinMode(i, OUTPUT);
  }

  // Se coloca como entrada el pin para registrar las pulsaciones del botón. Se
  // utiliza una resistencia de PULLUP para que cuando se presione el botón se
  // coloquen 0V, así leyendo una pulsación.
  pinMode(i_boton, INPUT_PULLUP);
  
  // Inicializador de la LCD backlight
  lcd.init(); // Le manda una señal de encendido
  lcd.init(); // Le manda una señal de encendido
  lcd.backlight();  // Enciende el led blanco
  lcd.clear();  // Limpia cualquier palabra que haya en la LCD

  // Se asigna el pin del sensor de sonido como entrada, ya que desea que envie una
  // señal al arduino cuando se aplude
  pinMode(sensor_sonido, INPUT);
}

/*--------------------------- Funciones para el programa --------------------------*/
/*
  Función para ingresar palabras en la LCD, para una sola linea o ambas
*/
void insertar_LCD_linea(int fila, int columna, String texto){
  lcd.setCursor(columna, fila); // Establecer la posición del cursor
  lcd.print(texto); // Imprimir el texto en la posición especificada
}
void insertar_LCD(String texto1, String texto2){
  lcd.setCursor(0, 0);
  lcd.print(texto1);
  lcd.setCursor(0, 1);
  lcd.print(texto2);
}

/*
  ---------------------------------------------------------------------------
  Patrón de LEDS cuando se sube de nivel. Simplemente sube escalonadamente la
  luz de los LEDS ida y vuelta, dos veces.
  ---------------------------------------------------------------------------
*/
void acierto_boton(){
  // Notas para la melodía de victoria
  int notas[] = {C, D, E, F, G, A, B, C2};
  int duracion = 200;

  // Reproduce la melodía en una "escalera ascendente"
  for (int i = 0; i < unidades_LEDS; i++) {
    // Enciende el LED correspondiente
    digitalWrite(i + 2, HIGH);

    // Toca la nota correspondiente
    nota(notas[i], time_delay/5);

    // Apaga el LED después de tocar la nota
    digitalWrite(i + 2, LOW);

    // Espera un breve tiempo antes de tocar la siguiente nota
    delay(time_delay/5);
  }

  // Reproduce la melodía en una "escalera descendente"
  for (int i = unidades_LEDS - 1; i >= 0; i--) {
    // Enciende el LED correspondiente
    digitalWrite(i + 2, HIGH);

    // Toca la nota correspondiente
    nota(notas[i], time_delay/5);

    // Apaga el LED después de tocar la nota
    digitalWrite(i + 2, LOW);

    // Espera un breve tiempo antes de tocar la siguiente nota
    delay(time_delay/5);
  }
}

/*
  ----------------------------------------------
  Patrón para los LEDS cuando se pierde una vida
  ----------------------------------------------
*/
void sin_acierto() {
  // Notas para la melodía de derrota
  int notas[] = {A_2, G_2, F_2, E_2, D_2, C_2, B_2, A_2, D_2};
  int duracion = 200; // Duración de cada nota

  // Reproduce la melodía descendente
  for (int i = 0; i < 9; i++) {
    // Enciende el LED correspondiente
    digitalWrite(i + 2, HIGH);

    // Toca la nota correspondiente
    nota(notas[i], time_delay/3);

    // Apaga el LED después de tocar la nota
    digitalWrite(i + 2, LOW);

    // Espera un breve tiempo antes de tocar la siguiente nota
    delay(time_delay/5);
  }
}

/* 
  --------------------------------------------------------------------------------
  Función para ir desplazando la luz de los LEDS en zig-zag (2->10 y 10->2 pines).
  Se utiliza un función millis para añadir un "delay" entre movimiento de los LEDS
  que no afecte el instante donde se precione el botón.
  --------------------------------------------------------------------------------
*/
void movimiento_LEDs() {
  // Se obtiene el tiempo actual desde el micro
  unsigned long tiempo_actual_LED = millis();

  // Si el tiempo transcurrido entre el paso de luz del LED X al Y es mayor o igual al
  // tiempo de espera actual, se procede a dar un nuevo paso de luz a otro LED
  if (tiempo_actual_LED - tiempo_anterior_LED >= time_delay) { // Verifica si ha pasado el tiempo necesario
    tiempo_anterior_LED = millis();   // Actualizar el tiempo anterior
    digitalWrite(LEDs_control, LOW);  // Se apaga el LED actual 

    // Se pregunta sobre cómo sigue el patrón de encendido (2 al 10 o 10 al 2), luego se continúa con el
    // siguiente LED (pin con true) o el LED anterior (pin con false).
    if (ascendencia_LEDS == true) {
      LEDs_control += 1;
      // Si se alcanzó el último LED (pin 10) se cambia el patrón
      if (LEDs_control == limite_superior_LED) {
        ascendencia_LEDS = false;
      }
    } else {
      LEDs_control -= 1;
      // Si se alcanzó el primer LED (pin 2) se cambia el patrón
      if (LEDs_control == limite_inferior_LED) {
        ascendencia_LEDS = true;
      }
    }

    // Se enciende el nuevo LED controlado
    digitalWrite(LEDs_control, HIGH);

    // Generar sonido
    if (LEDs_control == LED_central) {
      // Si el LED es el LED central (6), hacer un sonido diferente
      nota_sf(A, s);  // Sonido diferente
    } else {
      // Si el LED no es el LED central, hacer el sonido estándar
      nota_sf6(C, s);  // Sonido estándar
    }
  }
}

/*
  -------------------------------------------------------------------------------------------
  Función registrar las pulsaciones del botón, verificando si se sube de nivel (acierto)
  o se pierde una vida (no se toco en el led central). Se usa millis para no generar retrasos
  a la hora de accionar el LED
  -------------------------------------------------------------------------------------------
*/
void tocar_boton() {
  // Obtener el tiempo actual para la lectura del botón
  unsigned long tiempo_actual_boton = millis();

  // Verificar si ha pasado el intervalo necesario y si hay una señal en alto en el pin 
  // del boton (por si el usuario deja el botón presionado), y activar la bandera.
  if(tiempo_actual_boton - tiempo_anterior_boton >= intervalo_lectura_boton && digitalRead(i_boton) == HIGH) {
    pulsacion_habilitada = true;
    tiempo_anterior_boton = millis();; // ctualizar el tiempo anterior de lectura del botón
  }

  // Si el botón está presionado y se ha registrado una pulsación previamente
  if(digitalRead(i_boton) == LOW && pulsacion_habilitada) {

    // Llama a la función
    gana_pierde();

    // Una vez presionado el boton se necesita una nueva validación
    pulsacion_habilitada = false;
  }
}

/*
  -------------------------------------------------------------------------------------------
  Función registrar laos aplausos, verificando si se sube de nivel (acierto)
  o se pierde una vida (no se toco en el led central). Se usa millis para no generar retrasos
  a la hora de accionar el LED
  -------------------------------------------------------------------------------------------
*/
void aplaudir() {
  // Guarda en la variable la señal actual del sensor de sonido
  signal_sensor = digitalRead(sensor_sonido);

  // Si la señal está en HIGH significa que hubo un aplauso
  if (signal_sensor == HIGH) {
    // Verifica si no hay un aplauso en curso para evitar la repetición
    if (!aplauso_en_curso) {
      // Inicia el temporizador del aplauso
      tiempo_inicio_aplauso = millis();
      aplauso_en_curso = true;
      
      // Llama a la función
      cancion_sensor_sonido = true;
    }
  } else {
    // Si no hay señal de aplauso, verifica si ha pasado el tiempo de espera
    if (aplauso_en_curso && millis() - tiempo_inicio_aplauso >= delay_aplauso) {
      aplauso_en_curso = false; // Se restablece el estado del aplauso
    }
  }
}

/*
  Función para decidir si se sube de nivel o pierdes una vida: se envio una señal
  justo en el LED del centro se sube de nivel, sino se pierde una vida y se continua
  en el mismo nivel.
*/
void gana_pierde(){
  // Si se ingresó aquí es porque se modificará la LCD
  actualizar_LCD = true;

  if (LEDs_control == LED_central) {
    // Habilita la posibilidad de activar una canción
    canciones = true;

    // Se muestra información en la línea superior del LCD
    insertar_LCD_linea(0, 0, "==> LEVEL UP <==");

    // Se incrementa en uno la variable de nivel
    nivel += 1;

    // Se decrementa el tiempo entre LEDs
    time_delay -= time_sub;

    // Se accede al patrón de LEDs de acierto
    acierto_boton();
  } 
  else {
    insertar_LCD_linea(0, 0, "===> -1 vida <==");
    // Se decrementa en uno la variable de vida
    vidas -= 1;

    // Se accede al patrón de LEDs de pérdida
    sin_acierto();
  }
}

/* 
  -----------------------------------------------
  Función que actualiza el nivel/vida del jugador
  ----------------------------------------------
*/
void actualiza_juego(){
  char texto[15];
  sprintf(texto, "VIDA:%02d NIVEL:%02d", vidas, nivel);
  insertar_LCD("Aplaude/presiona", texto);
}

/* 
  -----------------------------------------
  Función que verifica si el juego se acabo
  -----------------------------------------
*/
void pregunta_final_juego(){
  if(vidas == 0){
    // Mensaje de perder en el LCD
    insertar_LCD_linea(0, 0, "FIN DEL JUEGO :(");
    delay(1000);
    // Se repite indefinidamente el patron de leds
    while(1){
      treinta_y_un_minutos();
    };
  }
  else if(time_delay == time_limit){
    // Mensaje de ganar en el LCD
    insertar_LCD_linea(0, 0, "FIN DEL JUEGO :D");
    delay(1000);

    // Se repite indefinidamente el patron de leds
    while(1){
      treinta_y_un_minutos();
    };
  }
}

/*
  -----------------------------------------------------------------------------------
  Función para encender aleatoriamente un LED evitando cambios consecutivos de estado
  -----------------------------------------------------------------------------------
*/
void encender_led_aleatorio() {
  // Apagar todos los LEDs
  for (int i = 0; i < unidades_LEDS; i++) {
    digitalWrite(leds[i], LOW);
  }

  // Encender un LED aleatorio
  int led_aleatorio = random(unidades_LEDS);
  digitalWrite(leds[led_aleatorio], HIGH);
}

/* 
  ----------------------------------------------
  Función para seleccionar música según el nivel
  ----------------------------------------------
*/
void cancion_niveles(){
  if ((nivel%nivel_por_cancion == 0 && canciones == true) || cancion_sensor_sonido == true){
    switch (random(0, 5)) {
      case 0:
        peaches_song();
        break;
      case 1:
        cuarteto_nos();
        break;
      case 2:
        popcorn();
        break;
      case 3:
        miranda_don();
        break;
      case 4:
        the_trooper();
        break;
      case 5:
        belanueva();
    }
    
    if (cancion_sensor_sonido == true){
      cancion_sensor_sonido = false;
    }
  }
  canciones = false;
}

/*-------------- Función que repite funciones(tareas) indefinidamente -------------*/
void loop() {
  // Se accede a la función que le da movimiento de zigzag a los LEDS
  movimiento_LEDs();

  // Se accede a la función y verifica si se apludió
  aplaudir();

  // Se accede a la función que verifica si se tocó el botón
  tocar_boton();

  // Se accede a la función para actualizar la estadistica del jugador
  if (actualizar_LCD = true){
    actualiza_juego();
    actualizar_LCD = false;
  }

  // Pone canciones según el nivel cursado o si hubo un aplauso
  cancion_niveles();

  // Se accede a la fuinción que se pregunta si se terminó el juego
  pregunta_final_juego();
}

/*--------------------------- Funciones de notas musicales ------------------------*/
void nota_sf6(int N ,int d ){

  int  ttt = sf/2;
  nota( N*0.70710678118 , ttt);
  nota( N*0.74915353843 , ttt);
  nota( N*0.79370052598 , ttt);
  nota( N*0.84089641525 , ttt);
  nota( N*0.89089871814 , ttt);
  nota( N*0.94387431268 , ttt);
  nota( N, d - 6*ttt);
}

void tocar_nota_sf6(int note, int duracion) {
  nota_sf6(note,duracion);
  encender_led_aleatorio();
}


void nota_sf(int N ,int d ){

  int  ttt = sf/2;
    
  nota( N*0.84089641525 , ttt);
  nota( N*0.89089871814 , ttt);
  nota( N*0.94387431268 , ttt);
  nota( N, d - 3*ttt);
}

void tocar_nota_sf(int note, int duracion) {
  nota_sf(note,duracion);
  encender_led_aleatorio();
}


void nota(int nota, int duracion){
  tone(pinBuzzer,nota, duracion);
  delay(duracion);
  noTone(pinBuzzer);
  delay(duracion);
}

void tocar_nota(int note, int duracion) {
  nota(note,duracion);
  encender_led_aleatorio();
}

/*------------------------------- Funciones musicales -----------------------------*/
/*
  ------------------------------------------------------
  Musica del cuarteto de nos: Contrapunto de Hombre y PC
  ------------------------------------------------------
*/
void cuarteto_nos(){
  insertar_LCD("CUARTETO DE NOS:","CONTRAPUNTO...  ");
  Intro2_cuarteto();

  verso1_cuarteto();
  verso2_cuarteto();
  verso1_cuarteto();
  verso2_cuarteto();
  verso1_cuarteto();
  verso2_cuarteto();
  verso1_cuarteto();
  verso2_cuarteto();
  verso3_cuarteto();
  verso4_cuarteto(); 


  Intro1_cuarteto();

  Intro1_cuarteto();
  Intro2_cuarteto();
  Intro2_cuarteto();


  Intro1_cuarteto();
  Intro1_cuarteto();
  Intro2_cuarteto();
  Intro2_cuarteto();
  

  // Apagar todos los LEDs
  for (int i = 0; i < unidades_LEDS; i++) {
    digitalWrite(leds[i], LOW);
  }
}

void Intro1_cuarteto(){
  tocar_nota(D2,s);
  tocar_nota(D3,s);

  tocar_nota(F2,s);
  tocar_nota(D3,s);

  tocar_nota(A_2,s);
  tocar_nota(F2,s);
  tocar_nota(D3,s);
  tocar_nota(A_2,s);

}

void Intro2_cuarteto(){
  tocar_nota(E2,s);
  tocar_nota(E3,s);
  tocar_nota(G2,s);
  tocar_nota(E3,s);

  tocar_nota(As2,s);
  tocar_nota(G2,s);
  tocar_nota(E3,s);
  tocar_nota(As2,s);

}


void verso1_cuarteto(){
  tocar_nota(D__,s);
  tocar_nota(F,s);
  tocar_nota(D,s);
  tocar_nota(A_,s);

  tocar_nota(A,c);
  tocar_nota(F,c);

  tocar_nota(As__,s);
  tocar_nota(F,s);
  tocar_nota(D,s);
  tocar_nota(A_,s);
  tocar_nota(A,c);
  tocar_nota(F,c);
}

void verso2_cuarteto(){
  tocar_nota(E__,s);
  tocar_nota(G,s);
  tocar_nota(E,s);
  tocar_nota(Cs,s);

  tocar_nota(As,c);
  tocar_nota(G,c);

  tocar_nota(As__,s);
  tocar_nota(G,s);
  tocar_nota(E,s);
  tocar_nota(Cs,s);

  tocar_nota(As,c);
  tocar_nota(G,c);
}


void verso3_cuarteto(){
  tocar_nota(D__,s);
  tocar_nota(F,s);
  tocar_nota(D,s);
  tocar_nota(A_,s);

  tocar_nota(A,c);
  tocar_nota(F,c);


  tocar_nota(G__,s);
  tocar_nota(G,s);
  tocar_nota(D,s);
  tocar_nota(As_,s);

  tocar_nota(As,c);
  tocar_nota(G,c);
}

void verso4_cuarteto(){
  tocar_nota(F__,s);
  tocar_nota(F,s);
  tocar_nota(D,s);
  tocar_nota(A_,s);

  tocar_nota(A,c);
  tocar_nota(F,c);

  tocar_nota(E__,s);
  tocar_nota(E,s);
  tocar_nota(Cs,s);
  tocar_nota(As_,s);

  tocar_nota(G,s);
  tocar_nota(E,s);
  tocar_nota(Cs,s);
  tocar_nota(A_,s);
}

/*
  -------------------------------
  Musica de Belanova: Rosa Pastel
  -------------------------------
*/
void belanueva(){
  insertar_LCD("CANCION DE:     ", "    BELANOVA    ");

  NoNoquieroSer();
  TuNoeres();
  YquenoQueda();
  YoTeolvidare();

  // Apagar todos los LEDs
  for (int i = 0; i < unidades_LEDS; i++) {
    digitalWrite(leds[i], LOW);
  }
}
void NoNoquieroSer(){
  insertar_LCD("No, no quiero   ","ser             ");
  tocar_nota_sf(A,n);
  tocar_nota(B_,c);
  tocar_nota(Fs,n);
  tocar_nota(B,c-f);
  tocar_nota(A,c+f);
  tocar_nota(A,np);
  tocar_nota(Fs_,c);
  insertar_LCD("esa mujer       ","                ");
  tocar_nota(Fs,n);
  tocar_nota(B,c-f);
  tocar_nota(A,c+f);
  tocar_nota(A,np);
  tocar_nota(G_,c);
  insertar_LCD("Ella se fue a   ","un abismo       ");
  tocar_nota_sf(Fs,c);
  tocar_nota(Fs,cp-f);
  tocar_nota(G ,cp+f);
  tocar_nota(A ,c  + c);

  tocar_nota(G,n);
  tocar_nota(Fs,n);
  tocar_nota(D,np);
}
void TuNoeres(){

  insertar_LCD(" /               ","Tu no eres aquel");
  tocar_nota_sf(A,n);
  tocar_nota(B_,c);
  tocar_nota(Fs,n);
  tocar_nota(B,c-f);
  tocar_nota(A,c+f);
  tocar_nota(A,np);
  tocar_nota(Fs_,c);
  insertar_LCD("           /    ","que prometio    ");
  tocar_nota(Fs,n);
  tocar_nota(B,c-f);
  tocar_nota(A,c+f);
  tocar_nota(A,np);
  tocar_nota(G_,c);

  insertar_LCD("   /           ","SerIa           "); 

  tocar_nota_sf(B_,c);
  tocar_nota(B_,cp-f);
  insertar_LCD("         /     ","mi superheroe   ");
  tocar_nota(Cs ,cp+f);
  tocar_nota(D , c+ c);

  tocar_nota(E,n);
  tocar_nota(Fs,n);
  tocar_nota(G,np);
}

void YquenoQueda(){

  insertar_LCD("               /","Y que todo acabo");
  //nota(Fs,c);

  tocar_nota(Fs,n);
  tocar_nota(B_,c);

  tocar_nota(E,n);
  tocar_nota(G,c-f);
  tocar_nota(Fs,c+f);
  tocar_nota(Fs,np);

  tocar_nota(Fs_,c);
  insertar_LCD("             /  ","    no queda mas");
  tocar_nota(E,n);
  tocar_nota(G,c-f);
  tocar_nota(Fs,c+f);
  tocar_nota(Fs,np);

  tocar_nota(G_,c);
  insertar_LCD("seremos dos  _  ","        extranos");
  tocar_nota_sf(D,c);
  tocar_nota(D,cp-f);
  tocar_nota(E ,cp+f);
  tocar_nota(Fs ,c + c);

  tocar_nota(G,n);
  tocar_nota(Fs,n);
  tocar_nota(E,np);
}
void YoTeolvidare(){
  insertar_LCD("              /  "," Yo te olvidare ");
  nota_sf(Fs,n);
  tocar_nota(B_,c);
  tocar_nota(E,n);
  tocar_nota(G,c);
  tocar_nota(Fs,n);
  tocar_nota(Cs,n);
  tocar_nota(Fs_,c);
 
  
  insertar_LCD("          /     ","Me olvidaras    ");
  tocar_nota(Cs,n);
  tocar_nota(G,c);
  tocar_nota(Fs,n);
  tocar_nota(B_,n);
  tocar_nota(G__,np);
  insertar_LCD("Hasta nunca...  ","                ");
  tocar_nota(B_,c);
  tocar_nota(B_,n);
  tocar_nota_sf(A,b);
  tocar_nota(Cs,b);
  tocar_nota(Sil,cp);
  tocar_nota(Sil,cp);
  tocar_nota(Sil,cp);
  tocar_nota(Sil,cp);
  tocar_nota(Sil,cp);
}

/*
  -------------------------
  Musica de Bowser: Peaches
  -------------------------
*/
void peaches_song() {
  Peaches();
  ILoveYou();
  Oh1();
  Peaches();
  ILoveYou();
  Oh2();
  MarioLuigi();
  AThousandtroops();
  Princess();

  Peaches();
  ILoveYou();
  Oh2();
  Fin();
  
  // Apagar todos los LEDs
  for (int i = 0; i < unidades_LEDS; i++) {
    digitalWrite(leds[i], LOW);
  }
}

void Peaches(){
  insertar_LCD("Peaches Peaches ","Peaches Peaches ");
  tocar_nota(As ,c);
  tocar_nota(Cs2,c);

  tocar_nota(As ,c);
  tocar_nota(Cs2,s);

  tocar_nota(As ,c);
  tocar_nota(Cs2,s);

  tocar_nota(As ,s);
  tocar_nota(Cs2,s);

  insertar_LCD("    Peaches     ","                ");
  tocar_nota(As ,c);
  tocar_nota(Cs2,c);

  insertar_LCD("Peaches Peaches ","Peaches Peaches ");

  tocar_nota(C2,c);
  tocar_nota(Ds2,c);

  tocar_nota(C2,c);
  tocar_nota(Ds2,s);

  tocar_nota(C2 ,c);
  tocar_nota(Ds2,s);

  tocar_nota(C2 ,s);
  tocar_nota(Ds2,s);

  insertar_LCD("    Peaches     ","                ");
  tocar_nota(C2 ,c);
  tocar_nota(Ds2,c);
}

void ILoveYou(){
  insertar_LCD("I love you      ","                ");
  tocar_nota_sf6(F2,np);
  tocar_nota(Gs2,n);
  tocar_nota(As2,c);
  tocar_nota(Sil,c);

  tocar_nota( E2,c);
  tocar_nota(Ds2,s);
  tocar_nota(Cs2,s);
  tocar_nota(As,c);
  tocar_nota(Sil,c);

}
void Oh1(){
  insertar_LCD("  Oh!           ","                ");
	tocar_nota(Ds2,n);
	tocar_nota( F2,np);

}

void Oh2(){
  insertar_LCD("  Oh!           ","                ");
	tocar_nota(F2,s);
	tocar_nota(Ds2,s);
	tocar_nota(Cs2,s);
	tocar_nota(Ds2,s);
	tocar_nota(F2,np);

}

void MarioLuigi(){
  insertar_LCD("Mario Luigi and ","a DonkeyKong too");
	tocar_nota(As,c);
	tocar_nota_sf(As,c);
	tocar_nota(As,c);
	tocar_nota(As,c);

	tocar_nota(As,c);
	tocar_nota(As,c);
	tocar_nota(Cs2,c);
	tocar_nota(Gs,c);

	tocar_nota(C2,c);
	tocar_nota(C2,c);
	tocar_nota(As,c);
	tocar_nota(As,s);
	tocar_nota(C2,s);
	tocar_nota(Gs,n);
	tocar_nota(Sil,c);
}
void AThousandtroops(){
  insertar_LCD("A thousand      ","troops of koopas");
	tocar_nota(Gs,c);

	tocar_nota(F2,c);
	tocar_nota(F2,c);
	tocar_nota(Ds2,c);
	tocar_nota(Ds2,c);

	tocar_nota(Ds2,c);
	tocar_nota(F2,c);

  insertar_LCD("Could'nt keep me","from you        ");
	tocar_nota(F2,c);
	tocar_nota(Cs2,c);

	tocar_nota(Cs2,c);
	tocar_nota(Cs2,c);
	tocar_nota(C2,c);
	tocar_nota(C2,s);
	tocar_nota(Cs2,s);
	tocar_nota(As,n);
	tocar_nota(Sil,n);
}

void Princess(){
  insertar_LCD("Princess Peach, ","                ");
	tocar_nota(As,c);
	tocar_nota(As,c);
	tocar_nota(Sil,c);
	tocar_nota_sf(As,c);
	tocar_nota(Sil,c);
	tocar_nota(Sil,c);

  insertar_LCD("At the end of   ","the line        ");
	tocar_nota(Cs2,c);
	tocar_nota(As,c);
	tocar_nota(C2,c);
	tocar_nota(C2,c);
	tocar_nota(As,c);
	tocar_nota(As,n);
	tocar_nota(Gs,c);
	tocar_nota(Sil,n);

  insertar_LCD("I'll make you   ","mine            ");
	tocar_nota_sf6(F2,np);
	tocar_nota_sf(Gs2,n);
	tocar_nota(As2,c);
	tocar_nota(Sil,c);
	tocar_nota_sf6(Ds2,cp);
	tocar_nota(Cs2,s);
	tocar_nota(As,c);
	tocar_nota(Sil,c);
	tocar_nota(Sil,c);
	tocar_nota(Sil,c);

  insertar_LCD("Oh              ","                ");
	tocar_nota_sf6(F2,np);
}
  
void Fin(){
  insertar_LCD("Peaches         ","                ");
	tocar_nota(E2,s);
	tocar_nota(Ds2,c);
	tocar_nota(Cs2,s);
	tocar_nota(Ds2,c);
	tocar_nota(F2,c);
	tocar_nota(Sil,n);

  insertar_LCD("Peaches         ","                ");
	tocar_nota_sf6(Gs2,n);
	tocar_nota(Cs2,c);
	tocar_nota(Ds2,c);
	tocar_nota(Cs2,c);

	tocar_nota(Sil,c);
	tocar_nota(Sil,n);
  insertar_LCD("    PEACH       ","                ");

	tocar_nota_sf6(F2,n+b);
	tocar_nota(Sil,n);
  insertar_LCD("                ","    PEACH       ");

	int ttt=b/15;
	tocar_nota_sf6(Gs2,n);
	tocar_nota(G2,ttt);
	tocar_nota(Fs2,ttt);
	tocar_nota(F2,ttt);
	tocar_nota(E2,ttt);
	tocar_nota(Ds2,ttt);
	tocar_nota(D2,ttt);
	tocar_nota(Cs2,ttt);
	tocar_nota(C2,ttt);
	tocar_nota(B ,ttt);
	tocar_nota(As,ttt);
	tocar_nota(A,ttt);
	tocar_nota(Gs,ttt);
	tocar_nota(G,ttt);
	tocar_nota(Fs,ttt);
	tocar_nota(F,ttt);
	tocar_nota(Sil,n);
}

/*
  -----------------------
  Musica de Miranda: Don!
  -----------------------
*/
void miranda_don(){
  tocar_nota(Sil,c);
  Intro();
  Intro();
  tocar_nota(Sil,n);
  insertar_LCD("      O O       ",         "       U        ");         
  tocar_nota(Sil,3*c);
  QuieroSaberQueMePasa();
  ConMisIdas();

  // Apagar todos los LEDs
  for (int i = 0; i < unidades_LEDS; i++) {
    digitalWrite(leds[i], LOW);
  }
}
void Intro(){
  tocar_nota(Sil,c);
  tocar_nota(Sil,c);
  tocar_nota(Sil,c);

  insertar_LCD(" Don            ","      Miranda :D");         
  tocar_nota(Cs2,c);
  tocar_nota(As ,c);
  tocar_nota(Cs2,c);
  tocar_nota(Sil,c);
  tocar_nota(Sil,c);
  tocar_nota(Sil,c);

  insertar_LCD("  Don           ","     Miranda  :)");         
  tocar_nota(Cs2,c);
  tocar_nota(As ,c);
  tocar_nota(Fs2,c);
  tocar_nota(Sil,c);
  tocar_nota(Sil,c);
  tocar_nota(Sil,c);

  insertar_LCD("   Don          ","    Miranda   :P");         
  tocar_nota(Cs2,c);
  tocar_nota(As ,c);
  tocar_nota(As2,c);
  tocar_nota(Sil,c);
  tocar_nota(Sil,c);

  insertar_LCD("    Don         ","   Miranda    :O");         
  tocar_nota(As2,c);
  tocar_nota(Cs2,c);
  tocar_nota(Gs2,c);
  tocar_nota(Fs2,c);

  }
void QuieroSaberQueMePasa(){
  insertar_LCD("                ","Quiero saber    ");         
  tocar_nota(Cs2,c);
  tocar_nota(Cs2,c);
  tocar_nota(Cs2,c);
  tocar_nota(Cs2,c);

  insertar_LCD("  /             ","que me pasa     ");         
  tocar_nota( B,c);
  tocar_nota(As,c);
  tocar_nota( B ,c);
  tocar_nota(Sil,c);
  tocar_nota(As ,n);

  insertar_LCD("                ","Te pregunto     ");         
  tocar_nota(Cs2,c);
  tocar_nota(As ,c);
  tocar_nota( B ,c);
  tocar_nota(Sil,c);
  tocar_nota(As ,n);

  insertar_LCD("  /             ","que me pasa     ");         
  tocar_nota(Cs2,c);
  tocar_nota( As,c);
  tocar_nota(  B,c);
  tocar_nota(Sil,c);
  tocar_nota( As,c);

  insertar_LCD("        /       ","Y no sabes      ");         
  tocar_nota( Gs,c);
  tocar_nota( As,c);
  tocar_nota( As,c);

  insertar_LCD("  /             ","que contestarme ");         
  tocar_nota(Sil,c);
  tocar_nota(Sil,c);
  tocar_nota(Sil,c);

  tocar_nota(Cs2,c);
  tocar_nota(  B,c);
  tocar_nota(As ,c);
  tocar_nota(  B,c);
  tocar_nota(Sil,c);
  tocar_nota(As ,n);

  insertar_LCD("                ","Porque claro    ");         
  tocar_nota(Cs2,c);
  tocar_nota(As ,c);
  tocar_nota(  B,c);
  tocar_nota(Sil,c);
  tocar_nota(As ,n);

  insertar_LCD("                ","de seguro       ");         
  tocar_nota(Cs2,c);
  tocar_nota(As ,c);
  tocar_nota( B ,c);
  tocar_nota(Sil,c);
  tocar_nota(As ,c);

  insertar_LCD("       /        ","te maree        ");         
  tocar_nota(Gs ,c);
  tocar_nota(As ,c);
  tocar_nota(Fs ,c);
}
void ConMisIdas(){
  insertar_LCD("                ","                ");         
	tocar_nota(Sil,c);

  insertar_LCD("Con mis idas    ","y vueltas       ");         
	tocar_nota(Fs2,c);
	tocar_nota(Fs2,c);
	tocar_nota(Fs2,c);
	tocar_nota(Fs2,c);
	tocar_nota(Fs2,c);
	tocar_nota(Fs2,c);
	tocar_nota(Sil,c);
	tocar_nota(As2,n);

  insertar_LCD("       /        ","Te canse con    ");         
	tocar_nota(Fs2,c);
	tocar_nota(Fs2,c);
	tocar_nota(Fs2,c);
	tocar_nota(Fs2,c);

  insertar_LCD("    /           ","mi camara lenta ");         
	tocar_nota(Fs2,c);
	tocar_nota(Fs2,c);
	tocar_nota(Fs2,c);
	tocar_nota(Fs2,c);
	tocar_nota(Fs2,c);
	tocar_nota(Sil,c);
	tocar_nota(As2,n);

  insertar_LCD("                ","Y aunque trato, ");         
	tocar_nota(Cs2,c);
	tocar_nota(Ds2,c);
	tocar_nota(Cs2,n);
	tocar_nota(As ,c);
	tocar_nota(Sil,c);
  insertar_LCD("                ","nunca puedo     ");         
	tocar_nota(Cs2,c);
	tocar_nota(Ds2,c);
	tocar_nota(Cs2,n);
	tocar_nota(As ,c);
	tocar_nota(Sil,c);

  insertar_LCD("apurar        / ","     mi decision");         
	tocar_nota(Cs2,c);
	tocar_nota(Ds2,c);
	tocar_nota(Cs2,n);
	tocar_nota(  B,c);
	tocar_nota( As,c);
	tocar_nota( Gs,c);
	tocar_nota( Fs,c);
	tocar_nota(Sil,c);
}

/*
  -----------------------------------
  Musica de Gershon Kingsley: PopCorn
  -----------------------------------
*/

void popcorn() {
  prePre();  Pantalla(1);  
  prePre2(); Pantalla(2);  
  uno_();    Pantalla(3);
  dos_();    Pantalla(4);  
  uno_();    Pantalla(1);
  dos_dos(); Pantalla(2);  
  tres();    Pantalla(3);  
  cuatro();  Pantalla(4);  
  tres();    Pantalla(1);
  /*cuatro();  Pantalla(2);  
  uno_();    Pantalla(3);
  dos_();    Pantalla(4);  
  uno_();    Pantalla(1);
  dos_dos(); Pantalla(2);  
  tres();    Pantalla(3);
  cuatro();  Pantalla(4);  
  tres();    Pantalla(1);
  cuatro(); Pantalla(2);*/

  // Apagar todos los LEDs
  for (int i = 0; i < unidades_LEDS; i++) {
    digitalWrite(leds[i], LOW);
  }  
}

void prePre(){ 
  int i=1;
  tocar_nota(A__,corch);  Pantalla(i++);
  tocar_nota(A_,semi);  
  tocar_nota(E_,semi);  
  tocar_nota(C_,semi);  
  tocar_nota(Sil,semi);  
  
  tocar_nota(A_,semi);  
  tocar_nota(E_,semi);  
  tocar_nota(C_,semi);

  tocar_nota(Sil,semi);  
  tocar_nota(A_,semi);  
  tocar_nota(E_,semi);  
  tocar_nota(C_,semi);
  tocar_nota(A__,semi);  
  tocar_nota(C_,semi);
  tocar_nota(A_,semi);  
};

void prePre2(){ 
  tocar_nota(A__,corch);  
  tocar_nota(A_,semi);  
  tocar_nota(E_,semi);  
  tocar_nota(C_,semi);  
  tocar_nota(Sil,semi);  
  
  tocar_nota(A_,semi);  
  tocar_nota(E_,semi);  
  tocar_nota(C_,semi);

  tocar_nota(Sil,semi);  
  tocar_nota(A_,semi);  
  tocar_nota(E_,semi);  
};

void uno_(){
  tocar_nota(A_3,semi);  tocar_nota(Sil,semi);
  tocar_nota(G3,semi);   tocar_nota(Sil,semi);
  tocar_nota(A_3,semi);  tocar_nota(Sil,semi);
  tocar_nota(E3,semi);   tocar_nota(Sil,semi);
  tocar_nota(C3,semi);   
  tocar_nota(E3,semi);   tocar_nota(Sil,semi);   
  tocar_nota(A_2,semi);  tocar_nota(Sil,semi); tocar_nota(Sil,semi);   

  tocar_nota(Sil,corch);   
  tocar_nota(A_3,semi);  tocar_nota(Sil,semi);
  tocar_nota(G3,semi);   tocar_nota(Sil,semi);
  tocar_nota(A_3,semi);  tocar_nota(Sil,semi);
  tocar_nota(E3,semi);   tocar_nota(Sil,semi);
  tocar_nota(C3,semi);   
  tocar_nota(E3,semi);   tocar_nota(Sil,semi);   
  tocar_nota(A_2,semi);  tocar_nota(Sil,semi);   tocar_nota(Sil,semi); 

  tocar_nota(Sil,corch);   
};

void dos_(){
  tocar_nota(A_3,semi);  tocar_nota(Sil,semi);
  tocar_nota(B3,semi);   tocar_nota(Sil,semi);
  tocar_nota(C4,semi);   tocar_nota(Sil,semi);
  tocar_nota(B3,semi);   
  tocar_nota(C4,semi);   tocar_nota(Sil,semi);
  tocar_nota(C4,semi);   

  tocar_nota(A_3,semi);  tocar_nota(Sil,semi);
  tocar_nota(B3,semi);   tocar_nota(Sil,semi);
  tocar_nota(A_3,semi);   
  tocar_nota(B3,semi);   tocar_nota(Sil,semi);
  tocar_nota(B3,semi);    
  
  tocar_nota(G3,semi);   tocar_nota(Sil,semi);
  tocar_nota(A_3,semi);   tocar_nota(Sil,semi);
  tocar_nota(G3,semi);   
  tocar_nota(A_3,semi);   tocar_nota(Sil,semi);
  tocar_nota(A_3,semi);   
  
  tocar_nota(F3,semi);     tocar_nota(Sil,semi);
  tocar_nota(A_3,semi);   tocar_nota(Sil,semi);
  tocar_nota(Sil,corch);   
};

void dos_dos(){
  tocar_nota(A_3,semi);  tocar_nota(Sil,semi);
  tocar_nota(B3,semi);   tocar_nota(Sil,semi);
  tocar_nota(C4,semi);   tocar_nota(Sil,semi);
  tocar_nota(B3,semi);   
  tocar_nota(C4,semi);   tocar_nota(Sil,semi);
  tocar_nota(C4,semi);   

  tocar_nota(A_3,semi);   tocar_nota(Sil,semi);
  tocar_nota(B3,semi);   tocar_nota(Sil,semi);
  tocar_nota(A_3,semi);   
  tocar_nota(B3,semi);   tocar_nota(Sil,semi);
  tocar_nota(B3,semi);    
  
  tocar_nota(G3,semi);   tocar_nota(Sil,semi);
  tocar_nota(A_3,semi);   tocar_nota(Sil,semi);
  tocar_nota(G3,semi);   
  tocar_nota(A_3,semi);   tocar_nota(Sil,semi);
  tocar_nota(A_3,semi);   
  
  tocar_nota(B3,semi);   tocar_nota(Sil,semi);
  tocar_nota(C4,semi);   tocar_nota(Sil,semi);
  tocar_nota(Sil,corch);   
};

void tres(){ 
  tocar_nota(E4,semi);   tocar_nota(Sil,semi);
  tocar_nota(D4,semi);   tocar_nota(Sil,semi);
  tocar_nota(E4,semi);   tocar_nota(Sil,semi);
  tocar_nota(C4,semi);   tocar_nota(Sil,semi);
  tocar_nota(G3,semi);   
  tocar_nota(C4,semi);   tocar_nota(Sil,semi);   
  tocar_nota(E3,semi);   tocar_nota(Sil,semi); tocar_nota(Sil,semi);   

  tocar_nota(Sil,corch);   
  tocar_nota(E4,semi);   tocar_nota(Sil,semi);
  tocar_nota(D4,semi);   tocar_nota(Sil,semi);
  tocar_nota(E4,semi);   tocar_nota(Sil,semi);
  tocar_nota(C4,semi);   tocar_nota(Sil,semi);
  tocar_nota(G3,semi);   
  tocar_nota(C4,semi);   tocar_nota(Sil,semi);   
  tocar_nota(E3,semi);   tocar_nota(Sil,semi); tocar_nota(Sil,semi);   

  tocar_nota(Sil,corch);   
};

void cuatro(){ 
  tocar_nota(E4,semi);   tocar_nota(Sil,semi);
  tocar_nota(Fs4,semi);   tocar_nota(Sil,semi);
  tocar_nota(G4,semi);   tocar_nota(Sil,semi);
  tocar_nota(Fs4,semi);   
  tocar_nota(G4,semi);   tocar_nota(Sil,semi);
  tocar_nota(G4,semi);   

  tocar_nota(E4,semi);   tocar_nota(Sil,semi);
  tocar_nota(Fs4,semi);   tocar_nota(Sil,semi);
  tocar_nota(E4,semi);   
  tocar_nota(Fs4,semi);   tocar_nota(Sil,semi);
  tocar_nota(Fs4,semi);    
  
  tocar_nota(D4,semi);   tocar_nota(Sil,semi);
  tocar_nota(E4,semi);   tocar_nota(Sil,semi);
  tocar_nota(D4,semi);   
  tocar_nota(E4,semi);   tocar_nota(Sil,semi);
  tocar_nota(E4,semi);   
  
  tocar_nota(C4,semi);     tocar_nota(Sil,semi);
  tocar_nota(E4,semi);   tocar_nota(Sil,semi);
  tocar_nota(Sil,corch);   
};

void Pantalla(int II){
  if (II == 1){   
  insertar_LCD("- Palomitas de -","      Maiz      ");}
  else if (II == 2){
  insertar_LCD("/ Palomitas de /","      Maiz      ");}
  else if (II == 3){
  insertar_LCD("| Palomitas de |","      Maiz      ");}
  else if (II == 4){
  insertar_LCD("_ Palomitas de _","      Maiz      ");}
}

/*
  ---------------------------
  Musica de 31 Minutos: Intro
  ---------------------------
*/
void treinta_y_un_minutos(){
 Parte_01();
 YDescurbiUnmundoMuyCompejooo();
}


void YDescurbiUnmundoMuyCompejooo(){
  tocar_nota(D2,n);
  tocar_nota(E2,c);
  tocar_nota(D2,n);
  tocar_nota(Fs2,n + c);
  tocar_nota(D2,n);
  tocar_nota(E2,c);
  tocar_nota(D2,n);
  tocar_nota(Fs2,n+c);
}

void Parte_01(){
  tocar_nota(D2,n);
  tocar_nota(E2,c);
  tocar_nota(D2,n);
  tocar_nota(Fs2,n + c);
  tocar_nota(D2,n);
  tocar_nota(E2,c);
  tocar_nota(D2,n);
  tocar_nota(Fs2,n+c);

  tocar_nota(D2,n);
  tocar_nota(E2,c);
  tocar_nota(D2,n);
  tocar_nota(B,n);
  tocar_nota(D2,n+b+c);

  tocar_nota(Sil,n);

  tocar_nota(D2,n);
  tocar_nota(E2,c);
  tocar_nota(D2,n);
  tocar_nota(Fs2,n + c);
  tocar_nota(D2,n);
  tocar_nota(E2,c);
  tocar_nota(D2,n);
  tocar_nota(Fs2,n+c);

  tocar_nota(D2,n);
  tocar_nota(E2,c);
  tocar_nota(D2,n);
  tocar_nota(B,r-c);
  tocar_nota(B2, c);
  tocar_nota(B, c);

  tocar_nota(Sil,n);

  tocar_nota(D2,n);
  tocar_nota(E2,c);
  tocar_nota(D2,n);
  tocar_nota(Fs2,n + c);
  tocar_nota(D2,n);
  tocar_nota(E2,c);
  tocar_nota(D2,n);
  tocar_nota(Fs2,n+c);
  tocar_nota(D2,n);

  tocar_nota(E2,c);
  tocar_nota(D2,n);
  tocar_nota(B,n);
  tocar_nota(D2,n);

  tocar_nota(Fs2,n);
  tocar_nota(E2,c);
  tocar_nota(D2,c);
  tocar_nota(E2,n);
  tocar_nota(D2,c);
  tocar_nota(B,n);

  tocar_nota(Sil,c);

  tocar_nota(D2,c);
  tocar_nota(E2,c);
  tocar_nota(D2,c);
  tocar_nota(Fs2,n );
  tocar_nota(D2,n);
  tocar_nota(E2,c);
  tocar_nota(D2,n);
  tocar_nota(A_2,n+c);

  tocar_nota(D2,n);
  tocar_nota(Fs2,c);
  tocar_nota(E2,n);
  tocar_nota(D2,b+n);

  tocar_nota(B,c);
  tocar_nota(D2,c);
  tocar_nota(E2,n);
}

/*
  ----------------------------------
  Musica de Iron Maiden: The Trooper
  ----------------------------------
*/
void the_trooper() {
  insertar_LCD("- The Trooper  -","   Iron Maiden  ");
  Intro_1();
  Punteo();

  // Apagar todos los LEDs
  for (int i = 0; i < unidades_LEDS; i++) {
    digitalWrite(leds[i], LOW);
  } 
}

void Intro_1(){
  tocar_nota(E2,c);
  tocar_nota(E2,c);
  
  tocar_nota(E2,s);
  tocar_nota(D2,s);
  tocar_nota(B,c);

  tocar_nota(D2,c);
  tocar_nota(D2,c);

  tocar_nota(D2,s);
  tocar_nota(C2,s);
  tocar_nota(A,c);

  // [2]
  tocar_nota(C2,c);
  tocar_nota(C2,c);

  tocar_nota(C2,s);
  tocar_nota(B,s);
  tocar_nota(G,c);

  tocar_nota(D2,c);
  tocar_nota(G2,s);
  tocar_nota(Sil,s);
  tocar_nota(D2,c);
  tocar_nota(E2,n);

  // ..[3]
  //tocar_nota(E2,c);

  
  tocar_nota(E2,c);
  
  tocar_nota(E2,s);
  tocar_nota(D2,s);
  tocar_nota(B,c);

  tocar_nota(D2,c);
  tocar_nota(D2,c);

  tocar_nota(D2,s);
  tocar_nota(C2,s);
  tocar_nota(A,c);

  tocar_nota(C2,c);
  tocar_nota(C2,c);

  tocar_nota(C2,s);
  tocar_nota(B,s);
  tocar_nota(G,c);
  
  tocar_nota(D2,c);
  tocar_nota(G2,s);
  tocar_nota(Sil,s);
  tocar_nota(D2,c);
  tocar_nota(E2,n);

  tocar_nota(E2,c);
  
  tocar_nota(E2,s);
  tocar_nota(D2,s);
  tocar_nota(B,c);

  tocar_nota(D2,c);
  tocar_nota(D2,c);

  tocar_nota(D2,s);
  tocar_nota(C2,s);
  tocar_nota(A,c);

  tocar_nota(C2,c);
  tocar_nota(C2,c);

  tocar_nota(C2,s);
  tocar_nota(B,s);
  tocar_nota(G,c);
  
  tocar_nota(D2,c);
  tocar_nota(G2,s);
  tocar_nota(Sil,s);
  tocar_nota(D2,c);
  tocar_nota(E2,n);

  tocar_nota(E2,c);

  tocar_nota(E2,s);
  tocar_nota(D2,s);
  tocar_nota(B,c);

  tocar_nota(D2,c);
  tocar_nota(D2,c);

  tocar_nota(D2,s);
  tocar_nota(C2,s);
  tocar_nota(A,c);

  tocar_nota(C2,c);
  tocar_nota(C2,c);

  tocar_nota(C2,s);
  tocar_nota(B,s);
  tocar_nota(G,s);
  tocar_nota(Sil,s);

  tocar_nota(D2,c);
  tocar_nota(G2,c);
  tocar_nota(D2,c);
  tocar_nota(E2,n);
}

void Punteo(){
  tocar_nota(G3,c);

  tocar_nota(Fs3,n/6);
  tocar_nota(G3,n/6);
  tocar_nota(Fs3,n/6);
  tocar_nota(G3,n/6);
  tocar_nota(Fs3,n/6);
  tocar_nota(G3,n/6);

  tocar_nota(B3,c);
  tocar_nota(G3,c);

  tocar_nota(Fs3,n/6);
  tocar_nota(G3,n/6);
  tocar_nota(Fs3,n/6);
  tocar_nota(G3,n/6);
  tocar_nota(Fs3,n/6);
  tocar_nota(G3,n/6);

  Tararara();

  Rapida();

  Tararara();

  Rapida();

  Tararara();

  Rapida();

  /*Tararara_();

  Rapida();

  Tararara();

  Rapida();

  Tararara();

  Rapida();

  Tararara();

  Rapida();

  Tararara_();*/
};
   

void Rapida(){

  tocar_nota(G3,c);

  tocar_nota(Fs3,n/6);
  tocar_nota(G3,n/6);
  tocar_nota(Fs3,n/6);
  tocar_nota(G3,n/6);
  tocar_nota(Fs3,n/6);
  tocar_nota(G3,n/6);

  tocar_nota(B3,c);
  tocar_nota(G3,c);

  tocar_nota(Fs3,n/6);

  tocar_nota(G3,n/6);
  tocar_nota(Fs3,n/6);
  tocar_nota(G3,n/6);
  tocar_nota(Fs3,n/6);
  tocar_nota(G3,n/6);
}


void Tararara(){
  tocar_nota(G3,c);
  tocar_nota(E3,c);

  tocar_nota(E3,n);

  tocar_nota(D3,c);
  tocar_nota(G3,c);
  tocar_nota(D3,c);
  tocar_nota(E3,n);
}

void Tararara_(){
  tocar_nota(G3,s);
  tocar_nota(Sil,s);
  tocar_nota(E3,s);
  tocar_nota(Sil,s);
  
  tocar_nota(E3,s);
  tocar_nota(Sil,s);
  tocar_nota(Sil,s);
  tocar_nota(Sil,s);

  tocar_nota(D3,s);
  tocar_nota(Sil,s);
  
  tocar_nota(G3,s);
  tocar_nota(Sil,s);
  
  tocar_nota(D3,s);
  tocar_nota(Sil,s);
  
  tocar_nota(E3,n);
}
