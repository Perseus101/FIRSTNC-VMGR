#ifndef JOB_H
#define JOB_H

#include <QString>

#include "xmlutil.h"
#include "rapidxml/rapidxml.hpp"

class Job
{
public:

    Job();
    /**
     * @brief For creating a new Job
     * @param parent Parent node to create the new Job in
     * @param n Job's name
     * @param d Job's day
     */
    Job(rapidxml::xml_node<> *parent, QString n, QString d);
    /**
     * @brief For loading a Job from a database
     * @param nd This job's node from the database
     */
    Job(rapidxml::xml_node<> * nd);

    QString getName() const;
    QString getDay() const;
    bool getPresent() const;

    void setName(const QString &value);
    void setDay(const QString &value);
    void setPresent(bool value);

private:
    rapidxml::xml_node<> *node;
    QString name, day;
    bool present;
};

#endif // JOB_H
