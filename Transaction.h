#ifndef TRANSACTION_H
#define TRANSACTION_H

#include <string>
using namespace std;

class Transaction {
private:
    double amount;
    string type;
    string category;
    string description;
    string date;

public:
    Transaction(double amount, string type, string category, 
                string description, string date) {
        this->amount = amount;
        this->type = type;
        this->category = category;
        this->description = description;
        this->date = date;
    }

    double getAmount()      { return amount; }
    string getType()        { return type; }
    string getCategory()    { return category; }
    string getDescription() { return description; }
    string getDate()        { return date; }
};

#endif