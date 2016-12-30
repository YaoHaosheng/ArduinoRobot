#ifndef Car_h
#define Car_h

#include <Arduino.h> 
class Car{
private:
	//�������ţ�L_IN1��R_IN3�ֱ�ΪС��ǰ��ʱIN1��IN3�ĵ�ƽ
	int IN1, IN2, IN3, IN4, MotorA, MotorB, L_IN1, R_IN3;

	//�����������ŵ�ƽ������������������ת����
	void Digital_out(int a, int b, int c, int d){
		digitalWrite(IN1, a);
		digitalWrite(IN2, b);
		digitalWrite(IN3, c);
		digitalWrite(IN4, d);
	}

	//����PWM���ţ����������������ٶ�
	void Analog_out(int a, int b){
		analogWrite(MotorA, a);
		analogWrite(MotorB, b);
	}

public:
	//���캯����Ĭ��L_IN1��R_IN3Ϊ1
	Car(int a, int b, int c, int d, int e, int f){
		IN1 = a; IN2 = b; IN3 = c; IN4 = d; MotorA = e; MotorB = f;
		L_IN1 = 1, R_IN3 = 1;
	}

	//����������Ϊ���
	void Mode(){
		pinMode(IN1, OUTPUT);
		pinMode(IN2, OUTPUT);
		pinMode(IN3, OUTPUT);
		pinMode(IN4, OUTPUT);
		pinMode(MotorA, OUTPUT);
		pinMode(MotorB, OUTPUT);
	}

	//������С��ǰ��ʱIN1��IN3�ĵ�ƽ���Դ˶�Move������������
	void Infer(int a, int b){
		L_IN1 = a; R_IN3 = b;
	}

	/*��һ�������βηֱ��ʾMotorA��MotorB��PWM���������β�ΪС
	���˶��ķ�ʽ��ȡֵΪ1��9���ֱ��ʾ�������ת�����ˣ������Һ�
	ת��˫����ת��ֹͣ��˫����ת��������ǰת��ǰ����������ǰת��
	���ĸ��βα�ʾ�ӳ�ʱ�䣬Ĭ��Ϊ1ms�������ô˺������Infer
	������С��ǰ��ʱIN1��IN3�ĵ�ƽ����ȷ�ϣ��Ӷ������˺���*/
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