// Our included files
#include <iostream>
#include <fstream>
#include <vector>
#include <stack>
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
    face_down = true;
  }
  Card(int symbol, Suit suit) { 
    mValue = symbol; 
    mSuit = suit;
    face_down = true;
  }

  //variables
  int mValue; // This will be between
  Suit mSuit;
  bool face_down;
};

// GLobal

// The different decks
Card deck[52];
vector<Card> vDeck;

vector<Card> hand_1;
vector<Card> hand_2;
double r = 0; // comments later
string file = "";
int N = 0; // Total number of turns
int T = 0; // the player winning the game transitioned four times
int current_winner = 0;
double L = 0; // T/N

void swap (Card &a, Card &b) {
  Card c = a;

  a = b;
  b = c;
}

// Fisher - Yates Code here
void shuffle() {
  int c = 0;
  while ( c < 51 ) {
    // p is a random index in [c,n-1]
    int p = r*(52-c) + c;
    swap(deck[c], deck[p]);

    ++c;
  }
}

// shuffle player hands
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
void initialize(string game) { 
  int i = 0;
  int value = 0;

  if (game == "war" ) value = 2;
  else value = 1;
  
  for (int i = 0; i < 49; i += 4) {
    deck[i] = Card(value, Card::SPADE);
    deck[i+1] = Card(value, Card::CLUB);
    deck[i+2] = Card(value, Card::DIAMOND);
    deck[i+3] = Card(value, Card::HEART);

    ++value;
  }

  // Convert the deck array to a deck vector
  for (int i = 0; i < 52; ++i) {
    vDeck.push_back(deck[i]);
  }
}

void dealHands() {
  for (int i = 0; i < 51; i+=2) {
    hand_1.push_back(deck[i]);
    hand_2.push_back(deck[i+1]);
  }
}

// The war simulation
void sim_war() {
  initialize("war");
  shuffle();
 
  vector<Card> pile_1;
  vector<Card> pile_2; 
  vector<Card> draw_pile;

  dealHands();

  // the start of the game
  while (true) {
  while (hand_1.size() != 0  && hand_2.size() != 0 ) {
    ++N;
    if (hand_1.back().mValue > hand_2.back().mValue) { // THis is player 1 win
      pile_1.push_back(hand_1.back());
      pile_1.push_back(hand_2.back());

      pile_1.insert(pile_1.end(), draw_pile.begin(), draw_pile.end());

      hand_1.pop_back();
      hand_2.pop_back();
      draw_pile.clear();


    }
    else if (hand_2.back().mValue > hand_1.back().mValue) { // THis is player 2 win
      pile_2.push_back(hand_1.back());
      pile_2.push_back(hand_2.back());

      pile_2.insert(pile_2.end(), draw_pile.begin(), draw_pile.end());

      hand_1.pop_back();
      hand_2.pop_back();
      draw_pile.clear();


    }
    else { // draw
      draw_pile.push_back(hand_1.back());
      draw_pile.push_back(hand_2.back());

      hand_1.pop_back();
      hand_2.pop_back();
    }
  }

    if (hand_1.size() == 0 && pile_1.size() == 0) break;
    else if (hand_1.size() == 0) {
      hand_1.insert(hand_1.end(), pile_1.begin(), pile_1.end());
      pile_1.clear();
      hand_shuffle(1);

      if (pile_2.size() > pile_1.size()) {
        if (T == 0) {
          T += 1;
          current_winner = 2;
        }
        else if (current_winner == 1) {
          T += 1;
          current_winner = 2;
        } 
      }    
    }

 
    if (hand_2.size() == 0 && pile_2.size() == 0) break;
    else if (hand_2.size() == 0) {
      hand_2.insert(hand_2.end(), pile_2.begin(), pile_2.end());
      pile_2.clear();
      hand_shuffle(2);
      if (pile_1.size() > pile_2.size()) {
        if (T == 0) {
          T += 1;
          current_winner = 1;
        }
        else if (current_winner == 2) {
          T += 1;
          current_winner = 1;
        } 
      }     
    }
  }
}

// Dealing hand to trash hands
void tDeal(Card *player[], int hand, int hand_size) {
  for (int i = 0; i < hand_size; ++i) {
    player[hand][i] = vDeck.back();
    vDeck.pop_back();
  }
}

// Did one of them win
bool won(Card *player[], int hand, int hand_size) {
  for (int i=0; i < hand_size; ++i) {
    if (player[hand][i].face_down) return false;
  }

  return true;
}

