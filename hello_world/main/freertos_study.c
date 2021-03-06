#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "freertos/semphr.h"
#include "freertos/event_groups.h"
#include "freertos/timers.h"
#include "freertos_study.h"

#define RTOS_TASK_STACK_SIZE    2048
#define RTOS_TASK_PRIORITY    1

void *rtos_study_task_handle_A;
void *rtos_study_task_handle_B;

#if defined(QUEUE_TEST) && QUEUE_TEST
#define RTOS_TEST_QUEUE_LEN    0x10
void *rtos_study_queue_handle;
#endif

#if defined(SEM_TEST) && SEM_TEST
void *rtos_study_sem_handle;
#endif

#if defined(MUTEX_TEST) && (MUTEX_TEST)
void *rtos_study_mutex_handle;
#endif

static void rtos_study_task_A(void *arg)
{
    printf("[%s] enter\r\n",__func__);
#if defined(QUEUE_TEST) && QUEUE_TEST
    queue_item item_send;
    rtos_study_queue_handle = xQueueCreate(RTOS_TEST_QUEUE_LEN,sizeof(queue_item));
#endif

#if defined(SEM_TEST) && SEM_TEST
    rtos_study_sem_handle = xSemaphoreCreateCounting(1,0);
#endif

#if defined(MUTEX_TEST) && (MUTEX_TEST)
    rtos_study_mutex_handle = xSemaphoreCreateMutex();
#endif

    while(1){
#if defined(QUEUE_TEST) && QUEUE_TEST
        item_send.type = 0xa;
        printf("\r\n[%s] send item to queue",__func__);
        xQueueSend(rtos_study_queue_handle,&item_send,1000/portTICK_PERIOD_MS);
#endif

#if defined(SEM_TEST) && SEM_TEST
        printf("\r\n[%s] give the sem",__func__);
        xSemaphoreGive(rtos_study_sem_handle);
#endif

#if defined(MUTEX_TEST) && (MUTEX_TEST)
        printf("\r\n[%s] try to take mutex",__func__);
        if(xSemaphoreTake(rtos_study_mutex_handle,1000/portTICK_PERIOD_MS)){
            printf("\r\n[%s] take the mutex success",__func__);
            vTaskDelay(1500/portTICK_PERIOD_MS);
            printf("\r\n[%s] give the mutex",__func__);
            xSemaphoreGive(rtos_study_mutex_handle);
        }
        else{
            printf("\r\n[%s] take the mutex timeout",__func__);
        }
#endif
        // vTaskDelay(2500/portTICK_PERIOD_MS);
    }
}

static void rtos_study_task_B(void *arg)
{
    printf("[%s] enter\r\n",__func__);
#if defined(QUEUE_TEST) && QUEUE_TEST
    queue_item item_recv;
#endif
    while(1){
#if defined(QUEUE_TEST) && QUEUE_TEST
        if(rtos_study_queue_handle){
            if(xQueueReceive(rtos_study_queue_handle,&item_recv,1000/portTICK_PERIOD_MS)){
                printf("\r\n[%s] receive item:0x%x",__func__,item_recv.type);
            }
            else{
                printf("\r\nreceive from queue timeout");
            }
        }
#endif

#if defined(SEM_TEST) && SEM_TEST
        if(rtos_study_sem_handle){
            if(xSemaphoreTake(rtos_study_sem_handle,1000/portTICK_PERIOD_MS)){
                printf("\r\n[%s] take sem success",__func__);
            }
            else{
                printf("\r\n[%s] take sem timeout",__func__);
            }
        }
#endif

#if defined(MUTEX_TEST) && (MUTEX_TEST)
        if(rtos_study_mutex_handle){
            printf("\r\n[%s] try to take mutex",__func__);
            if(xSemaphoreTake(rtos_study_mutex_handle,1000/portTICK_PERIOD_MS)){
                printf("\r\n[%s] take the mutex success",__func__);
                vTaskDelay(500/portTICK_PERIOD_MS);
                printf("\r\n[%s] give the mutex",__func__);
                xSemaphoreGive(rtos_study_mutex_handle);
            }
            else{
                printf("\r\n[%s] take the mutex timeout",__func__);
            }
        }
#endif
        // vTaskDelay(5/portTICK_PERIOD_MS);
    }
}

