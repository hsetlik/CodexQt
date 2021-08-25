#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QtWidgets>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::CodexQt)
{
    ui->setupUi(this);
    ui->centralwidget = ui->verticalLayoutWidget;
    auto stack = new MasterStackedWidget();
    ui->verticalLayout->addWidget(stack);
}

MainWindow::~MainWindow()
{
    delete ui;
}

