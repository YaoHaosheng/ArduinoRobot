#ifndef Robot_Conf_h
#define Robot_Conf_h
#include <Arduino.h>
#include "Servo.h"
#include "ColSensor.h"
#include "Track.h"
#include "Car.h"
#include "Model.h"
typedef unsigned long u_l;
#define FXXKDEBUG 0

int N = 12;

int Car_[6] = { 8, 9, 10, 11, 5, 6 };

int Ahead[2] = { 1, 1 };

int T_[5] = { A5, A4, A3, A2, A1 };

bool T_b = true;

int C_[6] = { 0, 1, 3, 4, 2, 10 };

/*舵机设置：1表示控制夹子的舵机，2表示控制机械
臂摆动的舵机，3表示控制机械臂底座旋转的舵机*/
int S_[3] = { 7, 12, A0 };

int angle[3][3];

int side[2];

int bucket[4];

int Tt = 13;

//舵机脉冲最大值
int SERVOS_MAX1 = 2400;
int SERVOS_MAX2 = 2400;
int SERVOS_MAX3 = 2400;

//小车左轮的速度
int SPEED1 = 150;
//小车右轮的速度
int SPEED2 = SPEED1 + 12;
//小车转弯时的速度
int TURN1 = SPEED1 + 30;
int TURN2 = TURN1 + SPEED2 - SPEED1;
//定义机器人循迹单位时间
int V = 2000;

Car car(Car_[0], Car_[1], Car_[2], Car_[3], Car_[4], Car_[5]);
Track track(T_[0], T_[1], T_[2], T_[3], T_[4], T_b);
ColSensor sensor(C_[0], C_[1], C_[2], C_[3], C_[4], C_[5]);

void Counter(){
	++sensor.g_count;
}

Model model;//创意赛需要修改

Servo servo1, servo2, servo3;

/********************舵机转动函数********************/
//控制舵机的转动
void ServoMove(int n, int a, int t = 1){
	if (FXXKDEBUG){
		Serial.println();
		Serial.print("ServoMove ");
		Serial.print(" --num ");
		Serial.print(n);
		Serial.print(" --angle ");
		Serial.print(a); return;
	}
	Servo p = servo1; int m;
	switch (n){
	case 1:p = servo1; m = SERVOS_MAX1; break;
	case 2:p = servo2; m = SERVOS_MAX2; break;
	case 3:p = servo3; m = SERVOS_MAX3; break;
	default:break;
	}
	//考虑到舵机的精确度，此函数用脉冲值来进行控制
	if (a <= 180)a = map(a, 0, 180, 544, m);
	int b = p.readMicroseconds();
	if (b > a)
	for (int i = b - t; i > a; i -= t){
		p.writeMicroseconds(i); delay(1);
	}
	else
	for (int i = b + t; i < a; i += t){
		p.writeMicroseconds(i); delay(1);
	}
	p.writeMicroseconds(a);
}

void Catch(int n){
	if (FXXKDEBUG){
		Serial.println();
		Serial.print("Catch ");
		Serial.print(" --num ");
		Serial.print(n); return;
	}
	servo1.attach(S_[0], 544, SERVOS_MAX1);
	servo2.attach(S_[1], 544, SERVOS_MAX2);
	servo3.attach(S_[2], 544, SERVOS_MAX3);

	ServoMove(2, angle[1][1]); ServoMove(3, angle[2][1]); ServoMove(1, angle[0][1]);
	ServoMove(2,angle[1][2]); ServoMove(1,angle[0][2]); ServoMove(2,angle[1][1]);
	ServoMove(3, bucket[n]); ServoMove(2, angle[1][0]); ServoMove(1, angle[0][0]);
	servo1.detach(); servo2.detach(); servo3.detach();
}

