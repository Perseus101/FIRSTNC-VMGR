#include "xmlutil.h"

rapidxml::xml_node<> * createAndAppendNode(rapidxml::xml_node<> *node, const char* n)
{
    char *name = node->document()->allocate_string(n);
    rapidxml::xml_node<> *newNode = node->document()->allocate_node(rapidxml::node_element, name);
    node->append_node(newNode);
    return newNode;
}

rapidxml::xml_attribute<> * createAndAppendAttribute(rapidxml::xml_node<> *node, const char* n, QString value)
{
    if(value.size() == 0)
        value.append(' ');
    char *name = node->document()->allocate_string(n);
    char *dat = node->document()->allocate_string(value.toStdString().c_str(), value.size());
    rapidxml::xml_attribute<> *attr = node->document()->allocate_attribute(name, dat);
    node->append_attribute(attr);
    return attr;
}

void replaceValue(rapidxml::xml_node<> *node, QString value)
{
    char *data = node->document()->allocate_string(0, value.size());
    strcpy(data, value.toStdString().c_str());
    node->value(data);
}

void replaceValue(rapidxml::xml_attribute<> *attr, QString value)
{
    char *data = attr->document()->allocate_string(0, value.size());
    strcpy(data, value.toStdString().c_str());
    attr->value(data);
}
