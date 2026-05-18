#ifndef BUDGET_H
#define BUDGET_H

#include <string>
using namespace std;

class Budget {
private:
    string category;
    double limit;
    double spent;

public:
    Budget(string category, double limit) {
        this->category = category;
        this->limit = limit;
        this->spent = 0;
    }

    void addSpent(double amount) {
        spent += amount;
    }

    bool isExceeded() {
        return spent > limit;
    }

    double getRemaining() {
        return limit - spent;
    }

    string getCategory() { return category; }
    double getLimit()    { return limit; }
    double getSpent()    { return spent; }
};

#endif