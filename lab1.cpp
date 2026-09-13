#include <iostream>  
#include <cstdlib>
#include <ctime>

using namespace std;

/*
 * Подсчитывает количество вхождений каждой буквы латинского алфавита в строке.
 *
 * @param text массив символов анализируемой строки.
 * @param n количество символов для анализа.
 * @param counts массив для сохранения подсчитанных частот букв.
 * @return ничего.
 */
void countLetters(char text[], int n, int counts[]);


/*
 * Определяет символ, который встречается в тексте чаще всего.
 *
 * @param counts массив частот появления букв алфавита.
 * @return символ латинского алфавита с наибольшей частотой.
 */
char findMostFrequent(int counts[]);


/*
 * Выводит частоты встречаемости букв в виде гистограммы из звёздочек.
 *
 * @param counts массив частот появления букв алфавита.
 * @return ничего.
 */
void printHistogram(int counts[]);


int main()
{
	const int N = 1000;

	char text[N] = {};
	int counts[26] = {0};

	srand(time(0));
	cout << "Input: ";
	for (int i = 0; i < N; i++) {
		text[i] = 'a' + rand() % 26;
		cout << text[i];
	}
	cout << endl;

	countLetters(text, N, counts);
	printHistogram(counts);
	char most_frequent = findMostFrequent(counts);
	cout << "Most commonly encountered: " << most_frequent << endl;
	

	return 0;
}

void countLetters(char text[], int n, int counts[]) {
	for (int i = 0; i < n; i++) {
		int ind = text[i] - 'a';
		counts[ind]++;
	}
}

char findMostFrequent(int counts[]) {
	int ind_max = 0;
	int loc_max = counts[0];

	for (int i = 1; i < 26; i++) {
		if (counts[i] > loc_max) {
			ind_max = i;
			loc_max = counts[i];
		}
	}

	return char('a' + ind_max);
}

void printHistogram(int counts[]) {
	cout << "Output:" << endl;
	for (int i = 0; i < 26; i++) {
		if (counts[i] > 0) {
			cout << char('a' + i) << ": ";
			for (int j = 0; j < counts[i]; j++) {
				cout << '*';
			}
			cout << endl;
		}
	}
}