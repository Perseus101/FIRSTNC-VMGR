#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    listModel(new TeamMemberListModel(this)),
    tableModel(new TeamMemberTableModel(this, &(listModel->memberList))),
    reg(),
    readingBarcode(false)
{
    ui->setupUi(this);

    connect(ui->actionNew_2, SIGNAL(triggered(bool)), this, SLOT(newData()));
    connect(ui->actionOpen_2, SIGNAL(triggered(bool)), this, SLOT(openData()));
    connect(ui->actionSave_2, SIGNAL(triggered(bool)), this, SLOT(saveData()));
    connect(ui->actionExport_Nametags, SIGNAL(triggered(bool)), this, SLOT(exportNametags()));
    connect(ui->actionExport_Spreadsheet, SIGNAL(triggered(bool)), this, SLOT(exportData()));
    connect(ui->actionImport, SIGNAL(triggered(bool)), this, SLOT(importData()));
    connect(ui->actionRegister, SIGNAL(triggered(bool)), this, SLOT(beginRegister()));
    connect(ui->actionRegister, SIGNAL(triggered(bool)), &reg, SLOT(show()));
    connect(ui->barcodeInput, SIGNAL(textChanged(QString)), this, SLOT(startBarcodeRead()));
    connect(ui->nameList, SIGNAL(clicked(QModelIndex)), this, SLOT(openMemberView(QModelIndex)));
    connect(ui->comments, SIGNAL(textChanged()), this, SLOT(updateSelectedComments()));

    connect(tableModel, SIGNAL(dataChanged(QModelIndex,QModelIndex)), this, SLOT(updateMemberData(QModelIndex,QModelIndex)));
    connect(&reg, SIGNAL(registered(TeamMember)), this, SLOT(finishRegister(TeamMember)));

//    connect(this, SIGNAL(updateTables()), ui->nameList, SLOT(refresh))

    // Set up the list of names to be populated from the database

    ui->nameList->setModel(listModel);
    ui->nameList->setEditTriggers(QAbstractItemView::NoEditTriggers); // Prevent editing data
    try
    {
        openData("database.db");
        reg.teamMembersNode = db.first_node()->first_node("teammembers");
    }
    catch(rapidxml::parse_error &e)
    {
        qDebug() << "RapidXML exception!" << e.what();
        openData("db_template.xml");
    }
    // Set up the admin panel
    ui->adminTable->setModel(tableModel);
    ui->adminTable->setItemDelegate(new TeamMemberTableDelegate());
}

MainWindow::~MainWindow()
{
    delete ui;
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
        //Clear out all currently loaded data
        listModel->memberList.clear();
        //Load the now blank database
        openData("database.db");
        return true;
    }
    else
    {
        //Database template cannot be found; crash
        //TODO something other than crash
        exit(-1);
    }
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
    //Populate listModel with names
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

        int i = 0;
        for (xml_node<> *member_data = team_members_node->first_node(); member_data; member_data = member_data->next_sibling())
            i++;

        if(i != 0)
            listModel->insertRows(0, i-1);

        QVariant var;
        i = 0;
        for (xml_node<> *member_data = team_members_node->first_node("member"); member_data; member_data = member_data->next_sibling(), i++)
        {
            TeamMember temp(member_data);
            //Load Jobs
            for (xml_node<> *job_node = member_data->first_node("job"); job_node; job_node = job_node->next_sibling())
            {
                temp.loadJob(job_node);
            }
            var.setValue(temp);
            listModel->setData(listModel->index(i), var);
        }
    }
    else
    {
        //TODO handle error
    }
    openMemberView(listModel->index(0));
}

void MainWindow::saveData()
{
    QString filter = QString("Database Files (*.db)");
    saveDatabase(QFileDialog::getSaveFileName(this, QString(), QString(), filter, &filter));
}

void MainWindow::saveDatabase(QString filename)
{
    //Print the contents of the database into the string
    std::string s;
    rapidxml::print(std::back_inserter(s), db);

    //Write the string containing the database to the given file.
    QFile db_file(filename);
    if(db_file.open(QFile::ReadWrite | QFile::Text))
    {
        db_file.resize(0);
        db_file.write(s.c_str());
    }
}

