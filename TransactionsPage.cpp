#include "TransactionsPage.h"
#include "MainWindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QLabel>
#include <QFrame>
#include <QDate>
#include <QPushButton>

TransactionsPage::TransactionsPage(Manager& mgr, QWidget* parent)
    : QWidget(parent), manager(mgr) {
    setObjectName("pageRoot");
    setupUi();
}

void TransactionsPage::setupUi() {
    auto* root = new QVBoxLayout(this);
    root->setContentsMargins(36, 32, 36, 32);
    root->setSpacing(22);

    auto* title = new QLabel("Транзакції", this);
    title->setObjectName("pageTitle");
    root->addWidget(title);

    auto* card = new QFrame(this);
    card->setObjectName("card");
    auto* fl = new QVBoxLayout(card);
    fl->setContentsMargins(24, 20, 24, 20);
    fl->setSpacing(14);

    auto* formTitle = new QLabel("Нова транзакція", card);
    formTitle->setStyleSheet("font-size:16px; font-weight:700; color:#0f172a;");
    fl->addWidget(formTitle);

    auto* row1 = new QHBoxLayout();
    row1->setSpacing(12);

    amountSpin = new QDoubleSpinBox(card);
    amountSpin->setRange(0.01, 1e9);
    amountSpin->setDecimals(2);
    amountSpin->setPrefix("грн ");
    amountSpin->setFixedWidth(160);

    typeCombo = new QComboBox(card);
    typeCombo->addItem("Витрата", "витрата");
    typeCombo->addItem("Дохід",   "дохід");
    typeCombo->setFixedWidth(140);

    categoryEdit = new QLineEdit(card);
    categoryEdit->setPlaceholderText("Категорія");

    descEdit = new QLineEdit(card);
    descEdit->setPlaceholderText("Опис");

    dateEdit = new QDateEdit(card);
    dateEdit->setDate(QDate::currentDate());
    dateEdit->setCalendarPopup(true);
    dateEdit->setDisplayFormat("yyyy-MM-dd");
    dateEdit->setFixedWidth(160);

    auto* addBtn = new QPushButton("Додати", card);
    addBtn->setObjectName("addBtn");
    addBtn->setFixedWidth(120);
    connect(addBtn, &QPushButton::clicked, this, &TransactionsPage::addTransaction);

    row1->addWidget(amountSpin);
    row1->addWidget(typeCombo);
    row1->addWidget(categoryEdit);
    row1->addWidget(descEdit, 1);
    row1->addWidget(dateEdit);
    row1->addWidget(addBtn);
    fl->addLayout(row1);
    root->addWidget(card);

    table = new QTableWidget(this);
    table->setColumnCount(5);
    table->setHorizontalHeaderLabels({"Тип", "Дата", "Категорія", "Опис", "Сума"});
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    table->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    table->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    table->horizontalHeader()->setSectionResizeMode(4, QHeaderView::ResizeToContents);
    table->verticalHeader()->setVisible(false);
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table->setAlternatingRowColors(true);
    root->addWidget(table, 1);

    auto* removeBtn = new QPushButton("Видалити вибране", this);
    removeBtn->setObjectName("removeBtn");
    removeBtn->setFixedWidth(220);
    connect(removeBtn, &QPushButton::clicked, this, &TransactionsPage::removeSelected);
    root->addWidget(removeBtn, 0, Qt::AlignRight);
}

void TransactionsPage::refresh() {
    const auto& txs = manager.getTransactions();
    table->setRowCount(static_cast<int>(txs.size()));
    for (size_t i = 0; i < txs.size(); ++i) {
        int row = static_cast<int>(i);
        const auto& t = txs[i];
        bool isIncome = (t.getType() == "дохід");

        auto* typeItem = new QTableWidgetItem(isIncome ? "Дохід" : "Витрата");
        auto* dateItem = new QTableWidgetItem(QString::fromStdString(t.getDate()));
        auto* catItem  = new QTableWidgetItem(QString::fromStdString(t.getCategory()));
        auto* descItem = new QTableWidgetItem(QString::fromStdString(t.getDescription()));
        auto* amtItem  = new QTableWidgetItem(Currency::format(t.getAmount()));

        amtItem->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
        typeItem->setForeground(isIncome ? QColor("#16a34a") : QColor("#dc2626"));

        table->setItem(row, 0, typeItem);
        table->setItem(row, 1, dateItem);
        table->setItem(row, 2, catItem);
        table->setItem(row, 3, descItem);
        table->setItem(row, 4, amtItem);
        table->setRowHeight(row, 48);
    }
}

void TransactionsPage::addTransaction() {
    QString cat = categoryEdit->text().trimmed();
    if (cat.isEmpty()) { categoryEdit->setStyleSheet("border-color:#ef4444;"); return; }
    categoryEdit->setStyleSheet("");

    double      amount = amountSpin->value();
    std::string type   = typeCombo->currentData().toString().toStdString();
    std::string date   = dateEdit->date().toString("yyyy-MM-dd").toStdString();
    std::string desc   = descEdit->text().toStdString();

    manager.addTransaction(Transaction(amount, type, cat.toStdString(), desc, date));
    amountSpin->setValue(0.01);
    categoryEdit->clear();
    descEdit->clear();
    dateEdit->setDate(QDate::currentDate());
    emit dataChanged();
}

void TransactionsPage::removeSelected() {
    auto selected = table->selectedItems();
    if (selected.isEmpty()) return;
    QSet<int> rows;
    for (auto* item : selected) rows.insert(item->row());
    QList<int> sorted = rows.values();
    std::sort(sorted.begin(), sorted.end(), std::greater<int>());
    for (int row : sorted)
        manager.removeTransaction(static_cast<size_t>(row));
    emit dataChanged();
}
