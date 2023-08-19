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
    Outcome o1 {"Red", 1};
    Outcome o2 {"Black", 1};
    Outcome o3 {"Even", 1};
    Bin b2 {{o1, o2}};

    int seed = 1;
    Wheel wheel(seed);
    wheel.addOutcome(14, b2);
    auto bin = wheel.choose();
    assert(bin.count({"Red",1}) == 1);

    wheel.addOutcome(37, b2);
    assert(wheel.get(37).size() == 2); // two outcomes in the bin at index 37
    assert(wheel.get(37).count(o1) == 1); // o1 present at 37
    assert(wheel.get(37).count(o3) == 0); // o3 absent at 37

    assert(wheel.getOutcome("Red") == o1);

    cout << "test3() OK." << endl;
}

void test4() {
    int bet;
    BinBuilder bb;
    Wheel w {1};
    bb.buildBins(w);

    bet = Game::StraightBet;
    // bb.buildStraightBets();
    assert(bb.binContainsOutcome(0, {"Number 0", bet}));
    assert(bb.binContainsOutcome(37, {"Number 00", bet}));
    assert(bb.binContainsOutcome(36, {"Number 36", bet}));
    assert(bb.binContainsOutcome(1, {"Number 1", bet}));
    bet = Game::SplitBet;
    // bb.buildSplitBets();
    assert(bb.binContainsOutcome(1, {"Split {1,2}", bet}));
    assert(bb.binContainsOutcome(1, {"Split {1,4}", bet}));
    assert(bb.binContainsOutcome(36, {"Split {33,36}", bet}));
    assert(bb.binContainsOutcome(36, {"Split {35,36}", bet}));
    bet = Game::StreetBet;
    // bb.buildStreetBets();
    assert(bb.binContainsOutcome(1, {"Street {1,2,3}", bet}));
    assert(bb.binContainsOutcome(36, {"Street {34,35,36}", bet}));
    bet = Game::CornerBet;
    // bb.buildCornerBets();
    assert(bb.binContainsOutcome(1, {"Corner {1,2,4,5}", bet}));
    assert(bb.binContainsOutcome(4, {"Corner {1,2,4,5}", bet}));
    assert(bb.binContainsOutcome(5, {"Corner {1,2,4,5}", bet}));
    assert(bb.binContainsOutcome(4, {"Corner {4,5,7,8}", bet}));
    assert(bb.binContainsOutcome(5, {"Corner {4,5,7,8}", bet}));
    assert(bb.binContainsOutcome(5, {"Corner {2,3,5,6}", bet}));
    assert(bb.binContainsOutcome(5, {"Corner {5,6,8,9}", bet}));
    bet = Game::LineBet;
    // bb.buildLineBets();
    assert(bb.binContainsOutcome(1, {"Line {1,2,3,4,5,6}", bet}));
    assert(bb.binContainsOutcome(4, {"Line {1,2,3,4,5,6}", bet}));
    assert(bb.binContainsOutcome(4, {"Line {4,5,6,7,8,9}", bet}));
    bet = Game::Outside12RangeBet;
    // bb.buildDozenBets();
    assert(bb.binContainsOutcome(1,  {"Dozen 1", bet}));
    assert(bb.binContainsOutcome(17, {"Dozen 2", bet}));
    assert(bb.binContainsOutcome(36, {"Dozen 3", bet}));
    bet = Game::OutsideColumnBet;
    // bb.buildColumnBets();
    assert(bb.binContainsOutcome(1,  {"Column 1", bet}));
    assert(bb.binContainsOutcome(17, {"Column 2", bet}));
    assert(bb.binContainsOutcome(36, {"Column 3", bet}));
    bet = Game::OutsideEvenMoneyBet;
    // bb.buildEvenMoneyBets();
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
    // bb.buildTheFiveBet();
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
    Bet b {5, o}; // bet 5 on "A"
    Table t {10, 100};
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
    Table t {10, 100};
    t.placeBet(b1);
    t.placeBet(b2);
    assert(t.isValid());
    cout << "test7() OK." << endl;
}

void test8(){
    Wheel w {1};
    BinBuilder().buildBins(w);
    Table t {10, 100};
    Passenger57 pp {w ,t};
    Player& p = pp;
    Game g {w, t};
    g.cycle(p);
    cout << "test8() OK." << endl;
}

void test9(){
    Wheel w {42};
    BinBuilder().buildBins(w);
    Table t {10, 100};
    Passenger57 pp {w ,t};
    Player& p = pp;
    Game g {w, t};
    g.cycle(p);
    cout << "test9() OK." << endl;
}

void test10() {
    Wheel w {1};
    BinBuilder().buildBins(w);
    Table t {10, 100};
    MartingalePlayer pp {t, 100, 7, w, 10, true};
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

void test11() {
    Wheel w {1};
    BinBuilder().buildBins(w);
    Table t {10, 100};
    MartingalePlayer pp {t, 100, 7, w, 10, false};
    Player& p = pp;
    Game g {w, t};
    Simulator s {p, g};
    s.gather();    

    cout << "test11() OK. No assertions, but produced a csv." << endl;
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
    };

    if (argc > 1)
    {
        int chosen_test = stoi(string(argv[1]));
        // Do *one* test, according to the choice communicated in the command line arg:
        test[chosen_test]();
    } else {
    
        test11();
    
    }


    return 0;
}