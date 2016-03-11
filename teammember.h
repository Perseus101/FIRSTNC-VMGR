#ifndef TEAMMEMBER_H
#define TEAMMEMBER_H

#include <QObject>
#include <QString>
#include <QStringList>
#include <QDateTime>
#include <QVector>

#include "rapidxml/rapidxml.hpp"
#include "rapidxml/rapidxml_utils.hpp"
#include "rapidxml/rapidxml_print.hpp"
#include <string>

#include "job.h"

class TeamMember
{
public:
    TeamMember();
    TeamMember(rapidxml::xml_node<> *);
    TeamMember(rapidxml::xml_node<> *, QString, QString, QString, QString, QString, QString, int);
    ~TeamMember();

    rapidxml::xml_node<> * getXML() const;

    QString getFname() const;
    QString getLname() const;
    QString getTitle() const;
    QString getEmail() const;
    QString getPhone() const;
    QString getComments() const;
    int getUid() const;

    void setLname(const QString &value);
    void setFname(const QString &value);
    void setTitle(const QString &value);
    void setEmail(const QString &value);
    void setPhone(const QString &value);
    void setComments(const QString &value);

    void addJob(const QString &name, const QString &day);
    void loadJob(rapidxml::xml_node<> * jobNode);

    QVector<Job> jobs;
private:

    rapidxml::xml_node<> *node;
    QString fname, lname, title, email, phone, comments;
    int uid;

};

Q_DECLARE_METATYPE(TeamMember)


#endif // TEAMMEMBER_H
