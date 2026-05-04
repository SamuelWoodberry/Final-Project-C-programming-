#include "game.h"
#include <stdio.h>
#include <stdlib.h>
int main() {
    printf("Starting stress test...\n");
    for(int i=0; i<10000; i++) {
        // We need a way to run the game without blocking on user input.
        // But the game waits for scanf.
        // We can't easily mock this without modifying game.c.
    }
    return 0;
}
