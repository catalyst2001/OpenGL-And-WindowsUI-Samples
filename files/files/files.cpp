/**
 *	Работа с файлами
 *	
 *	fopen - функция для открытия файла (считается устаревшей). Ей на смену пришла fopen_s со своим errno_t :)
 *	fclose - закрыть файл
 *	
 *	Режимы открытия файла
 *	 a - открыть файл для добавления информации в конец
 *	 w - открыть файл для перезаписи (удалить прошлые данные из файла)
 *	 r - открыть файл для чтения
 *	
 *	Нередко пишут вместе с режимом открытия букву t ("wt", "rt" и .тд), что означает что якобы файл открыт для работы с текстом а не с бинарными данными.
 *	Для работы с бинарными данными, пишут букву b ("wb", "rb" и .тд)
 *	Правда лично я отличий не вижу :)
 */

// Работа с текстом
//	fgets - считать строку из файла (считывается пока не достигнет '\n' в файле)
//	sscanf считать количество параметров из строкового буфера
//	fscanf считать параметры напрямик из файла

#include <stdio.h> //Input Output
#include <locale.h> //Language
#include <string.h>
#include <stdlib.h>
#pragma warning(disable:4996) //отключить предупреждение 4996

struct student_t {
	int age;
	int kyrs;
	char name[64];
};

int structsize = sizeof(student_t);

int main()
{
	setlocale(LC_ALL, "ru");

	/////////////////////// ZAPIS BLYAT ////////////////////////////
	FILE *fp = fopen("file.txt", "wb");
	if (!fp) {
		printf("Error open file!\n");
		return -1;
	}

	int num_of_students = 0;
	printf("Введите кол-во студентов: ");
	scanf("%d", &num_of_students);
	student_t *ptr = (student_t *)malloc(sizeof(student_t) * num_of_students);
	for (int i = 0; i < num_of_students; i++)
		ptr[i] = { i, i, "Kirill" };

	fwrite(&num_of_students, sizeof(int), 1, fp);
	fwrite(ptr, sizeof(student_t), num_of_students, fp);
	fclose(fp);
	free(ptr);

	/////////////////////// ЧТЕНИЕ ////////////////////////////
	fp = fopen("file.txt", "rb");
	int num = 0;
	fread(&num, sizeof(int), 1, fp);
	student_t *pstudentsarr = (student_t *)malloc(sizeof(student_t) * num);
	fread(pstudentsarr, sizeof(student_t), num, fp);
	for(int i = 0; i < num; i++)
		printf("age: %d  kyrs: %d  name: %s\n", pstudentsarr[i].age, pstudentsarr[i].kyrs, pstudentsarr[i].name);

	fclose(fp);
	free(pstudentsarr);

	return 0;
}