#include <iostream>
#include <fstream>
#include <string>

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

GLfloat vertices[] = {
	-0.5f, -0.5f, 0.0f,
	0.5f, -0.5f, 0.0f,
	0.0f, 0.5f, 0.0f
};

GLchar *read_file(const std::string filename) {
	std::string result;
	std::string line;
	std::ifstream file(filename);
	if(file.is_open()) {
		while(std::getline(file, line)) {
			result += line + '\n';
		}

		file.close();
	}
	
	return const_cast<GLchar *>(result.c_str());
}

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode) {
	if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}

int main() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	GLFWwindow *window = glfwCreateWindow(800, 600, "Learn OpenGL", nullptr, nullptr);
	if(window == nullptr) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwSetKeyCallback(window, key_callback);

	glfwMakeContextCurrent(window);

	glewExperimental = GL_TRUE;
	if(glewInit() != GLEW_OK) {
		std::cout << "Failed to initialise GLEW" << std::endl;
		return -1;
	}

	glViewport(0, 0, 800, 600);

	GLuint VBO;	// Vertex buffer object
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	GLuint vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	GLchar *shdrSrcVertex = read_file("vertex.glsl");
	glShaderSource(vertexShader, 1, &shdrSrcVertex, NULL);

	GLuint fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	GLchar *shdrSrcFragment = read_file("fragment.glsl");
	glShaderSource(fragmentShader, 1, &shdrSrcFragment, NULL);

	GLuint shaderProgram;
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	glUseProgram(shaderProgram);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	delete shdrSrcVertex;
	delete shdrSrcFragment;

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (GLvoid *)NULL);
	glEnableVertexAttribArray(0);

	// Game loop
	while(!glfwWindowShouldClose(window)) {
		glfwPollEvents();
		// Render...
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glfwSwapBuffers(window);
	}

	glfwTerminate();
	return 0;
}
