/*
 * player.c
 * 
 * Implementation of the player's actions, like giving cards or checking for books.
 */
#include "player.h"
#include <string.h>
#include <stdlib.h>

void init_player(Player *p, PlayerId id) {
    // Start fresh
    p->count = 0;
    p->books = 0;
    p->id = id;
}

void add_card(Player *p, Card c) {
    // Make sure we don't overflow the array, just to be safe
    if (p->count < MAX_CARDS) {
        p->cards[p->count++] = c;
    }
}

bool has_rank(const Player *p, Rank r) {
    // Loop through the hand to see if they have the requested rank
    for (int i = 0; i < p->count; ++i) {
        if (p->cards[i].rank == r) {
            return true; // Found one!
        }
    }
    return false; // Nope, don't have it
}

int give_cards_of_rank(Player *p, Rank r, Card *out_cards) {
    int given = 0;
    int new_count = 0;
    Card new_hand[MAX_CARDS]; // A temporary array to hold cards we are keeping
    
    // Go through every card in the hand
    for (int i = 0; i < p->count; ++i) {
        if (p->cards[i].rank == r) {
            // We have to give this card away!
            if (out_cards != NULL) {
                out_cards[given] = p->cards[i];
            }
            given++;
        } else {
            // We get to keep this one, so put it in our temporary new hand
            new_hand[new_count++] = p->cards[i];
        }
    }
    
    // Overwrite the old hand with the new hand (which now has fewer cards)
    p->count = new_count;
    for (int i = 0; i < new_count; ++i) {
        p->cards[i] = new_hand[i];
    }
    
    return given; // Return how many cards we had to give up
}

int check_and_remove_books(Player *p) {
    int books_found = 0;
    // Array to count how many of each rank the player has.
    // For example, rank_counts[2] will be the number of 2s in the hand.
    int rank_counts[NUM_RANKS] = {0};
    
    // First, tally up the occurrences of each rank
    for (int i = 0; i < p->count; ++i) {
        rank_counts[p->cards[i].rank]++;
    }
    
    // Now, check if any rank reached 4
    for (int r = RANK_A; r < NUM_RANKS; ++r) {
        if (rank_counts[r] == 4) {
            // We got a book! 
            // Use our own give_cards function to remove them from the hand.
            give_cards_of_rank(p, (Rank)r, NULL);
            p->books++;      // Increase score
            books_found++;   // Keep track of how many we found this turn
        }
    }
    
    return books_found;
}

int get_score(const Player *p) {
    return p->books;
}

int get_hand_size(const Player *p) {
    return p->count;
}

// This is a helper function for qsort. It tells qsort how to compare two cards.
static int compare_cards(const void *a, const void *b) {
    const Card *ca = (const Card *)a;
    const Card *cb = (const Card *)b;
    // First sort by rank
    if (ca->rank != cb->rank) {
        return (int)ca->rank - (int)cb->rank;
    }
    // If ranks are the same, sort by suit
    return (int)ca->suit - (int)cb->suit;
}

void sort_hand(Player *p) {
    // Only sort if we actually have cards. 
    // Using C's built-in quicksort (qsort) to keep things clean.
    if (p->count > 0) {
        qsort(p->cards, (size_t)p->count, sizeof(Card), compare_cards);
    }
}
