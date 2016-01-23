#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    model(new TeamMemberListModel(this)),
    reg(),
    readingBarcode(false)
{
    ui->setupUi(this);

    connect(ui->actionNew_2, SIGNAL(triggered(bool)), this, SLOT(newData()));
    connect(ui->actionOpen_2, SIGNAL(triggered(bool)), this, SLOT(openData()));
    connect(ui->actionSave_2, SIGNAL(triggered(bool)), this, SLOT(saveData()));
    connect(ui->actionExport_Nametags, SIGNAL(triggered(bool)), this, SLOT(exportData()));
    connect(ui->actionRegister, SIGNAL(triggered(bool)), this, SLOT(beginRegister()));
    connect(ui->actionRegister, SIGNAL(triggered(bool)), &reg, SLOT(show()));
    connect(ui->signIn, SIGNAL(clicked(bool)), this, SLOT(signIn()));
    connect(ui->signOut, SIGNAL(clicked(bool)), this, SLOT(signOut()));
    connect(ui->barcodeInput, SIGNAL(textChanged(QString)), this, SLOT(startBarcodeRead()));
    connect(ui->nameList, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(openMemberView(QModelIndex)));

    connect(&reg, SIGNAL(registered(TeamMember)), this, SLOT(finishRegister(TeamMember)));

    // Set up the list of names to be populated from the database

    ui->nameList->setModel(model);
    ui->nameList->setEditTriggers(QAbstractItemView::NoEditTriggers); // Prevent editing data

    openData("database.db");

    // Set up tables on admin panel showing in and out times
    ui->inTimes->setColumnCount(2);
    ui->inTimes->setRowCount(model->memberList.size());
    ui->inTimes->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch); //Make the columns fill the whitespace
    ui->inTimes->verticalHeader()->setVisible(false);

    ui->outTimes->setColumnCount(2);
    ui->outTimes->setRowCount(model->memberList.size());
    ui->outTimes->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch); //Make the columns fill the whitespace
    ui->outTimes->verticalHeader()->setVisible(false);

    QStringList headers;
    headers << "Member" << "In Time";
    ui->inTimes->setHorizontalHeaderLabels(headers);
    headers.replace(1, "Out Time");
    ui->outTimes->setHorizontalHeaderLabels(headers);

    int i = 0;
    for(QList<TeamMember>::iterator it = model->memberList.begin(); it != model->memberList.end(); it++, i++)
    {
        ui->inTimes->setItem(i, 0, new QTableWidgetItem(it->name));

        ui->outTimes->setItem(i, 0, new QTableWidgetItem(it->name));
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::saveDatabase(QString filename)
{
    using namespace rapidxml;
    char* nextUidString = db.allocate_string("", 8);
    sprintf(nextUidString, "%d", nextUid);
    db.first_node()->first_node("teammembers")->first_node("uid")->value(nextUidString);

    xml_node<> *root_node = db.first_node();
    xml_node<> *team_members_node = root_node->first_node("teammembers");

    //Save the in time and out time of each member who logged in at the event
    int i = 0;
    for (xml_node<> *member_data = team_members_node->first_node("member"); member_data; member_data = member_data->next_sibling(), i++)
    {
        //Build the element to be inserted
        if(model->memberList.at(i).in_time.isValid())
        {
            QString insertDataString = QString("<event name=\"%1\" date=\"%2\" inTime=\"%3\" outTime=\"%4\"/>")
                    .arg("Regular Meeting").arg(QDate::currentDate().toString("MM-dd-yyyy"))
                    .arg(model->memberList.at(i).in_time.toString("hh:mm:ss"))
                    .arg(model->memberList.at(i).out_time.isValid()?
                             model->memberList.at(i).out_time.toString("hh:mm:ss"):
                             QDateTime::currentDateTime().toString("hh:mm:ss"));

            // Parse the data and insert it into the database
            rapidxml::xml_document<> insertData;
            char* parse_data = db.allocate_string(insertDataString.toStdString().c_str());
            insertData.parse<0>(parse_data);
            rapidxml::xml_node<> *clone = db.clone_node(insertData.first_node());
            member_data->append_node(clone);
        }
        else
        {
            // Member didn't sign in
            continue;
        }
    }

    std::string s;
    rapidxml::print(std::back_inserter(s), db);

    QFile db_file(filename);
    if(db_file.open(QFile::ReadWrite | QFile::Text))
    {
        db_file.resize(0);
        db_file.write(s.c_str());
    }
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    saveDatabase("database.db");
}

bool MainWindow::newData()
{
    QMessageBox msgBox;
    msgBox.setText("Would you like to save this database?");
    msgBox.setInformativeText("If you do not save this database, all it's contents will be PERMANENTLY erased.");
    msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Save);
    int ret = msgBox.exec();
    switch(ret)
    {
    case QMessageBox::Save:
        saveDatabase(QFileDialog::getSaveFileName(this, QString(), QString(), tr("Database Files (*.db)")));
        break;
    case QMessageBox::Cancel:
        return false;
        break;
    case QMessageBox::Discard:
        break;
    }
    // Reset the database file to default
    if(QFile::exists("db_template.xml"))
    {
        QFile::remove("database.db");
        QFile::copy("db_template.xml", "database.db");
    }
    // Purge all members that were loaded into the model
    model->memberList.clear();
    // Clear out the member view
    closeMemberView();
    return true;
}

