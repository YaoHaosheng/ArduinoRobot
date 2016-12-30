#ifndef Model_h
#define Model_h

#include <Arduino.h>
#include <EEPROM.h>
#define Model_N 12
#define PRINTLN_(string, state) //{Serial.print(string);Serial.println(state);}
#define ENDBY_(end) //return end;

class Model{
private:

	int num;
	int Memory[Model_N + 1];
	int Trash[4];
	int Bucket[4];
	int BktCol[4];
	int Pred[4];
	int pos;
	int ori;

public:

	Model(int n = 1, int r = 4, int g = 4, int b = 4, int p = 0, int o = 8){
		num = Model_N;
		for (int i = 0; i < 4; ++i){
			Bucket[i] = 0; BktCol[i] = 0;
			Trash[i] = 0;
		}
		for (int j = 0; j <= num; ++j)
			Memory[j] = 0;
		Bucket[0] = n;
		Pred[1] = r; Pred[2] = g; Pred[3] = b;
		Pred[0] = num - (r + g + b); pos = p; ori = o;
		if (Pred[0] < 0)while (true);
	}

	void ROMput(char c,int n,int v){
		static int f = sizeof(int), m = 1, p = m + (Model_N + 1)*f;
		int temp = 0;
		switch (c){
		case 'M':
			temp = m; break;
		case 'P':
			temp = p; break;
		default: break;
		}
		EEPROM.put(temp + n*f, v);
	}

	void ROMread(){
		int m = 1;
		EEPROM.get(m, Memory);
		EEPROM.get(m + (Model_N + 1)*sizeof(int), Pred);
	}

	void ROMclr(){
		int m = 1;
		EEPROM.put(m, Memory);
		EEPROM.put(m + (Model_N + 1)*sizeof(int), Pred);
	}

	int Setblk(int c, int p = -1){
		if (p == -1)p = pos;
		if (Memory[p])return 0;

		int flag = c;
		if (c == -1) {
			++Memory[0];
			flag = 0;
			ROMput('M', 0, Memory[0]);
		}
		if (Pred[flag] == 0){
			int t = 0;
			randomSeed(millis());
			++Pred[random(0, 4)];
			while (!Pred[t = random(0, 4)]);
			--Pred[t];
			while (!Pred[t = random(0, 4)]);
			flag = t;
		}
		--Pred[flag];
		Memory[p] = c;
		for (int i = 0; i < 4; ++i)ROMput('P', i, Pred[i]);
		ROMput('M', p, Memory[p]);
		return c;
	}

	int Setcln(int b, int p = -1){
		if (p == -1)p = pos;
		if (Bucket[b] == Bucket[0] ||
			(BktCol[b] && BktCol[b] != Memory[p]) || Memory[p] == 0)return 0;
		BktCol[b] = Memory[p];
		Memory[p] = -1;
		ROMput('M', p, -1);
		return ++Bucket[b];
	}

	int Settra(int b){
		if (Bucket[b] <= 0 || BktCol[b] <= 0)return 0;
		Trash[b] += Bucket[b];
		Bucket[b] = 0;
		BktCol[b] = 0;
		return Trash[b];
	}

	void Setsts(int p, int o){
		pos = p;
		ori = o;
	}

	int Getblk(int p){
		return Memory[p];
	}

	int Getpos(){
		return pos;
	}

	int Getori(){
		return ori;
	}
	/*******************************************/
	int Getbkt(int c, int s = 1){
		if (s == 1){
			if (Bucket[c] < Bucket[0])return c;
			return 0;
		}
		/*for (int i = 1; i <= 3; ++i){
			if (BktCol[i] == c && Bucket[i] < Bucket[0])return i;
		}*/
		if (s == 0){
			for (int i = 1; i <= 3; ++i){
				if (BktCol[i] == 0)return i;
			}
			return 0;
		}
	}

	int Search(int c, int k, int p = -1){
		if (p == -1) p = pos;
		if (p > num) p = k == 1 ? 0 : num + 1;
		for (int i = 0; i < num; ++i){
			p += k;
			if (p <= 0) p = num;
			else if (p >= num + 1)p = 1;
			if (Memory[p] == c || (c == 4 && Memory[p] > 0))return p;
		}
		return -1;
	}

	int Getsit(int c, int k = 1, int p = -1){
		return Search(c, k, p);
	}

	int Getukw(int k = 1, int p = -1){
		return Search(0, k, p);
	}

	int Getuch(int k = 1, int p = -1){
		return Search(4, k, p);
	}

