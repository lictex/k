/*
 * by kpx
 * 2016.04
 */

#include "Keyboard.h"

int keyAmount=16;  //按键个数

int keyPin[4][4]={{2,3,4,5},{6,7,8,9},{10,11,12,13},{A0,A1,A2,A3}};  //按键端口

int keyASCII[4][4]={{122,120,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0}};  //各按键ASCII值

boolean isKeyPressed[4][4]={{false,false,false,false},{false,false,false,false},{false,false,false,false},{false,false,false,false}};  //按键是否被按下

long lastPress[4][4]={{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0}};  //最后按下的时间

long lastRelease[4][4]={{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0}};  //最后抬起的时间


void setup() {
  
  /*
   * 初始化端口
   * 上拉输入
   */
  for(int x=0;x<4;x++){
    for(int y=0;y<4;y++){
      pinMode(keyPin[y][x],INPUT_PULLUP);
    }
  }
  
  Serial.begin(9600);  //打开串口
  Keyboard.begin();  //启动键盘
  delay(2000);
  pinMode(17,INPUT_PULLUP);
  Serial.println(digitalRead(keyPin[3][3]));
  delay(5000);  //延时5秒启动
}

void loop() {
  /*
   * 遍历所有按键
   */
  for(int x=0;x<4;x++){
    for(int y=0;y<4;y++){
      int keyStatus=digitalRead(keyPin[y][x]);  //读取按键状态
      if(keyStatus==LOW){  //如果按键被按下
        if(isKeyPressed[y][x]==false){
          keyPressed(x,y);
        }else{
          lastPress[y][x]=millis();  //更新最后按下时间
        }
      }else{
        if(isKeyPressed[y][x]==true){
          checkReleased(x,y);  //除抖
        }
      }
    }
  }
}

/*
 * 按键按下时被调用
 */
void keyPressed(int kX,int kY){
  isKeyPressed[kY][kX]=true;
  lastPress[kY][kX]=millis();
  Serial.print("press ");
  Serial.println(lastPress[kY][kX]);
  Keyboard.press(keyASCII[kY][kX]);
}

/*
 * 除抖
 */
void checkReleased(int kX,int kY){
  lastRelease[kY][kX]=millis();
  if(millis()-lastPress[kY][kX]>10){
    keyReleased(kX,kY);
  }else{
        //isKeyPressed[kY][kX]=false;
    //lastPress[kY][kX]=millis();
  }
}

/*
 * 按键松开时被调用
 */
void keyReleased(int kX,int kY){
  isKeyPressed[kY][kX]=false;
  Serial.println("release");
  Keyboard.release(keyASCII[kY][kX]);
}






