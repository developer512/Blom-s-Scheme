#include <iostream>
#include <stdlib.h>
#include <ctime>
using namespace std;

short **matrixGen (short &m, short &p)
{
	short **matrix;
	matrix = new short* [m + 1];
	for (short i = 0; i <= m; i++)
		matrix[i] = new short [m + 1];

	srand (time (NULL));

	for (short i = 0; i <= m; i++)
		for (short j = i; j <= m; j++)
		{
			matrix[i][j] = rand() % p;
			matrix[j][i] = matrix[i][j];
		}
	
	return matrix;
}

void printMatrix (short **matrix, short &m)
{
	for (short i = 0; i <= m; i++)
	{
		for (short j = 0; j <= m; j++)
			cout << matrix[i][j] << "  ";
		cout << endl;
	}
}

short *cryptoNumToVector (short r, short &m, short &p)
{
	short *vector = new short [m + 1];

	for (short i = 0; i <= m; i++)
	{
		vector[i] = pow ((double) r, (double) i);
		vector[i] %= p;
	}

	return vector;
}

void printVector (short *vector, short size)
{
	for (short i = 0; i < size; i++)
		cout << vector[i] << "  ";
	cout << endl;
}

short *keyPolinomialGen (short &cryptoNum, short **matrix, short &m, short &p)
{
	short *rVector;
	rVector = cryptoNumToVector (cryptoNum, m, p);

	short *keyPolinomial = new short [m + 1];
	for (short i = 0; i <= m; i++)
		keyPolinomial[i] = 0;

	for (short j = 0; j <= m; j++)
	{
		for (short i = 0; i <= m; i++)
			keyPolinomial[j] += matrix[i][j] * rVector[i];
		keyPolinomial[j] %= p;
	}

	return keyPolinomial;
}

short communicationKeyCalc (short ** matrix, short *keyPolinomial, short ownCryptoNum, short targetCryptoNum, short &m, short &p)
{
	short *rVector1 = cryptoNumToVector (targetCryptoNum, m, p);

	short communicationKey = 0;

	for (short i = 0; i <= m; i++)
		communicationKey += keyPolinomial[i] * rVector1[i];
	communicationKey %= p;

	delete [] rVector1;
	return communicationKey;
}

int main () {
	while(true) {
		short m;
		short n;	
		short p;
		
		cout << "Enter m parameter" << endl;
		cin >> m;	
		cout << "Enter prime modulus p" << endl;
		cin >> p;
		
		do {
		cout << "Enter number of users" << endl;
		cin >> n;
		if (n >= p - 1)
			cout << "Excessively many users" << endl;
		} while (n >= p - 1);
		
		short **Matrix = matrixGen (m, p);
		short *R = new short [n];
		short **G = new short* [n];

		for (short i = 0; i < n; i++)
		{
			R[i] = i + 1;
			G[i] = keyPolinomialGen (R[i], Matrix, m, p);
		}
		
		cout << "Secrete matrix:" << endl;
		printMatrix (Matrix, m);
		cout << "---------------" << endl;
		
		cout << "Users' key vectors:" << endl;
		for (short i = 0; i < n; i++)
		{
			cout << "r" << i + 1 << " = " << R[i] << ":  ";
			printVector (G[i], m + 1);
		}

		for (short i = 0; i <= m; i++)
			delete [] Matrix[i];
		delete [] Matrix;
		for (short i = 0; i < n; i++)
			delete [] G[i];
		delete [] G;
		delete [] R;

		cout << "________________" << endl << "________________" << endl;
	}
		return 0;
}