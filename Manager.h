#ifndef MANAGER_H
#define MANAGER_H

#include <vector>
#include "Transaction.h"
#include "Budget.h"
#include "Goal.h"
using namespace std;

class Manager {
private:
    vector<Transaction> transactions;
    vector<Budget> budgets;
    vector<Goal> goals;

public:
    void addTransaction(Transaction t) {
        transactions.push_back(t);
    }

    void addBudget(Budget b) {
        budgets.push_back(b);
    }

    void addGoal(Goal g) {
        goals.push_back(g);
    }

    void showReport();
    void showGoals();
};

#endif