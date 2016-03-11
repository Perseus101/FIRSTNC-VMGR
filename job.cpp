#include "job.h"

Job::Job()
    : node(NULL), name(""), day(""), present(false)
{

}

Job::Job(rapidxml::xml_node<> *parent, QString n, QString d)
{
    node = parent->document()->allocate_node(rapidxml::node_data, "job");

    day = d;
    createAndAppendAttribute(parent, "day", day);

    name = n;
    createAndAppendAttribute(parent, "name", name);

    present = false;
    createAndAppendAttribute(parent, "pres", "0");
}

Job::Job(rapidxml::xml_node<> *nd)
{
    node = nd;
    day = QString(node->first_attribute("day")->value());
    name = QString(node->first_attribute("name")->value());
    present = atoi(node->first_attribute("pres")->value());
}

QString Job::getName() const
{
    return name;
}

void Job::setName(const QString &value)
{
    name = value;
}

QString Job::getDay() const
{
    return day;
}

void Job::setDay(const QString &value)
{
    day = value;
}

bool Job::getPresent() const
{
    return present;
}

void Job::setPresent(bool value)
{
    present = value;
}
