#include "engine.h"

// Constant suspect role names used for culprit identification and index mapping
const string GameEngine::ROLE_NAMES[7] = {
    "Cook",
    "Guard",
    "Servant",
    "Partner 1",
    "Partner 2",
    "PA",
    "Child"
};

/*
 * ScoreBoard implementation:
 * Writes match summaries to leader/scoreBoard.txt and displays past attempts.
 */

ScoreBoard::ScoreBoard()
{
}

void ScoreBoard::append(string name, string result, int credibility,
                        int turnsLeft, string culpritRole, string date)
{
    bool hasHeader = false;
    ifstream checkFile("leader/scoreBoard.txt");
    if (checkFile.is_open())
    {
        string headerLine;
        if (getline(checkFile, headerLine))
        {
            if (TextUtil::trimText(headerLine).length() > 0)
            {
                hasHeader = true;
            }
        }
        checkFile.close();
    }

    ofstream outFile("leader/scoreBoard.txt", ios::app);
    if (!outFile.is_open())
    {
        throw DataException("Cannot write the score board.");
    }

    if (!hasHeader)
    {
        outFile << "Detective|Result|Credibility|Turns_Left|Culprit|Date\n";
    }

    // Convert spaces to underscores for consistent delimiter-based parsing
    string underscoredRole = "";
    for (int i = 0; i < (int)culpritRole.length(); i++)
    {
        if (culpritRole[i] == ' ')
        {
            underscoredRole = underscoredRole + '_';
        }
        else
        {
            underscoredRole = underscoredRole + culpritRole[i];
        }
    }

    outFile << name << "|" << result << "|" << credibility << "|";
    if (turnsLeft < 10)
    {
        outFile << "0";
    }
    outFile << turnsLeft << "|" << underscoredRole << "|" << date << "\n";
    outFile.close();
}

void ScoreBoard::display() const
{
    ifstream inFile("leader/scoreBoard.txt");
    if (!inFile.is_open())
    {
        cout << "No scores yet." << endl;
        return;
    }

    string line;
    if (!getline(inFile, line))
    {
        cout << "No scores yet." << endl;
        inFile.close();
        return;
    }

    int    validRows = 0;
    string parts[7];

    while (getline(inFile, line))
    {
        line = TextUtil::trimText(line);
        if (line.length() == 0)
        {
            continue;
        }

        int count = TextUtil::splitLine(line, '|', parts, 6);
        if (count != 6)
        {
            continue;
        }

        if (validRows == 0)
        {
            cout << "Detective\tResult\tCredibility\tTurns_Left\tCulprit\tDate" << endl;
            cout << "------------------------------------------------------------------" << endl;
        }

        cout << parts[0] << "\t" << parts[1] << "\t" << parts[2] << "\t\t"
             << parts[3] << "\t\t" << parts[4] << "\t" << parts[5] << endl;
        validRows++;
    }

    inFile.close();

    if (validRows == 0)
    {
        cout << "No scores yet." << endl;
    }
}

string ScoreBoard::getTodayDate() const
{
    time_t now = time(NULL);
    tm    *ltm = localtime(&now);

    int day   = ltm->tm_mday;
    int month = ltm->tm_mon + 1;
    int year  = (ltm->tm_year + 1900) % 100;

    string dateStr = "";
    dateStr = dateStr + (char)('0' + (day   / 10)) + (char)('0' + (day   % 10));
    dateStr = dateStr + '-';
    dateStr = dateStr + (char)('0' + (month / 10)) + (char)('0' + (month % 10));
    dateStr = dateStr + '-';
    dateStr = dateStr + (char)('0' + (year  / 10)) + (char)('0' + (year  % 10));

    return dateStr;
}

/*
 * TextUtil: string manipulation without relying on standard C libraries.
 */

bool TextUtil::isLetter(char c)
{
    return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z');
}

bool TextUtil::isDigit(char c)
{
    return c >= '0' && c <= '9';
}

char TextUtil::toUpperChar(char c)
{
    if (c >= 'a' && c <= 'z')
    {
        return (char)(c - 'a' + 'A');
    }
    return c;
}

string TextUtil::toUpperText(string text)
{
    string result = "";
    for (int i = 0; i < (int)text.length(); i++)
    {
        result = result + toUpperChar(text[i]);
    }
    return result;
}

string TextUtil::trimText(string text)
{
    int start = 0;
    int len   = (int)text.length();

    while (start < len &&
           (text[start] == ' '  || text[start] == '\t' ||
            text[start] == '\r' || text[start] == '\n'))
    {
        start++;
    }

    int end = len - 1;
    while (end >= start &&
           (text[end] == ' '  || text[end] == '\t' ||
            text[end] == '\r' || text[end] == '\n'))
    {
        end--;
    }

    string result = "";
    for (int i = start; i <= end; i++)
    {
        result = result + text[i];
    }
    return result;
}

