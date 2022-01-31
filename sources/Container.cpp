#include <Container.h>

Container::Container()
{

}

Container::Container(const Container& other)
{
    for (auto it : other.components){
        components.push_back(it);
    }
}

Container& Container::operator=(const Container& other)
{
    components.clear();
    for (auto it : other.components){
        components.push_back(it);
    }
    return *this;
}

void Container::pull(const Container& other)
{
    for (auto it : other.components){
        components.push_back(it);
    }
}

void Container::addComponentFront(std::shared_ptr<Component> component)
{
    components.insert(components.begin(), component);
}

void Container::addComponentBack(std::shared_ptr<Component> component)
{
    components.push_back(component);
}

unsigned int Container::getSize()
{
    return components.size();
}

void Container::render()
{
    for (auto& component : components)
        component->use();
}

std::shared_ptr<Component> Container::getComponent(int index)
{
    if (index >= components.size())
        return nullptr;
    else return components[index];
}

void Container::deleteComponentFront()
{
    components.erase(components.begin());
}

void Container::deleteComponentBack()
{
    components.pop_back();
}

std::shared_ptr<Component>& Container::operator[](int i)
{
    return components[i];
}
