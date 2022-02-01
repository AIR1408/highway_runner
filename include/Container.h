#ifndef CONTAINER_H
#define CONTAINER_H

#include <memory>
#include <vector>

#include <Components/Component.h>

class Container
{
public:
    Container();
    Container(const Container& other);
    
    Container& operator=(const Container& other);
    std::shared_ptr<Component>& operator[](int i);
    
    void pull(const Container& other);

    void addComponentFront(std::shared_ptr<Component> component);
    void addComponentBack(std::shared_ptr<Component> component);

    void deleteComponentFront();
    void deleteComponentBack();

    unsigned int getSize();
    std::shared_ptr<Component> getComponent(int index);

    void render();

private:
    std::vector<std::shared_ptr<Component>> components;
};

#endif