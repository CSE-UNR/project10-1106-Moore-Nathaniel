//Name: Nathaniel Moore
//Date: 5/6/25
//Purpose: Wordle!


#include <stdio.h>
#include <stdbool.h>

#define MAX_GUESSES 6
#define WORD_LENGTH 5

// Converts a string to lowercase
void to_lowercase(char *str) {
    for (int i = 0; str[i] != '\0'; i++) {
        if (str[i] >= 'A' && str[i] <= 'Z') {
            str[i] = str[i] + ('a' - 'A');
        }
    }
}

// Returns the length of a string
int str_length(const char *str) {
    int len = 0;
    while (str[len] != '\0') {
        len++;
    }
    return len;
}

// Checks if guess is valid (5 letters, all alphabetic)
bool is_valid_guess(const char *guess) {
    if (str_length(guess) != WORD_LENGTH)
        return false;

    for (int i = 0; i < WORD_LENGTH; i++) {
        if (!((guess[i] >= 'a' && guess[i] <= 'z') || (guess[i] >= 'A' && guess[i] <= 'Z')))
            return false;
    }
    return true;
}

// Copies a string
void str_copy(char *dest, const char *src) {
    int i = 0;
    while ((dest[i] = src[i]) != '\0') {
        i++;
    }
    dest[i] = '\0';
}

// Loads the word from file
void load_word(char *word) {
    FILE *fp = fopen("word.txt", "r");
    if (!fp) {
        printf("Error: could not open word.txt\n");
        word[0] = '\0';
        return;
    }
    fscanf(fp, "%5s", word);
    fclose(fp);
    word[WORD_LENGTH] = '\0';
    to_lowercase(word);
}

// Gets a valid guess from the user
void get_guess(char *guess, int guess_count) {
	bool valid_input = false;
    while (!valid_input) {
    	if (guess_count == MAX_GUESSES - 1){
    		printf("FINAL GUESS: ");
    	}
    	else {
        	printf("GUESS %d! Enter your guess: ", guess_count + 1);
        }       
        scanf("%s", guess);       
        if (is_valid_guess(guess)) {
          valid_input = true;
        }          
        else { 
        	printf("Invalid input. Please enter exactly 5 letters.\n");
        }
     }
     printf("================================\n");
}

// Gives feedback (capitalization for correct letters)
void give_feedback(const char *guess, const char *mystery_word, char *output, char *pointer_line) {
    bool used_in_word[WORD_LENGTH] = { false };
    bool used_in_guess[WORD_LENGTH] = { false };

    for (int i = 0; i < WORD_LENGTH; i++) {
        if (guess[i] == mystery_word[i]) {
            output[i] = guess[i] - ('a' - 'A'); // Capitalize
            used_in_word[i] = true;
            used_in_guess[i] = true;
            pointer_line[i] = ' ';
        } else {
            output[i] = guess[i]; // Lowercase as-is
            pointer_line[i] = ' ';
        }
    }

    for (int i = 0; i < WORD_LENGTH; i++) {
        if (!used_in_guess[i]) {
            for (int j = 0; j < WORD_LENGTH; j++) {
                if (!used_in_word[j] && guess[i] == mystery_word[j]) {
                    pointer_line[i] = '^';
                    used_in_word[j] = true;
                    break;
                }
            }
        }
    }
    
    output[WORD_LENGTH] = '\0';
    pointer_line[WORD_LENGTH] = '\0';
}

// Compares guess to mystery word
bool is_correct_guess(const char *guess, const char *mystery_word) {
    for (int i = 0; i < WORD_LENGTH; i++) {
        if (guess[i] != mystery_word[i])
            return false;
    }
    return true;
}

// Displays all previous guesses and feedback
void display_guesses(char guesses[][WORD_LENGTH + 1], char outputs[][WORD_LENGTH + 1], char pointers[][WORD_LENGTH + 1], int guess_count) {
    for (int i = 0; i < guess_count; i++) {
        printf("%s\n", outputs[i]);
        printf("%s\n", pointers[i]); 
    }
    printf("\n");
}

// Main function
int main() {
    char mystery_word[WORD_LENGTH + 1];
    load_word(mystery_word);

    char guesses[MAX_GUESSES][WORD_LENGTH + 1];
    char outputs[MAX_GUESSES][WORD_LENGTH + 1];
    char pointers[MAX_GUESSES][WORD_LENGTH + 1];

    int guess_count = 0;

    while (guess_count < MAX_GUESSES) {
        char guess[100];
        get_guess(guess, guess_count);
        to_lowercase(guess);

        str_copy(guesses[guess_count], guess);
        give_feedback(guess, mystery_word, outputs[guess_count], pointers[guess_count]);
        display_guesses(guesses, outputs, pointers, guess_count + 1);

        if (is_correct_guess(guess, mystery_word)) {
    printf("You won in %d guess%s! Amazing!\n", guess_count + 1, (guess_count + 1) == 1 ? "" : "es");
    return 0;
	}
        guess_count++;
    }
    
    printf("Sorry, you've used all your guesses. The word was: %s\n", mystery_word);
    return 0;
}

