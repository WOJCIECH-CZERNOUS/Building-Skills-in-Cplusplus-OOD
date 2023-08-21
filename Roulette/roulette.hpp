#include <string>
#include <iostream>
#include <set>
#include <map>
#include <vector>
#include <initializer_list>
#include <random>
#include <exception>

namespace Roulette {

class Outcome {
    public:
        Outcome(std::string name, int odds) :
            name_{name}, odds_{odds} {}
        float winAmount(float amount) const {return odds_ * amount; }
        friend bool operator==(const Outcome& a, const Outcome& b);
        friend std::string to_string(const Outcome &);
        struct Cmp {
            bool operator()(const Outcome& a, const Outcome& b) const { return a.name_ < b.name_; }
        };
        const std::string& getName() const;
    private:
        std::string name_;
        int odds_;
};
bool operator==(const Outcome& a, const Outcome& b);
bool operator!=(const Outcome& a, const Outcome& b);
std::ostream& operator<<(std::ostream& os, const Outcome& x);

class Bin {
    public:
        Bin(std::initializer_list<Outcome> outcomes = {})
            : outcomes_{outcomes} {}
        size_t size() const { return outcomes_.size(); }
        bool count(const Outcome& o) const {return outcomes_.count(o);}
        void insert(const Outcome& o) { outcomes_.insert(o); }
        const std::set<Outcome,Outcome::Cmp>& getOutcomes() const;
    private:
        std::set<Outcome,Outcome::Cmp> outcomes_;
};

class BinBuilder {
    public:
        BinBuilder(std::vector<Bin>& bins)
        : bins_{bins}
         {
            buildStraightBets();
            buildColumnBets();
            buildCornerBets();
            buildEvenMoneyBets();
            buildDozenBets();
            buildLineBets();
            buildSplitBets();
            buildStreetBets();
            buildTheFiveBet();
        }

        bool binContainsOutcome(int index, const Outcome& o) {return bins_[index].count(o) == 1;}
    private:
        std::vector<Bin>& bins_;
        void insertSplitBet(int n1, int n2);
        void insertCornerBet(int n);
        void buildStraightBets();
        void buildColumnBets();
        void buildCornerBets();
        void buildEvenMoneyBets();
        void buildDozenBets();
        void buildLineBets();
        void buildSplitBets();
        void buildStreetBets();
        void buildTheFiveBet();
};

class Wheel {
    public:
        Wheel(int seed) : generator_(seed) {
            BinBuilder bb(bins_);
            for (auto bin : bins_) {
                for (auto outcome : bin.getOutcomes()) {
                    outcomeByName_.try_emplace(outcome.getName(), outcome);
                }
            }
        }
        const Bin& choose() const;
        const Bin& get(int index) const;
        const Outcome& getOutcome(std::string name) const;
    private:
        mutable std::default_random_engine generator_;
        mutable std::uniform_int_distribution<int> distribution {0, 37};
        std::vector<Bin> bins_{38};
        std::map<std::string, Outcome> outcomeByName_;
};

class Bet {
    public:
        Bet(int amountBet, const Outcome& outcome) 
        : amountBet_{amountBet}, outcome_{outcome} 
        {}
        int winAmount() const { return amountBet_ + int(outcome_.winAmount(amountBet_)); }
        int loseAmount() const { return amountBet_; }
        int getAmount() const { return amountBet_; }
        const Outcome& getOutcome() const { return outcome_; }

        friend std::string to_string(const Bet &);

