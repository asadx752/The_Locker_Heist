<div align="center"><h1>🕵️ THE LOCKER HEIST</h1><h3>OOP Semester Project · C++ · Visual Studio</h3><p><strong>A detective mystery experience, built from the ground up using Object-Oriented Programming.</strong></p></div>


<p align="center">
  <img src="https://img.shields.io/badge/Paradigm-Object%20Oriented-2563EB?style=for-the-badge" alt="OOP">
  <img src="https://img.shields.io/badge/Language-C%2B%2B-7C3AED?style=for-the-badge&logo=cplusplus&logoColor=white" alt="C++">
  <img src="https://img.shields.io/badge/Platform-Windows-16A34A?style=for-the-badge" alt="Windows">
  <img src="https://img.shields.io/badge/IDE-Visual%20Studio-F59E0B?style=for-the-badge" alt="Visual Studio">
</p><p align="center">
  <img src="https://img.shields.io/badge/Genre-Detective%20Mystery-E11D48?style=flat-square" alt="Detective Mystery">
  <img src="https://img.shields.io/badge/Mode-Single%20Player-0F766E?style=flat-square" alt="Single Player">
  <img src="https://img.shields.io/badge/Interface-Console-334155?style=flat-square" alt="Console">
	
</p>Investigate the scene. Collect clues. Decode the words. Unmask the culprit!


🎮 About the Game

The Locker Heist is a single-player detective mystery game developed for the Object-Oriented Programming (CL1004) semester project at FAST-NUCES.


During a party at a wealthy business owner's cottage, valuable documents and assets are stolen from a locker concealed behind a bookshelf. Hired by the owner's personal assistant, the player must investigate seven locations, interview eight people, collect fifteen letter clues and solve two connected word puzzles before running out of turns or credibility.

Built with C++, the project brings together abstraction, encapsulation, inheritance, polymorphism, dynamic memory allocation, templates, file handling and custom exception handling — all in a playable console-based mystery game.

👥 Team

**Asad Ali**	25F-0698	characters.h, characters.cpp, characters.txt

**Abdul Naffey**	25F-0527	boards.h, boards.cpp, clue.txt

**Moazzam Ali Jameel**	25F-0640	rules.txt, engine.cpp, engine.h, locations.txt, main.cpp, accusation_dialogue.txt, about.txt

✨ Features

🕹️ Main menu	New Game, Leader/Score Board, About, Rules and Exit

🏠 Investigation	Seven searchable cottage locations

👥 Characters	Eight interviewable people, including the victim and seven suspects

🎲 Replayability	Random culprit selection from seven predefined profiles

🔎 Clues	Fifteen letter clues with automatic board placement

🧩 Word puzzles	Two Scrabble-style rows containing seven and eight letters

🛡️ Credibility	Interview eligibility, limited turns and penalties for mistakes

💬 Accusation	Character statements, personalities and nationality hints

🏁 Game flow	Win/lose conditions and return to the main menu

🏆 Scoreboard	Persistent results stored in a text file

⚙️ Error handling	Custom exceptions for invalid actions, input, words and data


🧰 Technology Stack

	Game logic and Object-Oriented-Design
	Class Hierarchy, Polymorphism and Modular Design
	Development and Debugging
	Game data, rules and score history
	Collaboration and source management

💻 Requirements
- Visual Studio with the Desktop development with C++ workload, or a compatible C++ compiler.
- Windows is the intended environment (system("cls") is used for screen clearing).
- All seven C++ source/header files and six runtime text files kept together.
- A writable leader directory for score persistence.

🚀 Build & Run
1. Open the project in Visual Studio
Create an empty C++ project, add the four .cpp files and three .h files, and place the runtime text files in the project directory. Set the debugging working directory to the directory containing the data files. Create a leader folder there.
2. Compile the game
Build using Ctrl + Shift + B, or open the Visual Studio Developer Command Prompt in the project folder:
if not exist leader mkdir leader
cl /EHsc main.cpp engine.cpp characters.cpp boards.cpp /Fe:LockerHeist.exe
3. Run it
Press Ctrl + F5 in Visual Studio, or run:
LockerHeist.exe
Build using GCC / MinGW
if not exist leader mkdir leader
g++ -std=c++11 main.cpp engine.cpp characters.cpp boards.cpp -o LockerHeist.exe
LockerHeist.exe
Important: Keep the runtime data files in the working directory. Use lowercase filenames as shown; remove the extra .txt extension from the uploaded board source and header if present.

🎹 Controls

Main menu and gameplay

Enter a menu number and press Enter. Respond to confirmation prompts with Y or N.

Key	Main menu	Game menu

1	New Game	Visit a Location

2	View Leader/Score Board	Interview a Person

3	About	View Scrabble Board (free)

4	Rules	Form a Word

5	Exit	Accuse a Suspect

6	—	Quit to main menu


📋 Investigation rules

Rule	Default

Starting credibility	100

Maximum turns	25

Wrong turn penalty	−5 credibility

Wrong word penalty	−10 credibility

Win	Correct accusation after solving both words

Lose	Credibility reaches zero or turns run out


