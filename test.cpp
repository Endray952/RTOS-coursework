/*******************************/
/*           test.c            */
/*******************************/

#include <stdio.h>
#include <stdlib.h>

#include "rtos_api.h"


DeclareTask(Task1, 2);
DeclareTask(Task2, 3);
DeclareTask(Task3, 1);

//DeclareTask(Task6, 1);
//DeclareTask(Task7, 3);
//DeclareTask(Task8, 5);
//DeclareTask(Task9, 7);
//DeclareTask(Task10, 2);
//DeclareTask(Task11, 4);

//DeclareTask(Task12, 4);
//DeclareTask(Task13, 1);
//DeclareTask(Task14, 3);

DeclareResource(Res1, 5);
DeclareResource(Res2, 5);
DeclareResource(Res3, 5);
DeclareResource(Res4, 2);

DeclareSysEvent(1);

int main(void)
{
	printf("Hello!\n");
	char name[] = "Task1";
	StartOS(Task1, Task1prior, name);
	

	ShutdownOS();

	return 0;
}


TASK(Task1)
{
	printf("Start Task1\n");
	char name[] = "Task2";
	ActivateTask(Task2, Task2prior, name);

	printf("End Task1\n");

	TerminateTask();
}

TASK(Task2)
{
	printf("Start Task2\n");
	char name[] = "Task3";
	ActivateTask(Task3, Task3prior, name);

	printf("End Task2\n");

	TerminateTask();
}

TASK(Task3)
{
	printf("Start Task3\n");

	printf("End Task3\n");

	TerminateTask();
}



//TASK(Task6)
//{
//	printf("Start Task6\n");
//	/* name, ������� �������� � GetResource � ReleaseResource,
//	������ ���� ���� � ��� �� ������ (�� ������, � ������ ������),
//	��� ��� � ReleaseResource ���� ��������� �� ������ �� name*/
//	char name1[] = "Res4";
//	GetResource(Res4, name1);
//	ReleaseResource(Res4, name1);
//	//char name3[] = "Res1";
//	//GetResource(Res1, name3);
//
//	////ActivateTask(Task7,Task7prior,"Task7");
//	//char name4[] = "Res1";
//	//ReleaseResource(Res1, name4);
//
//
//
//	printf("Task6\n");
//
//	TerminateTask();
//}

//// 4
//TASK(Task12)
//{
//	printf("Start Task12\n");
//
//	char name[] = "Task13";
//	ActivateTask(Task13, Task13prior, name);
//	WaitSysEvent(1);
//
//	printf("End Task12\n");
//
//	TerminateTask();
//};
//
//TASK(Task13)
//{
//	printf("Start Task13\n");
//
//	char name[] = "Task14";
//	ActivateTask(Task14, Task14prior, name);
//
//	printf("End Task13\n");
//
//	TerminateTask();
//};
//
//// 3
//TASK(Task14)
//{
//	printf("Start Task14\n");
//
//	SetSysEvent(1);
//
//	printf("End Task14\n");
//
//	TerminateTask();
//};