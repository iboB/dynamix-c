#include <stdio.h>
#include <stdalign.h>

int main() {
    printf("align %lld\n", alignof(long double));
    return 0;
}
