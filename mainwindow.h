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
#include <QMessageBox>
#include <QDebug>

#include "teammemberlistmodel.h"
#include "teammembertablemodel.h"
#include "teammembertabledelegate.h"
#include "teammember.h"
#include "register.h"
#include "rapidxml/rapidxml.hpp"
#include "rapidxml/rapidxml_utils.hpp"
#include "rapidxml/rapidxml_print.hpp"

#include "xlsxdocument.h"

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
    void importData();

    void beginRegister();
    void finishRegister(TeamMember);

    void openMemberView(QModelIndex);
    void closeMemberView();

    void updateMemberData(QModelIndex, TeamMember);

    void startBarcodeRead();
    void endBarcodeRead();

    void signIn();
    void signOut();

    void updateSelectedComments();

private:

    void saveDatabase(QString);

    Ui::MainWindow *ui;
    TeamMemberListModel* listModel;
    TeamMemberTableModel* tableModel;

    rapidxml::xml_document<> db;
    Register reg;
    bool readingBarcode, memberChanged;
    QModelIndex selectedMember;

    int nextUid;

};

#endif // MAINWINDOW_H
