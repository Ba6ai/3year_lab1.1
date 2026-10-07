#include <iostream>
#include <windows.h>

using namespace std;

const int m = 4; // Строки
const int n = 6; // Столбцы

float mtx[m][n];	// Матрица

// Функция потока
DWORD WINAPI CreateRow(LPVOID param)
{
	// Получаем значение параметра
	int* prow = (int*)param;
	int row = *prow; // Берёт значение по адресу

	cout << "Поток получил строку: " << row << endl;

	srand(GetCurrentThreadId()); // Своё начальное значение для каждого потока
	cout << "Идентификатор текущего потока: " << GetCurrentThreadId() << endl;

	// Заполнение одной строки
	for (int j = 0; j < n; j++)
	{
		mtx[row][j] = (float)(rand() % 100);
	}
	cout << "\n";
	return 0;
}

int main()
{
	setlocale(LC_ALL, "ru");

	HANDLE hThread[m];
	DWORD dwThreadID[m];

	// Номер строк
	int row_numbers[m];

	// Создание потока
	for (int i = 0; i < m; i++)
	{
		row_numbers[i] = i;

		hThread[i] = CreateThread(
			NULL,			// Атрибут безопасности по умолчанию
			0,				// Размер стека по умолчанию
			CreateRow,		// Имя функции
			&(row_numbers[i]),	// Указатель на параметры (передаёт адрес ячейки памяти в param)
			0,				// Флаг создания
			&dwThreadID[i]	// Адрес переменной для идентификатора
		);
	}

	// Ожидание завершения всех потоков
	WaitForMultipleObjects(
		m,			// Кол-во потоков
		hThread,	// Указатель на массив указателей потоков
		true,		// Флаг ожидания. Показывает - нужно ли дождаться завершения всех потоков
		INFINITE	// Время ожидания завершения в миллисекундах
	);

	// Вывод матрицы
	for (int i = 0; i < m; i++)
	{
		for (int j = 0; j < n; j++)
		{
			cout << mtx[i][j] << "\t";
		}
		cout << endl;
	}

	// Закрытие потока
	for (int i = 0; i < m; i++)
	{
		CloseHandle(hThread[i]);
	}
	return 0;
}