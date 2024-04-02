#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

Dick::Dick(double radius1, double radius2) : Wall(radius1) {
    RadiusInside = radius2;
    index = indexNumber;
}

Dick::~Dick() {
    // деструктор Dick
}