void MainWindow::openData()
{
    // Clear out whatever is currently in the database
    if(newData())
        // Load new data
        openData(QFileDialog::getOpenFileName(this, QString(), QString(), tr("Database Files (*.db);; All Files (*.*)")));
}

void MainWindow::openData(QString filename)
{
    //Populate model with names
    QFile db_file(filename);
    if(db_file.open(QFile::ReadWrite | QFile::Text))
    {
        using namespace rapidxml;
        QTextStream in(&db_file);

        char* db_text = db.allocate_string("", db_file.size());
        strcpy(db_text, in.readAll().toStdString().c_str());

        // Use parse_no_data_nodes so that the data can be edited later
        db.parse<parse_no_data_nodes>(db_text);
        xml_node<> *root_node = db.first_node();
        if(!root_node)
        {
            qDebug() << "Invalid File Format"; // TODO Error Handling
            openData("db_template.xml");
            return;
        }

        xml_node<> *team_members_node = root_node->first_node("teammembers");
        if(!team_members_node)
        {
            qDebug() << "Invalid File Format"; // TODO Error Handling
            openData("db_template.xml");
            return;
        }

        nextUid = atoi(team_members_node->first_node("uid")->value());

        QVariant var;
        int i = 0;
        for (xml_node<> *member_data = team_members_node->first_node(); member_data; member_data = member_data->next_sibling())
            i++;

        if(i != 0)
            model->insertRows(0, i-1);

        i = 0;
        for (xml_node<> *member_data = team_members_node->first_node("member"); member_data; member_data = member_data->next_sibling(), i++)
        {
            char* name = db.allocate_string("");
            strcat(name, member_data->first_attribute("fname")->value());
            strcat(name, " ");
            strcat(name, member_data->first_attribute("lname")->value());

            TeamMember temp(name, atoi(member_data->first_attribute("uid")->value()));
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
    openMemberView(model->index(0));
}

void MainWindow::saveData()
{
    QString filter = QString("Database Files (*.db)");
    saveDatabase(QFileDialog::getSaveFileName(this, QString(), QString(), filter, &filter));
}

void MainWindow::exportData()
{
    using namespace rapidxml;

    xml_document<> doc;
    QFile templateFile ("template.html");
    if(templateFile.open(QFile::ReadWrite | QFile::Text))
    {
        QTextStream in(&templateFile);

        char* templateText = db.allocate_string("", templateFile.size());
        strcpy(templateText, in.readAll().toStdString().c_str());
        doc.parse<0>(templateText);
    }
    else
        return;

    xml_node<> *body = doc.first_node("body");

    QString nametags("<table><tbody>");
    // Populate the table with nametags

    for(QList<TeamMember>::iterator member = model->memberList.begin(); member != model->memberList.end(); member++)
    {
        //Start row
        nametags.append("<tr><td class=\"name\">");

        //First Column
        nametags.append(member->name);
        nametags.append("<br/>");
        nametags.append(member->subteam);
        nametags.append("</td><td class=\"barcode\">");
        nametags.append(QString("*%1*").arg(member->uid, 8, 10, QChar('0')));
        nametags.append("</td><td class=\"name\">");

        //Second Column
        member++;
        if(member == model->memberList.end())
        {
            nametags.append("</td><td class=\"barcode\"></td></tr>");
            break;
        }
        nametags.append(member->name);
        nametags.append("<br/>");
        nametags.append(member->subteam);
        nametags.append("</td><td class=\"barcode\">");
        nametags.append(QString("*%1*").arg(member->uid, 8, 10, QChar('0')));
        nametags.append("</td>");

        nametags.append("</tr>");
        //End row
    }
    // End the table
    nametags.append("</tbody></table>");

    qDebug() << nametags;

    rapidxml::xml_document<> tableData;
    char* parse_data = doc.allocate_string(nametags.toStdString().c_str());
    tableData.parse<0>(parse_data);
    rapidxml::xml_node<> *clone = doc.clone_node(tableData.first_node());
    doc.first_node()->first_node()->append_node(clone);

    std::string s;
    rapidxml::print(std::back_inserter(s), doc);
    QString str(s.c_str());
    str.replace("&quot;", "\"");
    s = str.toStdString();
    QString exportFileName = QFileDialog::getSaveFileName(this, QString(), QString(), tr("HTML Files (*.html)"));
    QFile exportFile (exportFileName);
    if(exportFile.open(QFile::ReadWrite | QFile::Text))
    {
        exportFile.resize(0);
        exportFile.write(s.c_str());
    }
}

void MainWindow::beginRegister()
{

}

void MainWindow::finishRegister(TeamMember member)
{
    //Assign the new member a uid
    member.uid = nextUid;
    nextUid++;

    // Put new member into database
    rapidxml::xml_document<> member_data;
    char* parse_data = db.allocate_string(member.getXML().toStdString().c_str());
    member_data.parse<0>(parse_data);
    rapidxml::xml_node<> *clone = db.clone_node(member_data.first_node());
    db.first_node()->first_node()->append_node(clone);

    //Display new member in member list
    QVariant temp;
    temp.setValue(member);
    model->setData(model->index(model->rowCount()-1), temp);
    model->refresh();
}

void MainWindow::openMemberView(QModelIndex index)
{
    if(!index.isValid())
        return;
    TeamMember member = qvariant_cast<TeamMember>(model->data(index, 6));

    ui->name->setText(member.name);
    ui->grade->setText(((member.grade == 13) ? "Mentor":QString("%1th Grade").arg(member.grade)));
    ui->team->setText(member.subteam);
    ui->email->setText(member.email);
    ui->parentEmail->setText(member.parentEmail);

    if(member.in_time.isValid())
    {
        ui->inTime->setText(QString("Signed in at %1").arg(member.in_time.toString("hh:mm:ss")));
        ui->signIn->setDisabled(true);
    }
    else
    {
        ui->inTime->setText(QString(""));
        ui->signIn->setDisabled(false);
    }
    if(member.out_time.isValid())
    {
        ui->outTime->setText(QString("Signed out at %1").arg(member.out_time.toString("hh:mm:ss")));
        ui->signOut->setDisabled(true);
    }
    else
    {
        ui->outTime->setText(QString(""));
        ui->signOut->setDisabled(false);
    }
    selectedMember = index;
}

void MainWindow::closeMemberView()
{
    ui->name->setText("");
    ui->grade->setText("");
    ui->team->setText("");
    ui->email->setText("");
    ui->parentEmail->setText("");

    ui->inTime->setText(QString(""));
    ui->outTime->setText(QString(""));

    ui->signIn->setDisabled(false);
    ui->signOut->setDisabled(false);

    selectedMember = model->index(-1);
}

void MainWindow::startBarcodeRead()
{
    if(!readingBarcode)
    {
        //Give the reader time to write all the characters of the barcode
        //If this is regular keyboard input, it will take longer than 100 ms, and then be cleared out
        QTimer::singleShot(100, this, SLOT(endBarcodeRead()));
        readingBarcode = true;
    }
}

void MainWindow::endBarcodeRead()
{
    if(QRegExp("^[0-9]{1,8}$").exactMatch(ui->barcodeInput->text()))// If the input text is exactly 8 numeric characters, it matches the barcode format we are looking for.
    {
        int searchUid = ui->barcodeInput->text().toInt();
        //Input matches valid barcode format
        int i = 0;
        for(QList<TeamMember>::iterator it = model->memberList.begin(); it != model->memberList.end(); it++, i++)
        {
            if(it->uid == searchUid)
                openMemberView(model->index(i));
        }
        if(!(model->memberList.at(selectedMember.row()).in_time.isValid()))
            signIn();
    }
    ui->barcodeInput->setText(QString(""));
    readingBarcode = false;
}

void MainWindow::signIn()
{
    if(selectedMember.isValid())
    {
        TeamMember temp = model->memberList.at(selectedMember.row());
        temp.in_time = QDateTime::currentDateTime();
        model->memberList.replace(selectedMember.row(), temp);
        ui->signIn->setDisabled(true);
        ui->inTime->setText(QString("Signed in at %1").arg(model->memberList.at(selectedMember.row()).in_time.toString("hh:mm:ss")));

        ui->inTimes->setItem(selectedMember.row(), 1, new QTableWidgetItem(model->memberList.at(selectedMember.row()).in_time.toString("hh:mm:ss")));
    }
}

void MainWindow::signOut()
{
    if(selectedMember.isValid())
    {
        if(!(model->memberList.at(selectedMember.row()).in_time.isValid())) // User isn't signed in, therefore can't sign out.
            return;
        TeamMember temp = model->memberList.at(selectedMember.row());
        temp.out_time = QDateTime::currentDateTime();
        model->memberList.replace(selectedMember.row(), temp);
        ui->signOut->setDisabled(true);
        ui->outTime->setText(QString("Signed out at %1").arg(model->memberList.at(selectedMember.row()).out_time.toString("hh:mm:ss")));

        ui->outTimes->setItem(selectedMember.row(), 1, new QTableWidgetItem(model->memberList.at(selectedMember.row()).out_time.toString("hh:mm:ss")));
    }
}
