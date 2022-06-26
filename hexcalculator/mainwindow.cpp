#include "mainwindow.h"
#include "ui_mainwindow.h"

int num = 0x0;
int oldNum = 0;
int op = 0;
int flag = 0;
int equPressedFlag = 0;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->label->setText("0");
    connect(ui->button0,SIGNAL(released()),this,SLOT(newNumber()));
    connect(ui->button1,SIGNAL(released()),this,SLOT(newNumber()));
    connect(ui->button2,SIGNAL(released()),this,SLOT(newNumber()));
    connect(ui->button3,SIGNAL(released()),this,SLOT(newNumber()));
    connect(ui->button4,SIGNAL(released()),this,SLOT(newNumber()));
    connect(ui->button5,SIGNAL(released()),this,SLOT(newNumber()));
    connect(ui->button6,SIGNAL(released()),this,SLOT(newNumber()));
    connect(ui->button7,SIGNAL(released()),this,SLOT(newNumber()));
    connect(ui->button8,SIGNAL(released()),this,SLOT(newNumber()));
    connect(ui->button9,SIGNAL(released()),this,SLOT(newNumber()));
    connect(ui->buttona,SIGNAL(released()),this,SLOT(newNumber()));
    connect(ui->buttonb,SIGNAL(released()),this,SLOT(newNumber()));
    connect(ui->buttonc,SIGNAL(released()),this,SLOT(newNumber()));
    connect(ui->buttond,SIGNAL(released()),this,SLOT(newNumber()));
    connect(ui->buttone,SIGNAL(released()),this,SLOT(newNumber()));
    connect(ui->buttonf,SIGNAL(released()),this,SLOT(newNumber()));
    connect(ui->add,SIGNAL(released()),this,SLOT(addPressed()));
    connect(ui->sub,SIGNAL(released()),this,SLOT(subPressed()));
    connect(ui->equ,SIGNAL(released()),this,SLOT(equPressed()));
    connect(ui->clr,SIGNAL(released()),this,SLOT(clrPressed()));


}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::newNumber(){
    if(equPressedFlag == 1){
        equPressedFlag = 0;
        num = 0;
    }
    QPushButton *button = (QPushButton *)sender();
    QString buttonText = button->text();
    int curButton = 0;

    switch(buttonText.back().unicode()){
        case '0': curButton = 0x0; break;
        case '1': curButton = 0x1; break;
        case '2': curButton = 0x2; break;
        case '3': curButton = 0x3; break;
        case '4': curButton = 0x4; break;
        case '5': curButton = 0x5; break;
        case '6': curButton = 0x6; break;
        case '7': curButton = 0x7; break;
        case '8': curButton = 0x8; break;
        case '9': curButton = 0x9; break;
        case 65: curButton = 0xA; break;
        case 66: curButton = 0xB; break;
        case 67: curButton = 0xC; break;
        case 68: curButton = 0xD; break;
        case 69: curButton = 0xE; break;
        case 70: curButton = 0xF; break;
        default: curButton = 5;
    };
    num = num*16 + curButton;
    //ui->label->setText(QString::fromStdString(dectohex(num)));
    ui->label->setText(QString::number(num, 16));

}

void MainWindow::addPressed(){

    flag = 0;
    if(!equPressedFlag)
        MainWindow::equPressed();
    flag = 0;
    equPressedFlag = 0;
    op = 0;
    oldNum = num;
    num = 0;

}

void MainWindow::subPressed(){

    flag = 0;
    if(!equPressedFlag)
        MainWindow::equPressed();
    flag = 0;
    equPressedFlag = 0;
    op = 1;
    oldNum = num;
    num = 0;

}

void MainWindow::clrPressed(){

    op = 0;
    oldNum = 0;
    num = 0;
    flag = 0;
    ui->label->setText(QString::number(num, 16));
}

void MainWindow::equPressed(){
    if(!flag){
        std::swap(oldNum, num);
        flag = 1;
    }
    if(op == 0){
        num = num + oldNum;

    } else{
        num =  num - oldNum;
    }
    ui->label->setText(QString::number(num, 16));

    equPressedFlag = 1;
}


