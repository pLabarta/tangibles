#define MOTOR_PIN_00 13        // Motor conectado al pin 13
#define MOTOR_PIN_01 12  
#define MOTOR_PIN_02 14
#define MOTOR_PIN_03 27
#define MOTOR_PIN_04 26
#define MOTOR_PIN_05 25

#define DISTANCIA_INFRA 36  

#define TRIG_PIN 23 // ESP32 pin GPIO23 connected to Ultrasonic Sensor's TRIG pin
#define ECHO_PIN 22 // ESP32 pin GPIO22 connected to Ultrasonic Sensor's ECHO pin

float duration_us, distance_cm;
float distance_old;
float counter;

//// Control de los tiempos
unsigned long previousMillisEscena = 0;
unsigned long previousMillisSensor = 0;
unsigned long interval = 50; // Interval for updating brightness (milliseconds)

//// Setup de la potencia inicial
float potencia = 0;


void setup() {
  distance_old = 0.0;
  counter = 0.0;
 // Inicializar comunicación Serial
  Serial.begin(115200);
  delay(3000); // Esperar a que se estabilice la comunicación serial
  Serial.println("Iniciando programa de prueba del sensor SHARP");
  Serial.println("---------------------------------------------");

// configure the trigger pin to output mode
  pinMode(TRIG_PIN, OUTPUT);
  // configure the echo pin to input mode
  pinMode(ECHO_PIN, INPUT);
  
  // Configurar el pin como salida
  pinMode(MOTOR_PIN_00, OUTPUT);
  pinMode(MOTOR_PIN_01, OUTPUT);
  pinMode(MOTOR_PIN_02, OUTPUT);
  pinMode(MOTOR_PIN_03, OUTPUT);
  pinMode(MOTOR_PIN_04, OUTPUT);
  pinMode(MOTOR_PIN_05, OUTPUT);
  
  // Inicialmente motor apagado
  analogWrite(MOTOR_PIN_00, 0);
  analogWrite(MOTOR_PIN_01, 0);
  analogWrite(MOTOR_PIN_02, 0);
  analogWrite(MOTOR_PIN_03, 0);
  analogWrite(MOTOR_PIN_04, 0);
  analogWrite(MOTOR_PIN_05, 0);
}

void loop() {

  sensar();
  escena1();

  counter += 1;
  delay(25);
}

void sensar() {
  float intervalo_sensado = 100;
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillisSensor >= intervalo_sensado) {
    previousMillisSensor = currentMillis;
    // SENSORES DISTANCIA
    // int lecturaADC = analogRead(DISTANCIA_INFRA);
    // Serial.println((int)lecturaADC);

    // generate 10-microsecond pulse to TRIG pin
    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG_PIN, LOW);

    // measure duration of pulse from ECHO pin
    duration_us = pulseIn(ECHO_PIN, HIGH);



    // Serial.print("duration_us: ");
    // Serial.println(duration_us);

    // // calculate the distance
    distance_cm = 0.017 * duration_us;
    distance_old = distance_old * 0.8 + distance_cm * 0.2;

    // // print the value to Serial Monitor
    // Serial.print("distance: ");
    // Serial.print(distance_old);
    // Serial.println(" cm");
  }
}

void escena1() {
  unsigned long currentMillis = millis();
  
  if (currentMillis - previousMillisEscena >= interval) {
    previousMillisEscena = currentMillis;
    // Serial.println(counter);

    float divisor = (distance_old);
    float potenciaExtra = 70 - distance_old;
    Serial.println(divisor);

    // Calcular el brillo de la luz basado en una sinusoide
    float sineWave = (
      sin(
        counter/divisor
        ) + 1.0
    ) / 2.0; // Scaled to 0-1 range
    potencia = int(sineWave * 128 + sineWave * potenciaExtra);
    // int redBrightness = int(sineWave * 255);
    // int greenBrightness = int(sineWave * 255 * 0.7); // Slightly less green for a warmer yellow
    // int blueBrightness = int(sineWave * 255 * 0.1);

    // Establecer el color de la luz
    setTodos(potencia);

    // Incrementar el brillo para la siguiente iteración
    // float fadeAmount = 0.1;
    // potencia += fadeAmount;

    // Reiniciar el ciclo si se alcanza el máximo
    // if (potencia > TWO_PI) {
    //   potencia = 0;
    // }

    Serial.print("potencia: ");
    Serial.println(potencia);
  }
}

