#include "teammember.h"

TeamMember::TeamMember()
{

}

TeamMember::TeamMember(QString n)
    : name(n)
{

}

TeamMember::~TeamMember()
{

}

QString TeamMember::getName() const
{
    return name;
}
