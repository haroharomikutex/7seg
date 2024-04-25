#include "TM1637.h"
#include <M5Stack.h>
#include <WiFi.h>
#include <time.h>
#define CLK 16
#define DIO 17
TM1637 tm1637(CLK,DIO);

// Wi-Fi 関連定義
const char* ssid = "1407"; 
const char* password = "advanceMODERNmonday1963"; //WiFi APのPassword
const char* ntpServer = "ntp.nict.jp";
const long gmtOffset_sec = 9 * 3600;
const int daylightOffset_sec = 0;
WiFiClient client;
// 時間関連
struct tm timeinfo;
uint8_t secLastReport = 0;
const char* week[7] = {"Sun", "Mon", "Tue", "wed", "Thu", "Fri", "Sat"};
// NTPによる時刻取得関数
int ntp(){
uint8_t wifi_retry_cnt;
  M5.Lcd.begin();       
  M5.Lcd.setTextSize(1);
  M5.Lcd.setCursor(0, 0);
  M5.Lcd.printf("Connecting to %s\n", ssid);
  WiFi.begin(ssid, password); //WiFi接続開始
  wifi_retry_cnt = 20; //0.5秒×20=最大10秒で接続タイムアウト
  while (WiFi.status() != WL_CONNECTED){
    delay(500);
    M5.Lcd.printf("*"); //0.5秒毎に”＊”を表示
    if(--wifi_retry_cnt == 0) {
      WiFi.disconnect(true); //タイムアウトでWiFiオフ
      WiFi.mode(WIFI_OFF);
      M5.Lcd.printf("\nCONNECTION FAIL"); //WiFi接続失敗表示
      return(false); //接続失敗でリターン
    }
  }
  M5.Lcd.printf("\nCONNECTED"); //WiFi接続成功表示
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer); //NTPによる時刻取得
  if (!getLocalTime(&timeinfo)) {
    M5.Lcd.printf("/nFailed to obtain time"); //時刻取得失敗表示
    WiFi.disconnect(true); //時刻取得失敗でWiFiオフ
    WiFi.mode(WIFI_OFF);
    M5.Lcd.printf("/nFailed to obtain time"); //時刻取得失敗表示
    return (false); //時刻取得失敗でリターン
  }
  WiFi.disconnect(true); //WiFi切断
  WiFi.mode(WIFI_OFF); //WiFiオフ
  M5.Lcd.fillScreen(TFT_BLACK); //画面消去
  return (true); //時刻取得成功でリターン
}
int8_t disp[4];
uint8_t ListDisp[4];
void setup() {
  M5.begin();
  tm1637.set(7);
  tm1637.init();
  M5.Lcd.fillScreen(TFT_BLACK); //M5Stackの画面初期化
  M5.Lcd.setTextFont(4);
  M5.Lcd.setTextSize(1);
  M5.Lcd.setTextColor(TFT_WHITE, TFT_BLACK);
  M5.Lcd.setTextDatum(TL_DATUM);
  tm1637.displayStr( "NICT-TIME" );
  if(ntp() == false) while(1); //時刻取得に失敗した場合は、動作停止
}
void loop() {
  delay(1000); // Update the display every second
  getLocalTime(&timeinfo);
  if((timeinfo.tm_hour == 2)&&(timeinfo.tm_min == 0)&&(timeinfo.tm_sec == 0)) ntp(); //毎日午前2時に時刻取得。時刻取得に失敗しても動作継続
// ループ関数の中で
  if (secLastReport != timeinfo.tm_sec) { // 秒が変わったときだけ表示を更新
    secLastReport = timeinfo.tm_sec;
    M5.Lcd.setCursor(0, 0);
    M5.Lcd.printf("%02d/%02d/%02d(%s) %02d:%02d:%02d",
        timeinfo.tm_year + 1900, timeinfo.tm_mon + 1, timeinfo.tm_mday, week[timeinfo.tm_wday],
        timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec);
        
   // disp[0] = tm1637.encodeDigit(timeinfo.tm_hour / 10);
   // disp[1] = tm1637.encodeDigit(timeinfo.tm_hour % 10) | 0b10000000; // ドットを2桁目に表示
   // disp[2] = tm1637.encodeDigit(timeinfo.tm_min / 10);
   // disp[3] = tm1637.encodeDigit(timeinfo.tm_min % 10);
    
    tm1637.display(disp);
}

  delay(100); //0.1秒ウェイト
}