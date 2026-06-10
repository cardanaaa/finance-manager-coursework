#ifndef MANAGER_H
#define MANAGER_H

#include <string>
#include <vector>
#include "Transaction.h"
#include "Budget.h"
#include "Goal.h"

struct ReportSummary {
    double income;
    double expense;
    double balance;
};

class Manager {
private:
    std::vector<Transaction> transactions;
    std::vector<Budget>      budgets;
    std::vector<Goal>        goals;

    void updateBudgetSpent(const std::string& category, double amount);
    int  findBudgetIndex(const std::string& category) const;

public:
    void addTransaction(const Transaction& t);
    void addBudget(const Budget& b);
    void addGoal(const Goal& g);
    void depositToGoal(size_t index, double amount);
    void removeTransaction(size_t index);
    void removeBudget(size_t index);
    void removeGoal(size_t index);

    ReportSummary getReport(const std::string& dateFrom = "",
                            const std::string& dateTo   = "") const;

    std::vector<Transaction> filterTransactions(const std::string& dateFrom,
                                                const std::string& dateTo) const;

    std::vector<std::string> getBudgetWarnings() const;
    void recalculateBudgetSpent();

    const std::vector<Transaction>& getTransactions() const { return transactions; }
    const std::vector<Budget>&      getBudgets()      const { return budgets; }
    const std::vector<Goal>&        getGoals()        const { return goals; }

    std::string toJson()               const;
    bool        fromJson(const std::string& json);
    void        clear() { transactions.clear(); budgets.clear(); goals.clear(); }
};

#endif
