#ifndef SCORE_COUNTER_H
#define SCORE_COUNTER_H

#include <Components/Transform.h>
#include <Components/Texture2D.h>
#include <Container.h>
#include <glm/vec3.hpp>
#include <vector>

class ScoreCounter
{
public:
    ScoreCounter(Container meshes, glm::vec3 position, std::shared_ptr<Texture2D> numbers, 
                std::shared_ptr<ShaderProgram> shader);

    void setScore(int x);
    void addScore(int x);

    void render(glm::vec3 position);
private:

    Container _meshes;
    std::vector<int> score_digits;
    std::shared_ptr<Transform> transform;
    std::shared_ptr<Texture2D> _numbers;

    int score;
};

#endif