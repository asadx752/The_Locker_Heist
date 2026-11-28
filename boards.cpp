#include "boards.h"

//  Static member initialisation for GameException penalties
int GameException::wrongTurnAmount = 5;
int GameException::wrongWordAmount = 10;
//  GameException base class

GameException::GameException(string msg)
{
    message = msg;
}

GameException::GameException(const char *text)
{
    message = string(text);
}

GameException::~GameException()
{
}

string GameException::getMessage() const
{
    return "HEIST ERROR: " + message;
}

int GameException::getPenalty() const
{
    return 0;
}

bool GameException::isFatal() const
{
    return false;
}

void GameException::setPenalties(int turnAmount, int wordAmount)
{
    wrongTurnAmount = turnAmount;
    wrongWordAmount = wordAmount;
}

//  Derived exception classes

InvalidWordException::InvalidWordException(string msg)
    : GameException(msg)
{
}

int InvalidWordException::getPenalty() const
{
    return wrongWordAmount;
}

RackFullException::RackFullException(string msg)
    : GameException(msg)
{
}

InvalidPositionException::InvalidPositionException(string msg)
    : GameException(msg)
{
}

InvalidInputException::InvalidInputException(string msg)
    : GameException(msg)
{
}

int InvalidInputException::getPenalty() const
{
    return wrongTurnAmount;
}

InvalidActionException::InvalidActionException(string msg)
    : GameException(msg)
{
}

DataException::DataException(string msg)
    : GameException(msg)
{
}

bool DataException::isFatal() const
{
    return true;
}

// Member B's exception classes
IndexOutOfRangeException::IndexOutOfRangeException(const char *text)
    : GameException(text)
{
}

InvalidPlacementException::InvalidPlacementException(const char *text)
    : InvalidPositionException(text)
{
}

IncompleteWordException::IncompleteWordException(const char *text)
    : GameException(text)
{
}

//  Helper functions for char arrays


// Counts characters until the '\0' that ends the text
int stringLength(const char *text)
{
    int count = 0;
    while (text[count] != '\0')
    {
        count++;
    }
    return count;
}

// Copies at most maxSize-1 characters and always ends with '\0'
void copyString(char *destination, const char *source, int maxSize)
{
    int i = 0;
    while (source[i] != '\0' && i < maxSize - 1)
    {
        destination[i] = source[i];
        i++;
    }
    destination[i] = '\0';
}

// Converts a lowercase letter to uppercase
char toUpperCase(char c)
{
    if (c >= 'a' && c <= 'z')
    {
        return (char)(c - 'a' + 'A');
    }
    return c;
}

