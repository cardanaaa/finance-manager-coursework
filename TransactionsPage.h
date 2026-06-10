#ifndef TRANSACTIONSPAGE_H
#define TRANSACTIONSPAGE_H

#include <QWidget>
#include <QTableWidget>
#include <QComboBox>
#include <QLineEdit>
#include <QDateEdit>
#include <QDoubleSpinBox>
#include "../models/Manager.h"

class TransactionsPage : public QWidget {
    Q_OBJECT
public:
    explicit TransactionsPage(Manager& mgr, QWidget* parent = nullptr);
    void refresh();
signals:
    void dataChanged();
private slots:
    void addTransaction();
    void removeSelected();
private:
    void setupUi();
    Manager&        manager;
    QTableWidget*   table;
    QDoubleSpinBox* amountSpin;
    QComboBox*      typeCombo;
    QLineEdit*      categoryEdit;
    QLineEdit*      descEdit;
    QDateEdit*      dateEdit;
};
#endif
