#include "GoalsPage.h"
#include "MainWindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QFrame>
#include <QPushButton>
#include <QProgressBar>
#include <QInputDialog>
#include <QStyle>

GoalsPage::GoalsPage(Manager& mgr, QWidget* parent)
    : QWidget(parent), manager(mgr) {
    setObjectName("pageRoot");
    setupUi();
}

void GoalsPage::setupUi() {
    auto* root = new QVBoxLayout(this);
    root->setContentsMargins(36, 32, 36, 32);
    root->setSpacing(22);

    auto* title = new QLabel("Цілі накопичення", this);
    title->setObjectName("pageTitle");
    root->addWidget(title);

    auto* card = new QFrame(this);
    card->setObjectName("card");
    auto* fl = new QHBoxLayout(card);
    fl->setContentsMargins(24, 20, 24, 20);
    fl->setSpacing(12);

    auto* formTitle = new QLabel("Нова ціль:", card);
    formTitle->setStyleSheet("font-size:16px; font-weight:700; color:#0f172a;");
    fl->addWidget(formTitle);

    nameEdit = new QLineEdit(card);
    nameEdit->setPlaceholderText("Назва (наприклад: Відпустка)");
    fl->addWidget(nameEdit, 1);

    targetSpin = new QDoubleSpinBox(card);
    targetSpin->setRange(1, 1e9);
    targetSpin->setDecimals(2);
    targetSpin->setPrefix("грн ");
    targetSpin->setFixedWidth(170);
    fl->addWidget(targetSpin);

    auto* addBtn = new QPushButton("Додати", card);
    addBtn->setObjectName("addBtn");
    addBtn->setFixedWidth(130);
    connect(addBtn, &QPushButton::clicked, this, &GoalsPage::addGoal);
    fl->addWidget(addBtn);
    root->addWidget(card);

    auto* scroll = new QScrollArea(this);
    scroll->setWidgetResizable(true);
    cardsContainer = new QWidget();
    cardsContainer->setStyleSheet("background:transparent;");
    cardsLayout = new QVBoxLayout(cardsContainer);
    cardsLayout->setContentsMargins(0, 0, 0, 0);
    cardsLayout->setSpacing(14);
    cardsLayout->addStretch();
    scroll->setWidget(cardsContainer);
    root->addWidget(scroll, 1);
}

void GoalsPage::addGoal() {
    QString name = nameEdit->text().trimmed();
    if (name.isEmpty()) { nameEdit->setStyleSheet("border-color:#ef4444;"); return; }
    nameEdit->setStyleSheet("");
    manager.addGoal(Goal(name.toStdString(), targetSpin->value()));
    nameEdit->clear();
    targetSpin->setValue(1);
    emit dataChanged();
}

void GoalsPage::depositToGoal(size_t index) {
    bool ok;
    double amount = QInputDialog::getDouble(
        this, "Поповнення", "Сума (грн):", 0, 0.01, 1e9, 2, &ok);
    if (!ok || amount <= 0) return;
    manager.depositToGoal(index, amount);
    emit dataChanged();
}

void GoalsPage::rebuildCards() {
    while (cardsLayout->count() > 1) {
        auto* item = cardsLayout->takeAt(0);
        delete item->widget();
        delete item;
    }

    const auto& goals = manager.getGoals();
    if (goals.empty()) {
        auto* empty = new QLabel("Цілей ще немає. Додайте першу!", cardsContainer);
        empty->setStyleSheet("color:#94a3b8; font-size:15px; padding:24px;");
        cardsLayout->insertWidget(0, empty);
        return;
    }

    for (size_t i = 0; i < goals.size(); ++i) {
        const auto& g = goals[i];
        bool completed = g.isCompleted();
        int  pct       = static_cast<int>(g.getProgress());

        auto* card = new QFrame(cardsContainer);
        card->setObjectName("goalCard");
        auto* vl = new QVBoxLayout(card);
        vl->setContentsMargins(24, 18, 24, 18);
        vl->setSpacing(10);

        auto* hl = new QHBoxLayout();

        auto* nameL = new QLabel(QString::fromStdString(g.getName()), card);
        nameL->setObjectName("cardTitle");

        auto* pctL = new QLabel(
            completed ? "Виконано!" : QString("%1%").arg(pct), card);
        pctL->setStyleSheet(completed
            ? "color:#16a34a; font-weight:700; font-size:16px;"
            : "color:#0ea5e9; font-weight:700; font-size:16px;");

        size_t idx = i;
        auto* depBtn = new QPushButton("Поповнити", card);
        depBtn->setObjectName("depositBtn");
        if (completed) depBtn->setEnabled(false);
        connect(depBtn, &QPushButton::clicked, this,
                [this, idx]() { depositToGoal(idx); });

        hl->addWidget(nameL);
        hl->addStretch();
        hl->addWidget(pctL);
        hl->addWidget(depBtn);
        vl->addLayout(hl);

        auto* bar = new QProgressBar(card);
        bar->setRange(0, 100);
        bar->setValue(pct);
        bar->setTextVisible(false);
        if (completed) bar->setProperty("completed", true);
        bar->style()->unpolish(bar);
        bar->style()->polish(bar);
        vl->addWidget(bar);

        auto* numsL = new QLabel(
            QString("Накопичено: %1  /  Ціль: %2")
                .arg(Currency::format(g.getSaved()))
                .arg(Currency::format(g.getTarget())),
            card);
        numsL->setObjectName("cardSub");
        vl->addWidget(numsL);
        cardsLayout->insertWidget(static_cast<int>(i), card);
    }
}

void GoalsPage::refresh() { rebuildCards(); }