bool TextUtil::isAllLetters(string text)
{
    if (text.length() == 0)
    {
        return false;
    }
    for (int i = 0; i < (int)text.length(); i++)
    {
        if (!isLetter(text[i]))
        {
            return false;
        }
    }
    return true;
}

bool TextUtil::isAllDigits(string text)
{
    if (text.length() == 0)
    {
        return false;
    }
    for (int i = 0; i < (int)text.length(); i++)
    {
        if (!isDigit(text[i]))
        {
            return false;
        }
    }
    return true;
}

int TextUtil::digitsToNumber(string text)
{
    int value = 0;
    for (int i = 0; i < (int)text.length(); i++)
    {
        value = value * 10 + (text[i] - '0');
    }
    return value;
}

string TextUtil::normalizeRole(string role)
{
    string result = "";
    for (int i = 0; i < (int)role.length(); i++)
    {
        char ch = role[i];
        if (ch == '_')
        {
            result = result + ' ';
        }
        else
        {
            result = result + toUpperChar(ch);
        }
    }
    return trimText(result);
}

string TextUtil::underscoresToSpaces(string text)
{
    string result = "";
    for (int i = 0; i < (int)text.length(); i++)
    {
        if (text[i] == '_')
        {
            result = result + ' ';
        }
        else
        {
            result = result + text[i];
        }
    }
    return result;
}

int TextUtil::splitLine(string line, char separator,
                        string parts[], int maxParts)
{
    if (line.length() == 0)
    {
        return 0;
    }

    int    count   = 0;
    string current = "";

    for (int i = 0; i < (int)line.length(); i++)
    {
        char ch = line[i];
        if (ch == separator)
        {
            if (count < maxParts)
            {
                parts[count] = trimText(current);
            }
            count++;
            current = "";
        }
        else
        {
            current = current + ch;
        }
    }

    if (count < maxParts)
    {
        parts[count] = trimText(current);
    }
    count++;

    if (count > maxParts)
    {
        return maxParts + 1;
    }

    return count;
}

/*
 * GameEngine singleton initialization and lifecycle control.
 */

GameEngine::GameEngine()
{
    state        = Menu;
    dataLoaded   = false;
    culpritIndex = -1;
    culpritRole  = "";

    maxTurns         = DEFAULT_MAX_TURNS;
    startCredibility = DEFAULT_CREDIBILITY;
    wrongTurnAmount  = DEFAULT_WRONG_TURN;
    wrongWordAmount  = DEFAULT_WRONG_WORD;
    winText          = "Accuse_Correct_Culprit";
    loseText         = "Credibility_Zero_Or_Turns_Exhausted";
    aboutText        = "";

    detective = NULL;
    cottage   = NULL;
    scrabble  = NULL;
    rack      = NULL;
    for (int i = 0; i < CHARACTER_COUNT; i++)
    {
        characters[i] = NULL;
    }
    scoreBoard = NULL;
}

GameEngine::~GameEngine()
{
    clearGame();
}

GameEngine& GameEngine::getInstance()
{
    static GameEngine instance;
    return instance;
}

void GameEngine::attachScoreBoard(ScoreBoard* board)
{
    scoreBoard = board;
}

void GameEngine::handleException(GameException& e)
{
    cout << e.getMessage() << endl;
    if (e.isFatal())
    {
        cout << "The game cannot continue." << endl;
        state = Exit;
        return;
    }

    if (state == Playing && e.getPenalty() > 0)
    {
        if (detective != NULL)
        {
            detective->loseCredibility(e.getPenalty());
            cout << "Credibility -" << e.getPenalty()
                 << " (now " << detective->getCredibility() << ")" << endl;
        }
    }

    if (state == Playing)
    {
        checkLoseConditions();
    }
}

GameState GameEngine::getState() const
{
    return state;
}

void GameEngine::setState(GameState s)
{
    state = s;
}

void GameEngine::ensureDataLoaded()
{
    if (!dataLoaded)
    {
        loadAllData();
    }
}

void GameEngine::runStep()
{
    if (!dataLoaded)
    {
        loadAllData();
        showTitle();
    }
    else if (state == Menu)
    {
        mainMenuStep();
    }
    else if (state == Playing)
    {
        gameMenuStep();
    }
    else if (state == Won || state == Lost)
    {
        endGame();
    }
}

/*
 * Console input reading and validation routines.
 */

string GameEngine::readLine()
{
    string line;
    if (!getline(cin, line))
    {
        state = Exit;
        throw InvalidInputException("Input closed.");
    }
    return TextUtil::trimText(line);
}

