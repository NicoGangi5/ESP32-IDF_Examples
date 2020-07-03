# ------------------------🔐 INDIIC 🔐------------------------ 

###### Autor: Nicolás Gangi  ---📘 Universidad Católica de Córdoba 📘---

In this repository you can find as of today (07/2020) a total of 12 projects for ESP32 with the IDF framework. 
 

### adc1:
Configuration of the ADC1 module through the "adc1_init()" function. 
To obtain a value of the AD converter, the function "adc1_get_value()" must be called, which returns an average value of 64 measurements. 
**How to use it:** Set the corresponding configuration in the FW, compile and load the program.

☼WORKING!


### blink:
Turns on and off the led embedded in the board of the ESP32 DEV KIT V1 (GPIO 2) every 1 second.
**How to use it:** Set the PIN number on the FW, compile and load the program.

☼WORKING!


### blufi:
It allows to share the information of a wifi network by means of a bluetooth connection. This makes the ESP32 module start without being connected to any WiFi network and wait for the respective information in the bluetooth connection. The tests were done using the Andriod and iOS device application provided by the manufacturer.
**How to use it:** Compile and load the FW. Use the "EspBlufi" app.

☼The example compiles and seems to work perfectly but at the moment of connecting it with the application it fails in the negotiation of the data.


### gpio:
Configure GPIO pins as input or output as indicated by the "gpio_setup()" function.
**How to use it:** Set the PIN number, IN or OUT on the FW, compile and load the program.

☼WORKING!


### json:
Create, read and print on screen Json objects.
**How to use it:** Compile, load the program and view the monitor.

☼WORKING!


### pwm:
First the configuration structure of the timer module is created and loaded to be PWM. Then the channel is configured to support PWM outputs. Once the structures are configured they are loaded and started by means of "pwm_init()". The functions "pwm_freq()" and "pwm_duty()" are used to modify the frequency and duty of the PWM module respectively anywhere in the FW. These functions change the configuration and update it.
**How to use it:** Load the data into the timer and channel structures. Then compile and load the program. 

☼WORKING!


### sc_mqtt:
It allows to generate a connection to a WiFi network through the ESP-TOUCH smart configuration protocol and once this is achieved, it tries to access a MQTT broker to subscribe and publish topics.
**TODO:** First see if the WiFi network information is in memory. If it is, connect to it. If not, use ESP-TOUCH to get the information, connect and save it in memory. 

☼WORKING ON


### simple_ota_example:
A ".json" file containing the version number allows the firmware to be updated via OTA. The ".bin" file with the new code must be uploaded to the web host which is detailed in the code. The new firmware is stored in a memory partition dedicated to OTA upgrades, it consists of 2 partitions to put the current version and a previous one.
**How to use it:** 

☼WORKING! PD.: It does not make use of the CA certification.


### smart_config:
El protocolo ESP-TOUCH de Espressif implementa la tecnología Smart Config para ayudar a los usuarios a conectar los dispositivos incorporados ESP8266EX y ESP32 a una red Wi-Fi 
a través de una simple configuración en un teléfono inteligente. 

☼WORKING!


### station:
Este ejemplo permite conectarse a una red WiFi especificada en menuconfig.

☼WORKING!


### tcp:
Genera una conexion TCP con un broker MQTT el cual es seteado en el código. Una vez lograda la conexión exitosa se procede a la subscripcion, desuscripción y publicacion en un tema dado. Las pruebas con alguna aplicacion de dashboard MQTT para telefono inteligente (en este caso MyMQTT) en la cual se puede susbscrir a un tema y ver los mensajes en viados por el ESP32 o publicar en un tema, en este caso los mensajes se ven en la consola del ESP32. 
    
```
Información del broker MQTT:
        - Broker host: mqtt.dioty.co
        - Broker port: 1883
        - Secure Broker port: 8883. Please download the ca_cert file from GitHub
        - WebSockets port: 8080
        - Secure WebSockets port: 8880
        - User id: 1605558@ucc.edu.ar
        - Your root topic: "/1605558@ucc.edu.ar/"
        - Password: Your password will be send to you via email.
```

☼WORKING!


### timer:

☼WORKING!


Cabe aclarar que cada ejemplo cuenta con su respectivo archivo README dentro de las carpetas contenedoras en donde se encuentra una explicación mas detallada del mismo.