#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QLabel>
#include <QPushButton>
#include <QVariant>

#include "teammemberlistmodel.h"
#include "teammember.h"

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
    QPushButton signIn, signOut;
};

#endif // MAINWINDOW_H
