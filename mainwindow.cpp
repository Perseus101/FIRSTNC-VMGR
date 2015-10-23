#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    model(new TeamMemberListModel(this)),
    memberName(),
    db_file("database.xml"),
    reg()
{
    ui->setupUi(this);

    connect(ui->actionNew_2, SIGNAL(triggered(bool)), this, SLOT(newData()));
    connect(ui->actionOpen_2, SIGNAL(triggered(bool)), this, SLOT(openData()));
    connect(ui->actionSave_2, SIGNAL(triggered(bool)), this, SLOT(saveData()));
    connect(ui->actionRegister, SIGNAL(triggered(bool)), this, SLOT(beginRegister()));
    connect(ui->actionRegister, SIGNAL(triggered(bool)), &reg, SLOT(show()));

    connect(&reg, SIGNAL(registered(TeamMember)), this, SLOT(finishRegister(TeamMember)));

    ui->nameList->setModel(model);
    ui->nameList->setEditTriggers(QAbstractItemView::NoEditTriggers);

    //Populate model with names

    if (db_file.open(QFile::ReadWrite | QFile::Text))
    {
        using namespace rapidxml;
        QTextStream in(&db_file);

        db_text = db.allocate_string("", db_file.size());

        strcpy(db_text, in.readAll().toStdString().c_str());

        db.parse<0>(db_text);
        xml_node<char> *root_node = db.first_node();
        xml_node<char> *team_members_node = root_node->first_node("teammembers");
        QVariant var;

        int i = 0;
        for (xml_node<> *member_data = team_members_node->first_node(); member_data; member_data = member_data->next_sibling())
            i++;

        model->insertRows(0, i);

        i = 0;
        for (xml_node<> *member_data = team_members_node->first_node(); member_data; member_data = member_data->next_sibling(), i++)
        {
            char* name = db.allocate_string("");
            strcat(name, member_data->first_attribute("fname")->value());
            strcat(name, " ");
            strcat(name, member_data->first_attribute("lname")->value());

            TeamMember temp(name);
            temp.email = member_data->first_attribute("eml")->value();
            temp.parentEmail = member_data->first_attribute("peml")->value();
            temp.subteam = member_data->first_attribute("team")->value();
            temp.grade = atoi(member_data->first_attribute("grade")->value());

            var.setValue(temp);
            model->setData(model->index(i), var);
        }
    }
    else
    {
        //TODO handle error
    }
    connect(ui->nameList, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(openMemberView(QModelIndex)));

    //Initialize Member View UI

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    std::string s;
    rapidxml::print(std::back_inserter(s), db);
    db_file.resize(0);
    db_file.write(s.c_str());
}

void MainWindow::newData()
{
    qDebug() << "new";
}

void MainWindow::openData()
{
    qDebug() << "open";
}

void MainWindow::saveData()
{
    qDebug() << "save";
}
void MainWindow::beginRegister()
{

}
void MainWindow::finishRegister(TeamMember member)
{
    // Put new member into database
    rapidxml::xml_document<> member_data;
    char* parse_data = db.allocate_string(member.getXML().toStdString().c_str());
    member_data.parse<0>(parse_data);
    rapidxml::xml_node<> *clone = db.clone_node(member_data.first_node());
    db.first_node()->first_node()->append_node(clone);

    //Display new member in member list
    model->insertRows(model->rowCount(), 1);
    QVariant temp;
    temp.setValue(member);
    model->setData(model->index(model->rowCount()-1), temp);
    model->refresh();
}
void MainWindow::openMemberView(QModelIndex index)
{
    TeamMember member = qvariant_cast<TeamMember>(model->data(index, 6));

    ui->name->setText(member.name);
    ui->grade->setText(((member.grade == 13) ? "Mentor":QString("%1th Grade").arg(member.grade)));
    ui->team->setText(member.subteam);
    ui->email->setText(member.email);
    ui->parentEmail->setText(member.parentEmail);
}
