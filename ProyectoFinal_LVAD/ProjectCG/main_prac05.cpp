/*---------------------------------------------------------*/
/* ----------------   Práctica 5 --------------------------*/
/*-----------------    2023-1   ---------------------------*/
/*------------- Alumno: Luna Villaseñor Angel David -------*/
/*------------- No. de cuenta: 315272244    ---------------*/
#include <glew.h>
#include <glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <shader_m.h>

#include <iostream>

void resize(GLFWwindow* window, int width, int height);
void my_input(GLFWwindow *window);

// settings
// Window size
int SCR_WIDTH = 3800;
int SCR_HEIGHT = 7600;

GLFWmonitor *monitors;
GLuint VBO, VAO, EBO;

void myData(void);
void getResolution(void);

//For Keyboard
float	movX = -1.5f,
		movY = 0.0f,
		movZ = -10.0f;
float	rotY = 0.0f,
		rotX = 0.0f,
		rotZ = 0.0f;

float	hombro = 0.0f,
		codo = 0.0f,
		muneca = 0.0f,
		dedo1pt1 = 0.0f,
		dedo1pt2 = 0.0f,
		dedo2pt1 = 0.0f,
		dedo2pt2 = 0.0f,
		dedo3pt1 = 0.0f,
		dedo3pt2 = 0.0f,
		dedo4pt1 = 0.0f,
		dedo4pt2 = 0.0f;

void getResolution()
{
	const GLFWvidmode * mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

	SCR_WIDTH = mode->width;
	SCR_HEIGHT = (mode->height) - 80;
}

