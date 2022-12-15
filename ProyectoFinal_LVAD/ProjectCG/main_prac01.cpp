/*---------------------------------------------------------*/
/* ----------------   Práctica 1 --------------------------*/
/*-----------------    2023-1   ---------------------------*/
/*------------- Luis Sergio Valencia Castro ---------------*/
/*------------- Luna Villaseñor Angel David ---------------*/
#include <glew.h>
#include <glfw3.h>

#include <iostream>

void resize(GLFWwindow* window, int width, int height);
void my_input(GLFWwindow *window);

// settings
// Window size
int SCR_WIDTH = 800;
int SCR_HEIGHT = 600;

GLFWmonitor *monitors;
GLuint VBO, VAO;
GLuint shaderProgramRed;

static const char* myVertexShader = "										\n\
#version 330																\n\
																			\n\
layout (location = 0) in vec4 pos;											\n\
																			\n\
void main()																	\n\
{																			\n\
    gl_Position = vec4(pos.x, pos.y, pos.z, 1.0);							\n\
}";

// Fragment Shader
static const char* myFragmentShaderRed = "									\n\
#version 330																\n\
																			\n\
out vec3 finalColor;														\n\
																			\n\
void main()																	\n\
{																			\n\
    finalColor = vec3(0.5f, 0.5f, 1.0f);									\n\
}";

void myData(void);
void setupShaders(void);
void getResolution(void);


void getResolution()
{
	const GLFWvidmode * mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

	SCR_WIDTH = mode->width;
	SCR_HEIGHT = (mode->height) - 80;
}

void myData()
{
	GLfloat vertices[] = {
		0.0f, 0.0f, 0.0f,	//0
		0.6f, 0.2f, 0.0f,	//1
		0.2f, 0.8f, 0.0f,	//2
		-0.4f, 0.6f, 0.0f,	//3
		-0.7f, -0.7f, 0.0f,	//4
		0.1f, -0.7f, 0.0f,	//5

		//Letra C
		0.5f,  0.6f, 0.0f,	//0
		0.5f,  0.5f,  0.0f,	//1
		-0.2f, 0.5f, 0.0f,	//2
		-0.2f, -0.5f, 0.0f,	//3
		0.5f,  -0.5f, 0.0f,	//4
		0.5f, -0.6f, 0.0f,	//5
		-0.4f, -0.6f, 0.0f,	//6
		-0.4f, 0.6f, 0.0f,	//7

		//Estrella
		-0.02f, 0.1f, 0.0f,	//0
		0.15f, 0.1f, 0.0f,  //1
		0.2f, 0.4f, 0.0f,	//2
		0.25f, 0.1f, 0.0f,	//3
		0.42f, 0.1f, 0.0f,	//4
		0.3f, -0.1f, 0.0f,	//5
		0.35f, -0.4f, 0.0f,	//6
		0.2f, -0.2f, 0.0f,	//7
		0.05f, -0.4f, 0.0f,	//8
		0.1f, -0.1f, 0.0f,	//9
		//Letra A
		0.7f, 0.4f, 0.0f,	//0
		0.9f, 0.2f, 0.0f,  //1
		0.9f, -0.3f, 0.0f,	//2
		0.8f, -0.3f, 0.0f,	//3
		0.8f, -0.1f, 0.0f,	//4
		0.6f, -0.1f, 0.0f,	//5
		0.6f, -0.3f, 0.0f,	//6
		0.5f, -0.3f, 0.0f,	//7
		0.5f, 0.2f, 0.0f,	//8

		0.7f, 0.3f, 0.0f,	//0
		0.8f, 0.2f, 0.0f,  //1
		0.8f, 0.0f, 0.0f,	//2
		0.6f, 0.0f, 0.0f,	//3
		0.6f, 0.2f, 0.0f	//4
	};

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);

}

void setupShaders()
{
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &myVertexShader, NULL);
	glCompileShader(vertexShader);

	unsigned int fragmentShaderRed = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShaderRed, 1, &myFragmentShaderRed, NULL);
	glCompileShader(fragmentShaderRed);

	shaderProgramRed = glCreateProgram();
	glAttachShader(shaderProgramRed, vertexShader);
	glAttachShader(shaderProgramRed, fragmentShaderRed);
	glLinkProgram(shaderProgramRed);

	//Check for errors 

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShaderRed);

}


int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    /*glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	*/

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif

    // glfw window creation
    // --------------------
	monitors = glfwGetPrimaryMonitor();
	getResolution();

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Practica 1", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
	glfwSetWindowPos(window, 0, 30); // posicion donde aparece la ventana
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, resize);

	glewExperimental = GL_TRUE;
	glewInit();

	myData();
	setupShaders();
    

    // render loop
    // While the windows is not closed
    while (!glfwWindowShouldClose(window))
    {
        // input
        // -----
        my_input(window);

        // render
        // Background color
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);


		//Display section
		glUseProgram(shaderProgramRed);
		glBindVertexArray(VAO);                  
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glPointSize(4.0f);			// tamaño del punto
		//glDrawArrays(GL_TRIANGLE_FAN, 0, 6);
		glDrawArrays(GL_LINE_LOOP, 6, 8);
		glDrawArrays(GL_LINE_LOOP, 14, 10);
		glDrawArrays(GL_LINE_LOOP, 24, 9);
		glDrawArrays(GL_LINE_LOOP, 33, 5);
		glBindVertexArray(0);


		glUseProgram(0);
		//End of Display Section


        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void my_input(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)  //GLFW_RELEASE
        glfwSetWindowShouldClose(window, true);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void resize(GLFWwindow* window, int width, int height)
{
    // Set the Viewport to the size of the created window
    glViewport(0, 0, width, height);
}