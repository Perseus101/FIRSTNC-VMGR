#ifndef XMLUTIL_H
#define XMLUTIL_H

#include <QString>
#include <QDebug>
#include <string>
#include "rapidxml/rapidxml.hpp"
#include "rapidxml/rapidxml_utils.hpp"
#include "rapidxml/rapidxml_print.hpp"

rapidxml::xml_node<> * createAndAppendNode(rapidxml::xml_node<> *, const char*);
rapidxml::xml_attribute<> * createAndAppendAttribute(rapidxml::xml_node<> *, const char*, QString);
void replaceValue(rapidxml::xml_node<> *, QString);
void replaceValue(rapidxml::xml_attribute<> *, QString);

#endif // XMLUTIL_H
