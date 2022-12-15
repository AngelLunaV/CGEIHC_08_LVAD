/*---------------------------------------------------------*/
/* ----------------   Práctica 4 --------------------------*/
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
float	movX = -10.0f,
		movY = 0.0f,
		movZ = -35.0f,
		rotX = 0.0f,
		rotY = 0.0f,
		rotZ = 0.0f;

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

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Practica 4 2023-1", NULL, NULL);
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
        // Backgound color
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//Mi bloque de dibujo
		/*******************************************/
		glm::mat4 modelOp = glm::mat4(1.0f);		// initialize Matrix, Use this matrix for individual models
		//Use "view" in order to affect all models
		viewOp = glm::translate(glm::mat4(1.0f), glm::vec3(movX, movY, movZ));
		viewOp = glm::rotate(viewOp, glm::radians(rotY), glm::vec3(0.0f, 1.0f, 0.0f));
		viewOp = glm::rotate(viewOp, glm::radians(rotX), glm::vec3(1.0f, 0.0f, 0.0f));
		viewOp = glm::rotate(viewOp, glm::radians(rotZ), glm::vec3(0.0f, 0.0f, 1.0f));
		// pass them to the shaders
		myShader.setMat4("model", modelOp);
		myShader.setMat4("view", viewOp);
		myShader.setMat4("projection", projectionOp);

		//Model
		glBindVertexArray(VAO);
		/*myShader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 0.0f));
		glDrawArrays(GL_TRIANGLES, 0, 36); // primer cubo

		glm::mat4 temp = modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(4.0f, 0.0f, 0.0f));
		modelOp = glm::scale(modelOp, glm::vec3(2.5f, 3.0f, 1.0f));
		myShader.setMat4("model", modelOp);
		myShader.setVec3("aColor", glm::vec3(1.0f, 0.0f, 0.0f));
		glDrawArrays(GL_TRIANGLES, 0, 36); // segundo cubo

		modelOp = glm::translate(temp, glm::vec3(0.0f, 5.5f, 0.0f)); //dibujar a traves de temporal genera dependencia
		modelOp = glm::scale(modelOp, glm::vec3(0.75f, 0.5f, 1.0f));
		myShader.setMat4("model", modelOp);
		myShader.setVec3("aColor", glm::vec3(0.0f, 0.0f, 1.0f));
		glDrawArrays(GL_TRIANGLES, 0, 36); // tercero cubo */
		//88888888888888888888888888888888888888888888888888888888888888
		//Personaje
		
		//pecho origen
		modelOp = glm::scale(glm::mat4(1.0f), glm::vec3(4.0f, 5.0f, 1.0f));
		myShader.setMat4("model", modelOp);
		myShader.setVec3("aColor", glm::vec3(0.3f, 0.6f, 0.3f));
		glDrawArrays(GL_TRIANGLES, 0, 36);
		
		//cuello
		//glm::mat4 temp = modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 2.75f, 0.0f)); //creacion de matriz temporal para poder usarla sin escala
		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 2.75f, 0.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.5f, 0.5f, 1.0f));
		myShader.setMat4("model", modelOp);
		myShader.setVec3("aColor", glm::vec3(0.7f, 0.7f, 0.3f));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//cabeza
		//modelOp = glm::translate(temp, glm::vec3(0.0f, 4.25f, 0.0f)); //trabajando con la matriztemporal
		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 4.25f, 0.0f));
		modelOp = glm::scale(modelOp, glm::vec3(1.5f, 2.5f, 1.0f));
		myShader.setMat4("model", modelOp);
		myShader.setVec3("aColor", glm::vec3(0.8f, 0.8f, 0.3f));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//brazo izq 1
		glm::mat4 temp = modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(-2.75f, 2.25f, 0.0f));
		modelOp = glm::scale(modelOp, glm::vec3(1.5f, 0.5f, 1.0f));
		myShader.setMat4("model", modelOp);
		myShader.setVec3("aColor", glm::vec3(0.3f, 0.5f, 0.3f));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//brazo izq 2
		modelOp = glm::translate(temp, glm::vec3(-0.25f, -2.0f, 0.0f));
		modelOp = glm::scale(modelOp, glm::vec3(1.0f, 3.5f, 1.0f));
		myShader.setMat4("model", modelOp);
		myShader.setVec3("aColor", glm::vec3(0.8f, 0.8f, 0.3f));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//brazo der 1
		temp = modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(2.75f, 2.25f, 0.0f));
		modelOp = glm::scale(modelOp, glm::vec3(1.5f, 0.5f, 1.0f));
		myShader.setMat4("model", modelOp);
		myShader.setVec3("aColor", glm::vec3(0.3f, 0.5f, 0.3f));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//brazo der 2
		modelOp = glm::translate(temp, glm::vec3(0.25f, -2.0f, 0.0f));
		modelOp = glm::scale(modelOp, glm::vec3(1.0f, 3.5f, 1.0f));
		myShader.setMat4("model", modelOp);
		myShader.setVec3("aColor", glm::vec3(0.8f, 0.8f, 0.3f));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//cadera
		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -3.0f, 0.0f));
		modelOp = glm::scale(modelOp, glm::vec3(4.0f, 1.0f, 1.0f));
		myShader.setMat4("model", modelOp);
		myShader.setVec3("aColor", glm::vec3(0.3f, 0.5f, 0.3f));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//pierna izq
		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(-1.25f, -5.25f, 0.0f));
		modelOp = glm::scale(modelOp, glm::vec3(1.5f, 3.5f, 1.0f));
		myShader.setMat4("model", modelOp);
		myShader.setVec3("aColor", glm::vec3(0.3f, 0.3f, 1.0f));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//pie izq
		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(-1.75f, -7.375f, 0.0f));
		modelOp = glm::scale(modelOp, glm::vec3(2.5f, 0.75f, 1.0f));
		myShader.setMat4("model", modelOp);
		myShader.setVec3("aColor", glm::vec3(0.1f, 0.1f, 0.1f));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//pierna der
		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(1.25f, -5.25f, 0.0f));
		modelOp = glm::scale(modelOp, glm::vec3(1.5f, 3.5f, 1.0f));
		myShader.setMat4("model", modelOp);
		myShader.setVec3("aColor", glm::vec3(0.3f, 0.3f, 1.0f));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//pie der
		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(1.75f, -7.375f, 0.0f));
		modelOp = glm::scale(modelOp, glm::vec3(2.5f, 0.75f, 1.0f));
		myShader.setMat4("model", modelOp);
		myShader.setVec3("aColor", glm::vec3(0.1f, 0.1f, 0.1f));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//mango espada
		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(3.0f, -1.65f, 0.0f));
		modelOp = glm::scale(modelOp, glm::vec3(1.0f, 0.3f, 1.0f));
		myShader.setMat4("model", modelOp);
		myShader.setVec3("aColor", glm::vec3(0.3f, 0.0f, 0.0f));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//mango espada2
		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(3.65f, -1.65f, 0.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.3f, 0.6f, 1.0f));
		myShader.setMat4("model", modelOp);
		myShader.setVec3("aColor", glm::vec3(0.3f, 0.0f, 0.0f));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//espada metal
		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(5.55f, -1.65f, 0.0f));
		modelOp = glm::scale(modelOp, glm::vec3(3.5f, 0.3f, 1.0f));
		myShader.setMat4("model", modelOp);
		myShader.setVec3("aColor", glm::vec3(0.6f, 0.8f, 0.8f));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//BABY DUCK
		
		//torso
		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f + 20.0f, 0.0f, 0.0f));
		modelOp = glm::scale(modelOp, glm::vec3(5.0f, 3.0f, 3.0f));
		myShader.setMat4("model", modelOp);
		myShader.setVec3("aColor", glm::vec3(0.7f, 0.6f, 0.0f));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//ala 1
		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f + 20.0f, 0.5f, 2.0f));
		modelOp = glm::scale(modelOp, glm::vec3(3.0f, 2.0f, 1.0f));
		myShader.setMat4("model", modelOp);
		myShader.setVec3("aColor", glm::vec3(0.7f, 0.6f, 0.0f));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//ala 2
		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f + 20.0f, 0.5f, -2.0f));
		modelOp = glm::scale(modelOp, glm::vec3(3.0f, 2.0f, 1.0f));
		myShader.setMat4("model", modelOp);
		myShader.setVec3("aColor", glm::vec3(0.7f, 0.6f, 0.0f));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//cabeza 
		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(1.0f + 20.0f, 3.5f, 0.0f));
		modelOp = glm::scale(modelOp, glm::vec3(3.0f, 4.0f, 3.0f));
		myShader.setMat4("model", modelOp);
		myShader.setVec3("aColor", glm::vec3(0.7f, 0.6f, 0.0f));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//pico 
		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(3.0f + 20.0f, 4.0f, 0.0f));
		modelOp = glm::scale(modelOp, glm::vec3(1.0f, 0.5f, 2.5f));
		myShader.setMat4("model", modelOp);
		myShader.setVec3("aColor", glm::vec3(0.6f, 0.2f, 0.0f));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//cola
		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(-2.0f + 20.0f, 1.75f, 0.0f));
		modelOp = glm::scale(modelOp, glm::vec3(1.0f, 0.5f, 2.25f));
		myShader.setMat4("model", modelOp);
		myShader.setVec3("aColor", glm::vec3(0.7f, 0.6f, 0.0f));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//pata1
		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(0.5f + 20.0f, -2.5f, 1.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.5f, 2.0f, 0.5f));
		myShader.setMat4("model", modelOp);
		myShader.setVec3("aColor", glm::vec3(0.6f, 0.2f, 0.0f));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//pata1.2
		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(0.5f + 20.0f, -3.625f, 1.0f));
		modelOp = glm::scale(modelOp, glm::vec3(1.25f, 0.25f, 1.25f));
		myShader.setMat4("model", modelOp);
		myShader.setVec3("aColor", glm::vec3(0.6f, 0.2f, 0.0f));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//pata2
		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(0.5f + 20.0f, -2.5f, -1.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.5f, 2.0f, 0.5f));
		myShader.setMat4("model", modelOp);
		myShader.setVec3("aColor", glm::vec3(0.6f, 0.2f, 0.0f));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//pata2.2
		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(0.5f + 20.0f, -3.625f, -1.0f));
		modelOp = glm::scale(modelOp, glm::vec3(1.25f, 0.25f, 1.25f));
		myShader.setMat4("model", modelOp);
		myShader.setVec3("aColor", glm::vec3(0.6f, 0.2f, 0.0f));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//ojo1
		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(1.5f + 20.0f, 4.5f, 1.55f));
		modelOp = glm::scale(modelOp, glm::vec3(0.5f, 0.5f, 0.1f));
		myShader.setMat4("model", modelOp);
		myShader.setVec3("aColor", glm::vec3(0.45f, 0.2f, 0.05f));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//ojo2
		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(1.5f + 20.0f, 4.5f, -1.55f));
		modelOp = glm::scale(modelOp, glm::vec3(0.5f, 0.5f, 0.1f));
		myShader.setMat4("model", modelOp);
		myShader.setVec3("aColor", glm::vec3(0.45f, 0.2f, 0.05f));
		glDrawArrays(GL_TRIANGLES, 0, 36);
		//88888888888888888888888888888888888888888888888888888888888888
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
		movX += 0.08f;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		movX -= 0.08f;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		movY += 0.08f;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		movY -= 0.08f;
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
		movZ -= 0.08f;
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
		movZ += 0.08f;
	if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)  //GLFW_RELEASE
		rotX -= 3.0f;
	if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)  //GLFW_RELEASE
		rotX += 3.0f;
	if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)  //GLFW_RELEASE
		rotY -= 3.0f;
	if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS)  //GLFW_RELEASE
		rotY += 3.0f;
	if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS)  //GLFW_RELEASE
		rotZ -= 3.0f;
	if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS)  //GLFW_RELEASE
		rotZ += 3.0f;

}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void resize(GLFWwindow* window, int width, int height)
{
    // Set the Viewport to the size of the created window
    glViewport(0, 0, width, height);
}