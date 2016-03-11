#ifndef REGISTER_H
#define REGISTER_H

#include <QWidget>

#include "teammember.h"

#include "rapidxml/rapidxml.hpp"

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
    rapidxml::xml_node<> *teamMembersNode;
    int nextUid;
signals:
    void registered(TeamMember);

private slots:

    void registerFinished();
};

#endif // REGISTER_H
