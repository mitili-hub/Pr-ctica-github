#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include "ThingSpeak.h"

// Conexión WiFi del aula
const char* ssid = "1D3";
const char* pass = "1D3_2019";

// Cliente que se usa para conectarse a ThingSpeak
WiFiClient cliente;

// Se crea un servidor web en el puerto 80 (HTTP)
ESP8266WebServer server(80);

// Pines del circuito
#define ldr A0        // Sensor LDR conectado al pin analógico A0
#define LED_PIN D4    // LED interno del ESP8266 (activo en LOW)

long ledOnTime = 0;       // Guarda el momento en que se encendió el LED
bool ledEncendido = false; // Indica si el LED está encendido o no

unsigned long lastTS = 0;           // Controla el tiempo entre envíos a ThingSpeak
const unsigned long TS_PERIOD = 15000; // Intervalo de envío (15 segundos)

// Cuando ThingSpeak envía una alerta, esta función se ejecuta
void handleAlerta() {
  String zona = server.arg("zona");  // Lee el parámetro “zona” enviado en la URL
  Serial.println("Alerta recibida desde ThingSpeak");
  if (zona != "") Serial.println("Zona: " + zona);  // Muestra la zona en consola si la hay

  // Enciende el LED (activo en LOW porque es el LED interno)
  digitalWrite(LED_PIN, LOW);
  ledEncendido = true;
  ledOnTime = millis();  // Guarda el momento del encendido

  // Responde a ThingSpeak confirmando que recibió la alerta
  server.send(200, "text/plain", "Alerta recibida");
}

void setup() {
  Serial.begin(115200);    // Inicia la comunicación por puerto serie
  pinMode(ldr, INPUT);     // Configura el LDR como entrada
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, HIGH); // LED apagado al inicio

  // Conexión del ESP8266 a la WiFi
  WiFi.begin(ssid, pass);
  Serial.print("Conectando a WiFi");
  while (WiFi.status() != WL_CONNECTED) {  // Espera hasta conectar
    delay(300);
    Serial.print(".");
  }
  Serial.println();
  Serial.print("IP asignada: ");
  Serial.println(WiFi.localIP()); // Muestra la IP del módulo

  // Inicializa la conexión con ThingSpeak
  ThingSpeak.begin(cliente);

  // Configura la ruta "/alerta" para recibir peticiones HTTP
  server.on("/alerta", handleAlerta);
  server.begin();
  Serial.println("Servidor web iniciado");
}

void loop() {
  // Atiende cualquier petición que llegue al servidor
  server.handleClient();

  // Si el LED está encendido, espera 10 segundos y lo apaga
  if (ledEncendido) {
    delay(10000);
    digitalWrite(LED_PIN, HIGH); // Apaga el LED (HIGH = apagado)
    ledEncendido = false;
    Serial.println("LED apagado automáticamente");
  }

  // Lectura del LDR y envío de datos a ThingSpeak
  int valor = analogRead(ldr); // Lee el nivel de luz (0–1023)
  ThingSpeak.setField(1, valor); // Asigna el valor al campo 1 del canal
  int http = ThingSpeak.writeFields(3101898, "S3Z46T4L4DPG1PNW"); // Envía los datos
  Serial.print(http);
  Serial.print("  LDR=");
  Serial.println(valor);

  // Espera 15 segundos antes del siguiente envío
  delay(15000);
}