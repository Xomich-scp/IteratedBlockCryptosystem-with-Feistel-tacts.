#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <windows.h>
#include <time.h>
typedef unsigned char u_int1;
typedef unsigned short u_int2;
typedef unsigned int u_int4;
void getKey(u_int2 K[], u_int4 Teta)
{
	u_int2 key1 = Teta & 0b00000000000000001111111111111111;
	u_int4 key2 = (Teta & 0b11111111111111110000000000000000) >> 16;
	K[0] = key1;
	K[1] = key2;
	K[2] = key1;
	K[3] = key2;
}
void SP(u_int2& X, u_int2 K, u_int1 P[], u_int1 S[])
{
	X = X ^ K;
	u_int1 tempX[2];
	memcpy(tempX, &X, 2);
	u_int1 tempX2[4];
	tempX2[2] = (X & 0b1111000000000000) >> 12;
	tempX2[3] = (X & 0b0000111100000000) >> 8;
	tempX2[0] = (X & 0b0000000011110000) >> 4;
	tempX2[1] = (X & 0b0000000000001111) >> 0;
	X = 0;
	for (int i = 0; i < sizeof(tempX2); i++)
	{
		for (int j = 0; j < 16; j++)
		{
			if (tempX2[i] == S[j]) {
				X = X | (S[(j + 1) % 16] << (12 - 4 * i));
				break;
			}
		}
	}
	X = ((0b0000000011111111 & X) << 8) + ((0b1111111100000000 & X) >> 8);
	u_int2 XX = X;
	memcpy(&XX, &X, 2);
	for (int i = 0; i < 16; i++)
	{
		u_int4 value = (XX >> (15 - P[i]) & 1UL);
		if (value > 0)
			X |= 1UL << 15 - P[(i + 1) % 16];
		else
			X &= ~(1UL << 15 - P[(i + 1) % 16]);
	}
	int a;
	a = 5;
}
void crypt(u_int1 input[], u_int4 size, u_int4 Teta, u_int1 rounds)
{
	u_int2 K[4];
	getKey(K, Teta);
	u_int4 key_whitening = ((K[0] ^ K[1]) << 16) + (K[0] ^ K[1]);
	for (int j = 0; j < rounds; j++)
	{
		for (int i = 0; i < size; i += 4)
		{
			u_int2 X1 = 0;
			u_int2 X2 = 0;
			memcpy(&X2, &input[i + 2], 2);
			memcpy(&X1, &input[i], 2);
			u_int2 XX;// = X2;
			memcpy(&XX, &X2, 2);
			u_int1 P[16];
			for (int i = 0; i < 16; i++)
			{
				P[i] = (13 * i + 1) % 16;
			}
			u_int1 S[] = { 14,4,13,1,2,15,11,8,3,10,6,12,5,9,0,7 };
			SP(XX, K[j], P, S);
			u_int4 ret;
			ret = (((X1 ^ XX) << 16) + X2) ^ key_whitening;
			input[i + 3] = (ret & 0b11111111000000000000000000000000) >> 24;
			input[i + 2] = (ret & 0b00000000111111110000000000000000) >> 16;
			input[i + 1] = (ret & 0b00000000000000001111111100000000) >> 8;
			input[i + 0] = (ret & 0b00000000000000000000000011111111) >> 0;
		}
	}
}
void decrypt(u_int1 input[], u_int4 size, u_int4 Teta, u_int1 rounds)
{
	u_int2 K[4];
	getKey(K, Teta);
	u_int4 key_whitening = ((K[0] ^ K[1]) << 16) + (K[0] ^ K[1]);
	for (int j = rounds - 1; j >= 0; j--)
	{
		for (int i = 0; i < size; i += 4)
		{
			u_int4* temp = (u_int4*)(&input[i]);
			u_int4 whitening = key_whitening ^ *temp;
			u_int2 X1 = 0;
			u_int2 X2 = 0;
			X1 = (whitening & 0b11111111111111110000000000000000) >> 16;
			X2 = (whitening & 0b00000000000000001111111111111111) >> 0;
			u_int2 XX;// = X2;
			memcpy(&XX, &X2, 2);
			u_int1 P[16];
			for (int i = 0; i < 16; i++)
			{
				P[i] = (13 * i + 1) % 16;
			}
			u_int1 S[] = { 14,4,13,1,2,15,11,8,3,10,6,12,5,9,0,7 };
			SP(XX, K[j], P, S);
			X1 = X1 ^ XX;
			input[i + 3] = (X2 & 0b1111111100000000) >> 8;
			input[i + 2] = (X2 & 0b0000000011111111) >> 0;
			input[i + 1] = (X1 & 0b1111111100000000) >> 8;
			input[i + 0] = (X1 & 0b0000000011111111) >> 0;
		}
	}
}
DWORD WINAPI halfWeakKey(void* args)
{
	u_int4 in = 1243467123;
	u_int1 input[4];
	u_int4 out;
	int i = (int)args; printf("Start thread #%d\n", i);
	for (u_int4 Teta = i * 1000000000; Teta < (i + 1) * 1000000000; Teta++)
	{
		memcpy(&input, &in, 4);
		crypt(input, 4, Teta, 4);
		crypt(input, 4, Teta, 4);
		memcpy(&out, &input, 4);
		if ((Teta - i * 1000000000) % 100000000 == 0)
		{
			printf("[%d] -> %u\n", i, Teta);
		}
		if (in == out)
		{
			printf("Find! %u\n", Teta); system("pause");
		}
	}
	return NULL;
}

