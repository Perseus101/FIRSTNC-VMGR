#include "teammember.h"

TeamMember::TeamMember()
{

}

TeamMember::TeamMember(QString fn, QString ln)
    : fname(fn), lname(ln)
{

}
TeamMember::TeamMember(QString fn, QString ln, int p_uid)
    : fname(fn), lname(ln), uid(p_uid)
{

}

TeamMember::~TeamMember()
{

}

QString TeamMember::getXML()
{
    return QString("<member fname=\"%1\" lname=\"%2\" eml=\"%3\" phone=\"%4\" job=\"%5\" age=\"%6\" uid=\"%7\"></member>")
            .arg(fname, lname, email, phone, job)
            .arg(age).arg(uid);
}
