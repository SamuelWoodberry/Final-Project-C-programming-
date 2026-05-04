/*
 * deck.h
 * 
 * This file contains the function prototypes for managing the deck of cards.
 * I separated this from the player logic to keep things modular.
 */
#ifndef DECK_H
#define DECK_H

#include "common.h"

// Initializes the deck with our 36 specific cards
void init_deck(void);

// Shuffles the deck so the game isn't the same every time
void shuffle_deck(void);

// Takes a card off the top of the deck. 
// Returns true if it worked, or false if the deck is empty.
bool draw_card(Card *out_card);

// Helper function to see how many cards are left to draw
int cards_left_in_deck(void);

#endif // DECK_H
