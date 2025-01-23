// #include <M5Unified.h>
// #include <WiFi.h>
// #include <WebServer.h>
// #include <SPI.h>
// #include <mcp_can.h>

// // WiFi設定
// const char* ssid = "hossyane"; 
// const char* password = "hossyan3845290716";

// // Webサーバー
// WebServer server(80);

// // スライドバーの値保持
// const int numSliders = 11;
// float sliderValues[numSliders];

// //ボタンの値
// bool hand_btn = true;

// // HTMLページ生成関数
// String createHTMLPage() {
//   return R"rawliteral(
//   <!DOCTYPE html>
// <html lang="en">

// <head>
//   <meta charset="UTF-8">
//   <meta name="viewport" content="width=device-width, initial-scale=1.0">
//   <title>Hossyan Controller</title>
//   <style>
//     body, html {
//       margin: 0;
//       padding: 0;
//       width: 100%;
//       height: 100%;
//       overflow-x: hidden; /* 横スクロールを無効にする */  
//     }

//     body {
//       text-align: center;
//       margin-top: 50px;
//     }

//     h1 {
//       font-size: 24px;
//     }

//     p {
//       font-size: 20px;
//       font-weight: bold;
//     }

//     p.box {
//       border-bottom: 2px solid rgb(85, 82, 82);
//       width: 80%;
//       margin: 0 auto;
//       padding: 10px 10px;
//     }

//     .item_apart {
//       width: 80%;
//       max-width: 1300px;
//       margin: 0 auto;
//       /* 中央配置 */
//       background-color: #807b7b2c;
//       padding: 0px 15px;
//       margin: 15px auto;
//       border-radius: 8px;
//       overflow: hidden;
//     }

//     .btn {
//       font-size: 20px;
//       padding: 10px 10px;
//       margin: 0px auto 20px auto;
//       background-color: #4CAF50;
//       color: white;
//       border: none;
//       cursor: pointer;
//     }

//     .btn:hover {
//       background-color: #45a049;
//     }

//     .slider {
//       width: 70vw;
//     }

//     .slider-container {
//       margin-bottom: 20px;
//     }

//     .slider-label {
//       font-size: 20px;
//       font-weight: bold;
//     }
//   </style>
// </head>

// <body>
//   <h1>Hossyan Controller</h1>
//   <!-- arm_1 -->
//   <div class="item_apart">
//     <div class="slider-container">
//       <p>arm_1_kp: <span id="sliderValue1">0</span></p>
//       <input type="range" min="0.00" max="1.00" step="0.01" value="0" id="slider1" class="slider"onchange="updateSliderValue(1)">
//     </div>
//     <div class="slider-container">
//       <p>arm_1_kd: <span id="sliderValue2">0</span></p>
//       <input type="range" min="0.00" max="1.00" step="0.01" value="0" id="slider2" class="slider"onchange="updateSliderValue(2)">
//     </div>
//     <div class="slider-container">
//       <p>arm_1_ki: <span id="sliderValue3">0</span></p>
//       <input type="range" min="0.00" max="1.00" step="0.01" value="0" id="slider3" class="slider"onchange="updateSliderValue(3)">
//     </div>
//     <p class="box"> </p>
//     <div class="slider-container">
//       <p>arm_1_angle: <span id="sliderValue4">0</span></p>
//       <input type="range" min="0.00" max="1.00" step="0.01" value="0" id="slider4" class="slider"onchange="updateSliderValue(4)">
//     </div>
//   </div>

//   <!-- arm_2 -->
//   <div class="item_apart">
//     <div class="slider-container">
//       <p>arm_2_kp: <span id="sliderValue5">0</span></p>
//       <input type="range" min="0.00" max="1.00" step="0.01" value="0" id="slider5" class="slider"onchange="updateSliderValue(5)">
//     </div>
//     <div class="slider-container">
//       <p>arm_2_kd: <span id="sliderValue6">0</span></p>
//       <input type="range" min="0.00" max="1.00" step="0.01" value="0" id="slider6" class="slider"onchange="updateSliderValue(6)">
//     </div>
//     <div class="slider-container">
//       <p>arm_2_ki: <span id="sliderValue7">0</span></p>
//       <input type="range" min="0.00" max="1.00" step="0.01" value="0" id="slider7" class="slider"onchange="updateSliderValue(7)">
//     </div>
//     <p class="box"> </p>
//     <div class="slider-container">
//       <p>arm_2_angle: <span id="sliderValue8">0</span></p>
//       <input type="range" min="0.00" max="1.00" step="0.01" value="0" id="slider8" class="slider"onchange="updateSliderValue(8)">
//     </div>
//   </div>

