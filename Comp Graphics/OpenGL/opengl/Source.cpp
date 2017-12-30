#include <iostream>
#include <vector>
#include <string>
// GLEW нужно подключать до GLFW.
// GLEW
#define GLEW_STATIC
#include <GL\glew.h>
// GLFW
#include <GLFW\glfw3.h>
// SOIL
#include <soil-src\SOIL.h>
// GLM
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

#include "Shader.h"
#include "Camera.h"
#include "Cube.h"

const GLuint WIDTH = 600, HEIGHT = 600;
const GLuint DEPTH_MAP_RESOLUTION = 1024;

struct Light {
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	glm::vec3 color;
	glm::vec3 position;
};

// Function prototypes
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void Do_Movement();
GLuint load_cubemap(std::vector<std::string> faces);
GLuint load_texture2D(const std::string path);

void setCubeTextureVertices(GLuint VAO, GLuint VBO, GLuint index, const GLfloat * texturesVertices);

void renderScene(
	const Shader & shader,
	const glm::mat4 & projectionMatrix, 
	const glm::mat4 & viewMatrix, 
	const glm::mat4 & lightSpaceMatrix
);
void renderShadow(
	const Shader & shader,
	const glm::mat4 & projectionMatrix,
	const glm::mat4 & viewMatrix
);
void renderCubes(
	const Shader & shader,
	const glm::mat4 & projectionMatrix,
	const glm::mat4 & viewMatrix,
	const glm::mat4 & lightSpaceMatrix
);
void renderFloor(
	const Shader & shader,
	const glm::mat4 & projectionMatrix,
	const glm::mat4 & viewMatrix,
	const glm::mat4 & lightSpaceMatrix
);
void renderLamp(
	const Shader & shader,
	const glm::mat4 & projectionMatrix,
	const glm::mat4 & viewMatrix
);
void set_uniforms(
	const Shader & shader,
	Material m,
	Light l,
	const glm::mat4 & projectionMatrix,
	const glm::mat4 & viewMatrix,
	const glm::mat4 & modelMatrix,
	const glm::mat4 & lightSpaceMatrix);
void gen_depth_map(GLuint * depthMap, GLuint * depthMapFBO);

GLuint texCoordBuffer;
GLuint cubeVAO;
GLuint planeVAO;
GLuint lightVAO;
Light light;
std::vector<Cube> cubes;
GLuint depthMap;
GLuint depthMapFBO;
GLuint lightCubeTexture;
GLuint cubeMapTexture;

// Camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
bool keys[1024];
GLfloat lastX = 400, lastY = 300;
bool firstMouse = true;

GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

