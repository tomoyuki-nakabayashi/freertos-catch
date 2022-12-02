# Example test cases with FreeRTOS POSIX porting + Catch2

## build & run

```shell
$ make prepare
$ make run
```

`make prepare` will download FreeRTOS kernel and Catch2 single header file, then build & run it.
You can see the following output.

```shell
$ make run
./build/hello
===============================================================================
All tests passed (9 assertions in 5 test cases
```
