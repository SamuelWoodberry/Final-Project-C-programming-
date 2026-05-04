/*
 * player.h
 * 
 * Defines what a Player is and what they can do. 
 * Both the user and the computer will use these same structures and functions.
 */
#ifndef PLAYER_H
#define PLAYER_H

#include "common.h"

// The Player struct holds their hand, how many cards they have, 
// and their score (number of books).
typedef struct {
    Card cards[MAX_CARDS]; // Array to hold the hand
    int count;             // How many cards are currently in the hand
    int books;             // The player's score
    PlayerId id;           // Are they the User or the PC?
} Player;

// Sets up a player with 0 cards and 0 score
void init_player(Player *p, PlayerId id);

// Adds a drawn card to their hand array
void add_card(Player *p, Card c);

// Checks if the player has at least one card of a certain rank.
// (You can only ask for a rank if you already have it).
bool has_rank(const Player *p, Rank r);

// When a player is asked for a card they have, they must give all of them.
// This removes them from their hand and puts them into out_cards.
int give_cards_of_rank(Player *p, Rank r, Card *out_cards);

// Checks if the player has 4 of the same rank (a "book").
// If they do, it removes those 4 cards and increases their score.
int check_and_remove_books(Player *p);

// Returns the current score
int get_score(const Player *p);

// Returns how many cards are in their hand right now
int get_hand_size(const Player *p);

// Sorts the hand so it looks nice when printed to the screen
void sort_hand(Player *p);

#endif // PLAYER_H
