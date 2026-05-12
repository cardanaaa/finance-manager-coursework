#include <iostream>
#include "Transaction.h"
#include "Caesar.h"
using namespace std;

int main() {
    cout << "╔════════════════════════════════╗" << endl;
    cout << "║     FINANCE MANAGER            ║" << endl;
    cout << "╠════════════════════════════════╣" << endl;
    cout << "║  1. Додати транзакцію          ║" << endl;
    cout << "║  2. Вийти                      ║" << endl;
    cout << "╚════════════════════════════════╝" << endl;
    cout << "Ваш вибір: ";

    int choice;
    cin >> choice;

    if (choice == 2) {
        cout << "До побачення! 👋" << endl;
    }

    return 0;
}