/*
 * game.c
 * 
 * This is where all the game rules actually happen! 
 * I structured it around a big while loop that runs until all the books are collected.
 */
#include "game.h"
#include "deck.h"
#include "ui.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

// The assignment says it's regular Go Fish rules but with fewer cards.
// Standard Go Fish usually starts with 7 cards for 2 players.
#define INITIAL_CARDS 7

// Creating our two players
static Player user;
static Player pc;

// This is a helper function I wrote to handle bad user input. 
// If they type a word instead of a letter, this clears the junk out so we don't infinite loop.
static void clear_input_buffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}

// Prompts the user to pick a card from their hand
static Rank get_user_ask_rank(void) {
    char input[10];
    Rank r = 0;
    while (1) {
        printf("\nWhich rank do you ask for? (A, 2-9): ");
        // scanf reads user input. I limited it to 9 chars to prevent buffer overflows
        int res = scanf("%9s", input);
        if (res == EOF) {
            printf("\nExiting game...\n");
            exit(0);
        }
        if (res != 1) {
            clear_input_buffer();
            continue;
        }
        
        // Figure out what they typed
        if (input[0] == 'A' || input[0] == 'a') {
            r = RANK_A;
        } else if (input[0] >= '2' && input[0] <= '9') {
            r = (Rank)(input[0] - '0'); // Convert char to our enum number
        } else {
            printf("Invalid rank. Please enter A or 2-9.\n");
            continue;
        }
        
        // Game rule: You can't ask for a rank you don't have!
        if (!has_rank(&user, r)) {
            printf("You must have at least one card of the rank you are asking for.\n");
            continue;
        }
        
        return r; // Success!
    }
}

// Logic for the computer to pick a card
static Rank get_pc_ask_rank(void) {
    // I decided to make the computer pick a random card from its hand. 
    // It's not a super genius AI, but it works well and is fair!
    if (pc.count == 0) return RANK_A; 
    
    // Pick a random index
    int idx = rand() % pc.count;
    return pc.cards[idx].rank;
}

// Helper to check for books and print a message if one was found
static void check_books_and_notify(Player *p, const char* name) {
    int books = check_and_remove_books(p);
    if (books > 0) {
        char msg[100];
        // Using sprintf to build a string, then printing it with our cool delay function
        sprintf(msg, ">>> %s completed %d book(s)!", name, books);
        print_message_delayed(msg);
    }
}

// This function handles a single turn. 
// It returns true if the player gets to go again, or false if their turn ends.
static bool play_turn(Player *current, Player *opponent, const char *current_name, const char *opponent_name) {
    // If your hand is empty, you just draw a card instead of asking
        if (current->count == 0) {
            if (cards_left_in_deck() > 0) {
                Card c;
                draw_card(&c);
                add_card(current, c);
                
                char msg[100];
                if (current->id == PLAYER_USER) {
                    sprintf(msg, "Your hand was empty. You drew a card: ");
                    printf("%s", msg);
                    print_card_name(c);
                    printf("\n");
                    sleep(1);
                } else {
                    sprintf(msg, "%s's hand was empty. Drew a card.", current_name);
                    print_message_delayed(msg);
                }
                check_books_and_notify(current, current_name);
            } else {
                print_message_delayed("Deck is empty and hand is empty. Turn ends.");
                return false;
            }
        }
    
    // Failsafe
    if (current->count == 0) return false;
    
    // Step 1: Ask for a card
    Rank asked_rank;
    if (current->id == PLAYER_USER) {
        asked_rank = get_user_ask_rank();
    } else {
        asked_rank = get_pc_ask_rank();
        char msg[100];
        sprintf(msg, "Computer asks: 'Do you have any %ss?'", get_rank_name(asked_rank));
        print_message_delayed(msg);
    }
    
    // Step 2: See if the opponent has it
    if (has_rank(opponent, asked_rank)) {
        char msg[100];
        if (current->id == PLAYER_PC) {
            sprintf(msg, "Yes, you have %ss!", get_rank_name(asked_rank));
        } else {
            sprintf(msg, "Yes, %s has %ss!", opponent_name, get_rank_name(asked_rank));
        }
        print_message_delayed(msg);
        
        // Opponent gives up all cards of that rank
        Card transfer[4];
        int given = give_cards_of_rank(opponent, asked_rank, transfer);
        for (int i = 0; i < given; ++i) {
            add_card(current, transfer[i]);
        }
        
        sprintf(msg, "%s received %d card(s).", current_name, given);
        print_message_delayed(msg);
        
        // Check if getting these cards completed a book!
        check_books_and_notify(current, current_name);
        
        return true; // They got what they asked for, so they go again!
        
    } else {
        // Step 3: GO FISH!
        char msg[100];
        sprintf(msg, "%s says: 'Go Fish!'", opponent_name);
        print_message_delayed(msg);
        
        // Draw a card from the deck
        if (cards_left_in_deck() > 0) {
            Card c;
            draw_card(&c);
            
            // Show the user what they drew (but hide it if it's the computer)
            if (current->id == PLAYER_USER) {
                printf("You drew: ");
                print_card_name(c);
                printf("\n");
                sleep(1); // Wait 1 second so the user can read what they got
            } else {
                print_message_delayed("Computer drew a card.");
            }
            
            add_card(current, c);
            check_books_and_notify(current, current_name);
            
            // If you drew the card you asked for, you get to go again!
            if (c.rank == asked_rank) {
                sprintf(msg, "%s drew the asked rank! Turn continues.", current_name);
                print_message_delayed(msg);
                return true;
            }
        } else {
            print_message_delayed("Deck is empty. Nothing to draw.");
        }
        
        return false; // Turn is over.
    }
}

