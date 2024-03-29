/*
*	DKU Operating System Lab
*		Lab1(Scheduler Algorithm Simulator)
*		Student id : 32194747
*		Student name : Choi Jiyun
*
*	lab1_sched.c :
*		- Lab1 source file.
*		- Must contains scueduler algorithm function'definition.
*
*/

#include <aio.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <time.h>
#include <sys/time.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <assert.h>
#include <pthread.h>
#include <asm/unistd.h>

#include "lab1_sched_types.h"

/*
 * you need to implement FCFS, RR, SPN, SRT, HRRN, MLFQ scheduler.
 */

void process1() // 첫번째 프로세스 케이스
{
    Task[0].name = 'A';
    Task[0].arrival = 0;
    Task[0].service = 3;
    Task[0].runtime[0] = 0;
    Task[0].runtime[1] = 0;
    Task[0].runtime[2] = 0;
    Task[0].r_run = 0;

    Task[1].name = 'B';
    Task[1].arrival = 2;
    Task[1].service = 6;
    Task[1].runtime[0] = 0;
    Task[1].runtime[1] = 0;
    Task[1].runtime[2] = 0;
    Task[1].r_run = 0;

    Task[2].name = 'C';
    Task[2].arrival = 4;
    Task[2].service = 4;
    Task[2].runtime[0] = 0;
    Task[2].runtime[1] = 0;
    Task[2].runtime[2] = 0;
    Task[2].r_run = 0;

    Task[3].name = 'D';
    Task[3].arrival = 6;
    Task[3].service = 5;
    Task[3].runtime[0] = 0;
    Task[3].runtime[1] = 0;
    Task[3].runtime[2] = 0;
    Task[3].r_run = 0;

    Task[4].name = 'E';
    Task[4].arrival = 8;
    Task[4].service = 2;
    Task[4].runtime[0] = 0;
    Task[4].runtime[1] = 0;
    Task[4].runtime[2] = 0;
    Task[4].r_run = 0;

    char Print[200] = { '\0' };
}
void process2() //두번째 프로세스 
{
    Task[0].name = 'A';
    Task[0].arrival = 0;
    Task[0].service = 2;
    Task[0].runtime[0] = 0;
    Task[0].runtime[1] = 0;
    Task[0].runtime[2] = 0;
    Task[0].r_run = 0;

    Task[1].name = 'B';
    Task[1].arrival = 3;
    Task[1].service = 7;
    Task[1].runtime[0] = 0;
    Task[1].runtime[1] = 0;
    Task[1].runtime[2] = 0;
    Task[1].r_run = 0;

    Task[2].name = 'C';
    Task[2].arrival = 7;
    Task[2].service = 10;
    Task[2].runtime[0] = 0;
    Task[2].runtime[1] = 0;
    Task[2].runtime[2] = 0;
    Task[2].r_run = 0;

    Task[3].name = 'D';
    Task[3].arrival = 13;
    Task[3].service = 5;
    Task[3].runtime[0] = 0;
    Task[3].runtime[1] = 0;
    Task[3].runtime[2] = 0;
    Task[3].r_run = 0;

    Task[4].name = 'E';
    Task[4].arrival = 17;
    Task[4].service = 2;
    Task[4].runtime[0] = 0;
    Task[4].runtime[1] = 0;
    Task[4].runtime[2] = 0;
    Task[4].r_run = 0;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int power(int x, int y)
{
    int result = 1;
    while (y)
    {
        if (y & 1)
        {
            result *= x;
        }
        y >>= 1;
        x *= x;
    }

    return result;

} //지수승을 구형 하기 위한 함수 

void InitQueue(Queue* queue)
{
    queue->front = queue->rear = NULL;  //front와 rear를 NULL로 설정
    queue->count = 0;                   //보관 개수를 0으로 설정
} //queue를 

int IsEmpty(Queue* queue)
{
    return queue->count == 0;          //보관 개수가 0이면 빈 상태
}

void Enqueue(Queue* queue, process* data)
{
    Node* now = (Node*)malloc(sizeof(Node));    //노드 생성
    now->data = data;                           //데이터 설정
    now->link = NULL;

    if (IsEmpty(queue))                         //큐가 비어있을 때
    {
        queue->front = now;                     //맨 앞을 now로 설정       
    }
    else                                        //비어있지 않을 때
    {
        queue->rear->link = now;                //맨 뒤의 다음을 now로 설정
    }
    queue->rear = now;                          //맨 뒤를 now로 설정   
    queue->count++;                             //보관 개수를 1 증가
}

Node* Dequeue(Queue* queue)
{
    Node* re = 0;
    Node* now;
    if (IsEmpty(queue))         //큐가 비었을 때
    {
        return re;
    }
    now = queue->front;         // front 앞의 노드를 now에 기억
    re = now;                   //반환할 값은 now의 data로 설정
    queue->front = now->link;   //맨 앞은 now의 다음 노드로 설정
    free(now);                  //now 소멸
    queue->count--;             //보관 개수를 1 감소
    return re;
}

void Show() {
    char p_name[] = { 'A', 'B', 'C', 'D', 'E' };
    for (int j = 0; j < 5; j++) {
        int i = 0;
        printf("%c ", p_name[j]);
        while (Print[i] != '\0') {
            if (Print[i] == p_name[j]) {
                printf("■ ");
                i++;
            }
            else {
                if (Print[i] != p_name[j]) {
                    printf("□ ");
                    i++;
                }
                continue;
            }
        }
        printf("\n");
    }
   
} // 구현한 스케쥴링에 대한 그림을 그려주는 함수 

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void FIFO(process a, process b, process c, process d, process e) {
    int time = a.service + b.service + c.service + d.service + e.service;
    int print_count = 0;
    int time_count = 0;

    while (time != 0) {
        int pcs;
        for (int i = 0; i < 5; i++) { // 서비스를 시작하지 않은 프로세스 하나 선택
            if (Task[i].r_run != Task[i].service) {
                pcs = i;
                break;
            }
        }

        for (int i = 0; i < 5; i++) { // 선택된 프로세스보다 빨리 도착한 프로세스가 있는지 확인 및 있으면 프로세스 변경
            if ((Task[i].arrival < Task[pcs].arrival) && (Task[i].r_run != Task[i].service)) {
                pcs = i;
            }
        }

        for (int i = 0; i < Task[pcs].service; i++) {
            Print[print_count] = Task[pcs].name;
            print_count++;
            time_count++;
            time--;
            Task[pcs].r_run++;
        }

        int turnaround_time = Task[pcs].arrival + Task[pcs].service; // 도착시간과 서비스시간을 더한다

        while (1) { // 앞의 프로세스가 끝나고 다음 프로세스를 실행할때까지 공백이 생기는지 조사 밑 공백이 있으면 빈칸
            int count = 0;
            for (int i = 0; i < 5; i++) {
                if (i == pcs) {
                    continue;
                }
                if (turnaround_time >= Task[i].arrival) { // 도착시간과 서비스시간을 더한 값보다 다른 task의 arrival이 작으면 count++
                    count++;
                }
            }
            if (count > 0) { // 한 개의 task라도 turnaround보다 작은값이 있으면 공백이 생기지 않는다
                break;
            }
            turnaround_time++; // turnaround보다 작은값이 없으므로 공백을 만들고 도착하는 다른 task를 조사하기 위해 turnaround++
            Print[print_count] = 'X';
            print_count++;
        }
    }
    Show();
}


void RR(process a, process b, process c, process d, process e, int q)
{
    int time_Slice = q; //타임 퀀텀
    int time_count = 0; // 진행 카운트
    int time = a.service + b.service + c.service + d.service + e.service;
    Queue RR_Queue; // RR용 큐 생성
    InitQueue(&RR_Queue);

    for (int i = 0; i < 5; i++)
    {
        if (Task[i].arrival == 0)
        {
            Enqueue(&RR_Queue, &Task[i]);
        }
    }//도착시간이 0인 것을 Enqueue

    while (time != 0)
    {
        if (!IsEmpty(&RR_Queue))
        {
            Print[time_count] = RR_Queue.front->data->name;
            RR_Queue.front->data->service--;
            RR_Queue.front->data->runtime[0]++;
            time_count++;
            time--;
        }  // 큐에 프로세스가 있는 경우 print배열에 이름 저장, service time 감소, runtime 증가
        else if (IsEmpty(&RR_Queue))
        {
            Print[time_count] = 'X';
            time_count++;
        }  // 큐가 비었을 경우 print배열에 X저장 및 time_count 증가

        for (int i = 0; i < 5; i++)
        {
            if (time_count == Task[i].arrival)
            {
                Enqueue(&RR_Queue, &Task[i]);
            }
        } // 실행중에 들어오는 프로세스가 있는지 검사 

        if (RR_Queue.front->data->runtime[0] == 0) // 공백이 생기는 특수한 경우에 Print배열에 이름저장, service time 감소, runtime 증가
        {
            Print[time_count] = RR_Queue.front->data->name;
            RR_Queue.front->data->service--;
            RR_Queue.front->data->runtime[0]++;
            time_count++;

            for (int i = 0; i < 5; i++)
            {
                if (time_count == Task[i].arrival)
                {
                    Enqueue(&RR_Queue, &Task[i]);
                }
            }
            time--; // time 감소
        }
        if (RR_Queue.front->data->service != 0 && (RR_Queue.front->data->runtime[0]) % (time_Slice) == 0)
        {
            Enqueue(&RR_Queue, RR_Queue.front->data);
            Dequeue(&RR_Queue);
        }// 서비스 시간이 남았지만 타임퀀텀을 초과한 경우 다시 뒤로 보냄
        else if ((RR_Queue.front->data->service == 0))
        {
            Dequeue(&RR_Queue);
        } //서비스 타임을 초과한 경우 큐에서 뺌
    }

    Show();
}


void SJF(process a, process b, process c, process d, process e) 
{
    int time = a.service + b.service + c.service + d.service + e.service;
    int print_count = 0;
    int time_count = 0;

    while (time != 0) {
        int pcs;
        for (int i = 0; i < 5; i++) { // 서비스를 시작하지 않은 프로세스 하나 선택
            if (Task[i].r_run != Task[i].service) {
                pcs = i;
                break;
            }
        }

        for (int i = 0; i < 5; i++) { // 선택된 프로세스보다 짧은 프로세스가 있는지 확인 및 있으면 프로세스 변경
            if ((Task[i].arrival <= time_count) && (Task[i].service < Task[pcs].service) && (Task[i].r_run != Task[i].service)) {
                pcs = i;
            }
        }

        for (int i = 0; i < Task[pcs].service; i++) {
            Print[print_count] = Task[pcs].name;
            print_count++;
            time_count++;
            time--;
            Task[pcs].r_run++;
        }

        int turnaround_time = Task[pcs].arrival + Task[pcs].service; // 도착시간과 서비스시간을 더한다

        while (1) { // 앞의 프로세스가 끝나고 다음 프로세스를 실행할때까지 공백이 생기는지 조사 밑 공백이 있으면 빈칸
            int count = 0;
            for (int i = 0; i < 5; i++) {
                if (i == pcs) {
                    continue;
                }
                if (turnaround_time >= Task[i].arrival) { // 도착시간과 서비스시간을 더한 값보다 다른 task의 arrival이 작으면 count++
                    count++;
                }
            }
            if (count > 0) { // 한 개의 task라도 turnaround보다 작은값이 있으면 공백이 생기지 않는다
                break;
            }
            turnaround_time++; // turnaround보다 작은값이 없으므로 공백을 만들고 도착하는 다른 task를 조사하기 위해 turnaround++
            Print[print_count] = 'X';
            print_count++;
        }
    }

    Show();
}


void MLFQ(process a, process b, process c, process d, process e, int q) {
    int time_Slice = q;
    int time_count = 0;
    int time = a.service + b.service + c.service + d.service + e.service;

    Queue Priorirty_1;   // 우선순위를 위한 큐 3개 생성
    Queue Priorirty_2;
    Queue Priorirty_3;

    InitQueue(&Priorirty_1);
    InitQueue(&Priorirty_2);
    InitQueue(&Priorirty_3);

    for (int i = 0; i < 5; i++) {
        if (0 == Task[i].arrival)
            Enqueue(&Priorirty_1, &Task[i]);
    }//도착시간이 0인 것을 1번 큐에 삽입

    while (time != 0) {
        ////////////////////////////////////////////1번째 우선순위를 가지는 
        if (!IsEmpty(&Priorirty_1)) {
            for (int i = 0; i < power(time_Slice, 0); i++) {        //타임 슬라이스 만큼 수행
                Print[time_count] = Priorirty_1.front->data->name;  // Print배열에 이름저장
                Priorirty_1.front->data->service--;                 // service 타임 감소
                Priorirty_1.front->data->runtime[0]++;              // 실제 실행시간 증가
            }
            time_count++;
            time--;
        }
        else if (IsEmpty(&Priorirty_1)) {   // 큐가 비었을
            Print[time_count] = 'X';        // Print배열에 이름저장 X
            time_count++;
        }
        for (int i = 0; i < 5; i++) {
            if (time_count == Task[i].arrival)
                Enqueue(&Priorirty_1, &Task[i]);
        } // 실행 중 들어오는 프로세스 검사 후 Enqueue 
        if (Priorirty_1.front->data->runtime[0] == 0) {  // 공백의 경우 print배열에 이름 저장, service 감소, runtime 증가
            for (int i = 0; i < power(time_Slice, 0); i++) {
                Print[time_count] = Priorirty_1.front->data->name;
                Priorirty_1.front->data->service--;
                Priorirty_1.front->data->runtime[0]++;
            }

            time_count++;
            for (int i = 0; i < 5; i++) {
                if (time_count == Task[i].arrival) {
                    Enqueue(&Priorirty_1, &Task[i]);    // 실행 중 들어오는 프로세스 검사 후  Enqueue
                }
            }
            time--;
        }
        if (!IsEmpty(&Priorirty_1) && Priorirty_1.front->data->service != 0 && (Priorirty_1.front->data->runtime[0]) % (int)power(time_Slice, 0) == 0) { // 수행시간은 끝나지 않았지만 타임슬라이스가 끝난 
            if (Priorirty_1.front->link == NULL && IsEmpty(&Priorirty_2) && IsEmpty(&Priorirty_3)) {
                Enqueue(&Priorirty_1, Priorirty_1.front->data);
                Dequeue(&Priorirty_1);
            }  // 큐에 혼자 있는 경우 우선순위1 큐와, 데이터 Enqueue, 후에 우선순위1 큐 Dequeue
            else {
                Enqueue(&Priorirty_2, Priorirty_1.front->data);
                Dequeue(&Priorirty_1);
            } // 아닐 경우 우선순위2큐와 우선순위1의 데이터 Enqueue, 후에 우선순위1 큐 Dequeue
        }
        else if ((Priorirty_1.front->data->service == 0))
            Dequeue(&Priorirty_1);
        ////////////////////////////////////////////////////2번째 
        while (IsEmpty(&Priorirty_1)) {
            if (!IsEmpty(&Priorirty_2)) {
                for (int i = 0; i < power(time_Slice, 1); i++) {
                    Print[time_count] = Priorirty_2.front->data->name;
                    Priorirty_2.front->data->service--;
                    Priorirty_2.front->data->runtime[1]++;

                    time_count++;
                    time--;
                    for (int i = 0; i < 5; i++) {
                        if (time_count == Task[i].arrival)
                            Enqueue(&Priorirty_1, &Task[i]);
                    }
                    if (IsEmpty(&Priorirty_1))
                        break;
                    if (Priorirty_2.front->data->service == 0)
                        break;
                }
            }
            if (!IsEmpty(&Priorirty_2) && Priorirty_2.front->data->service != 0 && ((Priorirty_2.front->data->runtime[1]) % (int)power(time_Slice, 1)) == 0) {
                Enqueue(&Priorirty_3, Priorirty_2.front->data);
                Dequeue(&Priorirty_2);
            }
            else if (!IsEmpty(&Priorirty_2) && (Priorirty_2.front->data->service == 0))
                Dequeue(&Priorirty_2);
            else if (IsEmpty(&Priorirty_2))
                break;
        }
        /////////////////////////////////////////////////// 3번째 
        while (IsEmpty(&Priorirty_1) && IsEmpty(&Priorirty_2)) {
            if (!IsEmpty(&Priorirty_3)) {
                for (int i = 0; i < power(time_Slice, 2); i++) {
                    Print[time_count] = Priorirty_3.front->data->name;
                    Priorirty_3.front->data->service--;
                    Priorirty_3.front->data->runtime[2]++;

                    time_count++;
                    time--;
                    for (int i = 0; i < 5; i++) {
                        if (time_count == Task[i].arrival)
                            Enqueue(&Priorirty_1, &Task[i]);
                    }
                    if (IsEmpty(&Priorirty_1))
                        break;
                    if (Priorirty_3.front->data->service == 0)
                        break;
                }
            }
            if (!IsEmpty(&Priorirty_3) && Priorirty_3.front->data->service != 0 && (Priorirty_3.front->data->runtime[2]) % (int)(power(time_Slice, 2)) == 0) {
                Enqueue(&Priorirty_3, Priorirty_3.front->data);
                Dequeue(&Priorirty_3);
            }
            else if (!IsEmpty(&Priorirty_3) && (Priorirty_3.front->data->service == 0))
                Dequeue(&Priorirty_3);
            if (IsEmpty(&Priorirty_3))
                break;
        }
    }

    Show();

}


void LOTTERY(int min, int max, int A_ticket, int B_ticket)
{
    int random_num[20];
    srand(time(NULL)); // 난수 초기화
    printf("  ");
    for (int i = 0; i < 20; i++) {
        random_num[i] = rand() % (max + 1) + min; // min ~ max 까지의 난수 생성
        printf("%2d ", random_num[i]);
    }
    printf("\n");
    for (int i = 0; i < 2; i++) {
        printf("%c ", 65 + i);
        for (int j = 0; j < 20; j++) {
            if (i == 0 && random_num[j] < A_ticket) printf("■  "); // A ticket 영역
            else if (i == 1 && random_num[j] >= A_ticket) printf("■  "); // B ticket 영역
            else printf("□  ");
        }
        printf("\n");
    }
}