int GameEngine::readNumber(int min, int max)
{
    string line = readLine();
    if (!TextUtil::isAllDigits(line) || line.length() > 3)
    {
        throw InvalidInputException("Invalid choice.");
    }
    int num = TextUtil::digitsToNumber(line);
    if (num < min || num > max)
    {
        throw InvalidInputException("Invalid choice.");
    }
    return num;
}

bool GameEngine::readYesNo()
{
    string line = readLine();
    if (line == "Y" || line == "y")
    {
        return true;
    }
    if (line == "N" || line == "n")
    {
        return false;
    }
    throw InvalidInputException("Please answer Y or N.");
}

void GameEngine::clearScreen()
{
    system("cls");
}

void GameEngine::pause()
{
    cout << "Press Enter to continue...";
    readLine();
}

/*
 * UI and menu display helpers.
 */

void GameEngine::showTitle()
{
    cout << aboutText << endl;
}

void GameEngine::showRules()
{
    cout << "====== THE LOCKER HEIST - RULES ======" << endl;
    cout << "Maximum Turns: "       << maxTurns        << endl;
    cout << "Starting Credibility: " << startCredibility << endl;
    cout << "Wrong Turn Penalty: -" << wrongTurnAmount  << " Credibility" << endl;
    cout << "Wrong Word Penalty: -" << wrongWordAmount  << " Credibility" << endl;
    cout << "Win Condition: "       << winText          << endl;
    cout << "Lose Condition: "      << loseText         << endl;
    cout << "======================================" << endl;
}

void GameEngine::mainMenuStep()
{
    cout << "\n=== THE LOCKER HEIST - MAIN MENU ===" << endl;
    cout << "[1] New Game"                 << endl;
    cout << "[2] View Leader/Score Board"  << endl;
    cout << "[3] About"                    << endl;
    cout << "[4] Rules"                    << endl;
    cout << "[5] Exit"                     << endl;
    cout << "Choice: ";

    int        choice     = readNumber(1, 5);
    MenuChoice menuChoice = (MenuChoice)choice;

    switch (menuChoice)
    {
    case New_Game:
        startNewGame();
        break;
    case Leader_Board:
        if (scoreBoard != NULL)
        {
            scoreBoard->display();
        }
        else
        {
            cout << "No scores yet." << endl;
        }
        pause();
        break;
    case About:
        cout << aboutText << endl;
        pause();
        break;
    case Rules:
        showRules();
        pause();
        break;
    case Quit:
        state = Exit;
        break;
    }
}

void GameEngine::showStatus()
{
    if (detective != NULL)
    {
        cout << "Credibility: " << detective->getCredibility()
             << " | Turns used: " << detective->getTurnsUsed() << endl;
    }
    if (rack != NULL)
    {
        rack->display();
    }
    if (scrabble != NULL)
    {
        scrabble->display();
    }
}

void GameEngine::gameMenuStep()
{
    showStatus();
    cout << "\n=== Game Menu ===" << endl;
    cout << "[1] Visit a Location"          << endl;
    cout << "[2] Interview a Person"         << endl;
    cout << "[3] View Scrabble Board (free)" << endl;
    cout << "[4] Form a Word"                << endl;
    cout << "[5] Accuse a Suspect"           << endl;
    cout << "[6] Quit"                       << endl;
    cout << "\n--- TURN " << (detective != NULL
                              ? detective->getTurnsUsed() + 1
                              : 1) << " ---" << endl;
    cout << "Choice: ";

    int choice = readNumber(1, 6);
    switch (choice)
    {
    case 1:
        visitLocation();
        break;
    case 2:
        interviewPerson();
        break;
    case 3:
        viewBoard();
        break;
    case 4:
        formWord();
        break;
    case 5:
        accuse();
        break;
    case 6:
        state = Menu;
        clearScreen();
        break;
    }

    checkLoseConditions();
}

/*
 * Gameplay mechanics: turn deduction, location visits, suspect questioning,
 * word resolution, and accusation flow.
 */

void GameEngine::spendTurn()
{
    if (detective != NULL)
    {
        detective->useTurn();
    }
}

void GameEngine::visitLocation()
{
    if (cottage == NULL)
    {
        return;
    }

    cottage->showMenu();
    cout << "Choice: ";
    int choice = readNumber(0, cottage->getLocationCount());
    if (choice == 0)
    {
        return;
    }

    int    index     = choice - 1;
    string placeName = cottage->getLocationName(index);

    if (!cottage->isSearched(index))
    {
        cout << "You have NOT searched " << placeName << ". Search? (Y/N): ";
        if (readYesNo())
        {
            Clue c = cottage->search(index);
            placeClue(c);
            cout << "Letter " << c.letter << " collected at position "
                 << c.position << " of Word 1. Letter has been placed." << endl;
            spendTurn();
        }
    }
    else
    {
        cout << placeName << " has been searched. Search again? (Y/N): ";
        if (readYesNo())
        {
            Clue c = cottage->showClueAgain(index);
            cout << "Letter " << c.letter << " is at position "
                 << c.position << " of Word 1 (already placed)." << endl;
            spendTurn();
        }
    }
}

