#ifndef REGISTER_H
#define REGISTER_H

#include <QWidget>

#include "teammember.h"

namespace Ui {
class Register;
}

class Register : public QWidget
{
    Q_OBJECT

public:
    Register(QWidget *parent = 0);
    ~Register();

    Ui::Register *ui;
signals:
    void registered(TeamMember);

private slots:

    void registerFinished();
};

#endif // REGISTER_H
