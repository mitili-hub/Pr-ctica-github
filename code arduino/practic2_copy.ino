#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

const char* ssid = "1D3";
const char* pass = "1D3_2019";

ESP8266WebServer server(80);
#define LED_PIN D5

unsigned long ledOnTime = 0;
bool ledEncendido = false;

void handleAlerta() {
  String zona = server.hasArg("zona") ? server.arg("zona") : "";
  Serial.println("Alerta recibida desde ThingSpeak");
  if (zona.length()) {
    Serial.print("Zona: ");
    Serial.println(zona);
  }
  digitalWrite(LED_PIN, HIGH);    // Encender LED
  ledEncendido = true;
  ledOnTime = millis();
  server.send(200, "text/plain", "Alerta recibida");
}

void setup() {
  Serial.begin(115200);

  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);     // LED inicialmente apagado

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(250);
    Serial.print(".");
  }
  Serial.println();
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());

  server.on("/alerta", handleAlerta); // Registrar endpoint
  server.begin();
  Serial.println("Servidor web iniciado");
}

void loop() {
  server.handleClient(); // Atiende peticiones HTTP entrantes

  // Apagar LED después de 30 segundos
  if (ledEncendido && (millis() - ledOnTime > 30000UL)) {
    digitalWrite(LED_PIN, LOW);
    ledEncendido = false;
  }
}
