#include "characters.h"

Character::Character(const string& n, const string& r, const string& nat, bool cul)
    : char_name(n), char_role(r), nationality(nat), is_culprit(cul),
      letter(' '), boardPos(0), interviewedFlag(false)
{
}

Character::Character(const string& n, const string& r, const string& nat,
                     char let, int pos, bool cul)
    : char_name(n), char_role(r), nationality(nat), is_culprit(cul),
      letter(let), boardPos(pos), interviewedFlag(false)
{
}

// Virtual destructor for Character
Character::~Character()
{
}

string Character::get_name() const
{
    return char_name;
}

string Character::get_role() const
{
    return char_role;
}

string Character::get_nationality() const
{
    return nationality;
}

bool Character::get_culprit() const
{
    return is_culprit;
}

string Character::getName() const
{
    return char_name;
}

string Character::getRole() const
{
    return char_role;
}

string Character::getNationality() const
{
    return nationality;
}

bool Character::isCulprit() const
{
    return is_culprit;
}

void Character::setCulprit(bool value)
{
    is_culprit = value;
}

bool Character::isInterviewed() const
{
    return interviewedFlag;
}

Clue Character::interview()
{
    interviewedFlag = true;
    Clue c;
    c.letter   = letter;
    c.position = boardPos;
    c.word     = 2;
    return c;
}

Clue Character::showClueAgain() const
{
    Clue c;
    c.letter   = letter;
    c.position = boardPos;
    c.word     = 2;
    return c;
}

void Character::checkAllowed(int) const
{
}

bool Character::isSuspect() const
{
    return false;
}

ostream& operator<<(ostream& out, const Character& ch)
{
    out << ch.char_name << " (" << ch.char_role << ", " << ch.nationality << ")";
    return out;
}

istream& operator>>(istream& in, Character& ch)
{
    in >> ch.char_name >> ch.char_role >> ch.nationality;
    return in;
}

Victim::Victim(const string& n, const string& nat)
    : Character(n, "Victim", nat, false)
{
}

Victim::Victim(const string& n, const string& r, const string& nat, char let, int pos)
    : Character(n, r, nat, let, pos, false)
{
}

void Victim::display() const
{
    cout << char_name << " (" << char_role << ") - "
         << (interviewedFlag ? "Interviewed" : "NOT Interviewed");
}

void Victim::checkAllowed(int credibility) const
{
    if (credibility < 80)
    {
        throw InvalidActionException("Mr. Leonardo will only talk to you if your credibility is at least 80.");
    }
}

bool Victim::isSuspect() const
{
    return false;
}

Detective::Detective(const string& n, int start_cred)
    : Character(n, "Detective", "", false), cred(100), turnsUsed(0)
{
    set_cred(start_cred);
}

void Detective::display() const
{
    cout << "\n--- Detective Information ---" << endl;
    cout << "Name: "        << char_name << endl;
    cout << "Role: "        << char_role << endl;
    cout << "Credibility: " << cred      << endl;
}

int Detective::get_cred() const
{
    return cred;
}

void Detective::set_cred(int new_cred)
{
    if (new_cred < 0)
    {
        cred = 0;
    }
    else if (new_cred > 100)
    {
        cred = 100;
    }
    else
    {
        cred = new_cred;
    }
}

int Detective::getCredibility() const
{
    return cred;
}

void Detective::loseCredibility(int amount)
{
    set_cred(cred - amount);
}

void Detective::useTurn()
{
    turnsUsed++;
}

int Detective::getTurnsUsed() const
{
    return turnsUsed;
}

Suspect::Suspect(const string& n, const string& r, const string& nat, bool cul)
    : Character(n, r, nat, cul)
{
}

Suspect::Suspect(const string& n, const string& r, const string& nat,
                 char let, int pos, bool cul)
    : Character(n, r, nat, let, pos, cul)
{
}

void Suspect::display() const
{
    cout << char_name << " (" << char_role << ") - "
         << (interviewedFlag ? "Interviewed" : "NOT Interviewed");
}

bool Suspect::isSuspect() const
{
    return true;
}

BusinessRelations::BusinessRelations(const string& n, const string& r,const string& nat, bool cul)
    : Suspect(n, r, nat, cul)
{
}

BusinessRelations::BusinessRelations(const string& n, const string& r,const string& nat, char let, int pos, bool cul)
    : Suspect(n, r, nat, let, pos, cul)
{
}

void BusinessRelations::display() const
{
    cout << char_name << " (" << char_role << ") - "<< (interviewedFlag ? "Interviewed" : "NOT Interviewed");
}

Househelp::Househelp(const string& n, const string& r, const string& nat, bool cul)
    : Suspect(n, r, nat, cul)
{
}

