/*---------------------------------------------------------*/
/* ----------------   Práctica 2 --------------------------*/
/*-----------------    2023-1   ---------------------------*/
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
GLuint VBO[2], VAO[2], EBO[2];
GLuint shaderProgramYellow, shaderProgramColor;

static const char* myVertexShader = "										\n\
#version 330 core															\n\
																			\n\
layout (location = 0) in vec3 aPos;											\n\
																			\n\
void main()																	\n\
{																			\n\
    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);							\n\
}";

static const char* myVertexShaderColor = "									\n\
#version 330 core															\n\
																			\n\
layout (location = 0) in vec3 aPos;											\n\
layout (location = 1) in vec3 aColor;										\n\
out vec3 ourColor;															\n\
void main()																	\n\
{																			\n\
    gl_Position = vec4(aPos, 1.0);											\n\
	ourColor = aColor;														\n\
}";

// Fragment Shader
static const char* myFragmentShaderYellow = "									\n\
#version 330																\n\
																			\n\
out vec3 finalColor;														\n\
																			\n\
void main()																	\n\
{																			\n\
    finalColor = vec3(1.0f, 0.0f, 1.0f);									\n\
}";

static const char* myFragmentShaderColor = "								\n\
#version 330 core															\n\
out vec4 FragColor;															\n\
in vec3 ourColor;															\n\
																			\n\
void main()																	\n\
{																			\n\
	FragColor = vec4(ourColor, 1.0f);										\n\
}";

void myData(void);
void setupShaders(void);
void display(void);
void getResolution(void);


void getResolution()
{
	const GLFWvidmode * mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

	SCR_WIDTH = mode->width;
	SCR_HEIGHT = (mode->height) - 80;
}

void myData()
{
	float vertices[] = 
	{
		// positions         //COLOR
		
		-0.5f, 0.5f, 0.0f,	1.0f, 1.0f, 1.0f,	// 0
		0.0f, 0.5f, 0.0f,	1.0f, 0.0f, 0.0f,	// 1
		0.0f, 0.4f, 0.0f,	0.0f, 1.0f, 0.0f,	// 2
		-0.4f, 0.4f, 0.0f,	1.0f, 1.0f, 0.0f,	// 3
		-0.4f, -0.1f, 0.0f,	0.0f, 0.0f, 1.0f,	// 4
		0.0f, -0.1f, 0.0f,	0.0f, 1.0f, 1.0f,	// 5
		0.0f, -0.2f, 0.0f,	1.0f, 0.0f, 1.0f,	// 6
		-0.5f, -0.2f, 0.0f,	0.6f, 0.2f, 0.75f,	// 7
		/*
		-0.5f, 0.5f, 0.0f,	1.0f, 1.0f, 1.0f,	// 0
		0.0f, 0.5f, 0.0f,	1.0f, 0.0f, 0.0f,	// 1
		-0.4f, 0.4f, 0.0f,	1.0f, 1.0f, 0.0f,	// 3

		0.0f, 0.5f, 0.0f,	1.0f, 0.0f, 0.0f,	// 1
		0.0f, 0.4f, 0.0f,	0.0f, 1.0f, 0.0f,	// 2
		-0.4f, 0.4f, 0.0f,	1.0f, 1.0f, 0.0f,	// 3

		-0.5f, 0.5f, 0.0f,	1.0f, 1.0f, 1.0f,	// 0
		-0.4f, 0.4f, 0.0f,	1.0f, 1.0f, 0.0f,	// 3
		-0.5f, 0.0f, 0.0f,	0.6f, 0.2f, 0.75f,	// 7

		- 0.4f, 0.4f, 0.0f,	1.0f, 1.0f, 0.0f,	// 3
		-0.4f, 0.1f, 0.0f,	0.0f, 0.0f, 1.0f,	// 4
		-0.5f, 0.0f, 0.0f,	0.6f, 0.2f, 0.75f,	// 7

		- 0.4f, 0.1f, 0.0f,	0.0f, 0.0f, 1.0f,	// 4
		-0.5f, 0.0f, 0.0f,	0.6f, 0.2f, 0.75f,	// 7
		0.0f, 0.0f, 0.0f,	1.0f, 0.0f, 1.0f,	// 6

		-0.4f, 0.1f, 0.0f,	0.0f, 0.0f, 1.0f,	// 4
		0.0f, 0.1f, 0.0f,	0.0f, 1.0f, 1.0f,	// 5
		0.0f, 0.0f, 0.0f,	1.0f, 0.0f, 1.0f	// 6
		*/
		// estrella
		-0.02f, 0.1f, 0.0f,	0.5f, 1.0f, 0.5f,	//0	8
		0.15f, 0.1f, 0.0f, 	0.5f, 1.0f, 0.5f, 	//1	9
		0.2f, 0.4f, 0.0f,	0.5f, 1.0f, 0.5f,	//2	10
		0.25f, 0.1f, 0.0f,	0.5f, 1.0f, 0.5f,	//3	11
		0.42f, 0.1f, 0.0f,	0.5f, 1.0f, 0.5f,	//4	12
		0.3f, -0.1f, 0.0f,	0.5f, 1.0f, 0.75f,	//5	13
		0.35f, -0.4f, 0.0f,	0.5f, 1.0f, 0.75f,	//6	14
		0.2f, -0.2f, 0.0f,	0.5f, 1.0f, 0.75f,	//7	15
		0.05f, -0.4f, 0.0f,	0.9f, 1.0f, 0.75f,	//8	16
		0.1f, -0.1f, 0.0f,	0.5f, 1.0f, 0.75f,	//9	17

		//Letra A
		0.7f, 0.4f, 0.0f,	1.0f, 0.0f, 0.0f,	//0 18
		0.9f, 0.2f, 0.0f,	1.0f, 1.0f, 0.0f,	//1 19
		0.9f, -0.3f, 0.0f,	0.0f, 0.0f, 1.0f,	//2 20
		0.8f, -0.3f, 0.0f,	0.0f, 1.0f, 0.0f,	//3 21
		0.8f, -0.1f, 0.0f,	0.0f, 1.0f, 1.0f,	//4 22
		0.6f, -0.1f, 0.0f,	1.0f, 0.0f, 1.0f,	//5 23
		0.6f, -0.3f, 0.0f,	1.0f, 1.0f, 1.0f,	//6 24
		0.5f, -0.3f, 0.0f,	0.6f, 0.2f, 0.8f,	//7 25
		0.5f, 0.2f, 0.0f,	1.0f, 0.0f, 0.0f,	//8 26
		 
		0.7f, 0.3f, 0.0f,	0.0f, 0.0f, 0.0f,	//0 27
		0.8f, 0.2f, 0.0f,	0.0f, 0.0f, 0.0f,	//1 28
		0.8f, 0.0f, 0.0f,	0.0f, 0.0f, 0.0f,	//2 29
		0.6f, 0.0f, 0.0f,	0.0f, 0.0f, 0.0f,	//3 30
		0.6f, 0.2f, 0.0f,	0.0f, 0.0f, 0.0f	//4 31
	};

	unsigned int indices[] =
	{
		//0, 1, 4, 2, 3,
		// C TRIANGLES FAN
		3, 2, 1, 0, 7, 4,
		4, 7, 6, 5,
		//C TRIANGLES
		/*0,1,3,
		1,2,3,
		0,3,7,
		3,4,7,
		4,7,6,
		4,5,6*/

		// Estrella TRIANGLES
		8,11,14,
		9,10,11,
		9,16,12,

		// Letra A TRIANGLES
		18,23,22,
		18,22,19,
		18,23,26,
		23,26,25,
		25,24,23,
		22,21,20,
		20,22,19,

		27,31,28,
		30,31,28,
		28,29,30
	};

	glGenVertexArrays(2, VAO);
	glGenBuffers(2, VBO);
	glGenBuffers(2, EBO);



	glBindVertexArray(VAO[0]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	//Para trabajar con indices (Element Buffer Object)
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[0]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);

}

