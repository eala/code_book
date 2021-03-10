#include <stdio.h>
#include <assert.h>

#define MACRO_OBJECT 1024
#define MACRO_FUNC(X) (X*X)
#define MACRO_TEST_FUNC(func_) \
    do { int val = (func_); \
         assert(val == (func_)); \
    } while (0)

int testFunc(int k) {
    return k;
}

int main(int argc, char *argv[]) {
    printf("MACRO_OBJECT: %d\n", MACRO_OBJECT);
    printf("MACRO_FUNC(5): %d\n", MACRO_FUNC(5));
    int a = 6;
    printf("int a = 5; MACRO_FUNC(a): %d\n", MACRO_FUNC(a));
    printf("MACRO_TEST_FUNC(testFunc(5)): \n");
    MACRO_TEST_FUNC(testFunc(5));
    int b;
    printf("int a = 5; int b; MACRO_TEST_FUNC(b = testFunc(a)): \n"); 
    MACRO_TEST_FUNC(testFunc(a));

    return 0;
}