	int Cal(int n, int m = -1){
		int p = pos;
		if (m != -1) p = m;
		if (n == -1)n = num + 1;
		if (p == n) return 0;

		//if (p == 0)return 1;

		if (n == 0||p == 0){
			int f = 1;
			if (p == 0){
				p = n; n = 0; f = -1;
			}
			if (p > num)return (4 - (p - num))*f;
			if (p <= num / 2)return (-p)*f;
			return (num - p + 1 + 1 + 4)*f;
		}

		int k = 1;
		if (p > n){
			int t;
			t = p; p = n; n = t; k = -1;
		}

		if (p > num) return k * (n - p);

		if (n > num)
		if (p <= num / 2) return -k * (p + 1 + 4 - (n - num));
		else return k * (num - p + 1 + 1 + n - num);

		if (n <= num / 2 || p > num / 2)return k * (n - p);

		if (num - 2 * (n - p) > 0)return k * (n - p + 8 - 1);
		else return -k * (p + 6 + 1 + num - n);
	}

	int Turnto(int o2, int o1 = 5){
		if (o1 == 5)o1 = ori;
		if (o1 == o2)return 0;
		if (o1 + o2 == 10)return 2;
		o1 = o1 * 2 + o2;
		if (o1 == 10 || o1 == 20)return 1;
		else return -1;
	}

	int Curori(int k, int p = -1){
		if (p == -1)p = pos;
		int o = ori;
		if (k == 0)
			if (p <= num / 2)o = p ? 4 : 8;
			else if (p <= num)o = 6;
			else o = 5;
		else if (p == 0) o = k == 1 ? 8 : 6;
		else if (p > num) o = k == 1 ? 4 : 6;
		else if (p <= num / 2) o = k == 1 ? 8 : 2;
		else o = k == 1 ? 2 : 8;
		return o;
	}

	int Turn(int k, int o = 5, int p = -1){
		int oo = Curori(k, p);
		if (oo == 5)oo = o;
		return Turnto(oo, o);
	}

	int Findfull(int c, int f = 1){
		for (int i = 1; i <= 3; ++i){
			if (BktCol[i] == c &&
				(Bucket[i] == Bucket[0] || (f == 0 && Bucket[i])))return i;
		}
		return 0;
	}

	int Deduce(){
		int sum = 0, x = 0;
		int flag[5]; flag[0] = 1; flag[4] = 0;
		for (int i = 1; i <= 3; ++i){
			flag[i] = Getbkt(i) ? 1 : 0;
		}
		for (int i = 0; i < 4; ++i){
			sum += Pred[i];
			if (flag[i])x += Pred[i];
			else if (Pred[i])++flag[4];
		}
		if (x == 0)return 0;
		if (x >= (sum - flag[4]))return 1;
		for (int i = 1; i <= 3; ++i){
			if (flag[i] && Pred[i] && (Getsit(i) == -1))return 1;
		}
		//if (2 * x >= sum)return 1;
		return 0;
	}

#define Model_Calculate(xp, col) { int p1[2] = { Search(col, 1), Search(col, -1) }; \
	int rp1[2] = { Cal(p1[0]), Cal(p1[1]) }; \
	int t = abs(rp1[0]) > abs(rp1[1]) ? 1 : 0; \
	xp[0] = p1[t]; xp[1] = rp1[t]; \
	}
	int Strategy(){
		int p = pos;
		//if (p == 0)return 1;
		for (int i = 1; i <= num; ++i)
			PRINTLN_("Memory:", Memory[i])
		for (int i = 0; i <= 3; ++i){
			PRINTLN_("Bucket:", Bucket[i])
				PRINTLN_("BktCol:", BktCol[i])
				PRINTLN_("Pred:", Pred[i])
		}
		PRINTLN_("pos:", pos)
			PRINTLN_("ori:", ori)
			int B_[4] = { 0, Getbkt(1), Getbkt(2), Getbkt(3) };
		for (int i = 0; i <= 3; ++i){
			PRINTLN_("B_:", B_[i])
		}
		if (!B_[1] && !B_[2] && !B_[3])return -1;

		int cp[5][2];
		for (int i = 0; i < 5; ++i)
			Model_Calculate(cp[i], i)

		for (int i = 0; i < 5; ++i){
			PRINTLN_("cp0:", cp[i][0])
				PRINTLN_("cp1:", cp[i][1])
		}
		if (0){
			for (int i = 0; i < 5; ++i)
				for (int j = 0; j < 2;++j)
			Serial.println(cp[i][j]);
		}
		if (cp[0][0] == -1 && cp[4][0] == -1){
			if (Bucket[1] || Bucket[2] || Bucket[3])return -2;
			else return 0;
		}

		if (B_[1] && B_[2] && B_[3]){
			if (cp[0][0] != -1)return cp[0][0];
			if (cp[4][0] != -1)return cp[4][0];
		}

		int pp = -1;
		for (int i = 1; i <= 3; ++i){
			if (cp[i][0] != -1 && B_[i]){
				if (pp == -1 || abs(cp[pp][1]) > abs(cp[i][1]))pp = i;
			}
		}
		int d = Deduce();
		PRINTLN_("Deduce:", d)
		if (pp == -1 && cp[0][0] == -1)return -1;
		if (cp[0][0] == -1)return cp[pp][0];
		if (d)return cp[0][0];
		if (pp == -1)return -1;
		return cp[pp][0];
	}
};
#endif