// Returns true if the character is an ASCII letter
bool isLetter(char c)
{
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

// Case-insensitive comparison of two C-strings
bool sameTextIgnoreCase(const char *a, const char *b)
{
    int i = 0;
    while (a[i] != '\0' && b[i] != '\0')
    {
        if (toUpperCase(a[i]) != toUpperCase(b[i]))
        {
            return false;
        }
        i++;
    }
    return a[i] == '\0' && b[i] == '\0';
}

//  Struct builder functions

// Creates a Clue
Clue makeClue(char letter, int word, int position)
{
    Clue c;
    c.letter   = letter;
    c.word     = word;
    c.position = position;
    return c;
}

// Creates a Location and fills both Member B and Member C fields
Location makeLocation(const char *name, char letter, int word, int position)
{
    Location l;
    copyString(l.name, name, NAME_SIZE);
    l.letter   = letter;
    l.word     = word;
    l.position = position;
    l.searched = false;
    return l;
}

// Prints: Letter O collected at position 3 of Word 1.
ostream &operator<<(ostream &out, const Clue &clue)
{
    out << "Letter " << clue.letter
        << " collected at position " << clue.position
        << " of Word " << clue.word << ".";
    return out;
}

// Two clues are equal when they carry the same letter (case-ignored).
// Used to compare letters read from clue.txt with letters on the board.
bool operator==(const Clue &a, const Clue &b)
{
    return toUpperCase(a.letter) == toUpperCase(b.letter);
}

// Two tiles are equal when they carry the same letter (case-ignored).
// Empty tiles all hold '_' so they compare equal to each other.
bool operator==(const Tile &a, const Tile &b)
{
    return toUpperCase(a.letter) == toUpperCase(b.letter);
}

// Prints a tile as [X], or [ ] when it is empty.
// Used by the board and rack displays.
ostream &operator<<(ostream &out, const Tile &tile)
{
    out << '[' << (tile.filled ? tile.letter : ' ') << ']';
    return out;
}

// Reads one letter into a tile and marks it filled.
// Sets the failbit if the input is not a letter.
istream &operator>>(istream &in, Tile &tile)
{
    char c;
    if (in >> c && toUpperCase(c) >= 'A' && toUpperCase(c) <= 'Z')
    {
        tile.letter = toUpperCase(c);
        tile.filled = true;
    }
    else
    {
        in.setstate(ios::failbit);
    }
    return in;
}

// Prints a location as: Name (letter X, position N)
// Hides nothing beyond what the debugging view needs.
ostream &operator<<(ostream &out, const Location &place)
{
    out << place.name << " (letter " << place.letter
        << ", position " << place.position << ")";
    return out;
}

// Reads a location as: name letter position
// The name is one word (use _ for spaces).
istream &operator>>(istream &in, Location &place)
{
    string name;
    char   letter;
    int    position;
    if (in >> name >> letter >> position)
    {
        copyString(place.name, name.c_str(), NAME_SIZE);
        place.letter   = toUpperCase(letter);
        place.position = position;
    }
    return in;
}

//  TileRack

TileRack::TileRack(int cap)
{
    capacity = cap;
}

// Adds the clue as a tile.
// Returns false if the same word/position is already stored.
//  Throws RackFullException if capacity is set and the rack is full.
bool TileRack::add(const Clue &clue)
{
    int  w   = clue.word;
    int  pos = clue.position;
    char let = clue.letter;

    for (int i = 0; i < tiles.getSize(); i++)
    {
        if (tiles[i].word == w && tiles[i].position == pos)
        {
            return false; // duplicate
        }
    }

    if (capacity > 0 && tiles.getSize() >= capacity)
    {
        throw RackFullException("The rack is full.");
    }

    Tile t;
    t.letter   = toUpperCase(let);
    t.word     = w;
    t.position = pos;
    t.filled   = true;
    tiles.add(t);
    return true;
}

TileRack &TileRack::operator+=(const Clue &clue)
{
    add(clue);
    return *this;
}

// Joins this rack and another with one space between, as a single clue string.
// Example: rack "A B" + rack "C D" gives "AB CD".
string TileRack::operator+(const TileRack &other) const
{
    string merged = "";
    for (int i = 0; i < tiles.getSize(); i++)
    {
        merged += tiles[i].letter;
    }
    merged += ' ';
    for (int i = 0; i < other.tiles.getSize(); i++)
    {
        merged += other.tiles[i].letter;
    }
    return merged;
}

bool TileRack::contains(char letter) const
{
    char wanted = toUpperCase(letter);
    for (int i = 0; i < tiles.getSize(); i++)
    {
        if (tiles[i].letter == wanted)
        {
            return true;
        }
    }
    return false;
}

int TileRack::getCount() const
{
    return tiles.getSize();
}

const Tile &TileRack::operator[](int index) const
{
    return tiles[index]; 
}

void TileRack::clear()
{
    tiles.clear();
}

void TileRack::display() const
{
    cout << *this;
}

ostream &operator<<(ostream &out, const TileRack &rack)
{
    if (rack.tiles.isEmpty())
    {
        out << "Rack: (empty)" << endl;
    }
    else
    {
        out << "Rack: ";
        for (int i = 0; i < rack.tiles.getSize(); i++)
        {
            out << rack.tiles[i].letter << ' ';
        }
        out << endl;
    }
    return out;
}

//  Scrabble


Scrabble::Scrabble()
{
    row1      = new Tile[WORD_ONE_LENGTH];
    row2      = new Tile[WORD_TWO_LENGTH];
    targetWord1 = "";
    targetWord2 = "";
    row1Solved  = false;
    row2Solved  = false;
    resetRows();
}

// Deep copy constructor
Scrabble::Scrabble(const Scrabble &other)
{
    row1        = new Tile[WORD_ONE_LENGTH];
    row2        = new Tile[WORD_TWO_LENGTH];
    targetWord1 = other.targetWord1;
    targetWord2 = other.targetWord2;
    row1Solved  = other.row1Solved;
    row2Solved  = other.row2Solved;
    copyRowsFrom(other);
}

Scrabble &Scrabble::operator=(const Scrabble &other)
{
    if (this == &other)
    {
        return *this;
    }
    targetWord1 = other.targetWord1;
    targetWord2 = other.targetWord2;
    row1Solved  = other.row1Solved;
    row2Solved  = other.row2Solved;
    copyRowsFrom(other);
    return *this;
}

Scrabble::~Scrabble()
{
    delete[] row1;
    delete[] row2;
    row1 = NULL;
    row2 = NULL;
}

// Gives every square its word number, position, and an empty state
void Scrabble::resetRows()
{
    for (int i = 0; i < WORD_ONE_LENGTH; i++)
    {
        row1[i].letter   = '_';
        row1[i].word     = 1;
        row1[i].position = i + 1;
        row1[i].filled   = false;
    }
    for (int i = 0; i < WORD_TWO_LENGTH; i++)
    {
        row2[i].letter   = '_';
        row2[i].word     = 2;
        row2[i].position = i + 1;
        row2[i].filled   = false;
    }
}

void Scrabble::copyRowsFrom(const Scrabble &other)
{
    for (int i = 0; i < WORD_ONE_LENGTH; i++)
    {
        row1[i] = other.row1[i];
    }
    for (int i = 0; i < WORD_TWO_LENGTH; i++)
    {
        row2[i] = other.row2[i];
    }
}

Tile *Scrabble::rowPointer(int word) const
{
    if (word == WORD_ONE || word == 1)
    {
        return row1;
    }
    if (word == WORD_TWO || word == 2)
    {
        return row2;
    }
    throw InvalidPlacementException("Word number must be 1 or 2!");
}

int Scrabble::rowLength(int word) const
{
    if (word == WORD_ONE || word == 1)
    {
        return WORD_ONE_LENGTH;
    }
    if (word == WORD_TWO || word == 2)
    {
        return WORD_TWO_LENGTH;
    }
    throw InvalidPlacementException("Word number must be 1 or 2!");
}

// Puts the clue's letter on the board.
// Placing the same letter on the same square again is harmless.
void Scrabble::place(const Clue &clue)
{
    int  w   = clue.word;
    int  pos = clue.position;
    char let = clue.letter;

    int    length = rowLength(w);
    Tile  *row    = rowPointer(w);

    if (pos < 1 || pos > length)
    {
        throw InvalidPlacementException("Position is outside the board!");
    }

    if (!isLetter(let))
    {
        throw InvalidPlacementException("Only letters can be placed!");
    }

    Tile &square = row[pos - 1];
    char  upper  = toUpperCase(let);

    if (square.filled && square.letter != upper)
    {
        throw InvalidPlacementException("That square already holds a different letter!");
    }

    square.letter = upper;
    square.filled = true;
}

Scrabble &Scrabble::operator+=(const Clue &clue)
{
    place(clue);
    return *this;
}

int Scrabble::filledCount(WordRow row) const
{
    int    length = rowLength(row);
    Tile  *tiles  = rowPointer(row);
    int    count  = 0;
    for (int i = 0; i < length; i++)
    {
        if (tiles[i].filled)
        {
            count++;
        }
    }
    return count;
}

bool Scrabble::isRowComplete(WordRow row) const
{
    return filledCount(row) == rowLength(row);
}

bool Scrabble::isBoardComplete() const
{
    return isRowComplete(WORD_ONE) && isRowComplete(WORD_TWO);
}

// Copies the finished word into 'out'.
// Throws IncompleteWordException when some squares are still empty.
void Scrabble::formWord(WordRow row, char *out, int outSize) const
{
    int    length = rowLength(row);
    Tile  *tiles  = rowPointer(row);

    if (outSize < length + 1)
    {
        throw InvalidPlacementException("Output buffer is too small!");
    }

    if (!isRowComplete(row))
    {
        throw IncompleteWordException("Word is incomplete!");
    }

    for (int i = 0; i < length; i++)
    {
        out[i] = tiles[i].letter;
    }
    out[length] = '\0';
}

// True when the finished row equals 'expected' (ignoring case).
// Throws IncompleteWordException when the row is not finished.
bool Scrabble::matchesWord(WordRow row, const char *expected) const
{
    char built[WORD_TWO_LENGTH + 1];
    formWord(row, built, WORD_TWO_LENGTH + 1);
    return sameTextIgnoreCase(built, expected);
}

// ---- Member C extensions ----

void Scrabble::setTargetWords(string word1, string word2)
{
    targetWord1 = word1;
    targetWord2 = word2;
    row1Solved  = false;
    row2Solved  = false;
}

bool Scrabble::isRowSolved(WordRow row) const
{
    if (row == Word_1)
    {
        return row1Solved;
    }
    return row2Solved;
}

void Scrabble::checkWord(WordRow row, string guess) const
{
    string target = (row == Word_1 ? targetWord1 : targetWord2);
    if (!matchesWord(row, target.c_str()) || guess != target)
    {
        throw InvalidWordException("Wrong word!");
    }
}

void Scrabble::markRowSolved(WordRow row)
{
    if (row == Word_1)
    {
        row1Solved = true;
    }
    else
    {
        row2Solved = true;
    }
}

void Scrabble::reset()
{
    resetRows();
}

void Scrabble::display() const
{
    cout << *this;
}

// Reads a clue as: letter word position, and places it on the board.
// Sets the failbit when the numbers are missing.
istream &operator>>(istream &in, Scrabble &board)
{
    char letter;
    int  word;
    int  position;
    if (in >> letter >> word >> position)
    {
        board.place(Clue(letter, position, word));
    }
    return in;
}

// Prints: Word 1: [_][_][O][T][_][_][_]
ostream &operator<<(ostream &out, const Scrabble &board)
{
    out << "Scrabble Board:" << endl;
    out << "Word 1: ";
    for (int i = 0; i < WORD_ONE_LENGTH; i++)
    {
        if (board.row1[i].filled)
        {
            out << '[' << board.row1[i].letter << "] ";
        }
        else
        {
            out << "[ ] ";
        }
    }
    out << endl;

    out << "Word 2: ";
    for (int i = 0; i < WORD_TWO_LENGTH; i++)
    {
        if (board.row2[i].filled)
        {
            out << '[' << board.row2[i].letter << "] ";
        }
        else
        {
            out << "[ ] ";
        }
    }
    out << endl;
    return out;
}

//  Cottage


Cottage::Cottage()
{
}

void Cottage::addLocation(const Location &place)
{
    locations.add(place);
}

int Cottage::getCount() const
{
    return locations.getSize();
}

const Location &Cottage::getLocation(int index) const
{
    return locations[index]; 
}

bool Cottage::isSearched(int index) const
{
    return locations[index].searched;
}

// Marks the location as searched and returns its Clue.
Clue Cottage::search(int index)
{
    if (index < 0 || index >= locations.getSize())
    {
        throw IndexOutOfRangeException("Invalid location index.");
    }
    Location &place = locations[index];
    place.searched  = true;
    return makeClue(place.letter, place.word, place.position);
}

bool Cottage::allSearched() const
{
    for (int i = 0; i < locations.getSize(); i++)
    {
        if (!locations[i].searched)
        {
            return false;
        }
    }
    return true;
}

void Cottage::resetSearched()
{
    for (int i = 0; i < locations.getSize(); i++)
    {
        locations[i].searched = false;
    }
}

void Cottage::display() const
{
    cout << *this;
}

// ---- Member C extensions ----

int Cottage::getLocationCount() const
{
    return locations.getSize();
}

string Cottage::getLocationName(int index) const
{
    if (index >= 0 && index < locations.getSize())
    {
        return string(locations[index].name);
    }
    return "";
}

void Cottage::showMenu() const
{
    cout << "====== Location Menu ======" << endl;
    for (int i = 0; i < locations.getSize(); i++)
    {
        cout << "[" << (i + 1) << "] " << locations[i].name << " - "
             << (locations[i].searched ? "Searched" : "NOT Searched") << endl;
    }
    cout << "[0] Back" << endl;
}

Clue Cottage::showClueAgain(int index) const
{
    if (index < 0 || index >= locations.getSize())
    {
        throw IndexOutOfRangeException("Invalid location index.");
    }
    const Location &place = locations[index];
    return makeClue(place.letter, place.word, place.position);
}

ostream &operator<<(ostream &out, const Cottage &cottage)
{
    out << "Locations:" << endl;
    for (int i = 0; i < cottage.locations.getSize(); i++)
    {
        out << "  " << (i + 1) << ". " << cottage.locations[i].name;
        if (cottage.locations[i].searched)
        {
            out << "  (searched)";
        }
        out << endl;
    }
    return out;
}
