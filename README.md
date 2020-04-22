# smart-cards
A program to study GRE vocab, or remember paired concepts.
It displays cards more frequently if they are less known. (5-Known very well, 1-Unknown)

To run program, in terminal: g++ smart_cards.cpp
                             ./a.out
When prompted for a dictionary file, enter "dict.txt". You could make your own dictionary, such as friend-phone_number.
The format for a dictionary file just has to be [front_of_flashcard]-[back_of_flashcard] [newlne].

Program does not save until user enters Q or exits properly. Saving creates another .txt file that saves card progress.
Increase CHANCE_BOOST to increase probability of seeing new cards.
Change CARD_FILENAME to change name of save file.

Enjoy!
