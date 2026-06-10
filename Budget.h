#ifndef BUDGET_H
#define BUDGET_H

#include <string>

class Budget {
private:
    std::string category;
    double limit;
    double spent;

public:
    Budget() : category(""), limit(0), spent(0) {}

    Budget(const std::string& category, double limit)
        : category(category), limit(limit), spent(0) {}

    void addSpent(double amount) { spent += amount; }
    void setSpent(double value) { spent = value; }

    bool isExceeded() const { return spent > limit; }
    double getRemaining() const { return limit - spent; }

    const std::string& getCategory() const { return category; }
    double getLimit() const { return limit; }
    double getSpent() const { return spent; }
};

#endif
