#include <iostream>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <map>
#include <windows.h>
using namespace std;
#define F(x,y,z) ((x&y)|((~x)&z))
#define G(x,y,z) ((x&y)|(x&z)|(y&z))
#define H(x,y,z) (x^y^z)
#define lshift(x,s) (((x) << s) | ((x) >> (32 - s)))
#define rshift(x,s) (((x) >> s) | ((x) << (32 - s)))
#define getbit(x,s) ((x << (32 - s)) >> 31)
#define modbit(x,s) (x=(x^(1<<(s-1))))
#define Rand ( (lshift(rand(),7))^(lshift(rand(),17))^(lshift(rand(),24))^(lshift(rand(),28)))
typedef unsigned int uint;
uint a[13];
uint b[13];
uint c[13];
uint d[13];

bool saved;
uint a1[13];
uint b1[13];
uint c1[13];
uint d1[13];
uint m[] = { 0x140acb55,0xb62bd23f,0xd78be412,0x89413d47,0xbde23e1c,0x786d57fb,0x1d74004a,0x749c140d,
0x99d7b41e,0xacbb2207,0xbfcf20c8,0xfacfa9fa,0x9727ce58,0xf500e393,0x2cae1196,0x8a27b5b3
};
uint m1[] = { 0x140acb55,0x362bd23f,0x478be412,0x89413d47,0xbde23e1c,0x786d57fb,0x1d74004a,0x749c140d,
0x99d7b41e,0xacbb2207,0xbfcf20c8,0xfacfa9fa,0x9726ce58,0xf500e393,0x2cae1196,0x8a27b5b3 };

uint aa = 0x67452301;
uint bb = 0xefcdab89;
uint cc = 0x98badcfe;
uint dd = 0x10325476;
uint round1(uint i, uint j, uint k, uint l, uint t, uint shift)
{
	return lshift((i + F(j, k, l) + t), shift);
}
uint round2(uint i, uint j, uint k, uint l, uint t, uint shift)
{
	return lshift((i + G(j, k, l) + t + 0x5a827999), shift);
}
uint round3(uint i, uint j, uint k, uint l, uint t, uint shift)
{
	return lshift((i + H(j, k, l) + t + 0x6ed9eba1), shift);
}

