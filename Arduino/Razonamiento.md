# Desarrollo del '*programa_definitivo.ino*'

En este documento se detalla el funcionamiento del programa de Arduino.

Son necesarias las **librerías**:
  - Wire.h
  - MPU6050.h
  - I2Cdev.h

## SetUp

El SetUp tiene la forma:
<p align="center">
<img src="https://github.com/eromerol/Smart-Cube-ESP8266/assets/145780382/3ec1909c-5a2e-47a7-b659-b6bda05bbcfb" width=600 heigt=300>

Donde se inicializa el sensor MPU6050 y se asigna el pin 16 (Vibration_signal) del sensor de vibración como entrada.

## Loop

Una vez dentro del bucle principal, obtenemos las lecturas de la rotación y de la aceleración:
<p align="center">
<img src="https://github.com/eromerol/Smart-Cube-ESP8266/assets/145780382/e42afc8c-7d1c-4a4a-98b6-fa321014cadc" width=400 heigt=100>

Y se le asignan a las variables creadas previamente:

<p align="center">
<img src="https://github.com/eromerol/Smart-Cube-ESP8266/assets/145780382/5c9f5e09-9cc1-4ab5-80ac-49605efde076" width=400 heigt=100>

**NOTA:** Si vamos al [repositorio de la biblioteca MPU6050](https://github.com/ElectronicCats/mpu6050), dentro de 'src' encontramos la deifinición de los procedimientos en la carpeta MPU6050.cpp:

- **getRotation**
<p align="center">
<img src="https://github.com/eromerol/Smart-Cube-ESP8266/assets/145780382/61b1f736-b5ac-4a50-8fb9-67414e7c6960" width=600 heigt=600>

- **getAcceleration**
<p align="center">
<img src="https://github.com/eromerol/Smart-Cube-ESP8266/assets/145780382/ed7cf806-35cc-458a-8d7d-382aebef0202" width=600 heigt=600>



Donde se lee que las lecturas son de tipo entero de 16 bits y están en formato complemento a 2, por lo que después se les hace la conversión al sistema internacional.

**NOTA:** los rangos con los que está configurado el MPU6050 pueden ser 2g/4g/8g/16g para el acelerómetro y 250/500/1000/2000(°/s) para el giroscopio. Trabajaremos con los rangos por defecto (2g y 250°/s). La resolución de las lecturas es de 16 bits, por lo que el rango de lectura es de -32768 a 32767.

Sabiendo estos datos, la conversión para la lectura del giroscópio y la del acelerómetro queda como se muestra:

<p align="center">
<img src="https://github.com/eromerol/Smart-Cube-ESP8266/assets/145780382/96d42ee0-cfff-41a1-aa8b-29c2d6da3c78" width=400 heigt=200>

## Cálculos de las rotaciones y las aceleraciones

Para calcular el ángulo actual necesitamos integrar la velocidad y conocer el ángulo incial. Esto lo hacemos usando la siguiente formula:

<p align="center">
<img src="https://github.com/eromerol/Smart-Cube-ESP8266/assets/145780382/905e662d-5b2a-4f8b-967a-14a17f6592d9" width=150 heigt=100>

<img src="https://github.com/eromerol/Smart-Cube-ESP8266/assets/145780382/dbbc3b80-93a4-43f9-97d1-0f6b8d36a5e1" width=300 heigt=200>

Por lo que se declara: *(no sé por qué lo divide por 131)*

<p align="center">
<img src="https://github.com/eromerol/Smart-Cube-ESP8266/assets/145780382/e77f92c8-dcf1-4098-8c06-d020185299f0" width=400 heigt=200>

---

Se calcula la **inclinación en cada eje** con la siguiente conversión.
La única fuerza que actúa sobre el sensor es la fuerza de la **gravedad**, por lo que los valores que obtenemos en las componentes del acelerómetro corresponden a la gravedad.

<p align="center">
<img src="https://github.com/eromerol/Smart-Cube-ESP8266/assets/145780382/9036521c-3fb6-4118-bbca-0ec522d843ba" width=600 heigt=300>

De donde se puede deducir:

<p align="center">
<img src="https://github.com/eromerol/Smart-Cube-ESP8266/assets/145780382/38a38759-3712-4f35-b601-4441118a867e" width=200 heigt=100>

Obteniendo los ángulos de inclinación:

<p align="center">
<img src="https://github.com/eromerol/Smart-Cube-ESP8266/assets/145780382/3fe591fe-ae0e-4290-aebb-ff47e098123a" width=600 heigt=300>

[EJEMPLO GET TEMPERATURE](https://www.i2cdevlib.com/forums/topic/612-why-the-3653-degree-offset-to-get-temperature/)
https://www.i2cdevlib.com/forums/topic/612-why-the-3653-degree-offset-to-get-temperature/
