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


//DeclareTask(Task6, 1);
//DeclareTask(Task7, 3);
//DeclareTask(Task8, 5);
//DeclareTask(Task9, 7);
//DeclareTask(Task10, 2);
//DeclareTask(Task11, 4);

//DeclareTask(Task12, 4);
//DeclareTask(Task13, 1);
//DeclareTask(Task14, 3);

//DeclareResource(Res1, 5);
//DeclareResource(Res2, 5);
//DeclareResource(Res3, 5);
//DeclareResource(Res4, 2);
//
//DeclareSysEvent(1);


// Тест ресурсов
DeclareTask(Task8, 1);
DeclareTask(Task9, 3);

DeclareResource(Res5, 4);

int main(void)
{
	printf("Hello!\n");
	char name[] = "Task8";
	StartOS(Task8, Task8prior, name);
	

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
TASK(Task8)
{
    printf("Start Task8\n");
    char name[] = "Task9";
    char name5[] = "Res5";
    GetResource(Res5, name5);
    ActivateTask(Task9, Task9prior, name);
    ReleaseResource(Res5, name5);
    printf("End Task8\n");

    TerminateTask();
}

TASK(Task9)
{
    printf("Start Task9\n");
    char name5[] = "Res5";
    GetResource(Res5, name5);
    ReleaseResource(Res5, name5);
    printf("End Task9\n");

    TerminateTask();
}