void findWeakKeys()
{
	const int t_count = 4;
	HANDLE tids[t_count];
	time_t timer = time(NULL);
	for (int i = 0; i < t_count; i++)
	{
		tids[i] = CreateThread(0, 0, halfWeakKey, (void*)i, 0, NULL);
		if (0 == tids[i])
		{
			printf("pthread_create failed. errno: %d\n", errno);
			return;
		}
	}
	WaitForMultipleObjects(t_count, tids, TRUE, INFINITE);
	for (int i = 0; i < t_count; i++)
		CloseHandle(tids[i]);
	printf("Time %ld \n", (time(NULL) - timer)); system("pause");
}
u_int4 changed(u_int1 a[], u_int1 b[], u_int4 key)
{
	u_int4 count = 0;
	for (int i = 0; i < 4; i++)
	{
		a[i] = a[i] ^ b[i];
		for (u_int4 j = 0, k = 1; j < sizeof(a[i]) * 8; j++)
		{
			if ((a[i] & k) > 0)
				count++;
			k = k << 1;
		}
	}
	return count;
}
void print(u_int1 text[], u_int4 size, const char* str)
{
	printf("%s: ", str);
	for (int i = 0; i < size; i++) printf("%c", text[i]);
}
int main()
{
	printf("key: ");
	u_int4 key; scanf("%d", &key);
	FILE* f = fopen("input.txt", "rb");
	int size;
	fseek(f, 0L, SEEK_END);
	size = ftell(f);
	fseek(f, 0L, SEEK_SET);
	int pag = size % 4;
	int pag_ = 4 - size % 4;
	u_int1* data;
	data = (u_int1*)malloc(size + pag + 4);
	int i = 0;
	char c;
	while ((c = fgetc(f)) != EOF)
	{
		data[i++] = (char)c;
	}
	print(data, size, "clear");
	while (pag % 4 != 0)
	{
		data[i] = 0x00;
		i++;
		pag++;
	}
	data[i] = pag_;
	data[i + 1] = 0x00;
	data[i + 2] = 0x00;
	data[i + 3] = 0x00;
	size = size + pag_ + 4;
	crypt(data, size, key, 4);
	print(data, size, "\n\nencrypted ");
	f = fopen("output.txt", "wb");
	for (i = 0; i < size; i++) fprintf(f, "%c", data[i]);
	//decrypt(data, size, key, 4);
	//size = size - data[size - 4] - 4;
	//print(data, size, "\n\ndecrypted");
	// findWeakKeys();
	/*u_int4 in = 23423412347;
	u_int1 input[4];
	memcpy(&input, &in, 4);
	u_int4 t_crypted;
	for (int i = 0; i < sizeof(input) * 8; i++)
	{
	printf("%d:\n", i);
	for (int j = 0; j < 4; j++)
	{
	u_int1 crypted_clear[4];
	memcpy(&crypted_clear, &in, 4);
	crypt(crypted_clear, 234235672, j);
	t_crypted = in ^ (1 << i);
	u_int1 crypted_error[4];
	memcpy(&crypted_error, &t_crypted, 4);
	crypt(crypted_error, 234235672, j);
	u_int4 round_one = changed(crypted_clear, crypted_error, 234235672);
	printf("\t%d -> %u\n", j, round_one);
	}
	printf("\n");
	}*/
	printf("\n\n"); system("pause");
	return 0;
}



