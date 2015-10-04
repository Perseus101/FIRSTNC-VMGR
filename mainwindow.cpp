#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    model(new TeamMemberListModel(this)),
    memberName(),
    db_file("database.xml")
{
    ui->setupUi(this);

    connect(ui->actionNew, SIGNAL(toggled(bool)), this, SLOT(newData()));
    connect(ui->actionOpen, SIGNAL(toggled(bool)), this, SLOT(openData()));
    connect(ui->actionSave, SIGNAL(toggled(bool)), this, SLOT(saveData()));

    ui->nameList->setModel(model);
    ui->nameList->setEditTriggers(QAbstractItemView::NoEditTriggers);

    //Populate model with names

    if (db_file.open(QFile::ReadOnly | QFile::Text))
    {
        using namespace rapidxml;
        QTextStream in(&db_file);

        db_text = new char[db_file.size()];

        strcpy(db_text, in.readAll().toStdString().c_str());

        db.parse<0>(db_text);
        xml_node<char> * root_node = db.first_node();
        xml_node<char> * team_members_node = root_node->first_node("teammembers");

        QVariant var;

        int i = 0;
        for (xml_node<> *member_data = team_members_node->first_node(); member_data; member_data = member_data->next_sibling(), i++)
        {
            TeamMember member(member_data->first_attribute("name")->value());
            var.setValue(member);
            model->setData(model->index(i,0), var);
        }
        qDebug() << i;
    }
    else
    {
        //TODO handle error
    }
    connect(ui->nameList, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(openMemberView(QModelIndex)));

    //Initialize Member View UI

    memberName.setParent(ui->memberView);
    memberName.move(0, 0);
    memberName.setFont(QFont("Arial", 24));
    memberName.setWordWrap(true);
    memberName.setText("                                         ");


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
