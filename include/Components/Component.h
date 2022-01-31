#ifndef COMPONENT_H
#define COMPONENT_H

class Component
{
public:
    virtual void use() = 0;
    virtual ~Component() {};
};

#endif