void MainWindow::exportNametags()
{
    //Create string with RTF header
    QString str("{\\rtf\\ansi\\deff0{\\fonttbl{\\f0\\fswiss Arial;}{\\f1\\fmodern IDAutomationHC39M;}}{\\colortbl;\\red0\\green0\\blue0;\\red255\\green0\\blue0;\\red255\\green102\\blue0;\\red0\\green128\\blue0;\\red0\\green0\\blue255;\\red128\\green0\\blue128;}{\\stylesheet{\\s11\\ri144\\li144\\aspalpha\\aspnum\\qc\\f0\\b\\fs96 FirstName;}{\\s12\\ri144\\li144\\aspalpha\\aspnum\\qc\\f0\\b\\fs72 LastName;}{\\s13\\ri144\\li144\\aspalpha\\aspnum\\qc\\f0\\b\\sb120\\fs56 Job;}{\\s14\\ri144\\li144\\aspalpha\\aspnum\\qc\\f1\\fs18 Barcode;}{\\s21\\sbasedon13\\cf5\\ri144\\li144\\aspalpha\\aspnum\\qc\\f0\\b\\sb120\\fs56 Job: Judge;}{\\s22\\sbasedon13\\cf2\\ri144\\li144\\aspalpha\\aspnum\\qc\\f0\\b\\sb120\\fs56 Job: Referee;}{\\s23\\sbasedon13\\cf3\\ri144\\li144\\aspalpha\\aspnum\\qc\\f0\\b\\sb120\\fs56 Job: Robot Inspector;}{\\s24\\sbasedon13\\cf4\\ri144\\li144\\aspalpha\\aspnum\\qc\\f0\\b\\sb120\\fs56 Job: Safety;}{\\s25\\sbasedon13\\cf6\\ri144\\li144\\aspalpha\\aspnum\\qc\\f0\\b\\sb120\\fs56 Job: Staff;}}\\paperw12240\\paperh15840\\margl225\\margr225\\margt720\\margb0\\viewkind1");

    //Populate the table with nametags
    for(QList<TeamMember>::iterator member = listModel->memberList.begin(); member != listModel->memberList.end(); member++)
    {
        //Start row
        QString row("");
        row.append("\\trowd\\irow0\\irowband0\\trgaph15\\trrh-2880\\trleft0\\trkeep\\trftsWidth1\\trpaddl15\\trpaddr15\\trpaddfl3\\trpaddfr3\\clvertalc \\cltxlrtb \\clbrdrt\\brdrtbl\\clbrdrl\\brdrtbl\\clbrdrb\\brdrtbl\\clbrdrr\\brdrtbl \\clshdrawnil \\clftsWidth3\\clwWidth5040\\cellx5040\\clvertalc \\cltxbtlr \\clbrdrt\\brdrtbl\\clbrdrl\\brdrtbl\\clbrdrb\\brdrtbl\\clbrdrr\\brdrtbl \\clshdrawnil \\clftsWidth3\\clwWidth720\\cellx5760\\clvertalc \\cltxlrtb \\clbrdrt\\brdrtbl\\clbrdrl\\brdrtbl\\clbrdrb\\brdrtbl\\clbrdrr\\brdrtbl \\clshdrawnil \\clftsWidth3\\clwWidth270\\cellx6030\\clvertalc \\cltxlrtb \\clbrdrt\\brdrtbl\\clbrdrl\\brdrtbl\\clbrdrb\\brdrtbl\\clbrdrr\\brdrtbl \\clshdrawnil \\clftsWidth3\\clwWidth5040\\cellx11070\\clvertalc \\cltxbtlr \\clbrdrt\\brdrtbl\\clbrdrl\\brdrtbl\\clbrdrb\\brdrtbl\\clbrdrr\\brdrtbl \\clshdrawnil \\clftsWidth3\\clwWidth720\\cellx11790");

        //First Column
        row.append(QString("\\pard\\plain \\intbl \\s11\\ri144\\li144\\aspalpha\\aspnum\\qc\\f0\\b\\fs96 %1\\par\\pard\\plain \\intbl \\s12\\ri144\\li144\\aspalpha\\aspnum\\qc\\f0\\b\\fs72 %2\\par\\pard\\plain \\intbl \\s13\\ri144\\li144\\aspalpha\\aspnum\\qc\\f0\\b\\sb120\\fs56 %3\\cell\\pard\\plain \\intbl \\s14\\ri144\\li144\\aspalpha\\aspnum\\qc\\f1\\fs18 *%4*\\cell\\pard\\plain \\intbl \\cell").arg(member->getFname()).arg(member->getLname()).arg(member->getTitle()).arg(member->getUid(), 8, 10, QChar('0')));

        //Second Column
        member++;
        if(member == listModel->memberList.end()) //If there are an odd number of nametags, create a blank one in the right column and exit
        {
            row.append(QString("\\pard\\plain \\intbl \\s11\\ri144\\li144\\aspalpha\\aspnum\\qc\\f0\\b\\fs96 \\par\\pard\\plain \\intbl \\s12\\ri144\\li144\\aspalpha\\aspnum\\qc\\f0\\b\\fs72 \\par\\pard\\plain \\intbl \\s13\\\\ri144\\li144\\aspalpha\\aspnum\\qc\\f0\\b\\sb120\\fs56 \\cell\\pard\\plain \\intbl \\s14\\ri144\\li144\\aspalpha\\aspnum\\qc\\f1\\fs18 \\cell\\pard\\plain \\intbl \\trowd\\irow0\\irowband0\\trgaph15\\trrh-2880\\trleft0\\trkeep\\trftsWidth1\\trpaddl15\\trpaddr15\\trpaddfl3\\trpaddfr3\\clvertalc \\cltxlrtb \\clbrdrt\\brdrtbl\\clbrdrl\\brdrtbl\\clbrdrb\\brdrtbl\\clbrdrr\\brdrtbl \\clshdrawnil \\clftsWidth3\\clwWidth5040\\cellx5040\\clvertalc \\cltxbtlr \\clbrdrt\\brdrtbl\\clbrdrl\\brdrtbl\\clbrdrb\\brdrtbl\\clbrdrr\\brdrtbl \\clshdrawnil \\clftsWidth3\\clwWidth720\\cellx5760\\clvertalc \\cltxlrtb \\clbrdrt\\brdrtbl\\clbrdrl\\brdrtbl\\clbrdrb\\brdrtbl\\clbrdrr\\brdrtbl \\clshdrawnil \\clftsWidth3\\clwWidth270\\cellx6030\\clvertalc \\cltxlrtb \\clbrdrt\\brdrtbl\\clbrdrl\\brdrtbl\\clbrdrb\\brdrtbl\\clbrdrr\\brdrtbl \\clshdrawnil \\clftsWidth3\\clwWidth5040\\cellx11070\\clvertalc \\cltxbtlr \\clbrdrt\\brdrtbl\\clbrdrl\\brdrtbl\\clbrdrb\\brdrtbl\\clbrdrr\\brdrtbl \\clshdrawnil \\clftsWidth3\\clwWidth720\\cellx11790\\row"));
            str.append(row);
            //End row
            break;
        }
        row.append(QString("\\pard\\plain \\intbl \\s11\\ri144\\li144\\aspalpha\\aspnum\\qc\\f0\\b\\fs96 %1\\par\\pard\\plain \\intbl \\s12\\ri144\\li144\\aspalpha\\aspnum\\qc\\f0\\b\\fs72 %2\\par\\pard\\plain \\intbl \\s135\\ri144\\li144\\aspalpha\\aspnum\\qc\\f0\\b\\sb120\\fs56 %3\\cell\\pard\\plain \\intbl \\s14\\ri144\\li144\\aspalpha\\aspnum\\qc\\f1\\fs18 *%4*\\cell\\pard\\plain \\intbl \\trowd\\irow0\\irowband0\\trgaph15\\trrh-2880\\trleft0\\trkeep\\trftsWidth1\\trpaddl15\\trpaddr15\\trpaddfl3\\trpaddfr3\\clvertalc \\cltxlrtb \\clbrdrt\\brdrtbl\\clbrdrl\\brdrtbl\\clbrdrb\\brdrtbl\\clbrdrr\\brdrtbl \\clshdrawnil \\clftsWidth3\\clwWidth5040\\cellx5040\\clvertalc \\cltxbtlr \\clbrdrt\\brdrtbl\\clbrdrl\\brdrtbl\\clbrdrb\\brdrtbl\\clbrdrr\\brdrtbl \\clshdrawnil \\clftsWidth3\\clwWidth720\\cellx5760\\clvertalc \\cltxlrtb \\clbrdrt\\brdrtbl\\clbrdrl\\brdrtbl\\clbrdrb\\brdrtbl\\clbrdrr\\brdrtbl \\clshdrawnil \\clftsWidth3\\clwWidth270\\cellx6030\\clvertalc \\cltxlrtb \\clbrdrt\\brdrtbl\\clbrdrl\\brdrtbl\\clbrdrb\\brdrtbl\\clbrdrr\\brdrtbl \\clshdrawnil \\clftsWidth3\\clwWidth5040\\cellx11070\\clvertalc \\cltxbtlr \\clbrdrt\\brdrtbl\\clbrdrl\\brdrtbl\\clbrdrb\\brdrtbl\\clbrdrr\\brdrtbl \\clshdrawnil \\clftsWidth3\\clwWidth720\\cellx11790\\row").arg(member->getFname()).arg(member->getLname()).arg(member->getTitle()).arg(member->getUid(), 8, 10, QChar('0')));
        str.append(row);
        //End row
    }
    // End the table

    //Append RTF "footer"
    str.append("}");
    QString exportFileName = QFileDialog::getSaveFileName(this, QString(), QString(), tr("Rich Text Files (*.rtf)"));
    QFile exportFile (exportFileName);
    if(exportFile.open(QFile::ReadWrite | QFile::Text))
    {
        exportFile.resize(0);
        exportFile.write(str.toLocal8Bit());
    }
}

