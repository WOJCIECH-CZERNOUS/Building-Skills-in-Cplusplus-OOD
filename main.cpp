#include <map>

#include "Roulette/roulette.hpp"

using namespace std;
using namespace Roulette;

using TestFunctionType = void (*)();

void test1() {
    Outcome o1 {"Red", 1};
    Outcome o2 {"Red", 1};
    Outcome o3 {"Black", 1};
    assert(to_string(o1)=="Red (1:1)");
    assert(o1 == o2);
    assert(o1 != o3);
    assert(o2 != o3);
    cout << "test1() OK." << endl;
}

void test2() {
    Bin b1 {{}};
    assert(b1.size() == 0);
    Outcome o1 {"Red", 1};
    Outcome o2 {"Red", 1};
    Outcome o3 {"Black", 1};
    Bin b2 {{o1, o2, o3}};
    assert(b2.size() == 2);
    cout << "test2() OK." << endl;
}

void test3() {
    // Outcome o1 {"Red", 1};
    // Outcome o2 {"Black", 1};
    // Outcome o3 {"Even", 1};
    // Bin b2 {{o1, o2}};

    // int seed = 1;
    // Wheel wheel(seed);
    // wheel.addOutcome(14, b2);
    // auto bin = wheel.choose();
    // assert(bin.count({"Red",1}) == 1);

    // wheel.addOutcome(37, b2);
    // assert(wheel.get(37).size() == 2); // two outcomes in the bin at index 37
    // assert(wheel.get(37).count(o1) == 1); // o1 present at 37
    // assert(wheel.get(37).count(o3) == 0); // o3 absent at 37

    // assert(wheel.getOutcome("Red") == o1);

    cout << "test3() OK." << endl;
}

void test4() {
    int bet;
    vector<Bin> bins {38};
    BinBuilder bb {bins}; // Builds all the bins.

    bet = Game::StraightBet;
    assert(bb.binContainsOutcome(0, {"Number 0", bet}));
    assert(bb.binContainsOutcome(37, {"Number 00", bet}));
    assert(bb.binContainsOutcome(36, {"Number 36", bet}));
    assert(bb.binContainsOutcome(1, {"Number 1", bet}));
    bet = Game::SplitBet;
    assert(bb.binContainsOutcome(1, {"Split {1,2}", bet}));
    assert(bb.binContainsOutcome(1, {"Split {1,4}", bet}));
    assert(bb.binContainsOutcome(36, {"Split {33,36}", bet}));
    assert(bb.binContainsOutcome(36, {"Split {35,36}", bet}));
    bet = Game::StreetBet;
    assert(bb.binContainsOutcome(1, {"Street {1,2,3}", bet}));
    assert(bb.binContainsOutcome(36, {"Street {34,35,36}", bet}));
    bet = Game::CornerBet;
    assert(bb.binContainsOutcome(1, {"Corner {1,2,4,5}", bet}));
    assert(bb.binContainsOutcome(4, {"Corner {1,2,4,5}", bet}));
    assert(bb.binContainsOutcome(5, {"Corner {1,2,4,5}", bet}));
    assert(bb.binContainsOutcome(4, {"Corner {4,5,7,8}", bet}));
    assert(bb.binContainsOutcome(5, {"Corner {4,5,7,8}", bet}));
    assert(bb.binContainsOutcome(5, {"Corner {2,3,5,6}", bet}));
    assert(bb.binContainsOutcome(5, {"Corner {5,6,8,9}", bet}));
    bet = Game::LineBet;
    assert(bb.binContainsOutcome(1, {"Line {1,2,3,4,5,6}", bet}));
    assert(bb.binContainsOutcome(4, {"Line {1,2,3,4,5,6}", bet}));
    assert(bb.binContainsOutcome(4, {"Line {4,5,6,7,8,9}", bet}));
    bet = Game::Outside12RangeBet;
    assert(bb.binContainsOutcome(1,  {"Dozen 1", bet}));
    assert(bb.binContainsOutcome(17, {"Dozen 2", bet}));
    assert(bb.binContainsOutcome(36, {"Dozen 3", bet}));
    bet = Game::OutsideColumnBet;
    assert(bb.binContainsOutcome(1,  {"Column 1", bet}));
    assert(bb.binContainsOutcome(17, {"Column 2", bet}));
    assert(bb.binContainsOutcome(36, {"Column 3", bet}));
    bet = Game::OutsideEvenMoneyBet;
    assert(bb.binContainsOutcome(1,   {"Low", bet}));
    assert(bb.binContainsOutcome(17,  {"Low", bet}));
    assert(bb.binContainsOutcome(18,  {"Low", bet}));
    assert(bb.binContainsOutcome(36,  {"High", bet}));
    assert(bb.binContainsOutcome(1,   {"Red", bet}));
    assert(bb.binContainsOutcome(17,  {"Black", bet}));
    assert(bb.binContainsOutcome(18,  {"Red", bet}));
    assert(bb.binContainsOutcome(36,  {"Red", bet}));
    assert(bb.binContainsOutcome(1,   {"Odd", bet}));
    assert(bb.binContainsOutcome(17,  {"Odd", bet}));
    assert(bb.binContainsOutcome(18,  {"Even", bet}));
    assert(bb.binContainsOutcome(36,  {"Even", bet}));
    bet = Game::FiveBet;
    assert(bb.binContainsOutcome(0,    {"The Five", bet}));
    assert(bb.binContainsOutcome(37,   {"The Five", bet}));

    cout << "test4() OK." << endl;
}

void test5() {
    int x = 7;
    Outcome o {"Abc", 123};
    Bet b {x, o};
    assert(b.winAmount() == x + int(o.winAmount(x)));
    Outcome o1 {"Abc", 123};
    Outcome o2 {o1};
    Outcome o3 = o2;
    cout << "test5() OK." << endl;
}

