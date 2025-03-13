#include <M5Unified.h>
#include <WiFi.h>
#include <WebServer.h>
#include <SPI.h>
#include <mcp_can.h>

// Web Server ------------------------------------------------------
// WiFi設定
const char* ssid = "GL-SFT1200-19f"; 
const char* password = "goodlife";

// Webサーバー
WebServer server(80);

// スライドバーの値保持
const int numSliders = 11;
float sliderValues[numSliders];

//ボタンの値
bool hand_btn = false;

// HTMLページ生成関数
String createHTMLPage() {
  return R"rawliteral(
  <!DOCTYPE html>
<html lang="en">

<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>Hossyan Controller</title>
  <style>
    body, html {
      margin: 0;
      padding: 0;
      width: 100%;
      height: 100%;
      overflow-x: hidden; /* 横スクロールを無効にする */  
    }

    body {
      text-align: center;
      margin-top: 50px;
    }

    h1 {
      font-size: 24px;
    }

    p {
      font-size: 20px;
      font-weight: bold;
    }

    p.box {
      border-bottom: 2px solid rgb(85, 82, 82);
      width: 80%;
      margin: 0 auto;
      padding: 10px 10px;
    }

    .item_apart {
      width: 80%;
      max-width: 1300px;
      margin: 0 auto;
      /* 中央配置 */
      background-color: #807b7b2c;
      padding: 0px 15px;
      margin: 15px auto;
      border-radius: 8px;
      overflow: hidden;
    }

    .btn {
      font-size: 20px;
      padding: 10px 10px;
      margin: 0px auto 20px auto;
      background-color: #4CAF50;
      color: white;
      border: none;
      cursor: pointer;
    }

    .btn:hover {
      background-color: #45a049;
    }

    .slider {
      width: 70vw;
    }

    .slider-container {
      margin-bottom: 20px;
    }

    .slider-label {
      font-size: 20px;
      font-weight: bold;
    }
  </style>
</head>

<body>
  <h1>Hossyan Controller</h1>
  <!-- arm_1 -->
  <div class="item_apart">
    <div class="slider-container">
      <p>arm_1_kp: <span id="sliderValue1">0</span></p>
      <input type="range" min="0.00" max="1.00" step="0.01" value="0" id="slider1" class="slider"onchange="updateSliderValue(1)">
    </div>
    <div class="slider-container">
      <p>arm_1_kd: <span id="sliderValue2">0</span></p>
      <input type="range" min="0.00" max="3.00" step="0.01" value="0" id="slider2" class="slider"onchange="updateSliderValue(2)">
    </div>
    <div class="slider-container">
      <p>arm_1_ki: <span id="sliderValue3">0</span></p>
      <input type="range" min="0.00" max="0.00" step="0.00" value="0" id="slider3" class="slider"onchange="updateSliderValue(3)">
    </div>
    <p class="box"> </p>
    <div class="slider-container">
      <p>arm_1_angle: <span id="sliderValue4">0</span></p>
      <input type="range" min="0.00" max="55000.00" step="50.00" value="0" id="slider4" class="slider"onchange="updateSliderValue(4)">
    </div>
  </div>

  <!-- arm_2 -->
  <div class="item_apart">
    <div class="slider-container">
      <p>arm_2_kp: <span id="sliderValue5">0</span></p>
      <input type="range" min="0.00" max="4.0" step="0.01" value="0" id="slider5" class="slider"onchange="updateSliderValue(5)">
    </div>
    <div class="slider-container">
      <p>arm_2_kd: <span id="sliderValue6">0</span></p>
      <input type="range" min="0.00" max="3.00" step="0.01" value="0" id="slider6" class="slider"onchange="updateSliderValue(6)">
    </div>
    <div class="slider-container">
      <p>arm_2_ki: <span id="sliderValue7">0</span></p>
      <input type="range" min="0.00" max="0.00" step="0.0" value="0" id="slider7" class="slider"onchange="updateSliderValue(7)">
    </div>
    <p class="box"> </p>
    <div class="slider-container">
      <p>arm_2_angle: <span id="sliderValue8">0</span></p>
      <input type="range" min="0.00" max="60000.00" step="50.00" value="0" id="slider8" class="slider"onchange="updateSliderValue(8)">
    </div>
  </div>

  <!-- hand -->
  <div class="item_apart">
    <button class="btn" onclick="sendCommand('up')">up</button>
    <button class="btn" onclick="sendCommand('down')">down</button>
  </div>

  <script>
    function sendCommand(command) {
      fetch('/' + command);
    }

    function updateSliderValue(sliderNumber) {
      var slider = document.getElementById("slider" + sliderNumber);
      var sliderValue = document.getElementById("sliderValue" + sliderNumber);

      // 小数第2位まで表示
      sliderValue.innerText = parseFloat(slider.value).toFixed(6);

      // スライダー番号と値をサーバーに送信
      fetch('/setSliderValue?slider=' + sliderNumber + '&value=' + slider.value);
    }
  </script>
</body>aa\
\[]

</html>
  )rawliteral";
}

