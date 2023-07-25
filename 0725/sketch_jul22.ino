#include <WiFi.h>
#include <HTTPClient.h>

const char* ssid = "XXXXXXXXXXXXX"; // WiFiのSSID（ネットワーク名）を入力
const char* password = "XXXXXXXXXX"; // WiFiのパスワードを入力

const int redLED = 13; // red ピン13番
const int blueLED = 12; // blue ピン12番
const int greenLED = 14; // green ピン14番

void setup() {
  pinMode(redLED, OUTPUT);
  pinMode(blueLED, OUTPUT);
  pinMode(greenLED, OUTPUT);

  Serial.begin(115200); //シリアル通信をボーレート115200で開始
  WiFi.begin(ssid, password); //WiFiに接続

  while (WiFi.status() != WL_CONNECTED) { //WiFi接続が確立するまで待機
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  Serial.println("WiFi connected");

}

void loop() {
  int countOnes = 0;
  int countZeros = 0;

  for (int i = 0; i < 10; i++) {
    int randomValue = random(2); // 0または1のランダムな値を生成
    if (randomValue == 0) {
      countZeros++;
    } else {
      countOnes++;
    }
    delay(1000);
  }

  if (countOnes == 0 || countOnes == 10) {
    // 全て0か全て1の場合、LEDを赤にする　偏っている
    digitalWrite(redLED, HIGH);
    digitalWrite(greenLED, LOW);
    digitalWrite(blueLED, LOW);
  } else if (countOnes == 5) {
    // ちょうど5つの1がある場合、LEDを青にする
    digitalWrite(redLED, LOW);
    digitalWrite(greenLED, LOW);
    digitalWrite(blueLED, HIGH);

    static int blueCounter = 0;
    blueCounter++;
    if (blueCounter >= 3) {
      // 青いLEDが3回点灯するたびに、Eメールを送信する（テスト用）
      sendEmail();
      blueCounter = 0;
    }
  } else {
    // それ以外の場合、LEDを緑にする
    digitalWrite(redLED, LOW);
    digitalWrite(greenLED, HIGH);
    digitalWrite(blueLED, LOW);
  }
}


void sendEmail() {
  String event_name = "BadLuck_notification"; // IFTTTのイベント名[BadLuck_notification]を指定
  String url = "https://maker.ifttt.com/trigger/" + event_name + "/with/key/xxxxxxxxxxx"; // IFTTT Webhooks URLを構築 最後の数値は与えられたAPI
  HTTPClient http;

  Serial.print("[HTTP] begin...\n");
  if (http.begin(url)) { // HTTP通信を開始
    Serial.print("[HTTP] GET...\n");
    // HTTP GETリクエストを送信
    int httpCode = http.GET();

    // httpCodeはエラーの場合には負の値になる
    if (httpCode > 0) {
      // HTTPヘッダーが送信され、サーバーからのレスポンスヘッダー処理
      Serial.printf("[HTTP] GET... code: %d\n", httpCode);
    } else {
      Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
    }

    // 接続を閉じる
    http.end();
  } else {
    Serial.printf("[HTTP} Unable to connect\n");
  }
}