void Down(int n, int k){
	k = k == 1 ? 0 : 1;
	if (FXXKDEBUG){
		Serial.println();
		Serial.print("Down ");
		Serial.print(" --num ");
		Serial.print(n);
		Serial.print(" --ori ");
		Serial.print(k); return;
	}
	servo1.attach(S_[0], 544, SERVOS_MAX1);
	servo2.attach(S_[1], 544, SERVOS_MAX2);
	servo3.attach(S_[2], 544, SERVOS_MAX3);
	static int temp = (angle[1][1] + angle[1][2]) / 2;
	if (angle[2][2] == -1)angle[2][2] = angle[0][1];
	ServoMove(2, angle[1][1]); ServoMove(3, bucket[n]); ServoMove(2, angle[1][0]);
	ServoMove(1, angle[2][2]);ServoMove(1, angle[0][2]); 
	ServoMove(2, angle[1][1]); ServoMove(3,side[k]);
	ServoMove(2, temp); ServoMove(1, angle[0][0]); ServoMove(2, angle[1][1]);
	ServoMove(3, angle[2][0]); ServoMove(2, angle[1][0]);
	servo1.detach(); servo2.detach(); servo3.detach();
}

void Tweet(int m, int n = 1){
	if (FXXKDEBUG){
		Serial.println();
		Serial.print("Tweet ");
		Serial.print(" --cycle ");
		Serial.print(m);
		Serial.print(" --num ");
		Serial.print(n); return;
	}
	m /= 2;
	while (n--){
		digitalWrite(Tt, HIGH);
		delay(m);
		digitalWrite(Tt, LOW);
		delay(m);
	}
}

