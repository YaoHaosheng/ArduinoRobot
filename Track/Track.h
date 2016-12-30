#ifndef Track_h
#define Track_h

#include <Arduino.h>
class Track{
private:
	//定义引脚，下标0至4对应从左到右五个红外
	int Light[5];
	//定义阀值，用以区分黑和白
	int NUM[5];
	//若为true，则高于阈值时是白色；若为false，则高于阈值时是黑色。默认为true
	bool flag;
	//用于存储读取的模拟量
	int value[5];
	void Tweet(int pin, int m, int n){
		m /= 2;
		while (n--){
			digitalWrite(pin, HIGH);
			delay(m);
			digitalWrite(pin, LOW);
			delay(m);
		}
	}
public:
	Track(int a, int b, int c, int d, int e, bool f = true){
		Light[0] = a; Light[1] = b; Light[2] = c;
		Light[3] = d; Light[4] = e; flag = f;
	}
	//将引脚配置为输入
	void Mode(){
		for (int i = 0; i<5; i++)
			pinMode(Light[i], INPUT);
	}
	//设置阀值，区分黑白
	void Set(int a, int b, int c, int d, int e){
		NUM[0] = a; NUM[1] = b; NUM[2] = c; NUM[3] = d; NUM[4] = e;
	}
	//读取模拟量
	void Test(){
		for (int i = 0; i < 5; ++i)
			value[i] = analogRead(Light[i]);
	}
	//自动设置阈值
	void Check(int pin = 13, int n = 16){
		pinMode(pin, OUTPUT);

		int test1[5] = { 0, 0, 0, 0, 0 };
		int test2[5] = { 0, 0, 0, 0, 0 };
		int t[5] = { 0, 0, 0, 0, 0 };
		//将红外全部对准黑线部分放置
		Tweet(pin, 200, 15);
		for (int i = 0; i < n; ++i){
			Test();
			Serial.println("TestB");
			for (int i = 0; i < 5; ++i){
				test1[i] += value[i];
				Serial.println(value[i]);
			}
		}
		//将红外全部对准白色部分放置
		Tweet(pin, 500, 6);
		for (int i = 0; i < n; ++i){
			Test();
			Serial.println("TestW");
			for (int i = 0; i < 5; ++i){
				test2[i] += value[i];
				Serial.println(value[i]);
			}
		}
		for (int i = 0; i<5; ++i){
			test1[i] /= n;
			test2[i] /= n;
			t[i] = (test1[i] + test2[i]) / 2;
		}
		Set(t[0], t[1], t[2], t[3], t[4]);
		Tweet(pin, 200, 3);
	}
	//读取信息，并返回对应的整型值
	int Digital_in(int a = 5){
		int sum = 0;
		Test();
		for (int i = 0; i < 5; ++i){
			if (a == i)return (value[i] > NUM[i] == flag);
			sum = sum * 10 + (value[i] > NUM[i] == flag);
		}
		return sum;
	}
};
#endif