void MainWindow::exportData()
{
    QString csv = tableModel->getCSV();
    QString filter = QString("Comma Separated Value Files (*.csv)");
    QFile db_file(QFileDialog::getSaveFileName(this, QString(), QString(), filter, &filter));
    if(db_file.open(QFile::ReadWrite | QFile::Text))
    {
        db_file.resize(0);
        db_file.write(csv.toStdString().c_str());
    }
}

void MainWindow::importData()
{
    if(!newData()) //Clear out the old data
        return;

    rapidxml::xml_node<> *root_node = db.first_node();
    rapidxml::xml_node<> *team_members_node = root_node->first_node("teammembers");

    QXlsx::Document data(QFileDialog::getOpenFileName(this, QString(), QString(), tr("Excel Spreadsheet (*.xlsx)")));
    QString temp("");
    nextUid = 0;
    int i = 13; // The first row of data is row 13
    while(!(temp = data.read(i,2).toString()).isEmpty()) // Iterate through all the rows with first name values
    {
        QString comments;
        //Build comments section
        if(data.read(i, 1).toString() == QString("Minor"))
            comments.append("***Minor***\n");
        if(data.read(i, 10).toString() != QString("Passed"))
            comments.append("***Not Screened***\n");
        if(data.read(i, 11).toString() != QString("Passed"))
            comments.append("***Not Certified***\n");
        comments.append(data.read(i, 12).toString() + "\n"); //Physical Limitations
        comments.append("Shirt Size: " + data.read(i, 13).toString() + "\n"); //Shirt Size
        comments.append("Team Affiliation: " + data.read(i, 14).toString() + "\n"); //Team
        TeamMember temp(team_members_node, data.read(i, 2).toString(), data.read(i, 3).toString(), ""/*Title will be set later*/, data.read(i, 4).toString(), data.read(i, 5).toString(), comments, nextUid++);
        //Role is column 6, Day is column 7
        do
        {
            if(data.read(i,7).toString().isEmpty()) //If the day column is empty, this is a special title
                temp.setTitle(data.read(i, 6).toString());
            else
            {
                temp.addJob(data.read(i, 6).toString(), data.read(i, 7).toString());
            }
        }
        //If the next row contains more data about the same person, continue parsing their data.
        //If the next row contains data about a different person, finish this loop, insert them
        //into the database and continue to the next iteration.
        while((data.read(++i, 2) == temp.getFname()) && (data.read(i, 3) == temp.getLname()));

        if(temp.getTitle().isEmpty())
            temp.setTitle(data.read(i-1, 6).toString());
        //Append the volunteer to the UI
        listModel->memberList.append(temp);
    }
    listModel->refresh();
    emit updateTables();
}

