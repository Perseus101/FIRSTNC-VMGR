#include "teammember.h"

TeamMember::TeamMember()
{

}

TeamMember::TeamMember(QString n)
    : name(n)
{

}

TeamMember::TeamMember(QString n, int p_uid)
    : name(n), uid(p_uid)
{

}

TeamMember::~TeamMember()
{

}

QString TeamMember::getXML()
{
    QStringList nameList = name.split(" ");
    return QString("<member fname=\"%1\" lname=\"%2\" eml=\"%3\" peml=\"%4\" team=\"%5\" grade=\"%6\" uid=\"%7\"></member>")
            .arg(nameList.first(), nameList.last(), email, parentEmail, subteam)
            .arg(grade).arg(uid);
}
