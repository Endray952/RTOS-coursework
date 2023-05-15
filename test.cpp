/*******************************/
/*           test.c            */
/*******************************/

#include <stdio.h>
#include <stdlib.h>

#include "rtos_api.h"

// Тест диспетчера
//DeclareTask(Task1, 2);
//DeclareTask(Task2, 3);
//DeclareTask(Task3, 1);


// Тест ресурсов
//DeclareTask(Task4, 1);
//DeclareTask(Task5, 3);
//
//DeclareResource(Res1, 4);


// Тест событий
DeclareTask(Task6, 3);
DeclareTask(Task7, 1);
DeclareTask(Task8, 2);

DeclareSysEvent(1);


int main(void)
{
	printf("Hello!\n");
	char name[] = "Task6";
	StartOS(Task6, Task6prior, name);
	

	ShutdownOS();

	return 0;
}

// Тест диспетчера
//TASK(Task1)
//{
//	printf("Start Task1\n");
//	char name[] = "Task2";
//	ActivateTask(Task2, Task2prior, name);
//
//	printf("End Task1\n");
//
//	TerminateTask();
//}
//
//TASK(Task2)
//{
//	printf("Start Task2\n");
//	char name[] = "Task3";
//	ActivateTask(Task3, Task3prior, name);
//
//	printf("End Task2\n");
//
//	TerminateTask();
//}
//
//TASK(Task3)
//{
//	printf("Start Task3\n");
//
//	printf("End Task3\n");
//
//	TerminateTask();
//}


// Тест ресурсов
//TASK(Task4)
//{
//    printf("Start Task4\n");
//    char name[] = "Task5";
//    char name1[] = "Res1";
//    GetResource(Res1, name1);
//    ActivateTask(Task5, Task5prior, name);
//    ReleaseResource(Res1, name1);
//    printf("End Task4\n");
//
//    TerminateTask();
//}
//
//TASK(Task5)
//{
//    printf("Start Task5\n");
//    char name1[] = "Res1";
//    GetResource(Res5, name1);
//    ReleaseResource(Res1, name1);
//    printf("End Task5\n");
//
//    TerminateTask();
//}


// Тест событий
TASK(Task6)
{
    printf("Start Task6\n");
	char name[] = "Task7";
	ActivateTask(Task7, Task7prior, name);
	WaitSysEvent(1);
	printf("End Task6\n");

    TerminateTask();
}

TASK(Task7)
{
	printf("Start Task7\n");
	char name[] = "Task8";
	ActivateTask(Task8, Task8prior, name);
	printf("End Task7\n");

	TerminateTask();
}

TASK(Task8)
{
	printf("Start Task8\n");
	SetSysEvent(1);
	printf("End Task8\n");

	TerminateTask();
}
