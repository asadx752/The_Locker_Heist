#ifndef BOARDS_H
#define BOARDS_H
#include <iostream>
#include <string>
using namespace std;

//  Constants
const int WORD_ONE_LENGTH = 7;   // Word 1 comes from the 7 locations
const int WORD_TWO_LENGTH = 8;   // Word 2 comes from the 8 characters
const int NAME_SIZE       = 40;  // max characters in a location name
const int MESSAGE_SIZE    = 100; // max characters in an exception message



enum WordRow
{
    Word_1   = 1,
    Word_2   = 2,
    WORD_ONE = 1,
    WORD_TWO = 2
};

//  Helper functions for char arrays

int  stringLength(const char *text);
void copyString(char *destination, const char *source, int maxSize);
char toUpperCase(char c);
bool isLetter(char c);
bool sameTextIgnoreCase(const char *a, const char *b);

//  Exception handling

class GameException
{
protected:
    string message;

    // Static penalty values set by Member C's engine at startup
    static int wrongTurnAmount;
    static int wrongWordAmount;

public:
    // Constructor accepting a string (used by Member C's exceptions)
    GameException(string msg);

    // Constructor accepting a C-string (used by Member B's exceptions)
    GameException(const char *text = "Game error");

    virtual ~GameException();

    // Returns the exception message
    virtual string getMessage() const;

    // Returns the credibility penalty for this exception type
    virtual int getPenalty() const;

    // Returns true for DataException (fatal errors)
    virtual bool isFatal() const;

    // Called by engine at startup to set penalty amounts from rules.txt
    static void setPenalties(int turnAmount, int wordAmount);
};

// Thrown when a player submits a wrong word guess
class InvalidWordException : public GameException
{
public:
    InvalidWordException(string msg);
    virtual int getPenalty() const;
};

// Thrown when attempting to add a tile to a full tile rack
class RackFullException : public GameException
{
public:
    RackFullException(string msg);
};

// Thrown when placing a tile at an out-of-range or occupied position
class InvalidPositionException : public GameException
{
public:
    InvalidPositionException(string msg);
};

// Thrown when player enters invalid input in menus or prompts
class InvalidInputException : public GameException
{
public:
    InvalidInputException(string msg);
    virtual int getPenalty() const;
};

// Thrown when an interview is refused due to low credibility
class InvalidActionException : public GameException
{
public:
    InvalidActionException(string msg);
};

// Thrown when data files are missing or corrupted
class DataException : public GameException
{
public:
    DataException(string msg);
    virtual bool isFatal() const;
};

// Member B's specific exception classes
class IndexOutOfRangeException : public GameException
{
public:
    IndexOutOfRangeException(const char *text = "Index is out of range!");
};

// InvalidPlacementException inherits from InvalidPositionException
// (compatible with both Member B's throw and Member C's catch)
class InvalidPlacementException : public InvalidPositionException
{
public:
    InvalidPlacementException(const char *text = "Letter cannot be placed there!");
};

class IncompleteWordException : public GameException
{
public:
    IncompleteWordException(const char *text = "Word is incomplete!");
};

//  Struct: Tile

struct Tile
{
    bool filled;
    char letter;   // '_' while empty, uppercase letter when placed
    int  word;     // 1 or 2 (which word row)
    int  position; // 1-based position inside the word

    Tile() : filled(false), letter('_'), word(0), position(0) {}

    friend istream &operator>>(istream &in, Tile &tile);
    friend ostream &operator<<(ostream &out, const Tile &tile);
};

//  Struct: Clue
struct Clue
{
    char letter;   // the revealed letter ('_' while unset)
    int  word;     // 1 = Word 1, 2 = Word 2
    int  position; // 1-based position inside the word

    Clue() : letter('_'), word(1), position(0) {}

    Clue(char let, int pos, int w) : letter(let), word(w), position(pos) {}
};

//  Struct: Location

struct Location
{
    char name[NAME_SIZE]; // location display name
    char letter;          // the clue letter hidden here
    int  word;            // 1 = Word 1 (location clues always go to Word 1)
    int  position;        // 1-based position inside the word
    bool searched;        // true once the player has searched here

    Location() : letter('_'), word(1), position(0), searched(false)
    {
        name[0] = '\0';
    }

    friend istream &operator>>(istream &in, Location &place);
    friend ostream &operator<<(ostream &out, const Location &place);
};

Clue     makeClue(char letter, int word, int position);
Location makeLocation(const char *name, char letter, int word, int position);

// Stream operator for Clue
ostream &operator<<(ostream &out, const Clue &clue);

// Equality operators: two tiles (or clues) are equal when their letters match
bool operator==(const Tile &a, const Tile &b);
bool operator==(const Clue &a, const Clue &b);

