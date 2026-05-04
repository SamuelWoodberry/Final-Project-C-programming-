/*
 * common.h
 * 
 * I created this file to store the basic structures and enums that 
 * multiple other files need to share. This prevents circular dependencies 
 * and makes the code cleaner.
 */
#ifndef COMMON_H
#define COMMON_H

#include <stdbool.h>

// Defining the 4 suits using an enum makes the code easier to read
typedef enum {
    HEARTS,
    DIAMONDS,
    CLUBS,
    SPADES,
    NUM_SUITS // I use this trick to keep track of how many suits there are
} Suit;

// The assignment says we only use A and 2-9. So 9 ranks total.
// I set RANK_A to 1 so the numbers match up better logically.
typedef enum {
    RANK_A = 1,
    RANK_2,
    RANK_3,
    RANK_4,
    RANK_5,
    RANK_6,
    RANK_7,
    RANK_8,
    RANK_9,
    NUM_RANKS // This will equal 10, meaning 9 valid ranks (1-9)
} Rank;

// A Card is just a combination of a Suit and a Rank
typedef struct {
    Suit suit;
    Rank rank;
} Card;

// We have 9 ranks * 4 suits = 36 cards total for this modified version
#define MAX_CARDS 36

// Simple way to tell whose turn it is or who owns a hand
typedef enum {
    PLAYER_USER,
    PLAYER_PC
} PlayerId;

#endif // COMMON_H
