#ifndef GOAL_H
#define GOAL_H

#include <string>

class Goal {
private:
    std::string name;
    double target;
    double saved;

public:
    Goal() : name(""), target(0), saved(0) {}

    Goal(const std::string& name, double target)
        : name(name), target(target), saved(0) {}

    void addSaved(double amount) { saved += amount; }

    double getProgress() const {
        if (target <= 0) return 0;
        double p = (saved / target) * 100.0;
        return p > 100.0 ? 100.0 : p;
    }

    bool isCompleted() const { return target > 0 && saved >= target; }

    const std::string& getName() const { return name; }
    double getTarget() const { return target; }
    double getSaved() const { return saved; }
    void setSaved(double value) { saved = value; }
};

#endif
