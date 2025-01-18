// #include <WiFi.h>
// #include <M5Unified.h>

// // Wi-Fi接続情報
// const char* ssid = "hossyane";       // Wi-FiのSSID
// const char* password = "hossyan3845290716"; // Wi-Fiのパスワード

// WiFiServer server(80); // Webサーバーをポート80で開始

// void setup() {
//     M5.begin(); // M5Stackの初期化
//     Serial.begin(115200);

//     // Wi-Fiに接続
//     Serial.print("Connecting to Wi-Fi...");
//     WiFi.begin(ssid, password);
//     while (WiFi.status() != WL_CONNECTED) {
//         delay(500);
//         Serial.print(".");
//     }
//     Serial.println("\nWiFi connected.");
//     Serial.print("IP Address: ");
//     Serial.println(WiFi.localIP());

//     server.begin(); // Webサーバーを開始
// }

// void loop() {
//     WiFiClient client = server.available(); // クライアントの接続を確認
//     if (client) {
//         Serial.println("New Client Connected.");
//         String request = "";
//         while (client.connected() && client.available()) {
//             char c = client.read();
//             request += c;
//         }

//         // シンプルなHTMLレスポンスを返す
//         client.println("HTTP/1.1 200 OK");
//         client.println("Content-type:text/html");
//         client.println("Connection: close");
//         client.println();
//         client.println("<!DOCTYPE html><html>");
//         client.println("<head><title>ESP32 Web Server</title></head>");
//         client.println("<body><h1>ESP32 Web Server</h1>");
//         client.println("<p>Connection Successful!</p>");
//         client.println("</body></html>");
//         client.stop(); // クライアント接続を終了
//         Serial.println("Client disconnected.");
//     }
// }
