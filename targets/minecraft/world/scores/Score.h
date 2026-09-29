#pragma once

#include <memory>
#include <string>
#include <vector>

class Scoreboard;
class Objective;
class Player;

class Score {
public:
    // 4J Not converted

private:
    Scoreboard* scoreboard;
    Objective* objective;
    std::string owner;
    int count;

public:
    Score(Scoreboard* scoreboard, Objective* objective,
          const std::string& owner);

    void add(int count);
    void remove(int count);
    void increment();
    void decrement();
    int getScore();
    void setScore(int score);
    Objective* getObjective();
    std::string getOwner();
    Scoreboard* getScoreboard();
    void updateFor(std::vector<std::shared_ptr<Player> >* players);
};