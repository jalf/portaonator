
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

// Update these with values suitable for your network.

const char* ssid = ".....";
const char* password = ".....";
const char* mqtt_server = ".....";

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;

void setup() {
  pinMode(D7, OUTPUT);     // Inicializa a porta do transmissor como saida
//  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void setup_wifi() {

  delay(10);
  // Começa conectando ao wifi
//  Serial.println();
//  Serial.print("Conectando em ");
//  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
//    Serial.print(".");
  }

//  Serial.println("");
//  Serial.println("WiFi conectado");
//  Serial.println("IP : ");
//  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
//  Serial.print("Chegou mensagem [");
//  Serial.print(topic);
//  Serial.print("] ");
//  for (int i = 0; i < length; i++) {
//    Serial.print((char)payload[i]);
//  }
//  Serial.println();

  // Ativa se a mensagem for '1'
  if ((char)payload[0] == '1') {
    digitalWrite(D7, HIGH);   // Transmite o sinal do portão
    delay(2000);  // Transmite por 2 segundos
    digitalWrite(D7, LOW);   // desliga o transmissor
  }
}

void reconnect() {
  // Fica em loop até conectar
  while (!client.connected()) {
//    Serial.print("Tentando conectar ao servidor MQTT ...");
    // Attempt to connect
    if (client.connect("portaonator")) {
//      Serial.println("conectado");
      // Uma vez conectado, publique algo..
      client.publish("portaonator/out", "Alô do portaonator!");
      // ... e assine
      client.subscribe("portaonator/in");
    } else {
//      Serial.print("falhou, rc=");
//      Serial.print(client.state());
//      Serial.println(" Tentando novamente em 5 segundos");
      delay(5000);
    }
  }
}
void loop() {

  if (!client.connected()) {
    reconnect();
  }
  client.loop();
}
