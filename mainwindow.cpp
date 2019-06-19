#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_manager = new GsmManager(false);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_send_clicked()
{
    m_manager->sendSms(ui->lineEdit->text(),ui->plainTextEdit->toPlainText());
}
