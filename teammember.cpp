#include "teammember.h"

TeamMember::TeamMember()
    : fname(), lname(), title(), email(), phone(), comments(), uid(), node(), jobs()
{
    jobs.resize(3);
}

TeamMember::TeamMember(rapidxml::xml_node<> * nd)
    : node(nd), jobs()
{
    jobs.resize(3);
    fname = QString(node->first_attribute("fname")->value());
    lname = QString(node->first_attribute("lname")->value());
    email = QString(node->first_attribute("eml")->value());
    phone = QString(node->first_attribute("phone")->value());
    title = QString(node->first_attribute("title")->value());
    comments = QString(node->first_attribute("comments")->value());
    for(rapidxml::xml_node<> *jobNode = node->first_node(); jobNode; jobNode = jobNode->next_sibling())
    {
        int pos = -1;
        if(jobNode->first_attribute("day")->value() == "Fri")
            pos = 0;
        else if(jobNode->first_attribute("day")->value() == "Sat")
            pos = 1;
        else if(jobNode->first_attribute("day")->value() == "Sun")
            pos = 2;
        jobs.insert(pos, Job(jobNode));
    }
}

TeamMember::TeamMember(rapidxml::xml_node<> *parent, QString fn, QString ln, QString ti, QString eml, QString ph, QString com, int p_uid)
    : fname(fn), lname(ln), title(ti), email(eml), phone(ph), comments(com), uid(p_uid)
{
    jobs.resize(3);
    node = createAndAppendNode(parent, "member");
    createAndAppendAttribute(node, "fname", fname);
    createAndAppendAttribute(node, "lname", lname);
    createAndAppendAttribute(node, "eml", email);
    createAndAppendAttribute(node, "phone", phone);
    createAndAppendAttribute(node, "title", title);
    createAndAppendAttribute(node, "comments", comments);
    createAndAppendAttribute(node, "uid", QString::number(uid));
}

TeamMember::~TeamMember()
{

}

rapidxml::xml_node<> * TeamMember::getXML() const
{
    return node;
    //    QString temp = QString("<member fname=\"%1\" lname=\"%2\" eml=\"%3\" phone=\"%4\" title=\"%5\" comments=\"%6\"uid=\"%7\">")
    //            .arg(fname, lname, email, phone, title, comments).arg(uid);
    //    for(QMap<QString, Job>::const_iterator it = jobs.begin(); it != jobs.end(); it++)
    //        temp.append(QString("<job day=\"%1\"name=\"%2\" pres=\"%3\"/>").arg(it.key(), it->name, QString("%1").arg(it->present?1:0)));
    //    temp.append("</member>");
    //    return temp;
}

QString TeamMember::getFname() const
{
    return fname;
}
QString TeamMember::getLname() const
{
    return lname;
}
QString TeamMember::getTitle() const
{
    return title;
}
QString TeamMember::getEmail() const
{
    return email;
}
QString TeamMember::getPhone() const
{
    return phone;
}
QString TeamMember::getComments() const
{
    return comments;
}
int TeamMember::getUid() const
{
    return uid;
}

void TeamMember::setFname(const QString &value)
{
    fname = value;
    replaceValue(node->first_attribute("fname"), fname);
}
void TeamMember::setLname(const QString &value)
{
    lname = value;
    replaceValue(node->first_attribute("lname"), lname);
}
void TeamMember::setTitle(const QString &value)
{
    title = value;
    replaceValue(node->first_attribute("title"), title);
}
void TeamMember::setEmail(const QString &value)
{
    email = value;
    replaceValue(node->first_attribute("eml"), email);
}
void TeamMember::setPhone(const QString &value)
{
    phone = value;
    replaceValue(node->first_attribute("phone"), phone);
}
void TeamMember::setComments(const QString &value)
{
    comments = value;
    replaceValue(node->first_attribute("comments"), comments);
}
void TeamMember::addJob(const QString &name, const QString &day)
{
    if(day == QString("Fri"))
    {
        jobs.replace(0, Job(node, name, day));
    }
    else if(day == QString("Sat"))
    {
        jobs.replace(1, Job(node, name, day));
    }
    else if(day == QString("Sun"))
    {
        jobs.replace(2, Job(node, name, day));
    }
}
void TeamMember::loadJob(rapidxml::xml_node<> *jobNode)
{
    int pos = -1;
    if(jobNode->first_attribute("day")->value() == "Fri")
        pos = 0;
    else if(jobNode->first_attribute("day")->value() == "Sat")
        pos = 1;
    else if(jobNode->first_attribute("day")->value() == "Sun")
        pos = 2;
    jobs.insert(pos, Job(jobNode));
}