// Reload teh vDeck
void reload(Card *player[], int hand, int hand_size, stack<Card> &discard) {
  Card empty;
  for (int i=0; i < hand_size; ++i) {
    player[hand][i].face_down = true;
    vDeck.push_back(player[hand][i]);
  }

  while (!discard.empty()) {
    vDeck.push_back(discard.top());
    discard.pop();
  }
 
  int c = 0;
  while ( c < vDeck.size()-1) {
    // p is a random index in [c,n-1]
    int p = r*(vDeck.size()-c) + c;
    swap(vDeck[c], vDeck[p]);

    ++c;
  }
}

//optimal jack
vector<int> check(Card *player1[], Card *player2[], int p1_hand, int p2_hand, int p1_hand_size, int p2_hand_size, stack<Card> discard, int n) {
  vector<int> check;

  for (int i =0; i < n; ++i) check.push_back(0);

  //cout << "after for loop" << endl;
  // loop through player1's hand
  for (int i=0; i < p1_hand_size; ++i) {
    if (player1[p1_hand][i].mValue <= n) {
      check.at(player1[p1_hand][i].mValue-1)++;
    }
  }


  //cout << "after forl lop 2" << endl;

  for (int i=0; i < p2_hand_size; ++i) {
    if (player2[p2_hand][i].mValue <= n) {
      check.at(player2[p2_hand][i].mValue-1)++;
    }
  }

  //cout << "for loop three" << endl;
  // checking the stack
  stack<Card> x;

  while (!discard.empty()) {
    int v = discard.top().mValue;
    if (v <= n) {
      check.at(v-1)++;
    }
    x.push(discard.top());
    discard.pop();
  }

  //cout << "end" << endl;
  return check;
}

