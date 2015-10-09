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

QString TeamMember::getXML()
{
    return QString("<member fname=%1 lname=%2 eml=%3 peml=%4 team=%5 grade=%6></member>");
}
