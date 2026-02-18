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

pir = 14
led_amarillo = 15
led_rojo = 18

GPIO.setmode(GPIO.BCM)
GPIO.setup(pir, GPIO.IN)
GPIO.setup(led_amarillo, GPIO.OUT)
GPIO.setup(led_rojo, GPIO.OUT)

print("Sistema antirobo activado")

while True:
    if GPIO.input(SENSOR) == True:
        print("Movimiento detectado")
        GPIO.output(LED_AMARILLO, True)
        numero = random.randint(0,4)
        clave = claves[numero]
        respuesta = input("Dime un " + clave + ":")

        if respuesta == contrasenas[clave]:
            print("Correcto")
            GPIO.output(led_amarillo, False)

        else:
            print("Incorrecto")
            GPIO.output(led_rojo, True)
	    GPIO.output(led_amarillo, False)
            os.system('cp plantilla.call llamada.call')
            os.system('mv llamada.call /var/spool/asterisk/outgoing/')
            
	    print("Avisando poli")

     time.sleep(2)