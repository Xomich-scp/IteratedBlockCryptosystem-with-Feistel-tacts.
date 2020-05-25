#include <vector>
#include <iostream>

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
				if (Sbox[x ^ alpha] == (Sbox[x] ^ beta)) count++;

			table[alpha][beta] = count;
			//table[alpha][beta] /= 16.0;
		}
	}
	for (int i = 0; i < 16; i++) {
		for (int j = 0; j < 16; j++)
			cout << table[i][j] << ' ';
		cout << endl;
	}

	struct diff
	{
		int alpha;
		int beta;
	};
	vector<vector<diff>> weights(7, vector<diff>());
	double probable_max[7] = { 0.0 };
	for (int weight = 2; weight < 9; weight++)
	{
		for (int alpha = 0; alpha < 16; alpha++)
			for (int beta = 0; beta < 16; beta++)
				if (GetBits(alpha) + GetBits(beta) == weight)
					if (probable_max[weight - 2] < table[alpha][beta])
						probable_max[weight - 2] = table[alpha][beta];
	}
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

	return 0;
}