int main() {
    //Инициализация GLFW
    glfwInit();
    //Настройка GLFW
    //Задается минимальная требуемая версия OpenGL. 
    //Мажорная 
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    //Минорная
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    //Установка профайла для которого создается контекст
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //Выключение возможности изменения размера окна
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    // Create a GLFWwindow object that we can use for GLFW's functions
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "LR 1", nullptr, nullptr);
    if (window == nullptr) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // Set the required callback functions
    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // Options
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        std::cout << "Failed to initialize GLEW" << std::endl;
        return -1;
    }
    
    // Define Viewport Dimension
    glViewport(0, 0, WIDTH, HEIGHT);

    // OpenGL Options
    glEnable(GL_DEPTH_TEST);

	// Загрузка шейдеров
    Shader myShader("shader.vert", "shader.frag");
	Shader depthShader("depth_shader.vert", "depth_shader.frag");
    Shader lampShader("lamp_shader.vert", "lamp_shader.frag");

	GLfloat planeVertices[] = {
		// positions            // normals      
		25.0f, -0.5f, 25.0f,   0.0f, 1.0f, 0.0f,
		-25.0f, -0.5f, 25.0f,  0.0f, 1.0f, 0.0f,
		-25.0f, -0.5f, -25.0f, 0.0f, 1.0f, 0.0f,
												
		25.0f, -0.5f, 25.0f,   0.0f, 1.0f, 0.0f,
		-25.0f, -0.5f, -25.0f, 0.0f, 1.0f, 0.0f,
		25.0f, -0.5f, -25.0f,  0.0f, 1.0f, 0.0f,
	};
	glm::vec3 planeColor(0.28f, 0.29f, 0.31f);

	// GLuint planeVAO;
	GLuint planeVerticesBuffer;
	glGenVertexArrays(1, &planeVAO);
	glGenBuffers(1, &planeVerticesBuffer);
	
	glBindVertexArray(planeVAO);
		// plane vertices buffer
		glBindBuffer(GL_ARRAY_BUFFER, planeVerticesBuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), planeVertices, GL_STATIC_DRAW);
		// positions
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid *)0);
		glEnableVertexAttribArray(0);
		// normals
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid *)(3 * sizeof(GLfloat)));
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

    GLfloat cubeVertices[] = {
        //vertices            //normals
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
        0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
        0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
                                                 
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f, 
        0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f, 
        0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f, 
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f, 
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f, 
                                                 
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
                                                 
        0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f, 
        0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f, 
        0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f, 
        0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f, 
        0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f, 
        0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f, 
                                                 
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
        0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, 
        0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, 
        0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, 
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
                                                 
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f, 
        0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, 
        0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, 
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
    };
	GLfloat textureVertices[] = {
		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,
		1.0f, 1.0f,
		0.0f, 1.0f,
		0.0f, 0.0f,

		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,
		1.0f, 1.0f,
		0.0f, 1.0f,
		0.0f, 0.0f,

		1.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 1.0f,
		0.0f, 1.0f,
		0.0f, 0.0f,
		1.0f, 0.0f,

		1.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 1.0f,
		0.0f, 1.0f,
		0.0f, 0.0f,
		1.0f, 0.0f,

		0.0f, 1.0f,
		1.0f, 1.0f,
		1.0f, 0.0f,
		1.0f, 0.0f,
		0.0f, 0.0f,
		0.0f, 1.0f,

		0.0f, 1.0f,
		1.0f, 1.0f,
		1.0f, 0.0f,
		1.0f, 0.0f,
		0.0f, 0.0f,
		0.0f, 1.0f
	};
    // объект вершинного буфера - точки фигуры
    GLuint cubeVerticesBuffer;
    // объект вершинного буфера - координаты текстуры

    glGenBuffers(1, &cubeVerticesBuffer);
    glGenBuffers(1, &texCoordBuffer);
    glGenVertexArrays(1, &cubeVAO);
    glGenVertexArrays(1, &lightVAO);

    // привязка VAO
    glBindVertexArray(cubeVAO);
        // копирование массива с вершинами в буфер
        glBindBuffer(GL_ARRAY_BUFFER, cubeVerticesBuffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);

        // установка указателя на вершинный атрибут
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(0);
        
        // массив нормалей
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

    glBindVertexArray(lightVAO);
        glBindBuffer(GL_ARRAY_BUFFER, cubeVerticesBuffer);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(0);
		setCubeTextureVertices(lightVAO, texCoordBuffer, 1, textureVertices);

	glBindVertexArray(0);

	// источник света
	light = Light {
		glm::vec3(1.0f),  // ambient
		glm::vec3(1.0f),  // diffuse
		glm::vec3(1.0f),  // specular
		glm::vec3(1.0f),  // color
		glm::vec3(2.0f, 2.0f, 4.0f)  // position
	};

	lightCubeTexture = load_texture2D("resources/lava.png");

	// Кубы
	// fake cube not showed (see renderLamp)
	cubes.push_back(Cube(1.0f, -0.25f, 0.5f, 0.0f, 0.0f, 0.0f));
	cubes.back().bindTexture(load_texture2D("resources/lava.png"), GL_TEXTURE_2D);
	cubes.back().setTextureSize(1.0f);
	cubes.back().setMaterial(
		1.0f, 1.0f, 1.0f,			// ambient
		0.6f, 0.6f, 0.6f,			// diffuse
		0.5f, 0.5f, 0.5f,			// specular
		32.0f);						// shininess

	cubes.push_back(Cube(0.3f, -0.25f, 2.0f, 0.0f, 15.0f, 0.0f));
	cubes.back().bindTexture(load_texture2D("resources/brick.png"), GL_TEXTURE_2D);
	cubes.back().setTextureSize(8.0f);
	cubes.back().setMaterial(
		0.6f, 0.6f, 0.6f,			// ambient
		0.6f, 0.6f, 0.6f,			// diffuse
		0.5f, 0.5f, 0.5f,			// specular
		32.0f);						// shininess

	cubes.push_back(Cube(0.15f, -0.25f, -0.5f, 0.0f, -15.0f, 0.0f));
	cubes.back().bindTexture(load_texture2D("resources/clay.png"), GL_TEXTURE_2D);
	cubes.back().setTextureSize(4.0f);
	cubes.back().setMaterial(
		0.6f, 0.6f, 0.6f,			// ambient
		0.6f, 0.6f, 0.6f,			// diffuse
		0.5f, 0.5f, 0.5f,			// specular
		32.0f);						// shininess

	cubes.push_back(Cube(-1.0f, -0.25f, -0.5f, 0.0f, 30.0f, 0.0f));
	cubes.back().bindTexture(load_texture2D("resources/cobblestone.png"), GL_TEXTURE_2D);
	cubes.back().setTextureSize(4.0f);
	cubes.back().setMaterial(
		0.6f, 0.6f, 0.6f,			// ambient
		0.6f, 0.6f, 0.6f,			// diffuse
		0.5f, 0.5f, 0.5f,			// specular
		32.0f);						// shininess

	cubes.push_back(Cube(1.5f, -0.25f, -1.0f, 0.0f, -30.0f, 0.0f));
	cubes.back().bindTexture(load_texture2D("resources/wood_spruce.png"), GL_TEXTURE_2D);
	cubes.back().setTextureSize(1.0f);
	cubes.back().setMaterial(
		0.6f, 0.6f, 0.6f,			// ambient
		0.6f, 0.6f, 0.6f,			// diffuse
		0.5f, 0.5f, 0.5f,			// specular
		32.0f);						// shininess

    std::vector<std::string> faces{
        "resources/cubemap/moondust_rt.png",
        "resources/cubemap/moondust_lf.png",
        "resources/cubemap/moondust_up.png",
        "resources/cubemap/moondust_dn.png",
        "resources/cubemap/moondust_bk.png",
        "resources/cubemap/moondust_ft.png"
    };
	// Кубическая карта
    cubeMapTexture = load_cubemap(faces);

	// Depth Map
	gen_depth_map(&depthMap, &depthMapFBO);

    // основной цикл
	while (!glfwWindowShouldClose(window)) {
		// Время вывода одного кадра
		GLfloat currentFrame = (GLfloat)glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// Проверяем события и вызываем функции обратного вызова.
		glfwPollEvents();
		Do_Movement();

		// Определение матриц для проекции
		glm::mat4 viewMatrix;
		glm::mat4 projectionMatrix;
		viewMatrix = camera.GetViewMatrix();
		projectionMatrix = glm::perspective(glm::radians(camera.Zoom), (GLfloat)WIDTH / (GLfloat)HEIGHT, 0.1f, 100.0f);

		glm::mat4 lightProjectionMatrix = glm::perspective(glm::radians(45.0f), 1.0f, 2.0f, 100.0f);
		glm::mat4 lightViewMatrix = glm::lookAt(light.position,
			glm::vec3(0.0f, 0.0f, 0.0f),
			glm::vec3(0.0f, 1.0f, 0.0f));
		glm::mat4 lightSpaceMatrix = lightProjectionMatrix * lightViewMatrix * glm::inverse(viewMatrix);

		renderShadow(depthShader, lightProjectionMatrix, lightViewMatrix);
		renderScene(myShader, projectionMatrix, viewMatrix, lightSpaceMatrix);
		renderLamp(lampShader, projectionMatrix, viewMatrix);

		glBindVertexArray(0);
        glUseProgram(0);

        // Меняем буферы местами
        glfwSwapBuffers(window);
    }

    glfwTerminate();
    return 0;
}