void GameEngine::interviewPerson()
{
    cout << "====== Character Menu ======" << endl;
    for (int i = 0; i < CHARACTER_COUNT; i++)
    {
        cout << "[" << (i + 1) << "] ";
        if (characters[i] != NULL)
        {
            characters[i]->display();
        }
        cout << endl;
    }
    cout << "[0] Back" << endl;
    cout << "Choice: ";

    int choice = readNumber(0, CHARACTER_COUNT);
    if (choice == 0)
    {
        return;
    }

    Character *ch = characters[choice - 1];
    if (ch == NULL)
    {
        return;
    }

    if (!ch->isInterviewed())
    {
        ch->checkAllowed(detective != NULL ? detective->getCredibility() : 100);

        cout << "You have NOT interviewed " << ch->getName()
             << " yet. Interview? (Y/N): ";
        if (readYesNo())
        {
            Clue c = ch->interview();
            placeClue(c);
            cout << "Letter " << c.letter << " collected at position "
                 << c.position << " of Word 2. Placed." << endl;
            spendTurn();
        }
    }
    else
    {
        cout << ch->getName() << " has been interviewed. Interview again? (Y/N): ";
        if (readYesNo())
        {
            Clue c = ch->showClueAgain();
            cout << "Letter " << c.letter << " is at position "
                 << c.position << " of Word 2 (already placed)." << endl;
            spendTurn();
        }
    }
}

void GameEngine::viewBoard()
{
    if (scrabble != NULL)
    {
        scrabble->display();
    }
    pause();
}

void GameEngine::placeClue(Clue c)
{
    if (rack != NULL)
    {
        rack->add(c);
    }
    if (scrabble != NULL)
    {
        scrabble->place(c);
    }
}

void GameEngine::formWord()
{
    if (scrabble == NULL)
    {
        return;
    }

    bool r1Solved = scrabble->isRowSolved(Word_1);
    bool r2Solved = scrabble->isRowSolved(Word_2);
    bool r1Ready  = scrabble->isRowComplete(Word_1) && !r1Solved;
    bool r2Ready  = scrabble->isRowComplete(Word_2) && !r2Solved;

    if (r1Solved && r2Solved)
    {
        throw InvalidInputException("Both words are already solved.");
    }
    if (!r1Ready && !r2Ready)
    {
        throw InvalidInputException("No row is complete yet.");
    }

    cout << "Which row? [1] Word 1  [2] Word 2  [0] Back" << endl;
    cout << "Choice: ";
    int choice = readNumber(0, 2);
    if (choice == 0)
    {
        return;
    }

    WordRow row = (choice == 1 ? Word_1 : Word_2);
    if (!(row == Word_1 ? r1Ready : r2Ready))
    {
        throw InvalidInputException("That row is not ready.");
    }

    int expectedLen = (row == Word_1 ? WORD1_LENGTH : WORD2_LENGTH);
    cout << "Type the word (0 to cancel): ";
    string guess = readLine();
    if (guess == "0")
    {
        return;
    }
    if (!TextUtil::isAllLetters(guess) || (int)guess.length() != expectedLen)
    {
        throw InvalidInputException(string("The word must have ")
                                    + (char)('0' + expectedLen) + " letters.");
    }

    spendTurn();
    scrabble->checkWord(row, TextUtil::toUpperText(guess));
    scrabble->markRowSolved(row);
    cout << "Word " << choice << " solved" << endl;

    if (scrabble->isRowSolved(Word_1) && scrabble->isRowSolved(Word_2))
    {
        revealCulprit();
    }
}

void GameEngine::revealCulprit()
{
    cout << "\nBoth words have been accepted." << endl;
    cout << "Review the available character profiles and accusation evidence before making your decision.\n" << endl;
}

