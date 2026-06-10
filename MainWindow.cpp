#include "MainWindow.h"
#include "DashboardPage.h"
#include "TransactionsPage.h"
#include "GoalsPage.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QTimer>
#include <QMessageBox>

bool Currency::isUSD = false;

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {
    Storage::load(manager);
    setupUi();
    applyStylesheet();
    refreshAll();
    auto* timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::saveData);
    timer->start(60000);
}

MainWindow::~MainWindow() { saveData(); }

void MainWindow::setupUi() {
    setWindowTitle("Трекер фінансів");
    setMinimumSize(1000, 680);
    resize(1200, 780);
    centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    auto* root = new QHBoxLayout(centralWidget);
    root->setContentsMargins(0, 0, 0, 0);
    root->setSpacing(0);
    setupSidebar();
    root->addWidget(sidebar);
    stack = new QStackedWidget(centralWidget);
    dashPage = new DashboardPage(manager, stack);
    txPage   = new TransactionsPage(manager, stack);
    goalPage = new GoalsPage(manager, stack);
    stack->addWidget(dashPage);
    stack->addWidget(txPage);
    stack->addWidget(goalPage);
    root->addWidget(stack, 1);
    connect(txPage,   &TransactionsPage::dataChanged, this, &MainWindow::refreshAll);
    connect(goalPage, &GoalsPage::dataChanged,        this, &MainWindow::refreshAll);
}

void MainWindow::setupSidebar() {
    sidebar = new QWidget(centralWidget);
    sidebar->setObjectName("sidebar");
    sidebar->setFixedWidth(220);
    auto* lay = new QVBoxLayout(sidebar);
    lay->setContentsMargins(0, 0, 0, 0);
    lay->setSpacing(0);
    auto* logo = new QLabel("Трекер\nфінансів", sidebar);
    logo->setObjectName("sidebarLogo");
    logo->setAlignment(Qt::AlignCenter);
    logo->setFixedHeight(80);
    lay->addWidget(logo);
    const char* labels[3] = {"Дашборд", "Транзакції", "Цілі"};
    for (int i = 0; i < 3; ++i) {
        auto* btn = new QPushButton(labels[i], sidebar);
        btn->setObjectName("navBtn");
        btn->setCheckable(true);
        btn->setFixedHeight(58);
        btn->setCursor(Qt::PointingHandCursor);
        connect(btn, &QPushButton::clicked, this, [this, i]() { switchPage(i); });
        lay->addWidget(btn);
        navButtons[i] = btn;
    }
    lay->addStretch();
    auto* currLabel = new QLabel("Валюта:", sidebar);
    currLabel->setAlignment(Qt::AlignCenter);
    currLabel->setStyleSheet("color:#6b7280; font-size:14px;");
    lay->addWidget(currLabel);
    currencyBtn = new QPushButton("Гривня (грн)", sidebar);
    currencyBtn->setObjectName("currencyBtn");
    currencyBtn->setFixedHeight(50);
    currencyBtn->setCursor(Qt::PointingHandCursor);
    connect(currencyBtn, &QPushButton::clicked, this, [this]() {
        Currency::isUSD = !Currency::isUSD;
        currencyBtn->setText(Currency::isUSD ? "Долар ($)" : "Гривня (грн)");
        refreshAll();
    });
    lay->addWidget(currencyBtn);
    auto* saveBtn = new QPushButton("Зберегти", sidebar);
    saveBtn->setObjectName("saveBtn");
    saveBtn->setFixedHeight(50);
    saveBtn->setCursor(Qt::PointingHandCursor);
    connect(saveBtn, &QPushButton::clicked, this, [this]() {
        saveData();
        QMessageBox::information(this, "Збережено", "Дані збережено!");
    });
    lay->addWidget(saveBtn);
    auto* ver = new QLabel("v1.0", sidebar);
    ver->setAlignment(Qt::AlignCenter);
    ver->setObjectName("versionLabel");
    ver->setFixedHeight(30);
    lay->addWidget(ver);
}

void MainWindow::switchPage(int index) {
    stack->setCurrentIndex(index);
    for (int i = 0; i < 3; ++i)
        navButtons[i]->setChecked(i == index);
    currentPage = index;
}

void MainWindow::saveData() { Storage::save(manager); }
void MainWindow::refreshAll() {
    saveData();
    dashPage->refresh();
    txPage->refresh();
    goalPage->refresh();
}