Househelp::Househelp(const string& n, const string& r, const string& nat,char let, int pos, bool cul)
    : Suspect(n, r, nat, let, pos, cul)
{
}

void Househelp::display() const
{
    cout << char_name << " (" << char_role << ") - "<< (interviewedFlag ? "Interviewed" : "NOT Interviewed");
}

void Househelp::checkAllowed(int) const
{
}

Closerelations::Closerelations(const string& n, const string& r,
                               const string& nat, bool cul)
    : Suspect(n, r, nat, cul)
{
}

Closerelations::Closerelations(const string& n, const string& r,
                               const string& nat, char let, int pos, bool cul)
    : Suspect(n, r, nat, let, pos, cul)
{
}

void Closerelations::display() const
{
    cout << char_name << " (" << char_role << ") - "<< (interviewedFlag ? "Interviewed" : "NOT Interviewed");
}

Partner::Partner(const string& n, const string& nat, bool cul)
    : Suspect(n, "Partner", nat, cul),
      BusinessRelations(n, "Partner", nat, cul)
{
}

Partner::Partner(const string& n, const string& r, const string& nat,
                 char let, int pos)
    : Suspect(n, r, nat, let, pos, false),
      BusinessRelations(n, r, nat, let, pos, false)
{
}

void Partner::display() const
{
    cout << char_name << " (" << char_role << ") - "
         << (interviewedFlag ? "Interviewed" : "NOT Interviewed");
}

// Requires credibility >= 50
void Partner::checkAllowed(int credibility) const
{
    if (credibility < 50)
    {
        throw InvalidActionException("Partners will only talk to you if your credibility is at least 50.");
    }
}

Cook::Cook(const string& n, const string& nat, bool cul)
    : Househelp(n, "Cook", nat, cul)
{
}

Cook::Cook(const string& n, const string& r, const string& nat, char let, int pos)
    : Househelp(n, r, nat, let, pos, false)
{
}

void Cook::display() const
{
    cout << char_name << " (" << char_role << ") - "<< (interviewedFlag ? "Interviewed" : "NOT Interviewed");
}

Guard::Guard(const string& n, const string& nat, bool cul)
    : Househelp(n, "Guard", nat, cul)
{
}

Guard::Guard(const string& n, const string& r, const string& nat, char let, int pos)
    : Househelp(n, r, nat, let, pos, false)
{
}

void Guard::display() const
{
    cout << char_name << " (" << char_role << ") - "<< (interviewedFlag ? "Interviewed" : "NOT Interviewed");
}

Servant::Servant(const string& n, const string& nat, bool cul)
    : Househelp(n, "Servant", nat, cul)
{
}

Servant::Servant(const string& n, const string& r, const string& nat,
                 char let, int pos)
    : Househelp(n, r, nat, let, pos, false)
{
}

void Servant::display() const
{
    cout << char_name << " (" << char_role << ") - "<< (interviewedFlag ? "Interviewed" : "NOT Interviewed");
}

Child::Child(const string& n, const string& nat, bool cul, int childage)
    : Suspect(n, "Child", nat, cul),
      Closerelations(n, "Child", nat, cul),
      age(childage)
{
}

Child::Child(const string& n, const string& r, const string& nat,
             char let, int pos, int childage)
    : Suspect(n, r, nat, let, pos, false),
      Closerelations(n, r, nat, let, pos, false),
      age(childage)
{
}

void Child::display() const
{
    cout << char_name << " (" << char_role << ") - "<< (interviewedFlag ? "Interviewed" : "NOT Interviewed");
}

int Child::get_age() const
{
    return age;
}

void Child::checkAllowed(int credibility) const
{
    if (credibility < 75)
    {
        throw InvalidActionException("Ms. Celine will only talk to you if your credibility is at least 75.");
    }
}

PA::PA(const string& n, const string& nat, bool cul)
    : Suspect(n, "PA", nat, cul),
      BusinessRelations(n, "PA", nat, cul),
      Closerelations(n, "PA", nat, cul)
{
}

PA::PA(const string& n, const string& r, const string& nat, char let, int pos)
    : Suspect(n, r, nat, let, pos, false),
      BusinessRelations(n, r, nat, let, pos, false),
      Closerelations(n, r, nat, let, pos, false)
{
}

void PA::display() const
{
    cout << char_name << " (" << char_role << ") - "<< (interviewedFlag ? "Interviewed" : "NOT Interviewed");
}

void PA::checkAllowed(int credibility) const
{
    if (credibility < 35)
    {
        throw InvalidActionException(
            "Mr. Jad will only talk to you if your credibility is at least 35.");
    }
}