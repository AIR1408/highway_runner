#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <ShaderProgram.h>
#include <FileLoader.h>
#include <Container.h>
#include <Components/Camera.h>
#include <Components/PointLight.h>
#include <Components/Transform.h>
#include <Components/Mesh.h>
#include <Components/Texture2D.h>
#include <Components/Material.h>
#include <ScoreCounter.h>

#include <ThirdPersonCamera.h>

#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <thread>

using namespace std;

int winSizeX = 1500, winSizeY = 700;

glm::vec3 lightColor = glm::vec3(1.0f, 1.0f, 1.0f);
glm::vec3 lightPos = glm::vec3(0.0f, 10.0f, -5.0f);

const float platformWidth = 20, platformLength = 100, platformSpeed = 10.0;
const glm::vec3 roadOriginPosition[3] = {
	{platformWidth / 2, 0.0, platformLength / 2 - platformLength + 60},
	{platformWidth / 2, 0.0, platformLength / 2 + 60},
	{platformWidth / 2, 0.0, platformLength / 2 + platformLength + 60}
	};
const float 
	space_between_lane = 5.f,
	height_of_platform = 1.f,
	cars_speed_in_lane[4] = {4.0, 7.0, 17.0, 15.0},
	space_between_cars[4] = {23, 27, 35, 38};
	
const int nPlatforms = 3,
	nCoLanes = 2, nCarsPerCoLane = 6, 
	nVsLanes = 2, nCarsPerVsLane = 4;

float carSpeed = 0, deltaV = 0;
bool game_over = false;

void window_size_callback(GLFWwindow *window, int width, int height)
{
	winSizeX = width, winSizeY = height;
	glViewport(0, 0, winSizeX, winSizeY);
}

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GLFW_TRUE);
}

