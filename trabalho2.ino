#include <WiFi.h>
#include <PubSubClient.h>

const char* ssid = "Kessiner";
const char* password = "kessinao";
const char* mqttServer = "heloec2.duckdns.org";
const int mqttPort = 1883;
const char* mqttUser = "helo";
const char* mqttPassword = "Amendoin14";
const char* mqttTopicalternador = "esp32/alternador";
const char* mqttTopicpastilha_de_freio = "esp32/pastilha_de_freio";
const int sensorPinalternador = 36; // Potenciômetro do alternador
const int sensorPinpastilha_de_freio = 35; // Potenciômetro da pastilha

WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
  Serial.begin(115200);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.print("Conectado ao WiFi com endereço IP: ");
  Serial.println(WiFi.localIP());

  client.setServer(mqttServer, mqttPort);
  while (!client.connected()) {
    Serial.println("Conectando ao servidor MQTT...");
    if (client.connect("ESP32Client", mqttUser, mqttPassword)) {
      Serial.println("Conectado ao servidor MQTT!");
    } else {
      Serial.print("Falha na conexão - Estado: ");
      Serial.print(client.state());
      delay(2000);
    }
  }
}

void loop() {
  int sensorValuealternador = analogRead(sensorPinalternador);
  int sensorValuepastilha_de_freio= analogRead(sensorPinpastilha_de_freio);

  float alternador=((float)sensorValuealternador/4095)*100;
  float pastilha_de_freio=((float)sensorValuepastilha_de_freio/4095)*100;

  String alternadorPayload = String(alternador);
  String pastilha_de_freioPayload = String(pastilha_de_freio);

  Serial.print("Enviando dados para o Node-RED - alternador: ");
  Serial.println(alternador);
  client.publish(mqttTopicalternador, alternadorPayload.c_str());

  Serial.print("Enviando dados para o Node-RED - pastilha_de_freio: ");
  Serial.println(pastilha_de_freio);
  client.publish(mqttTopicpastilha_de_freio, pastilha_de_freioPayload.c_str());

  delay(1000);
}
