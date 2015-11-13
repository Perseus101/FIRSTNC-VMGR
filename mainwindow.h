#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QLabel>
#include <QPushButton>
#include <QVariant>
#include <QFile>
#include <QTextStream>
#include <QFileDialog>
#include <QTimer>
#include <QDebug>
#include <QMessageBox>

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
    bool newData();
    void openData();
    void openData(QString);
    void saveData();
    void exportData();

    void beginRegister();
    void finishRegister(TeamMember);

    void openMemberView(QModelIndex);
    void closeMemberView();

    void startBarcodeRead();
    void endBarcodeRead();

    void signIn();
    void signOut();
private:

    void saveDatabase(QString);

    Ui::MainWindow *ui;
    TeamMemberListModel* model;

    rapidxml::xml_document<> db;
    Register reg;
    bool readingBarcode;
    QModelIndex selectedMember;

    int nextUid;

};

#endif // MAINWINDOW_H
