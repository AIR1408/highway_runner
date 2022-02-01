#ifndef COLLISION_BOX_H
#define COLLISION_BOX_H

#include <glm/vec3.hpp>
#include <glm/ext/scalar_common.hpp>

class CollisionBox
{
public:
    CollisionBox();
    CollisionBox(glm::vec3 point1, glm::vec3 point2);

    bool checkCollision(CollisionBox other, glm::vec3 this_position, glm::vec3 other_position);
    // glm::vec3 checkCollision(CollisionBox b, glm::vec3 a_position, glm::vec3 b_position);
   
private:    
    bool isDefined;
    glm::vec3 max_point, min_point;    
};

#endif