template <class T>
class DynamicArray
{
private:
    T   *data;
    int  size;
    int  capacity;

    void grow()
    {
        int  newCapacity = capacity * 2;
        T   *bigger      = new T[newCapacity];
        for (int i = 0; i < size; i++)
        {
            bigger[i] = data[i];
        }
        delete[] data;
        data     = bigger;
        capacity = newCapacity;
    }

public:
    DynamicArray(int startCapacity = 4)
    {
        if (startCapacity < 1)
        {
            startCapacity = 1;
        }
        capacity = startCapacity;
        size     = 0;
        data     = new T[capacity];
    }

    DynamicArray(const DynamicArray<T> &other)
    {
        capacity = other.capacity;
        size     = other.size;
        data     = new T[capacity];
        for (int i = 0; i < size; i++)
        {
            data[i] = other.data[i];
        }
    }

    DynamicArray<T> &operator=(const DynamicArray<T> &other)
    {
        if (this == &other)
        {
            return *this;
        }
        delete[] data;
        capacity = other.capacity;
        size     = other.size;
        data     = new T[capacity];
        for (int i = 0; i < size; i++)
        {
            data[i] = other.data[i];
        }
        return *this;
    }

    ~DynamicArray()
    {
        delete[] data;
        data = NULL;  
    }

    void add(const T &item)
    {
        if (size == capacity)
        {
            grow();
        }
        data[size] = item;
        size++;
    }

    T &operator[](int index)
    {
        if (index < 0 || index >= size)
        {
            throw IndexOutOfRangeException();
        }
        return data[index];
    }

    const T &operator[](int index) const
    {
        if (index < 0 || index >= size)
        {
            throw IndexOutOfRangeException();
        }
        return data[index];
    }

    int  getSize()  const { return size; }
    bool isEmpty()  const { return size == 0; }
    void clear()          { size = 0; }
};

class TileRack
{
private:
    DynamicArray<Tile> tiles;
    int capacity; // maximum allowed tiles (0 = unlimited)

public:
    TileRack(int cap = 0);  // cap=0 means unlimited; engine passes 15

    bool      add(const Clue &clue);    // false if same position already stored
    TileRack &operator+=(const Clue &clue);
    string    operator+(const TileRack &other) const; // both racks joined by a space
    bool      contains(char letter) const;
    int       getCount() const;
    const Tile &operator[](int index) const;
    void      clear();
    void      display() const;

    friend ostream &operator<<(ostream &out, const TileRack &rack);
};

class Scrabble
{
private:
    Tile  *row1;         // dynamic array of WORD_ONE_LENGTH tiles
    Tile  *row2;         // dynamic array of WORD_TWO_LENGTH tiles
    string targetWord1;  // Member C extension: answer for Word 1
    string targetWord2;  // Member C extension: answer for Word 2
    bool   row1Solved;   // Member C extension: true after correct guess
    bool   row2Solved;   // Member C extension

    Tile *rowPointer(int word) const;
    int   rowLength(int word)  const;
    void  resetRows();
    void  copyRowsFrom(const Scrabble &other);

public:
    Scrabble();
    Scrabble(const Scrabble &other);            // deep copy
    Scrabble &operator=(const Scrabble &other); // deep assignment
    ~Scrabble();

    // ---- Member B's original interface ----
    void     place(const Clue &clue);
    Scrabble &operator+=(const Clue &clue);

    int  filledCount(WordRow row)   const;
    bool isRowComplete(WordRow row) const;
    bool isBoardComplete()          const;

    void formWord(WordRow row, char *out, int outSize) const;
    bool matchesWord(WordRow row, const char *expected) const;

    void reset();
    void display() const;

    friend ostream &operator<<(ostream &out, const Scrabble &board);
    friend istream &operator>>(istream &in, Scrabble &board);

    // ---- Member C extensions ----
    void setTargetWords(string word1, string word2);
    bool isRowSolved(WordRow row) const;
    void checkWord(WordRow row, string guess) const;
    void markRowSolved(WordRow row);
};

class Cottage
{
private:
    DynamicArray<Location> locations;

public:
    Cottage();

    // ---- Member B's original interface ----
    void            addLocation(const Location &place);
    int             getCount()                     const;
    const Location &getLocation(int index)         const;
    bool            isSearched(int index)          const;
    Clue            search(int index);
    bool            allSearched()                  const;
    void            resetSearched();
    void            display()                      const;

    friend ostream &operator<<(ostream &out, const Cottage &cottage);

    // ---- Member C extensions ----
    int    getLocationCount()       const;
    string getLocationName(int index) const;
    void   showMenu()               const;
    Clue   showClueAgain(int index) const;
};

#endif // BOARDS_H