u_l Tracking(int h = 0, u_l time = 0, int s1 = SPEED1, int s2 = SPEED2){
	int d = 0;
	int temp = 11011; 
	//if (flag == 4)temp = 1111; 
	//else if (flag == 6)temp = 11110;
	int t = 1;
	int tc = 100;
	int L = 4, R = 6, L1 = L, R1 = R;
	u_l limit = 300; limit = limit * 1000 / 633;
	u_l up = 100; up = up * 1000 / 633;
	u_l time1 = millis();
	u_l T = 0;
	int timer = 0;
	if (time)timer = up;
	if (FXXKDEBUG){
		Serial.println();
		Serial.print("Tracking ");
		Serial.print(" --half ");
		Serial.print(h); return V + 1;
	}
	car.Move(s1, s2, 8);
	//if (flag)car.Move(s1, s2, flag);
	while (t){
		t = track.Digital_in();

		if (t == 11111){
			if (-timer != up)--timer;
			if ((++T) > limit){
				car.Move(0, 0, 5);
				/*if (flag){
					car.Move(s1, s2, 10 - flag); 
					while (track.Digital_in() == 11111);
					flag = 0; T = 0; time = millis(); time1 = 0; continue;
				}*/
				while (track.Digital_in() != 11011){ Tweet(tc * 3, 3); };
				Tweet(tc, 3);  temp = 11011; t = 11011; timer = 0; T = 0;
				time = 0; time1 = millis();
			}
		}

		if (h && ((t == 1) || (t == 11) || (t == 111)))t = 0;
		switch (t){
		case 1100: case 1101: case 10100:
		case 1000: case 1001: case 11010:
		case 110: case 101: case 10110:
		case 10: case 10010: case 1011:
		case 100: case 1110: case 10101:
		case 1010:
			t = temp; break;
		default: break;
		}
		switch (t){
		case 11000: case 11100: case 11001: case 11110: case 11101:
			car.Move(s1, s2, R); temp = t; if (-timer != up)--timer; T = 0;
			break;

		case 11: case 111: case 10011: case 1111: case 10111:
			car.Move(s1, s2, L); temp = t; if (-timer != up)--timer; T = 0;
			break;

		case 11011: case 10001: case 1: case 10000: 
			car.Move(s1, s2, 8); temp = t; T = 0;
			if (timer != up) ++timer;
			if (timer == 0){ time = 0; time1 = millis(); }
			break;

		case 0:
			/*使小车越过黑线，防止红外重复检测同一黑线造成判断错
			误，当红外最左端和最右端均检测到白色时退出此循环*/
			car.Move(s1, s2, 8);
			while (!track.Digital_in(0) || !track.Digital_in(4)){ 
				++d; if (timer != up) ++timer;
				if (timer == 0){
					time = 0; time1 = millis();
				}
			}
			if (d <= 3){
				t = 1; temp = 11011; d = 0; T = 0;
			} else t = 0; break;
		default:break;
		}
		//delay(d);
	}
	if (timer < 0)time = 0;
	else time = millis() - time1 + time;
	car.Move(0, 0, 5);
	return time;
}
//小车原地转弯
u_l Turn(int n = 1, int t1 = TURN1, int t2 = TURN2){
	static u_l DELAY = 0;
	static int temp = t1;
	if (FXXKDEBUG){
		Serial.println();
		Serial.print("Turn ");
		Serial.print(" --ori ");
		Serial.print(n); return V;
	}
	if (n == 0)return 0;
	if (n == 1 || n == -1){
		u_l time = 0;
		car.Move(t1, t2, n == 1 ? 4 : 6);
		time = millis();
		delay(100);
		while (!track.Digital_in(2));
		while (track.Digital_in(2));
		int tempo = n + 2;
		while (!track.Digital_in(tempo));
		tempo += n;
		while (!track.Digital_in(tempo));
		car.Move(0, 0, 5);
		time = millis() - time; 
		car.Move(0, 0, 5);
		if (t1 - temp > 3||DELAY == 0){
			DELAY = time; temp = t1;
		} return DELAY;
	}
	int o = n > 0 ? 4 : 6;	
	u_l d = DELAY * 3 / 2;
	//if (n != 1)d += DELAY*(n - 1) / 2;
	car.Move(t1, t2, o); delay(d);
	while (track.Digital_in(2));
	int tempo = 7 - o;
	while (!track.Digital_in(tempo));
	tempo = 12 - 2 * o;
	while (!track.Digital_in(tempo));
	car.Move(0, 0, 5);
	return d;
}
//智能刹车
int Brake(u_l t, int m = 0, int s1 = SPEED1, int s2 = SPEED2){
	u_l time = 0;
	int result = 1;
	int s = (s1 + s2) / 2;
	static int f = s * 15 / 16;
	static int flag = 1;
	static int step = 8;
	//u_l T = (s - f)*V*V / 3;
	//int S = 3 * t / (V*V) - f;
	//if (S < 0)S = 0;
	//if (S>255)S = 255;
	if (f > s)f = s;
	t = t * (s - f) / (255 + f);
	int wb = 1;
	if (FXXKDEBUG){
		Serial.println();
		Serial.print("Brake "); return 1;
	}
	//if (t > 600)t = 600;
	//int flag = 0;
	if (m == 0){ t = t / 4 + 1; }
	time = millis(); car.Move(255, 255, 2);
	while ((millis() - time) < t && (track.Digital_in(0) || (wb = 0)));
	time = millis(); car.Move(0, 0, 5); t *= 2;
	while ((millis() - time) < t){
		if (track.Digital_in(0) || (wb = 0));
	}

	if (wb == 0){
		result = 0;
		int temp = track.Digital_in(0);
		car.Move(s1 * 3 / 4, s2 * 3 / 4, 8);
		while (track.Digital_in(0) == temp);
		car.Move(0, 0, 5); delay(t);
		if (flag == -1){
			flag = 1; step = (step + 1) / 2;
		}
	}
	else{
		if (flag == 1){
			flag = -1; step = (step + 1) / 2;
		}
	}
	if (flag == 1 || (m && flag == -1)){
		f = f + flag * step;
	}
	return result;
}
//加速
void Quicken(u_l t1, u_l t2 = V){
	if (FXXKDEBUG){
		Serial.println();
		Serial.print("Quicken "); return;
	}
	if (t1 <= t2)return;
	int M = max(SPEED1, SPEED2);
	M = max(M, TURN1); M = max(M, TURN2);
	u_l S = t1 * M / t2;
	if (S > 255)S = 255;
	int p = S - M;
	SPEED1 += p;
	SPEED2 += p;
	TURN1 += p;
	TURN2 += p;
}
//进岔路
u_l Enter(int n = 1, int t1 = TURN1, int t2 = TURN2, int s1 = SPEED1, int s2 = SPEED2){
	if (n == 0)n = 2;
	if (FXXKDEBUG){
		Serial.println();
		Serial.print("Enter "); 
	}
	Turn(n, t1, t2); 
	u_l time = Tracking(0, 0, s1, s2);
	if (Brake(time, 1)&&!FXXKDEBUG){
		/*car.Move(s1, s2, 2);
		u_l t = millis();
		while (track.Digital_in() == 11111 && (millis() - t) <= 30);
		car.Move(0, 0, 5);*/
	}
	Quicken(time); 
	return time;
}
//出岔路
u_l Out(int t1 = TURN1, int t2 = TURN2, int s1 = SPEED1, int s2 = SPEED2){
	if (FXXKDEBUG){
		Serial.println();
		Serial.print("Out ");
	}
	Turn(2, t1, t2);
	u_l time = Tracking(0, 0, s1, s2);
	Brake(time);
	Quicken(time);
	return time;
}