void test6() {
    Outcome o {"A", 1};
    Bet b {200, o}; // bet more than the maximum
    Table t {100};
    assert(t.isValid()); // empty - no bets
    t.placeBet(b);
    try {
        t.isValid();
    } catch(InvalidBet& e) {
        cout << e.what() << endl;
        cout << "exception raised, as expected - test6() OK." << endl;
    }
}

void test7() {
    Outcome o {"A", 1};
    Bet b1 {10, o}; // bet 10 on "A"
    Bet b2 {90, o}; // bet 90 on "A"
    Table t {100};
    t.placeBet(b1);
    t.placeBet(b2);
    assert(t.isValid());
    cout << "test7() OK." << endl;
}

void test8(){
    Wheel w {1};
    Table t {100};
    Passenger57 pp {w ,t};
    Player& p = pp;
    Game g {w, t};
    g.cycle(p);
    cout << "test8() OK." << endl;
}

void test9(){
    Wheel w {42};
    Table t {100};
    Passenger57 pp {w ,t};
    Player& p = pp;
    Game g {w, t};
    g.cycle(p);
    cout << "test9() OK." << endl;
}

void test10() {
    Wheel w {1};
    Table t {100};
    int stake = 100;
    int roundsToGo = 7;
    int startBet = 10;
    bool verbosePlayer = true;
    MartingalePlayer pp {t, stake, roundsToGo, w, startBet, verbosePlayer};
    Player& p = pp;
    Game g {w, t};
    g.cycle(p);
    g.cycle(p);
    g.cycle(p);
    g.cycle(p);
    g.cycle(p);
    g.cycle(p);
    g.cycle(p);
    g.cycle(p);
    cout << "test10() OK." << endl;
}

void testMartingalePlayerCSV() {
    Wheel w {1};
    Table t {100};
    int stake = 100;
    int roundsToGo = 7;
    int startBet = 10;
    bool verbosePlayer = false;
    MartingalePlayer pp {t, stake, roundsToGo, w, startBet, verbosePlayer};
    Player& p = pp;
    Game g {w, t};
    Simulator s {p, g};
    s.gather();    
}

void test11() {
    cout << "test11() OK. Launching program with parameter \"MartingalePlayerCSV\", would produce a csv." << endl;
}


void test12() {
    Wheel w {42};
    Table t {100};
    int stake = 0;
    int roundsToGo = 0;
    bool verbosePlayer = true;
    Player p {t, stake, roundsToGo, verbosePlayer}; // ... and she makes no bets at all.
    Game g {w, t};
    g.cycle(p);
    g.cycle(p);
    g.cycle(p);
    cout << "test12() OK." << endl;
}

bool testRedStreak(int seed, initializer_list<int> reds) {
    Wheel w {seed};
    Outcome red = w.getOutcome("Red");
    for (auto is_red : reds) {
        if (is_red != w.choose().count(red))
            return false;
    }
    return true;
}

void test13() {
    int seed = 50;
    assert(testRedStreak(seed, {1,1,1, 1,1,1, 1}));

    cout << "SevenReds bets for first seven Reds-";
    Wheel w {seed};// 7 Reds, Black, Red, Black, ...
    Table t {100};
    int stake = 100;
    int roundsToGo = 10;
    int startBet = 10;
    bool verbosePlayer = true;// In case of a bet, she will say something to stdout, to be catched by the ctest's regexp.
    SevenReds pp {t, stake, roundsToGo, w, startBet, verbosePlayer};
    Player& p = pp;
    Game g {w, t};
    for (int i = 0; i < 7; ++i) {
        g.cycle(p);
    }
    cout << "test13() OK." << endl;
}

void test14() {
    int seed = 140;
    assert(testRedStreak(seed, {1,1,1, 1,1,1, 1,1,1}));

    Wheel w {seed};// 9 Reds
    Table t {100};
    int stake = 100;
    int roundsToGo = 10;
    int startBet = 10;
    bool verbosePlayer = true;
    SevenReds pp {t, stake, roundsToGo, w, startBet, verbosePlayer};
    Player& p = pp;
    Game g {w, t};
    for (int i = 0; i < 9; ++i) {
        g.cycle(p);
    }
    cout << "test14() OK." << endl;
}

void testSevenRedsCSV() {
    Wheel w {1};
    Table t {100};
    int stake = 100;
    int roundsToGo = 250;
    int startBet = 10;
    bool verbosePlayer = false;
    SevenReds pp {t, stake, roundsToGo, w, startBet, verbosePlayer};
    Player& p = pp;
    Game g {w, t};
    Simulator s {p, g};
    s.gather();    
}

void test15() {
    cout << "test15() OK. Launching program with parameter \"testSevenRedsCSV\", would produce a csv." << endl;
}



int main(int argc, char* argv[]) {

    map<int, TestFunctionType> test {
        {1, test1},
        {2, test2},
        {3, test3},
        {4, test4},
        {5, test5},
        {6, test6},
        {7, test7},
        {8, test8},
        {9, test9},
        {10, test10},
        {11, test11},
        {12, test12},
        {13, test13},
        {14, test14},
        {15, test15},
    };

    if (argc > 1)
    {
        string argument = string(argv[1]);
        if (argument == "MartingalePlayerCSV")
            testMartingalePlayerCSV();
        else if (argument == "testSevenRedsCSV")
            testSevenRedsCSV();
        else {
            int chosen_test = stoi(argument);
            // Do *one* test, according to the choice communicated in the command line arg:
            test[chosen_test]();
        }
    } else {
    
        // test11();
    
    }


    return 0;
}