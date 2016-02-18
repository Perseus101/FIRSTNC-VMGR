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
    TeamMember temp(ui->firstName->text(), ui->lastName->text());
    //temp.age = 0; //TODO

    temp.title = ui->team->currentText();
    temp.email = ui->personalEmail->text();
    temp.phone = ui->parentEmail->text();
    hide();

    ui->firstName->setText("");
    ui->lastName->setText("");
    ui->personalEmail->setText("");
    ui->parentEmail->setText("");
    ui->grade->setCurrentIndex(0);
    ui->team->setCurrentIndex(0);

    emit registered(temp);
}
