#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>
#include <QPushButton>
#include <QLabel>
#include <QString>
#include "../models/Manager.h"
#include "../storage/Storage.h"

class DashboardPage;
class TransactionsPage;
class GoalsPage;

struct Currency {
    static bool isUSD;
    static QString symbol() { return isUSD ? "$" : "грн"; }
    static double convert(double uah) { return isUSD ? uah / 41.0 : uah; }
    static QString format(double uah) {
        double val = convert(uah);
        char buf[64];
        snprintf(buf, sizeof(buf), "%.2f", val);
        return QString(buf) + " " + symbol();
    }
};

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow() override;
    Manager& getManager() { return manager; }

public slots:
    void saveData();
    void refreshAll();

private:
    void setupUi();
    void setupSidebar();
    void applyStylesheet();
    void switchPage(int index);

    Manager         manager;
    QWidget*        centralWidget;
    QWidget*        sidebar;
    QStackedWidget* stack;

    DashboardPage*    dashPage;
    TransactionsPage* txPage;
    GoalsPage*        goalPage;

    QPushButton* navButtons[3];
    QPushButton* currencyBtn;
    int          currentPage = 0;
};

#endif