void *event_group_handle;

static void rtos_event_task_A(void *arg)
{
    printf("\r\n[%s] enter",__func__);
    event_group_handle = xEventGroupCreate();
    while(1){
        vTaskDelay(500/portTICK_PERIOD_MS);
        printf("\r\n[%s] SET event bit 0x80",__func__);
        xEventGroupSetBits(event_group_handle,0x80);
        
        vTaskDelay(500/portTICK_PERIOD_MS);
        printf("\r\n[%s] SET event bit 0x800",__func__);
        xEventGroupSetBits(event_group_handle,0x800);
    }
}

static void rtos_event_task_B(void *arg)
{
    printf("\r\n[%s] enter",__func__);
    while(1){
        if(event_group_handle){
            printf("\r\n[%s] start wait event 0x80",__func__);
            if(xEventGroupWaitBits(event_group_handle,0x80,true,false,1000/portTICK_PERIOD_MS)){
                printf("\r\n[%s]get the event SUCCESS",__func__);
            }
            else{
                printf("\r\n[%s]get event timeout",__func__);
            }
        }
        vTaskDelay(10/portTICK_PERIOD_MS);
    }
}

static void rtos_event_task_C(void *arg)
{
    printf("\r\n[%s] enter",__func__);
    while(1){
        if(event_group_handle){
            printf("\r\n[%s] start wait event 0x880",__func__);
            if(xEventGroupWaitBits(event_group_handle,0x880,true,true,1000/portTICK_PERIOD_MS)){
                printf("\r\n[%s]get the event SUCCESS",__func__);
            }
            else{
                printf("\r\n[%s]get event timeout",__func__);
            }
        }
        vTaskDelay(10/portTICK_PERIOD_MS);
    }
}

void *test_timer_handle;

void test_timer_callback(void *arg)
{
    uint32_t timer_id;
    timer_id = pvTimerGetTimerID(arg);
    printf("\r\n[%s] timer id:%d",__func__,timer_id);
}

static void rtos_timer_task(void *arg)
{
    printf("\r\n[%s] enter",__func__);
    test_timer_handle = xTimerCreate("test timer",1000/portTICK_PERIOD_MS,true,5,test_timer_callback);
    if(test_timer_handle){
        xTimerStart(test_timer_handle,1000/portTICK_PERIOD_MS);
    }
    while(1){
        // printf("\r\ntimer test");
        vTaskDelay(1000/portTICK_PERIOD_MS);
    }
}

void task_create_test(void)
{
#if (defined(QUEUE_TEST) && QUEUE_TEST) || \
    (defined(SEM_TEST) && SEM_TEST) || \
    (defined(MUTEX_TEST) && MUTEX_TEST)
    xTaskCreate(rtos_study_task_A,"study_task_A",RTOS_TASK_STACK_SIZE,NULL,RTOS_TASK_PRIORITY,rtos_study_task_handle_A);  // stack size should more than 2048
    xTaskCreate(rtos_study_task_B,"study_task_B",RTOS_TASK_STACK_SIZE,NULL,RTOS_TASK_PRIORITY,rtos_study_task_handle_B);
#endif

#if defined(EVENT_GROUP_TEST) && EVENT_GROUP_TEST
    xTaskCreate(rtos_event_task_A,"event_task_A",RTOS_TASK_STACK_SIZE,NULL,RTOS_TASK_PRIORITY,NULL);
    xTaskCreate(rtos_event_task_B,"event_task_B",RTOS_TASK_STACK_SIZE,NULL,RTOS_TASK_PRIORITY,NULL);
    xTaskCreate(rtos_event_task_C,"event_task_C",RTOS_TASK_STACK_SIZE,NULL,RTOS_TASK_PRIORITY,NULL);
#endif

#if defined(TIMER_TEST) && TIMER_TEST
    xTaskCreate(rtos_timer_task,"timer_task",RTOS_TASK_STACK_SIZE,NULL,RTOS_TASK_PRIORITY,NULL);
#endif

}
