#include <M5Unified.h>
#include <WiFi.h>
#include <WebServer.h>

// WiFi設定
const char* ssid = "hossyane"; // WiFiのSSID
const char* password = "hossyan3845290716"; // WiFiのパスワード

// Webサーバー
WebServer server(80);

// HTMLページ生成関数
String createHTMLPage() {
  return R"rawliteral(
    <!DOCTYPE html>
    <html>
    <head>
      <title>Hossyan Controller</title>
    </head>
    <body style="text-align: center; margin-top: 50px;">
      <h1>M5Stack Controller</h1>
      <button style="font-size: 20px; padding: 10px 20px; margin: 10px;" onclick="sendCommand('on')">ON</button>
      <button style="font-size: 20px; padding: 10px 20px; margin: 10px;" onclick="sendCommand('off')">OFF</button>
      <script>
        function sendCommand(command) {
          fetch('/' + command);
        }
      </script>
    </body>
    </html>
  )rawliteral";
}

// ルートページハンドラ
void handleRoot() {
  server.send(200, "text/html", createHTMLPage());
}

// ONボタンハンドラ
void handleOn() {
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setCursor(100, 100);
  M5.Lcd.setTextSize(3);
  M5.Lcd.setTextColor(WHITE, BLACK);
  M5.Lcd.print("ON");

  server.send(200, "text/plain", "ON command received");
}

// OFFボタンハンドラ
void handleOff() {
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setCursor(100, 100);
  M5.Lcd.setTextSize(3);
  M5.Lcd.setTextColor(WHITE, BLACK);
  M5.Lcd.print("OFF");

  server.send(200, "text/plain", "OFF command received");
}

void setup() {
  // M5Stack初期化
  M5.begin();
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setTextColor(WHITE);
  M5.Lcd.setTextSize(2);
  M5.Lcd.setCursor(0, 0);
  M5.Lcd.println("Connecting to WiFi...");

  // WiFi接続
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    M5.Lcd.print(".");
  }
  M5.Lcd.println("\nWiFi Connected!");
  M5.Lcd.println("IP Address:");
  M5.Lcd.println(WiFi.localIP());

  // Webサーバーハンドラの設定
  server.on("/", handleRoot);   // ルートページ
  server.on("/on", handleOn);   // ONボタン
  server.on("/off", handleOff); // OFFボタン

  // Webサーバー開始
  server.begin();
  M5.Lcd.println("Server started!");
}

void loop() {
  // Webサーバーのリクエスト処理
  server.handleClient();
}
