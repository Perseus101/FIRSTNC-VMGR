#ifndef TEAMMEMBER_H
#define TEAMMEMBER_H

#include <QString>
#include <QDateTime>

class TeamMember
{
public:
    TeamMember();

private:
    QString name, uid;
    QDateTime in_time,out_time;
};

#endif // TEAMMEMBER_H
