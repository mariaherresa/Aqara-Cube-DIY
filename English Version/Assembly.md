The Aqara Cube is a smart device that can be used to control your home. It has six sides, each with a different sensor that can detect motion, gestures, and sounds.

The Aqara Cube can be used to control a wide range of smart devices, such as lights, thermostats, doors, and windows. It can also be used to create automations, such as turning on the lights when you enter a room or turning off the thermostat when you leave home.

Our handmade version will take care of the same features and in this document we will show you how to do it:

🔌 COMPONENTS AND SUPPLIES
--------------------------

<img align="right" src="https://github.com/isbkch/esp32-aws-weather-station/blob/master/docs/esp-32.jpg?raw=true" style="max-width:100%;" height="200">

*   [ESP8266 Board](https://amzn.eu/d/aHI7mCz)
*   [3 Axis Accelerometer Gyroscope MPU-6050](https://amzn.eu/d/f57rFlo) 
*   [Vibration Sensor SW-420](https://amzn.eu/d/2fdRQOv) 
*   [5 LiPo Batteries And Charger](https://amzn.eu/d/7FSwxyN)
*   [Breadboard](https://amzn.eu/d/8O4BMnn)
*   [Jumper Wires](https://amzn.eu/d/6FpwoLk)

👨‍💻​APPS
------

*   [Arduino IDE](https://www.arduino.cc/en/software)
*   [Fritzing](https://fritzing.org/)
*   [Node-RED](https://nodered.org/)
*   [Mongo DB](https://www.mongodb.com/es)
*   [Telegram](https://web.telegram.org/)

📚Libraries
=====
*   [Adafruit_MPU6050](https://github.com/adafruit/Adafruit_MPU6050)
*   [Adafruit_Sensor](https://github.com/adafruit/Adafruit_Sensor)
*   [PubSubClient](https://www.arduinolibraries.info/libraries/pub-sub-client)
*   [Wire](https://github.com/espressif/arduino-esp32/tree/master/libraries/Wire)
*   [WiFi](https://github.com/espressif/arduino-esp32/tree/master/libraries/WiFi)



COMPONENTS DESCRIPTION
==============================
### MPU6050

The MPU-6050 is a 6-axis motion sensor that combines a 3-axis accelerometer and a 3-axis gyroscope in a single chip. It is a popular choice for a variety of applications, including robotics, drones, and virtual reality.

The MPU-6050 can measure acceleration and angular velocity in three axes: x, y, and z. This data is going to be used to track the movement of an object, such as the position, orientation, or rotation of our magic cube.

The connection diagram of this device with the ESP8266 board, together with the pinout of the component, can be seen in the following images:

![image](https://github.com/eromerol/Smart-Cube-ESP8266/assets/124197400/73b19861-db9b-42c4-96d1-a6d4bfbdbda7)

![conexion_esp8266_mpu6050](https://github.com/eromerol/Smart-Cube-ESP8266/assets/124197400/a3d7a836-a8cd-406d-9d5a-cf6c957a9cb2)


Info via: [Link1](http://www.sinaptec.alomar.com.ar/2017/10/tutorial-23-esp8266-obtener-inclinacion.html)
          [Link2](https://randomnerdtutorials.com/esp8266-nodemcu-mpu-6050-accelerometer-gyroscope-arduino/)

### SW-420

The SW-420 is a sensor that detects vibrations from a threshold that you can set with the included potentiometer. The module is based on the SW-420 sensor and an LM393 comparator producing a TTL signal when activated.

The SW-420 motion vibration sensor is a simple sensor that uses a single data pin to detect motion. In addition to this, the onboard potentiometer provides a simple way to adjust the level of motion needed to trigger the digital pin.

No ESP8266 specific libraries are required as this only uses the single digital pin. However it is important to ensure you connect the VCC pin to the 3.3 volts line. This is because this is the maximum voltage accepted by the digital pins.

The connection diagram of this device with the ESP8266 board, together with the pinout of the component, can be seen in the following images:

![image](https://github.com/eromerol/Smart-Cube-ESP8266/assets/124197400/0fb04cb4-2d2e-4b0d-878b-456949d6580f)

![conexion_esp8266_sw420](https://github.com/eromerol/Smart-Cube-ESP8266/assets/124197400/8bf33d0d-faca-4e17-8df2-ab275bcd34fd)

Info via: [Link1](https://chewett.co.uk/blog/1849/using-the-sw-420-motion-vibration-sensor-with-a-wemos-d1-mini-esp8266/)


### SCHEMATIC DESIGN

The complete schematic design, including both components described above together with the board would be as follows:

![conexion_esp8266_mpu6050_sw420](https://github.com/eromerol/Smart-Cube-ESP8266/assets/124197400/3d7beb1b-dcb0-45f7-9d3a-9f52a9a9f4a0)

This image, like the previous schematics, is made with the fritzing application. The component files can be found in the following folder: [Fritzing](https://github.com/eromerol/Smart-Cube-ESP8266/tree/main/Fritzing)
