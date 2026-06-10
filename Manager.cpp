#include "Manager.h"
#include <sstream>
#include <algorithm>


static bool dateInRange(const std::string& date,
                        const std::string& from,
                        const std::string& to) {
    if (!from.empty() && date < from) return false;
    if (!to.empty()   && date > to)   return false;
    return true;
}

static std::string jsonStr(const std::string& s) {
    std::string out = "\"";
    for (char c : s) {
        if (c == '"')  out += "\\\"";
        else if (c == '\\') out += "\\\\";
        else           out += c;
    }
    return out + "\"";
}

static std::string extractStr(const std::string& json, const std::string& key) {
    std::string pat = "\"" + key + "\"";
    size_t p = json.find(pat);
    if (p == std::string::npos) return "";
    p = json.find(':', p);
    if (p == std::string::npos) return "";
    p = json.find('"', p);
    if (p == std::string::npos) return "";
    size_t e = json.find('"', p + 1);
    while (e != std::string::npos && json[e-1] == '\\') e = json.find('"', e+1);
    if (e == std::string::npos) return "";
    std::string val = json.substr(p+1, e-p-1);
    // unescape
    std::string res;
    for (size_t i = 0; i < val.size(); ++i) {
        if (val[i] == '\\' && i+1 < val.size()) { res += val[++i]; }
        else res += val[i];
    }
    return res;
}

static double extractNum(const std::string& json, const std::string& key) {
    std::string pat = "\"" + key + "\"";
    size_t p = json.find(pat);
    if (p == std::string::npos) return 0;
    p = json.find(':', p);
    if (p == std::string::npos) return 0;
    ++p;
    while (p < json.size() && (json[p]==' '||json[p]=='\t')) ++p;
    return std::stod(json.substr(p));
}

static std::vector<std::string> splitObjects(const std::string& arr) {
    std::vector<std::string> out;
    int depth = 0;
    size_t start = std::string::npos;
    for (size_t i = 0; i < arr.size(); ++i) {
        if (arr[i] == '{') {
            if (depth == 0) start = i;
            ++depth;
        } else if (arr[i] == '}') {
            --depth;
            if (depth == 0 && start != std::string::npos) {
                out.push_back(arr.substr(start, i - start + 1));
                start = std::string::npos;
            }
        }
    }
    return out;
}

static std::string extractArray(const std::string& json, const std::string& key) {
    std::string pat = "\"" + key + "\"";
    size_t p = json.find(pat);
    if (p == std::string::npos) return "[]";
    p = json.find('[', p);
    if (p == std::string::npos) return "[]";
    size_t e = p + 1;
    int depth = 1;
    while (e < json.size() && depth > 0) {
        if (json[e] == '[') ++depth;
        else if (json[e] == ']') --depth;
        ++e;
    }
    return json.substr(p, e - p);
}


int Manager::findBudgetIndex(const std::string& category) const {
    for (size_t i = 0; i < budgets.size(); i++)
        if (budgets[i].getCategory() == category) return static_cast<int>(i);
    return -1;
}

void Manager::updateBudgetSpent(const std::string& category, double amount) {
    int idx = findBudgetIndex(category);
    if (idx >= 0) budgets[static_cast<size_t>(idx)].addSpent(amount);
}

void Manager::recalculateBudgetSpent() {
    for (auto& b : budgets) b.setSpent(0);
    for (const auto& t : transactions)
        if (t.getType() == "витрата")
            updateBudgetSpent(t.getCategory(), t.getAmount());
}


void Manager::addTransaction(const Transaction& t) {
    transactions.push_back(t);
    if (t.getType() == "витрата")
        updateBudgetSpent(t.getCategory(), t.getAmount());
}

void Manager::addBudget(const Budget& b) {
    if (findBudgetIndex(b.getCategory()) >= 0) return;
    budgets.push_back(b);
    recalculateBudgetSpent();
}

void Manager::addGoal(const Goal& g) {
    goals.push_back(g);
}

void Manager::depositToGoal(size_t index, double amount) {
    if (index < goals.size() && amount > 0)
        goals[index].addSaved(amount);
}

void Manager::removeTransaction(size_t index) {
    if (index < transactions.size()) {
        transactions.erase(transactions.begin() + static_cast<int>(index));
        recalculateBudgetSpent();
    }
}

