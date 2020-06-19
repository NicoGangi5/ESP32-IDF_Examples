# ------------------------🔐 ESP32-IDF_Examples 🔐------------------------ 

###### Autor: Nicolás Gangi  ---📘 Universidad Católica de Córdoba 📘---



En este repositorio se encuentran a fecha de hoy (07/04/2020) con un total de 11 proyectos para el ESP32 con el framework IDF. 


### ADC:

☼FUNCIONANDO!


### blink:
Enciende y apaga el led incoporado en la placa del ESP32 DEV KIT V1 (GPIO 2) cada 1 segundo 

☼FUNCIONANDO!


### blufi:
Permite compartir la informacion de una red wifi por medio de una conexión bluetooth. Esto hace que el módulo ESP32 se inicie sin estar conectado a ninguna red WiFi y espere
por la respectiva informacion en la conexión bluethoot. Las pruebas se hicieron usando la aplicación para dispositivos Andriod y iOS brindada por el fabricante.

☼El ejemplo compila y parece funcionar perfectamente pero al momento de conectarlo con la aplicacion falla en la negociación de los datos.


### GPIO:

☼FUNCIONANDO!


### Json:

☼FUNCIONANDO!


### PWM:

☼FUNCIONANDO!


### sc_mqtt:
Permite generar una conexion a una red wifi por medio del protocolo ESP-TOUCH smart configuration y una vez lo grado esto intenta acceder a un broker MQTT para subscribirse y publicar topics.

☼WORKING ON


### simple_ota_example:
Por medio de un archivo .json el cual contiene el numero de version permite la actualización del firmware via OTA. El archivo .bin con el nuevo código debe ser subido al host web el cual
se detalla en el código. El firmware nuevo se aloja en una partición de memoria destinada a actualizaciones OTA, consta de 2 particiones para poner tener la versión actual y una anterior. 

☼FUNCIONANDO! PD.: No hace uso de la certificacion CA.


### smart_config:
El protocolo ESP-TOUCH de Espressif implementa la tecnología Smart Config para ayudar a los usuarios a conectar los dispositivos incorporados ESP8266EX y ESP32 a una red Wi-Fi 
a través de una simple configuración en un teléfono inteligente. 

☼FUNCIONANDO!


### station:
Este ejemplo permite conectarse a una red WiFi especificada en menuconfig.

☼FUNCIONANDO!


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

☼FUNCIONANDO!



Cabe aclarar que cada ejemplo cuenta con su respectivo archivo README dentro de las carpetas contenedoras en donde se encuentra una explicación mas detallada del mismo.
