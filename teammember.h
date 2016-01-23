#ifndef TEAMMEMBER_H
#define TEAMMEMBER_H

#include <QObject>
#include <QString>
#include <QStringList>
#include <QDateTime>

#include <QDebug>

class TeamMember
{
public:
    TeamMember();
    TeamMember(QString, QString);
    TeamMember(QString, QString, int);
    ~TeamMember();

    QString getXML();

    QString fname, lname, subteam, email, parentEmail;
    int grade, uid;
    QDateTime in_time,out_time;

};

Q_DECLARE_METATYPE(TeamMember)


#endif // TEAMMEMBER_H
