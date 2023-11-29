#include <ETH.h>
#include <PubSubClient.h> // go to Sketch --> Include Library --> Manage Libraries... --> search for PubSubClient by Nick O'Leary
#include <string.h>

#define RELAY1  32
#define RELAY2  33
#define BUTTON  34

// MQTT Broker
const char *mqtt_broker = "broker.emqx.io";
const char *topic = "emqx/ESP32-EVB_GPIO";
const char *mqtt_username = "emqx";
const char *mqtt_password = "public";
const int mqtt_port = 1883;

WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
    // Set software serial baud to 115200;
    Serial.begin(115200);
    ETH.begin();

    //connecting to a mqtt broker
    client.setServer(mqtt_broker, mqtt_port);
    client.setCallback(callback);
    while (!client.connected()) {
        String client_id = "esp32-client-";
        client_id += String(WiFi.macAddress());
        Serial.printf("The client %s connects to the public MQTT broker\n", client_id.c_str());
        if (client.connect(client_id.c_str(), mqtt_username, mqtt_password)) {
            Serial.println("Public EMQX MQTT broker connected");
        } else {
            Serial.print("failed with state ");
            Serial.print(client.state());
            delay(2000);
        }
    }
    // Publish and subscribe
    client.publish(topic, "Hi, I'm Olimex board ESP32-EVB\n\rThis is GPIO example\n\rType one (or more) of the following commands:\n\r");
    client.publish(topic, "R1H - Relay 1 to High\n\rR1L - Relay 1 to Low\n\rR1T - Relay 1 to Toggle");
    client.publish(topic, "R2H - Relay 2 to High\n\rR2L - Relay 2 to Low\n\rR2T - Relay 2 to Toggle");
    client.publish(topic, "SB  - Scan button level");
    client.subscribe(topic);

    pinMode (RELAY1, OUTPUT);
    pinMode (RELAY2, OUTPUT);
    pinMode (BUTTON, INPUT_PULLUP);
}

void callback(char *topic_, byte *payload, unsigned int length)
{
  char OutputMessage[32], R1H=0, R1L=0, R1T=0, R2H=0, R2L=0, R2T=0, SB=0;
  payload[length]=0;
  R1H = strstr ((char*)payload, "R1H")!=NULL;
  R1L = strstr ((char*)payload, "R1L")!=NULL;
  R1T = strstr ((char*)payload, "R1T")!=NULL;
  R2H = strstr ((char*)payload, "R2H")!=NULL;
  R2L = strstr ((char*)payload, "R2L")!=NULL;
  R2T = strstr ((char*)payload, "R2T")!=NULL;
  SB  = strstr ((char*)payload, "SB")!=NULL;

  if (R1H)
    digitalWrite (RELAY1, HIGH);
  if (R1L)
    digitalWrite (RELAY1, LOW);
  if (R1T)
    digitalWrite (RELAY1, !digitalRead(RELAY1));

  if (R2H)
    digitalWrite (RELAY2, HIGH);
  if (R2L)
    digitalWrite (RELAY2, LOW);
  if (R2T)
    digitalWrite (RELAY2, !digitalRead(RELAY2));

  if (SB)
  {
    sprintf (OutputMessage, "Button is %d\n\r", digitalRead(BUTTON));
    client.publish((const char*)topic, (const char*)OutputMessage, strlen(OutputMessage));
  }
}

void loop() {
    client.loop();
}