void MainWindow::applyStylesheet() {
    navButtons[0]->setChecked(true);
    setStyleSheet(R"(
        QMainWindow, QWidget { font-family: Ubuntu, sans-serif; font-size: 15px; }
        QWidget#sidebar { background-color: #14532d; }
        QLabel#sidebarLogo { color: #ffffff; font-size: 17px; font-weight: 700; border-bottom: 1px solid #166534; padding: 8px; }
        QPushButton#navBtn { background: transparent; color: #bbf7d0; border: none; text-align: left; padding: 0 28px; font-size: 16px; }
        QPushButton#navBtn:hover { background: #166534; color: #ffffff; }
        QPushButton#navBtn:checked { background: #052e16; color: #4ade80; border-left: 4px solid #4ade80; }
        QPushButton#currencyBtn { background: #166534; color: #dcfce7; border: none; font-size: 14px; font-weight: 600; margin: 4px 16px; border-radius: 8px; }
        QPushButton#currencyBtn:hover { background: #15803d; }
        QPushButton#saveBtn { background: #4ade80; color: #052e16; border: none; font-size: 15px; font-weight: 700; margin: 4px 16px 10px 16px; border-radius: 8px; }
        QPushButton#saveBtn:hover { background: #22c55e; }
        QLabel#versionLabel { color: #166534; font-size: 12px; }
        QStackedWidget, QWidget#pageRoot { background: #f0fdf4; }
        QLabel#pageTitle { color: #052e16; font-size: 26px; font-weight: 700; }
        QFrame#card { background: white; border-radius: 12px; border: 1px solid #bbf7d0; }
        QLabel#statTitle { color: #4b7c5e; font-size: 13px; font-weight: 600; }
        QLabel#statIncome { color: #16a34a; font-size: 28px; font-weight: 700; }
        QLabel#statExpense { color: #dc2626; font-size: 28px; font-weight: 700; }
        QLabel#statBalance { color: #15803d; font-size: 28px; font-weight: 700; }
        QTableWidget { background: white; border: 1px solid #bbf7d0; border-radius: 10px; font-size: 15px; }
        QTableWidget::item { padding: 12px 14px; color: #1a2e1a; }
        QTableWidget::item:selected { background: #dcfce7; color: #14532d; }
        QTableWidget::item:alternate { background: #f0fdf4; }
        QHeaderView::section { background: #dcfce7; color: #14532d; font-size: 14px; font-weight: 600; padding: 12px 14px; border: none; border-bottom: 1px solid #bbf7d0; }
        QLineEdit, QDoubleSpinBox, QDateEdit, QComboBox { background: white; border: 2px solid #bbf7d0; border-radius: 8px; padding: 10px 14px; font-size: 15px; color: #1a2e1a; min-height: 42px; }
        QLineEdit:focus, QDoubleSpinBox:focus, QDateEdit:focus, QComboBox:focus { border-color: #22c55e; }
        QComboBox::drop-down { border: none; width: 28px; }
        QPushButton#addBtn { background: #22c55e; color: white; border: none; border-radius: 8px; padding: 12px 24px; font-size: 15px; font-weight: 700; min-height: 44px; }
        QPushButton#addBtn:hover { background: #16a34a; }
        QPushButton#removeBtn { background: #fee2e2; color: #dc2626; border: none; border-radius: 8px; padding: 10px 22px; font-size: 14px; font-weight: 600; }
        QPushButton#removeBtn:hover { background: #fecaca; }
        QPushButton#depositBtn { background: #dcfce7; color: #15803d; border: none; border-radius: 7px; padding: 10px 20px; font-size: 14px; font-weight: 600; }
        QPushButton#depositBtn:hover { background: #bbf7d0; }
        QFrame#goalCard { background: white; border-radius: 12px; border: 1px solid #bbf7d0; }
        QLabel#cardTitle { color: #052e16; font-size: 17px; font-weight: 600; }
        QLabel#cardSub { color: #4b7c5e; font-size: 14px; }
        QProgressBar { border: none; border-radius: 5px; background: #dcfce7; max-height: 12px; }
        QProgressBar::chunk { border-radius: 5px; background: #22c55e; }
        QScrollArea { border: none; background: transparent; }
        QScrollBar:vertical { background: #dcfce7; width: 7px; border-radius: 3px; }
        QScrollBar::handle:vertical { background: #86efac; border-radius: 3px; }
        QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical { height: 0; }
    )");
}