u_l Autodown(int c, int k, u_l t = 0, int f = 1){
	int j = 0, flag = 1;
	if (FXXKDEBUG){
		Serial.println();
		Serial.print("Autodown ");
	}
	while (j = model.Findfull(c, f)){
		if (flag){
			Brake(t); flag = 0; t = 0;
		}
		Down(j, k);
		if (!model.Settra(j))Tweet(100, 100);
	}
	return t;
}

u_l Next(int p, u_l t = 0, int k = 1, int dump = 0, int f = 1){
	int h = 0;
	if (k == 0) return t;
	if (FXXKDEBUG){
		Serial.println();
		Serial.print("Next ");
		Serial.print(" --now ");
		Serial.print(p);
	}
	if ((k == -1) && ((p == 1) || (p == (N + 1))))h = 1;
	t = Tracking(h, t);

	if ((k == 1) && ((p == N) || (p == (N + 3)))){
		Brake(t); Turn(-1); t = 0;
		if (p == N) {
			t = Tracking(); //t = t / 2;
		}
	}
	else if (h){
		Brake(t); Turn(); t = 0;
		if (p == N + 1) {
			t = Tracking(); //t = t / 2;
		}
	}

	int temp = p;
	p += k; 
	if (p < 0)p = N + 3;
	else if (p > N + 3)p = 0;
	if (dump && p > N)t = Autodown(p - N, k, t, f);
	if ((2 * temp + k) == (N + 1))t = 0;
	return t;
}

int Goto(int p1, int n, int k, int p2){	
	int f = 1, dump = 1;
	if (FXXKDEBUG){
		Serial.println();
		Serial.print("Goto ");
	}
	if ((p2 == -1) || (p2 == -2)){
		dump = 1;
		f = 2 + p2;
		p2 = 3;
		if (k == -1)p2 = 1;
		for (int i = 0; i < 3; ++i){
			if (model.Findfull(p2, f)){
				p2 += N; break;
			}
			p2 -= k;
		}
	}
	/*else if (((k == 1) && p1 > p2) || ((k == -1) && p1 < p2)){
		int t = 3;
		if (k == -1)t = 1;
		for (int i = 0; i < 3; ++i){
			if (model.Findfull(t)){
				t += N; break;
			}
			t -= k;
		}
		if (t > N)p2 = t;
	}*/
	Turn(n);
	u_l time = 0;
	/*int flag = 0;
	if (n > 0)flag = 4;
	else if (n < 0)flag = 6;*/
	for (int i = p1; i != p2;){
		time = Next(i, time, k, dump, f);
		/*if (flag){
			flag = 0;
		}*/
		i += k;
		if (i > N + 3)i = 0;
		else if (i < 0)i = N + 3;
		/*if (i == 0){
			if (k == 1)flag = 6;
			else if (k == -1)flag = 4;
		}*/
	}
	Brake(time);
	return p2;
}