// void escena2() {
//   if distancia <= 60.0 && distancia >= 30 {
//     setTodos(128);
//   }
// }

void setTodos(int potencia) {
  analogWrite(MOTOR_PIN_00, potencia);  // ~25% de potencia
  analogWrite(MOTOR_PIN_01, potencia);
  analogWrite(MOTOR_PIN_02, potencia);
  analogWrite(MOTOR_PIN_03, potencia);
  analogWrite(MOTOR_PIN_04, potencia);
  analogWrite(MOTOR_PIN_05, potencia);
}

void demo1() {
  // Demostración 1: Velocidad baja
  // Serial.println("Velocidad baja");
  analogWrite(MOTOR_PIN_00, 64);  // ~25% de potencia
  analogWrite(MOTOR_PIN_01, 64);
  analogWrite(MOTOR_PIN_02, 64);
  analogWrite(MOTOR_PIN_03, 64);
  analogWrite(MOTOR_PIN_04, 64);
  analogWrite(MOTOR_PIN_05, 64);
  delay(2000);
}

void demo2() {
  // Demostración 2: Velocidad media
  // Serial.println("Velocidad media");
  analogWrite(MOTOR_PIN_00, 128); // 50% de potencia
  analogWrite(MOTOR_PIN_01, 128);
  analogWrite(MOTOR_PIN_02, 128);
  analogWrite(MOTOR_PIN_03, 128);
  analogWrite(MOTOR_PIN_04, 128);
  analogWrite(MOTOR_PIN_05, 128);
  delay(2000);
}

void demo3() {
  // Demostración 3: Velocidad alta
  // Serial.println("Velocidad alta");
  analogWrite(MOTOR_PIN_00, 255); // 100% de potencia
  analogWrite(MOTOR_PIN_01, 255);
  analogWrite(MOTOR_PIN_02, 255);
  analogWrite(MOTOR_PIN_03, 255);
  analogWrite(MOTOR_PIN_04, 255);
  analogWrite(MOTOR_PIN_05, 255);
  delay(2000);
}

void demo4() {
  // Demostración 4: Motor apagado
  // Serial.println("Motor apagado");
  analogWrite(MOTOR_PIN_00, 0);   // 0% - apagado
  analogWrite(MOTOR_PIN_01, 0);
  analogWrite(MOTOR_PIN_02, 0);
  analogWrite(MOTOR_PIN_03, 0);
  analogWrite(MOTOR_PIN_04, 0);
  analogWrite(MOTOR_PIN_05, 0);
  delay(2000);
}

void demo5() {
  // Demostración 5: Incremento gradual
  // Serial.println("Incrementando velocidad...");
  for(int i = 0; i <= 255; i += 5) {
    analogWrite(MOTOR_PIN_00, i);
    analogWrite(MOTOR_PIN_01, i);
    analogWrite(MOTOR_PIN_02, i);
    analogWrite(MOTOR_PIN_03, i);
    analogWrite(MOTOR_PIN_04, i);
    analogWrite(MOTOR_PIN_05, i);
    // Serial.print("Velocidad: ");
    // Serial.println(i);
    delay(100);
  }
}

void demo6() {
  // Demostración 6: Decremento gradual
  // Serial.println("Disminuyendo velocidad...");
  for(int i = 255; i >= 0; i -= 5) {
    analogWrite(MOTOR_PIN_00, i);
    analogWrite(MOTOR_PIN_01, i);
    analogWrite(MOTOR_PIN_02, i);
    analogWrite(MOTOR_PIN_03, i);
    analogWrite(MOTOR_PIN_04, i);
    analogWrite(MOTOR_PIN_05, i);
    // Serial.print("Velocidad: ");
    // Serial.println(i);
    delay(100);
  }
}