void setupShaders()
{
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &myVertexShader, NULL);
	glCompileShader(vertexShader);

	unsigned int vertexShaderColor = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShaderColor, 1, &myVertexShaderColor, NULL);
	glCompileShader(vertexShaderColor);

	unsigned int fragmentShaderYellow = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShaderYellow, 1, &myFragmentShaderYellow, NULL);
	glCompileShader(fragmentShaderYellow);

	unsigned int fragmentShaderColor = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShaderColor, 1, &myFragmentShaderColor, NULL);
	glCompileShader(fragmentShaderColor);


	//Crear el Programa que combina Geometría con Color
	shaderProgramYellow = glCreateProgram();
	glAttachShader(shaderProgramYellow, vertexShader);
	glAttachShader(shaderProgramYellow, fragmentShaderYellow);
	glLinkProgram(shaderProgramYellow);

	shaderProgramColor = glCreateProgram();
	glAttachShader(shaderProgramColor, vertexShaderColor);
	glAttachShader(shaderProgramColor, fragmentShaderColor);
	glLinkProgram(shaderProgramColor);
	//Check for errors 

	//ya con el Programa, el Shader no es necesario
	glDeleteShader(vertexShader);
	glDeleteShader(vertexShaderColor);
	glDeleteShader(fragmentShaderYellow);
	glDeleteShader(fragmentShaderColor);

}

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    /*glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);*/

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif

    // glfw window creation
    // --------------------
	monitors = glfwGetPrimaryMonitor();
	getResolution();

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Practica 2", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
	glfwSetWindowPos(window, 0, 30);
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, resize);

	glewInit();


	//My Functions
	//Setup Data to use
	myData();
	//To Setup Shaders
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

		//Display Section
		glUseProgram(shaderProgramColor);

		glBindVertexArray(VAO[0]);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[0]);

		glPointSize(5.0);
		glDrawElements(GL_TRIANGLE_FAN, 6, GL_UNSIGNED_INT, 0);
		glDrawElements(GL_TRIANGLE_FAN, 4, GL_UNSIGNED_INT, (void*)(6*sizeof(float)));
		glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_INT, (void*)(10 * sizeof(float)));
		glDrawElements(GL_TRIANGLES, 21, GL_UNSIGNED_INT, (void*)(19 * sizeof(float)));
		glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_INT, (void*)(40 * sizeof(float)));
		//glDrawArrays(GL_TRIANGLES, 0, 18);


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