void InitSer(){
	if (FXXKDEBUG){
		Serial.println();
		Serial.print("InitSer ");
	}
	servo3.attach(S_[3], 544, SERVOS_MAX1);
	servo2.attach(S_[2], 544, SERVOS_MAX2);
	servo1.attach(S_[1], 544, SERVOS_MAX3);
	for (int i = 2; i >= 0; --i)
		ServoMove(i + 1, angle[i][0]);
	servo1.detach(); servo2.detach(); servo3.detach();
}

void Run(){
	if (FXXKDEBUG){
		Serial.println();
		Serial.print("Run ");
		Serial.print(" --R "); Serial.print(model.Getbkt(1));
		Serial.print(" --B "); Serial.print(model.Getbkt(2));
		Serial.print(" --G "); Serial.println(model.Getbkt(3));
		for (int i = N / 2; i > 0; --i){
			Serial.print(model.Getblk(i));
			Serial.print(" ---------- ");
			Serial.println(model.Getblk(N + 1 - i));
		}
	}
	int n = model.Strategy();
	int p = model.Getpos();
	int o = model.Getori();
	int k = model.Cal(n);
	if (k > 0)k = 1;
	else if (k < 0)k = -1;

	n = Goto(p, model.Turn(k), k, n);

	if (n == 0) {
		if (FXXKDEBUG)Serial.print("END");
		model.Setsts(0, k == 1 ? 8 : 6);
		while (true);
	}

	if (n <= N){
		Enter(k);
		int c = model.Getblk(n);
		if (!c){
			if (FXXKDEBUG){
				Serial.print("Press...");
				Serial.print(" --num ");
				Serial.println(n);
				while (true){
					if (Serial.available() > 0){
						c = Serial.read() - '0';
						if (c <= 3 && c >= 0)break;
					}
				}	
			}
			if (!FXXKDEBUG && (c = sensor.Get()))Tweet(100, c);
			if(!model.Setblk(c == 0 ? -1 : c, n))Tweet(100,100);
		}
		if (c){
			int b = model.Getbkt(c);
			if (b){
				Catch(b); if (!model.Setcln(b, n))Tweet(100, 100);
			}
		}
		Out(); 
		model.Setsts(n, n <= N / 2 ? 6 : 4); 
		return;
	}
	model.Setsts(n, k == 1 ? 4 : 6);
}
//调节白平衡
void Ready(){
	sensor.SetFunc(Counter);
	if (FXXKDEBUG){
		Serial.println();
		Serial.print("Ready "); 
		while (true){
			if (Serial.available() > 0){
				int t = Serial.read();
				if (t == 'y'){
					model.ROMread(); break;
				}
				else if(t== 'n'){
					model.ROMclr();break;
				}
			}
		}
		return;
	}
	sensor.Test();
	Tweet(500, 3);
	while (!sensor.Get(true));
	Tweet(200, 3); 
	if (sensor.Get(true)){
		Tweet(500, 3);
		model.ROMread();
	}
	else{
		model.ROMclr();
	}
}

void Init(){
	Serial.begin(9600);
	if (FXXKDEBUG){
		Serial.print("Init ");
	}
	pinMode(Tt, OUTPUT);
	car.Mode();
	car.Infer(Ahead[0], Ahead[1]);
	track.Mode();
	sensor.Mode();
	InitSer();
	track.Check();
	Ready();
}
#endif