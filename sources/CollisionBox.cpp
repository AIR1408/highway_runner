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

bool CollisionBox::checkCollision(CollisionBox other, glm::vec3 this_position, glm::vec3 other_position)
{
    if (!isDefined || !other.isDefined) return false;

    glm::vec3 size1 = max_point - min_point, size2 = other.max_point - other.min_point;
    glm::vec3 center1 = this_position / size1 * 2.f + 1.f, center2 = other_position / size2 * 2.f + 1.f;
    glm::vec3 in = glm::abs(center2 - center1);

    return glm::max(in.x, glm::max(in.y, in.z)) <= 2.f;
}
