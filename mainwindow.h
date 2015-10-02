#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStringListModel>
#include <QStringList>

#include <QLabel>
#include <QPushButton>

#include "tablemodel.h"

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
    QStringListModel* model;

    QLabel memberName;
    QPushButton signIn, signOut;
};

#endif // MAINWINDOW_H
