#ifndef Track_h
#define Track_h

#include <Arduino.h>
class Track{
private:
	//�������ţ��±�0��4��Ӧ�������������
	int Light[5];
	//���巧ֵ���������ֺںͰ�
	int NUM[5];
	//��Ϊtrue���������ֵʱ�ǰ�ɫ����Ϊfalse���������ֵʱ�Ǻ�ɫ��Ĭ��Ϊtrue
	bool flag;
	//���ڴ洢��ȡ��ģ����
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
	//����������Ϊ����
	void Mode(){
		for (int i = 0; i<5; i++)
			pinMode(Light[i], INPUT);
	}
	//���÷�ֵ�����ֺڰ�
	void Set(int a, int b, int c, int d, int e){
		NUM[0] = a; NUM[1] = b; NUM[2] = c; NUM[3] = d; NUM[4] = e;
	}
	//��ȡģ����
	void Test(){
		for (int i = 0; i < 5; ++i)
			value[i] = analogRead(Light[i]);
	}
	//�Զ�������ֵ
	void Check(int pin = 13, int n = 16){
		pinMode(pin, OUTPUT);

		int test1[5] = { 0, 0, 0, 0, 0 };
		int test2[5] = { 0, 0, 0, 0, 0 };
		int t[5] = { 0, 0, 0, 0, 0 };
		//������ȫ����׼���߲��ַ���
		Tweet(pin, 200, 15);
		for (int i = 0; i < n; ++i){
			Test();
			Serial.println("TestB");
			for (int i = 0; i < 5; ++i){
				test1[i] += value[i];
				Serial.println(value[i]);
			}
		}
		//������ȫ����׼��ɫ���ַ���
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
	//��ȡ��Ϣ�������ض�Ӧ������ֵ
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