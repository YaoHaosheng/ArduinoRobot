#ifndef ColSensor_h
#define ColSensor_h

#include <Arduino.h>
typedef void(*Count)();
class ColSensor{
private:

	//��������
	int S0, S1, S2, S3, OUT;

	//������ʱ
	int time;
	//�������飬������Ŷ�ȡ����RGBֵ
	int g_array[3];

	//�������飬������ű���ϵ��
	float g_SF[3];

	int code[4];
	/*�����������ţ��л��˲������죬�̣����ֱ��
	ӦS2��S3��00��11��01��ƽ��10ʱ����������ɫ*/
	void Digital_out(int a, int b){
		digitalWrite(S2, !(a == 0));
		digitalWrite(S3, !(b == 0));
	}

public:

	//���������
	volatile unsigned long g_count;

	//���캯��
	ColSensor(int a, int b, int c, int d, int e, int f){
		S0 = a; S1 = b; S2 = c; S3 = d; OUT = e; time = f;
		g_count = 0; code[0] = 0; Setcode(1, 3, 2);
	}

	void Setcode(int r, int g, int b){
		code[1] = r; code[2] = g; code[3] = b;

	}
	//�����жϴ�����
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

	//����������Ϊ�����ͬʱ���������������
	void Mode(int a = 1, int b = 1){
		pinMode(S0, OUTPUT);
		pinMode(S1, OUTPUT);
		pinMode(S2, OUTPUT);
		pinMode(S3, OUTPUT);
		pinMode(OUT, INPUT);
		digitalWrite(S0, !(a == 0));
		digitalWrite(S1, !(b == 0));
	}

	//�������ϵ��
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

	//����RGBֵ�������ؽ��
	/*��ⷽ����ɫ��������һ��ֵ��1��ʾ��ɫ��2��ʾ��ɫ��3��ʾ��ɫ��0��ʾû�м�⵽��
	ɫ�����ǵ���ɫ����������Ч����Ƚ϶̣�Ϊ����߼������׼ȷ�ʣ��������ڲ��ò�ֵ��
	�ϵķ�������ԭ���ǣ����ҳ�RGBֵ�е����ֵ��ֻҪ������������ֵ�е�ĳһ�����a
	�����ϣ����⵽����ɫ�����ֵ����Ӧ����ɫȷ������������Ϊû�м�⵽����Ҫ�����ɫ*/
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