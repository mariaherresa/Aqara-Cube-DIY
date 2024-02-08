El cubo de Aqara es un dispositivo inteligente que se puede utilizar para controlar la domótica de tu hogar. Tiene seis lados, cada uno con un sensor diferente que puede detectar movimientos, gestos y sonidos.

El cubo de Aqara se puede utilizar para controlar una amplia gama de dispositivos inteligentes, como luces, termostatos, puertas y ventanas. También se puede utilizar para crear automatizaciones, como encender las luces cuando entras en una habitación o apagar el termostato cuando sales de casa.

Nuestra versión hecha a mano se encargará de las mismas caraterísticas, y en este documento te enseñaremos como hacerlo:

🔌 COMPONENTES Y BATERÍAS
--------------------------

<img align="right" src="https://github.com/isbkch/esp32-aws-weather-station/blob/master/docs/esp-32.jpg?raw=true" style="max-width:100%;" height="200">

*   [Placa ESP8266](https://amzn.eu/d/aHI7mCz)
*   [3 ejes acelerómetro giroscopio MPU-6050](https://amzn.eu/d/f57rFlo) 
*   [Sensor de Vibración SW-420](https://amzn.eu/d/2fdRQOv) 
*   [Baterías y Cargador LiPo](https://amzn.eu/d/7FSwxyN)
*   [Placa de pruebas](https://amzn.eu/d/8O4BMnn)
*   [Cables Dupont](https://amzn.eu/d/6FpwoLk)
*   Pulsador
*   Resistencias

👨‍💻​APPS
------

*   [Arduino IDE](https://www.arduino.cc/en/software)
*   [Fritzing](https://fritzing.org/)
*   [Node-RED](https://nodered.org/)
*   [Mongo DB](https://www.mongodb.com/es)
*   [Telegram](https://web.telegram.org/)

📚LIBRERÍAS
==========
*   [Adafruit_MPU6050](https://github.com/adafruit/Adafruit_MPU6050)
*   [MPU6050](https://github.com/jrowberg/i2cdevlib/tree/master/Arduino/MPU6050)
*   [PubSubClient](https://www.arduinolibraries.info/libraries/pub-sub-client)
*   [Wire](https://github.com/espressif/arduino-esp32/tree/master/libraries/Wire)
*   [WiFi](https://github.com/espressif/arduino-esp32/tree/master/libraries/WiFi)



DESCRIPCIÓN DEL HARDWARE
==============================
### MPU6050
El MPU-6050 es un sensor de movimiento de 6 ejes que combina un acelerómetro de 3 ejes y un giroscopio de 3 ejes en un solo chip. Es una opción popular para diversas aplicaciones, como robótica, drones y realidad virtual.

El MPU-6050 puede medir la aceleración y la velocidad angular en tres ejes: x, y y z. Estos datos se van a utilizar para seguir el movimiento de un objeto, como la posición, orientación o rotación de nuestro cubo mágico.

El esquema de conexión de este dispositivo con la placa ESP8266, junto con el pinout del componente, se puede ver en las siguientes imágenes:

<p align="center">
<img src="https://github.com/eromerol/Smart-Cube-ESP8266/assets/124197400/46695a02-ad73-451e-aa2e-697742e766d3" width=600 height=300>

    
<p align="center">
<img src="https://github.com/eromerol/Smart-Cube-ESP8266/assets/124197400/9ef484c0-5d0d-419f-b800-3151b1fdcd97" width=500 height=400>


Info vía: [Link1](http://www.sinaptec.alomar.com.ar/2017/10/tutorial-23-esp8266-obtener-inclinacion.html)
          [Link2](https://randomnerdtutorials.com/esp8266-nodemcu-mpu-6050-accelerometer-gyroscope-arduino/)

### SW-420

El SW-420 es un sensor que detecta vibraciones a partir de un umbral que puedes ajustar con el potenciómetro incluido. El módulo se basa en el sensor SW-420 y un comparador LM393 que produce una señal TTL cuando se activa.

El sensor de vibración de movimiento SW-420 es un sensor sencillo que utiliza un único pin de datos para detectar el movimiento. Además, el potenciómetro integrado proporciona una forma sencilla de ajustar el nivel de movimiento necesario para activar el pin digital.

No se necesitan librerías específicas para el ESP8266, ya que sólo utiliza el pin digital. Sin embargo, es importante asegurarse de conectar el pin VCC a la línea de 3,3 voltios. Esto se debe a que este es el voltaje máximo aceptado por los pines digitales.

El diagrama de conexión de este dispositivo con la placa ESP8266, junto con el pinout del componente, se puede ver en las siguientes imágenes:

<p align="center">
<img src="https://github.com/eromerol/Smart-Cube-ESP8266/assets/124197400/22e217f5-1d83-432e-b3ff-43aa797a11b2" width=600 height=300>

<p align="center">
<img src="https://github.com/eromerol/Smart-Cube-ESP8266/assets/124197400/842322b1-bf7d-4206-b93c-53274cbb4ba2" width=500 height=400>


Info vía: [Link1](https://chewett.co.uk/blog/1849/using-the-sw-420-motion-vibration-sensor-with-a-wemos-d1-mini-esp8266/)


### ESQUEMÁTICO COMPLETO

Para el desarrollo completo del cubo, se han añadido un **botón de presencia** (pulsador) y un **LED**. El esquemático completo, incluyendo los dos componentes descritos anteriormente, el pulsador, el LED y la placa, sería el siguiente:
<p align="center">
<img src="https://github.com/eromerol/Smart-Cube-ESP8266/assets/145780382/a305d1a0-a08e-4393-8b93-d0e8f610e64b" width=900 height=500>

**NOTA:** para la comunicación I2C se han añadido las resistencias de Pull-Up en las líneas SCL y SDA.

Esta imagen, al igual que los esquemáticos anteriores, está realizada con la aplicación fritzing. Los archivos de los componentes se encuentran en la siguiente carpeta: [Fritzing](https://github.com/eromerol/Smart-Cube-ESP8266/tree/main/Fritzing)
