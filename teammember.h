#ifndef TEAMMEMBER_H
#define TEAMMEMBER_H

#include <QObject>
#include <QString>
#include <QStringList>
#include <QDateTime>

class TeamMember
{
public:
    TeamMember();
    TeamMember(QString);
    ~TeamMember();

    QString getXML();
public:
    QString name, uid, subteam, email, parentEmail;
    int grade;
private:
    QDateTime in_time,out_time;
};

Q_DECLARE_METATYPE(TeamMember)


#endif // TEAMMEMBER_H
