#include <Wire.h>
#include <MPU6050.h> //Biblioteca sensor
#include <I2Cdev.h>  //Biblioteca comunicacion I2C
#include "Button2.h" //Biblioteca pulsaciones
#include <ArduinoJson.h> //Biblioteca JSON
#include <PubSubClient.h> //Biblioteca MQTT
#include <ESP8266WiFi.h> // Biblioteca WIFI ESP8266
#include <ESP8266HTTPClient.h> //Biblioteca HTTP ESP8266
#include <string>
#include <iostream>
#include <sstream>

/////////////////////////////////////////////////////////////////

#define BUTTON_PIN  12
#define MPU 0x68

/////////////////////////////////////////////////////////////////

Button2 button;
MPU6050 mpu;

// Inicializamos el cliente WiFi
WiFiClient espClient;
PubSubClient mqtt_client(espClient);

/////////////////////////////////////////////////////////////////
// Inicializamos el cliente HTTPS

HTTPClient https;

//#define __HTTPS__
#ifdef __HTTPS__
  #include <WiFiClientSecure.h>
  WiFiClientSecure ClienteWiFi;
  const String URL_BASE = "https://iot.ac.uma.es:1880";
  // huella digital SHA-1 del servidor iot.ac.uma.es (vencimiento enero 2024)
  const char* fingerprint = "DE:3C:76:79:45:D8:F0:13:9F:22:A5:42:97:0B:F6:56:4E:E6:B8:FD";
#else
  #include <WiFiClient.h>
  WiFiClient ClienteWiFi;
  const String URL_BASE = "http://192.168.0.32:1880";
#endif


/////////////////////////////////////////////////////////////////

// Definimos pines generales
int pinled = 13;
int led = 2;
int Vibration_signal = 14;

// Declaramos variables
int brillo = 0;
int lamp = 0;
String cerradura = "LOCKED";
float temperatura;
int Cortina_prev = 40;
int Cortina;
int termostato;
int termostato_prev = 25;
int volumen;
int volumen_prev = 30;
int contador = 1;
bool subir = true;
int codigoStatus;
String respuesta = "";

// Definimos datos de la red WiFi
const String ssid = "infind";
const String password = "1518wifi";

// Definimos el servidor MQTT
const String mqttServer = "iot.ac.uma.es";
const String mqttUser = "II7";
const String mqttPassword = "hbum58oh";

// Definimos String
String ID_PLACA;
String topic_Alexa;

// Variables del acelerometro
int16_t ax, ay, az; 

// Variables del giroscopio
int16_t gx, gy, gz;
int16_t temp_raw;

long tiempo_prev, dt;
float girosc_ang_x, girosc_ang_y, girosc_ang_z;
float girosc_ang_x_prev, girosc_ang_y_prev, girosc_ang_z_prev;

float umbral = 70.0; // Umbral para detección de giro
//-------------------------------------------------------
//                     CONECTA WIFI
//-------------------------------------------------------
void conecta_wifi() {
  Serial.println("Conectando a " + ssid);
 
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(200);
    Serial.print(".");
  }
  Serial.println();
  Serial.println("WiFi conectado, direccion IP: " + WiFi.localIP().toString());
}

//-------------------------------------------------------
//                     CONECTA MQTT
//-------------------------------------------------------
void conecta_mqtt() {
  // Bucle hasta que estemos reconectados

  while (!mqtt_client.connected()) {
    Serial.println("Conectando al servidor MQTT...");

    // Intentamos conectarnos
    if (mqtt_client.connect(ID_PLACA.c_str(), mqttUser.c_str(), mqttPassword.c_str())) {
      Serial.println("Conectado a: " + mqttServer);

    } else {
       Serial.println("ERROR:"+ String(mqtt_client.state()) +" reintento en 5s" );

      // Esperamos 5 segundos antes de volver a intentarlo
      delay(5000);
    }
  }
}

