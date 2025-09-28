//Prueba modulos OLED y sensor infrarrojo
//**
//**
#include <Wire.h>             // Comunicación I2C (usada para conectar la OLED)
#include <Adafruit_GFX.h>     // Librería gráfica base
#include <Adafruit_SSD1306.h> // Librería para pantallas OLED SSD1306

// Configuración de la pantalla OLED
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

int sensor1 = 8;   // Sensor infrarrojo bajo (Tamaño de recipiente 1)
int sensor2 = 10;  // Sensor infrarrojo alto (Tamaño de recipiente 2)

void setup() {
  Serial.begin(9600);
  pinMode(sensor1, INPUT);
  pinMode(sensor2, INPUT);

  // Inicializar la pantalla OLED
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {  
    Serial.println(F("Error al inicializar la pantalla OLED"));
  }
  
  // Mensaje inicial
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println(F("Iniciando..."));
  display.display();
  delay(2000);  // Esperar 2 segundos
}

void loop() {
  int estado1 = digitalRead(sensor1); // Lee estado del sensor 1
  int estado2 = digitalRead(sensor2); // Lee estado del sensor 2

  // Invertimos la lógica: LOW = detecta el recipiente
  bool detecta1 = (estado1 == LOW);
  bool detecta2 = (estado2 == LOW);

  String mensaje;

  // Condiciones
  if (!detecta1 && !detecta2) { 
    mensaje = "Esperando...";
  }
  else if (!detecta1 && detecta2) {
    mensaje = "Tamanio 2";
  }
  else if (detecta1 && !detecta2) {
    mensaje = "Tamanio 1";
  }
  else if (detecta1 && detecta2) {
    mensaje = "Tamanio 2"; // prioridad al sensor 2
  }

  // Mostrar en Monitor Serie
  Serial.println(mensaje);

  // Mostrar en pantalla OLED
  display.clearDisplay();
  display.setTextSize(2);
  display.setCursor(0, 20);
  display.println(mensaje);
  display.display();

  delay(600); // Pequeño retardo
}