void GameEngine::showAccusationDialogue()
{
    ifstream file("accusation_dialogue.txt");
    if (!file.is_open())
    {
        cout << "\n[Notice] Could not load accusation_dialogue.txt." << endl;
        return;
    }

    cout << "\n--------------------------------------------------------" << endl;
    cout << "       ACCUSATION STAGE: SUSPECT STATEMENTS & HINTS      " << endl;
    cout << "--------------------------------------------------------" << endl;

    string line;
    bool headerSkipped = false;
    string parts[5];
    int entriesLoaded = 0;

    while (getline(file, line))
    {
        line = TextUtil::trimText(line);
        if (line.length() == 0 || line[0] == '#')
        {
            continue;
        }

        if (!headerSkipped)
        {
            headerSkipped = true;
            continue;
        }

        int count = TextUtil::splitLine(line, '|', parts, 5);
        if (count == 5)
        {
            cout << "\n[" << parts[0] << " - " << parts[1] << "]" << endl;
            cout << "  Personality: " << parts[2] << endl;
            cout << "  Statement:   " << parts[3] << endl;
            cout << "  Hint:        " << parts[4] << endl;
            entriesLoaded++;
        }
    }

    file.close();

    if (entriesLoaded == 0)
    {
        cout << "No dialogue entries found." << endl;
    }

    cout << "\n--------------------------------------------------------\n" << endl;
}

void GameEngine::accuse()
{
    if (scrabble == NULL
        || !scrabble->isRowSolved(Word_1)
        || !scrabble->isRowSolved(Word_2))
    {
        throw InvalidInputException("Solve both words first.");
    }

    showAccusationDialogue();

    int suspectIndices[SUSPECT_COUNT];
    int count = 0;

    cout << "====== Suspect Menu ======" << endl;
    for (int i = 0; i < CHARACTER_COUNT; i++)
    {
        if (characters[i] != NULL && characters[i]->isSuspect())
        {
            suspectIndices[count] = i;
            cout << "[" << (count + 1) << "] " << characters[i]->getName()
                 << " (" << characters[i]->getRole() << ")" << endl;
            count++;
        }
    }
    cout << "[0] Back" << endl;
    cout << "Choice: ";

    int choice = readNumber(0, count);
    if (choice == 0)
    {
        return;
    }

    spendTurn();

    int        chosenIdx = suspectIndices[choice - 1];
    Character *ch        = characters[chosenIdx];

    if (ch != NULL && ch->isCulprit())
    {
        cout << "Correct!" << endl;
        state = Won;
    }
    else
    {
        if (detective != NULL)
        {
            detective->loseCredibility(wrongTurnAmount);
            cout << "Wrong accusation! Credibility -" << wrongTurnAmount
                 << " (now " << detective->getCredibility() << ")" << endl;
        }
    }
}

void GameEngine::checkLoseConditions()
{
    if (state != Playing || detective == NULL)
    {
        return;
    }
    if (detective->getCredibility() <= 0 || detective->getTurnsUsed() >= maxTurns)
    {
        state = Lost;
    }
}

void GameEngine::endGame()
{
    if (detective == NULL)
    {
        state = Menu;
        return;
    }

    string culpritInfo = "";
    for (int i = 0; i < CHARACTER_COUNT; i++)
    {
        if (characters[i] != NULL && characters[i]->isCulprit())
        {
            culpritInfo = characters[i]->getRole() + " " + characters[i]->getName();
            break;
        }
    }

    string resStr    = (state == Won ? "WIN" : "LOSE");
    int    turnsLeft = maxTurns - detective->getTurnsUsed();
    if (turnsLeft < 0)
    {
        turnsLeft = 0;
    }

    cout << "\n----------------------------------------" << endl;
    cout << detective->getName() << " " << resStr << "!" << endl;
    cout << "Final credibility: " << detective->getCredibility()
         << " | Turns used: "    << detective->getTurnsUsed()
         << " | Culprit: "       << culpritInfo << endl;
    cout << "----------------------------------------" << endl;

    if (scoreBoard != NULL)
    {
        scoreBoard->append(detective->getName(), resStr,
                           detective->getCredibility(),
                           turnsLeft, culpritRole,
                           scoreBoard->getTodayDate());
    }

    pause();
    clearScreen();
    state = Menu;
}

/*
 * Game session setup: name prompt, object instantiation, and culprit selection.
 */

string GameEngine::askDetectiveName()
{
    while (true)
    {
        cout << "Enter detective name: ";
        string name = readLine();

        if (name.length() == 0)
        {
            cout << "Name cannot be empty. Please try again." << endl;
            continue;
        }

        bool hasSpace = false;
        bool hasPipe  = false;
        for (int i = 0; i < (int)name.length(); i++)
        {
            if (name[i] == ' ')
            {
                hasSpace = true;
            }
            if (name[i] == '|')
            {
                hasPipe = true;
            }
        }

        if (hasSpace)
        {
            cout << "Use _ or . instead of spaces" << endl;
            continue;
        }

        if (hasPipe)
        {
            cout << "Name cannot contain '|'. Please try again." << endl;
            continue;
        }

        return name;
    }
}

int GameEngine::findClueRecord(string role)
{
    string target = TextUtil::normalizeRole(role);
    for (int i = 0; i < 7; i++)
    {
        if (TextUtil::normalizeRole(clueTable[i].culpritRole) == target)
        {
            return i;
        }
    }
    throw DataException("Clue record not found for culprit role.");
}

