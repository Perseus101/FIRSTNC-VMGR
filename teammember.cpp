#include "teammember.h"

TeamMember::TeamMember()
{
    Job *temp = new Job("", false);
    jobs.insert("Fri", *temp);
    jobs.insert("Sat", *temp);
    jobs.insert("Sun", *temp);

}

TeamMember::TeamMember(QString fn, QString ln)
    : fname(fn), lname(ln)
{
    Job *temp = new Job("", false);
    jobs.insert("Fri", *temp);
    jobs.insert("Sat", *temp);
    jobs.insert("Sun", *temp);
}
TeamMember::TeamMember(QString fn, QString ln, int p_uid)
    : fname(fn), lname(ln), uid(p_uid)
{
    Job *temp = new Job("", false);
    jobs.insert("Fri", *temp);
    jobs.insert("Sat", *temp);
    jobs.insert("Sun", *temp);
}

TeamMember::~TeamMember()
{

}

QString TeamMember::getXML() const
{
    QString temp = QString("<member fname=\"%1\" lname=\"%2\" eml=\"%3\" phone=\"%4\" title=\"%5\" comments=\"%6\"uid=\"%7\">")
            .arg(fname, lname, email, phone, title, comments).arg(uid);
    for(QMap<QString, Job>::const_iterator it = jobs.begin(); it != jobs.end(); it++)
        temp.append(QString("<job day=\"%1\"name=\"%2\" pres=\"%3\"/>").arg(it.key(), it->name, QString("%1").arg(it->present?1:0)));
    temp.append("</member>");
    return temp;
}