// Moves/alters the camera positions based on user input
void Do_Movement()
{
    // Camera controls
    if (keys[GLFW_KEY_W])
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (keys[GLFW_KEY_S])
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (keys[GLFW_KEY_A])
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (keys[GLFW_KEY_D])
        camera.ProcessKeyboard(RIGHT, deltaTime);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) {
    // Когда пользователь нажимает ESC, мы устанавливаем свойство WindowShouldClose в true, 
    // и приложение после этого закроется
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
    if (key >= 0 && key < 1024) {
        if (action == GLFW_PRESS) {
            keys[key] = true;
        }
        else if (action == GLFW_RELEASE) {
            keys[key] = false;
        }
    }
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = (GLfloat)xpos;
        lastY = (GLfloat)ypos;
        firstMouse = false;
    }

    GLfloat xoffset = (GLfloat)(xpos - lastX);
    GLfloat yoffset = (GLfloat)(lastY - ypos);  // Reversed since y-coordinates go from bottom to left

    lastX = (GLfloat)xpos;
    lastY = (GLfloat)ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll((GLfloat)yoffset);
}

GLuint load_cubemap(std::vector<std::string> faces) {
    GLuint textureId;
    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureId);

    int width, height;
    for (GLuint i = 0; i < faces.size(); i++) {
        unsigned char *image = SOIL_load_image(faces[i].c_str(), &width, &height, 0, SOIL_LOAD_RGB);
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
        SOIL_free_image_data(image);
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    // glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
    return textureId;
}

