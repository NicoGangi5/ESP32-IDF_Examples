# --------------------🔐 ESP32 - IDF 🔐-------------------- 

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
A ".json" file containing the version number allows the firmware to be updated via OTA. The ".bin" file with the new code must be uploaded to the web host which is detailed in the code. The new firmware is stored in a memory partition dedicated to OTA upgrades, it consists of 2 partitions to put the current version and a previous one.Every 30 seconds the FW will search the web host's ".json" file to see if the existing version is equal to or greater than the current one. In case it is higher, the new ".bin" will be downloaded and reboot automatically.
**How to use it:** Modify the code, compile it and upload the ".bin" file to the web host. Then modify the ".json" file inside the host with the new version number (same as the new FW).
Remember that the new FW must keep the OTA functions in order to continue being able to update.


<img width="1438" alt="OTA_2" src="https://user-images.githubusercontent.com/29799634/86482074-95a1bb00-bd27-11ea-91dc-25b7aa4a60a5.png">
<img width="897" alt="OTA_1" src="https://user-images.githubusercontent.com/29799634/86482069-92a6ca80-bd27-11ea-865e-e439c6449d58.png">


☼WORKING! PD.: It does not make use of the CA certification.


### smart_config:
The ESP-TOUCH protocol implements the Smart Config technology which allows through a mobile application to share the data of the WiFi network to which it is connected. The module receives this information and automatically connects to the network.
**How to use it:** Compile and load the program. Use the application "EspTouch: SmartConfig for ESP8266, ESP32" available for Android and iOS.

☼WORKING!


### station:
This example allows you to connect to a WiFi network specified into the FW.
**How to use it:** Load the information of the WiFi network you want to connect to into the FW. Compile and load the code.

☼WORKING!


### tcp:
It generates a TCP connection with a MQTT broker which is set in the code. Once the connection is successful we proceed to the subscription, unsubscription and publication in a given topic. The tests with some MQTT dashboard application for smartphone (MyMQTT) in which you can subscribe to a topic and see the messages sent by the ESP32 or publish in a topic, in this case the messages are seen in the console of the ESP32.  
**How to use it:** Upload the MQTT broker information, compile and load the program. Then you can see the information on the monitor.

☼WORKING!


### timer:
It generates a timer every 500ms which is used to change the state of a led. Every half second the code goes to the "periodic_timer_callback" function. In this case the timer is configured on a periodic basis.
**How to use it:** Set the time. Compile and load the program.

☼WORKING!


It should be noted that each example has its respective README file within the container folders where you can find a more detailed explanation of it.
