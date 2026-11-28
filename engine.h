#ifndef ENGINE_H
#define ENGINE_H

#include "characters.h"
#include <fstream>
#include <cstdlib>
#include <ctime>

/*
 * Game state enumeration tracking the primary lifecycle states of the engine.
 */
enum GameState
{
    Menu,
    Playing,
    Won,
    Lost,
    Exit
};

/*
 * Menu options available on the main landing screen.
 */
enum MenuChoice
{
    New_Game     = 1,
    Leader_Board = 2,
    About        = 3,
    Rules        = 4,
    Quit         = 5
};

/*
 * Data structures used for caching raw records parsed from text files.
 */
struct LocationRecord
{
    string placeName;
    int    boardPos;
    char   letters[7];
};

struct CharacterRecord
{
    string name;
    string role;
    string nationality;
    int    boardPos;
    char   letters[7];
};

struct ClueRecord
{
    string culpritRole;
    string word1;
    string word2;
};

/*
 * TextUtil: Static utility methods for string inspection and sanitization.
 * Implemented manually without standard C-string or ctype libraries.
 */
class TextUtil
{
public:
    static bool   isLetter(char c);
    static bool   isDigit(char c);
    static char   toUpperChar(char c);
    static string toUpperText(string text);
    static string trimText(string text);
    static bool   isAllLetters(string text);
    static bool   isAllDigits(string text);
    static int    digitsToNumber(string text);
    static string normalizeRole(string role);
    static string underscoresToSpaces(string text);
    static int    splitLine(string line, char separator,
                            string parts[], int maxParts);
};

/*
 * ScoreBoard: Handles file persistence and display of game session histories.
 */
class ScoreBoard
{
public:
    ScoreBoard();
    void   append(string name, string result, int credibility,
                  int turnsLeft, string culpritRole, string date);
    void   display() const;
    string getTodayDate() const;
};

/*
 * GameEngine: Singleton controller orchestrating state, game loop, data loading,
 * and user interactions across The Locker Heist.
 */
class GameEngine
{
public:
    static const int    LOCATION_COUNT    = 7;
    static const int    CHARACTER_COUNT   = 8;
    static const int    SUSPECT_COUNT     = 7;
    static const int    WORD1_LENGTH      = 7;
    static const int    WORD2_LENGTH      = 8;
    static const int    DEFAULT_MAX_TURNS = 25;
    static const int    DEFAULT_CREDIBILITY = 100;
    static const int    DEFAULT_WRONG_TURN  = 5;
    static const int    DEFAULT_WRONG_WORD  = 10;
    static const string ROLE_NAMES[7];

private:
    GameState state;
    bool      dataLoaded;
    int       culpritIndex;
    string    culpritRole;

    int    maxTurns;
    int    startCredibility;
    int    wrongTurnAmount;
    int    wrongWordAmount;
    string winText;
    string loseText;

    LocationRecord  locationTable[7];
    CharacterRecord characterTable[8];
    ClueRecord      clueTable[7];
    string          aboutText;

    Detective  *detective;
    Cottage    *cottage;
    Scrabble   *scrabble;
    TileRack   *rack;
    Character  *characters[8];

    ScoreBoard *scoreBoard;

    // Singleton design: private constructor and non-copyable declarations
    GameEngine();
    GameEngine(const GameEngine&);
    GameEngine& operator=(const GameEngine&);

    // Dynamic file parsing and initializers
    void loadAllData();
    void loadRules();
    void loadCharacters();
    void loadLocations();
    void loadClues();
    void loadAbout();

    // Console input handlers
    string readLine();
    int    readNumber(int min, int max);
    bool   readYesNo();
    void   clearScreen();
    void   pause();

    // User interface rendering
    void showTitle();
    void showRules();
    void mainMenuStep();
    void gameMenuStep();
    void showStatus();
    void showAccusationDialogue();

    // Core gameplay workflows
    void visitLocation();
    void interviewPerson();
    void viewBoard();
    void placeClue(Clue c);
    void formWord();
    void revealCulprit();
    void accuse();

    string askDetectiveName();
    void   createCharacters();
    int    findClueRecord(string role);
    void   startNewGame();
    void   clearGame();
    void   checkLoseConditions();
    void   spendTurn();
    void   endGame();

public:
    ~GameEngine();
    static GameEngine& getInstance();
    void       attachScoreBoard(ScoreBoard* board);
    void       handleException(GameException& e);
    GameState  getState()  const;
    void       setState(GameState s);
    void       runStep();
    void       ensureDataLoaded();

    // Queries and diagnostic accessors
    bool          isDataLoaded()          const;
    int           getMaxTurns()           const;
    int           getStartCredibility()   const;
    int           getWrongTurnAmount()    const;
    int           getWrongWordAmount()    const;
    string        getWinText()            const;
    string        getLoseText()           const;
    string        getAboutText()          const;
    const LocationRecord&  getLocationRecord(int index)  const;
    const CharacterRecord& getCharacterRecord(int index) const;
    const ClueRecord&      getClueRecord(int index)      const;
    int       getCulpritIndex() const;
    string    getCulpritRole()  const;
    Detective*  getDetective()  const;
    Cottage*    getCottage()    const;
    Scrabble*   getScrabble()   const;
    TileRack*   getRack()       const;
    Character*  getCharacter(int index) const;
};

#endif // ENGINE_H