GLuint load_texture2D(const std::string path) {
	GLuint textureId;
	glGenTextures(1, &textureId);
	// загрузка текстуры ящика
	glBindTexture(GL_TEXTURE_2D, textureId);
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		// ширина и высота загружаемой текстуры
		int texWidth, texHeight;
		unsigned char *image = SOIL_load_image(path.c_str(), &texWidth, &texHeight, 0, SOIL_LOAD_RGB);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texWidth, texHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		glGenerateMipmap(GL_TEXTURE_2D);
		SOIL_free_image_data(image);
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	return textureId;
}

void setCubeTextureVertices(GLuint VAO, GLuint VBO, GLuint index, const GLfloat * texturesVertices)
{
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER, 72 * sizeof(GLfloat), texturesVertices, GL_DYNAMIC_DRAW);
	
	glVertexAttribPointer(index, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(index);
}

void renderScene(
	const Shader & shader,
	const glm::mat4 & projectionMatrix, 
	const glm::mat4 & viewMatrix, 
	const glm::mat4 & lightSpaceMatrix
)
{
	glViewport(0, 0, WIDTH, HEIGHT);
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	shader.use();

	glActiveTexture(GL_TEXTURE0 + 40);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	shader.setInt("shadowMap", 40);

	/* Источники света */
	glm::vec3 viewSpaceLightDir = viewMatrix * glm::vec4(-glm::normalize(light.position), 0.0f);
	shader.setVec3("viewSpaceLightDir", viewSpaceLightDir);
	shader.setFloat("spotInnerAngle", glm::cos(glm::radians(18.0f)));
	shader.setFloat("spotOuterAngle", glm::cos(glm::radians(22.0f)));

	glm::vec3 pos(1.0f, 0.0f, 0.5f);
	glm::vec3 baseColor(1.0f, 0.5f, 0.0f);
	glm::vec3 advColor(1.0f, 0.8f, 0.0f);
	glm::vec3 color1(1.0f, 0.65f, 0.0f);

	glm::vec3 color2 = baseColor;
	color2.x = baseColor.x + (GLfloat)glm::sin(3 * glfwGetTime()) * (advColor.x - baseColor.x);
	color2.y = baseColor.y + (GLfloat)glm::sin(3 * glfwGetTime()) * (advColor.y - baseColor.y);
	color2.z = baseColor.z + (GLfloat)glm::sin(3 * glfwGetTime()) * (advColor.z - baseColor.z);

	shader.setVec3("aLight_position", pos);
	shader.setVec3("aLight_color_1", color2);
	shader.setVec3("aLight_color_2", color1);


	renderCubes(shader, projectionMatrix, viewMatrix, lightSpaceMatrix);
	renderFloor(shader, projectionMatrix, viewMatrix, lightSpaceMatrix);
}

void renderShadow(
	const Shader & shader,
	const glm::mat4 & projectionMatrix, 
	const glm::mat4 & viewMatrix
)
{
	shader.use();

	glViewport(0, 0, DEPTH_MAP_RESOLUTION, DEPTH_MAP_RESOLUTION);
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_POLYGON_OFFSET_FILL);
	glPolygonOffset(2.5, 10);
	
	glm::mat4 fakeLightSpaceMatrix;

	renderCubes(shader, projectionMatrix, viewMatrix, fakeLightSpaceMatrix);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glDisable(GL_POLYGON_OFFSET_FILL);
}

void renderCubes(
	const Shader & shader, 
	const glm::mat4 & projectionMatrix, 
	const glm::mat4 & viewMatrix, 
	const glm::mat4 & lightSpaceMatrix
)
{
	GLuint texturesCount = 0;
	for (Cube &cube : cubes) {
		// Загрузка текстур в шейдер
		setCubeTextureVertices(cubeVAO, texCoordBuffer, 2, cube.getTextureVertices());

		glActiveTexture(GL_TEXTURE0 + texturesCount);	
		glBindTexture(GL_TEXTURE_2D, cube.getTexture());
		shader.setInt("textureBox", texturesCount);
		shader.setInt("texture_type", 1);
		texturesCount++;

		// Загрузка матриц
		glm::mat4 modelMatrix;
		modelMatrix = glm::translate(modelMatrix, cube.getPosition());
		modelMatrix = glm::rotate(modelMatrix, cube.getAngle().y, glm::vec3(0.0f, 1.0f, 0.0f));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(0.5f));

		set_uniforms(shader, cube.getMaterial(), light, projectionMatrix, viewMatrix,
			modelMatrix, lightSpaceMatrix);
		// render cube
		glBindVertexArray(cubeVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
	}

	// Зеркальный куб
	glActiveTexture(GL_TEXTURE0 + texturesCount);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMapTexture);
	shader.setInt("environmentMap", texturesCount);
	shader.setInt("texture_type", 2);
	texturesCount++;

	// Загрузка матриц
	glm::mat4 modelMatrix;
	modelMatrix = glm::translate(modelMatrix, glm::vec3(-0.5f, -0.25f, 0.3f));
	modelMatrix = glm::rotate(modelMatrix, 12.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	modelMatrix = glm::scale(modelMatrix, glm::vec3(0.5f));

	set_uniforms(shader, cubes[0].getMaterial(), light, projectionMatrix, viewMatrix,
		modelMatrix, lightSpaceMatrix);
	shader.setMat4("inverseViewNormalMatrix", glm::transpose(viewMatrix));

	// render cube
	glBindVertexArray(cubeVAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);
}

