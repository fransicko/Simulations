// Our included files
#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

// The card class
class Card {
public:
  enum Suit {
    SPADE, CLUB, DIAMOND, HEART, NONE
  };

  Card() {
    mValue = 0;
    mSuit = NONE;
  }
  Card(int symbol, Suit suit) { 
    mValue = symbol; 
    mSuit = suit;
  }

  //variables
  int mValue; // This will be between
  Suit mSuit;
};

// GLobal
Card deck[52];
vector<Card> hand_1;
vector<Card> hand_2;
double r = 0; // comments later
string file = "";
int N = 0; // Total number of turns
int T = 0; // the player winning the game transitioned four times
double L = 0; // T/N

void swap (Card &a, Card &b) {
  Card c = a;

  a = b;
  b = c;
}


void shuffle() {
  int c = 0;
  while ( c < 51 ) {
    // p is a random index in [c,n-1]
    int p = r*(52-c) + c;
    swap(deck[c], deck[p]);

    ++c;
  }
}

void hand_shuffle(int hand) {
  int c = 0;
  int n = 0;

  if ( hand == 1) n = hand_1.size();
  else n = hand_2.size();
  while ( c < (n-1) ) {
    // p is a random index in [c,n-1]
    int p = r*(n-c) + c;

    if (hand == 1) swap(hand_1.at(c), hand_1.at(p));
    else if (hand == 2) swap(hand_2.at(c), hand_2.at(p));

    ++c;
  }
}
// this will initialize the deck with 
void initialize() { 
  int i = 0;
  int value = 2;
  // Spades

  for (int i = 0; i < 49; i += 4) {
    deck[i] = Card(value, Card::SPADE);
    deck[i+1] = Card(value, Card::CLUB);
    deck[i+2] = Card(value, Card::DIAMOND);
    deck[i+3] = Card(value, Card::HEART);

    ++value;
  }
}

void print() {
  for (int i = 0; i < 52; ++i) cout << deck[i].mValue << " ";
  cout << endl;
}

void dealHands() {
  for (int i = 0; i < 51; i+=2) {
    hand_1.push_back(deck[i]);
    hand_2.push_back(deck[i+1]);
  }
}

void sim_war() {
  initialize();
  shuffle();
 
  vector<Card> pile_1;
  vector<Card> pile_2; 
  vector<Card> draw_pile;

  dealHands();
  cout << "Hand delt" << endl;
  cout << hand_1.size() << "  " << hand_2.size() << endl;

  // the start of the game
  while (true) {
  ++N;
  while (hand_1.size() != 0  && hand_2.size() != 0 ) {
    if (hand_1.back().mValue > hand_2.back().mValue) {
      pile_1.push_back(hand_1.back());
      pile_1.push_back(hand_2.back());

      pile_1.insert(pile_1.end(), draw_pile.begin(), draw_pile.end());

      hand_1.pop_back();
      hand_2.pop_back();
      draw_pile.clear();
    }
    else if (hand_2.back().mValue > hand_1.back().mValue) {
      pile_2.push_back(hand_1.back());
      pile_2.push_back(hand_2.back());

      pile_2.insert(pile_2.end(), draw_pile.begin(), draw_pile.end());

      hand_1.pop_back();
      hand_2.pop_back();
      draw_pile.clear();
    }
    else {
      draw_pile.push_back(hand_1.back());
      draw_pile.push_back(hand_2.back());

      hand_1.pop_back();
      hand_2.pop_back();
    }
  }

     cout << draw_pile.size() << " " << hand_1.size() << " " << pile_1.size() << " " << hand_2.size() << " " << pile_2.size()  << endl;
    if (hand_1.size() == 0 && pile_1.size() == 0) break;
    else if (hand_1.size() == 0) {
      hand_1.insert(hand_1.end(), pile_1.begin(), pile_1.end());
      pile_1.clear();
      hand_shuffle(1);
    }

 
    if (hand_2.size() == 0 && pile_2.size() == 0) break;
    else if (hand_2.size() == 0) {
      hand_2.insert(hand_2.end(), pile_2.begin(), pile_2.end());
      pile_2.clear();
      hand_shuffle(2);
    }

  }
}

int main(int argc, char** argv) {
  string game = argv[1];
  file = argv[2];
  ifstream inFile;

  inFile.open(file.c_str());

  if (!inFile) {
    cerr << "WTF";
  }

  inFile >> r;
  sim_war();
 // cout << deck[0].getSymbol() << endl;
  inFile.close();
  return 0;
}
