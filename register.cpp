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
    TeamMember temp(teamMembersNode, ui->firstName->text(), ui->lastName->text(), ui->team->currentText(), ui->personalEmail->text(), ui->parentEmail->text(), "", nextUid++);

    hide();

    ui->firstName->setText("");
    ui->lastName->setText("");
    ui->personalEmail->setText("");
    ui->parentEmail->setText("");
    ui->grade->setCurrentIndex(0);
    ui->team->setCurrentIndex(0);

    emit registered(temp);
}
