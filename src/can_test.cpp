// #include <Arduino.h>
// #include <math.h>
// #include <mcp_can.h>  
// #include <M5Unified.h>

// // canのデータ配列の作成
// unsigned char data[8] = {0};

// //can setup
// long unsigned int rxId;
// unsigned char len = 0;
// unsigned char rxBuf[8];
// #define CAN0_INT 15  // Set INT to pin 2
// MCP_CAN CAN0(27);    // Set CS to pin 10

// // エンコーダパラメータ
// int encoder_max = 8191;
// int encoder_min = 0;
// float motor_gear_ratio = 19.2;

// // エンコーダに関する構造体
// struct Encoder
// {
//     float angle;
//     float pre_angle;
//     bool flag;
// };

// Encoder arm_1_encoder = {0.0, 0.0, true}; 
// Encoder arm_2_encoder = {0.0, 0.0, true}; 
// float motor_angle[2] = {0.0, 0.0};

// // PIDパラメータ
// unsigned long elapsed_time;
// unsigned long pre_time = 0;
// unsigned long dt;

// // PIDパラメータの構造体
// struct PID
// {
//     float kp;
//     float ki;
//     float kd;
//     float integral;
//     float pre_error;
// };

// PID arm_1_param = {0.0007, 0.0, 0.0, 0.0, 0.0};
// PID arm_2_param = {0.00007, 0.0, 0.007, 0.0, 0.0};
// constexpr int motor_number = 2;
// int motor_out[motor_number] = {0};
// float motor_target[motor_number] = {0.0};
// float target_inc[motor_number] = {200.0, 200.0};
// float arm_target[motor_number] = {1850.0, 3000.0};

// // 関数ゾーン-------------------------------------------------------------------------------------
// // can初期化
// void init_can() {
//     if (CAN0.begin(MCP_ANY, CAN_1000KBPS, MCP_8MHZ) == CAN_OK) {
//         Serial.println("MCP2515 Initialized Successfully!");
//     } else {
//         Serial.println("Error Initializing MCP2515...");
//     }
//     CAN0.setMode(MCP_NORMAL);
// }

// // DJIのCAN送信
// void DJI_CAN_send(){
//     int16_t r_currentValue = (int16_t)motor_out[0]; // 制御したい電流値 (-16384 〜 16384 の範囲)
//     data[0] = (r_currentValue >> 8) & 0xFF; // 上位バイト
//     data[1] = r_currentValue & 0xFF;        // 下位バイト
//     int16_t l_currentValue = (int16_t)motor_out[1]; // 制御したい電流値 (-16384 〜 16384 の範囲)
//     data[2] = (l_currentValue >> 8) & 0xFF; // 上位バイト
//     data[3] = l_currentValue & 0xFF;        // 下位バイト

//     byte sndStat = CAN0.sendMsgBuf(0x200, 0, 8, data);
// }

// float encoder_calc(Encoder &enc, int16_t angle_raw){
//     if(enc.flag){
//         enc.pre_angle = angle_raw;
//         enc.flag = !enc.flag;
//     }

//     if(angle_raw - enc.pre_angle < -4000){
//         enc.angle += ((encoder_max - enc.pre_angle) + angle_raw) / motor_gear_ratio;
//     }else if(angle_raw - enc.pre_angle > 4000){
//         enc.angle -= ((enc.pre_angle - encoder_min) + (encoder_max - angle_raw)) / motor_gear_ratio;
//     }else{
//         enc.angle += (angle_raw - enc.pre_angle) / motor_gear_ratio;
//     }

//     enc.pre_angle = angle_raw;

//     return enc.angle;
// }

// void DJI_CAN_read(){
//     if (CAN0.checkReceive() == CAN_MSGAVAIL) {
//         CAN0.readMsgBuf(&rxId, &len, rxBuf);
//         if (rxId == 0x201) {
//             int16_t angle_1 = ((rxBuf[0] << 8) | rxBuf[1]); // 上位と下位バイトを結合
//             motor_angle[0] = encoder_calc(arm_1_encoder, angle_1);
//         }
//         if (rxId == 0x202) {
//             int16_t angle_2 = ((rxBuf[0] << 8) | rxBuf[1]); // 上位と下位バイトを結合
//             motor_angle[1] = encoder_calc(arm_2_encoder, angle_2);
//         }
//     }
// }

// // PID計算
// int pid_calc(PID &pid, float target, float current){
//     float error = target - current;
//     pid.integral += error * dt;
//     float deriv = (error - pid.pre_error) / dt;
//     pid.pre_error = error;
//     float output = (error * pid.kp) + (pid.integral * pid.ki) + (deriv * pid.kd);

//     //整形
//     if(output >= 1.0){
//         output = 1.0;
//     }else if(output <= -1.0){
//         output = -1.0;
//     }
//     output = 10000 * output; //数値は最大電流制限

//     return static_cast<int>(output);
// }

// void input_shaping_filter() {
//     for(int i=0; i < motor_number; i++){
//         //targetの差がincより小さかった時用
//         if(abs(arm_target[i] - motor_target[i]) <= target_inc[i]) {
//             motor_target[i] = arm_target[i];
//         }

//         //出力の増減を行う
//         if(motor_target[i] < arm_target[i]) {
//             motor_target[i] += target_inc[i];
//         }else if(motor_target[i] > arm_target[i]) {
//             motor_target[i] -= target_inc[i];
//         }
//     }
// }

// void setup(){
//     M5.begin();
//     M5.Power.begin();
//     Serial.begin(115200);

//     Serial.println("M5begin!!");

//     M5.Lcd.clear(BLACK);
//     M5.Lcd.setTextSize(2); 
//     M5.Lcd.print("M5start!");

//     init_can();
// }

// void loop(){
//     M5.update();
//     DJI_CAN_read();

//     //入力整形
//     auto now = millis();
//     static auto pre_100ms = now;
//     if(now - pre_100ms) {
//         input_shaping_filter();
//         pre_100ms = now;
//     }

//     //10ms周期で行うもの:シリアルモニタ,cansend
//     static auto pre_10ms = now;
//     if(now - pre_10ms > 10) {
//         //シリアルモニタ
//         Serial.printf("angle1:%f",motor_angle[0]);
//         Serial.printf("\tangle2:%f",motor_angle[1]);
//         Serial.printf("\toutput1:%d",motor_out[0]);
//         Serial.printf("\toutput2:%d",motor_out[1]);
//         Serial.println();

//         //モータ制御
//         elapsed_time = millis();
//         dt = elapsed_time - pre_time;
//         pre_time = elapsed_time;
//         motor_out[0] = pid_calc(arm_1_param, motor_target[0], motor_angle[0]);
//         motor_out[1] = pid_calc(arm_2_param, motor_target[1], motor_angle[1]);
//         DJI_CAN_send();

//         //時間
//         pre_10ms = now;
//   }
// }