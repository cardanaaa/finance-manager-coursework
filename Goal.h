#ifndef GOAL_H
#define GOAL_H

#include <string>
using namespace std;

class Goal {
private:
    string name;
    double target;
    double saved;

public:
    Goal(string name, double target) {
        this->name = name;
        this->target = target;
        this->saved = 0;
    }

    void addSaved(double amount) {
        saved += amount;
    }

    double getProgress() {
        return (saved / target) * 100;
    }

    bool isCompleted() {
        return saved >= target;
    }

    string getName()   { return name; }
    double getTarget() { return target; }
    double getSaved()  { return saved; }
};

#endif