// Trash simulations
void sim_trash() {
  initialize("trash");
  shuffle();

  //cout << "In he trash." << endl;

  stack<Card> discard;
  Card *play_1[10];
  Card *play_2[10];

  for (int i = 0; i < 10; ++i) {
    play_1[i] = new Card[10-i];
    play_2[i] = new Card[10-i];
  }

  // These let us know which array size they are currently on
  int p1_hand_size = 10;
  int p2_hand_size = 10;

  // Which hand they are using
  int p1_hand = 0;
  int p2_hand = 0;

  // The card in hand
  Card p1_card;
  Card p2_card;

  // filling hands
  tDeal(play_1, p1_hand, p1_hand_size);
  tDeal(play_2, p2_hand, p2_hand_size);

  discard.push(vDeck.back());
  vDeck.pop_back();
  // Needs an ace to win
  // loop for the hand
  while (play_1[9][0].mValue == 0 && play_2[9][0].mValue == 0) {
    // Put the top card into the stack

    // loop for the game
    //while (true) {
      // We put the top discard in our hand for referance
      p1_card = discard.top();
      bool start_turn = true;
      // loop for player 1 turn
      while (true) {
        //cout << "Player 1's" << endl;
        // We start the turn
        if (start_turn) {
          // Check if we can use the discard card
          if (p1_card.mValue <= p1_hand_size || p1_card.mValue == 11) {
            // check if we can use it
            // check joker
            if (p1_card.mValue == 11) {
              bool used_j = false;
              // put in the next open spot
              //cout << "here" << endl;
              vector<int> jack = check(play_1, play_2, p1_hand, p2_hand, p1_hand_size, p2_hand_size, discard, p1_hand_size);
              // find the maximum occurance
              int max = 0;
              for (int i=0; i < jack.size();++i) {
                if (jack.at(i) > max) {
                  max = jack.at(i);
                }
              }

               // else need to check 
               while (!used_j) {
                 for (int i = 0; i < jack.size(); ++i) {
                   if (jack.at(i) == max && play_1[p1_hand][i].face_down) {
                     p1_card.face_down = false;
                     swap(play_1[p1_hand][i], p1_card);
                     used_j = true;
                     p1_card.face_down = true;
                     break;
                   } 
                 }
                 --max;
               }
            
              if (won(play_1, p1_hand, p1_hand_size)) {
                if (T == 0) {
                  T += 1;
                  current_winner = 1;
                }
                else if (current_winner == 2) {
                  T += 1;
                  current_winner = 1;
                }
                  
                //cout << "**We have a winning array. for Player 1**" << endl;
                // put all the cards in the discard pile and hand into the deck
                reload(play_1, p1_hand, p1_hand_size, discard);
                p1_hand++;
                p1_hand_size--; 
                tDeal(play_1, p1_hand, p1_hand_size);
                discard.push(vDeck.back());
                vDeck.pop_back();
              } 
            }
            else { // else check if we can put in right spot
              if (play_1[p1_hand][p1_card.mValue-1].face_down) {
                discard.pop();
                //cout << "p1 used a card: " << p1_card.mValue << endl;
                p1_card.face_down = false; 
                swap(play_1[p1_hand][p1_card.mValue-1], p1_card);
                p1_card.face_down = true;
              }
              else if (play_1[p1_hand][p1_card.mValue-1].mValue == 11) { // We will switch
                discard.pop();
                //cout << "p1 switched the card: " << p1_card.mValue << endl;
                p1_card.face_down = false; 
                swap(play_1[p1_hand][p1_card.mValue-1], p1_card);
                p1_card.face_down = true;
              }
              else { // we draw from teh deck
                p1_card = vDeck.back();
                vDeck.pop_back();
                //cout << "p1 drawed a card: " << p1_card.mValue << endl;
              }

              if (won(play_1, p1_hand, p1_hand_size)) {
                if (T == 0) {
                  T += 1;
                  current_winner = 1;
                }
                else if (current_winner == 2) {
                  T += 1;
                  current_winner = 1;
                }
                  
                //cout << "**We have a winning array. for Player 1**" << endl;
                // put all the cards in the discard pile and hand into the deck
                reload(play_1, p1_hand, p1_hand_size, discard);
                p1_hand++;
                p1_hand_size--; 
                tDeal(play_1, p1_hand, p1_hand_size);
                discard.push(vDeck.back());
                vDeck.pop_back();
              }
            }
          } // end o the if we can use the card statement
          else { // Else we will draw a card
            p1_card = vDeck.back();
            vDeck.pop_back();
            //cout << "p1 we drew a card:" << p1_card.mValue << endl;
          }
          start_turn = false;
        } // end the of the first turn loop
        else if (!start_turn) { // we have a card in hand and are trying to switch it with a card in the array
          // DOn't worry about discrad pile / we have a card in hand
          if (p1_card.mValue <= p1_hand_size || p1_card.mValue == 11) {
            // check if we can use it
            // check joker
            if (p1_card.mValue == 11) {
              bool used_j = false;
              // put in the next open spot
              //cout << 431 << endl;
              vector<int> jack = check(play_1, play_2, p1_hand, p2_hand, p1_hand_size, p2_hand_size, discard, p1_hand_size);
              // find the maximum occurance
              int max = 0;
              for (int i=0; i < jack.size();++i) {
                if (jack.at(i) > max) {
                  max = jack.at(i);
                }
              }

               //cout << " fuck you." << endl;
               //for (int i = 0; i < p1_hand_size; ++i ) cout << play_1[p1_hand][i].mValue << " ";
               //cout << endl;
               // else need to check 
               while (!used_j) {
                 //cout << max << endl;
                 for (int i = 0; i < jack.size(); ++i) {
                   if (jack.at(i) == max && play_1[p1_hand][i].face_down) {
                     p1_card.face_down = false;
                     //cout << "before: " << p1_card.mValue << endl;
                     swap(play_1[p1_hand][i], p1_card);
                     used_j = true;
                     p1_card.face_down = true;
                     //ncout << "after: " << p1_card.mValue << endl;
                     break;
                   } 
                 }
                 --max;
               }
            
              if (won(play_1, p1_hand, p1_hand_size)) {
                if (T == 0) {
                  T += 1;
                  current_winner = 1;
                }
                else if (current_winner == 2) {
                  T += 1;
                  current_winner = 1;
                }
                  
                //cout << "**We have a winning array. for Player 1**" << endl;
                // put all the cards in the discard pile and hand into the deck
                reload(play_1, p1_hand, p1_hand_size, discard);
                p1_hand++;
                p1_hand_size--; 
                tDeal(play_1, p1_hand, p1_hand_size);
                discard.push(vDeck.back());
                vDeck.pop_back();
              } 
            }
            else { // else check if we can put in right spot
              if (play_1[p1_hand][p1_card.mValue-1].face_down) {
                //cout << "turn 2 p1 card in right place: " << p1_card.mValue << endl;
                p1_card.face_down = false; 
                swap(play_1[p1_hand][p1_card.mValue-1], p1_card);
                p1_card.face_down = true;
              }
              else if (play_1[p1_hand][p1_card.mValue-1].mValue == 11) { // We will switch
                //cout << "turn 2 p1 we swapped a jack" << p1_card.mValue << endl;
                p1_card.face_down = false; 
                swap(play_1[p1_hand][p1_card.mValue-1], p1_card);
                p1_card.face_down = true;
              }
              else {
                discard.push(p1_card);
                //cout << "turn 2 p1 usless card." << p1_card.mValue << endl;
                break;
              }

              if (won(play_1, p1_hand, p1_hand_size)) {
                if (T == 0) {
                  T += 1;
                  current_winner = 1;
                }
                else if (current_winner == 2) {
                  T += 1;
                  current_winner = 1;
                }
                  
                //cout << "**We have a winning array. for Player 1**" << endl;
                // put all the cards in the discard pile and hand into the deck
                reload(play_1, p1_hand, p1_hand_size, discard);
                p1_hand++;
                p1_hand_size--; 
                tDeal(play_1, p1_hand, p1_hand_size);
                discard.push(vDeck.back());
                vDeck.pop_back();
              } 
             // }
            }
          } // end o the if we can use the card statement
          else { // Else we will draw a card
            discard.push(p1_card);
            //cout << "turn 2 THe usless card is " << p1_card.mValue << endl;
            break;
          }
        }
      } // End player 1 while loop

      //cout << "PLayer ones turn is over." << endl << endl;
      // We need to check if player 1 has a full array
      
      ++N;

      // reinitialize the discard pile
      // We put the top discard in our hand for referance
      p2_card = discard.top();
      start_turn = true;
      // loop for player 2 turn
      while (true) {
        // We start the turn
        if (start_turn) {
          // Check if we can use the discard card
          if (p2_card.mValue <= p2_hand_size || p2_card.mValue == 11) {
            // check if we can use it
            // check joker
            if (p2_card.mValue == 11) {
              bool used_j = false;
              // put in the next open spot
              //cout << 550 << endl;
              vector<int> jack = check(play_1, play_2, p1_hand, p2_hand, p1_hand_size, p2_hand_size, discard, p2_hand_size);
              // find the maximum occurance
              int max = 0;
              for (int i=0; i < jack.size();++i) {
                if (jack.at(i) > max) {
                  max = jack.at(i);
                }
              }

               // else need to check 
               while (!used_j) {
                 for (int i = 0; i < jack.size(); ++i) {
                   if (jack.at(i) == max && play_2[p2_hand][i].face_down) {
                     p2_card.face_down = false;
                     swap(play_2[p2_hand][i], p2_card);
                     used_j = true;
                     p2_card.face_down = true;
                     break;
                   } 
                 }
                 --max;
               }
              if (won(play_2, p2_hand, p2_hand_size)) {
                if (T == 0) {
                  T += 1;
                  current_winner = 2;
                }
                else if (current_winner == 1) {
                  T += 1;
                  current_winner = 2;
                }
                  
                //cout << "**We have a winning array. for Player 2**" << endl;
                // put all the cards in the discard pile and hand into the deck
                reload(play_2, p2_hand, p2_hand_size, discard);
                p2_hand++;
                p2_hand_size--; 
                tDeal(play_2, p2_hand, p2_hand_size);
                discard.push(vDeck.back());
                vDeck.pop_back();
              }
            }
            else { // else check if we can put in right spot
              if (play_2[p2_hand][p2_card.mValue-1].face_down) {
                discard.pop();
                //cout << "p2 used a card: " << p2_card.mValue << endl;
                p2_card.face_down = false; 
                swap(play_2[p2_hand][p2_card.mValue-1], p2_card);
                p2_card.face_down = true;
              }
              else if (play_2[p2_hand][p2_card.mValue-1].mValue == 11) { // We will switch
                discard.pop();
                //cout << "p2 switched the card: " << p2_card.mValue << endl;
                p2_card.face_down = false; 
                swap(play_2[p2_hand][p2_card.mValue-1], p2_card);
                p2_card.face_down = true;
              }
              else { // we draw from teh deck
                p2_card = vDeck.back();
                vDeck.pop_back();
                //cout << "p2 drawed a card: " << p2_card.mValue << endl;
              }
              if (won(play_2, p2_hand, p2_hand_size)) {
                if (T == 0) {
                  T += 1;
                  current_winner = 2;
                }
                else if (current_winner == 1) {
                  T += 1;
                  current_winner = 2;
                }
                  
                //cout << "**We have a winning array. for Player 2**" << endl;
                // put all the cards in the discard pile and hand into the deck
                reload(play_2, p2_hand, p2_hand_size, discard);
                p2_hand++;
                p2_hand_size--; 
                tDeal(play_2, p2_hand, p2_hand_size);
                discard.push(vDeck.back());
                vDeck.pop_back();
              }
            }
          } // end o the if we can use the card statement
          else { // Else we will draw a card
            p2_card = vDeck.back();
            vDeck.pop_back();
            //cout << "p2 drawed a card: " << p2_card.mValue << endl;
          }
          start_turn = false;
        } // end the of the first turn loop
        else if (!start_turn) { // we have a card in hand and are trying to switch it with a card in the array
          // DOn't worry about discrad pile / we have a card in hand
          if (p2_card.mValue <= p2_hand_size || p2_card.mValue == 11) {
            // check if we can use it
            // check joker
            if (p2_card.mValue == 11) {
              bool used_j = false;
              // put in the next open spot
              //cout << 649 << endl;
              vector<int> jack = check(play_1, play_2, p1_hand, p2_hand, p1_hand_size, p2_hand_size, discard, p2_hand_size);
              // find the maximum occurance
              //cout << 652 << endl;
              int max = 0;
              for (int i=0; i < jack.size();++i) {
                if (jack.at(i) > max) {
                  max = jack.at(i);
                  //cout << i << endl;
                }
              }
              //cout << 660 << endl;

             //cout << max << endl;
               // else need to check 
               while (!used_j) {
                // cout << "p2 turn 2 loop" << endl;
                // cout << max << endl;
                 for (int i = 0; i < jack.size(); ++i) {
                   if (jack.at(i) == max && play_2[p2_hand][i].face_down) {
                     p2_card.face_down = false;
                     //cout << "Before: " << p2_card.mValue << endl;
                     swap(play_2[p2_hand][i], p2_card);
                     used_j = true;
                     p2_card.face_down = true;
                     //cout << "after: " << p2_card.mValue << endl;
                     break;
                   } 
                 }
                 --max;
               }
              if (won(play_2, p2_hand, p2_hand_size)) {
                if (T == 0) {
                  T += 1;
                  current_winner = 2;
                }
                else if (current_winner == 1) {
                  T += 1;
                  current_winner = 2;
                }
                  
                //cout << "**We have a winning array. for Player 2**" << endl;
                // put all the cards in the discard pile and hand into the deck
                reload(play_2, p2_hand, p2_hand_size, discard);
                p2_hand++;
                p2_hand_size--; 
                tDeal(play_2, p2_hand, p2_hand_size);
                discard.push(vDeck.back());
                vDeck.pop_back();
              }
            }
            else { // else check if we can put in right spot
              if (play_2[p2_hand][p2_card.mValue-1].face_down) {
                //cout << "turn 2 p2 card in right place: " << p2_card.mValue << endl;
                p2_card.face_down = false; 
                swap(play_2[p2_hand][p2_card.mValue-1], p2_card);
                p2_card.face_down = true;
                //cout << p2_card.mValue << endl;
              }
              else if (play_2[p2_hand][p2_card.mValue-1].mValue == 11) { // We will switch
                //cout << "turn 2 p2 we swapped a jack" << p2_card.mValue << endl;
                p2_card.face_down = false; 
                swap(play_2[p2_hand][p2_card.mValue-1], p2_card);
                p2_card.face_down = true;
              }
              else {
                discard.push(p2_card);
                //cout << "turn 2 p2 usless card." << p2_card.mValue << endl;
                break;
              }

              if (won(play_2, p2_hand, p2_hand_size)) {
                if (T == 0) {
                  T += 1;
                  current_winner = 2;
                }
                else if (current_winner == 1) {
                  T += 1;
                  current_winner = 2;
                }
                  
                //cout << "**We have a winning array. for Player 2**" << endl;
                // put all the cards in the discard pile and hand into the deck
                reload(play_2, p2_hand, p2_hand_size, discard);
                p2_hand++;
                p2_hand_size--; 
                tDeal(play_2, p2_hand, p2_hand_size);
                discard.push(vDeck.back());
                vDeck.pop_back();
              }
            }
          } // end o the if we can use the card statement
          else { // Else we will draw a card
            discard.push(p2_card);
            //cout << "turn 2 p2 usless card." << p2_card.mValue << endl;
            break;
          }
        }
      }
      ++N;
      //cout << "PLayer 2 turn is over." << endl;
    //} // end the current hand while loop


  } // end the game loop
}

int main(int argc, char** argv) {
  string game = argv[1];
  file = argv[2];
  ifstream inFile;

  inFile.open(file.c_str());

  if (!inFile) {
    cerr << "File does not exist" << endl;
    return 1;
  }

  inFile >> r;

  if (game == "war") {
    sim_war();
  }
  else if (game == "trash") {
    sim_trash();
  }
  
  inFile.close();

  cout << "OUTPUT " << game << " turns " << N << " transitions " << T << " last " << (double)T/N << endl;
  return 0;
}
