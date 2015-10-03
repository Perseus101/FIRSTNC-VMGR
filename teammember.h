#ifndef TEAMMEMBER_H
#define TEAMMEMBER_H

#include <QObject>
#include <QString>
#include <QDateTime>

class TeamMember
{
public:
    TeamMember();
    TeamMember(QString);
    ~TeamMember();

   QString getName() const;
private:
    QString name, uid;
    QDateTime in_time,out_time;
};

Q_DECLARE_METATYPE(TeamMember)


#endif // TEAMMEMBER_H
