#ifndef Car_h
#define Car_h

#include <Arduino.h> 
class Car{
private:
	//定义引脚，L_IN1和R_IN3分别为小车前进时IN1和IN3的电平
	int IN1, IN2, IN3, IN4, MotorA, MotorB, L_IN1, R_IN3;

	//控制数字引脚电平，进而控制左右轮旋转方向
	void Digital_out(int a, int b, int c, int d){
		digitalWrite(IN1, a);
		digitalWrite(IN2, b);
		digitalWrite(IN3, c);
		digitalWrite(IN4, d);
	}

	//控制PWM引脚，进而控制左右轮速度
	void Analog_out(int a, int b){
		analogWrite(MotorA, a);
		analogWrite(MotorB, b);
	}

public:
	//构造函数，默认L_IN1和R_IN3为1
	Car(int a, int b, int c, int d, int e, int f){
		IN1 = a; IN2 = b; IN3 = c; IN4 = d; MotorA = e; MotorB = f;
		L_IN1 = 1, R_IN3 = 1;
	}

	//将引脚配置为输出
	void Mode(){
		pinMode(IN1, OUTPUT);
		pinMode(IN2, OUTPUT);
		pinMode(IN3, OUTPUT);
		pinMode(IN4, OUTPUT);
		pinMode(MotorA, OUTPUT);
		pinMode(MotorB, OUTPUT);
	}

	//可设置小车前进时IN1和IN3的电平，以此对Move函数进行修正
	void Infer(int a, int b){
		L_IN1 = a; R_IN3 = b;
	}

	/*第一，二个形参分别表示MotorA和MotorB的PWM，第三个形参为小
	车运动的方式，取值为1至9，分别表示单边左后转，后退，单边右后
	转，双边左转，停止，双边右转，单边左前转，前进，单边右前转，
	第四个形参表示延迟时间，默认为1ms，可以用此函数配合Infer
	函数对小车前进时IN1和IN3的电平进行确认，从而修正此函数*/
	void Move(int a, int b, int c){
		switch (c){
		case 1:Digital_out(0, 0, !R_IN3, R_IN3); break;
		case 2:Digital_out(!L_IN1, L_IN1, !R_IN3, R_IN3); break;
		case 3:Digital_out(!L_IN1, L_IN1, 0, 0); break;
		case 4:Digital_out(!L_IN1, L_IN1, R_IN3, !R_IN3); break;
		case 5:Digital_out(0, 0, 0, 0); break;
		case 6:Digital_out(L_IN1, !L_IN1, !R_IN3, R_IN3); break;
		case 7:Digital_out(0, 0, R_IN3, !R_IN3); break;
		case 9:Digital_out(L_IN1, !L_IN1, 0, 0); break;
		case 8: default:Digital_out(L_IN1, !L_IN1, R_IN3, !R_IN3); break;
		}
		Analog_out(a, b);
	}
};
#endif