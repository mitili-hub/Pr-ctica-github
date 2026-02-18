
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