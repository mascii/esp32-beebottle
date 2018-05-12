#include <WiFiClientSecure.h>

// PubSubClientライブラリでのパケットサイズは128バイトなのを拡張
#define MQTT_MAX_PACKET_SIZE 1024
#include <PubSubClient.h>

#include <ArduinoJson.h>

#include "config.h"
#include "certificate.h"
#define LED_PIN 2

const char* host = "mqtt.beebotte.com";

// メッセージを受け取ったらシリアルにプリント
void callback(char* topic, byte* payload, unsigned int length) {
  // PubSubClient.hで定義されているMQTTの最大パケットサイズ
  char buffer[MQTT_MAX_PACKET_SIZE];

  snprintf(buffer, sizeof(buffer), "%s", payload);
  Serial.println("received:");
  Serial.print("topic: ");
  Serial.println(topic);
  Serial.println(buffer);

  // 受け取ったJSON形式のペイロードをデコードする
  StaticJsonBuffer<MQTT_MAX_PACKET_SIZE> jsonBuffer;
  JsonObject& root = jsonBuffer.parseObject(buffer);

  if (!root.success()) {
    Serial.println("parseObject() failed");
    return;
  }

  const char* parsedPayload = root["data"];

  if (parsedPayload != NULL) {
    Serial.print("payload: ");
    Serial.println(parsedPayload);
    if (strcmp(parsedPayload, "led_on") == 0) {
      digitalWrite(LED_PIN, HIGH);
    } else if (strcmp(parsedPayload, "led_off") == 0) {
      digitalWrite(LED_PIN, LOW);
    }
  }
}

WiFiClientSecure wifiClient;
PubSubClient client(host, 8883, wifiClient);

void setup() {
  Serial.begin(115200);
  Serial.println();
  pinMode(LED_PIN, OUTPUT);
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.print("connecting to ");
    Serial.print(ssid);
    Serial.println("...");
    WiFi.begin(ssid, password);

    if (WiFi.waitForConnectResult() != WL_CONNECTED) {
      // Wi-Fiアクスポイントへの接続に失敗したら5秒間待ってリトライ
      Serial.println("failed to connect");
      delay(5000);
      return;
    } else {
      Serial.print("WiFi connected: ");
      Serial.println(WiFi.localIP());
    }

    wifiClient.setCACert(test_ca_cert);
  }

  // クライアントがサーバに接続されていなければ
  if (!client.connected()) {
    // ユーザ名を指定して接続
    String username = "token:";
    username += channelToken;
    client.connect(clientID, username.c_str(), NULL);

    if (client.connected()) {
      Serial.println("MQTT connected");
      client.setCallback(callback);

      // トピック名を指定してsubscribe
      client.subscribe(topic);
    } else {
      Serial.print("MQTT connection failed: ");
      Serial.println(client.state());
      delay(5000);
    }
  } else {
    // 既にサーバに接続されていれば通常処理を行う
    client.loop();
  }
}
