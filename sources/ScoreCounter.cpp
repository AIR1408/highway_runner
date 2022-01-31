#include <glad/glad.h>
#include <ScoreCounter.h>
#include <Components/Transform.h>

ScoreCounter::ScoreCounter(Container meshes, glm::vec3 position, std::shared_ptr<Texture2D> numbers,
                           std::shared_ptr<ShaderProgram> shader)
{
    _meshes = meshes,  _numbers = numbers;
    score = 0;
    //score_digits.push_back(0);
    transform = std::make_shared<Transform>(shader);
    transform->translate(position);
    transform->rotate(3.14159265, {0.0, 1.0, 0.0});
}

void ScoreCounter::addScore(int x)
{
    score += x;
    score_digits.clear();
    x = score;
    while (x > 0)
    {
        score_digits.push_back(x % 10);
        x /= 10;
    }
}

void ScoreCounter::render(glm::vec3 position)
{
    _numbers->use();
    transform->translate(position - transform->getPosition());
    for (int i = 0; i < score_digits.size(); i++)
    {
        transform->use();
        _meshes.getComponent(score_digits[i])->use();
        transform->translate({2.0, 0.0, 0.0});
    }
    transform->translate(position - transform->getPosition());
    glBindTexture(GL_TEXTURE_2D, 0);
}

void ScoreCounter::setScore(int x)
{
    score = x;
    score_digits.clear();
    while (x > 0)
    {
        score_digits.push_back(x % 10);
        x /= 10;
    }
}