//-----------------------------------------------------------
//                Peticiones HTTP
//-----------------------------------------------------------
int http_GET(String URL, String* respuesta)
{
  return peticion_HTTP("GET", URL, "", respuesta);
}
//-----------------------------------------------------------
int http_POST(String URL, String body, String* respuesta)
{
  return peticion_HTTP("POST", URL, body, respuesta);
}
//-----------------------------------------------------------
int peticion_HTTP (String metodo, String URL, String body, String* respuesta)
{
  int httpCode=-1;
  unsigned long start = millis();
  if (https.begin(ClienteWiFi, URL)) {  // HTTPS

      Serial.println(metodo +" petición... " + URL);
      // start connection and send HTTP header
      https.addHeader("Content-Type", "application/json");
      if(metodo=="GET" )  httpCode = https.GET();
      if(metodo=="POST"){ httpCode = https.POST(body);
        Serial.println("cuerpo solicitud: \n     "+ body);
      }
      // httpCode will be negative on error
      if (httpCode > 0) {
        Serial.println(metodo +" respuesta... código Status: "+ String(httpCode));

        // queremos la respuesta del servidor
        if (respuesta!=NULL) {
          *respuesta = https.getString();
          Serial.println("cuerpo respuesta: \n     "+ *respuesta);
        }
      } else {
        Serial.println(metodo +"... falló, error: "+ String(https.errorToString(httpCode).c_str()) );
      }

      https.end();
    } else {
      Serial.println("No se pudo conectar");
    }
    Serial.println("tiempo de respuesta: "+ String(millis()-start) +" ms\n");
    return httpCode;
}



//-------------------------------------------------------
//                      SETUP
//-------------------------------------------------------
void setup(void) {
  respuesta="";
  Serial.begin(115200);

  Wire.begin();
  Wire.beginTransmission(MPU);
  Wire.endTransmission(true);

  pinMode(pinled, OUTPUT);
  pinMode(Vibration_signal, INPUT); //Set pin as input

  mpu.initialize(); // Inicializamos el giroscopio

  // Deteccion de pulsacion del boton
  button.begin(BUTTON_PIN);
  button.setClickHandler(singleClick);
  button.setLongClickHandler(longClick);
  button.setDoubleClickHandler(doubleClick);
  button.setTripleClickHandler(tripleClick);
  //button.setLongClickHandler(longClickDetected);
  
  ID_PLACA = "ESP" + String(ESP.getChipId()); //Obtenemos el chipID

  // Creamos los topics
  topic_Alexa="II7/"+ ID_PLACA +"/Alexa"; //Publicar

  // Nos conectamos al WiFi
  conecta_wifi();

   // Inicializamos el servidor MQTT y nos conectamos
  mqtt_client.setServer(mqttServer.c_str(), 1883);
  mqtt_client.setBufferSize(512); // para poder enviar mensajes de hasta X bytes
  conecta_mqtt();

  #ifdef __HTTPS__
  ClienteWiFi.setFingerprint(fingerprint); // se comprobará el certificado del servidor
  //sslClienteWiFi.setInsecure(); // si no se quiere comprobar el certificado del servidor
  #endif
    
  if (mpu.testConnection()) Serial.println("Sensor iniciado correctamente");
  else Serial.println("Error al iniciar el sensor");
  
  Serial.println("MPU6050 Found!");

  // Calibramos el giroscopio
  mpu.CalibrateGyro(6);

  // Inicializamos el termostato
  termostato=termostato_prev;
  
  // Inicializamos el volumen
  volumen=volumen_prev;

  // Inicializamos la cortina
  Cortina=Cortina_prev;

  // Inicilizamos la temperatura
  temperatura=20;
  
  delay(100);

  tiempo_prev = millis();
  Serial.println("Fin setup");
}
//-------------------------------------------------------
//                      LOOP
//-------------------------------------------------------
void loop() {
  if (!mqtt_client.connected()) conecta_mqtt();
  
  mqtt_client.loop(); // esta llamada para que la librería recupere el control

  button.loop(); //Comprueba todo el tiempo el tipo de pulsación

}

