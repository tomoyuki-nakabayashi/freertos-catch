#define CATCH_CONFIG_RUNNER
#include <catch.hpp>
#include <FreeRTOS.h>
#include <task.h>

struct Context {
    int result;
    int argc;
    char** argv;
};

static void test_main(void *arg)
{
    Context *ctx = (Context*)arg;
    ctx->result = Catch::Session().run(ctx->argc, ctx->argv);
    vTaskEndScheduler();
}

extern "C" int main( int argc, char* argv[] )
{
    Context ctx = Context {
        0,
        argc,
        argv,
    };
    xTaskCreate(&test_main, "test", 32000, &ctx, 5, NULL);
    vTaskStartScheduler();

    return ctx.result;
}
