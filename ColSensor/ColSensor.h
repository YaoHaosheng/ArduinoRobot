#ifndef ColSensor_h
#define ColSensor_h

#include <Arduino.h>
typedef void(*Count)();
class ColSensor{
private:

	//定义引脚
	int S0, S1, S2, S3, OUT;

	//定义延时
	int time;
	//定义数组，用来存放读取到的RGB值
	int g_array[3];

	//定义数组，用来存放比例系数
	float g_SF[3];

	int code[4];
	/*控制数字引脚，切换滤波器。红，绿，蓝分别对
	应S2与S3的00，11，01电平；10时过滤所有颜色*/
	void Digital_out(int a, int b){
		digitalWrite(S2, !(a == 0));
		digitalWrite(S3, !(b == 0));
	}

public:

	//定义计数器
	volatile unsigned long g_count;

	//构造函数
	ColSensor(int a, int b, int c, int d, int e, int f){
		S0 = a; S1 = b; S2 = c; S3 = d; OUT = e; time = f;
		g_count = 0; code[0] = 0; Setcode(1, 3, 2);
	}

	void Setcode(int r, int g, int b){
		code[1] = r; code[2] = g; code[3] = b;

	}
	//设置中断处理函数
	void SetFunc(Count c){
		int i = OUT;
		switch (i){
		case 2: case 3:
			i -= 2; break;
		case 18: case 19: case 20: case 21:
			i = 23 - i; break;
		default:
			i = 0; break;
		}
		attachInterrupt(i, c, RISING);
	}

	//将引脚配置为输出，同时设置输出比例因子
	void Mode(int a = 1, int b = 1){
		pinMode(S0, OUTPUT);
		pinMode(S1, OUTPUT);
		pinMode(S2, OUTPUT);
		pinMode(S3, OUTPUT);
		pinMode(OUT, INPUT);
		digitalWrite(S0, !(a == 0));
		digitalWrite(S1, !(b == 0));
	}

	//计算比例系数
	void Test(int n = 3){
		Serial.println("Test");
		g_array[0] = 0; g_array[1] = 0; g_array[2] = 0;
		for (int j = 0; j < 3; ++j){
			for (int i = 0; i < n; ++i){
				g_count = 0;
				switch (j){
				case 0: Digital_out(0, 0); break;
				case 1: Digital_out(1, 1); break;
				case 2: Digital_out(0, 1); break;
				default:break;
				}
				delay(this->time);
				g_array[j] += g_count;
			}
			g_array[j] /= n;
			g_SF[j] = 255.0 / g_array[j];
		}
		Digital_out(1, 0);
		g_count = 0;
		Serial.println(g_array[0]);
		Serial.println(g_array[1]);
		Serial.println(g_array[2]);
		Serial.println(g_SF[0]);
		Serial.println(g_SF[1]);
		Serial.println(g_SF[2]);
	}

	//计算RGB值，并返回结果
	/*检测方块颜色，并返回一个值。1表示红色，2表示绿色，3表示蓝色，0表示没有检测到颜
	色。考虑到颜色传感器的有效距离比较短，为了提高检测结果的准确率，本程序在采用差值比
	较的方法。其原理是：先找出RGB值中的最大值，只要它比其它两个值中的某一个相差a
	或以上，则检测到的颜色由最大值所对应的颜色确定；否则则认为没有检测到符合要求的颜色*/
	int Get(bool none = false, int a = 128, int n = 3){
		Serial.println("Get");
		int result = 0;
		while (n-- && !result){
			for (int i = 0; i < 3; ++i){
				g_count = 0;
				switch (i){
				case 0: Digital_out(0, 0); break;
				case 1: Digital_out(1, 1); break;
				case 2: Digital_out(0, 1); break;
				default:break;
				}
				delay(this->time);
				g_array[i] = g_count;
				g_array[i] = g_SF[i] * g_array[i];
			}
			Digital_out(1, 0);
			int i = 0, min1 = 1, min2 = 2;
			if (g_array[1] > g_array[i]){
				min1 = i; i = 1;
			}
			if (g_array[2] > g_array[i]){
				min2 = i; i = 2;
			}

			if (n == 0 && !none)result = i + 1;
			else if ((g_array[i] - g_array[min1]) > a &&
				(g_array[i] - g_array[min2]) > a)result = i + 1;
		}
		Serial.println(g_array[0]);
		Serial.println(g_array[1]);
		Serial.println(g_array[2]);
		return code[result];
	}
};
#endif