void GameEngine::createCharacters()
{
    string normCulprit    = TextUtil::normalizeRole(culpritRole);
    bool   culpritAssigned = false;

    for (int i = 0; i < CHARACTER_COUNT; i++)
    {
        const CharacterRecord& rec  = characterTable[i];
        string name        = rec.name;
        string role        = rec.role;
        string nationality = rec.nationality;
        char   letter      = rec.letters[culpritIndex];
        int    boardPos    = rec.boardPos;

        string normRole = TextUtil::normalizeRole(role);

        if (normRole == "VICTIM")
        {
            characters[i] = new Victim(name, role, nationality, letter, boardPos);
        }
        else if (normRole == "COOK")
        {
            characters[i] = new Cook(name, role, nationality, letter, boardPos);
        }
        else if (normRole == "GUARD")
        {
            characters[i] = new Guard(name, role, nationality, letter, boardPos);
        }
        else if (normRole == "SERVANT")
        {
            characters[i] = new Servant(name, role, nationality, letter, boardPos);
        }
        else if (normRole == "PARTNER 1" || normRole == "PARTNER 2")
        {
            characters[i] = new Partner(name, role, nationality, letter, boardPos);
        }
        else if (normRole == "PA")
        {
            characters[i] = new PA(name, role, nationality, letter, boardPos);
        }
        else if (normRole == "CHILD")
        {
            characters[i] = new Child(name, role, nationality, letter, boardPos);
        }
        else
        {
            throw DataException("characters.txt: unknown character role.");
        }

        if (normRole == normCulprit)
        {
            characters[i]->setCulprit(true);
            culpritAssigned = true;
        }
    }

    if (!culpritAssigned)
    {
        throw DataException("Failed to assign culprit character.");
    }
}

void GameEngine::clearGame()
{
    delete detective;
    delete cottage;
    delete scrabble;
    delete rack;
    detective = NULL;
    cottage   = NULL;
    scrabble  = NULL;
    rack      = NULL;
    for (int i = 0; i < CHARACTER_COUNT; i++)
    {
        delete characters[i];
        characters[i] = NULL;
    }
}

void GameEngine::startNewGame()
{
    clearGame();

    string detName = askDetectiveName();

    culpritIndex = rand() % 7;
    culpritRole  = ROLE_NAMES[culpritIndex];

    int clueIdx = findClueRecord(culpritRole);

    detective = new Detective(detName, startCredibility);

    cottage = new Cottage();
    for (int i = 0; i < LOCATION_COUNT; i++)
    {
        Location loc;
        copyString(loc.name, locationTable[i].placeName.c_str(), NAME_SIZE);
        loc.letter   = locationTable[i].letters[culpritIndex];
        loc.position = locationTable[i].boardPos;
        loc.word     = 1;
        loc.searched = false;
        cottage->addLocation(loc);
    }

    createCharacters();

    scrabble = new Scrabble();
    scrabble->setTargetWords(clueTable[clueIdx].word1,
                             clueTable[clueIdx].word2);

    rack = new TileRack(15);

    state = Playing;
    cout << "A culprit has been secretly chosen. The game begins!" << endl;
}

/*
 * File parsers: schema verification and loading for rules, characters,
 * locations, clues, and about files.
 */

static bool readNextNonEmptyLine(ifstream& fin, string& line)
{
    while (getline(fin, line))
    {
        line = TextUtil::trimText(line);
        if (line.length() > 0)
        {
            return true;
        }
    }
    return false;
}

static void validateFieldCount(const string& line, string parts[],
                               int expected, const string& errorMsg)
{
    if (TextUtil::splitLine(line, '|', parts, expected) != expected)
    {
        throw DataException(errorMsg);
    }
}

static void openDataFile(ifstream& fin, const string& file,
                         string parts[], int fieldCount)
{
    fin.open(file.c_str());
    if (!fin.is_open())
    {
        throw DataException("File missing: " + file);
    }
    string line;
    if (!readNextNonEmptyLine(fin, line))
    {
        throw DataException(file + ": missing header");
    }
    validateFieldCount(line, parts, fieldCount, file + ": invalid header");
}

static void readLetters(string parts[], int first, char out[], const string& file)
{
    for (int k = 0; k < 7; k++)
    {
        if (parts[first + k].length() != 1 || !TextUtil::isLetter(parts[first + k][0]))
        {
            throw DataException(file + ": invalid letter");
        }
        out[k] = TextUtil::toUpperChar(parts[first + k][0]);
    }
}

static int findRoleIndex(const string& normRole)
{
    for (int r = 0; r < 7; r++)
    {
        if (normRole == TextUtil::normalizeRole(GameEngine::ROLE_NAMES[r]))
        {
            return r;
        }
    }
    return -1;
}