void MainWindow::beginRegister()
{

}

void MainWindow::finishRegister(TeamMember member)
{
    //Display new member in member list
    listModel->memberList.append(member);
    listModel->refresh();
    tableModel->refresh();
}

void MainWindow::openMemberView(QModelIndex index)
{
    if(!index.isValid())
        return;
    TeamMember member = qvariant_cast<TeamMember>(listModel->data(index, 6));

    ui->name->setText(member.getFname() + " " + member.getLname());
    ui->email->setText(member.getEmail());
    ui->phone->setText(member.getPhone());

    //Set this bool to true to prevent saving the commnets to the database as they are changed
    memberChanged = true;
    ui->comments->clear();
    memberChanged = true;
    ui->comments->appendPlainText(member.getComments());

    ui->title->setText(member.getTitle());
    ui->friday->setText(member.jobs.at(0).getName());
    ui->saturday->setText(member.jobs.at(1).getName());
    ui->sunday->setText(member.jobs.at(2).getName());

    selectedMember = index;
}

void MainWindow::closeMemberView()
{
    ui->name->setText("");
    ui->email->setText("");
    ui->phone->setText("");

    memberChanged = true;
    ui->comments->clear();

    selectedMember = listModel->index(-1);
}

void MainWindow::updateMemberData(QModelIndex index, TeamMember new_member)
{
    if(!index.isValid())
        return;
    TeamMember old_member = qvariant_cast<TeamMember>(listModel->data(index, 6));

    //Check which elements have been changed and update them in the database file.
    if(old_member.getFname() != new_member.getFname())
    {
        old_member.setFname(new_member.getFname());
    }
    if(old_member.getLname() != new_member.getLname())
    {
        old_member.setLname(new_member.getLname());
    }
    if(old_member.getEmail() != new_member.getEmail())
    {
        old_member.setEmail(new_member.getEmail());
    }
    if(old_member.getPhone()!= new_member.getPhone())
    {
        old_member.setPhone(new_member.getPhone());
    }
    if(old_member.getTitle()!= new_member.getTitle())
    {
        old_member.setTitle(new_member.getTitle());
    }
    if(old_member.getComments()!= new_member.getComments())
    {
        old_member.setComments(new_member.getComments());
    }

    QVariant var;
    var.setValue(new_member);
    listModel->setData(index, var);
}