Character	Minimum credibility for first interview

Victim	80

Child	75

Partners	50

PA	35

Cook, Guard, Servant	No minimum


🗂️ Source Code Structure

The project uses a modular, single-folder structure with separate character, board and engine interfaces and implementations. main.cpp is the entry point.

File	Responsibility

main.cpp	Starts the singleton engine, attaches the scoreboard and catches game exceptions in the main loop.

engine.h	Declares GameEngine, ScoreBoard, TextUtil, game states, menu choices and file-data records.

engine.cpp	Implements data loading, menus, culprit selection, investigation, accusations, game outcomes and score persistence.

characters.h	Declares the abstract Character base and twelve derived character classes with interview interfaces.

characters.cpp	Implements character construction, polymorphic displays, clue retrieval, interview eligibility and detective statistics.

boards.h	Defines shared clue/tile/location structures, custom exceptions, DynamicArray and board-related class interfaces.

boards.cpp	Implements Cottage, Scrabble, TileRack, custom exception behavior, text helpers and overloaded operators.

characters.txt	Stores the victim and seven suspects, their nationalities, clue positions and profile-specific letters.

locations.txt	Stores seven cottage locations and their Word 1 clue positions and letters for each culprit profile.

clue.txt	Maps each possible culprit role to its two target words.

rules.txt	Configures turn limits, starting credibility, penalties and win/lose rule text.

accusation_dialogue.txt	Supplies character personalities, statements and nationality hints for the accusation stage.

about.txt	Provides the introductory story, mission, course details and instructions shown by the engine.

leader/scoreBoard.txt	Runtime-generated history of completed games, including results, credibility, turns left, culprit 
and date.

README.md	Documents the game, team, setup, architecture and known limitations.

⚙️ How It Works

The investigation follows this sequence:

        ┌──────────────────────────┐
        │      Start new game      │
        └────────────┬─────────────┘
                     ↓
        ┌──────────────────────────┐
        │ Randomly select culprit  │
        └────────────┬─────────────┘
                     ↓
        ┌──────────────────────────┐
        │ Search rooms / interview │
        └────────────┬─────────────┘
                     ↓
        ┌──────────────────────────┐
        │ Collect and place clues  │
        └────────────┬─────────────┘
                     ↓
        ┌──────────────────────────┐
        │     Solve both words     │
        └────────────┬─────────────┘
                     ↓
        ┌──────────────────────────┐
        │ Review evidence / accuse │
        └────────────┬─────────────┘
                     ↓
        ┌──────────────────────────┐
        │   Win / lose and score   │
        └────────────┬─────────────┘
                     └──────────────→ Main menu
					 
The engine checks turn and credibility limits during gameplay; the investigation can end before all clues are collected.

👤 Character hierarchy

Character (abstract)

├── Victim

├── Detective

└── Suspect

    ├── BusinessRelations
    │   ├── Partner
    │   └── PA ─────────┐
    ├── Househelp       │
    │   ├── Cook        │
    │   ├── Guard       │
    │   └── Servant     │
    └── Closerelations  │
        ├── Child       │
        └── PA ◄────────┘
		
BusinessRelations and Closerelations virtually inherit Suspect to resolve the diamond problem in PA. Both business partners are instances of Partner.

🧩 Boards and clue management

- Cottage manages searchable locations using a custom dynamic array.
- TileRack stores collected letter clues and prevents duplicate entries.
- Scrabble manages two dynamically allocated rows and validates completed words.

💾 Game engine and persistence

- The singleton GameEngine coordinates menus, game state, characters, boards and outcomes.
- Structured text files supply character profiles, location clues, target words and rules.
- ScoreBoard records completed games with credibility, remaining turns, culprit and date.

🎓 Educational Objectives
This project demonstrates practical use of:
- Encapsulation through controlled access to class attributes.
- Abstraction through the abstract Character base class.
- Single, hierarchical, multilevel, multiple and virtual inheritance.
- Runtime polymorphism through virtual functions and custom exceptions.
- Dynamic memory allocation, deep copying and virtual destructors.
- Generic programming using the DynamicArray<T> template.
- Operator overloading and friend functions/classes.
- Singleton design pattern and object relationships.
- File handling, exception handling and modular C++ development.

⚠️ Known Limitations
- The game is console-based and supports seven predefined culprit profiles.
- Clues are placed automatically and stored in one shared rack rather than two separate racks; Scrabble uses raw dynamic rows.
- Solving both words unlocks accusation evidence instead of directly revealing the culprit, preserving the final deduction.
- Repeat interviews reveal existing clues without rechecking credibility.
- Score saving requires a writable leader directory; the data files contain puzzle spoilers.
- The supplied about.txt contains placeholder credits, and the default Child age is 12 rather than the specified teenage range.

⚠️ Disclaimer
The Locker Heist is an educational project developed for the Object-Oriented Programming course at FAST-NUCES. The game is intended for academic demonstration and learning.
🕵️ Thanks for checking out our project!
Built with C++ · Powered by logic · Driven by curiosity
<div align="center">
<sub>OOP Semester Project · Fall 2026 · FAST-NUCES</sub>
</div>