void myData()
{
	GLfloat verticesCubo[] = {
		//Position				//Color
		-0.5f, -0.5f, 0.5f,		//V0 - Frontal
		0.5f, -0.5f, 0.5f,		//V1
		0.5f, 0.5f, 0.5f,		//V5
		-0.5f, -0.5f, 0.5f,		//V0
		-0.5f, 0.5f, 0.5f,		//V4
		0.5f, 0.5f, 0.5f,		//V5

		0.5f, -0.5f, -0.5f,		//V2 - Trasera
		-0.5f, -0.5f, -0.5f,	//V3
		-0.5f, 0.5f, -0.5f,		//V7
		0.5f, -0.5f, -0.5f,		//V2
		0.5f, 0.5f, -0.5f,		//V6
		-0.5f, 0.5f, -0.5f,		//V7

		-0.5f, 0.5f, 0.5f,		//V4 - Izq
		-0.5f, 0.5f, -0.5f,		//V7
		-0.5f, -0.5f, -0.5f,	//V3
		-0.5f, -0.5f, -0.5f,	//V3
		-0.5f, 0.5f, 0.5f,		//V4
		-0.5f, -0.5f, 0.5f,		//V0

		0.5f, 0.5f, 0.5f,		//V5 - Der
		0.5f, -0.5f, 0.5f,		//V1
		0.5f, -0.5f, -0.5f,		//V2
		0.5f, 0.5f, 0.5f,		//V5
		0.5f, 0.5f, -0.5f,		//V6
		0.5f, -0.5f, -0.5f,		//V2

		-0.5f, 0.5f, 0.5f,		//V4 - Sup
		0.5f, 0.5f, 0.5f,		//V5
		0.5f, 0.5f, -0.5f,		//V6
		-0.5f, 0.5f, 0.5f,		//V4
		-0.5f, 0.5f, -0.5f,		//V7
		0.5f, 0.5f, -0.5f,		//V6

		-0.5f, -0.5f, 0.5f,		//V0 - Inf
		-0.5f, -0.5f, -0.5f,	//V3
		0.5f, -0.5f, -0.5f,		//V2
		-0.5f, -0.5f, 0.5f,		//V0
		0.5f, -0.5f, -0.5f,		//V2
		0.5f, -0.5f, 0.5f,		//V1
	};

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticesCubo), verticesCubo, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);
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

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Practica 5 - 20231", NULL, NULL);
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


	//Mis funciones
	//Datos a utilizar
	myData();
	glEnable(GL_DEPTH_TEST);

	Shader myShader("shaders/shader_projection.vs", "shaders/shader_projection.fs");
	myShader.use();

	glm::mat4 viewOp = glm::mat4(1.0f);			//Use this matrix for ALL models
	glm::mat4 projectionOp = glm::mat4(1.0f);	//This matrix is for Projection
	glm::mat4 Aux1 = glm::mat4(1.0f);
	glm::mat4 Aux2 = glm::mat4(1.0f);

	//Use "projection" in order to change how we see the information
	projectionOp = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

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
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//Mi bloque de dibujo
		/*******************************************/
		glm::mat4 modelOp = glm::mat4(1.0f);		// initialize Matrix, Use this matrix for individual models
		//Use "view" in order to affect all models
		viewOp = glm::translate(glm::mat4(1.0f), glm::vec3(movX, movY, movZ));
		viewOp = glm::rotate(viewOp, glm::radians(rotX), glm::vec3(1.0f, 0.0f, 0.0f));
		viewOp = glm::rotate(viewOp, glm::radians(rotY), glm::vec3(0.0f, 1.0f, 0.0f));
		viewOp = glm::rotate(viewOp, glm::radians(rotZ), glm::vec3(0.0f, 0.0f, 1.0f));
		
		// pass them to the shaders
		myShader.setMat4("model", modelOp);
		myShader.setMat4("view", viewOp);
		myShader.setMat4("projection", projectionOp);

		//Model
		glBindVertexArray(VAO);

		//modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
		//modelOp = glm::scale(modelOp, glm::vec3(1.0f, 1.0f, 1.0f));
		//myShader.setMat4("model", modelOp);
		//myShader.setVec3("aColor", glm::vec3(1.0f, 0.0f, 0.0f));
		//glDrawArrays(GL_TRIANGLES, 0, 36);	//Dibujo de elemento
		//8888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(-1.0f, 0.0f, 0.0f));
		modelOp = glm::rotate(modelOp, glm::radians(hombro), glm::vec3(0.0f, 0.0, 1.0f));
		//hombro
		Aux1 = modelOp = glm::translate(modelOp, glm::vec3(1.0f, 0.0f, 0.0f));
		modelOp = glm::scale(modelOp, glm::vec3(2.0f, 1.0f, 1.0f));
		myShader.setMat4("model", modelOp);
		myShader.setVec3("aColor", glm::vec3(1.0f, 0.0f, 0.0f));
		glDrawArrays(GL_TRIANGLES, 0, 36);	//Dibujo A

		//art codo
		modelOp = glm::translate(Aux1, glm::vec3(1.0f, 0.0f, 0.0f));
		modelOp = glm::rotate(modelOp, glm::radians(codo), glm::vec3(0.0f, 1.0, 0.0f));
		//codo
		Aux1 = modelOp = glm::translate(modelOp, glm::vec3(1.25f, 0.0f, 0.0f));
		modelOp = glm::scale(modelOp, glm::vec3(2.5f, 1.0f, 1.0f));
		myShader.setMat4("model", modelOp);
		myShader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 0.0f));
		glDrawArrays(GL_TRIANGLES, 0, 36);	//Dibujo B

		//art dedo1pt1
		modelOp = glm::translate(Aux1, glm::vec3(1.25f, -0.35f, 0.375f));
		modelOp = glm::rotate(modelOp, glm::radians(dedo1pt1), glm::vec3(0.0f, 1.0, 0.0f));
		//dedo1pt1
		Aux2 = modelOp = glm::translate(modelOp, glm::vec3(0.25f, 0.0f, 0.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.5f, 0.3f, 0.25f));
		myShader.setMat4("model", modelOp);
		myShader.setVec3("aColor", glm::vec3(0.0f, 1.0f, 0.0f));
		glDrawArrays(GL_TRIANGLES, 0, 36);	//Dibujo E

		//art dedo1pt2
		modelOp = glm::translate(Aux2, glm::vec3(0.25f, 0.0f, 0.0f));
		modelOp = glm::rotate(modelOp, glm::radians(dedo1pt2), glm::vec3(0.0f, 1.0, 0.0f));
		//dedo1pt2
		Aux2 = modelOp = glm::translate(modelOp, glm::vec3(0.25f, 0.0f, 0.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.5f, 0.3f, 0.25f));
		myShader.setMat4("model", modelOp);
		myShader.setVec3("aColor", glm::vec3(0.0f, 1.0f, 1.0f));
		glDrawArrays(GL_TRIANGLES, 0, 36);	//Dibujo F

		//art dedo2pt1
		modelOp = glm::translate(Aux1, glm::vec3(1.25f, 0.35f, 0.375f));
		modelOp = glm::rotate(modelOp, glm::radians(dedo2pt1), glm::vec3(0.0f, 0.0, 1.0f));
		//dedo2pt1
		Aux2 = modelOp = glm::translate(modelOp, glm::vec3(0.25f, 0.0f, 0.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.5f, 0.3f, 0.25f));
		myShader.setMat4("model", modelOp);
		myShader.setVec3("aColor", glm::vec3(0.0f, 1.0f, 0.0f));
		glDrawArrays(GL_TRIANGLES, 0, 36);	//Dibujo C

		//art dedo2pt2
		modelOp = glm::translate(Aux2, glm::vec3(0.25f, 0.0f, 0.0f));
		modelOp = glm::rotate(modelOp, glm::radians(dedo2pt2), glm::vec3(0.0f, 0.0, 1.0f));
		//dedo2pt2
		Aux2 = modelOp = glm::translate(modelOp, glm::vec3(0.325f, 0.0f, 0.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.75f, 0.3f, 0.25f));
		myShader.setMat4("model", modelOp);
		myShader.setVec3("aColor", glm::vec3(0.0f, 1.0f, 1.0f));
		glDrawArrays(GL_TRIANGLES, 0, 36);	//Dibujo D

		//art dedo3pt1
		modelOp = glm::translate(Aux1, glm::vec3(1.25f, 0.35f, -0.375f));
		modelOp = glm::rotate(modelOp, glm::radians(dedo3pt1), glm::vec3(0.0f, 0.0, 1.0f));
		//dedo3pt1
		Aux2 = modelOp = glm::translate(modelOp, glm::vec3(0.375f, 0.0f, 0.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.75f, 0.3f, 0.25f));
		myShader.setMat4("model", modelOp);
		myShader.setVec3("aColor", glm::vec3(0.0f, 1.0f, 0.0f));
		glDrawArrays(GL_TRIANGLES, 0, 36);	//Dibujo I
		

		//art dedo3pt2
		modelOp = glm::translate(Aux2, glm::vec3(0.325f, 0.0f, 0.0f));
		modelOp = glm::rotate(modelOp, glm::radians(dedo3pt2), glm::vec3(0.0f, 0.0, 1.0f));
		//dedo3pt2
		Aux2 = modelOp = glm::translate(modelOp, glm::vec3(0.325f, 0.0f, 0.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.75f, 0.3f, 0.25f));
		myShader.setMat4("model", modelOp);
		myShader.setVec3("aColor", glm::vec3(0.0f, 1.0f, 1.0f));
		glDrawArrays(GL_TRIANGLES, 0, 36);	//Dibujo J

		//art dedo4pt1
		modelOp = glm::translate(Aux1, glm::vec3(1.25f, 0.35f, 0.0f));
		modelOp = glm::rotate(modelOp, glm::radians(dedo4pt1), glm::vec3(0.0f, 0.0, 1.0f));
		//dedo4pt1
		Aux2 = modelOp = glm::translate(modelOp, glm::vec3(0.375f, 0.0f, 0.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.75f, 0.3f, 0.25f));
		myShader.setMat4("model", modelOp);
		myShader.setVec3("aColor", glm::vec3(0.0f, 1.0f, 0.0f));
		glDrawArrays(GL_TRIANGLES, 0, 36);	//Dibujo H

		//art dedo4pt2
		modelOp = glm::translate(Aux2, glm::vec3(0.325f, 0.0f, 0.0f));
		modelOp = glm::rotate(modelOp, glm::radians(dedo4pt2), glm::vec3(0.0f, 0.0, 1.0f));
		//dedo4pt2
		Aux2 = modelOp = glm::translate(modelOp, glm::vec3(0.5f, 0.0f, 0.0f));
		modelOp = glm::scale(modelOp, glm::vec3(1.0f, 0.3f, 0.25f));
		myShader.setMat4("model", modelOp);
		myShader.setVec3("aColor", glm::vec3(0.0f, 1.0f, 1.0f));
		glDrawArrays(GL_TRIANGLES, 0, 36);	//Dibujo G
		

		//8888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888
		glBindVertexArray(0);
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
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		movX += 0.1f;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		movX -= 0.1f;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		movY += 0.1f;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		movY -= 0.1f;
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
		movZ -= 0.1f;
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
		movZ += 0.1f;
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		rotY += 1.0f;
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		rotY -= 1.0f;
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		rotX += 1.0f;
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		rotX -= 1.0f;
	//movimiento brazo
	if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS && 90.0f >= hombro)
		hombro += 0.8f;
	if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS && -120.0f <= hombro)
		hombro -= 0.8f;
	if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS && 5.0f >= codo)
		codo += 0.8f;
	if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS && -100.0f <= codo)
		codo -= 0.8f;
	if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS && 75.0f >= dedo1pt1)
		dedo1pt1 += 0.8f;
	if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS && -90.0f <= dedo1pt1)
		dedo1pt1 -= 0.8f;
	if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS && 90.0f >= dedo1pt2)
		dedo1pt2 += 0.8f;
	if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS && -5.0f <= dedo1pt2)
		dedo1pt2 -= 0.8f;
	if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS && 30.0f >= dedo2pt1)
		dedo2pt1 += 0.8f;
	if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS && -90.0f <= dedo2pt1)
		dedo2pt1 -= 0.8f;
	if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS && 7.0f >= dedo2pt2)
		dedo2pt2 += 0.8f;
	if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS && -120.0f <= dedo2pt2)
		dedo2pt2 -= 0.8f;
	if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS && 30.0f >= dedo3pt1)
		dedo3pt1 += 0.8f;
	if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS && -90.0f <= dedo3pt1)
		dedo3pt1 -= 0.8f;
	if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS && 7.0f >= dedo3pt2)
		dedo3pt2 += 0.8f;
	if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS && -120.0f <= dedo3pt2)
		dedo3pt2 -= 0.8f;
	if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS && 30.0f >= dedo4pt1)
		dedo4pt1 += 0.8f;
	if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS && -90.0f <= dedo4pt1)
		dedo4pt1 -= 0.8f;
	if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS && 7.0f >= dedo4pt2)
		dedo4pt2 += 0.8f;
	if (glfwGetKey(window, GLFW_KEY_COMMA) == GLFW_PRESS && -120.0f <= dedo4pt2)
		dedo4pt2 -= 0.8f;



}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void resize(GLFWwindow* window, int width, int height)
{
    // Set the Viewport to the size of the created window
	//2023-1
    glViewport(0, 0, width, height);
}