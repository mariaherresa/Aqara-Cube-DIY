# Control mediante dispositivo Alexa

## Configuración de nodos Alexa en Node-RED

### Instalar App
NOTA: Para hacer los pasos no es necesario tener un Alexa.
- Descargar la app Amazon Alexa y seguir los pasos para su configuración.
- Una vez la app esté configurada, le damos a Más -> Skills y juegos.
- Descargamos Spotify y Node-RED Smart Home Control y autorizamos el uso de ambas.
  
### Creamos los dispositivos a utilizar en Node-RED
- Nos vamos al siguiente enlace: https://red.cb-net.co.uk/
- Creamos una cuenta e iniciamos sesión.
- En https://red.cb-net.co.uk/devices añadimos los dispositivos necesarios junto a su descripción y la habitación en la que están.
- Estos dispositivos, una vez creados, deberían de aparecer en al app de Alexa, en el apartado de Dispositivos.
  
---

He creado los dispositivos del ejemplo: **sensor de temperatura** y **lámpara**.

<p align="center">
<img src="https://github.com/eromerol/Smart-Cube-ESP8266/assets/145780382/65444f5a-be8e-438e-a2de-b37bb27ba240" weight=300 height=600>

### Configuración en Node-RED
- Instalamos el paquete node-red-contrib-alexa-smart-home en Manege-Palette.
- Para poder usar los nodos necesitamos crear un nuevo autor, introduciendo los datos de la cuenta de https://red.cb-net.co.uk/
- Para añadir los dispositivos que poseemos en la app de Alexa usamos el nodo: alexa-smart-home-v3-state.
- Para añadir el propio Alexa usamos el nodo: alexa-smart-home-v3.

<p align="center">
<img src="https://github.com/eromerol/Smart-Cube-ESP8266/assets/124197400/1d11f738-f66b-44ba-9ae5-afb4abee3725" weight=150 height=350>

---

## Desarrollo de nuestro proyecto

Se crea un nuevo dispositivo llamado **termostato**.

<p align="center">
<img src="https://github.com/eromerol/Smart-Cube-ESP8266/assets/145780382/14bd6b27-a051-4727-9fc2-9346e3328a04" weight=300 height=600>

Se añade al flujo, junto con los dispositivos ya proporcionados por el profesor:

<p align="center">
<img src="https://github.com/eromerol/Smart-Cube-ESP8266/assets/145780382/252ee901-1288-41f3-9c31-75b449ccf106" weight=600 height=400>

Y creamos una función para interpretar las órdenes para el termostato:

<p align="center">
<img src="https://github.com/eromerol/Smart-Cube-ESP8266/assets/145780382/7682f6db-89b3-486a-894a-acfd57cab13b" weight=200 height=200>

La función recoge el mensaje cuando la orden es de [ajustar la temperatura](https://docs.cb-net.co.uk/en/development/node-outputs.html) del termostato y transmite el payload. Como se indica, msg.payload = 1 significa incrementar en 1ºC la temperatura, y -1, disminuirla 1ºC.
