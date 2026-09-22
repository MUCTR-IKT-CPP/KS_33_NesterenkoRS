#include <iostream>
#include <cstdlib>
#include <ctime>

using namespace std;


void generationArray(int** mas, int N);
void printArray(void* mas, int N);
void deleteArray(int** mas, int N);
void runMenu(int** mas, int N);
void mirrorVertically(int** mas, int N);
void mirrorHorizontally(int** mas, int N);
void sumNeighbors(int** mas, int N);

int main()
{
    int N;
    cout << "Enter the array size: ";
    cin >> N;

    int** mas = new int* [N];
    for (int i = 0; i < N; i++) {
        mas[i] = new int[N];
    }

    generationArray(mas, N);
    printArray(mas, N); 

    runMenu(mas, N);

    deleteArray(mas, N);
    return 0;
}

/*
 * Заполнение двумерного массива случайными числами.
 *
 * @param mas указатель на двумерный массив.
 * @param N размер матрицы.
 * @return ничего.
 */
void generationArray(int** mas, int N) {
    srand(time(0));
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            mas[i][j] = rand() % 10;
        }
    }
}

/*
 * Вывод матрицы в терминал.
 *
 * @param ptr бестиповый указатель на матрицу.
 * @param N размер матрицы.
 * @return ничего.
 */
void printArray(void* ptr, int N) {
    int** mas = (int**)ptr;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            cout << mas[i][j] << "\t";
        }
        cout << endl;
    }
}

/*
 * Очистка динамической памяти двумерного массива.
 *
 * @param mas указатель на двумерный массив.
 * @param N размер матрицы.
 * @return ничего.
 */
void deleteArray(int** mas, int N) {
    for (int i = 0; i < N; i++) {
        delete[] mas[i];
    }
    delete[] mas;
}

/*
 * Меню взаимодействия с пользователем.
 *
 * @param mas указатель на двумерный массив.
 * @param N размер матрицы.
 * @return ничего.
 */
void runMenu(int** mas, int N) {
    int choice;
    do {
        cout << "\n--- Menu ---\n";
        cout << "1. Mirror vertically\n";
        cout << "2. Mirror horizontally\n";
        cout << "3. Neighbors sum\n";
        cout << "0. Exit\n";
        cout << "Your choice: ";
        cin >> choice;

        switch (choice) {
        case 1:
            mirrorVertically(mas, N);
            cout << "Vertical reflection done.\n";
            printArray(mas, N);
            break;
        case 2:
            mirrorHorizontally(mas, N);
            cout << "Horizontal reflection done.\n";
            printArray(mas, N);
            break;
        case 3:
            cout << "Neighbors sum calculated.\n";
            sumNeighbors(mas, N);
            break;
        case 0:
            cout << "Exiting...\n";
            break;
        default:
            cout << "Wrong choice. Try again.\n";
            break;
        }
    } while (choice != 0);
}

/*
 * Зеркальное отражение матрицы по вертикали.
 * Меняет местами элементы в строках от краев к центру.
 *
 * @param mas указатель на двумерный массив.
 * @param N размер матрицы.
 * @return ничего.
 */
void mirrorVertically(int** mas, int N) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N / 2; j++) {
            int temp = mas[i][j];
            mas[i][j] = mas[i][N - 1 - j];
            mas[i][N - 1 - j] = temp;
        }
    }
}

/*
 * Зеркальное отражение матрицы по горизонтали.
 * Оптимизированный подход: меняет местами указатели на строки, а не сами элементы.
 *
 * @param mas указатель на двумерный массив.
 * @param N размер матрицы.
 * @return ничего.
 */
void mirrorHorizontally(int** mas, int N) {
    for (int i = 0; i < N / 2; i++) {
        int* temp = mas[i];
        mas[i] = mas[N - 1 - i];
        mas[N - 1 - i] = temp;
    }
}

/*
 * Построение новой матрицы сумм соседних элементов и ее вывод.
 *
 * @param mas исходный указатель на двумерный массив.
 * @param N размер матрицы.
 * @return ничего.
 */
void sumNeighbors(int** mas, int N) {
    int** tempMas = new int* [N];
    for (int i = 0; i < N; i++) {
        tempMas[i] = new int[N];
    }

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            int sum = 0;
            if (i > 0) sum += mas[i - 1][j];
            if (i < N - 1) sum += mas[i + 1][j];
            if (j > 0) sum += mas[i][j - 1];
            if (j < N - 1) sum += mas[i][j + 1];

            tempMas[i][j] = sum;
        }
    }

    printArray(tempMas, N);

    for (int i = 0; i < N; i++) {
        delete[] tempMas[i];
    }
    delete[] tempMas;
}