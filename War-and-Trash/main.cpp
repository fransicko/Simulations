// Our included files
#include <iostream>
using namespace std;

// The card class
class Card {
public:
  enum Suit {
    SPADE, CLUB, DIAMOND, HEART
  };

  Card() {
    mSymbol = 'l';
    mSuit = SPADE;
  }
  Card(char symbol, Suit suit) { 
    mSymbol = symbol; 
    mSuit = suit;
  }

  char getSymbol() { return mSymbol;}

private:
  char mSymbol; // This will be between
  Suit mSuit;
};

// GLobal
Card deck[52];
int N = 0; // Total number of turns
int T = 0; // the player winning the game transitioned four times
double L = 0; // T/N

// this will initialize the deck with 
void initialize() { 
  deck[0] = Card('1', Card::SPADE);
}

int main() {
  initialize();

  cout << deck[0].getSymbol() << endl;

  return 0;
}
