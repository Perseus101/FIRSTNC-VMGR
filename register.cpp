#include "register.h"
#include "ui_register.h"

Register::Register(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Register)
{
    ui->setupUi(this);
    ui->team->setInsertPolicy(QComboBox::InsertAtBottom);

    connect(ui->registerButton, SIGNAL(pressed()), this, SLOT(registerFinished()));
}

Register::~Register()
{
    delete ui;
}

void Register::registerFinished()
{
    TeamMember temp(ui->firstName->text() + " " + ui->lastName->text());
    if(ui->grade->currentText() == "9th Grade")
    {
        temp.grade = 9;
    }
    else if(ui->grade->currentText() == "10th Grade")
    {
        temp.grade = 10;
    }
    else if(ui->grade->currentText() == "11th Grade")
    {
        temp.grade = 11;
    }
    else if(ui->grade->currentText() == "12th Grade")
    {
        temp.grade = 12;
    }
    else if(ui->grade->currentText() == "Mentor")
    {
        temp.grade = 13;
    }

    temp.subteam = ui->team->currentText();
    temp.email = ui->personalEmail->text();
    temp.parentEmail = ui->parentEmail->text();
    hide();

    ui->firstName->setText("");
    ui->lastName->setText("");
    ui->personalEmail->setText("");
    ui->parentEmail->setText("");
    ui->grade->setCurrentIndex(0);
    ui->team->setCurrentIndex(0);

    emit registered(temp);
}
