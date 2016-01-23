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
    TeamMember(QString);
    TeamMember(QString, int);
    ~TeamMember();

    QString getXML();

    QString name, subteam, email, parentEmail;
    int grade, uid;
    QDateTime in_time,out_time;

};

Q_DECLARE_METATYPE(TeamMember)


#endif // TEAMMEMBER_H