void md4(char *result, uint* m)
{
	a[0] = aa;
	b[0] = bb;
	c[0] = cc;
	d[0] = dd;

	uint que[16] = { 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15 };
	uint t = 0;
	for (uint i = 1; i <= 4; i++)
	{
		a[i] = round1(a[i - 1], b[i - 1], c[i - 1], d[i - 1], m[que[t++]], 3);
		d[i] = round1(d[i - 1], a[i], b[i - 1], c[i - 1], m[que[t++]], 7);
		c[i] = round1(c[i - 1], d[i], a[i], b[i - 1], m[que[t++]], 11);
		b[i] = round1(b[i - 1], c[i], d[i], a[i], m[que[t++]], 19);
	}

	t = 0;
	uint que2[16] = { 0,4,8,12,1,5,9,13,2,6,10,14,3,7,11,15 };
	for (uint i = 5; i <= 8; i++)
	{
		a[i] = round2(a[i - 1], b[i - 1], c[i - 1], d[i - 1], m[que2[t++]], 3);
		d[i] = round2(d[i - 1], a[i], b[i - 1], c[i - 1], m[que2[t++]], 5);
		c[i] = round2(c[i - 1], d[i], a[i], b[i - 1], m[que2[t++]], 9);
		b[i] = round2(b[i - 1], c[i], d[i], a[i], m[que2[t++]], 13);
	}

	t = 0;
	uint que3[16] = { 0,8,4,12,2,10,6,14,1,9,5,13,3,11,7,15 };
	for (uint i = 9; i <= 12; i++)
	{
		a[i] = round3(a[i - 1], b[i - 1], c[i - 1], d[i - 1], m[que3[t++]], 3);
		d[i] = round3(d[i - 1], a[i], b[i - 1], c[i - 1], m[que3[t++]], 9);
		c[i] = round3(c[i - 1], d[i], a[i], b[i - 1], m[que3[t++]], 11);
		b[i] = round3(b[i - 1], c[i], d[i], a[i], m[que3[t++]], 15);
	}
	sprintf(result, "%x%x%x%x%c\n", a[12]+a[0], b[12]+b[0], c[12]+c[0], d[12]+d[0], 0x00);
}
bool coRight()
{
	for (int i = 0; i < 16; i++)m1[i] = m[i];
	m1[1] = m[1] + uint(0x80000000);
	m1[2] = m[2] + uint(0x80000000)-uint(0x10000000);
	m1[12] = m[12] - uint(0x00010000);
	char result[130];
	char result1[130];
	md4(result, m);
	md4(result1, m1);
	if (strcmp(result, result1) == 0)
	{
		for (int i = 0; i < 16; i++)printf("%x ", m[i]);
		printf("\nresult: %s\n\n", result);
		for (int i = 0; i < 16; i++)printf("%x ", m1[i]);
		printf("\nresult1: %s\n\n", result1);
		return true;
	}
	return false;
}
void iniM()
{
	m[0] = Rand;
	a[1] = round1(a[0], b[0], c[0], d[0], m[0], 3);
	if (getbit(a[1], 7) != getbit(b[0], 7))modbit(a[1], 7);
	m[0] = rshift(a[1], 3) - a[0] - F(b[0], c[0], d[0]);

	m[1] = Rand;
	d[1] = round1(d[0], a[1], b[0], c[0], m[1], 7);
	if (getbit(d[1], 7) != 0) modbit(d[1], 7);
	if (getbit(d[1], 8) != getbit(a[1], 8)) modbit(d[1], 8);
	if (getbit(d[1], 11) != getbit(a[1], 11))modbit(d[1], 11);
	m[1] = rshift(d[1], 7) - d[0] - F(a[1], b[0], c[0]);

	m[2] = Rand; 
	c[1] = round1(c[0], d[1], a[1], b[0], m[2], 11);
	if (getbit(c[1], 7) != 1)modbit(c[1], 7);
	if (getbit(c[1], 8) != 1)modbit(c[1], 8);
	if (getbit(c[1], 11) != 0)modbit(c[1], 11);
	if (getbit(c[1], 26) != getbit(d[1], 26))modbit(c[1], 26);
	m[2] = rshift(c[1], 11) - c[0] - F(d[1], a[1], b[0]);

	m[3] = Rand;
	b[1] = round1(b[0], c[1], d[1], a[1], m[3], 19);
	if (getbit(b[1], 7) != 1)modbit(b[1], 7);
	if (getbit(b[1], 8) != 0)modbit(b[1], 8);
	if (getbit(b[1], 11) != 0)modbit(b[1], 11);
	if (getbit(b[1], 26) != 0)modbit(b[1], 26);
	m[3] = rshift(b[1], 19) - b[0] - F(c[1], d[1], a[1]);

	m[4] = Rand;
	a[2] = round1(a[1], b[1], c[1], d[1], m[4], 3);
	if (getbit(a[2], 8) != 1)modbit(a[2], 8);
	if (getbit(a[2], 11) != 1)modbit(a[2], 11);
	if (getbit(a[2], 26) != 0)modbit(a[2], 26);
	if (getbit(a[2], 14) != getbit(b[1], 14))modbit(a[2], 14);
	m[4] = rshift(a[2], 3) - a[1] - F(b[1], c[1], d[1]);
	
	m[5] = Rand;
	d[2] = round1(d[1], a[2], b[1], c[1], m[5], 7);
	if (getbit(d[2], 14) != 0)modbit(d[2], 14);
	if (getbit(d[2], 26) != 1)modbit(d[2], 26);
	if (getbit(d[2], 19) != getbit(a[2], 19))modbit(d[2], 19);
	if (getbit(d[2], 20) != getbit(a[2], 20))modbit(d[2], 20);
	if (getbit(d[2], 21) != getbit(a[2], 21))modbit(d[2], 21);
	if (getbit(d[2], 22) != getbit(a[2], 22))modbit(d[2], 22);
	m[5] = rshift(d[2], 7) - d[1] - F(a[2], b[1], c[1]);

	m[6] = Rand;
	c[2] = round1(c[1], d[2], a[2], b[1], m[6], 11);
	if (getbit(c[2], 14) != 0)modbit(c[2], 14);
	if (getbit(c[2], 19) != 0)modbit(c[2], 19);
	if (getbit(c[2], 20) != 0)modbit(c[2], 20);
	if (getbit(c[2], 21) != 1)modbit(c[2], 21);
	if (getbit(c[2], 22) != 0)modbit(c[2], 22);
	if (getbit(c[2], 13) != getbit(d[2], 13))modbit(c[2], 13);
	if (getbit(c[2], 15) != getbit(d[2], 15))modbit(c[2], 15);
	m[6] = rshift(c[2], 11) - c[1] - F(d[2], a[2], b[1]);

	m[7] = Rand;
	b[2] = round1(b[1], c[2], d[2], a[2], m[7], 19);
	if (getbit(b[2], 13) != 1)modbit(b[2], 13);
	if (getbit(b[2], 14) != 1)modbit(b[2], 14);
	if (getbit(b[2], 15) != 0)modbit(b[2], 15);
	if (getbit(b[2], 19) != 0)modbit(b[2], 19);
	if (getbit(b[2], 20) != 0)modbit(b[2], 20);
	if (getbit(b[2], 21) != 0)modbit(b[2], 21);
	if (getbit(b[2], 22) != 0)modbit(b[2], 22);
	if (getbit(b[2], 17) != getbit(c[2], 17))modbit(b[2], 17);
	m[7] = rshift(b[2], 19) - b[1] - F(c[2], d[2], a[2]);

	m[8] = Rand;
	a[3] = round1(a[2], b[2], c[2], d[2], m[8], 3);
	if (getbit(a[3], 13) != 1)modbit(a[3], 13);
	if (getbit(a[3], 14) != 1)modbit(a[3], 14);
	if (getbit(a[3], 15) != 1)modbit(a[3], 15);
	if (getbit(a[3], 17) != 0)modbit(a[3], 17);
	if (getbit(a[3], 19) != 0)modbit(a[3], 19);
	if (getbit(a[3], 20) != 0)modbit(a[3], 20);
	if (getbit(a[3], 21) != 0)modbit(a[3], 21);
	if (getbit(a[3], 22) != 1)modbit(a[3], 22);
	if (getbit(a[3], 23) != getbit(b[2], 23))modbit(a[3], 23);
	if (getbit(a[3], 26) != getbit(b[2], 26))modbit(a[3], 26);
	m[8] = rshift(a[3], 3) - a[2] - F(b[2], c[2], d[2]);

	m[9] = Rand;
	d[3] = round1(d[2], a[3], b[2], c[2], m[9], 7);
	if (getbit(d[3], 13) != 1)modbit(d[3], 13);
	if (getbit(d[3], 14) != 1)modbit(d[3], 14);
	if (getbit(d[3], 15) != 1)modbit(d[3], 15);
	if (getbit(d[3], 17) != 0)modbit(d[3], 17);
	if (getbit(d[3], 20) != 0)modbit(d[3], 20);
	if (getbit(d[3], 21) != 1)modbit(d[3], 21);
	if (getbit(d[3], 22) != 1)modbit(d[3], 22);
	if (getbit(d[3], 23) != 0)modbit(d[3], 23);
	if (getbit(d[3], 26) != 1)modbit(d[3], 26);
	if (getbit(d[3], 30) != getbit(a[3], 30))modbit(d[3], 30);
	m[9] = rshift(d[3], 7) - d[2] - F(a[3], b[2], c[2]);

	m[10] = Rand;
	c[3] = round1(c[2], d[3], a[3], b[2], m[10], 11);
	if (getbit(c[3], 17) != 1)modbit(c[3], 17);
	if (getbit(c[3], 20) != 0)modbit(c[3], 20);
	if (getbit(c[3], 21) != 0)modbit(c[3], 21);
	if (getbit(c[3], 22) != 0)modbit(c[3], 22);
	if (getbit(c[3], 23) != 0)modbit(c[3], 23);
	if (getbit(c[3], 26) != 0)modbit(c[3], 26);
	if (getbit(c[3], 30) != 1)modbit(c[3], 30);
	if (getbit(c[3], 32) != getbit(d[3], 32))modbit(c[3], 32);
	m[10] = rshift(c[3], 11) - c[2] - F(d[3], a[3], b[2]);

	m[11] = Rand;
	b[3] = round1(b[2], c[3], d[3], a[3], m[11], 19);
	if (getbit(b[3], 20) != 0)modbit(b[3], 20);
	if (getbit(b[3], 21) != 1)modbit(b[3], 21);
	if (getbit(b[3], 22) != 1)modbit(b[3], 22);
	if (getbit(b[3], 26) != 1)modbit(b[3], 26);
	if (getbit(b[3], 30) != 0)modbit(b[3], 30);
	if (getbit(b[3], 32) != 0)modbit(b[3], 32);
	if (getbit(b[3], 23) != getbit(c[3], 23))modbit(b[3], 23);
	m[11] = rshift(b[3], 19) - b[2] - F(c[3], d[3], a[3]);

	m[12] = Rand; 
	a[4] = round1(a[3], b[3], c[3], d[3], m[12], 3);
	if (getbit(a[4], 23) != 0)modbit(a[4], 23);
	if (getbit(a[4], 26) != 0)modbit(a[4], 26);
	if (getbit(a[4], 30) != 1)modbit(a[4], 30);
	if (getbit(a[4], 32) != 0)modbit(a[4], 32);
	if (getbit(a[4], 27) != getbit(b[3], 27))modbit(a[4], 27);
	if (getbit(a[4], 29) != getbit(b[3], 29))modbit(a[4], 29);
	m[12] = rshift(a[4], 3) - a[3] - F(b[3], c[3], d[3]);

	m[13] = Rand;
	d[4] = round1(d[3], a[4], b[3], c[3], m[13], 7);
	if (getbit(d[4], 23) != 0)modbit(d[4], 23);
	if (getbit(d[4], 26) != 0)modbit(d[4], 26);
	if (getbit(d[4], 27) != 1)modbit(d[4], 27);
	if (getbit(d[4], 29) != 1)modbit(d[4], 29);
	if (getbit(d[4], 30) != 0)modbit(d[4], 30);
	if (getbit(d[4], 32) != 1)modbit(d[4], 32);
	m[13] = rshift(d[4], 7) - d[3] - F(a[4], b[3], c[3]);

	m[14] = Rand;
	c[4] = round1(c[3], d[4], a[4], b[3], m[14], 11);
	if (getbit(c[4], 23) != 1)modbit(c[4], 23);
	if (getbit(c[4], 26) != 1)modbit(c[4], 26);
	if (getbit(c[4], 27) != 0)modbit(c[4], 27);
	if (getbit(c[4], 29) != 0)modbit(c[4], 29);
	if (getbit(c[4], 30) != 0)modbit(c[4], 30);
	if (getbit(c[4], 19) != getbit(d[4], 19))modbit(c[4], 19);
	m[14] = rshift(c[4], 11) - c[3] - F(d[4], a[4], b[3]);

	m[15] = Rand;
	b[4] = round1(b[3], c[4], d[4], a[4], m[15], 19);
	if (getbit(b[4], 19) != 0)modbit(b[4], 19);
	if (getbit(b[4], 26) != 1)modbit(b[4], 26);
	if (getbit(b[4], 27) != 1)modbit(b[4], 27);
	if (getbit(b[4], 29) != 1)modbit(b[4], 29);
	if (getbit(b[4], 30) != 0)modbit(b[4], 30);

	if (getbit(b[4], 32) != getbit(c[4], 32))modbit(b[4], 32);
	m[15] = rshift(b[4], 19) - b[3] - F(c[4], d[4], a[4]);

	{
		//多步消息修改之a[5]
		a[5] = round2(a[4], b[4], c[4], d[4], m[0], 3);
		if (getbit(a[5], 19) != getbit(c[4], 19)) {
			if (getbit(m[0], 16) == 1)
				m[0] = m[0] - (1 << 15);
			else
				m[0] = m[0] + (1 << 15);
			a[1] = round1(a[0], b[0], c[0], d[0], m[0], 3);
			m[1] = rshift(d[1], 7) - d[0] - F(a[1], b[0], c[0]);
			m[2] = rshift(c[1], 11) - c[0] - F(d[1], a[1], b[0]);
			m[3] = rshift(b[1], 19) - b[0] - F(c[1], d[1], a[1]);
			m[4] = rshift(a[2], 3) - a[1] - F(b[1], c[1], d[1]);
		}

		if (getbit(a[5], 26) != 1) {
			if (getbit(m[0], 23) == 1)
				m[0] = m[0] - (1 << 22);
			else
				m[0] = m[0] + (1 << 22);
			a[1] = round1(a[0], b[0], c[0], d[0], m[0], 3);
			m[1] = rshift(d[1], 7) - d[0] - F(a[1], b[0], c[0]);
			m[2] = rshift(c[1], 11) - c[0] - F(d[1], a[1], b[0]);
			m[3] = rshift(b[1], 19) - b[0] - F(c[1], d[1], a[1]);
			m[4] = rshift(a[2], 3) - a[1] - F(b[1], c[1], d[1]);
		}

		if (getbit(a[5], 27) != 0) {
			if (getbit(m[0], 24) == 1)
				m[0] = m[0] - (1 << 23);
			else
				m[0] = m[0] + (1 << 23);
			a[1] = round1(a[0], b[0], c[0], d[0], m[0], 3);
			m[1] = rshift(d[1], 7) - d[0] - F(a[1], b[0], c[0]);
			m[2] = rshift(c[1], 11) - c[0] - F(d[1], a[1], b[0]);
			m[3] = rshift(b[1], 19) - b[0] - F(c[1], d[1], a[1]);
			m[4] = rshift(a[2], 3) - a[1] - F(b[1], c[1], d[1]);
		}

		if (getbit(a[5], 29) != 1) {
			if (getbit(m[0], 26) == 1)
				m[0] = m[0] - (1 << 25);
			else
				m[0] = m[0] + (1 << 25);
			a[1] = round1(a[0], b[0], c[0], d[0], m[0], 3);
			m[1] = rshift(d[1], 7) - d[0] - F(a[1], b[0], c[0]);
			m[2] = rshift(c[1], 11) - c[0] - F(d[1], a[1], b[0]);
			m[3] = rshift(b[1], 19) - b[0] - F(c[1], d[1], a[1]);
			m[4] = rshift(a[2], 3) - a[1] - F(b[1], c[1], d[1]);
		}

		if (getbit(a[5], 32) != 1) {
			if (getbit(m[0], 29) == 1)
				m[0] = m[0] - (1 << 28);
			else
				m[0] = m[0] + (1 << 28);
			a[1] = round1(a[0], b[0], c[0], d[0], m[0], 3);
			m[1] = rshift(d[1], 7) - d[0] - F(a[1], b[0], c[0]);
			m[2] = rshift(c[1], 11) - c[0] - F(d[1], a[1], b[0]);
			m[3] = rshift(b[1], 19) - b[0] - F(c[1], d[1], a[1]);
			m[4] = rshift(a[2], 3) - a[1] - F(b[1], c[1], d[1]);
		}

	}
}
int main()
{
	printf("Finding Md4 Collision......\n");
	int Seed = (uint)time(NULL);
	Seed =1524720550 ;
	srand(Seed);
	printf("Seed:%x\n", Seed);
	a[0] = aa; b[0] = bb; c[0] = cc; d[0] = dd;
	int times = 0;
	int hit = 0;
	saved = false;
	DWORD t1, t2;
	t1 = GetTickCount();
	while (true)
	{
		times++;
		iniM();
		if (coRight())
		{
			t2 = GetTickCount();
			printf("Success!\n Use time:%.3fs\n Try %d times\n", (t2 - t1)*1.0 / 1000,times);
			Beep(2750, 2000);
		}
		t2 = GetTickCount();
		if (times%1000000 == 0)
			printf("Try %d times\nTime:%.3fs\n", times, (t2 - t1)*1.0 / 1000);
	}
	system("pause");
}
