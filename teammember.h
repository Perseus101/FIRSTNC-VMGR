#ifndef TEAMMEMBER_H
#define TEAMMEMBER_H

#include <QObject>
#include <QString>
#include <QStringList>
#include <QDateTime>
#include <QMap>

#include <QDebug>

struct Job
{
    QString Name;
    bool present;
};

class TeamMember
{
public:
    TeamMember();
    TeamMember(QString, QString);
    TeamMember(QString, QString, int);
    ~TeamMember();

    QString getXML() const;

    QString fname, lname, job, email, phone, comments;
    int uid;

};

Q_DECLARE_METATYPE(TeamMember)


#endif // TEAMMEMBER_H
