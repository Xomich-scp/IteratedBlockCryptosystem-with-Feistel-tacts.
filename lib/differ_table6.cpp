#include "lib.h"
#include <vector>
#include <iostream>
#include <fstream>
using namespace std;
using std::vector;
using std::cout;
using std::endl;
int GetBits(int x)
{
	int count = 0;
	while (x != 0)
	{
		if (x & 1) count++;
		x >>= 1;
	}
	return count;
}

void differ_table(int* Sbox)
{

	float table[16][16] = { 0.0 };
	for (int alpha = 0; alpha < 16; alpha++)
	{
		for (int beta = 0; beta < 16; beta++)
		{
			int count = 0;
			for (int x = 0; x < 16; x++)
				if (Sbox[x ^ alpha] == (Sbox[x] ^ beta)) count++; //I событие, суммирование

			table[alpha][beta] = count;
			//table[alpha][beta] /= 16.0;
		}
	}
	ofstream fout;
	fout.open("table_of_diff_1.txt");
	for (int i = 1; i < 16; i++) 
	{
		for (int j = 1; j < 16; j++)
			fout << table[i][j] << ",";
			//printf("%2.0f ", table[i][j]);
		fout << endl;
	}
	fout.close();
	struct diff
	{
		int alpha;
		int beta;
	};
	vector<vector<diff>> weights(7, vector<diff>());
	double probable_max[7] = { 0.0 };
	// table[a][b] - количество событий. вес - количество бит в а и б
	for (int weight = 2; weight < 9; weight++)
	{
		for (int alpha = 0; alpha < 16; alpha++)
			for (int beta = 0; beta < 16; beta++)
				if (GetBits(alpha) + GetBits(beta) == weight) //считаем количество бит, если совпало с текущим весом
					if (probable_max[weight - 2] < table[alpha][beta])
						probable_max[weight - 2] = table[alpha][beta];
	}
	/*
	вероятность = вес из table / 16.
	*/
	for (int weight = 2; weight < 9; weight++)
	{
		for (int alpha = 0; alpha < 16; alpha++)
			for (int beta = 0; beta < 16; beta++)
				if (GetBits(alpha) + GetBits(beta) == weight)
					if (table[alpha][beta] != 0 && table[alpha][beta] == probable_max[weight - 2])
					{
						diff differ;
						differ.alpha = alpha;
						differ.beta = beta;
						weights[weight - 2].push_back(differ);
					}
	}


	fout.open("table_of_diff_2.txt");
	for (int weight = 2; weight < 9; weight++)
	{
		if (weights[weight - 2].size() > 0)
		{
			fout << weight << "," << table[weights[weight - 2].begin()->alpha][weights[weight - 2].begin()->beta] / 16 << ",";
			//printf("%d,%0.3f,", weight,										table[weights[weight - 2].begin()->alpha][weights[weight - 2].begin()->beta] / 16			);
			
			//printf("%d, ", weight);
			//std::vector<diff>::iterator it = weights[weight - 2].begin();
			//printf("a=%d b=%d\t", it->alpha, it->beta);

			for (std::vector<diff>::iterator it = weights[weight - 2].begin(); it != weights[weight - 2].end(); it++)
			{
				fout << "[" << it->alpha << ", " << it->beta << "],";
				//printf("[%2d, %2d],", it->alpha, it->beta);
			}
			fout << endl;
			//printf("\n");
		}
	}
	fout.close();
}