//-------------------------------------------------------
//               FUNCIONES PULSACIONES
//-------------------------------------------------------
void singleClick(Button2& btn) {
  
  Serial.println("click\n");

  // Desbloqueamos la cerradura de casa 
  cerradura="UNLOCKED";
  Alexa();
}
//void longClickDetected(Button2& btn) {
//    Serial.println("long click detected");
//}
void longClick(Button2& btn) {

  Serial.println("long click\n");

  // Almacenamos el tiempo de pulsacion
  unsigned int time= btn.wasPressedFor();
  if (time>1500){ // Si es mayor a 1.5 Segundos, se llama al loop 2
    loop2();
  }
}
void doubleClick(Button2& btn) {

    Serial.println("double click\n");

    // Bloqueamos la cerradura de casa 
    cerradura="LOCKED";
    Alexa();
}
void tripleClick(Button2& btn) {
    Serial.println("triple click\n");
     
    if (Cortina==100){
      Cortina = Cortina_prev - 20;
      subir=false;
    } else if (Cortina==0) {
      Cortina = Cortina_prev + 20;
      subir=true;
    } else {
        if (subir==true){
        Cortina = Cortina_prev + 20;
      } else{
        Cortina = Cortina_prev - 20;
      }
    }
    Alexa();
    Cortina_prev=Cortina;
}
//-------------------------------------------------------
//               PUBLICACION ALEXA
//-------------------------------------------------------
void Alexa()
{
  // Construimos el objeto JSON para el topic 'Alexa'
  StaticJsonDocument<300> jsonDoc;
  jsonDoc["Lampara"] = lamp;
  jsonDoc["Volumen"] = volumen;
  jsonDoc["Temperatura"] = temperatura;
  jsonDoc["Termostato"] = termostato;
  jsonDoc["Cortina"] = Cortina;
  jsonDoc["Lock"] = cerradura;

  // Serializar el objeto JSON a una cadena
  String jsonPayload;
  serializeJson(jsonDoc, jsonPayload);

  // Publicar el JSON en el topic 'Alexa'
  mqtt_client.publish(topic_Alexa.c_str(), jsonPayload.c_str());
  Serial.println("\nTopic   : "+ topic_Alexa);
  Serial.println("Payload : "+ jsonPayload);
}
//-------------------------------------------------------
//               PULSACION LARGA
//-------------------------------------------------------
void loop2(){
  // Leer las aceleraciones y velocidades angulares, todas de tipo entero de 16 bits y en complemento a 2
  mpu.getAcceleration(&ax, &ay, &az);
  mpu.getRotation(&gx, &gy, &gz);

  // Conversion a sistema internacional
  float ax_m_s2 = ax * (9.81/16384.0);
  float ay_m_s2 = ay * (9.81/16384.0);
  float az_m_s2 = az * (9.81/16384.0);
  float gx_deg_s = gx * (250.0/32768.0);
  float gy_deg_s = gy * (250.0/32768.0);
  float gz_deg_s = gz * (250.0/32768.0);

  //Calcular los angulos de inclinacion:
  float accel_ang_x=atan(ax/sqrt(pow(ay,2) + pow(az,2)))*(180.0/3.14);
  float accel_ang_y=atan(ay/sqrt(pow(ax,2) + pow(az,2)))*(180.0/3.14);
  float accel_ang_z=atan(az/sqrt(pow(ax,2) + pow(ay,2)))*(180.0/3.14);

  // Necesitamos el incremento de tiempo
  dt = millis()-tiempo_prev;
  tiempo_prev=millis();
  
  // El nuevo angulo de rotacion es el incremento de tiempo por la velocidad angular (gx) + la rotacion anterior 
  girosc_ang_x = (gx/131)*dt/1000.0 + girosc_ang_x_prev;
  girosc_ang_y = (gy/131)*dt/1000.0 + girosc_ang_y_prev;
  girosc_ang_z = (gz/131)*dt/1000.0 + girosc_ang_z_prev;

  girosc_ang_x_prev=girosc_ang_x;
  girosc_ang_y_prev=girosc_ang_y;
  girosc_ang_z_prev=girosc_ang_z;

  // Limitamos que la lectura sea entre 0 y 180 (fmod devuelve el resto de dividir girosc_ang_z/180)
  float girx = fmod(girosc_ang_x,float(180));
  float giry = fmod(girosc_ang_y,float(180));
  float girz = fmod(girosc_ang_z,float(180));

  //Mostrar los angulos separadas por un [tab]
  Serial.print("Rotacion en X: ");
  Serial.print(girx); 
  Serial.print("\tRotacion en Y: ");
  Serial.print(giry);
  Serial.print("\tRotacion en Z: ");
  Serial.print(girz);
  
  // Registramos la temperatura y la convertimos a grados celsius
  temp_raw = mpu.getTemperature(); // Temperatura en formato int_16 y en complemento a 2
  temperatura = (temp_raw + 521)/340 + 35.0;

  // ---------- INTERPRETACION DE LAS LECTURAS ------------

  // ----------- ORDENES DESDE DASHBOARD ---------

  int codigoStatus = http_GET(URL_BASE+"/brillo", &respuesta);
  if (codigoStatus == 200) { // Si el código de estado HTTP es 200 (OK)
      // Interpretar la respuesta
      StaticJsonDocument<200> doc;
      DeserializationError error = deserializeJson(doc, respuesta);
      if (error) {
        Serial.println("Falló la deserialización del JSON");
      } else {
        // Extraer los valores de la respuesta
        lamp = doc["brillo"];
        Serial.println("Brillo desde dashboard: " + String(lamp));
        Alexa();
      }
    } else {
      Serial.println("Falló la petición GET, código de estado: " + String(codigoStatus));
    }

  // ------------ VIBRACIONES ---------------

  Serial.print("\nVibration status: ");
  bool Sensor_State = digitalRead(Vibration_signal);
  if (Sensor_State == true) {
    Serial.println("Sensing vibration\n");

    if (contador==1){
      String payload = "{\"power\":\"" + String("ON") + "\"}";

      codigoStatus = http_POST(URL_BASE+"/power", payload, &respuesta);

      contador = contador + 1;
      
    }else{
      String payload = "{\"power\":\"" + String("OFF") + "\"}";

      codigoStatus = http_POST(URL_BASE+"/power", payload, &respuesta);

      contador = 1; //Reseteamos el contador
    }
    
    delay(500);
  }
  else if(Sensor_State == false){
    Serial.println("No vibration\n");
    delay(500);

    // Si la rotación en el eje X supera el umbral establecido
    if (abs(accel_ang_x) >= umbral) {
      Serial.println("\n¡Giro en el eje X detectado! " +  String(girx) + "\t Musica");
      if(accel_ang_x>0){   // Pasa a la cancion siguiente
        if (volumen_prev<100){
          volumen = volumen_prev + 1;
        }
      } else if (accel_ang_x<0){   // Retrocede a la cancion anterior
        if (volumen_prev>0){
          volumen = volumen_prev - 1;
        }
      } 
      Alexa();
      volumen_prev=volumen;   
    }
    // Si la rotación en el eje Y supera el umbral establecido
    else if (abs(accel_ang_y) >= umbral) {
      Serial.println("\n¡Giro en el eje Y detectado! " + String(giry) + "\t Termostato");
      // Realiza alguna acción aquí (p. ej., encender un LED, enviar una señal, etc.)
      if(accel_ang_y<0){
        termostato = termostato_prev - 1;   // Decrementa un grado 
      }else if(accel_ang_y>0){
        termostato = termostato_prev + 1;    // Incrementa un grado
      }
      termostato_prev = termostato; // Lo almacenamos
      // Publicamos en Alexa
      Alexa();
    }
    // Si la rotación en el eje Z supera el umbral establecido
    else if (abs(girz) >= umbral) {
      Serial.println("\n¡Giro en el eje Z detectado!: " + String(girz) + "\t Lampara");
      // Limita la intensidad de la lampara de 0 a 100
      lamp = map(abs(girz), 0,180, 100,0);
      lamp = constrain(lamp,0,100);
      // Publicamos en Alexa
      Alexa();

      // Limita la intensidad del LED de 0 a 255
      brillo = map(lamp, 0,100, 255,0);
      brillo = constrain(brillo,0,255);
      analogWrite(pinled, brillo);   // Lo manda al LED
    }
      
  }

  delay(100);
}
