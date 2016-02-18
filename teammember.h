#ifndef TEAMMEMBER_H
#define TEAMMEMBER_H

#include <QObject>
#include <QString>
#include <QStringList>
#include <QDateTime>
#include <QMap>

#include <QDebug>

class Job
{
public:
    Job(QString n, bool p)
    {
        name = n;
        present = p;
    }

    QString name;
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

    QMap<QString, Job> jobs;

    QString fname, lname, title, email, phone, comments;
    int uid;

};

Q_DECLARE_METATYPE(TeamMember)


#endif // TEAMMEMBER_H
