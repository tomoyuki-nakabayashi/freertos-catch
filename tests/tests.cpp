#include <catch.hpp>
#include <iostream>

#include "test_task_runner.hpp"

TEST_CASE("usual test but executed on FreeRTOS task")
{
    REQUIRE(true);    
}

TEST_CASE("test sub task activity")
{
    auto sub = [](void *arg) {
        int32_t *result = static_cast<int32_t*>(arg);
        *result = 1;
        vTaskDelete(NULL);
    };

    int32_t result = 0;
    xTaskCreate(sub, "sub-task", 4096, &result, 2, NULL);

    // wait a minute to sub-task completion
    vTaskDelay(pdMS_TO_TICKS(10));
    REQUIRE(result == 1);
}

TEST_CASE("oops! forgot to free a buffer", "[.]")
{
    auto sub = [](void *arg) {
        int32_t *result = static_cast<int32_t*>(arg);

        // it's meaningless memory allocation, but enough to show sanitizer function
        int32_t *buf = static_cast<int32_t*>(malloc(sizeof(int32_t)));
        *buf = 1;
        *result = *buf;

        // oops
        // free(buf);
        vTaskDelete(NULL);
    };

    int32_t result = 0;
    xTaskCreate(sub, "sub-task", 4096, &result, 2, NULL);

    // wait a minute to sub-task completion
    vTaskDelay(pdMS_TO_TICKS(10));
    REQUIRE(result == 1);
}

TEST_CASE("with test task runner helper")
{
    int32_t result = 0;
    auto test_case = make_test_case([&]() {
        // this lambda is executed on another FreeRTOS task.
        result = 1;
    });

    // `test_case.execute()` blocks the execution until lambda completion or timeout.
    REQUIRE(test_case.execute(10));
    REQUIRE(result == 1);

    auto timeout_case = make_test_case([&]() {
        vTaskDelay(pdMS_TO_TICKS(100));
    });
    // lambda will not complete the execution in 10[ms]
    REQUIRE_FALSE(timeout_case.execute(10));
}

TEMPLATE_TEST_CASE_SIG("parameterized task configuration test", "", ((size_t P, size_t S), P, S), (2, 1024), (3, 512))
{
    int32_t result = 0;
    auto test_case = make_test_case<P, S>([&]() {
        result = 1;
    });

    REQUIRE(test_case.execute(10));
    REQUIRE(result == 1);
}