// ルートページハンドラ
void handleRoot() {
  server.send(200, "text/html", createHTMLPage());
}

// ハンド開
void armup() {
  hand_btn = true; 
  server.send(200, "text/plain", "up command received");
}

// ハンド閉
void armdown() {
  hand_btn = false;
  server.send(200, "text/plain", "down command received");
}

// スライダー値変更ハンドラ
void handleSliderValue() {
  // URLパラメータからスライダー番号と値を取得
  String sliderNumberStr = server.arg("slider");
  String value = server.arg("value");

  // sliderNumberStr を int 型に変換
  int sliderNumber = sliderNumberStr.toInt();  // String を int に変換

  if (sliderNumber >= 1 && sliderNumber <= numSliders) {
    // sliderValues 配列のインデックスに値を代入
    sliderValues[sliderNumber - 1] = value.toFloat(); 
  }

  // HTTP レスポンスを送信
  server.send(200, "text/plain", "Slider " + String(sliderNumber) + " value set to " + value);
}

void Serial_update() {
//   M5.Lcd.setTextSize(2);
//   M5.Lcd.fillScreen(BLACK);
//   M5.Lcd.setCursor(0, 0);
  Serial.printf(" a1_kp:%.2f",sliderValues[0]);
  Serial.printf("  a1_kd:%.2f",sliderValues[1]);
  Serial.printf("  a1_ki:%.2f",sliderValues[2]);
  Serial.printf("  a1_angle:%.2f",sliderValues[3]);
  Serial.printf("  a2_kp:%.2f",sliderValues[4]);
  Serial.printf("  a2_kd:%.2f",sliderValues[5]);
  Serial.printf("  a2_ki:%.2f",sliderValues[6]);
  Serial.printf("  a2_angle:%.2f",sliderValues[7]);
  Serial.printf("  hand_kp:%.2f",sliderValues[8]);
  Serial.printf("  hand_kp:%.2f",sliderValues[9]);
  Serial.printf("  hand_kp:%.2f",sliderValues[10]);
  if(hand_btn == true){
    Serial.println("  hand:open");
  }else if(hand_btn == false){
    Serial.println("  hand:close");
  }
}

// Motor control ----------------------------------------------------------------------------------------------
// canのデータ配列の作成
unsigned char data[8] = {0};

//can setup
long unsigned int rxId;
unsigned char len = 0;
unsigned char rxBuf[8];
#define CAN0_INT 15  // Set INT to pin 2
MCP_CAN CAN0(27);    // Set CS to pin 10

// エンコーダパラメータ
int encoder_max = 8191; 
int encoder_min = 0;
float motor_gear_ratio = 19.2;

// エンコーダに関する構造体
struct Encoder
{
    float angle;
    float pre_angle;
    bool flag;
};

