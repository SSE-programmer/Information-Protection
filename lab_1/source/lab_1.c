#include "lab_1.h"

void quickSort(int **numbers, int left, int right) //& 0x7FFFFFFF
{
	int pivot, pivot1;			// разрешающий элемент
	int l_hold = left;	//левая граница
	int r_hold = right; // правая граница

	pivot = numbers[left][0];
	pivot1 = numbers[left][1];

	while (left < right) // пока границы не сомкнутся
	{
		while ((numbers[right][0] >= pivot) && (left < right))
			right--;	   // сдвигаем правую границу пока элемент [right] больше [pivot]
		if (left != right) // если границы не сомкнулись
		{
			numbers[left][0]  = numbers[right][0]; // перемещаем элемент [right] на место разрешающего
			numbers[left][1]  = numbers[right][1];
			left++;							// сдвигаем левую границу вправо
		}
		while ((numbers[left][0] <= pivot) && (left < right))
			left++;		   // сдвигаем левую границу пока элемент [left] меньше [pivot]
		if (left != right) // если границы не сомкнулись
		{
			numbers[right][0] = numbers[left][0]; // перемещаем элемент [left] на место [right]
			numbers[right][1] = numbers[left][1];
			right--;						// сдвигаем правую границу вправо
		}
	}
	numbers[left][0] = pivot; // ставим разрешающий элемент на место
	numbers[left][1] = pivot1;
	pivot = left;
	left = l_hold;
	right = r_hold;
	if (left < pivot) // Рекурсивно вызываем сортировку для левой и правой части массива
		quickSort(numbers, left, pivot - 1);
	if (right > pivot)
		quickSort(numbers, pivot + 1, right);
}

int isPrime(int p)
{
	if (p <= 1)
		return -1;

	int b = (int)pow(p, 0.5);

	for (int i = 2; i <= b; ++i)
	{
		if ((p % i) == 0)
		{
			return -1;
		}
	}

	return 1;
}

/*Прим. y = a^x mod p; */
int BSGS(int y, int a, int p)
{
	int m = (int)pow(p, 0.5) + 1,
		k = m;

	int x = 0;

	int **U = (int**)malloc(sizeof(int*) * m * 2);

	for (int i = 0; i < m * 2; i++)
	{
		U[i] = (int*)malloc(sizeof(int) * 2);
	}

	for (int i = 0; i < m; i++)
	{
		U[i][0] = y * (int)pow(a, i) % p;
		U[i][1] = i;
		U[m + i][0] = ((int)pow(a, k * (i + 1)) % p);
		U[m + i][1] = i | 0x80000000;
	}

	/*for (int i = 0; i < m * 2; i++)
	{
		printf("\t%d", U[i][0]);
	}
	printf("\n");
	for (int i = 0; i < m * 2; i++)
	{
		printf("\t%d", U[i][1] & 0x7FFFFFFF);
	}*/

	quickSort(U, 0, m * 2 - 1);

	for (int i = 0; i < m * 2 - 1; i++)
	{
		if ((U[i][0] == U[i + 1][0]) && ((U[i][1] & 0x80000000) != (U[i + 1][1] & 0x80000000)))
		{
			int l, h;

			if ((U[i][1] & 0x80000000) == 0)
			{
				l = U[i][1] & 0x7FFFFFFF;
				h = (U[i + 1][1] + 1) & 0x7FFFFFFF;
			}
			else
			{
				l = (U[i + 1][1] + 1) & 0x7FFFFFFF;
				h = U[i][1] & 0x7FFFFFFF; 
			}
			

			x = h * m - l;
		}
	}

	return x;
}

int DHKG()
{
	srand(time(NULL));

	int p, q, g;

	do
	{
		do
		{
			q = rand() % RANGE;
		} while (isPrime(q) < 1);

		p = q * 2 + 1;
	} while (isPrime(p) < 1);

	do
	{
		g = rand() % (p - 1) + 1;
	} while (FME(g, q, p) == 1);

	int Xa = rand() % RANGE / 10,
		Xb = rand() % RANGE / 10;


	int Ya = FME(g, Xa, p),
		Yb = FME(g, Xb, p);

	int Zab = FME(Yb, Xa, p),
		Zba = FME(Ya, Xb, p);

	if (Zab == Zba)
	{
		return Zab;
	}

	printf("Произошла ошибка, т.к. открытый ключ двух абонентов не совпадает;\n");

	return -1;
}

int FME(int x, int y, int n)
{
	if (y == 0)
	{
		return 1;
	}

	int z = FME(x, y / 2, n);

	if (y % 2 == 0)
	{
		return (z * z) % n;
	}
	else
	{
		return (x * z * z) % n;
	}
}

int GEA_GCD(int a, int b, int *x, int *y)
{
	a = abs(a);
	b = abs(b);

	if (a < b)
	{
		a += b;
		b = a - b;
		a -= b;
	}

	int q = 0;

	int *U = calloc(3, sizeof(int)),
		*V = calloc(3, sizeof(int)),
		*T = calloc(3, sizeof(int));

	U[0] = a;
	U[1] = 1;
	U[2] = 0;

	V[0] = b;
	V[1] = 0;
	V[2] = 1;

	while (V[0] != 0)
	{
		q = U[0] / V[0];

		T[0] = U[0] % V[0];
		T[1] = U[1] - V[1] * q;
		T[2] = U[2] - V[2] * q;

		for (int i = 0; i < 3; i++)
		{
			U[i] = V[i];
			V[i] = T[i];
		}
	}

	int GCD = U[0];
	*x = U[1];
	*y = U[2];

	free(U);
	free(V);
	free(T);

	return GCD;
}

int main()
{
	/*	int x, y, n;

	printf("Введите X:\n");
	scanf("%d", &x);
	printf("Введите Y:\n");
	scanf("%d", &y);
	printf("Введите N:\n");
	scanf("%d", &n);

	printf("Результат возведения числа %d в степень %d по модулю %d: %d;\n", x, y, n, QuickModExp(x, y, n));
*/
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	/*	int a, b, x, y;

	printf("Введите A:\n");
	scanf("%d", &a);
	printf("Введите B:\n");
	scanf("%d", &b);

	int GCD = GEA_GCD(a, b, &x, &y);

	printf("Наибольший общий делитель чисел %d и %d: %d;\n", a, b, GCD);
	printf("Неизвестные X и Y из уравнения: %d и %d;\n", x, y);
*/

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	printf("Открытый ключ двух абонентов: %d;\n", DHKG());

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	printf("Шаг младенца, шаг великана: %d;\n", BSGS(9, 2, 23));

	return 0;
}