static int parseRuleNumber(const string& val, bool& negative)
{
    negative = (val.length() > 0 && val[0] == '-');
    string digits = "";
    for (int i = (negative ? 1 : 0); i < (int)val.length(); i++)
    {
        digits += val[i];
    }
    return TextUtil::isAllDigits(digits) ? TextUtil::digitsToNumber(digits) : -1;
}

void GameEngine::loadRules()
{
    maxTurns         = DEFAULT_MAX_TURNS;
    startCredibility = DEFAULT_CREDIBILITY;
    wrongTurnAmount  = DEFAULT_WRONG_TURN;
    wrongWordAmount  = DEFAULT_WRONG_WORD;
    winText          = "Accuse_Correct_Culprit";
    loseText         = "Credibility_Zero_Or_Turns_Exhausted";

    ifstream fin("rules.txt");
    string line;
    string kv[2];
    while (fin.is_open() && readNextNonEmptyLine(fin, line))
    {
        if (TextUtil::splitLine(line, '=', kv, 2) != 2)
        {
            continue;
        }
        const string& key = kv[0];
        const string& val = kv[1];

        bool negative;
        int  num = parseRuleNumber(val, negative);

        if (key == "Max_Turns" && !negative && num > 0)
        {
            maxTurns = num;
        }
        else if (key == "Starting_Credibility" && !negative && num > 0)
        {
            startCredibility = num;
        }
        else if (key == "Every_Wrong_Turn" && num >= 0)
        {
            wrongTurnAmount = num;
        }
        else if (key == "Every_Wrong_Word" && num >= 0)
        {
            wrongWordAmount = num;
        }
        else if (key == "Win_Condition" && val.length() > 0)
        {
            winText = val;
        }
        else if (key == "Lose_Condition" && val.length() > 0)
        {
            loseText = val;
        }
    }

    GameException::setPenalties(wrongTurnAmount, wrongWordAmount);
}

void GameEngine::loadCharacters()
{
    const string file = "characters.txt";
    ifstream fin;
    string   parts[13];
    string   line;
    openDataFile(fin, file, parts, 12);

    int  rowCount         = 0;
    bool usedPositions[9] = { false };
    bool roleFound[7]     = { false };
    bool victimFound      = false;

    while (readNextNonEmptyLine(fin, line))
    {
        if (rowCount >= CHARACTER_COUNT)
        {
            throw DataException(file + ": too many rows");
        }
        validateFieldCount(line, parts, 12, file + ": wrong field count");

        if (parts[0].length() == 0 || parts[1].length() == 0 || parts[2].length() == 0)
        {
            throw DataException(file + ": empty name, role, or nationality");
        }
        if (!TextUtil::isAllDigits(parts[3]))
        {
            throw DataException(file + ": invalid board position");
        }
        int pos = TextUtil::digitsToNumber(parts[3]);
        if (pos < 1 || pos > 8 || usedPositions[pos])
        {
            throw DataException(file + ": duplicate board position");
        }
        usedPositions[pos] = true;

        readLetters(parts, 4, characterTable[rowCount].letters, file);

        string normRole = TextUtil::normalizeRole(parts[1]);
        if (normRole == "VICTIM")
        {
            if (victimFound)
            {
                throw DataException(file + ": duplicate victim");
            }
            victimFound = true;
        }
        else
        {
            int roleIdx = findRoleIndex(normRole);
            if (roleIdx == -1 || roleFound[roleIdx])
            {
                throw DataException(file + ": invalid or duplicate role");
            }
            roleFound[roleIdx] = true;
        }

        characterTable[rowCount].name        = TextUtil::underscoresToSpaces(parts[0]);
        characterTable[rowCount].role        = TextUtil::underscoresToSpaces(parts[1]);
        characterTable[rowCount].nationality = TextUtil::underscoresToSpaces(parts[2]);
        characterTable[rowCount].boardPos    = pos;
        rowCount++;
    }

    if (rowCount != CHARACTER_COUNT || !victimFound)
    {
        throw DataException(file + ": wrong row count");
    }
    for (int r = 0; r < 7; r++)
    {
        if (!roleFound[r])
        {
            throw DataException(file + ": missing role");
        }
    }
}