/*
#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <windows.h>
#include <time.h>
typedef unsigned char u_int1;
typedef unsigned short u_int2;
typedef unsigned int u_int4;
void getKey(u_int2 K[], u_int4 Teta)
{
	u_int2 key1 = Teta & 0b00000000000000001111111111111111;
	u_int4 key2 = (Teta & 0b11111111111111110000000000000000) >> 16;
	K[0] = key1;
	K[1] = key2;
	K[2] = key1;
	K[3] = key2;
}
void SP(u_int2& X, u_int2 K, u_int1 P[], u_int1 S[])
{
	X = X ^ K;
	u_int1 tempX[2];
	memcpy(tempX, &X, 2);
	u_int1 tempX2[4];
	tempX2[2] = (X & 0b1111000000000000) >> 12;
	tempX2[3] = (X & 0b0000111100000000) >> 8;
	tempX2[0] = (X & 0b0000000011110000) >> 4;
	tempX2[1] = (X & 0b0000000000001111) >> 0;
	X = 0;
	for (int i = 0; i < sizeof(tempX2); i++)
	{
		for (int j = 0; j < 16; j++)
		{
			if (tempX2[i] == S[j]) {
				X = X | (S[(j + 1) % 16] << (12 - 4 * i));
				break;
			}
		}
	}
	X = ((0b0000000011111111 & X) << 8) + ((0b1111111100000000 & X) >> 8);
	u_int2 XX = X;
	memcpy(&XX, &X, 2);
	for (int i = 0; i < 16; i++)
	{
		u_int4 value = (XX >> (15 - P[i]) & 1UL);
		if (value > 0)
			X |= 1UL << 15 - P[(i + 1) % 16];
		else
			X &= ~(1UL << 15 - P[(i + 1) % 16]);
	}
	int a;
	a = 5;
}
void decrypt(u_int1 input[], u_int4 size, u_int4 Teta, u_int1 rounds)
{
	u_int2 K[4];
	getKey(K, Teta);
	u_int4 key_whitening = ((K[0] ^ K[1]) << 16) + (K[0] ^ K[1]);
	for (int j = rounds - 1; j >= 0; j--)
	{
		for (int i = 0; i < size; i += 4)
		{
			u_int4* temp = (u_int4*)(&input[i]);
			u_int4 whitening = key_whitening ^ *temp;
			u_int2 X1 = 0;
			u_int2 X2 = 0;
			X1 = (whitening & 0b11111111111111110000000000000000) >> 16;
			X2 = (whitening & 0b00000000000000001111111111111111) >> 0;
			u_int2 XX;// = X2;
			memcpy(&XX, &X2, 2);
			u_int1 P[16];
			for (int i = 0; i < 16; i++)
			{
				P[i] = (13 * i + 1) % 16;
			}
			u_int1 S[] = { 14,4,13,1,2,15,11,8,3,10,6,12,5,9,0,7 };
			SP(XX, K[j], P, S);
			X1 = X1 ^ XX;
			input[i + 3] = (X2 & 0b1111111100000000) >> 8;
			input[i + 2] = (X2 & 0b0000000011111111) >> 0;
			input[i + 1] = (X1 & 0b1111111100000000) >> 8;
			input[i + 0] = (X1 & 0b0000000011111111) >> 0;
		}
	}
}
void print(u_int1 text[], u_int4 size, const char* str)
{
	printf("%s: ", str);
	for (int i = 0; i < size; i++) printf("%c", text[i]);
}
int main()
{
	printf("key: ");
	u_int4 key; scanf("%d", &key);
	FILE* f = fopen("../../Crypta2/Crypta2/output.txt", "rb");
	int size;
	fseek(f, 0L, SEEK_END);
	size = ftell(f);
	fseek(f, 0L, SEEK_SET);
	u_int1* data;
	data = (u_int1*)malloc(size);
	int i = 0;
	char c;
	while ((c = fgetc(f)) != EOF)
	{
		data[i++] = (char)c;
	}
	decrypt(data, size, key, 4);
	size = size - data[size - 4] - 4; print(data, size, "decrypted");
	printf("\n\n"); system("pause");
	return 0;
	*/