#include <iostream>
#include <random>
#include <numeric>
#include <string>
#include <numeric>
#include <algorithm>
#include <stdexcept>      // std::out_of_range
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
    int index = distribution_(generator_);
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

const Outcome &Wheel::getOutcome(int index) const
{
    return outcomes_.at(index);
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
    if (!player.isActive()) {
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
    if (!isActive()) {
        throw(bad_function_call());
    }
    int sum_of_bets = accumulate(
        bets_.begin(),
        bets_.end(),
        0,
        [](int x,Bet b) { return x+b.getAmount(); }
    );
    if (sum_of_bets > stake_) {
        deactivate();
        if (verbose_) cout << "Wants to bet " << sum_of_bets << ", but has only " << stake_ << ". ";
        return;
    }
    for (auto bet : bets_) {
        if (verbose_) cout << "Bets " << bet.getAmount() << ", " << stake_-bet.getAmount() << " left. ";
        table_.placeBet(bet);
    }
    stake_ -= sum_of_bets;
    bets_.clear();
}
void Player::win(const Bet &bet)
{
    stake_ += bet.winAmount();
    if (verbose_) cout << "Won " << bet.winAmount() << " and now has " << stake_ << ". ";
    completeTheRound();
}
void Player::lose(const Bet &)
{
    if (verbose_) cout << "Lost. ";
    completeTheRound();
}
void Player::completeTheRound()
{
    --roundsToGo_;
    if (roundsToGo_ == 0) { 
        deactivate();
        if (verbose_) cout << "This was the last round.";
    }
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


template <typename PlayerType>
std::vector<int> Simulator<PlayerType>::session()
{
    PlayerType newPlayer {player_};
    // player_.restart(initStake_, initDuration_);
    std::vector<int> final_stakes;
    while (newPlayer.isActive()) {
        game_.cycle(newPlayer);
        final_stakes.push_back(newPlayer.getStake());
    }
    return final_stakes;
}
template <typename PlayerType>
void Simulator<PlayerType>::gather()
{
    if(verbose_) cout << "duration;maximum\n";
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
        if(verbose_) cout <<  duration << ";" << maximum << "\n";
    }
}
template class Simulator<MartingalePlayer>;
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
template class Simulator<SevenReds>;
const Outcome &RandomOutcomeGenerator::outcome(int i) const
{
    int n = outcomes_.size();
    if (n > 0) {
        if (i < n) {
            return wheel_.getOutcome(outcomes_[i]);
        }
        else 
            throw out_of_range("invalid index to outcomes_");
    } 

    int index = distribution_(generator_);
    return wheel_.getOutcome(index);
}
void RandomPlayer::placeBets()
{   
    Player::prepareBet({startBet_, rng_.outcome(betIndex_)});
    betIndex_++;
    Player::placeBets();
}
template class Simulator<RandomPlayer>;
void StatefulPlayer::transitionTo(unique_ptr<State> newState) {
    state = std::move(newState);
    state->player = this;
}
void State1326NoWins::processLoss(){ player->transitionTo( make_unique<State1326NoWins>() ); }
void State1326OneWin::processLoss(){ player->transitionTo( make_unique<State1326NoWins>() ); }
void State1326TwoWins::processLoss(){ player->transitionTo( make_unique<State1326NoWins>() ); }
void State1326ThreeWins::processLoss(){ player->transitionTo( make_unique<State1326NoWins>() ); }
void State1326NoWins::processWin(){ player->transitionTo( make_unique<State1326OneWin>() ); }
void State1326OneWin::processWin(){ player->transitionTo( make_unique<State1326TwoWins>() ); }
void State1326TwoWins::processWin(){ player->transitionTo( make_unique<State1326ThreeWins>() ); }
void State1326ThreeWins::processWin(){ player->transitionTo( make_unique<State1326NoWins>() ); }
void Player1326::init()
{
    transitionTo(std::make_unique<State1326NoWins>());
}
void Player1326::placeBets()
{
    int betMultiple = state->currentBet();
    int bet_value = startBet_ * betMultiple;
    Bet bet {bet_value, favoriteOutcome_};
    Player::prepareBet(bet);
    Player::placeBets();
}
void Player1326::win(const Bet &bet)
{
    Player::win(bet);
    state->processWin();
}
void Player1326::lose(const Bet &bet)
{
    Player::lose(bet);
    state->processLoss();
}
template class Simulator<Player1326>;
void PlayerCancellation::placeBets()
{
    int smallBet = betsToCancel_.front();
    int bigBet = betsToCancel_.back();
    int betMultiple = smallBet + bigBet;
    int betValue = startBet_ * betMultiple;
    Bet bet {betValue, favoriteOutcome_};
    Player::prepareBet(bet);
    Player::placeBets();
}
void PlayerCancellation::win(const Bet &bet)
{   
    Player::win(bet);
    // cancel two, at list's ends:
    betsToCancel_.pop_front();
    if (!betsToCancel_.empty())
        betsToCancel_.pop_back();
    if (betsToCancel_.empty())
        deactivate();
}
void PlayerCancellation::lose(const Bet &bet)
{
    Player::lose(bet);
    // will try to regain the loss, next time:
    betsToCancel_.push_back(bet.getAmount());
}
template class Simulator<PlayerCancellation>;
void PlayerFibonacci::placeBets()
{
    int betValue = startBet_ * current_;
    Bet bet {betValue, favoriteOutcome_};
    Player::prepareBet(bet);
    Player::placeBets();
}
void PlayerFibonacci::win(const Bet &bet)
{   
    Player::win(bet);
    restartFibonacci();
}
void PlayerFibonacci::lose(const Bet &bet)
{
    Player::lose(bet);
    int next = previous_ + current_;
    previous_ = current_;
    current_ = next;
}
template class Simulator<PlayerFibonacci>;

}