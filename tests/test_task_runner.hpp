#include <FreeRTOS.h>
#include <task.h>

struct ITestRunner {
    virtual void run() = 0;
};

static void task_handler(void* arg)
{
    auto runner = reinterpret_cast<ITestRunner*>(arg);
    runner->run();
    vTaskDelete(NULL);
}

template<typename TTestBody, size_t PRIORITY=5, size_t STACK=32000>
struct FreertosTestTaskRunner : public ITestRunner {
    TaskHandle_t parent;
    TaskHandle_t runner;
    TTestBody testbody;

    FreertosTestTaskRunner(TTestBody&& testbody) : testbody(testbody) {
        this->parent = xTaskGetCurrentTaskHandle();
        this->runner = NULL;
    }

    void run() override {
        this->testbody();
        xTaskNotify(this->parent, 1, eSetBits);
    }

    bool execute(int timeout_ms) {
        if ( xTaskCreate(&task_handler, "test", STACK, this, PRIORITY, &this->runner) != pdPASS ) {
            return false;
        }
        if( xTaskNotifyWait(0, 1, NULL, pdMS_TO_TICKS(timeout_ms)) != 1 ) {
            vTaskDelete(this->runner);
            return false;
        }
        return true;
    }
};

template<size_t P=5, size_t S=32000, typename TTestBody>
static FreertosTestTaskRunner<TTestBody, P, S> make_test_case(TTestBody&& body) {
    return FreertosTestTaskRunner<TTestBody, P, S>(std::forward<TTestBody>(body));
}
