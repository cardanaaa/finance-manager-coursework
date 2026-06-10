#ifndef GOALSPAGE_H
#define GOALSPAGE_H

#include <QWidget>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QDoubleSpinBox>
#include <QScrollArea>
#include "../models/Manager.h"

class GoalsPage : public QWidget {
    Q_OBJECT
public:
    explicit GoalsPage(Manager& mgr, QWidget* parent = nullptr);
    void refresh();
signals:
    void dataChanged();
private slots:
    void addGoal();
    void depositToGoal(size_t index);
private:
    void setupUi();
    void rebuildCards();
    Manager&       manager;
    QWidget*       cardsContainer;
    QVBoxLayout*   cardsLayout;
    QLineEdit*     nameEdit;
    QDoubleSpinBox* targetSpin;
};
#endif
