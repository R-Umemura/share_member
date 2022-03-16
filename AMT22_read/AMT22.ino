// GR-PEACHボードを使ったプログラムです
// このプログラムは，AMT22を読み取るまたは原点をリセットするものです．
// uenosuke(上野先生)が作成したmbedライブラリを元に作成したライブラリの使用を前提としています．
// 作成日 2021/12/27
// 最終更新日 2022/01/14
// 作成者 Rumemura(梅村留奈)

//メモ
// AMT22値を初期化＆読み取れるプログラムにしたい
//ライブラリの移行がうまくいってなさそう(-_-;)

#include <Arduino.h>
#include <MsTimer2.h>
#include <SPI.h>
#include <math.h>
#include "define.h"
#include "AMT22VBPeach.h"

AMT22V amt22_1(&SPI, PIN_CSB1);
AMT22V amt22_2(&SPI, PIN_CSB2);

bool flag_10ms = false; // loop関数で10msごとにシリアルプリントできるようにするフラグ
bool flag_100ms = false;
int count = 0;

// LEDをチカチカさせるための関数
void LEDblink(byte pin, int times, int interval)
{
  analogWrite(pin, 0);
  for (int i = 0; i < times; i++)
  {
    delay(interval);
    analogWrite(pin, 255);
    delay(interval);
    analogWrite(pin, 0);
  }
}

// setupで有効にされるタイマ割り込み処理が書いてある場所
void timer_warikomi()
{
  Serial.print("warikomi");
  // RGB LED を良い感じに光らせるための処理
  static int count = 0;
  static int count_flag = 0;
  count += 2; // ここで光る周期を変えられる(はず)
  count_flag++;

  if (count < 255)
  {
    analogWrite(PIN_LED_RED, count);
    analogWrite(PIN_LED_BLUE, 255 - count);
  }
  else if (count < 255 * 2)
  {
    analogWrite(PIN_LED_GREEN, count - 255);
    analogWrite(PIN_LED_RED, 255 * 2 - count);
  }
  else if (count < 255 * 3)
  {
    analogWrite(PIN_LED_BLUE, count - 255 * 2);
    analogWrite(PIN_LED_GREEN, 255 * 3 - count);
  }
  else
  {
    count = 0;
  }

  // フラグ立てるための処理
  flag_10ms = true;
}

void setup()
{
  Serial.begin(115200);

  pinMode(DSW_1, INPUT);
  pinMode(DSW_2, INPUT);

  // AMT22Vの初期化
  SPI.begin();                         // ここでSPIをbeginしてあげないとちゃんと動かなかった
  SPI.setClockDivider(SPI_CLOCK_DIV8); // SPI通信のクロックを1MHzに設定 beginの後に置かないと，処理が止まる
  Serial.println(amt22_1.init());
  Serial.println(amt22_2.init());
  //ここのinitで1が出力されなかった場合はうまく読み取れてない．

  LEDblink(PIN_LED_GREEN, 2, 100); // 初期が終わった証拠にブリンク
  Serial.println("AMT22V init done!");
  Serial.flush();

  delay(100);
  //amt22_1.setZeroPos();
  delay(100);
  amt22_2.setZeroPos();
  delay(100);
  MsTimer2::set(10, timer_warikomi); // 10ms period
  MsTimer2::start();
}

void loop()
{
  // 10msに1回ピン情報を出力する
  if (flag_10ms)
  {

    int Enc_read_1;
    int Enc_read_2; /*,preEnc_read*/

    int sw_read_1 = digitalRead(DSW_1);
    int sw_read_2 = digitalRead(DSW_2);

    Enc_read_1 = amt22_1.getEncount();
    Enc_read_2 = amt22_2.getEncount();

    // AMT22の原点をリセットする>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
    /*
    if (count == 1)
    {
      amt22_1.setZeroPos();
      amt22_2.setZeroPos();
    }
    else if (sw_read_2 == 0)
    {
      amt22_2.setZeroPos();
    }
    */
    //<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

    //読み取った値をtera termで表示する

    Serial.print("SW1_");
    Serial.print(sw_read_1);
    Serial.print("\t");
    Serial.print("SW2_");
    Serial.print(sw_read_2);
    Serial.print("\t");
    Serial.print("ENC1_");
    Serial.print(Enc_read_1);
    Serial.print("\t");
    Serial.print("ENC2_");
    Serial.println(Enc_read_2);

    flag_10ms = false;
  }
}
/*
 */