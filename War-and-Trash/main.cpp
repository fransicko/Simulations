// Our included files


// The card class
class Card {
public:
  enum Suit {
    SPADE, CLUB, DIAMOND, HEART
  };

  Card(char symbol, enum suit) { 
    this.symbol = symbol; 
    this.suit = suit;
  }

private:
  Card::char symbol; // This will be between
  Card::enum suit;
};


// this will initialize the deck with 
void initialize() { 

}

int main() {


  return 0;
}
