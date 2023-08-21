#include <iostream>
#include <random>
#include <numeric>
#include <string>
#include <numeric>
#include <algorithm>
#include "roulette.hpp"

using namespace std;

namespace Roulette {

bool operator==(const Outcome& a, const Outcome& b) { return a.name_ == b.name_ && a.odds_ == b.odds_; }
bool operator!=(const Outcome& a, const Outcome& b) { return !(a==b); }
std::string to_string(const Outcome& x) { return x.name_ + " (" + std::to_string(x.odds_) + ":1)"; }
std::ostream& operator<<(std::ostream& os, const Outcome& x) { os << to_string(x); return os; }
const std::string &Outcome::getName() const { return name_; }

const std::set<Outcome, Outcome::Cmp> &Bin::getOutcomes() const { return outcomes_; }

const Bin& Wheel::choose() const
{
    int index = distribution(generator_);
    return bins_[index];
}

const Bin& Wheel::get(int index) const 
{
    return bins_[index];
}

const Outcome &Wheel::getOutcome(std::string name) const
{
    return outcomeByName_.at(name);
}

void BinBuilder::buildStraightBets()
{
    for (int i = 1; i < 37; ++i) {
        std::string name = "Number " + std::to_string(i);
        Outcome o {name, Game::StraightBet};
        bins_[i].insert(o);
    }
    bins_[0].insert({"Number 0", Game::StraightBet});
    bins_[37].insert({"Number 00", Game::StraightBet});
}

void BinBuilder::insertSplitBet(int n1, int n2)
{
    std::string name = "Split {" + std::to_string(n1) + "," + std::to_string(n2) + "}";
    bins_[n1].insert({name, Game::SplitBet});
    bins_[n2].insert({name, Game::SplitBet});
}

void BinBuilder::insertCornerBet(int n)
{
    int n1 = n;
    int n2 = n+1;
    int n3 = n+3;
    int n4 = n+4;

    std::string name = "Corner {" 
        + std::to_string(n1) + "," 
        + std::to_string(n2) + "," 
        + std::to_string(n3) + "," 
        + std::to_string(n4) + "}";

    bins_[n1].insert({name, Game::CornerBet});
    bins_[n2].insert({name, Game::CornerBet});
    bins_[n3].insert({name, Game::CornerBet});
    bins_[n4].insert({name, Game::CornerBet});
}

void BinBuilder::buildSplitBets()
{
    for (int r = 0; r < 12; ++r) {
        insertSplitBet(3*r+1, 3*r+2);
        insertSplitBet(3*r+2, 3*r+3);
    }
    for (int i = 1; i <= 33; ++i) {
        insertSplitBet(i, i+3);
    }
}

void BinBuilder::buildStreetBets()
{
    for (int r = 0; r < 12; ++r) {
        int n = 3*r + 1;
        int n1 = n;
        int n2 = n + 1;
        int n3 = n + 2;
        std::string name = "Street {" 
            + std::to_string(n1) + "," 
            + std::to_string(n2) + "," 
            + std::to_string(n3) + "}";
        bins_[n1].insert({name, Game::StreetBet});
        bins_[n2].insert({name, Game::StreetBet});
        bins_[n3].insert({name, Game::StreetBet});
    }
}
void BinBuilder::buildCornerBets()
{
    for (int r = 0; r < 11; ++r) {
        insertCornerBet(3*r + 1);
        insertCornerBet(3*r + 2);
    }
}
void BinBuilder::buildLineBets()
{
    for (int r = 0; r < 11; ++r) {
        int n = 3*r + 1;
        int n1 = n;
        int n2 = n+1;
        int n3 = n+2;
        int n4 = n+3;
        int n5 = n+4;
        int n6 = n+5;

        std::string name = "Line {" 
            + std::to_string(n1) + "," 
            + std::to_string(n2) + "," 
            + std::to_string(n3) + "," 
            + std::to_string(n4) + "," 
            + std::to_string(n5) + "," 
            + std::to_string(n6) + "}";

        bins_[n1].insert({name, Game::LineBet});
        bins_[n2].insert({name, Game::LineBet});
        bins_[n3].insert({name, Game::LineBet});
        bins_[n4].insert({name, Game::LineBet});
        bins_[n5].insert({name, Game::LineBet});
        bins_[n6].insert({name, Game::LineBet});
    }
}
void BinBuilder::buildDozenBets()
{
    for (int d = 0; d < 3; ++d) {
        Outcome dozen {"Dozen "+std::to_string(d+1), Game::Outside12RangeBet};
        for (int m = 0; m < 12; ++m) {
            bins_[12*d+m+1].insert(dozen);
        }
    }
}
void BinBuilder::buildColumnBets()
{
    for (int c = 0; c < 3; ++c) {
        Outcome column {"Column "+std::to_string(c+1), Game::OutsideColumnBet};
        for (int r = 0; r < 12; ++r) {
            bins_[3*r+c+1].insert(column);
        }
    }
}
void BinBuilder::buildEvenMoneyBets()
{
    int bet = Game::OutsideEvenMoneyBet;
    for (int n = 1; n < 37; ++n) {
        Bin& bin = bins_[n];

        if (n < 19) {
            bin.insert({"Low", bet});
        } else {
            bin.insert({"High", bet});
        }

        if (n % 2 == 0) {
            bin.insert({"Even", bet});
        } else {
            bin.insert({"Odd", bet});
        }
        // 9 _10 _11 12
        // 10: 0+0
        // 11: 1+1
        if (n%2 + (((n-1)%18)>9) == 1) {
            bin.insert({"Red", bet});
        } else {
            bin.insert({"Black", bet});
        }
    }
}
void BinBuilder::buildTheFiveBet()
{
    Outcome o {"The Five", Game::FiveBet};
    bins_[0].insert(o);
    bins_[37].insert(o);
    bins_[1].insert(o);
    bins_[2].insert(o);
    bins_[3].insert(o);
}

string to_string(const Bet& x) { return std::to_string(x.amountBet_) + " on " + to_string(x.outcome_); }
ostream& operator<<(ostream& os, const Bet& x) { os << to_string(x); return os; }

string to_string(const Table& t) { string s = "Current bets: "; for (auto x : t.bets_) { s += to_string(x); } return s; }
ostream& operator<<(ostream& os, const Table& x) { os << to_string(x); return os; }

bool Table::isValid() const
{
    int all_bets = accumulate(bets_.begin(), bets_.end(), 0, [](int x, const Bet& b){return x + b.getAmount();});
    bool are_bets_ok = all_bets <= limit_;
    if (!are_bets_ok) {
        throw InvalidBet();
    }
    return are_bets_ok;
}

void Passenger57::placeBets()
{
    Bet bet {10, black_};
    Player::prepareBet(bet);
    Player::placeBets();
}

void Passenger57::win(const Bet &bet)
{
    cout << "Passenger57 won " << bet.winAmount() << ". ";
}
void Passenger57::lose(const Bet &bet)
{
    cout << "Passenger57 lost " << bet.loseAmount() << ". ";
}
void Game::cycle(Player &player)
{
    if (!player.playing()) {
        cout << "No more bets. ";
        return;
    }
    player.placeBets();
    
    // Spin the wheel!
    const Bin& winning_bin = wheel_.choose();

    // Notify the player, which outcomes are the winners:
    player.winners(winning_bin.getOutcomes());

    // Serve Winners and Losers...
    for (auto bet : table_.getBets())// iterate through {all bets from the table}
    {
        // if the outcome (of the bet) belongs to the winning bin ...
        const Outcome& bet_outcome = bet.getOutcome();
        if (winning_bin.getOutcomes().count(bet_outcome) == 1)
        {
            player.win(bet);
        } else {
            player.lose(bet);
        }
    }

    // Clear the table...
    table_.clearBets();
}
void Player::winners(const set<Outcome, Outcome::Cmp>&)
{
}
void Player::prepareBet(const Bet &bet)
{
    bets_.push_back(bet);
}
void Player::placeBets()
{
    if (!active_) {
        throw(bad_function_call());
    }
    int sum_of_bets = accumulate(
        bets_.begin(),
        bets_.end(),
        0,
        [](int x,Bet b) { return x+b.getAmount(); }
    );
    bool stake_exceeded = sum_of_bets > stake_;
    if (stake_exceeded) if (verbose_) cout << "Wants to bet " << sum_of_bets << ", but has only " << stake_ << ". ";
    if (!stake_exceeded)
    {
        for (auto bet : bets_) {
            if (verbose_) cout << "Bets " << bet.getAmount() << ", " << stake_-bet.getAmount() << " left. ";
            table_.placeBet(bet);
        }
    }
    stake_ -= sum_of_bets;
    bets_.clear();
    --roundsToGo_;
    bool roundsEnded = roundsToGo_ == 0;
    if (roundsEnded) 
        if (verbose_) cout << "This was the last round.";
    if (roundsEnded || stake_exceeded)
        active_ = false;
}
void Player::win(const Bet &bet)
{
    stake_ += bet.winAmount();
    if (verbose_) cout << "Won " << bet.winAmount() << " and now has " << stake_ << ". ";
}
void Player::lose(const Bet &)
{
    if (verbose_) cout << "Lost. ";
}
bool Player::playing() const
{
    return active_;
}


void MartingalePlayer::placeBets()
{
    int bet_value = startBet_ * betMultiple_;
    Bet bet {bet_value, black_};
    Player::prepareBet(bet);
    Player::placeBets();
}
void MartingalePlayer::win(const Bet &bet)
{
    Player::win(bet);
    lossCount_ = 0;
    betMultiple_ = 1;
}
void MartingalePlayer::lose(const Bet &bet)
{
    Player::lose(bet);
    ++lossCount_;
    betMultiple_ *= 2;
}
void MartingalePlayer::restart(int stake, int roundsToGo)
{
    Player::restart(stake, roundsToGo);
    lossCount_ = 0;
    betMultiple_ = 1;
}
std::vector<int> Simulator::session()
{
    player_.restart(initStake_, initDuration_);
    std::vector<int> final_stakes;
    while (player_.playing()) {
        game_.cycle(player_);
        final_stakes.push_back(player_.getStake());
    }
    return final_stakes;
}
void Simulator::gather()
{
    cout << "duration;maximum\n";
    for (int i = 0; i < samples_; ++i) {
        auto session_list = session();
        int duration = session_list.size();
        durations_.push_back(duration);
        int maximum = 
            (duration > 0) 
            ? 
            *max_element(session_list.begin(), session_list.end())
            :
            0;
        maxima_.push_back(maximum);
        cout <<  duration << ";" << maximum << "\n";
    }
}
void SevenReds::placeBets()
{
    if (redCount_ == 0)
        MartingalePlayer::placeBets();
}
void SevenReds::winners(const set<Outcome, Outcome::Cmp>& outcomes)
{
    bool redWins = false;
    for(auto o : outcomes) {
        if (o == red_){
            if (redCount_ > 0) // After 7 reds in a row, we stop counting until a new black appears, for which we bet.
                --redCount_; 
            redWins = true;
            break;
        }
    }
    if (!redWins)
        redCount_ = 7;
}

}