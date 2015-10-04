#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QLabel>
#include <QPushButton>
#include <QVariant>
#include <QFile>
#include <QTextStream>
#include <QDebug>

#include <vector>

#include "teammemberlistmodel.h"
#include "teammember.h"
#include "rapidxml/rapidxml.hpp"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void newData();
    void openData();
    void saveData();

    void openMemberView(QModelIndex);
private:
    Ui::MainWindow *ui;
    TeamMemberListModel* model;

    QLabel memberName;
    QFile db_file;
    char *db_text;
    rapidxml::xml_document<> db;
};

#endif // MAINWINDOW_H