//   <!-- hand -->
//   <div class="item_apart">
//     <div class="slider-container">
//       <p>hand_kp: <span id="sliderValue9">0</span></p>
//       <input type="range" min="0.00" max="1.00" step="0.01" value="0" id="slider9" class="slider"onchange="updateSliderValue(9)">
//     </div>
//     <div class="slider-container">
//       <p>hand_kd: <span id="sliderValue10">0</span></p>
//       <input type="range" min="0.00" max="1.00" step="0.01" value="0" id="slider10" class="slider"onchange="updateSliderValue(10)">
//     </div>
//     <div class="slider-container">
//       <p>hand_ki: <span id="sliderValue11">0</span></p>
//       <input type="range" min="0.00" max="1.00" step="0.01" value="0" id="slider11" class="slider"onchange="updateSliderValue(11)">
//     </div>
//     <button class="btn" onclick="sendCommand('open')">open</button>
//     <button class="btn" onclick="sendCommand('close')">close</button>
//   </div>

//   <script>
//     function sendCommand(command) {
//       fetch('/' + command);
//     }

//     function updateSliderValue(sliderNumber) {
//       var slider = document.getElementById("slider" + sliderNumber);
//       var sliderValue = document.getElementById("sliderValue" + sliderNumber);

//       // 小数第2位まで表示
//       sliderValue.innerText = parseFloat(slider.value).toFixed(2);

//       // スライダー番号と値をサーバーに送信
//       fetch('/setSliderValue?slider=' + sliderNumber + '&value=' + slider.value);
//     }
//   </script>
// </body>

// </html>
//   )rawliteral";
// }

// // ルートページハンドラ
// void handleRoot() {
//   server.send(200, "text/html", createHTMLPage());
// }

// // ハンド開
// void handopen() {
//   hand_btn = true; 
//   server.send(200, "text/plain", "Open command received");
// }

// // ハンド閉
// void handClose() {
//   hand_btn = false;
//   server.send(200, "text/plain", "Close command received");
// }

// // スライダー値変更ハンドラ
// void handleSliderValue() {
//   // URLパラメータからスライダー番号と値を取得
//   String sliderNumberStr = server.arg("slider");
//   String value = server.arg("value");

//   // sliderNumberStr を int 型に変換
//   int sliderNumber = sliderNumberStr.toInt();  // String を int に変換

//   if (sliderNumber >= 1 && sliderNumber <= numSliders) {
//     // sliderValues 配列のインデックスに値を代入
//     sliderValues[sliderNumber - 1] = value.toFloat(); 
//   }

//   // HTTP レスポンスを送信
//   server.send(200, "text/plain", "Slider " + String(sliderNumber) + " value set to " + value);
// }

// void Serial_update() {
// //   M5.Lcd.setTextSize(2);
// //   M5.Lcd.fillScreen(BLACK);
// //   M5.Lcd.setCursor(0, 0);
//   Serial.printf(" a1_kp:%.2f",sliderValues[0]);
//   Serial.printf("  a1_kd:%.2f",sliderValues[1]);
//   Serial.printf("  a1_ki:%.2f",sliderValues[2]);
//   Serial.printf("  a1_angle:%.2f",sliderValues[3]);
//   Serial.printf("  a2_kp:%.2f",sliderValues[4]);
//   Serial.printf("  a2_kd:%.2f",sliderValues[5]);
//   Serial.printf("  a2_ki:%.2f",sliderValues[6]);
//   Serial.printf("  a2_angle:%.2f",sliderValues[7]);
//   Serial.printf("  hand_kp:%.2f",sliderValues[8]);
//   Serial.printf("  hand_kp:%.2f",sliderValues[9]);
//   Serial.printf("  hand_kp:%.2f",sliderValues[10]);
//   if(hand_btn == true){
//     Serial.println("  hand:open");
//   }else if(hand_btn == false){
//     Serial.println("  hand:close");
//   }
// }

// void setup() {
//   // M5Stack初期化
//   M5.begin();
//   Serial.begin(115200);
//   M5.Lcd.fillScreen(BLACK);
//   M5.Lcd.setTextColor(WHITE);
//   M5.Lcd.setTextSize(2);
//   M5.Lcd.setCursor(0, 0);
//   M5.Lcd.println("Connecting to WiFi...");

//   // WiFi接続
//   WiFi.begin(ssid, password);
//   while (WiFi.status() != WL_CONNECTED) {
//     delay(500);
//     M5.Lcd.print(".");
//   }
//   M5.Lcd.println();
//   M5.Lcd.println("server start!!");
//   Serial.println("\nWiFi Connected!");
//   Serial.println("IP Address:");
//   Serial.println(WiFi.localIP());

//   // Webサーバーハンドラの設定
//   server.on("/", handleRoot);       // ルートページ
//   server.on("/open", handopen);       // Openボタン
//   server.on("/close", handClose);     // Closeボタン
//   server.on("/setSliderValue", handleSliderValue); // スライダー値の設定

//   // Webサーバー開始
//   server.begin();
// //   M5.Lcd.fillScreen(BLACK);
// }

// void loop() {
//   // Webサーバーのリクエスト処理
//   server.handleClient();

//   // 100ms周期で行うもの
//   auto now = millis();
//   static auto last_display = now;
//   if(now - last_display > 10){
//     Serial_update();
//     last_display = now;
//   }
// }
