import RPi.GPIO as GPIO
import time
import random
import os

# ---------------- CONFIG ----------------
PIR = 14
LED_AMARILLO = 15
LED_ROJO = 18

# Si tienes buzzer, descomenta:
# ZUMBADOR = 23

GPIO.setwarnings(False)
GPIO.setmode(GPIO.BCM)
GPIO.setup(PIR, GPIO.IN)
GPIO.setup(LED_AMARILLO, GPIO.OUT)
GPIO.setup(LED_ROJO, GPIO.OUT)
# GPIO.setup(ZUMBADOR, GPIO.OUT)

# Preguntas y respuestas (puedes cambiarlas)
claves = ["color", "animal", "ciudad", "comida", "deporte"]
contrasenas = {
    "color": "azul",
    "animal": "perro",
    "ciudad": "madrid",
    "comida": "pizza",
    "deporte": "futbol"
}

INTENTOS_MAX = 3
COOLDOWN_SEG = 10        # tiempo “enfriamiento” tras un evento
ESPERA_MOV = 0.5         # antirebote PIR
ASTERISK_OUT = "/var/spool/asterisk/outgoing/"
PLANTILLA = "plantilla.call"

def parpadeo_led(pin, veces=6, t=0.15):
    for _ in range(veces):
        GPIO.output(pin, True)
        time.sleep(t)
        GPIO.output(pin, False)
        time.sleep(t)

def llamar_asterisk():
    # Evita errores si no existe la plantilla
    if not os.path.exists(PLANTILLA):
        print(" No existe plantilla.call, no se puede llamar.")
        return

    # Nombre único para no pisar archivos
    llamada_tmp = f"llamada_{int(time.time())}.call"

    os.system(f"cp {PLANTILLA} {llamada_tmp}")
    os.system(f"mv {llamada_tmp} {ASTERISK_OUT}")
    print(" Llamada enviada a Asterisk (outgoing).")

print(" Sistema antirobo activado")

try:
    while True:
        # Espera movimiento (antirebote básico)
        if GPIO.input(PIR):
            print(" Movimiento detectado")
            time.sleep(ESPERA_MOV)

            # Si sigue detectando, confirmamos evento
            if not GPIO.input(PIR):
                continue

            GPIO.output(LED_ROJO, False)
            intentos = 0
            llamada_hecha = False

            # Reto aleatorio
            clave = random.choice(claves)

            while intentos < INTENTOS_MAX:
                parpadeo_led(LED_AMARILLO, veces=4, t=0.12)
                respuesta = input(f"Dime un {clave}: ").strip().lower()

                if respuesta == contrasenas[clave]:
                    print(" Correcto. Alarma desactivada.")
                    GPIO.output(LED_AMARILLO, False)
                    GPIO.output(LED_ROJO, False)
                    break
                else:
                    intentos += 1
                    print(f" Incorrecto ({intentos}/{INTENTOS_MAX})")
                    GPIO.output(LED_AMARILLO, False)
                    GPIO.output(LED_ROJO, True)

                    # Si quieres buzzer:
                    # GPIO.output(ZUMBADOR, True); time.sleep(0.2); GPIO.output(ZUMBADOR, False)

            # Si agotó intentos → avisar
            if intentos >= INTENTOS_MAX:
                print(" Avisando a la poli (Asterisk)")
                if not llamada_hecha:
                    llamar_asterisk()
                    llamada_hecha = True

            # Cooldown para no repetir evento infinito
            print(f" Cooldown {COOLDOWN_SEG}s...")
            time.sleep(COOLDOWN_SEG)

        time.sleep(0.2)

finally:
    GPIO.output(LED_AMARILLO, False)
    GPIO.output(LED_ROJO, False)
    # GPIO.output(ZUMBADOR, False)
    GPIO.cleanup()
    print(" GPIO cleanup realizado")
