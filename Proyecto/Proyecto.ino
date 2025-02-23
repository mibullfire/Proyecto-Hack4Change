/*
Código elaborado por Miguel Toro y Pepe Clavijo
*/

#include <Servo.h>  // Biblioteca estándar para servos en Arduino

/*
En este código se recoge el funcionamiento básico del coche/carretilla que vamos a implementar.
Reglas importantes a la hora de usar los pines:
  - Todo motor estará conectado a un PIN con valor numérico PAR.
  - Todo sensor estará conectado a un PIN con valor numérico IMPAR.
    * Los sensores idéndicos serán conectados lo más adayacente posible, cumpliendo la norma anterior.
    * Si un sensor necesita más de un puerto, será conectado de forma totalmente adayacente, a pesar de que
    su número no sea impar.


Definición de los motores:

                      ^
                      |
                    _____
     frontalIzq ->  |   |   <- frontalDer
                    |   |
        colaIzq ->  |___|   <- colaDer

Nota: El coche, en su recorrido hacia delante, sigue la flecha.


Explicación variable "destino":
*/

// Variables:

// Motores
const int motor1 = 2;
const int motor2 = 4;
const int motor3 = 6;
const int motor4 = 8;

const int controladorVelocidad = 3; // Variable para controlar la velocidad de un motor, se necesita el driver "L298N"

// Sensores IR
const int ir1 = 5;
const int ir2 = 7;
const int en1 = 1;
const int en2 = 13;

// Sensores Ultrasónicos
const int echo1 = 9;
const int echo2 = 11;
const int trigger1 = 10;
const int trigger2 = 12;

// Cadena con el destino de la carretilla:
String destino;


void setup() {
  Serial.begin(9600);

  // Motores
  pinMode(motor1, OUTPUT);
  pinMode(motor2, OUTPUT);
  pinMode(motor3, OUTPUT);
  pinMode(motor4, OUTPUT);

  pinMode(controladorVelocidad, OUTPUT);

  // Sensores IR
  pinMode(ir1, INPUT); // Sensor de la derecha
  pinMode(ir2, INPUT); // Sensor de la izquierda
    // Por defecto, activamos ambos sensores:
  digitalWrite(en1, HIGH);
  digitalWrite(en2, HIGH);

  // Sensores Ultrasónicos
  pinMode(echo1, INPUT);
  pinMode(echo2, INPUT);
  pinMode(trigger1, OUTPUT);
  pinMode(trigger2, OUTPUT);
    // Inicializamos los triggers en LOW:
  digitalWrite(trigger1, LOW);
  digitalWrite(trigger2, LOW);


}

void loop() {

  // Condiciones:
  // Si la distancia recibida por los ultrasónicos es menor a 10, el coche se detiene, luego:
  if (distancia(trigger1, echo1) < 10 || distancia(trigger2, echo2)) {
    detener();
  } else {
    // Si la distancia supera los 10 cm, entonces el coche se moverá:
    if (sensoresIR(ir1) !=1 && sensoresIR(ir2) !=1) {
      // Si solo se activa un sensor o ninguno, el movimiento del coche será "normal";
      movimiento(ir1, ir2);

    } else {
      // Código para cuando encuentre una "rotonda o un objeto", es decir, cuando ambos sensores se activen.
    }

  }
  

}


/*
 \
  \
   \
FUNCIONES
   /
  /
*/

// Función para mover el coche hacia adelante
void adelante() {
  digitalWrite(motor1, HIGH);
  digitalWrite(motor2, HIGH);
  digitalWrite(motor3, HIGH);
  digitalWrite(motor4, HIGH);
  analogWrite(controladorVelocidad, 200); // Velocidad media (0-255)
}

// Función para mover el coche hacia atrás
void atras() {
  digitalWrite(motor1, LOW);
  digitalWrite(motor2, LOW);
  digitalWrite(motor3, LOW);
  digitalWrite(motor4, LOW);
  analogWrite(controladorVelocidad, 200);
}

// Función para girar a la izquierda
void izquierda() {
  digitalWrite(motor1, LOW);
  digitalWrite(motor2, HIGH);
  digitalWrite(motor3, LOW);
  digitalWrite(motor4, HIGH);
  analogWrite(controladorVelocidad, 200);
}

// Función para girar a la derecha
void derecha() {
  digitalWrite(motor1, HIGH);
  digitalWrite(motor2, LOW);
  digitalWrite(motor3, HIGH);
  digitalWrite(motor4, LOW);
  analogWrite(controladorVelocidad, 200);
}

// Función para detener el coche
void detener() {
  digitalWrite(motor1, LOW);
  digitalWrite(motor2, LOW);
  digitalWrite(motor3, LOW);
  digitalWrite(motor4, LOW);
}

// Función para el movimiento normal de la carretilla:
void movimiento(int sensorIRDerecha, int sensorIRIzquierda) {
  if (sensoresIR(sensorIRDerecha) == 0 && sensoresIR(sensorIRIzquierda == 0)) {
    adelante();
  } else if (sensoresIR(sensorIRDerecha) == 1 && sensoresIR(sensorIRIzquierda) == 0) {
    derecha();
  } else if (sensoresIR(sensorIRDerecha) == 0 && sensoresIR(sensorIRIzquierda) == 1) {
    izquierda();
  }
}

int distancia(int Trigger, int Echo) {
  long t; // Tiempo que tarda en llegar el eco
  long d; // Distancia en centimetros
 
  digitalWrite(Trigger, HIGH);
  delayMicroseconds(10);          //Enviamos un pulso de 10us
  digitalWrite(Trigger, LOW);
  
  t = pulseIn(Echo, HIGH); 
  d = t/59;

  return d;
}

int sensoresIR(int sensorIR) {
  int estado = digitalRead(sensorIR);
  if (estado == LOW) {
    return 0;
  }
  return 1;
}