Encoder arm_1_encoder = {0.0, 0.0, true}; 
Encoder arm_2_encoder = {0.0, 0.0, true}; 
float motor_angle[2] = {0.0, 0.0};

// PIDパラメータ
unsigned long elapsed_time;
unsigned long pre_time = 0;
unsigned long dt;

// PIDパラメータの構造体
struct PID
{
    float kp;
    float ki;
    float kd;
    float integral;
    float pre_error;
};

PID arm_1_param = {0.48, 0.0, 2.27, 0.0, 0.0};
PID arm_2_param = {0.46, 0.0, 2.56, 0.0, 0.0};
constexpr int motor_number = 2;
int motor_out[motor_number] = {0};
float motor_target[motor_number] = {0.0};
float target_inc[motor_number] = {10.0, 15.0};
float arm_target[motor_number] = {0.0, 0.0};

// 関数ゾーン-----------------
// can初期化
void init_can() {
    if (CAN0.begin(MCP_ANY, CAN_1000KBPS, MCP_8MHZ) == CAN_OK) {
        Serial.println("MCP2515 Initialized Successfully!");
    } else {
        Serial.println("Error Initializing MCP2515...");
    }
    CAN0.setMode(MCP_NORMAL);
}

// DJIのCAN送信
void DJI_CAN_send(){
    int16_t r_currentValue = (int16_t)motor_out[0]; // 制御したい電流値 (-16384 〜 16384 の範囲)
    data[0] = (r_currentValue >> 8) & 0xFF; // 上位バイト
    data[1] = r_currentValue & 0xFF;        // 下位バイト
    int16_t l_currentValue = (int16_t)motor_out[1]; // 制御したい電流値 (-16384 〜 16384 の範囲)
    data[2] = (l_currentValue >> 8) & 0xFF; // 上位バイト
    data[3] = l_currentValue & 0xFF;        // 下位バイト

    byte sndStat = CAN0.sendMsgBuf(0x200, 0, 8, data);
}

float encoder_calc(Encoder &enc, int16_t angle_raw){
    if(enc.flag){
        enc.pre_angle = angle_raw;
        enc.flag = !enc.flag;
    }

    if(angle_raw - enc.pre_angle < -4000){
        enc.angle += ((encoder_max - enc.pre_angle) + angle_raw);
    }else if(angle_raw - enc.pre_angle > 4000){
        enc.angle -= ((enc.pre_angle - encoder_min) + (encoder_max - angle_raw));
    }else{
        enc.angle += (angle_raw - enc.pre_angle);
    }

    enc.pre_angle = angle_raw;

    return enc.angle;
}

void DJI_CAN_read(){
    if (CAN0.checkReceive() == CAN_MSGAVAIL) {
        CAN0.readMsgBuf(&rxId, &len, rxBuf);
        if (rxId == 0x201) {
            int16_t angle_1 = ((rxBuf[0] << 8) | rxBuf[1]); // 上位と下位バイトを結合
            motor_angle[0] = encoder_calc(arm_1_encoder, angle_1);
        }
        if (rxId == 0x202) {
            int16_t angle_2 = ((rxBuf[0] << 8) | rxBuf[1]); // 上位と下位バイトを結合
            motor_angle[1] = encoder_calc(arm_2_encoder, angle_2);
        }
    }
}

// PID計算
int pid_calc(PID &pid, float target, float current){
    float error = target - current;
    pid.integral += error * dt;
    float deriv = (error - pid.pre_error) / dt;
    pid.pre_error = error;
    float output = (error * pid.kp) + (pid.integral * pid.ki) + (deriv * pid.kd);

    //整形
    if(output >= 12000){
        output = 12000;
    }else if(output <= -12000){
        output = -12000;
    }
    return static_cast<int>(output);
}

// motor0のフィードフォワード
void arm0_anti_gravity(){
  int support_current_max = 5800;
  int encoder_90_deg = 34900;
  int anti_gravity = (encoder_90_deg - motor_angle[0]) / encoder_90_deg * support_current_max;

  motor_out[0] = motor_out[0] + anti_gravity;
}

