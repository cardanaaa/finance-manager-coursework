cmake_minimum_required(VERSION 3.16)
project(FinanceTracker VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets REQUIRED)

set(PROJECT_SOURCES
    main.cpp
    storage/Storage.h
    models/Budget.h
    models/Goal.h
    models/Transaction.h
    models/Manager.h
    models/Manager.cpp
    ui/MainWindow.h
    ui/MainWindow.cpp
    ui/DashboardPage.h
    ui/DashboardPage.cpp
    ui/TransactionsPage.h
    ui/TransactionsPage.cpp
    ui/GoalsPage.h
    ui/GoalsPage.cpp
)

add_executable(FinanceTracker ${PROJECT_SOURCES})

target_link_libraries(FinanceTracker PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

target_include_directories(FinanceTracker PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})