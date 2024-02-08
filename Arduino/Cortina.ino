#include <ArduinoJson.h> // Biblioteca JSON
#include <WiFi.h>        // Biblioteca WiFi ESP32
#include <PubSubClient.h> // Biblioteca MQTT
#include <Stepper.h>   // Biblioteca para motores paso a paso

// Inicializamos el cliente WiFi
WiFiClient espClient;
PubSubClient mqtt_client(espClient);


// Motor paso a paso
int pasosPorVuelta = 2048;
int porcentajeanterior = 0;
// ULN2003 Motor Driver Pins
#define IN1 19
#define IN2 18
#define IN3 5
#define IN4 17
Stepper myStepper(pasosPorVuelta, IN1, IN3, IN2, IN4);

// Topic Cortina
String topic_Alexa;

// Definimos datos de la red WiFi
const String ssid = "MOVISTAR_1AFF_INV";
const String password = "971867820";

// Definimos el servidor MQTT
const String mqttServer = "iot.ac.uma.es";
const String mqttUser = "II7";
const String mqttPassword = "hbum58oh";

// Definimos el ID_PLACA(MAC) DEL ESP32
uint64_t esp32ChipId = ESP.getEfuseMac();
String ID_PLACA = String(esp32ChipId, HEX);
//-------------------------------------------------------
//                     CONECTA WIFI
//-------------------------------------------------------
void conecta_wifi()
{
  Serial.println("Conectando a " + String(ssid));

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(200);
    Serial.print(".");
  }
  Serial.println();
  Serial.println("WiFi conectado, dirección IP: " + WiFi.localIP().toString());
}

//-------------------------------------------------------
//                     CONECTA MQTT
//-------------------------------------------------------
void conecta_mqtt()
{
  // Bucle hasta que estemos reconectados
  while (!mqtt_client.connected())
  {
    Serial.println("Conectando al servidor MQTT...");

    // Intentamos conectarnos
    if (mqtt_client.connect(ID_PLACA.c_str(), mqttUser.c_str(), mqttPassword.c_str()))
    {
      Serial.println("Conectado a: " + String(mqttServer));
      mqtt_client.subscribe(topic_Alexa.c_str());
    }
    else
    {
      Serial.println("ERROR:" + String(mqtt_client.state()) + " reintento en 5s");

      // Esperamos 5 segundos antes de volver a intentarlo
      delay(5000);
    }
  }
}

//-----------------------------------------------------
void procesa_mensaje(char *topic, byte *payload, unsigned int length)
{
  String mensaje = String((char *)payload);
  Serial.println("Mensaje recibido [" + String(topic) + "] " + mensaje);

  // Verificamos el tema y actuamos en consecuencia
  if (String(topic) == topic_Alexa.c_str())
  {
    StaticJsonDocument<512> jsonDoc;
    DeserializationError error = deserializeJson(jsonDoc, mensaje);

    // Comprobar errores de deserialización
    if (error)
    {
      Serial.print("Error al deserializar JSON: ");
      Serial.println(error.c_str());
    }
    else
    {
      // Comprobar si el campo "Cortina" está presente
      if (jsonDoc.containsKey("Cortina"))
      {
        String estadoCortina = jsonDoc["Cortina"];
        int porcentaje = estadoCortina.toInt();
        // if (pocentaje < porcentajeanterior){
        //   for (int i = 0; i < porcentaje; i++)
        // myStepper.step(pasoPorVuelta);
        // } if else (pocentaje > porcentajeanterior){
        // myStepper.step(-pasoPorVuelta);
        // } else 
      
        if (porcentaje >= 0 && porcentaje <= 100)
        {
          Serial.println("Cortina: " + String(porcentaje) + "%");

          // Calcula la cantidad de pasos basándote en un rango de porcentaje
          int pasos = map(porcentaje, 0, 100,-pasosPorVuelta, pasosPorVuelta);
          int tiempo = map(abs(porcentajeanterior-porcentaje),0,100,0,20);
          porcentajeanterior = porcentaje;
          // Mueve el motor a la posición correspondiente al porcentaje recibi
          for (int i = 0; i < tiempo; i++){
           myStepper.step(pasos);
          }
        }
        else
        {
          Serial.println("Estado de la cortina desconocido: " + estadoCortina);
        }
      }
      else
      {
        Serial.println("Mensaje no contiene información sobre la cortina");
      }
    }
  }
}

void setup()
{
  Serial.begin(115200);
  Serial.println();
  Serial.println("Empieza setup...");
  myStepper.setSpeed(10);
  Serial.println("ID único del ESP32: " + ID_PLACA);
  topic_Alexa = "II7/ESP833252/Cortina";
  mqtt_client.setServer(mqttServer.c_str(), 1883);
  mqtt_client.setBufferSize(512); // para poder enviar mensajes de hasta X bytes
  mqtt_client.setCallback(procesa_mensaje);
  conecta_wifi();
  conecta_mqtt(); // Mover esta línea aquí
  Serial.println("Fin setup...");
}

void loop()
{
  if (!mqtt_client.connected())
  {
    conecta_mqtt();
  }

  mqtt_client.loop(); // Esta llamada permite que la librería recupere el control
}
