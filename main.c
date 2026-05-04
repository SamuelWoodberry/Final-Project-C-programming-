/*
 * ECE 101 Final Project: Modified Go Fish
 * By: [Your Name]
 * 
 * This is the main entry point for the game. I kept it super simple 
 * and moved all the heavy lifting into game.c to keep the code modular,
 * just like the professor suggested in the assignment!
 */
#include "game.h"

int main(void) {
    // Start the game loop. Everything happens inside here.
    run_game();
    
    // Return 0 means the program ran successfully
    return 0;
}
