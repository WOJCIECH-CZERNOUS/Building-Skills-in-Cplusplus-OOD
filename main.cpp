#include <valarray>

#include "Roulette/roulette.hpp"

using namespace std;
using namespace Roulette;

bool testRedStreak(int seed, const valarray<int>& reds) {
    Wheel w {seed};
    Outcome red = w.getOutcome("Red");
    for (auto is_red : reds) {
        if (is_red != w.choose().count(red))
            return false;
    }
    return true;
}

pair<int,bool> findSeedForRedStreak(const valarray<int>& reds, int lengthMultiplier = 2) {
    int n = lengthMultiplier * (1 << reds.size());
    for (int i = 0; i < n; ++i) {
        if (testRedStreak(i, reds))
            return {i,true};
    }
    return {0,false};
}

ostream& operator<<(ostream& os, const vector<int>& v) {os<<"{";for(int x:v){os<<x<<",";}os<<"}";return os;}

bool approxZero(double x, double eps) { return fabs(x) < eps; }

using TestFunctionType = void (*)();

int main(int argc, char* argv[]) {
    map<string, TestFunctionType> test {
        { "test1", []() {// Outcome
            Outcome o1 {"Red", 1};
            Outcome o2 {"Red", 1};
            Outcome o3 {"Black", 1};
            assert(to_string(o1)=="Red (1:1)");
            assert(o1 == o2);
            assert(o1 != o3);
            assert(o2 != o3);
            cout << "test1() OK." << endl;
        } },
        { "test2", []() {// Bin
            Bin b1 {{}};
            assert(b1.size() == 0);
            Outcome o1 {"Red", 1};
            Outcome o2 {"Red", 1};
            Outcome o3 {"Black", 1};
            Bin b2 {{o1, o2, o3}};
            assert(b2.size() == 2);
            cout << "test2() OK." << endl;
        } },
        { "test3", []() {// [obsolete]
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
        } },
        { "test4", []() {// BinBuilder
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
        } },
        { "test5", []() {// Bet
            int x = 7;
            Outcome o {"Abc", 123};
            Bet b {x, o};
            assert(b.winAmount() == x + int(o.winAmount(x)));
            Outcome o1 {"Abc", 123};
            Outcome o2 {o1};
            Outcome o3 = o2;
            cout << "test5() OK." << endl;
        } },
        { "test6", []() {// Table
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
        } },
        { "test7", []() {// bets validation
            Outcome o {"A", 1};
            Bet b1 {10, o}; // bet 10 on "A"
            Bet b2 {90, o}; // bet 90 on "A"
            Table t {100};
            t.placeBet(b1);
            t.placeBet(b2);
            assert(t.isValid());
            cout << "test7() OK." << endl;
        } },
        { "test8", []() {// Passenger57
            Wheel w {1};
            Table t {100};
            Passenger57 p {w ,t};
            Game g {w, t};
            g.cycle(p);
            cout << "test8() OK." << endl;
        } },
        { "test9", []() {// Passenger57 
            Wheel w {42};
            Table t {100};
            Passenger57 p {w ,t};
            Game g {w, t};
            g.cycle(p);
            cout << "test9() OK." << endl;
        } },
        { "test10", []() {// MartingalePlayer
            Wheel w {1};
            Table t {100};
            int stake = 100;
            int roundsToGo = 7;
            int startBet = 10;
            bool verbosePlayer = true;
            MartingalePlayer p {t, stake, roundsToGo, w, startBet, verbosePlayer};
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
        } },
        { "test11", []() {// Simulator<MartingalePlayer>
            Wheel w {1};
            Table t {100};
            int stake = 100;
            int roundsToGo = 7;
            int startBet = 10;
            bool verbosePlayer = false;
            MartingalePlayer p {t, stake, roundsToGo, w, startBet, verbosePlayer};
            Game g {w, t};
            // Set 'verboseSimulator' to true, if you want csv output:
            bool verboseSimulator = false;
            Simulator<MartingalePlayer> s {p, g, verboseSimulator};
            s.gather();    
            cout << "test11() OK." << endl;
        } },
        { "test12", []() {// Player with no bets
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
        } },
        { "test13", []() {// SevenReds
            int seed = 50;
            assert(testRedStreak(seed, {1,1,1, 1,1,1, 1}));

            cout << "SevenReds bets for first seven Reds-";
            Wheel w {seed};// 7 Reds, Black, Red, Black, ...
            Table t {100};
            int stake = 100;
            int roundsToGo = 10;
            int startBet = 10;
            bool verbosePlayer = true;// In case of a bet, she will say something to stdout, to be catched by the ctest's regexp.
            SevenReds p {t, stake, roundsToGo, w, startBet, verbosePlayer};
            Game g {w, t};
            for (int i = 0; i < 7; ++i) {
                g.cycle(p);
            }
            cout << "test13() OK." << endl;
        } },
        { "test14", []() {// SevenReds
            int seed = 140;
            assert(testRedStreak(seed, {1,1,1, 1,1,1, 1,1,1}));

            Wheel w {seed};// 9 Reds
            Table t {100};
            int stake = 100;
            int roundsToGo = 10;
            int startBet = 10;
            bool verbosePlayer = true;
            SevenReds p {t, stake, roundsToGo, w, startBet, verbosePlayer};
            Game g {w, t};
            for (int i = 0; i < 9; ++i) {
                g.cycle(p);
            }
            cout << "test14() OK." << endl;
        } },
        { "test15", []() {// SevenReds stats
            Wheel w {1};
            Table t {100};
            int stake = 100;
            int roundsToGo = 250;
            int startBet = 10;
            bool verbosePlayer = false;
            SevenReds p {t, stake, roundsToGo, w, startBet, verbosePlayer};
            Game g {w, t};
            // Set 'verboseSimulator' to true, if you want csv output:
            bool verboseSimulator = false;
            Simulator s {p, g, verboseSimulator};
            s.gather();    
            cout << "test15() OK." << endl;
        } },
        { "test16", []() {// Statistics
            Statistics stat {{10, 8, 13, 9, 11, 14, 6, 4, 12, 7, 5}};
            auto [m,s] = stat.getStats();
            double epsilon = 1e-3;
            assert(approxZero(m - 9, epsilon));
            assert(approxZero(s - 3.317, epsilon));
            cout << "test16() OK." << endl;
        } },
        { "test17", []() {// RandomPlayer
            Wheel w {1};
            vector<string> outcomes;
            int n = w.numberOfOutcomes();
            for (int i = 0; i < n; ++i) {
                string s = w.getOutcome(i).getName();
                outcomes.push_back(s);
            }
            RandomOutcomeGenerator rog {w, outcomes};
            Table t {100};
            int stake = 100;
            int roundsToGo = n;
            int startBet = 1;
            bool verbosePlayer = true;
            RandomPlayer p {rog, t, stake, roundsToGo, startBet, verbosePlayer};
            Game g {w, t};
            // Set 'verboseSimulator' to true, if you want csv output:
            bool verboseSimulator = false;
            Simulator s {p, g, verboseSimulator};
            s.gather();    
            cout << "test17() OK." << endl;
        } },
        { "test18", []() {// Player1326
            auto v = {9,19,14,39,3,23,18,2,20,31,26,0,4,15,11,10,};
            int i = 0;
            map<int,int> nextBet {{1,3},{3,2},{2,6},{6,1}};
            for (int seed:v) {
                valarray<int> wins {1&(i>>3), 1&(i>>2), 1&(i>>1), 1&(i>>0)};
                ++i;
                assert(testRedStreak(seed, wins));

                valarray<int> bets {1,1,1,1};
                for (int j:{0,1,2}) { if (wins[j]==1) bets[j+1] = nextBet[bets[j]]; }
                int gain = (bets * (2 * wins - 1)).sum();

                Wheel w {seed};
                Table t {100};
                int stake = 10;
                int roundsToGo = 4;
                int startBet = 1;
                bool verbosePlayer = false;
                Player1326 p {t, stake, roundsToGo, w, startBet, verbosePlayer};
                Game g {w, t};
                for (int j = 0; j < 4; ++j) {
                    g.cycle(p);
                }
                assert(p.getStake() == stake + gain);
            }
            cout << "test18() OK." << endl;
        }},
        { "test19", []() {// Simulator<Player1326>
            Wheel w {1};
            Table t {100};
            int stake = 100;
            int roundsToGo = 250;
            int startBet = 10;
            bool verbosePlayer = false;
            Player1326 p {t, stake, roundsToGo, w, startBet, verbosePlayer};
            Game g {w, t};
            // Set 'verboseSimulator' to true, if you want csv output:
            bool verboseSimulator = false;
            Simulator<Player1326> s {p, g, verboseSimulator};
            s.gather();    
            cout << "test19() OK." << endl;
        } },
        { "test20", []() {// PlayerCancellation
            auto v = {9,19,14,39,3,23,18,2,20,31,26,0,4,15,11,10,};
            int i = 0;
            map<int,int> nextBet {{1,3},{3,2},{2,6},{6,1}};
            for (int seed:v) {
                valarray<int> wins {1&(i>>3), 1&(i>>2), 1&(i>>1), 1&(i>>0)};
                ++i;
                assert(testRedStreak(seed, wins));

                initializer_list<int> bets {1,2,3,4,5,6};
                Wheel w {seed};
                Table t {100};
                int stake = 21;
                int roundsToGo = 4;
                int startBet = 1;
                bool verbosePlayer = true;
                PlayerCancellation p {vector<int>(bets), t, stake, roundsToGo, w, startBet, verbosePlayer};
                Game g {w, t};
                for (int j = 0; j < 4; ++j) {
                    g.cycle(p);
                }

                // independent check of strategy results:
                deque<int> b {bets};
                int gain = 0;
                for (int j = 0; j < 4; ++j) {
                    if (b.empty())
                        break;
                    int bet = b.front() + b.back();
                    if (bet > stake + gain)
                        break;
                    if (wins[j] == 1) {
                        gain += bet;
                        b.pop_back();
                        if (!b.empty()) b.pop_front();
                    } else {
                        gain -= bet;
                        b.push_back(bet);
                    }
                }
                assert(p.getStake() == stake + gain);
            }
            cout << "test20() OK." << endl;
        }},
    };

    if (argc > 1)
    {
        string name = string(argv[1]);
        // Do *one* test, according to the choice communicated in the command line arg:
        test[name]();
    } else {
        test["test20"]();
    }
    return 0;
} 