void Manager::removeBudget(size_t index) {
    if (index < budgets.size())
        budgets.erase(budgets.begin() + static_cast<int>(index));
}

void Manager::removeGoal(size_t index) {
    if (index < goals.size())
        goals.erase(goals.begin() + static_cast<int>(index));
}

ReportSummary Manager::getReport(const std::string& dateFrom,
                                  const std::string& dateTo) const {
    ReportSummary r{0, 0, 0};
    for (const auto& t : transactions) {
        if (!dateInRange(t.getDate(), dateFrom, dateTo)) continue;
        if (t.getType() == "дохід")   r.income  += t.getAmount();
        else                           r.expense += t.getAmount();
    }
    r.balance = r.income - r.expense;
    return r;
}

std::vector<Transaction> Manager::filterTransactions(const std::string& dateFrom,
                                                      const std::string& dateTo) const {
    std::vector<Transaction> out;
    for (const auto& t : transactions)
        if (dateInRange(t.getDate(), dateFrom, dateTo))
            out.push_back(t);
    return out;
}

std::vector<std::string> Manager::getBudgetWarnings() const {
    std::vector<std::string> w;
    for (const auto& b : budgets)
        if (b.isExceeded())
            w.push_back("Перевищено бюджет \"" + b.getCategory() + "\"");
    return w;
}


std::string Manager::toJson() const {
    std::ostringstream o;
    o << "{\n";

    o << "\"transactions\":[\n";
    for (size_t i = 0; i < transactions.size(); ++i) {
        const auto& t = transactions[i];
        o << "{"
          << "\"amount\":"      << t.getAmount()                 << ","
          << "\"type\":"        << jsonStr(t.getType())           << ","
          << "\"category\":"    << jsonStr(t.getCategory())       << ","
          << "\"description\":" << jsonStr(t.getDescription())    << ","
          << "\"date\":"        << jsonStr(t.getDate())           << "}";
        if (i + 1 < transactions.size()) o << ",";
        o << "\n";
    }
    o << "],\n";

    // budgets
    o << "\"budgets\":[\n";
    for (size_t i = 0; i < budgets.size(); ++i) {
        const auto& b = budgets[i];
        o << "{"
          << "\"category\":"  << jsonStr(b.getCategory()) << ","
          << "\"limit\":"     << b.getLimit()              << ","
          << "\"spent\":"     << b.getSpent()              << "}";
        if (i + 1 < budgets.size()) o << ",";
        o << "\n";
    }
    o << "],\n";

    o << "\"goals\":[\n";
    for (size_t i = 0; i < goals.size(); ++i) {
        const auto& g = goals[i];
        o << "{"
          << "\"name\":"   << jsonStr(g.getName()) << ","
          << "\"target\":" << g.getTarget()         << ","
          << "\"saved\":"  << g.getSaved()           << "}";
        if (i + 1 < goals.size()) o << ",";
        o << "\n";
    }
    o << "]\n}";

    return o.str();
}

bool Manager::fromJson(const std::string& json) {
    try {
        clear();

        auto tarr = splitObjects(extractArray(json, "transactions"));
        for (const auto& obj : tarr) {
            double      amount = extractNum(obj, "amount");
            std::string type   = extractStr(obj, "type");
            std::string cat    = extractStr(obj, "category");
            std::string desc   = extractStr(obj, "description");
            std::string date   = extractStr(obj, "date");
            transactions.emplace_back(amount, type, cat, desc, date);
        }

        auto barr = splitObjects(extractArray(json, "budgets"));
        for (const auto& obj : barr) {
            std::string cat   = extractStr(obj, "category");
            double      limit = extractNum(obj, "limit");
            double      spent = extractNum(obj, "spent");
            Budget b(cat, limit);
            b.setSpent(spent);
            budgets.push_back(b);
        }

        auto garr = splitObjects(extractArray(json, "goals"));
        for (const auto& obj : garr) {
            std::string name   = extractStr(obj, "name");
            double      target = extractNum(obj, "target");
            double      saved  = extractNum(obj, "saved");
            Goal g(name, target);
            g.setSaved(saved);
            goals.push_back(g);
        }

        return true;
    } catch (...) {
        return false;
    }
}
