# esp32-beebottle
## 概要
ESP32 で MQTT over TLS (Port: 8883) を用いて Beebottle に繋ぐテスト

## このリポジトリ内のファイルの説明
* esp32-beebottle.ino
  * Arduino IDE スケッチ
    * Requirement
      * [knolleary/pubsubclient](https://github.com/knolleary/pubsubclient)
      * [bblanchon/ArduinoJson](https://github.com/bblanchon/ArduinoJson)
* config_sample.h
  * config.h のテンプレート
* convcert.sh
  * `bash convcert.sh mqtt.beebotte.com.pem`
    * certificate.h が作成されます
* convcert.js
  * `node convcert mqtt.beebotte.com.pem`
    * certificate.h が作成されます
  * convcert.html からも利用されています
* convcert.html
  * ブラウザでpemファイルを変換する(Bash, Node.jsがない環境の方向け)
  * スタイルシートには[Milligram](https://milligram.io/)を使っております

## 作成する必要があるファイル
* config.h
  * config_sample.h をベースにWiFiアクセスポイントやBeebotteの設定を記入
  * `password` にはハッシュ化した値を入力することを推奨 ([ESP32のArduinoスケッチにWi-Fiのパスワードを平文で書くのはやめよう！](https://qiita.com/mascii/items/bbee7b7bff9cf5a2f833))
* certificate.h
  * CA証明書
  * mqtt.beebotte.com.pem を変換して作成
    * `https://beebotte.com/certs/mqtt.beebotte.com.pem` からダウンロード

## 動作テスト
```
curl -d '{"data":"led_on"}' -H "Content-Type: application/json" -X POST \
 'https://api.beebotte.com/v1/data/publish/(channel)/(resource)?token=token_xxxxxxxxxxxxxxxx'
```

## トラブルシューティング
証明書の確認がうまくいっていない可能性がある場合は `#include "certificate.h"` と `wifiClient.setCACert(test_ca_cert);` をコメントアウトして動作確認する

# 参考資料
[IFTTTのトリガーおよびアクションをESP8266で実行する](https://qiita.com/mayfair/items/e761c788a9d8787bc610)