    private:
        const int amountBet_;
        const Outcome& outcome_;
};
std::ostream& operator<<(std::ostream& os, const Bet& x);

struct InvalidBet : std::exception {
    const char* what() const noexcept { return "A Player instance attempts to place a bet which exceeds the table’s limit!\n"; }
};

class Table {
    public:
        Table(int limit, const std::vector<Bet>& bets = {}) 
        : limit_{limit}, bets_{bets} {} 
        void placeBet(const Bet& bet) { bets_.push_back(bet); }
        void clearBets() { bets_.clear(); }
        friend std::string to_string(const Table &);
        bool isValid() const;
        const std::vector<Bet>& getBets() const { return bets_; }
    private:
        int limit_;
        std::vector<Bet> bets_; // FIXME: we will want to remove elements somehow
};
std::ostream& operator<<(std::ostream& os, const Table& x);

class PlayerInterface {
    public:
        virtual void placeBets() = 0;
        virtual void win(const Bet& bet) = 0;
        virtual void lose(const Bet& bet) = 0;
};

class Player : public PlayerInterface {
    public:
        Player(Table& table, int stake, int roundsToGo, 
            bool verbose = false) 
        : table_{table}, stake_{stake}, roundsToGo_{roundsToGo},
            verbose_{verbose}
        {}

        void placeBets() override;
        void win(const Bet& bet) override;
        void lose(const Bet& bet) override;

        bool playing() const;
        virtual void restart(int stake, int roundsToGo) {stake_ = stake; roundsToGo_ = roundsToGo; active_=true;}
        int getStake() const {return stake_;}

        virtual void winners(const std::set<Outcome, Outcome::Cmp>& outcomes);
    protected:
        void prepareBet(const Bet& bet);
    private:
        Table& table_;
        int stake_;
        int roundsToGo_;
        bool active_ = true;
        std::vector<Bet> bets_ = {};
        bool verbose_;
};

class Passenger57 : public Player {
    public:
        Passenger57(const Wheel& wheel, Table& table) 
        : Player{table, 10, 6}, black_{wheel.getOutcome("Black")} {}
        void placeBets() override;
        void win(const Bet& bet) override;
        void lose(const Bet& bet) override;
    private:
        const Outcome& black_;
};

class MartingalePlayer : public Player {
    public:
        MartingalePlayer(
            Table& table, 
            int stake,
            int roundsToGo,
            const Wheel& wheel, 
            int startBet,
            bool verbose = false) 
        : 
        Player{table, stake, roundsToGo, verbose}, 
        startBet_{startBet},
        black_{wheel.getOutcome("Black")} 
        {}

        virtual void placeBets() override;
        void win(const Bet& bet) override;
        void lose(const Bet& bet) override;

        void restart(int stake, int roundsToGo) override;
        
    private:
        const int startBet_;
        const Outcome& black_;
        int lossCount_ = 0;
        int betMultiple_ = 1;
};

struct Game {
    Game(Wheel& wheel, Table& table)
    : wheel_{wheel}, table_{table} {}

    void cycle(Player& player);

    static const int StraightBet = 35;
    static const int SplitBet = 17;
    static const int StreetBet = 11;
    static const int CornerBet = 8;
    static const int FiveBet = 6;
    static const int LineBet = 5;
    static const int Outside12RangeBet = 2;
    static const int OutsideColumnBet = 2;
    static const int Outside18RangeBet = 1;
    static const int OutsideEvenMoneyBet = 1;

    Wheel& wheel_;
    Table& table_;
    Player* pplayer_;
};

class Simulator {
    public:
        Simulator(Player& player, Game& game) 
        : 
        player_{player}, game_{game} 
        {}

        std::vector<int> session();
        void gather();
    private:
        int initDuration_ = 250;
        int initStake_ = 100;
        int samples_ = 50;
        std::vector<int> durations_ = {}; 
        std::vector<int> maxima_ = {};
        Player& player_;
        Game& game_;
};


class SevenReds : public MartingalePlayer {
    public:
        SevenReds(
            Table& table, 
            int stake,
            int roundsToGo,
            const Wheel& wheel, 
            int startBet,
            bool verbose = false) 
        : MartingalePlayer(table, stake, roundsToGo, wheel, startBet, verbose),
        red_{wheel.getOutcome("Red")} 
        {}

        void placeBets() override;
        void winners(const std::set<Outcome, Outcome::Cmp>& outcomes) override;
    private:
        int redCount_ = 7;
        const Outcome& red_;

};

}