// The main driver function
void run_game(void) {
    // Seed the random number generator so the deck shuffles differently every time
    srand((unsigned int)time(NULL));
    
    // Setup everything
    init_deck();
    shuffle_deck();
    init_player(&user, PLAYER_USER);
    init_player(&pc, PLAYER_PC);
    init_ui();
    
    print_welcome_banner();
    
    // Deal the starting hands (7 cards each)
    for (int i = 0; i < INITIAL_CARDS; ++i) {
        Card c;
        if (draw_card(&c)) add_card(&user, c);
        if (draw_card(&c)) add_card(&pc, c);
    }
    
    // Just in case someone was incredibly lucky and got a book in their starting hand!
    check_and_remove_books(&user);
    check_and_remove_books(&pc);
    
    int turn = 0; // 0 = User's turn, 1 = Computer's turn
    
    // The game continues until all 9 books (since there's 36 cards) are found
    // NUM_RANKS is 10, so NUM_RANKS - 1 is 9.
    while ((user.books + pc.books) < NUM_RANKS - 1) { 
        // Sort hands so they look organized
        sort_hand(&user);
        sort_hand(&pc);
        
        // Print the scoreboard
        print_game_status(&user, &pc, cards_left_in_deck());
        
        if (turn == 0) {
            print_hand(&user);
            print_hand(&pc); // This only prints how many cards they have
            
            // Run the user's turn
            bool again = play_turn(&user, &pc, "You", "Computer");
            if (!again) {
                turn = 1; // Swap to computer if turn is over
            }
        } else {
            print_hand(&user); // Show hand so user can see what's being asked
            print_hand(&pc);
            
            // Run the computer's turn
            bool again = play_turn(&pc, &user, "Computer", "You");
            if (!again) {
                turn = 0; // Swap to user if turn is over
            }
        }
        
        // A visual divider between turns
        printf("\n=================================================\n");
        sleep(1); // Small delay so the text isn't overwhelming
    }
    
    // Game is over, determine the winner!
    printf("\n" COLOR_YELLOW "=== GAME OVER ===" COLOR_RESET "\n");
    printf("You made %d books.\n", user.books);
    printf("Computer made %d books.\n", pc.books);
    
    if (user.books > pc.books) {
        printf(COLOR_GREEN "Congratulations, you won!\n" COLOR_RESET);
    } else if (pc.books > user.books) {
        printf(COLOR_RED "The computer won. Better luck next time.\n" COLOR_RESET);
    } else {
        printf("It's a tie!\n");
    }
}