void GameEngine::loadLocations()
{
    const string file = "locations.txt";
    ifstream fin;
    string   parts[10];
    string   line;
    openDataFile(fin, file, parts, 9);

    int  rowCount         = 0;
    bool usedPositions[8] = { false };

    while (readNextNonEmptyLine(fin, line))
    {
        if (rowCount >= LOCATION_COUNT)
        {
            throw DataException(file + ": too many rows");
        }
        validateFieldCount(line, parts, 9, file + ": wrong field count");

        if (parts[0].length() == 0)
        {
            throw DataException(file + ": empty location name");
        }
        if (!TextUtil::isAllDigits(parts[1]))
        {
            throw DataException(file + ": invalid board position");
        }
        int pos = TextUtil::digitsToNumber(parts[1]);
        if (pos < 1 || pos > 7 || usedPositions[pos])
        {
            throw DataException(file + ": duplicate board position");
        }
        usedPositions[pos] = true;

        readLetters(parts, 2, locationTable[rowCount].letters, file);

        locationTable[rowCount].placeName = TextUtil::underscoresToSpaces(parts[0]);
        locationTable[rowCount].boardPos  = pos;
        rowCount++;
    }

    if (rowCount != LOCATION_COUNT)
    {
        throw DataException(file + ": wrong row count");
    }
    for (int p = 1; p <= 7; p++)
    {
        if (!usedPositions[p])
        {
            throw DataException(file + ": missing board position");
        }
    }
}

void GameEngine::loadClues()
{
    const string file = "clue.txt";
    ifstream fin;
    string   parts[4];
    string   line;
    openDataFile(fin, file, parts, 3);

    int  rowCount     = 0;
    bool roleFound[7] = { false };

    while (readNextNonEmptyLine(fin, line))
    {
        if (rowCount >= 7)
        {
            throw DataException(file + ": too many rows");
        }
        validateFieldCount(line, parts, 3, file + ": wrong field count");

        int roleIdx = findRoleIndex(TextUtil::normalizeRole(parts[0]));
        if (roleIdx == -1 || roleFound[roleIdx])
        {
            throw DataException(file + ": invalid or duplicate role");
        }
        roleFound[roleIdx] = true;

        if (parts[1].length() != 7 || !TextUtil::isAllLetters(parts[1]))
        {
            throw DataException(file + ": invalid Word 1 length or letters");
        }
        if (parts[2].length() != 8 || !TextUtil::isAllLetters(parts[2]))
        {
            throw DataException(file + ": invalid Word 2 length or letters");
        }

        clueTable[rowCount].culpritRole = TextUtil::underscoresToSpaces(parts[0]);
        clueTable[rowCount].word1       = TextUtil::toUpperText(parts[1]);
        clueTable[rowCount].word2       = TextUtil::toUpperText(parts[2]);
        rowCount++;
    }

    if (rowCount != 7)
    {
        throw DataException(file + ": wrong row count");
    }
    for (int r = 0; r < 7; r++)
    {
        if (!roleFound[r])
        {
            throw DataException(file + ": missing clue role");
        }
    }
}

void GameEngine::loadAbout()
{
    ifstream fin("about.txt");
    if (!fin.is_open())
    {
        throw DataException("File missing: about.txt");
    }

    aboutText = "";
    string line;
    while (getline(fin, line))
    {
        aboutText = aboutText + line + "\n";
    }

    fin.close();

    if (aboutText.length() == 0)
    {
        throw DataException("about.txt: empty file");
    }
}

void GameEngine::loadAllData()
{
    loadRules();
    loadCharacters();
    loadLocations();
    loadClues();
    loadAbout();
    srand((unsigned int)time(NULL));
    dataLoaded = true;
}

/*
 * Public inspection and accessor methods.
 */

bool GameEngine::isDataLoaded() const { return dataLoaded; }
int  GameEngine::getMaxTurns()  const { return maxTurns; }
int  GameEngine::getStartCredibility()  const { return startCredibility; }
int  GameEngine::getWrongTurnAmount()   const { return wrongTurnAmount; }
int  GameEngine::getWrongWordAmount()   const { return wrongWordAmount; }
string GameEngine::getWinText()  const { return winText; }
string GameEngine::getLoseText() const { return loseText; }
string GameEngine::getAboutText() const { return aboutText; }

const LocationRecord&  GameEngine::getLocationRecord(int index)  const
{
    return locationTable[index];
}

const CharacterRecord& GameEngine::getCharacterRecord(int index) const
{
    return characterTable[index];
}

const ClueRecord& GameEngine::getClueRecord(int index) const
{
    return clueTable[index];
}

int    GameEngine::getCulpritIndex() const { return culpritIndex; }
string GameEngine::getCulpritRole()  const { return culpritRole; }

Detective* GameEngine::getDetective() const { return detective; }
Cottage*   GameEngine::getCottage()   const { return cottage; }
Scrabble*  GameEngine::getScrabble()  const { return scrabble; }
TileRack*  GameEngine::getRack()      const { return rack; }

Character* GameEngine::getCharacter(int index) const
{
    if (index >= 0 && index < CHARACTER_COUNT)
    {
        return characters[index];
    }
    return NULL;
}
