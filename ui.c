/*
 * ui.c
 * 
 * The actual code that makes the game look pretty in the terminal.
 * I used ANSI escape codes here to add colors.
 */
#include "ui.h"
#include <stdio.h>
#include <unistd.h> // I included this so I can use usleep() for the text delay


// Using UTF-8 characters for the card suits to make it look unique!
static const char* suit_symbols[] = {
    "\xE2\x99\xA5", // Hearts ♥
    "\xE2\x99\xA6", // Diamonds ♦
    "\xE2\x99\xA3", // Clubs ♣
    "\xE2\x99\xA0"  // Spades ♠
};

// Simple array to map our ranks to strings. Index 1 = "A", Index 2 = "2", etc.
static const char* rank_names[] = {
    "?", "A", "2", "3", "4", "5", "6", "7", "8", "9"
};

void init_ui(void) {
    // Just a placeholder in case we need to clear the screen or something later
}

void print_welcome_banner(void) {
    // Printing a big bordered box for the title
    printf(COLOR_CYAN "=================================================\n");
    printf("=                                               =\n");
    printf("=" COLOR_YELLOW "               MODIFIED GO FISH                " COLOR_CYAN "=\n");
    printf("=                                               =\n");
    printf("=================================================\n" COLOR_RESET);
    printf("Welcome! Try to collect the most 4-of-a-kind books.\n\n");
}

const char* get_rank_name(Rank r) {
    // Safety check to make sure we don't go out of bounds of our array
    if (r >= RANK_A && r < NUM_RANKS) {
        return rank_names[r];
    }
    return "?";
}

void print_card_name(Card c) {
    // I thought it would be cool if Hearts and Diamonds were actually red!
    if (c.suit == HEARTS || c.suit == DIAMONDS) {
        printf(COLOR_RED "%s %s" COLOR_RESET, rank_names[c.rank], suit_symbols[c.suit]);
    } else {
        // Clubs and Spades stay the default terminal color
        printf("%s %s", rank_names[c.rank], suit_symbols[c.suit]);
    }
}

void print_hand(const Player *p) {
    if (p->id == PLAYER_USER) {
        printf(COLOR_CYAN "Your Hand:" COLOR_RESET "\n");
    } else {
        printf(COLOR_MAGENTA "Computer's Hand (Hidden):" COLOR_RESET "\n");
        // We shouldn't show the computer's actual cards, so just show how many they have!
        printf("[%d cards]\n", p->count);
        return; // Stop here so we don't print their cards
    }
    
    // Print each card in the user's hand inside brackets
    for (int i = 0; i < p->count; ++i) {
        printf("[ ");
        print_card_name(p->cards[i]);
        printf(" ] ");
    }
    printf("\n");
}

void print_game_status(const Player *user, const Player *pc, int deck_cards_left) {
    // This prints the "scoreboard"
    printf("\n" COLOR_YELLOW "--- GAME STATUS ---" COLOR_RESET "\n");
    printf("Deck: %d cards left\n", deck_cards_left);
    printf(COLOR_CYAN "You: %d books | %d cards" COLOR_RESET "\n", user->books, user->count);
    printf(COLOR_MAGENTA "PC:  %d books | %d cards" COLOR_RESET "\n", pc->books, pc->count);
    printf(COLOR_YELLOW "-------------------" COLOR_RESET "\n\n");
}

void print_message_delayed(const char *msg) {
    printf("%s", msg);
    fflush(stdout); // Force the text to print immediately
    usleep(400000); // Wait for 0.4 seconds. This adds a dramatic effect to drawing cards!
    printf("\n");
}
