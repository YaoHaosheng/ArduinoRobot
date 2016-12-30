#include <Servo.h>
#include <Model.h>
#include <Car.h>
#include <Track.h>
#include <ColSensor.h>
#include <Functions.h>

void setup() {
	//电机速度
	SPEED1 = 150; 
	SPEED2 = SPEED1 + 12;
	//舵机角度/脉冲值。分别对应1-3号舵机（-1的位置不用改动）
	int temp[3][3] = { { 90, 120, 60 }, { 90, 110, 40 }, { 160,0, -1 } };
	//红色垃圾桶和绿色垃圾桶的角度/脉冲值
	bucket[1] = 180; bucket[3] = 140;
	TURN1 = SPEED1 + 30;
	TURN2 = TURN1 + SPEED2 - SPEED1;
	side[0] = (temp[2][0] - temp[2][1]) * 2 / 3 + temp[2][1];
	side[1] = (temp[2][0] - temp[2][1]) * 4 / 3 + temp[2][1];
	bucket[2] = temp[2][0];
	for (int i = 0; i < 3; ++i){
		for (int j = 0; j < 3; ++j){
			angle[i][j] = temp[i][j];
		}
	}
	Init();
}

void loop() {
	Run();
}
