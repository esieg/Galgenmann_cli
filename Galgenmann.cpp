#include <iostream>
#include <map>
#include <string>
#include <cstdlib>
#include <cctype>
#include <limits>
#include <random>
#include <fstream>

class Gallows {
    /*Our class for the gamemethods and properties*/
    public:
        const uint dead_num = 9; // when we are dead?
        std::string word; // the word for guessing
        std::map<char, bool> wordDict; // map that contains every character of the word and a bool if charactere was found
        std::vector<char> failed_guesses; // a list of every failed guess until now
        uint error_count = 0; // counter of the failed guesses
        int result = 0; // tristate result: 0 - continue, 1 - win, -1 - lose

    void choose_word() {
        /*choose word from out words-file*/
        // read TXT with words
        std::ifstream file("Words.txt");
            std::vector<std::string> words;
            std::string row;

            while(std::getline(file, row)) {
                words.push_back(row);
            }

        file.close();

        // choose word by random
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, words.size() - 1);

        int ix = dis(gen);
        
        // set word
        word = words[ix];
    }

    void init_wordDict() {
        for (char c: word) {
            wordDict[c] = false; 
        }
    }

    void clear_screen() {
        /*Clear the terminal, so that only the game in the current round is visible*/
        std::cout << "\033[2J\033[H"; // Clear Screen and position the Cursor
        std::cout << "\033[?25l"; // Hide the Cursor
        std::cout << "\n\n"; //Draw 2 NewLines
    }

    void draw_word() {
        /*Draw the word with hidden, unguessed chars*/
        for (char c : word) {
            if(wordDict[c]) {
                std::cout << c << " ";  // guessed char
            } else {
                std::cout << "_ "; // not guessed char
            }
        }
        std::cout << "\n\n" << std::endl; // draw spacer lines
    }

    void draw_gallows() {
        /*Draw the gallows, state is depending on the error count*/
        switch (error_count) {
            case 9:
                std::cout << " ||========\n || /   |\n ||/    O\n ||    /|\\ \n ||    / \\ \n ||\n============\n";
                break;
            case 8:
                std::cout << " ||========\n || /   |\n ||/    O\n ||    /|\\ \n ||\n ||\n============\n";
                break;
            case 7:
                std::cout << " ||========\n || /   |\n ||/    O\n ||     |\n ||\n ||\n============\n"; 
                break;
            case 6:
                std::cout << " ||========\n || /   |\n ||/    O\n || \n ||\n ||\n============\n";
                break;
            case 5:
                std::cout << " ||========\n || /   |\n ||/ \n || \n ||\n ||\n============\n";
                break;
            case 4:
                std::cout << " ||========\n || /\n ||/ \n || \n ||\n ||\n============\n";
                break;
            case 3:
                std::cout << " ||========\n ||\n ||\n || \n ||\n ||\n============\n";
                break;
            case 2:
                std::cout << " ||\n ||\n ||\n ||\n ||\n ||\n============\n";
                break;
            case 1:
                std::cout << "\n\n\n\n\n\n============\n";
                break;
            case 0:
                std::cout << "\n\n\n\n\n\n\n";
                break;
        }
    }

    void draw_failure() {
        /*Draw the "failure screen"*/
        draw_gallows();
        std::cout << "\nFehlerhafte Versuche: " << error_count << " von " << dead_num << std::endl;
        for (char c : failed_guesses) {
            std::cout << c << " ";
        }
        std::cout << std::endl;
    }

    void draw_screen() {
        /*Draw the gamescreen*/
        clear_screen();
        draw_failure(); //contains draw_gallows
        draw_word();
    }

    void evaluate_result() {
        /*evaluate the current win-state of the game*/
        // Good Case: Word is guessed:
        result = 1;
        // Continue Case: Word is not guessed:
        for (const auto& pair : wordDict) {
            if (!pair.second) {
                result = 0;
            }
        }
        // Bad Case: Error Counter hit the dead_num (or above)
        if (dead_num <= error_count) {
            result = -1;
        }
    }

    void guess_char() {
        /*let the player guess the next char and evaluate the input*/
        char guess;
        std::cout << "Bitte rate einen Buchstaben: ";
        std::cin >> guess;
        guess = std::toupper(guess); // convert lower case to upper case character
        if (wordDict.find(guess) != wordDict.end()) {
            wordDict[guess] = true;
        } else {
            error_count = error_count + 1;
            failed_guesses.push_back(guess);
        }

    // clear cin
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
};

void initialize_game(Gallows &gallows) {
    /* Initialize the game, by choosing a word an set it */
    gallows.choose_word();
    gallows.init_wordDict();
}

void play_game(Gallows &gallows) {
    /* loop around the game, until word is guessed or 9 wrong guesses happens*/
    do {
        gallows.draw_screen(); // draw the gamescreen
        gallows.guess_char(); // take user input for next char guess
        gallows.evaluate_result(); // get result
    } while(gallows.result == 0);
}

int end_game(Gallows &gallows) {
    /* last steps before the game comes to a end (and maybe start again)*/
    // last draw
    gallows.draw_screen();
    // result print
    char userIn;
    switch (gallows.result) {
        case -1:
            std::cout << "Leider verloren" << std::endl;
            std::cout << "Das Wort war: " << gallows.word << std::endl;
            break;
        case 1:
            std::cout << "Glückwunsch, Gewonnen!" << std::endl;
            break;
    }
    // ask for replay
    std::cout << "Neues Spiel? (j):  ";
    std::cin >> userIn;
    return(userIn == 'j');
}

int main() {
    /* mainfunction, loop around the game for a replay*/
    bool replay = false;
    do {
        Gallows gallows;
        initialize_game(gallows);
        play_game(gallows);
        replay = end_game(gallows);
    } while(replay);
}