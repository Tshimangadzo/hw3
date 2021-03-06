#include "mm_alloc.h"
#include <stdio.h>
#include <assert.h>

int stuff;

int main(int argc, char **argv)
{
    int *data;  
    data = (int*) mm_malloc(4);
    printf("_main @ %lx\n", (long unsigned int) main);
    printf("_main stack %lx\n", (long unsigned int) &data);
    printf("static data: %lx\n", (long unsigned int) &stuff);
    printf("Heap: malloc: %lx\n", (long unsigned int) data);

    data[0] = 1;
    mm_free(data);
    printf("Malloc sanity test successful!\n");
    data = (int*) mm_malloc(0);
    mm_free(data);
    printf("Zero test successful!\n");
    data = (int*) mm_malloc(-1);
    mm_free(data);
    printf("negative test successful!\n");
    void* array[10];
    array[0] = (int*)mm_malloc(5);
    array[1] = (int*)mm_malloc(10);
    array[2] = (int*)mm_malloc(4);
    array[3] = (int*)mm_malloc(15);
    array[4] = (int*)mm_malloc(10);
    mm_free(array[3]);
    array[5] = (int*)mm_malloc(10);
    printf("second array %x, fourth array %x\n", array[3], array[5]);
    mm_free(array[4]);
    mm_free(array[0]);
    mm_free(array[1]);
    mm_free(array[2]);
    assert(array[3] == array[5]);
    mm_free(array[5]);
    printf("The first fitting algorithm works correctly!\n");
    array[1] = (int*)mm_malloc(5);

    fprintf(stderr, "first array: %x second array: %x\n", array[0], array[1]);
    assert(array[0] == array[1]);
    mm_free(array[1]);

    printf("The first fitting algorithm works correctly!\n");
    array[1] = (int*)mm_malloc(20);
    fprintf(stderr, "first array: %x second array: %x\n", array[0], array[1]);
    mm_free(array[1]);

    printf("The first fitting algorithm works correctly!\n");
    array[0] = (int*)mm_malloc(5);
    array[1] = (int*)mm_malloc(10);
    array[2] = (int*)mm_malloc(4);
    array[3] = (int*)mm_malloc(15);
    array[4] = (int*)mm_malloc(10);
    mm_free(array[1]); 
    mm_free(array[2]); 

    array[2] = mm_malloc(14);
    assert(array[1] == array[2]);
    array[2] = mm_malloc(14);
    mm_free(array[0]);
    mm_free(array[3]);

    array[0] = mm_malloc(14);
    assert(array[0] == array[3]);
    mm_free(array[0]);
    mm_free(array[2]);
    printf("The fusion function works correctly!\n");
    return 0;
}
