#include <FreeRTOS.h>
#include <task.h>

#include <stdio.h>

static void test_task(void *arg)
{
    (void)arg;
    for (int i = 0; i < 10; i++) {
        printf("hello from task\n");
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
    vTaskEndScheduler();
}

int main(void)
{    
    printf("start\n");
    xTaskCreate(&test_task, "test", 1024, NULL, 2, NULL);
    vTaskStartScheduler();
    printf("finish!\n");

    return 0;
}
