/*
 * deck.c
 * 
 * This handles all the deck operations like creating, shuffling, and drawing.
 */
#include "deck.h"
#include <stdlib.h>

// I'm using static global variables here so they are only accessible 
// within this file (encapsulation!). 
static Card g_deck[MAX_CARDS];
static int g_deck_size = 0;

void init_deck(void) {
    g_deck_size = 0;
    
    // Nested loops to generate all 36 cards. 
    // We go through each suit, and for each suit, we go through ranks A, 2-9.
    for (int s = HEARTS; s < NUM_SUITS; ++s) {
        for (int r = RANK_A; r < NUM_RANKS; ++r) {
            g_deck[g_deck_size].suit = (Suit)s;
            g_deck[g_deck_size].rank = (Rank)r;
            g_deck_size++; // Move to the next slot in the array
        }
    }
}

void shuffle_deck(void) {
    // If there's 1 or fewer cards, no need to shuffle
    if (g_deck_size <= 1) return;
    
    // I used the Fisher-Yates shuffle algorithm here because it's the standard
    // way to shuffle an array perfectly in C.
    for (int i = g_deck_size - 1; i > 0; --i) {
        int j = rand() % (i + 1); // Pick a random index from 0 to i
        // Swap the cards
        Card temp = g_deck[i];
        g_deck[i] = g_deck[j];
        g_deck[j] = temp;
    }
}

bool draw_card(Card *out_card) {
    // Can't draw if the deck is empty!
    if (g_deck_size == 0) {
        return false;
    }
    
    // We draw from the "top" of the deck, which is the end of the array.
    // This is much faster than shifting all array elements if we drew from index 0.
    g_deck_size--;
    if (out_card != NULL) {
        *out_card = g_deck[g_deck_size];
    }
    return true; // Successfully drew a card
}

int cards_left_in_deck(void) {
    return g_deck_size;
}
