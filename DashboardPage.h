#ifndef DASHBOARDPAGE_H
#define DASHBOARDPAGE_H

#include <QWidget>
#include <QLabel>
#include "../models/Manager.h"

class DashboardPage : public QWidget {
    Q_OBJECT
public:
    explicit DashboardPage(Manager& mgr, QWidget* parent = nullptr);
    void refresh();
private:
    void setupUi();
    Manager& manager;
    QLabel* incomeLabel;
    QLabel* expenseLabel;
    QLabel* balanceLabel;
    QWidget* recentContainer;
};
#endif
