#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QLabel>
#include <QPushButton>
#include <QVariant>
#include <QFile>
#include <QTextStream>
#include <QFileDialog>
#include <QDebug>

#include "teammemberlistmodel.h"
#include "teammember.h"
#include "register.h"
#include "rapidxml/rapidxml.hpp"
#include "rapidxml/rapidxml_utils.hpp"
#include "rapidxml/rapidxml_print.hpp"



namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
     void closeEvent(QCloseEvent *event);

private slots:
    void newData();
    void openData();
    void saveData();
    void exportData();

    void beginRegister();
    void finishRegister(TeamMember);

    void openMemberView(QModelIndex);
private:
    Ui::MainWindow *ui;
    TeamMemberListModel* model;

    QLabel memberName;
    QFile db_file;
    char *db_text;
    rapidxml::xml_document<> db;
    Register reg;

    int nextUid;
};

#endif // MAINWINDOW_H
