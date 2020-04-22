// Author: Faiyaz A. Chowdhury
// Date Created: April 20, 2020
// Last Updated: April 21, 2020, by Faiyaz A. Chowdhury
// A program to study GRE vocab. It displays cards more frequently if they less known. (5-Known very well, 0-Unknown)
// Dictionary format: word-definition [newline]
// DOES NOT SAVE UNTIL ESC HIT WHEN PROMPTED!

#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <time.h>
#include <bits/stdc++.h>

#define CARD_FILENAME "smart_cards_memory.txt"
#define CHANCE_BOOST 5

using namespace std;

class Card{
    string word;
    string definition;
    int strength;
    int trial_count;
public:
    Card(const string& word, const string& definition):word(word),definition(definition),strength(0),trial_count(0){}
    Card(const string& word, const string& definition, const int& strength, const int& trial_count): word(word), definition(definition), strength(strength), trial_count(trial_count) {}
    friend ostream& operator<<(ostream& os, const Card& card){os << card.word << "-" << card.definition << "-" << card.strength << " " << card.trial_count << endl;}
    friend istream& operator>>(istream& is, Card& card){}
    string show_card() const{
        cout << "What is the meaning of " << word << "? ";
        string entry;
        getline(cin, entry, '\n');
        if(entry.compare("Q")!=0) cout << definition << endl;
        return entry;
    }
    string update_strength() {
        string entry;
        cout << "How well did you know the word? 1(Unknown)--> 5(Know it already): ";
        cin >> entry;
        if(entry.compare("1")==0){
            strength = 1; trial_count = 1;
        } else if(entry.compare("2")==0){
            strength = 2; trial_count = 2;
        } else if(entry.compare("3")==0){
            strength = 3; trial_count = 3;
        } else if(entry.compare("4")==0){
            strength = 4; trial_count = 4;
        } else if(entry.compare("5")==0){
            strength = 5; trial_count = 5;
        } else {
            entry = "Q";
            return entry;
        }
        cout << "      Card Updated" << endl << endl;
        cin.ignore();
        return entry;
    }
    bool forgottern_card(){
        trial_count--;
        if(trial_count<1){
            return true;
        }
        return false;
    }
};

int dictionary_file_to_program(vector<Card>& card_list){
    cout << "Please enter existing dictionary file name: ";
    string dictionary_filename;
    cin >> dictionary_filename;
    ifstream dictionary_file(dictionary_filename);
    while(!dictionary_file.is_open()){
        cout << "Cannot find dictionary file " << dictionary_filename << endl << "Please enter dictionary file name: ";
        cin >> dictionary_filename;
        dictionary_file.open(dictionary_filename);
    }

    int number_of_cards = 0;

    string line, word, definition;    
    while(getline(dictionary_file, line)){
        istringstream line_stream(line);
        getline(line_stream, word, '-');
        getline(line_stream, definition);
        card_list.push_back(Card(word,definition));
        //cout << card_list[number_of_cards];
        number_of_cards++;
    }

    dictionary_file.close();
    return number_of_cards;
}

int cards_file_to_program(fstream& cards_file, vector<Card>& card_list, int& number_of_known_cards){
    int number_of_cards;
    cards_file >> number_of_cards >> number_of_known_cards;
    cards_file.ignore();

    string line, word, definition;
    int strength, trial_count;
    while(getline(cards_file, line)){
        istringstream line_stream(line);
        getline(line_stream, word, '-');
        getline(line_stream, definition, '-');
        line_stream >> strength >> trial_count;
        card_list.push_back(Card(word,definition,strength,trial_count));
    }

    return number_of_cards;
}

void program_to_cards_file(ofstream& cards_file, const vector<Card>& card_list, const int& number_of_cards, const int& number_of_known_cards){
    cards_file << number_of_cards << ' ' << number_of_known_cards << endl;
    for(const Card& card: card_list){
        cards_file << card;
        // cout << card;
    }
    cout << "Data has been saved. Have a great day!" << endl;
    return;
}

int calculate_card_index(const int& number_of_cards, int& number_of_known_cards){
    int card_index = 0;
    if(number_of_known_cards==number_of_cards){
        card_index = rand()%number_of_known_cards;
    } else if(number_of_known_cards>0){
        card_index = rand()%(number_of_known_cards+1+CHANCE_BOOST);
        if(card_index > number_of_known_cards){ // if card_index has higher chance
            card_index = number_of_known_cards;
        }
    }
    return card_index;
}

void learn_cards(vector<Card>& card_list, const int& number_of_cards, int& number_of_known_cards){
    cout << "Lets learn! Press ""Q"" to save and exit. Press anything to see word meaning" << endl;
    string entry;
    cin.ignore();

    int card_index = calculate_card_index(number_of_cards, number_of_known_cards);
    while(!card_list[card_index].forgottern_card()){
        card_index = calculate_card_index(number_of_cards, number_of_known_cards);
    }
    entry = card_list[card_index].show_card();

    while(entry.compare("Q")!=0){
        entry = card_list[card_index].update_strength();
        if(entry.compare("Q")==0) break;
        if(card_index==number_of_known_cards){
            number_of_known_cards++;
        }
        card_index = calculate_card_index(number_of_cards, number_of_known_cards);
        while(!card_list[card_index].forgottern_card()){
            card_index = calculate_card_index(number_of_cards, number_of_known_cards);
        }
        entry = card_list[card_index].show_card();
    }
}

int main(){
    srand(time(NULL));
    vector<Card> card_list;
    int number_of_cards = 0, number_of_known_cards = 0;

    fstream cards_file(CARD_FILENAME);
    if(!cards_file.is_open()){
        number_of_cards = dictionary_file_to_program(card_list); // If GRE_vocab.txt does not exist
    } else {
        number_of_cards = cards_file_to_program(cards_file, card_list, number_of_known_cards);
    }
    cards_file.close();


    learn_cards(card_list, number_of_cards, number_of_known_cards);


    ofstream out_cards_file(CARD_FILENAME);
    program_to_cards_file(out_cards_file, card_list, number_of_cards, number_of_known_cards); // Save to GRE_vocab.txt
    cards_file.close();
    return 0;
}