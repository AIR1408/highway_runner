#include <Components/CollisionBox/CollisionBox.h>

CollisionBox::CollisionBox()
{
    isDefined = false;
}

CollisionBox::CollisionBox(glm::vec3 point1, glm::vec3 point2)
{
    max_point = glm::max(point1, point2);
    min_point = glm::min(point1, point2);

    isDefined = true;
}
/*
bool CollisionBox::checkCollision(CollisionBox other, glm::vec3 this_position, glm::vec3 other_position)
{
    glm::bvec3 intersection;
    if (!isDefined || !other.isDefined) return false;

    glm::vec3 min1 = this_position + min_point, max1 = this_position + max_point,
              min2 = other_position + other.min_point, max2 = other_position + other.max_point;
    
    intersection.x = checkIntersection(min1.x, max1.x, min2.x, max2.x);
    intersection.y = checkIntersection(min1.y, max1.y, min2.y, max2.y);
    intersection.z = checkIntersection(min1.z, max1.z, min2.z, max2.z);

    return intersection.x && intersection.y && intersection.z;
}*/

bool CollisionBox::checkIntersection(float a, float b, float c, float d)
{
    return a <= c && c <= b || a <= d && d <= b || c <= a && a <= d || c <= b && b <= d;
}

glm::vec3 CollisionBox::checkCollision(CollisionBox b, glm::vec3 a_position, glm::vec3 b_position)
{
    glm::vec3 intersection;
    if (!isDefined || !b.isDefined) return {-1, -1, -1};

    glm::vec3 min1 = a_position + min_point, max1 = a_position + max_point,
              min2 = b_position + b.min_point, max2 = b_position + b.max_point;

    intersection = {glm::min(max1.x, max2.x) - glm::max(min1.x, min2.x),
                    glm::min(max1.y, max2.y) - glm::max(min1.y, min2.y),
                    glm::min(max1.z, max2.z) - glm::max(min1.z, min2.z)};
    return intersection;
}
