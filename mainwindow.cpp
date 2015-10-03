#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    model(new TeamMemberListModel(this)),
    memberName(), signIn(), signOut()
{
    ui->setupUi(this);

    connect(ui->actionNew, SIGNAL(toggled(bool)), this, SLOT(newData()));
    connect(ui->actionOpen, SIGNAL(toggled(bool)), this, SLOT(openData()));
    connect(ui->actionSave, SIGNAL(toggled(bool)), this, SLOT(saveData()));

    ui->nameList->setModel(model);
    ui->nameList->setEditTriggers(QAbstractItemView::NoEditTriggers);

    //Populate model with names

    QVariant var;

    model->insertRows(0,5);
    var.setValue(TeamMember("Test"));
    model->setData(model->index(0,0), var);
    var.setValue(TeamMember("Test"));
    model->setData(model->index(1,0), var);
    var.setValue(TeamMember("Test"));
    model->setData(model->index(2,0), var);
    var.setValue(TeamMember("Test"));
    model->setData(model->index(3,0), var);
    var.setValue(TeamMember("Test"));
    model->setData(model->index(4,0), var);

    connect(ui->nameList, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(openMemberView(QModelIndex)));

    //Initialize Member View UI

    memberName.setParent(ui->memberView);
    memberName.move(0, 0);
    memberName.setFont(QFont("Arial", 24));
    memberName.setWordWrap(true);
    memberName.setText("                                         ");

    signIn.setParent(ui->memberView);
    signIn.move(0, 145);
    signIn.setText("Sign In");
    signIn.setFont(QFont("Arial", 24));
    signIn.show();

    signOut.setParent(ui->memberView);
    signOut.move(110, 145);
    signOut.setText("Sign Out");
    signOut.setFont(QFont("Arial", 24));
    signOut.show();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::newData()
{

}

void MainWindow::openData()
{

}

void MainWindow::saveData()
{

}

void MainWindow::openMemberView(QModelIndex index)
{
    memberName.setText((model->data(index, Qt::DisplayRole)).toString());
}