void input_shaping_filter() {
    for(int i=0; i < motor_number; i++){
        //targetの差がincより小さかった時用
        if(abs(arm_target[i] - motor_target[i]) <= target_inc[i]) {
            motor_target[i] = arm_target[i];
        }

        //出力の増減を行う
     
        if(motor_target[i] < arm_target[i]) {
            motor_target[i] += target_inc[i];
        }else if(motor_target[i] > arm_target[i]) {
            motor_target[i] -= target_inc[i];
        }
    }
}

void lcd_update() {
  int x = 50;   // 表示位置（X座標）
  int y = 200;  // 表示位置（Y座標）
  int w = 100;  // 消去する幅
  int h = 40;   // 消去する高さ

  M5.Lcd.fillRect(x, y, w, h, BLACK);
  M5.Lcd.setCursor(x, y);
  if(hand_btn == true){
    M5.Lcd.println("up");
  }else if(hand_btn == false){
    M5.Lcd.println("down");
  }
}

void motor_param_update() {
    // arm_1_param.kp = sliderValues[0];
    // arm_1_param.kd = sliderValues[1];
    // arm_1_param.ki = sliderValues[2];
    // arm_target[0] = sliderValues[3];
    // arm_2_param.kp = sliderValues[4];
    // arm_2_param.kd = sliderValues[5];
    // arm_2_param.ki = sliderValues[6];
    // arm_target[1] = sliderValues[7];
    if(hand_btn == true){
      arm_target[0] = 40000;
      arm_target[1] = 60000;
    }else if(hand_btn == false){
      arm_target[0] = 0;
      arm_target[1] = 0;
    }
}

void setup() {
  // M5Stack初期化
  M5.begin();
  M5.Power.begin();
  Serial.begin(115200);
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
  M5.Lcd.println();
  M5.Lcd.println("server start!!");
  M5.Lcd.println("\nWiFi Connected!");
  M5.Lcd.println("IP Address:");
  M5.Lcd.println(WiFi.localIP());

  // Webサーバーハンドラの設定
  server.on("/", handleRoot);       // ルートページ
  server.on("/up", armup);       // Openボタン
  server.on("/down", armdown);     // Closeボタン
  server.on("/setSliderValue", handleSliderValue); // スライダー値の設定

  // Webサーバー開始
  server.begin();

  //can初期化
  init_can();
}

void loop() {
    // Webサーバーのリクエスト処理
    server.handleClient();
    motor_param_update();

    //センサー値取得
    DJI_CAN_read();

    //入力整形
    auto now = millis();
    static auto pre_100ms = now;
    if(now - pre_100ms) {
        input_shaping_filter();
        pre_100ms = now;
    }

    //10ms周期で行うもの:シリアルモニタ,cansend
    static auto pre_10ms = now;
    if(now - pre_10ms > 10) {
        //シリアルモニタ
        Serial.printf("angle1:%f",motor_angle[0]);
        Serial.printf("\tangle2:%f",motor_angle[1]);
        Serial.printf("\toutput1:%d",motor_out[0]);
        Serial.printf("\toutput2:%d",motor_out[1]);
        Serial.printf("\ta2_kp:%f",arm_2_param.kp);
        Serial.printf("\ta2_kd:%f",arm_2_param.kd);
        Serial.printf("\ta2_target:%f",motor_target[1]);
        Serial.println();

        //モータ制御
        elapsed_time = millis();
        dt = elapsed_time - pre_time;
        pre_time = elapsed_time;
        motor_out[0] = pid_calc(arm_1_param, motor_target[0], motor_angle[0]);
        arm0_anti_gravity();
        motor_out[1] = pid_calc(arm_2_param, motor_target[1], motor_angle[1]);
        DJI_CAN_send();

        //時間
        pre_10ms = now;
  }
}
