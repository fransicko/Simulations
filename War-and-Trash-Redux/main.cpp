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
vector<Card> vDeck; // Trash deck

ifstream inFile; // The file
vector<Card> hand_1;
vector<Card> hand_2;
double r = 0; // comments later
string file = "";
int N = 0; // Total number of turns
int T = 0; // the player winning the game transitioned four times
int L = 0; // The last turn in which the game was played
int current_winner = 0;

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
//    cout << "Its the p." << endl;
    if (inFile.eof()) {
      cerr << "Reached end of file." << endl;
      exit(1);
    }
    inFile >> r;
    int p = r*(52-c) + c;
//    cout << "P: " << p << ", R: " << r << ", C: " << c << endl;
    swap(deck[c], deck[p]);
    swap(vDeck.at(c), vDeck.at(p));
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
    if (inFile.eof()) {
      cerr << "Reached end of file." << endl;
      exit(1);
    }
    inFile >> r;
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
  //cout << "Its in war" << endl;
  initialize("war");
  //cout << "After initialize." << endl;
  shuffle();
  //cout << "After shuffle" << endl;
 
  vector<Card> pile_1;
  vector<Card> pile_2; 
  vector<Card> draw_pile;

  dealHands();

  // the start of the game
  while (true) {
  while (hand_1.size() != 0  && hand_2.size() != 0 ) {
    //cout << "Hand 1 size: " << hand_1.size() << " Hand 2 size: " << hand_2.size() << endl;
    //cout << "Pile 1 size: " << pile_1.size() << " Pile 2 size: " << pile_2.size() << endl;
    if (hand_1.back().mValue > hand_2.back().mValue) { // THis is player 1 win
      pile_1.push_back(hand_1.back());
      pile_1.push_back(hand_2.back());
      
      //cout << "P1 wins" << endl;
      pile_1.insert(pile_1.end(), draw_pile.begin(), draw_pile.end());

      hand_1.pop_back();
      hand_2.pop_back();
      draw_pile.clear();

      ++N;
    // Who's winning
    if (hand_1.size() + pile_1.size() > hand_2.size() + pile_2.size() ) {
      if (T == 0) {
        T += 1;
	current_winner = 1;
	L = N;
      }
      else if (current_winner == 2) {
        T += 1;
        current_winner = 1;
	L = N;
      }  
    } 
    else if (hand_2.size() + pile_2.size() > hand_1.size() + pile_1.size() ) {
      if (T == 0) {
        T += 1;
	current_winner = 2;
	L = N;
      }
      else if (current_winner == 1) {
        T += 1;
        current_winner = 2;
	L = N;
      }  
    }
    }
    else if (hand_2.back().mValue > hand_1.back().mValue) { // THis is player 2 win
      pile_2.push_back(hand_1.back());
      pile_2.push_back(hand_2.back());

      //cout << "P2 wins." << endl;
      pile_2.insert(pile_2.end(), draw_pile.begin(), draw_pile.end());

      hand_1.pop_back();
      hand_2.pop_back();
      draw_pile.clear();

     // Who's winning
    if (hand_1.size() + pile_1.size() > hand_2.size() + pile_2.size() ) {
      if (T == 0) {
        T += 1;
	current_winner = 1;
        L = N;
      }
      else if (current_winner == 2) {
        T += 1;
        current_winner = 1;
        L = N;
      }  
    } 
    else if (hand_2.size() + pile_2.size() > hand_1.size() + pile_1.size() ) {
      if (T == 0) {
        T += 1;
	current_winner = 2;
        L = N;
      }
      else if (current_winner == 1) {
        T += 1;
        current_winner = 2;
        L = N;
      }  
    }     
    ++N;

    }
    else { // draw
      //cout << "Draw" << endl;
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

      //cout << "\n*************************************************8" << endl;
      //cout << "This is Player 1's hand before shuffle" << endl;
      for (int i = 0; i < hand_1.size(); ++i) {
        //cout << hand_1.at(i).mValue << " ";
      }
      

      hand_shuffle(1);

      //cout << "\nThis is Player 1's hand after shuffle" << endl;
      for (int i = 0; i < hand_1.size(); ++i) {
        //cout << hand_1.at(i).mValue << " ";
      }
      //cout << "\n*************************************************8\n" << endl;
      // Player 1 out of cards
          
    }

    // Player 2 out of cards 
    if (hand_2.size() == 0 && pile_2.size() == 0) break;
    else if (hand_2.size() == 0) {
      hand_2.insert(hand_2.end(), pile_2.begin(), pile_2.end());
      pile_2.clear();
      hand_shuffle(2);
          
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
    if (inFile.eof()) {
      cerr << "Reached end of file." << endl;
      exit(1);
    }
    inFile >> r;
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
  while (p2_hand_size != 0 && p2_hand_size != 0) {
    // Put the top card into the stack

    // loop for the game
    //while (true) {
      // We put the top discard in our hand for referance
      p1_card = discard.top();
      bool start_turn = true;
      // loop for player 1 turn
      while (true) {
	if (p1_hand_size == 0) break; // Player one has won the game.
        //cout << "Player 1's" << endl;
	//cout << "Hand: " << p1_hand << " Hand size: " << p1_hand_size << endl;
        // We start the turn
        if (start_turn) {
          ++N;
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
		 //cout << "Player 1 turn 1 jack." << endl;
		 // Do we already have a face down card in the right position
		 for (int i = 0; i < p1_hand_size; ++i) {
                   if (play_1[p1_hand][i].face_down && play_1[p1_hand][i].mValue == (i+1)) {
                     p1_card.face_down = false;
                     swap(play_1[p1_hand][i], p1_card);
                     used_j = true;
                     p1_card.face_down = true;
                     break;
		   }
		 }
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
                /*if (T == 0) {
                  T += 1;
                  current_winner = 1;
		  L = N;
                }
                else if (current_winner == 2) {
                  T += 1;
                  current_winner = 1;
		  L = N;
                }
		*/
                  
                //cout << "**We have a winning array. for Player 1**" << endl;
                // put all the cards in the discard pile and hand into the deck
		//cout << "Discard size: " << discard.size() << endl;
		if (discard.size() == 0) {
                  reload(play_1, p1_hand, p1_hand_size, discard);
                  p1_hand++;
                  p1_hand_size--; 
                  tDeal(play_1, p1_hand, p1_hand_size);
		  discard.push(vDeck.back());
		  vDeck.pop_back();
		}
		else {
		  Card top_discard = discard.top();
		  discard.pop();
                  reload(play_1, p1_hand, p1_hand_size, discard);
                  p1_hand++;
                  p1_hand_size--; 
                  tDeal(play_1, p1_hand, p1_hand_size);
                  discard.push(top_discard);
		}
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
                /*if (T == 0) {
                  T += 1;
                  current_winner = 1;
		  L = N;
                }
                else if (current_winner == 2) {
                  T += 1;
                  current_winner = 1;
		  L = N;
                }
		*/
                  
                //cout << "**We have a winning array. for Player 1**" << endl;
                // put all the cards in the discard pile and hand into the deck
		//cout << "Discard size: " << discard.size() << endl;
		if (discard.size() == 0) {
                  reload(play_1, p1_hand, p1_hand_size, discard);
                  p1_hand++;
                  p1_hand_size--; 
                  tDeal(play_1, p1_hand, p1_hand_size);
		  discard.push(vDeck.back());
		  vDeck.pop_back();
		}
		else {
		  Card top_discard = discard.top();
		  discard.pop();
                  reload(play_1, p1_hand, p1_hand_size, discard);
                  p1_hand++;
                  p1_hand_size--; 
                  tDeal(play_1, p1_hand, p1_hand_size);
                  discard.push(top_discard);
		}
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
	/*
               cout << "Player 1's hand:" << endl;
               for (int i = 0; i < p1_hand_size; ++i ) cout << play_1[p1_hand][i].mValue << " "; 
	       cout << endl;
	       */
               // else need to check 
               while (!used_j) {
                 //cout << max << endl;
                 //cout << "Player 1 turn 2 jack" << endl;
		 for (int i = 0; i < p1_hand_size; ++i) {
                   if (play_1[p1_hand][i].face_down && play_1[p1_hand][i].mValue == (i+1)) {
                     p1_card.face_down = false;
                     swap(play_1[p1_hand][i], p1_card);
                     used_j = true;
                     p1_card.face_down = true;
                     break;
		   }
		 }
                 for (int i = 0; i < jack.size(); ++i) {
                   if (jack.at(i) == max && play_1[p1_hand][i].face_down) {
                     p1_card.face_down = false;
                     //cout << "Not first turn, before switch: " << p1_card.mValue << endl;
                     swap(play_1[p1_hand][i], p1_card);
                     used_j = true;
                     p1_card.face_down = true;
                     //cout << "Not first turn, after switch: " << p1_card.mValue << endl;
                     break;
                   } 
                 }
                 --max;
               }
            
              if (won(play_1, p1_hand, p1_hand_size)) {
                /*if (T == 0) {
                  T += 1;
                  current_winner = 1;
		  L = N;
                }
                else if (current_winner == 2) {
                  T += 1;
                  current_winner = 1;
		  L = N;
                }
		*/
                  
                //cout << "**We have a winning array. for Player 1**" << endl;
                // put all the cards in the discard pile and hand into the deck
		//cout << "Discard size: " << discard.size() << endl;
		if (discard.size() == 0) {
                  reload(play_1, p1_hand, p1_hand_size, discard);
                  p1_hand++;
                  p1_hand_size--; 
                  tDeal(play_1, p1_hand, p1_hand_size);
		  discard.push(vDeck.back());
		  vDeck.pop_back();
		}
		else {
		  Card top_discard = discard.top();
		  discard.pop();
                  reload(play_1, p1_hand, p1_hand_size, discard);
                  p1_hand++;
                  p1_hand_size--; 
                  tDeal(play_1, p1_hand, p1_hand_size);
                  discard.push(top_discard);
		}
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
                /*if (T == 0) {
                  T += 1;
                  current_winner = 1;
		  L = N;
                }
                else if (current_winner == 2) {
                  T += 1;
                  current_winner = 1;
		  L = N;
                }
		*/
                  
                //cout << "**We have a winning array. for Player 1**" << endl;
                // put all the cards in the discard pile and hand into the deck
		//cout << "Discard size: " << discard.size() << endl;
		if (discard.size() == 0) {
                  reload(play_1, p1_hand, p1_hand_size, discard);
                  p1_hand++;
                  p1_hand_size--; 
                  tDeal(play_1, p1_hand, p1_hand_size);
		  discard.push(vDeck.back());
		  vDeck.pop_back();
		}
		else {
		  Card top_discard = discard.top();
		  discard.pop();
                  reload(play_1, p1_hand, p1_hand_size, discard);
                  p1_hand++;
                  p1_hand_size--; 
                  tDeal(play_1, p1_hand, p1_hand_size);
                  discard.push(top_discard);
		}
              } 
             // }
            }
          } // end o the if we can use the card statement
          else { // Else we will discard the card
            discard.push(p1_card);
            //cout << "turn 2 THe usless card is " << p1_card.mValue << endl;
	    if (p1_hand > p2_hand) { // Player 1 has cleared more boards
               if (T == 0) {
                  T += 1;
                  current_winner = 1;
		  L = N;
                }
                else if (current_winner == 2) {
                  T += 1;
                  current_winner = 1;
		  L = N;
                }
	    }
	    else if (p1_hand == p2_hand) {
              int p1_flip = 0;
	      int p2_flip = 0;

	      for (int i = 0; i < p1_hand_size; ++i) {
                if (!play_1[p1_hand][i].face_down) p1_flip++;
		if (!play_2[p2_hand][i].face_down) p2_flip++;
	      }

	      if (p1_flip > p2_flip) { // Player 1 has more cards flipped over
	        if (T == 0) {
                  T += 1;
                  current_winner = 1;
		  L = N;
                }
                else if (current_winner == 2) {
                  T += 1;
                  current_winner = 1;
		  L = N;
                }
	      }
	    }
            break;
          }
        }
      } // End player 1 while loop



      //cout << "vDeck size: " << vDeck.size() << endl;
      //cout << endl;
      //cout << "**************************************************" << endl;
      //cout << "PLayer ones turn is over." << endl << endl;
      // We need to check if player 1 has a full array
      //cout << "Player 1's hand:" << endl;
      //for (int i = 0; i < p1_hand_size; ++i ) //cout << play_1[p1_hand][i].mValue << " ";
      //cout << endl;
      //cout << "**************************************************" << endl;

      if (p1_hand_size == 0) break;


      // reinitialize the discard pile
      // We put the top discard in our hand for referance
      p2_card = discard.top();
      start_turn = true;
      // loop for player 2 turn
      while (true) {
	if (p2_hand_size == 0) break;
        // We start the turn
	//cout << "Player 2" << endl;
        if (start_turn) {
	  ++N;
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
		 //cout << "PLayer 2 turn 1 Jack." << endl;
                 for (int i = 0; i < p2_hand_size; ++i) {
                   if (play_2[p2_hand][i].face_down && play_2[p2_hand][i].mValue == (i+1)) {
                     p2_card.face_down = false;
                     swap(play_2[p2_hand][i], p2_card);
                     used_j = true;
                     p2_card.face_down = true;
                     break;
		   }
		 }
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
                /*if (T == 0) {
                  T += 1;
                  current_winner = 2;
		  L = N;
                }
                else if (current_winner == 1) {
                  T += 1;
                  current_winner = 2;
		  L = N;
                }
		*/
                  
                //cout << "**We have a winning array. for Player 2**" << endl;
                // put all the cards in the discard pile and hand into the deck
		if (discard.size() == 0) {
                  reload(play_2, p2_hand, p2_hand_size, discard);
                  p2_hand++;
                  p2_hand_size--; 
                  tDeal(play_2, p2_hand, p2_hand_size);
		  discard.push(vDeck.back());
		  vDeck.pop_back();
		}
		else {
		  Card top_discard = discard.top();
		  discard.pop();
                  reload(play_2, p2_hand, p2_hand_size, discard);
                  p2_hand++;
                  p2_hand_size--; 
                  tDeal(play_2, p2_hand, p2_hand_size);
                  discard.push(top_discard);
		}
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
                /*if (T == 0) {
                  T += 1;
                  current_winner = 2;
		  L = N;
                }
                else if (current_winner == 1) {
                  T += 1;
                  current_winner = 2;
		  L = N;
                }
		*/
                  
                //cout << "**We have a winning array. for Player 2**" << endl;
                // put all the cards in the discard pile and hand into the deck
		if (discard.size() == 0) {
                  reload(play_2, p2_hand, p2_hand_size, discard);
                  p2_hand++;
                  p2_hand_size--; 
                  tDeal(play_2, p2_hand, p2_hand_size);
		  discard.push(vDeck.back());
		  vDeck.pop_back();
		}
		else {
		  Card top_discard = discard.top();
		  discard.pop();
                  reload(play_2, p2_hand, p2_hand_size, discard);
                  p2_hand++;
                  p2_hand_size--; 
                  tDeal(play_2, p2_hand, p2_hand_size);
                  discard.push(top_discard);
		}
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
                //cout << "p2 turn 2 jake loop" << endl;
                // cout << max << endl;
		for (int i = 0; i < p2_hand_size; ++i) {
                   if (play_2[p2_hand][i].face_down && play_2[p2_hand][i].mValue == (i+1)) {
                     p2_card.face_down = false;
                     swap(play_2[p2_hand][i], p2_card);
                     used_j = true;
                     p2_card.face_down = true;
                     break;
		   }
		 }
                 for (int i = 0; i < jack.size(); ++i) {
                   if (jack.at(i) == max && play_2[p2_hand][i].face_down) {
                     p2_card.face_down = false;
                     //cout << "Not first turn P2 Before switch: " << p2_card.mValue << endl;
                     swap(play_2[p2_hand][i], p2_card);
                     used_j = true;
                     p2_card.face_down = true;
                     //cout << "Not first turn P2 after switch: " << p2_card.mValue << endl;
                     break;
                   } 
                 }
                 --max;
               }
              if (won(play_2, p2_hand, p2_hand_size)) {
                /*if (T == 0) {
                  T += 1;
                  current_winner = 2;
		  L = N;
                }
                else if (current_winner == 1) {
                  T += 1;
                  current_winner = 2;
		  L = N;
                }
		*/
                  
                //cout << "**We have a winning array. for Player 2**" << endl;
                // put all the cards in the discard pile and hand into the deck
		if (discard.size() == 0) {
                  reload(play_2, p2_hand, p2_hand_size, discard);
                  p2_hand++;
                  p2_hand_size--; 
                  tDeal(play_2, p2_hand, p2_hand_size);
		  discard.push(vDeck.back());
		  vDeck.pop_back();
		}
		else {
		  Card top_discard = discard.top();
		  discard.pop();
                  reload(play_2, p2_hand, p2_hand_size, discard);
                  p2_hand++;
                  p2_hand_size--; 
                  tDeal(play_2, p2_hand, p2_hand_size);
                  discard.push(top_discard);
		}
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
                /*if (T == 0) {
                  T += 1;
                  current_winner = 2;
		  L = N;
                }
                else if (current_winner == 1) {
                  T += 1;
                  current_winner = 2;
		  L = N;
                }
		*/
                  
                //cout << "**We have a winning array. for Player 2**" << endl;
                // put all the cards in the discard pile and hand into the deck
		if (discard.size() == 0) {
                  reload(play_2, p2_hand, p2_hand_size, discard);
                  p2_hand++;
                  p2_hand_size--; 
                  tDeal(play_2, p2_hand, p2_hand_size);
		  discard.push(vDeck.back());
		  vDeck.pop_back();
		}
		else {
		  Card top_discard = discard.top();
		  discard.pop();
                  reload(play_2, p2_hand, p2_hand_size, discard);
                  p2_hand++;
                  p2_hand_size--; 
                  tDeal(play_2, p2_hand, p2_hand_size);
                  discard.push(top_discard);
		}
              }
            }
          } // end o the if we can use the card statement
          else { // Else we will discard the card
            discard.push(p2_card);
            //cout << "turn 2 p2 usless card." << p2_card.mValue << endl;
            if (p1_hand < p2_hand) { // Player 2 has cleared more boards
               if (T == 0) {
                  T += 1;
                  current_winner = 2;
		  L = N;
                }
                else if (current_winner == 1) {
                  T += 1;
                  current_winner = 2;
		  L = N;
                }
	    }
	    else if (p1_hand == p2_hand) {
              int p1_flip = 0;
	      int p2_flip = 0;

	      for (int i = 0; i < p1_hand_size; ++i) {
                if (!play_1[p1_hand][i].face_down) p1_flip++;
		if (!play_2[p2_hand][i].face_down) p2_flip++;
	      }

	      if (p1_flip < p2_flip) { // Player 2 has more cards flipped over
	        if (T == 0) {
                  T += 1;
                  current_winner = 2;
		  L = N;
                }
                else if (current_winner == 1) {
                  T += 1;
                  current_winner = 2;
		  L = N;
                }
	      }
	    }
            break;
          }
        }
      }

      //cout << "vDeck size: " << vDeck.size() << endl;
      //cout << endl;
      //cout << "**************************************************" << endl;
      //cout << "Player 2's hand:" << endl;
      //for (int i = 0; i < p2_hand_size; ++i ) cout << play_2[p2_hand][i].mValue << " ";
      //cout << endl;
      //cout << "**************************************************" << endl;
      //cout << "PLayer 2 turn is over." << endl;
      if (p2_hand_size == 0) break;
      //cout << "havent break yet" << endl;

    //} // end the current hand while loop


  } // end the game loop
  // CHeck for the last transition
  if (p1_hand_size == 0) {
    if (T == 0) {
      T += 1;
      current_winner = 1;
      L = N;
    }
    else if (current_winner == 2) {
      T += 1;
      current_winner = 1;
      L = N;
    }
  }
  else if (p2_hand_size == 0) {
    if (T == 0) {
      T += 1;
      current_winner = 2;
      L = N;
    }
    else if (current_winner == 1) {
      T += 1;
      current_winner = 2;
      L = N;
    }
  }
} // end of trash sim

int main(int argc, char** argv) {
  //cout << "Its in the main." << endl;
  string game = argv[1];
  file = argv[2];

  //if (file == "/dev/fd/63") exit(1);

  inFile.open(file.c_str());
  //cout << "file: " << file.c_str() << endl;
  if (!inFile) {
    cerr << file << " does not exist" << endl;
    exit(1);
  }

  inFile >> r;
  //cout << "Main R: " << r << endl;

  if (game == "war") {
    sim_war();
  }
  else if (game == "trash") {
    sim_trash();
  }
  else {
    cerr << "No game specified." << endl;
    exit(1);
  }
  
  inFile.close();

  cout << "OUTPUT " << game << " turns " << N << " transitions " << T << " last " << (double)L/N << endl;
  return 0;
}