int main()
{
	try
	{

		// Инициализация библиотеки GLFW
		if (!glfwInit())
			throw "ERROR::GLFW::INITIALIZATION FAILED";

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		// Создание окна
		GLFWwindow *pWindow;
		pWindow = glfwCreateWindow(winSizeX, winSizeY, "runner", NULL, NULL);
		if (!pWindow)
			throw "ERROR::GLFW::WINDOW CREATION";

		// Отключение курсора, обработка событий изменения размера окна и нажатия клавиш
		glfwSetInputMode(pWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		glfwSetWindowSizeCallback(pWindow, window_size_callback);
		glfwSetKeyCallback(pWindow, key_callback);
		glfwMakeContextCurrent(pWindow);

		// Инициализация библиотеки GLAD
		if (!gladLoadGL())
			throw "ERROR::GLAD::INITIALIZATION FAILED";

		// Создание класса для загрузки файлов: текстур, моделей, шейдеров, материалов
		FileLoader files("./resources/");

		{
		// Создание основного шейдера
		auto shader = files.loadShaderProgram();

		// Загрузка текстур, моделей, материалов
		auto main_platform = files.loadObjFile("/cube.obj");
		auto cars_mtl	   = files.loadMtlFile("/Car.mtl", shader);
		auto car 		   = files.loadObjFile("/Car.obj", cars_mtl);
		auto numbers_meshes= files.loadObjFile("/numbers.obj");

		auto numbers_texture = files.loadTexture2D("digital_numbers.jpg", 0);
		auto platform_texture = files.loadTexture2D("asphalt.jpg", 0);
		main_platform.addComponentFront(platform_texture);

		// Создание матриц моделей
		std::shared_ptr<Transform> walls[2];
		std::shared_ptr<Transform> platform_matrix[nPlatforms];
		std::shared_ptr<Transform> cars_matrix_co_direction[nCoLanes][nCarsPerCoLane];
		std::shared_ptr<Transform> cars_matrix_vs_direction[nVsLanes][nCarsPerVsLane];

		// Создание столкновений
		CollisionBox car_box({-1.0, -0.5, -2.0}, {1.0, 0.5, 2.0});

		// Создание источника света
		std::shared_ptr<PointLight> point_light = std::make_shared<PointLight>(shader, lightPos, lightColor);

		// Иницилизация матриц моделей
		walls[0] = std::make_shared<Transform>(shader);
		walls[1] = std::make_shared<Transform>(shader);
		walls[0]->translate({-1.0, 1.0, 0.0});
		walls[1]->translate({21.0, 1.0, 0.0});
		walls[0]->setCollisionBox(car_box);
		walls[1]->setCollisionBox(car_box);
		
		for (int i = 0; i < nCoLanes; i++)
			for (int j = 0; j < nCarsPerCoLane; j++){
				cars_matrix_co_direction[i][j] = std::make_shared<Transform>(shader);
				cars_matrix_co_direction[i][j]->translate({
					space_between_lane * i + 2.5, 
					height_of_platform,
					space_between_cars[i] * j + 20});
				cars_matrix_co_direction[i][j]->setCollisionBox(car_box);
			}
		for (int i = 0; i < nVsLanes; i++)
			for (int j = 0; j < nCarsPerVsLane; j++){
				cars_matrix_vs_direction[i][j] = std::make_shared<Transform>(shader);
				cars_matrix_vs_direction[i][j]->translate({
					space_between_lane * (i + nCoLanes) + 2.5, 
					height_of_platform,
					space_between_cars[i + nCoLanes] * j});
				cars_matrix_vs_direction[i][j]->rotate(glm::radians(180.0f), {0.0, 1.0, 0.0});
				cars_matrix_vs_direction[i][j]->setCollisionBox(car_box);
			}

		for (int i = 0; i < nPlatforms; i++){
			platform_matrix[i] = std::make_shared<Transform>(shader);
			platform_matrix[i]->translate(roadOriginPosition[i]);
			platform_matrix[i]->scale({platformWidth / 2, 1.0, platformLength / 2});
		}
		
		// Иницилизация матрицы проекции
		glm::mat4 projection = glm::perspective(glm::radians(45.f), float(winSizeX) / winSizeY, 0.1f, 130.f);

		// Настройка шейдера
		shader->Use();
		shader->setInt("Texture0", 0);
		shader->setMatrix("projection", projection);
		
		// Камера от третьего лица
		ThirdPersonCamera camera_car(pWindow, shader, glm::vec3(5.0, height_of_platform, 0.0), car);
		camera_car.setCollisionBox(car_box);
		ScoreCounter scoreCounter(numbers_meshes, {-3.0, 7.0, 20.0}, numbers_texture, shader);

		// Вычисление времени
		double currentTime = glfwGetTime(), lastTime = currentTime, deltaTime = 0.0;

		glClearColor(0.0, 191.0 / 255, 1.0, 0.1);
		glEnable(GL_DEPTH_TEST);

		// Вспомогательный игровой цикл
		std::thread thr([&camera_car, &pWindow](){
			while (!glfwWindowShouldClose(pWindow))
				if (game_over){
					camera_car.switchKeyboardBlock();
					break;
				}
		});
		
		// Главный игровой цикл
		while (!glfwWindowShouldClose(pWindow))
		{
			// Очищение буфера цвета и глубины
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			point_light->use();

			scoreCounter.render(camera_car.getPosition() + glm::vec3(-12.0, 6.0, 20.0));

			camera_car.render(deltaTime);

			for (int i = 0; i < nCoLanes; i++){
				for (int j = 0; j < nCarsPerCoLane; j++){
					if (!game_over) {
						cars_matrix_co_direction[i][j]->translate({0.0, 0.0, 
							-deltaTime * cars_speed_in_lane[i] - carSpeed});
						glm::vec3 pos = cars_matrix_co_direction[i][j]->getPosition();
						if (pos.z < -10.0) 
							cars_matrix_co_direction[i][j]->translate({
								space_between_lane * i + 2.5 - 
								pos.x + (rand() % 11 - 5) / 5.0, 0.0, 
								nCarsPerCoLane * space_between_cars[i]});
						game_over = cars_matrix_co_direction[i][j]->checkCollision(camera_car.getTransform());
					}
					cars_matrix_co_direction[i][j]->use();
					car.render();
				}
			}
			for (int i = 0; i < nVsLanes; i++){
				for (int j = 0; j < nCarsPerVsLane; j++){
					if (!game_over) {
						cars_matrix_vs_direction[i][j]->translate({0.0, 0.0, 
							-deltaTime * cars_speed_in_lane[i + nCoLanes] - carSpeed});
						glm::vec3 pos = cars_matrix_vs_direction[i][j]->getPosition();
						if (pos.z < -10.0) 
							cars_matrix_vs_direction[i][j]->translate({
								space_between_lane * (i + nCoLanes) + 2.5 - 
								pos.x + (rand() % 11 - 5) / 5.0, 0.0, 
								nCarsPerVsLane * space_between_cars[i + nCoLanes]});
						game_over = cars_matrix_vs_direction[i][j]->checkCollision(camera_car.getTransform());
					}
					cars_matrix_vs_direction[i][j]->use();
					car.render();
				}
			}
		
			for (int i = 0; i < nPlatforms; i++){
				if (!game_over) {
					deltaV = platformSpeed * deltaTime + carSpeed;
					platform_matrix[i]->translate({0.0, 0.0, -deltaV});
					if (platform_matrix[i]->getPosition().z < -platformLength / 2)
						platform_matrix[i]->translate({0.0, 0.0, platformLength * nPlatforms});
				}
				platform_matrix[i]->use();
				main_platform.render();
			}
			if (!game_over) {
				scoreCounter.setScore(glfwGetTime());
				game_over = walls[0]->checkCollision(camera_car.getTransform()) ||
							walls[1]->checkCollision(camera_car.getTransform());
			}

			// Сброс текстур
			glBindTexture(GL_TEXTURE_2D, 0);

			// Замена буфера
			glfwSwapBuffers(pWindow);

			// Обработка событий
			glfwPollEvents();

			// Вычисление времени между двумя отрисовками
			lastTime = currentTime;
			currentTime = glfwGetTime();
			deltaTime = currentTime - lastTime;

			carSpeed += deltaTime/1000;
		}
		thr.join();

		}
		glfwTerminate();
	}
	catch (const char* message)
	{
		std::cerr << message << std::endl;
		// system("pause");
	}
	catch (string message)
	{
		std::cerr << message << std::endl;
		// system("pause");
	}
	catch (...)
	{
		std::cerr << "ERROR unexpected problem" << std::endl;
		// system("pause");
	}
	return 0;
}
