#ifndef TRANSACTION_H
#define TRANSACTION_H

#include <string>

class Transaction {
private:
    double amount;
    std::string type;
    std::string category;
    std::string description;
    std::string date;

public:
    Transaction() : amount(0), type(""), category(""), description(""), date("") {}

    Transaction(double amount, const std::string& type, const std::string& category,
                const std::string& description, const std::string& date)
        : amount(amount), type(type), category(category),
          description(description), date(date) {}

    double getAmount() const { return amount; }
    const std::string& getType() const { return type; }
    const std::string& getCategory() const { return category; }
    const std::string& getDescription() const { return description; }
    const std::string& getDate() const { return date; }
};

#endif
