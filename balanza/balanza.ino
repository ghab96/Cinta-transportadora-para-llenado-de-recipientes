#include "HX711.h"

const int DOUT_PIN = 3;
const int SCK_PIN = 2;

HX711 balanza;

float factor_calibracion = -96650.0;
float peso_cero = 0;

void setup() {
  Serial.begin(9600);
  Serial.println("Iniciando sistema de pesaje...");
  
  balanza.begin(DOUT_PIN, SCK_PIN);
  
  Serial.println("Esperando a que el HX711 esté listo...");
  delay(2000);
  
  balanza.set_scale(factor_calibracion);

  balanza.tare();
 
  Serial.println("Sistema listo para medir peso");
  Serial.println("Comandos disponibles:");
  Serial.println("'t' - Tarar (establecer cero)");
  Serial.println("'c' - Modo calibración");
  Serial.println("'r' - Lectura normal");
  Serial.println("------------------------------");
}

void loop() {
  if (Serial.available()) {
    char comando = Serial.read();
    
    switch(comando) {
      case 't': 
        tararBalanza();
        break;
      case 'c': 
        calibrarBalanza();
        break;
      case 'r': 
        leerPeso();
        break;
    }
  }
  
  static unsigned long ultimaLectura = 0;
  if (millis() - ultimaLectura > 2000) {
    leerPeso();
    ultimaLectura = millis();
  }
}

void leerPeso() {
  float peso = balanza.get_units(10);
  
  Serial.print("Peso: ");
  Serial.print(peso, 3);
  Serial.println(" g");
}

void tararBalanza() {
  Serial.println("Tarando... No coloques nada en la balanza");
  delay(3000);
  balanza.tare();
  Serial.println("Tara completada");
}

void calibrarBalanza() {
  Serial.println("=== MODO CALIBRACIÓN ===");
  Serial.println("1. Retira todo peso de la balanza");
  Serial.println("2. Presiona cualquier tecla cuando esté vacía");
  
  while(!Serial.available()) {
    delay(100);
  }
  Serial.read();
  
  Serial.println("Leyendo valor con balanza vacía...");
  delay(2000);
  long valor_vacio = balanza.read_average(10);
  Serial.print("Valor vacío: ");
  Serial.println(valor_vacio);
  
  Serial.println("3. Coloca un peso conocido en la balanza");
  Serial.println("4. Ingresa el peso en gramos:");
  
  while(!Serial.available()) {
    delay(100);
  }
  
  float peso_conocido = Serial.parseFloat();
  Serial.print("Peso conocido: ");
  Serial.print(peso_conocido);
  Serial.println(" g");
  
  Serial.println("Leyendo valor con peso conocido...");
  delay(2000);
  long valor_con_peso = balanza.read_average(10);
  Serial.print("Valor con peso: ");
  Serial.println(valor_con_peso);
  
  factor_calibracion = (valor_con_peso - valor_vacio) / peso_conocido;
  
  Serial.print("Nuevo factor de calibración: ");
  Serial.println(factor_calibracion, 6);
  
  balanza.set_scale(factor_calibracion);
  
  Serial.println("Calibración completada!");
  Serial.println("Guarda el factor de calibración para uso futuro:");
  Serial.print("factor_calibracion = ");
  Serial.println(factor_calibracion, 6);
}