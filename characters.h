#ifndef CHARACTERS_H
#define CHARACTERS_H
#include "boards.h"

class Character
{
protected:
    string char_name;
    string char_role;
    string nationality;
    bool   is_culprit;

    // Extra fields added so the engine can store a clue per character
    char letter;
    int  boardPos;
    bool interviewedFlag;

public:
    Character(const string& n, const string& r, const string& nat, bool cul = false);
    Character(const string& n, const string& r, const string& nat, char let, int pos, bool cul = false);

    virtual ~Character();

    virtual void display() const = 0;

    string get_name()        const;
    string get_role()        const;
    string get_nationality() const;
    bool   get_culprit()     const;

    string getName()        const;
    string getRole()        const;
    string getNationality() const;
    bool   isCulprit()      const;
    void   setCulprit(bool value);
    bool   isInterviewed()  const;

    Clue interview();
    Clue showClueAgain() const;

    virtual void checkAllowed(int credibility) const;
    virtual bool isSuspect() const;

    friend class GameEngine;
    friend istream &operator>>(istream &in, Character &ch);
    friend ostream &operator<<(ostream &out, const Character &ch);
};

class Victim : public Character
{
public:
    Victim(const string& n, const string& nat);
    Victim(const string& n, const string& r, const string& nat, char let, int pos);

    virtual void display() const;

    // Requires credibility >= 80 to interview
    virtual void checkAllowed(int credibility) const;
    virtual bool isSuspect() const;
};

class Detective : public Character
{
private:
    int cred;
    int turnsUsed;

public:
    Detective(const string& n, int start_cred);

    virtual void display() const;

    int  get_cred() const;
    void set_cred(int new_cred);

    int  getCredibility()     const;
    void loseCredibility(int amount);
    void useTurn();
    int  getTurnsUsed()       const;
};

class Suspect : public Character
{
public:
    Suspect(const string& n, const string& r, const string& nat, bool cul = false);
    Suspect(const string& n, const string& r, const string& nat,char let, int pos, bool cul = false);

    virtual void display() const;
    virtual bool isSuspect() const;
};

class BusinessRelations : virtual public Suspect
{
public:
    BusinessRelations(const string& n, const string& r, const string& nat,bool cul = false);
    BusinessRelations(const string& n, const string& r, const string& nat,char let, int pos, bool cul = false);

    virtual void display() const;
};

class Househelp : public Suspect
{
public:
    Househelp(const string& n, const string& r, const string& nat, bool cul = false);
    Househelp(const string& n, const string& r, const string& nat,char let, int pos, bool cul = false);

    virtual void display() const;

    virtual void checkAllowed(int credibility) const;
};

class Closerelations : virtual public Suspect
{
public:
    Closerelations(const string& n, const string& r, const string& nat,bool cul = false);
    Closerelations(const string& n, const string& r, const string& nat,char let, int pos, bool cul = false);

    virtual void display() const;
};

class Partner : public BusinessRelations
{
public:
    Partner(const string& n, const string& nat, bool cul = false);
    Partner(const string& n, const string& r, const string& nat, char let, int pos);

    virtual void display() const;

    // Requires credibility >= 50
    virtual void checkAllowed(int credibility) const;
};

class Cook : public Househelp
{
public:
    Cook(const string& n, const string& nat, bool cul = false);
    Cook(const string& n, const string& r, const string& nat, char let, int pos);

    virtual void display() const;
};

class Guard : public Househelp
{
public:
    Guard(const string& n, const string& nat, bool cul = false);
    Guard(const string& n, const string& r, const string& nat, char let, int pos);

    virtual void display() const;
};

class Servant : public Househelp
{
public:
    Servant(const string& n, const string& nat, bool cul = false);
    Servant(const string& n, const string& r, const string& nat, char let, int pos);

    virtual void display() const;
};

class Child : public Closerelations
{
private:
    int age;

public:
    Child(const string& n, const string& nat, bool cul, int childage);
    Child(const string& n, const string& r, const string& nat,char let, int pos, int childage = 12);

    virtual void display() const;

    int get_age() const;

    // Requires credibility >= 75
    virtual void checkAllowed(int credibility) const;
};

class PA : public BusinessRelations, public Closerelations
{
public:
    PA(const string& n, const string& nat, bool cul = false);
    PA(const string& n, const string& r, const string& nat, char let, int pos);

    virtual void display() const;

    // Requires credibility >= 35
    virtual void checkAllowed(int credibility) const;
};

#endif 
