/*
   by kpx
   for Arduino Micro
   2016.04
   Last Update: 4.17
*/

#include "Keyboard.h"
int comboDelay = 10;

int keyAmount = 16; //按键个数

int keyPin[4][4] = {{2, 3, 4, 5}, {6, 7, 8, 9}, {10, 12, A0, A1}, {A2, A3, A4, A5}}; //按键端口

int keyASCII[4][4][5] = {
  {   //第一横排
    {KEY_ESC, 0, 0, 0, 0}, {96, 0, 0, 0, 0}, {99, 0, 0, 0, 0}, {100, 0, 0, 0, 0}
  }, {//第二横排
    {101, 0, 0, 0, 0}, {102, 0, 0, 0, 0}, {103, 0, 0, 0, 0}, {104, 0, 0, 0, 0}
  }, {//第三横排
    {122, 0, 0, 0, 0}, {120, 0, 0, 0, 0}, {99, 0, 0, 0, 0}, {118, 0, 0, 0, 0}
  }, {//第四横排
    {KEY_LEFT_CTRL, 99, 0, 0, 0}, {KEY_LEFT_CTRL, 118, 0, 0, 0}, {111, 0, 0, 0, 0}, {112, 0, 0, 0, 0}
  }
}; //各按键ASCII值,单按键最大可设置5键组合键

boolean isKeyPressed[4][4] = {{false, false, false, false}, {false, false, false, false}, {false, false, false, false}, {false, false, false, false}}; //按键是否被按下

long lastPress[4][4] = {{0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}}; //最后按下的时间

long lastRelease[4][4] = {{0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}}; //最后抬起的时间


void setup() {

  /*
     初始化端口
     上拉输入
  */
  for (int x = 0; x < 4; x++) {
    for (int y = 0; y < 4; y++) {
      pinMode(keyPin[y][x], INPUT_PULLUP);
    }
  }

  Serial.begin(9600);  //打开串口
  Keyboard.begin();  //启动键盘
  delay(2000);
  pinMode(17, INPUT_PULLUP);
  Serial.println(digitalRead(keyPin[3][3]));
  delay(5000);  //延时5秒启动
}

void loop() {
  /*
     遍历所有按键
  */
  for (int x = 0; x < 4; x++) {
    for (int y = 0; y < 4; y++) {
      int keyStatus = digitalRead(keyPin[y][x]); //读取按键状态
      if (keyStatus == LOW) { //如果按键被按下
        if (isKeyPressed[y][x] == false) {
          keyPressed(x, y);
        } else {
          lastPress[y][x] = millis(); //更新最后按下时间
        }
      } else {
        if (isKeyPressed[y][x] == true) {
          checkReleased(x, y); //除抖
        }
      }
    }
  }
}

/*
   按键按下时被调用
*/
void keyPressed(int kX, int kY) {
  isKeyPressed[kY][kX] = true;
  lastPress[kY][kX] = millis();
  Serial.print("press ");
  Serial.println(lastPress[kY][kX]);
  for (int i = 0; i < 5; ) {

    Keyboard.press(keyASCII[kY][kX][i]);
    i++;
    if (keyASCII[kY][kX][i] == 0) {
      break;
    }
    delay(comboDelay);
  }
}

/*
   除抖
*/
void checkReleased(int kX, int kY) {
  lastRelease[kY][kX] = millis();
  if (millis() - lastPress[kY][kX] > 10) {
    keyReleased(kX, kY);
  } else {
    //isKeyPressed[kY][kX]=false;
    //lastPress[kY][kX]=millis();
  }
}

/*
   按键松开时被调用
*/
void keyReleased(int kX, int kY) {
  isKeyPressed[kY][kX] = false;
  Serial.println("release");
  for (int i = 0; i < 5; i++) {
    Keyboard.release(keyASCII[kY][kX][i]);
  }
}