void MainWindow::updateMemberData(QModelIndex topLeft, QModelIndex bottomRight)
{
/*    TeamMember new_member = qvariant_cast<TeamMember>(listModel->data(topLeft, 6));
    //The model only ever edits one cell at a time, meaning topLeft and bottomRight will always be the same.
    switch(topLeft.column())
    {
    case 0: //First Name
    {
        char *dat = db.allocate_string(0, new_member.fname.size());
        strcpy(dat, new_member.fname.toStdString().c_str());
        member_data->first_attribute("fname")->value(dat);
    }
        break;
    case 1: //Last Name
    {
        char *dat = db.allocate_string(0, new_member.lname.size());
        strcpy(dat, new_member.lname.toStdString().c_str());
        member_data->first_attribute("lname")->value(dat);
    }
        break;
    case 2: //Email
    {
        char *dat = db.allocate_string(0, new_member.email.size());
        strcpy(dat, new_member.email.toStdString().c_str());
        member_data->first_attribute("email")->value(dat);
    }
        break;
    case 3: //Phone
    {
        char *dat = db.allocate_string(0, new_member.phone.size());
        strcpy(dat, new_member.phone.toStdString().c_str());
        member_data->first_attribute("phone")->value(dat);
    }
        break;
    case 4: //Title
    {
        char *dat = db.allocate_string(0, new_member.title.size());
        strcpy(dat, new_member.title.toStdString().c_str());
        member_data->first_attribute("title")->value(dat);
    }
        break;
    case 5: //Friday
    case 6: //Saturday
    case 7: //Sunday
    {
        char* targetDay = new char[3];
        switch(topLeft.column())
        {
        case 5: //Friday
            targetDay = "Fri";
            break;
        case 6: //Saturday
            targetDay = "Sat";
            break;
        case 7: //Sunday
            targetDay = "Sun";
            break;
        }

        for(rapidxml::xml_node<> *job_node = member_data->first_node(); job_node; job_node = job_node->next_sibling())
        {
            if(strcmp(job_node->first_attribute("day")->value(), targetDay))
            {
                QString newName = new_member.jobs.find(QString(targetDay)).value().getName();
                char *dat = db.allocate_string(0, newName.size());
                strcpy(dat, newName.toStdString().c_str());
                job_node->first_attribute("name")->value(dat);
                return;
            }
        }
    }
        break;
    default:
        break;
    }
*/
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
        for(QList<TeamMember>::iterator it = listModel->memberList.begin(); it != listModel->memberList.end(); it++, i++)
        {
            if(it->getUid() == searchUid)
                openMemberView(listModel->index(i));
        }
        signIn();
    }
    ui->barcodeInput->setText(QString(""));
    readingBarcode = false;
}

void MainWindow::signIn()
{
    if(selectedMember.isValid())
    {
        TeamMember new_member = qvariant_cast<TeamMember>(listModel->data(selectedMember, 6));
        //TODO new_member.getJob()
        QVariant temp;
        temp.setValue(new_member);
        listModel->setData(selectedMember, temp);
    }
}

void MainWindow::signOut()
{
    if(selectedMember.isValid())
    {
        //Method stub
    }
}

void MainWindow::updateSelectedComments()
{
    if(memberChanged)
    {
        memberChanged = false;
        return;
    }
    TeamMember new_member = qvariant_cast<TeamMember>(listModel->data(selectedMember, 6));
    new_member.setComments(ui->comments->toPlainText());
    QVariant temp;
    temp.setValue(new_member);
    listModel->setData(selectedMember, temp);
}
