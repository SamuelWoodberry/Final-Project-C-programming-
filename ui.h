/*
 * ui.h
 *
 * Handles all the printing to the screen.
 * I wanted the game to look really cool and professional, so I added some
 * colors and ASCII art instead of just plain text.
 */
#ifndef UI_H
#define UI_H

#include "player.h"

// ANSI Color Codes for terminal output
#define COLOR_RESET "\x1b[0m"
#define COLOR_CYAN "\x1b[36m"
#define COLOR_MAGENTA "\x1b[35m"
#define COLOR_YELLOW "\x1b[33m"
#define COLOR_RED "\x1b[31m"
#define COLOR_GREEN "\x1b[32m"

// Sets up anything needed for the UI (if any)
void init_ui(void);

// Prints a nice looking title screen
void print_welcome_banner(void);

// Shows the player's cards with symbols like [ A ♥ ]
void print_hand(const Player *p);

// Prints the current score and how many cards are left in the deck
void print_game_status(const Player *user, const Player *pc,
                       int deck_cards_left);

// Prints a single card with the correct color (Red for hearts/diamonds)
void print_card_name(Card c);

// Converts our Rank enum into a string (like RANK_A to "A")
const char *get_rank_name(Rank r);

// Prints text but waits for a fraction of a second.
// This makes the game feel more like a real game instead of a wall of text
// instantly appearing.
void print_message_delayed(const char *msg);

#endif // UI_H