void renderFloor(
	const Shader & shader, 
	const glm::mat4 & projectionMatrix, 
	const glm::mat4 & viewMatrix, 
	const glm::mat4 & lightSpaceMatrix
)
{
	glm::vec3 planeColor(0.28f, 0.29f, 0.31f);
	Material m{
		glm::vec3(0.8f, 0.8f, 0.8f),
		glm::vec3(0.8f, 0.8f, 0.8f),
		glm::vec3(0.5f, 0.5f, 0.5f),
		128.0f
	};

	shader.setVec3("color", planeColor);
	shader.setInt("texture_type", 0);
	glm::mat4 modelMatrix;

	set_uniforms(shader, m, light, projectionMatrix, viewMatrix, modelMatrix, lightSpaceMatrix);
	// render
	glBindVertexArray(planeVAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
}

void renderLamp(const Shader & shader, const glm::mat4 & projectionMatrix, const glm::mat4 & viewMatrix)
{
	shader.use();
	glm::mat4 modelMatrix;
	modelMatrix = glm::translate(modelMatrix, light.position);
	modelMatrix = glm::scale(modelMatrix, glm::vec3(0.2f));

	shader.setMat4("modelViewProjection", projectionMatrix * viewMatrix * modelMatrix);
	shader.setVec3("lampColor", light.color);
	shader.setInt("hasTexture", 0);
	glBindVertexArray(lightVAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	
	glm::vec3 pos(1.0f, -0.25f, 0.5f);
	glm::vec3 color(1.0f, 1.0f, 0.0f);

	modelMatrix = glm::mat4();
	modelMatrix = glm::translate(modelMatrix, pos);
	modelMatrix = glm::scale(modelMatrix, glm::vec3(0.5001f));
	shader.setMat4("modelViewProjection", projectionMatrix * viewMatrix * modelMatrix);
	shader.setVec3("lampColor", color);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, lightCubeTexture);
	shader.setInt("textureBox", 0);
	shader.setInt("hasTexture", 1);

	glBindVertexArray(lightVAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);
}

void set_uniforms(
	const Shader & shader,
	Material m, 
	Light l,
	const glm::mat4 & projectionMatrix,
	const glm::mat4 & viewMatrix, 
	const glm::mat4 & modelMatrix,
	const glm::mat4 & lightSpaceMatrix
)
{
	shader.setMat4("modelViewProjection", projectionMatrix * viewMatrix * modelMatrix);
	shader.setMat4("modelView", viewMatrix * modelMatrix);
	shader.setMat4("model", modelMatrix);
	shader.setMat4("lightSpace", lightSpaceMatrix);

	shader.setVec3("gLight.position", viewMatrix * glm::vec4(l.position, 1.0f));
	shader.setVec3("gLight.ambient", l.ambient);
	shader.setVec3("gLight.diffuse", l.diffuse);
	shader.setVec3("gLight.specular", l.specular);
	
	shader.setVec3("material.ambient", m.ambient);
	shader.setVec3("material.diffuse", m.diffuse);
	shader.setVec3("material.specular", m.specular);
	shader.setFloat("material.shininess", m.shininess);
}

void gen_depth_map(GLuint * depthMap, GLuint * depthMapFBO)
{
	glGenFramebuffers(1, depthMapFBO);
	glGenTextures(1, depthMap);

	glBindTexture(GL_TEXTURE_2D, *depthMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
		DEPTH_MAP_RESOLUTION, DEPTH_MAP_RESOLUTION, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glm::vec4 zeros(0.0f);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, &zeros.x);

	glBindFramebuffer(GL_FRAMEBUFFER, *depthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, *depthMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
