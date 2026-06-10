#include "DashboardPage.h"
#include "MainWindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFrame>
#include <QScrollArea>

DashboardPage::DashboardPage(Manager& mgr, QWidget* parent)
    : QWidget(parent), manager(mgr) {
    setObjectName("pageRoot");
    setupUi();
}

void DashboardPage::setupUi() {
    auto* root = new QVBoxLayout(this);
    root->setContentsMargins(36, 32, 36, 32);
    root->setSpacing(24);

    auto* title = new QLabel("Дашборд", this);
    title->setObjectName("pageTitle");
    root->addWidget(title);

    auto* row = new QHBoxLayout();
    row->setSpacing(18);

    auto makeCard = [&](const QString& label, QLabel** valueOut, const QString& obj) {
        auto* card = new QFrame(this);
        card->setObjectName("card");
        card->setFixedHeight(120);
        auto* vl = new QVBoxLayout(card);
        vl->setContentsMargins(24, 18, 24, 18);
        vl->setSpacing(8);

        auto* lbl = new QLabel(label, card);
        lbl->setObjectName("statTitle");
        vl->addWidget(lbl);

        auto* val = new QLabel("0.00 грн", card);
        val->setObjectName(obj);
        vl->addWidget(val);

        *valueOut = val;
        return card;
    };

    row->addWidget(makeCard("ДОХОДИ",  &incomeLabel,  "statIncome"));
    row->addWidget(makeCard("ВИТРАТИ", &expenseLabel, "statExpense"));
    row->addWidget(makeCard("БАЛАНС",  &balanceLabel, "statBalance"));
    root->addLayout(row);

    auto* recentTitle = new QLabel("Останні операції", this);
    recentTitle->setStyleSheet("font-size:18px; font-weight:700; color:#0f172a;");
    root->addWidget(recentTitle);

    auto* scroll = new QScrollArea(this);
    scroll->setWidgetResizable(true);
    recentContainer = new QWidget();
    recentContainer->setStyleSheet("background:transparent;");
    auto* rl = new QVBoxLayout(recentContainer);
    rl->setContentsMargins(0, 0, 0, 0);
    rl->setSpacing(8);
    scroll->setWidget(recentContainer);
    root->addWidget(scroll, 1);
}

void DashboardPage::refresh() {
    auto rep = manager.getReport();
    incomeLabel ->setText(Currency::format(rep.income));
    expenseLabel->setText(Currency::format(rep.expense));
    balanceLabel->setText(Currency::format(rep.balance));

    QLayoutItem* item;
    while ((item = recentContainer->layout()->takeAt(0)) != nullptr) {
        delete item->widget();
        delete item;
    }

    const auto& txs = manager.getTransactions();
    if (txs.empty()) {
        auto* empty = new QLabel("Операцій ще немає. Додайте першу!", recentContainer);
        empty->setStyleSheet("color:#94a3b8; font-size:15px; padding:20px;");
        recentContainer->layout()->addWidget(empty);
        return;
    }

    int start = static_cast<int>(txs.size()) - 10;
    if (start < 0) start = 0;

    for (int i = static_cast<int>(txs.size()) - 1; i >= start; --i) {
        const auto& t = txs[static_cast<size_t>(i)];
        bool isIncome = (t.getType() == "дохід");

        auto* row = new QFrame(recentContainer);
        row->setStyleSheet("QFrame{background:white;border-radius:8px;border:1px solid #e2e8f0;}");
        auto* hl = new QHBoxLayout(row);
        hl->setContentsMargins(18, 12, 18, 12);

        auto* catLbl = new QLabel(QString::fromStdString(t.getCategory()), row);
        catLbl->setStyleSheet("font-weight:600; color:#1e293b; font-size:15px;");

        auto* descLbl = new QLabel(QString::fromStdString(t.getDescription()), row);
        descLbl->setStyleSheet("color:#94a3b8; font-size:14px;");

        auto* dateLbl = new QLabel(QString::fromStdString(t.getDate()), row);
        dateLbl->setStyleSheet("color:#94a3b8; font-size:14px;");

        auto* amtLbl = new QLabel(
            (isIncome ? "+" : "-") + Currency::format(t.getAmount()), row);
        amtLbl->setStyleSheet(
            QString("font-weight:700; font-size:16px; color:%1;")
                .arg(isIncome ? "#16a34a" : "#dc2626"));

        hl->addWidget(catLbl);
        hl->addWidget(descLbl, 1);
        hl->addWidget(dateLbl);
        hl->addWidget(amtLbl);
        recentContainer->layout()->addWidget(row);
    }
    static_cast<QVBoxLayout*>(recentContainer->layout())->addStretch();
}
