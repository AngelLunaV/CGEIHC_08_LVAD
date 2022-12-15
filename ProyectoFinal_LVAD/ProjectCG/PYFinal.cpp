/*---------------------------------------------------------*/
/* ----------------  Proyecto Final             -----------*/
/*-----------------    2023-1   ---------------------------*/
/*------------- Alumno: Luna Villaseñor Angel David -------*/
/*------------- No. Cuenta: 315272244       ---------------*/
#include <Windows.h>

#include <glad/glad.h>
#include <glfw3.h>	//main
#include <stdlib.h>		
#include <glm/glm.hpp>	//camera y model
#include <glm/gtc/matrix_transform.hpp>	//camera y model
#include <glm/gtc/type_ptr.hpp>
#include <time.h>
#include <iostream>
#include <fstream>


#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>	//Texture

#define SDL_MAIN_HANDLED
#include <SDL/SDL.h>

#include <shader_m.h>
#include <camera.h>
#include <modelAnim.h>
#include <model.h>
#include <Skybox.h>
#include <iostream>

#pragma comment(lib, "winmm.lib")

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
//void my_input(GLFWwindow *window);
void my_input(GLFWwindow* window, int key, int scancode, int action, int mods);
void animate(void);

// settings
unsigned int SCR_WIDTH = 800;
unsigned int SCR_HEIGHT = 600;
GLFWmonitor *monitors;

void getResolution(void);

// camera
Camera camera(glm::vec3(0.0f, 10.0f, 90.0f));
float MovementSpeed = 0.1f;
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
const int FPS = 60;
const int LOOP_TIME = 1000 / FPS; // = 16 milisec // 1000 millisec == 1 sec
double	deltaTime = 0.0f,
		lastFrame = 0.0f;

//Lighting
glm::vec3 lightPosition(0.0f, 4.0f, -10.0f);
glm::vec3 lightDirection(0.0f, -1.0f, -1.0f);
glm::vec3 lightColor(0.0f, 0.0f, 0.0f);

// posiciones
//float x = 0.0f;
//float y = 0.0f;
float	movAuto_x = 0.0f,
		movAuto_z = 0.0f,
		movAuto_y = 0.0f,
		orienta = 0.0f;

float	giroLlantas = 0.0f;
int		estado = 0,
		estadopez = 0,
		estadopez2 = 0,
		estadopez3 = 0,
		estadoTortuga = 0;

bool	animacion = false,
		recorrido1 = true,
		recorrido2 = false,
		recorrido3 = false,
		recorrido4 = false;

float	miVariable = 0.0f,
		miVariable2 = -1.0f,
		miColorR = 0.0f,
		miColorG = 0.0f,
		miColorB = 0.0f,
		Encendidox = 0.0f,
		Encendidoy = 0.0f,
		Encendidoz = 0.0f;

bool	sound = false;

float	pez1X = 66.0f,
		pez1Y = 20.0f,
		pez1Z = -111.0f,
		pez1R = 0.0f,

		pez2X = 66.0f,
		pez2Y = 30.0f,
		pez2Z = 107.0f,
		pez2R = 0.0f,

		pez3X = 69.0f,
		pez3Y = 12.0f,
		pez3Z = -111.0f,
		pez3R = 0.0f,

		pez4X = 69.0f,
		pez4Y = 46.0f,
		pez4Z = 107.0f,
		pez4R = 0.0f,
	
		pez5X = -49.0f,
		pez5Y = 14.0f,
		pez5Z = 21.0f,
		pez5R = 0.0f,
	
		pez6X = -76.0f,
		pez6Y = 34.0f,
		pez6Z = 82.0f,
		pez6R = -180.0f,

		pez7X = -66.0f,
		pez7Y = 14.0f,
		pez7Z = 181.0f,
		pez7R = 0.0f,

		pez8X = -66.0f,
		pez8Y = 34.0f,
		pez8Z = 154.0f,
		pez8R = -270.0f,

		pez9X = 47.0f,
		pez9Y = 14.0f,
		pez9Z = 155.0f,
		pez9R = -90.0f,

		pez10X = 47.0f,
		pez10Y = 44.0f,
		pez10Z = 172.0f,
		pez10R = -90.0f,
	
		torX = -43.0f,
		torY = 30.0f,
		torZ = 25.0f,
		torR = 0.0f;

float	posx = 0.0f,
		posy = 0.0f,
		posz = 0.0f,
		posX = 0.0f,
		posY = 0.0f,
		posZ = 0.0f,
		rotr = 0.0f,
		rotR = 0.0f;
		
//shark
float	sharkX = 371.0f,
		sharkY = 244.0f,
		sharkZ = -345.0f,
		sharkB = 0.0f,
		sharkC = -13.0f,
		sharkD = 11.0f,
		sharkI = -9.0f,
		pezX = 73.0f,
		pezY = 46.0f,
		pezZ = -28.0f;

float	sharkXInc = 0.0f,
		sharkYInc = 0.0f,
		sharkZInc = 0.0f,
		sharkBInc = 0.0f,
		sharkCInc = 0.0f,
		sharkDInc = 0.0f,
		sharkIInc = 0.0f,
		pezXInc = 0.0f,
		pezYInc = 0.0f,
		pezZInc = 0.0f;

//delfin
float	delfinX = 31.0f,
		delfinY = 39.0f,
		delfinZ = 159.0f,
		delfinR = 23.0f,
		delfinC = -17.0f,
		delfinAD = 0.0f,
		delfinAI = 0.0f;
float	delfinXInc = 0.0f,
		delfinYInc = 0.0f,
		delfinZInc = 0.0f,
		delfinRInc = 0.0f,
		delfinCInc = 0.0f,
		delfinADInc = 0.0f,
		delfinAIInc = 0.0f;

//cangrejo
float	cangrejoX = 63.0f,
		cangrejoY = 9.0f,
		cangrejoZ = -226.0f,
		cofreX = -19.0f,
		cofreY = -1.0f,
		cofreZ = -60.0f,
		cangrejoR = -93.0f,
		cangrejoR2 = 2.0f,
		cangrejoBD = 19.0f,
		cangrejoBI = -45.0f,
		cangrejoPD = 12.0f,
		cangrejoPI = -17.0f;
float	cangrejoXInc = 0.0f,
		cangrejoYInc = 0.0f,
		cangrejoZInc = 0.0f,
		cofreXInc = 0.0f,
		cofreYInc = 0.0f,
		cofreZInc = 0.0f,
		cangrejoRInc = 0.0f,
		cangrejoR2Inc = 0.0f,
		cangrejoBDInc = 0.0f,
		cangrejoBIInc = 0.0f,
		cangrejoPDInc = 0.0f,
		cangrejoPIInc = 0.0f;

//pulpo
float	pulpoX = -335.0f,
		pulpoY = 19.0f,
		pulpoZ = 0.0f,
		pulpoR = -90.0f,
		ten1 = 16.0f,
		ten2 = -1.0f,
		ten3 = 0.0f,
		ten4 = 0.0f,
		ten5 = 0.0f,
		ten6 = -2.0f,
		ten7 = -4.0f,
		ten8 = 0.0f;
float	pulpoXInc = 0.0f,
		pulpoYInc = 0.0f,
		pulpoZInc = 0.0f,
		pulpoRInc = 0.0f,
		ten1Inc = 0.0f,
		ten2Inc = 0.0f,
		ten3Inc = 0.0f,
		ten4Inc = 0.0f,
		ten5Inc = 0.0f,
		ten6Inc = 0.0f,
		ten7Inc = 0.0f,
		ten8Inc = 0.0f;


//Keyframes (Manipulación y dibujo)


#define MAX_FRAMES 10
int i_max_steps = 60;	//cuadros intermedios
int i_curr_steps = 0;
typedef struct _frame
{
	//Variables para GUARDAR Key Frames
	float sharkX;
	float sharkY;
	float sharkZ;
	float sharkB;
	float sharkC;
	float sharkI;
	float sharkD;
	float pezX;
	float pezY;
	float pezZ;

	float delfinX;
	float delfinY;
	float delfinZ;
	float delfinR;
	float delfinC;
	float delfinAD;
	float delfinAI;

	float cangrejoX;
	float cangrejoY;
	float cangrejoZ;
	float cangrejoR;
	float cangrejoR2;
	float cangrejoBD;
	float cangrejoBI;
	float cangrejoPD;
	float cangrejoPI;
	float cofreX;
	float cofreY;
	float cofreZ;

	float pulpoX;
	float pulpoY;
	float pulpoZ;
	float pulpoR;
	float ten1;
	float ten2;
	float ten3;
	float ten4;
	float ten5;
	float ten6;
	float ten7;
	float ten8;


}FRAME;

FRAME KeyFrame[MAX_FRAMES];
int FrameIndex = 10;			//numero de pasos guardados
bool play = false;
int playIndex = 0; 

void saveFrame(void)
{
	//printf("frameindex %d\n", FrameIndex);
	std::cout << "Frame Index = " << FrameIndex << std::endl;

	//sahrk
	KeyFrame[FrameIndex].sharkX = sharkX;
	KeyFrame[FrameIndex].sharkY = sharkY;
	KeyFrame[FrameIndex].sharkZ = sharkZ;
	KeyFrame[FrameIndex].sharkB = sharkB;
	KeyFrame[FrameIndex].sharkC = sharkC;
	KeyFrame[FrameIndex].sharkD = sharkD;
	KeyFrame[FrameIndex].sharkI = sharkI;
	KeyFrame[FrameIndex].pezX = pezX;
	KeyFrame[FrameIndex].pezY = pezY;
	KeyFrame[FrameIndex].pezZ = pezZ;

	KeyFrame[FrameIndex].delfinX = delfinX;
	KeyFrame[FrameIndex].delfinY = delfinY;
	KeyFrame[FrameIndex].delfinZ = delfinZ;
	KeyFrame[FrameIndex].delfinR = delfinR;
	KeyFrame[FrameIndex].delfinC = delfinC;
	KeyFrame[FrameIndex].delfinAD = delfinAD;
	KeyFrame[FrameIndex].delfinAI = delfinAI;

	KeyFrame[FrameIndex].cangrejoX = cangrejoX;
	KeyFrame[FrameIndex].cangrejoY = cangrejoY;
	KeyFrame[FrameIndex].cangrejoZ = cangrejoZ;
	KeyFrame[FrameIndex].cangrejoR = cangrejoR;
	KeyFrame[FrameIndex].cangrejoR2 = cangrejoR2;
	KeyFrame[FrameIndex].cangrejoBD = cangrejoBD;
	KeyFrame[FrameIndex].cangrejoBI = cangrejoBI;
	KeyFrame[FrameIndex].cangrejoPD = cangrejoPD;
	KeyFrame[FrameIndex].cangrejoPI = cangrejoPI;
	KeyFrame[FrameIndex].cofreX = cofreX;
	KeyFrame[FrameIndex].cofreY = cofreY;
	KeyFrame[FrameIndex].cofreZ = cofreZ;

	KeyFrame[FrameIndex].pulpoX = pulpoX;
	KeyFrame[FrameIndex].pulpoX = pulpoY;
	KeyFrame[FrameIndex].pulpoX = pulpoZ;
	KeyFrame[FrameIndex].pulpoX = pulpoR;
	KeyFrame[FrameIndex].ten1 = ten1;
	KeyFrame[FrameIndex].ten2 = ten2;
	KeyFrame[FrameIndex].ten3 = ten3;
	KeyFrame[FrameIndex].ten4 = ten4;
	KeyFrame[FrameIndex].ten5 = ten5;
	KeyFrame[FrameIndex].ten6 = ten6;
	KeyFrame[FrameIndex].ten7 = ten7;
	KeyFrame[FrameIndex].ten8 = ten8;

	FrameIndex++;
}

void ambientmusic()
{
	if (sound) 
	{
		bool played = PlaySound(L"ambient.wav", NULL, SND_LOOP | SND_ASYNC);
		sound = false;
	}
	
}

void resetElements(void)
{
	//Shark
	sharkX = KeyFrame[0].sharkX;
	sharkY = KeyFrame[0].sharkY;
	sharkZ = KeyFrame[0].sharkZ;
	sharkB = KeyFrame[0].sharkB;
	sharkC = KeyFrame[0].sharkC;
	sharkD = KeyFrame[0].sharkD;
	sharkI = KeyFrame[0].sharkI;
	pezX = KeyFrame[0].pezX;
	pezY = KeyFrame[0].pezY;
	pezZ = KeyFrame[0].pezZ;
	
	//delfin
	delfinX = KeyFrame[0].delfinX;
	delfinY = KeyFrame[0].delfinY;
	delfinZ = KeyFrame[0].delfinZ;
	delfinR = KeyFrame[0].delfinR;
	delfinC = KeyFrame[0].delfinC;
	delfinAD = KeyFrame[0].delfinAD;
	delfinAI = KeyFrame[0].delfinAI;

	//krabs
	cangrejoX = KeyFrame[0].cangrejoX;
	cangrejoY = KeyFrame[0].cangrejoY;
	cangrejoZ = KeyFrame[0].cangrejoZ;
	cangrejoR = KeyFrame[0].cangrejoR;
	cangrejoR2 = KeyFrame[0].cangrejoR2;
	cangrejoBD = KeyFrame[0].cangrejoBD;
	cangrejoBI = KeyFrame[0].cangrejoBI;
	cangrejoPD = KeyFrame[0].cangrejoPD;
	cangrejoPI = KeyFrame[0].cangrejoPI;
	cofreX = KeyFrame[0].cofreX;
	cofreY = KeyFrame[0].cofreY;
	cofreZ = KeyFrame[0].cofreZ;

	//pulpo
	pulpoX = KeyFrame[0].pulpoX;
	pulpoY = KeyFrame[0].pulpoY;
	pulpoZ = KeyFrame[0].pulpoZ;
	pulpoR = KeyFrame[0].pulpoR;
	ten1 = KeyFrame[0].ten1;
	ten2 = KeyFrame[0].ten2;
	ten3 = KeyFrame[0].ten3;
	ten4 = KeyFrame[0].ten4;
	ten5 = KeyFrame[0].ten5;
	ten6 = KeyFrame[0].ten6;
	ten7 = KeyFrame[0].ten7;
	ten8 = KeyFrame[0].ten8;
}

void interpolation(void)
{
	//Shark
	sharkXInc = (KeyFrame[playIndex + 1].sharkX - KeyFrame[playIndex].sharkX) / i_max_steps;
	sharkYInc = (KeyFrame[playIndex + 1].sharkY - KeyFrame[playIndex].sharkY) / i_max_steps;
	sharkZInc = (KeyFrame[playIndex + 1].sharkZ - KeyFrame[playIndex].sharkZ) / i_max_steps;
	sharkBInc = (KeyFrame[playIndex + 1].sharkB - KeyFrame[playIndex].sharkB) / i_max_steps;
	sharkCInc = (KeyFrame[playIndex + 1].sharkC - KeyFrame[playIndex].sharkC) / i_max_steps;
	sharkDInc = (KeyFrame[playIndex + 1].sharkD - KeyFrame[playIndex].sharkD) / i_max_steps;
	sharkIInc = (KeyFrame[playIndex + 1].sharkI - KeyFrame[playIndex].sharkI) / i_max_steps;
	pezXInc = (KeyFrame[playIndex + 1].pezX - KeyFrame[playIndex].pezX) / i_max_steps;
	pezYInc = (KeyFrame[playIndex + 1].pezY - KeyFrame[playIndex].pezY) / i_max_steps;
	pezZInc = (KeyFrame[playIndex + 1].pezZ - KeyFrame[playIndex].pezZ) / i_max_steps;

	delfinXInc = (KeyFrame[playIndex + 1].delfinX - KeyFrame[playIndex].delfinX) / i_max_steps;
	delfinYInc = (KeyFrame[playIndex + 1].delfinY - KeyFrame[playIndex].delfinY) / i_max_steps;
	delfinZInc = (KeyFrame[playIndex + 1].delfinZ - KeyFrame[playIndex].delfinZ) / i_max_steps;
	delfinRInc = (KeyFrame[playIndex + 1].delfinR - KeyFrame[playIndex].delfinR) / i_max_steps;
	delfinCInc = (KeyFrame[playIndex + 1].delfinC - KeyFrame[playIndex].delfinC) / i_max_steps;
	delfinADInc = (KeyFrame[playIndex + 1].delfinAD - KeyFrame[playIndex].delfinAD) / i_max_steps;
	delfinAIInc = (KeyFrame[playIndex + 1].delfinAI - KeyFrame[playIndex].delfinAI) / i_max_steps;

	cangrejoXInc = (KeyFrame[playIndex + 1].cangrejoX - KeyFrame[playIndex].cangrejoX) / i_max_steps;
	cangrejoYInc = (KeyFrame[playIndex + 1].cangrejoY - KeyFrame[playIndex].cangrejoY) / i_max_steps;
	cangrejoZInc = (KeyFrame[playIndex + 1].cangrejoZ - KeyFrame[playIndex].cangrejoZ) / i_max_steps;
	cangrejoRInc = (KeyFrame[playIndex + 1].cangrejoR - KeyFrame[playIndex].cangrejoR) / i_max_steps;
	cangrejoR2Inc = (KeyFrame[playIndex + 1].cangrejoR2 - KeyFrame[playIndex].cangrejoR2) / i_max_steps;
	cangrejoBDInc = (KeyFrame[playIndex + 1].cangrejoBD - KeyFrame[playIndex].cangrejoBD) / i_max_steps;
	cangrejoBIInc = (KeyFrame[playIndex + 1].cangrejoBI - KeyFrame[playIndex].cangrejoBI) / i_max_steps;
	cangrejoPDInc = (KeyFrame[playIndex + 1].cangrejoPD - KeyFrame[playIndex].cangrejoPD) / i_max_steps;
	cangrejoPIInc = (KeyFrame[playIndex + 1].cangrejoPI - KeyFrame[playIndex].cangrejoPI) / i_max_steps;
	cofreXInc = (KeyFrame[playIndex + 1].cofreX - KeyFrame[playIndex].cofreX) / i_max_steps;
	cofreYInc = (KeyFrame[playIndex + 1].cofreY - KeyFrame[playIndex].cofreY) / i_max_steps;
	cofreZInc = (KeyFrame[playIndex + 1].cofreZ - KeyFrame[playIndex].cofreZ) / i_max_steps;

	//pulpo
	pulpoXInc = (KeyFrame[playIndex + 1].pulpoX - KeyFrame[playIndex].pulpoX) / i_max_steps;
	pulpoYInc = (KeyFrame[playIndex + 1].pulpoY - KeyFrame[playIndex].pulpoY) / i_max_steps;
	pulpoZInc = (KeyFrame[playIndex + 1].pulpoZ - KeyFrame[playIndex].pulpoZ) / i_max_steps;
	pulpoRInc = (KeyFrame[playIndex + 1].pulpoR - KeyFrame[playIndex].pulpoR) / i_max_steps;
	ten1Inc = (KeyFrame[playIndex + 1].ten1 - KeyFrame[playIndex].ten1) / i_max_steps;
	ten2Inc = (KeyFrame[playIndex + 1].ten2 - KeyFrame[playIndex].ten2) / i_max_steps;
	ten3Inc = (KeyFrame[playIndex + 1].ten3 - KeyFrame[playIndex].ten3) / i_max_steps;
	ten4Inc = (KeyFrame[playIndex + 1].ten4 - KeyFrame[playIndex].ten4) / i_max_steps;
	ten5Inc = (KeyFrame[playIndex + 1].ten5 - KeyFrame[playIndex].ten5) / i_max_steps;
	ten6Inc = (KeyFrame[playIndex + 1].ten6 - KeyFrame[playIndex].ten6) / i_max_steps;
	ten7Inc = (KeyFrame[playIndex + 1].ten7 - KeyFrame[playIndex].ten7) / i_max_steps;
	ten8Inc = (KeyFrame[playIndex + 1].ten8 - KeyFrame[playIndex].ten8) / i_max_steps;

}

void animate(void)
{
	lightPosition.x = 300.0f * cos(miVariable);		//100
	lightPosition.z = 300.0f * sin(miVariable);		//400
	miVariable += 0.01f;

	// cambio de color
	lightColor.x = miColorR;
	lightColor.y = miColorG;
	lightColor.z = miColorB;
	miColorR += 0.1f;
	miColorG += 0.01f;
	miColorB += 0.001f;
	if (miColorR >= 1.0f)
	{
		miColorR = 0.0f;
	}
	if (miColorG >= 1.0f)
	{
		miColorG = 0.0f;
	}
	if (miColorB >= 1.0f)
	{
		miColorB = 0.0f;
	}
	ambientmusic();
	if (play)
	{

		if (i_curr_steps >= i_max_steps) //end of animation between frames?
		{
			playIndex++;
			if (playIndex > FrameIndex - 2)	//end of total animation?
			{
				std::cout << "Animation ended" << std::endl;
				//printf("termina anim\n");
				playIndex = 0;
				play = false;
			}
			else //Next frame interpolations
			{
				i_curr_steps = 0; //Reset counter
								  //Interpolation
				interpolation();
			}
		}
		else
		{
			//Draw animation
			//shark
			sharkX += sharkXInc;
			sharkY += sharkYInc;
			sharkZ += sharkZInc;
			sharkB += sharkBInc;
			sharkC += sharkCInc;
			sharkD += sharkDInc;
			sharkI += sharkIInc;
			pezX += pezXInc;
			pezY += pezYInc;
			pezZ += pezZInc;

			//delfin
			delfinX += delfinXInc;
			delfinY += delfinYInc;
			delfinZ += delfinZInc;
			delfinR += delfinRInc;
			delfinC += delfinCInc;
			delfinAD += delfinADInc;
			delfinAI += delfinAIInc;

			//krabs
			cangrejoX += cangrejoXInc;
			cangrejoY += cangrejoYInc;
			cangrejoZ += cangrejoZInc;
			cangrejoR += cangrejoRInc;
			cangrejoR2 += cangrejoR2Inc;
			cangrejoBD += cangrejoBDInc;
			cangrejoBI += cangrejoBIInc;
			cangrejoPD += cangrejoPDInc;
			cangrejoPI += cangrejoPIInc;
			cofreX += cofreXInc;
			cofreY += cofreYInc;
			cofreZ += cofreZInc;

			//pulpo
			pulpoX += pulpoXInc;
			pulpoY += pulpoYInc;
			pulpoZ += pulpoZInc;
			pulpoR += pulpoRInc;
			ten1 += ten1Inc;
			ten2 += ten2Inc;
			ten3 += ten3Inc;
			ten4 += ten4Inc;
			ten5 += ten5Inc;
			ten6 += ten6Inc;
			ten7 += ten7Inc;
			ten8 += ten8Inc;

			i_curr_steps++;
		}
	}

	if (estadopez == 0)
	{
		pez1Z += 0.5f;
		pez1R = 0.0f;
		pez3Z += 0.5f;
		pez3R = 0.0f;
		pez2Z -= 0.5f;
		pez2R = 180.0f;
		pez4Z -= 0.5f;
		pez4R = 180.0f;
		if (pez1Z >= 107.0f)
		{
			estadopez = 1;
		}
	}
	
	if (estadopez == 1)
	{
		pez1Z -= 0.5f;
		pez1R = 180.0f;
		pez3Z -= 0.5f;
		pez3R = 180.0f;
		pez2Z += 0.5f;
		pez2R = 0.0f;
		pez4Z += 0.5f;
		pez4R = 0.0f;
		if (pez1Z <= -111.0f )
		{
			estadopez = 0;
		}
	}

	if (estadopez2 == 0)
	{
		pez5Z += 0.5f;
		pez5R = 0.0f;
		pez6Z -= 0.5f;
		pez6R = 180.0f;
		if (pez5Z >= 82.0f)
		{
			estadopez2 = 1;
		}
	}

	if (estadopez2 == 1)
	{
		pez5Z -= 0.5f;
		pez5R = 180.0f;
		pez6Z += 0.5f;
		pez6R = 0.0f;
		if (pez5Z <= 21.0f)
		{
			estadopez2 = 0;
		}
	}
	
	if (estadopez3 == 0)
	{
		pez7X += 0.5f;
		pez7R = -270.0f;
		pez8X += 0.5f;
		pez8R = -270.0f;
		pez9X -= 0.5f;
		pez9R = -90.0f;
		pez10X -= 0.5f;
		pez10R = -90.0f;
		if (pez7X >= 47.0f)
		{
			estadopez3 = 1;
		}
	}

	if (estadopez3 == 1)
	{
		pez7X -= 0.5f;
		pez7R = -90.0f;
		pez8X -= 0.5f;
		pez8R = -90.0f;
		pez9X += 0.5f;
		pez9R = -270.0f;
		pez10X += 0.5f;
		pez10R = -270.0f;
		if (pez7X <= -66.0f)
		{
			estadopez3 = 0;
		}
	}

	if (estadoTortuga == 0)
	{
		torX -= 0.0f;
		torY += 0.0f;
		torZ += 0.5f;
		torR = 0.0f;
		if (torZ >= 79.0f)
		{
			estadoTortuga = 1;
		}
	}
	if (estadoTortuga == 1)
	{
		torX -= 0.3f; //35
		torY -= 0.245f;	//30
		torZ -= 0.5f; // 58
		torR = -135.0f;
		if (torZ <= 16.0f)
		{
			estadoTortuga = 2;
		}
	}
	if (estadoTortuga == 2)
	{
		torX -= 0.0f; //35
		torY -= 0.0f;	//30
		torZ += 0.125f; // 58
		torR = 0.0f;
		if (torZ >= 62.0f)
		{
			estadoTortuga = 3;
		}
	}
	if (estadoTortuga == 3)
	{
		torX -= 0.0f; //35
		torY -= 0.0f;	//30
		torZ += 0.125f; // 58
		torR = 0.0f;
		if (torZ >= 80.0f)
		{
			estadoTortuga = 4;
		}
	}
	if (estadoTortuga == 4)
	{
		torX += 0.335f; //37
		torY += 0.27f;	//30
		torZ -= 0.5f; // 55
		torR = -225.0f;
		if (torZ <= 25.0f)
		{
			estadoTortuga = 0;
		}
	}
}

void getResolution()
{
	const GLFWvidmode * mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

	SCR_WIDTH = mode->width;
	SCR_HEIGHT = (mode->height) - 80;
}


int main()
{
	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	/*glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);*/

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// glfw window creation
	// --------------------
	// --------------------
	monitors = glfwGetPrimaryMonitor();
	getResolution();

	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "CGeIHC", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwSetWindowPos(window, 0, 30);
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetKeyCallback(window, my_input);

	// tell GLFW to capture our mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// configure global opengl state
	// -----------------------------
	glEnable(GL_DEPTH_TEST);

	// build and compile shaders
	// -------------------------
	Shader staticShader("Shaders/shader_Lights.vs", "Shaders/shader_Lights_mod.fs");
	Shader skyboxShader("Shaders/skybox.vs", "Shaders/skybox.fs");
	Shader animShader("Shaders/anim.vs", "Shaders/anim.fs");

	vector<std::string> faces
	{
		"resources/skybox/right.jpg",
		"resources/skybox/left.jpg",
		"resources/skybox/top.jpg",
		"resources/skybox/bottom.jpg",
		"resources/skybox/front.jpg",
		"resources/skybox/back.jpg"
	};

	Skybox skybox = Skybox(faces);

	// Shader configuration
	// --------------------
	skyboxShader.use();
	skyboxShader.setInt("skybox", 0);

	// load models
	// -----------
	Model piso("resources/objects/objects/piso/piso.obj");
	Model casaC("resources/objects/ObjetosA/CasaCalamar/CasaC.obj");
	Model arbol1("resources/objects/ObjetosA/Arboles/Arboles/Arbol1.obj");
	Model arbol2("resources/objects/ObjetosA/Arboles/Arboles/Arbol2.obj");
	Model arbol3("resources/objects/ObjetosA/Arboles/Arboles/Arbol3.obj");
	Model arbol4("resources/objects/ObjetosA/Arboles/Arboles/Arbol4.obj");
	Model arbol5("resources/objects/ObjetosA/Arboles/Arboles/Arbol5.obj");
	Model pecera("resources/objects/ObjetosA/Pecera/peceraOK.obj");
	Model peceraT("resources/objects/ObjetosA/Pecera/peceraT.obj");
	Model peceraT2("resources/objects/ObjetosA/Pecera/peceraTiburon.obj");
	
	//shark
	Model sharkT("resources/objects/ObjetosA/Shark/torsoS.obj");
	Model sharkBoca("resources/objects/ObjetosA/Shark/bocaS.obj");
	Model sharkCola("resources/objects/ObjetosA/Shark/colaS.obj");
	Model sharkAD("resources/objects/ObjetosA/Shark/aletaD.obj");
	Model sharkAI("resources/objects/ObjetosA/Shark/aletaI.obj");

	//delfin
	Model delfinT("resources/objects/ObjetosA/Delfin/Torso_delfin.obj");
	Model delfinD("resources/objects/ObjetosA/Delfin/AletaD_delfin.obj");
	Model delfinI("resources/objects/ObjetosA/Delfin/AletaI_delfin.obj");
	Model delfinCo("resources/objects/ObjetosA/Delfin/Cola_delfin.obj");

	//krabs
	Model cangrejoT("resources/objects/ObjetosA/Krabs/Torso.obj");
	Model cangrejoD("resources/objects/ObjetosA/Krabs/PataD.obj");
	Model cangrejoI("resources/objects/ObjetosA/Krabs/PataI.obj");
	Model cangrejoMD("resources/objects/ObjetosA/Krabs/ManoD.obj");
	Model cangrejoMI("resources/objects/ObjetosA/Krabs/ManoI.obj");
	Model cofre("resources/objects/ObjetosA/Krabs/Chest.obj");

	//pulpo
	Model octoT("resources/objects/ObjetosA/pulpo/Torso.obj");
	Model octoT1("resources/objects/ObjetosA/pulpo/tentaculo1.obj");
	Model octoT2("resources/objects/ObjetosA/pulpo/tentaculo2.obj");
	Model octoT3("resources/objects/ObjetosA/pulpo/tentaculo3.obj");
	Model octoT4("resources/objects/ObjetosA/pulpo/tentaculo4.obj");
	Model octoT5("resources/objects/ObjetosA/pulpo/tentaculo5.obj");
	Model octoT6("resources/objects/ObjetosA/pulpo/tentaculo6.obj");
	Model octoT7("resources/objects/ObjetosA/pulpo/tentaculo7.obj");
	Model octoT8("resources/objects/ObjetosA/pulpo/tentaculo8.obj");

	ModelAnim pez1("resources/objects/ObjetosA/fish/Pez1/the_fish.fbx");
	pez1.initShaders(animShader.ID);

	ModelAnim pez2("resources/objects/ObjetosA/fish/Pez2/the_fish.fbx");
	pez2.initShaders(animShader.ID);

	ModelAnim pez3("resources/objects/ObjetosA/fish/Pez3/the_fish.fbx");
	pez3.initShaders(animShader.ID);

	ModelAnim pez4("resources/objects/ObjetosA/fish/Pez4/the_fish.fbx");
	pez4.initShaders(animShader.ID);
		
	ModelAnim tortuga("resources/objects/ObjetosA/Tortuga/Turtle.fbx");
	tortuga.initShaders(animShader.ID);

	//leer archivo
	string sharkfps = "FPS/shark.txt";
	ifstream archivo(sharkfps.c_str());
	string sharkV;
	
	//Inicialización de KeyFrames
	for (int i = 0; i < MAX_FRAMES; i++)
	{
		getline(archivo, sharkV);
		float pzx = std::stof(sharkV);
		getline(archivo, sharkV);
		float pzy = std::stof(sharkV);
		getline(archivo, sharkV);
		float pzz = std::stof(sharkV);
		getline(archivo, sharkV);		
		float shx = std::stof(sharkV);
		getline(archivo, sharkV);
		float shy = std::stof(sharkV);
		getline(archivo, sharkV);
		float shz = std::stof(sharkV);
		getline(archivo, sharkV);
		float shb = std::stof(sharkV);
		getline(archivo, sharkV);
		float shc = std::stof(sharkV);
		getline(archivo, sharkV);
		float shi = std::stof(sharkV);
		getline(archivo, sharkV);
		float shd = std::stof(sharkV);

		KeyFrame[i].pezX = pzx;
		KeyFrame[i].pezY = pzy;
		KeyFrame[i].pezZ = pzz;
		KeyFrame[i].sharkX = shx;
		KeyFrame[i].sharkY = shy;
		KeyFrame[i].sharkZ = shz;
		KeyFrame[i].sharkB = shb;
		KeyFrame[i].sharkC = shc;
		KeyFrame[i].sharkI = shi;
		KeyFrame[i].sharkD = shd;
	}
	//Delfin
	string delfinfps = "FPS/delfin.txt";
	ifstream archivo2(delfinfps.c_str());
	string delfinV;

	//Inicialización de KeyFrames
	for (int j = 0; j < MAX_FRAMES; j++)
	{
		getline(archivo2, delfinV);
		float dex = std::stof(delfinV);
		getline(archivo2, delfinV);
		float dey = std::stof(delfinV);
		getline(archivo2, delfinV);
		float dez = std::stof(delfinV);
		getline(archivo2, delfinV);
		float der = std::stof(delfinV);
		getline(archivo2, delfinV);
		float dec = std::stof(delfinV);
		getline(archivo2, delfinV);
		float ded = std::stof(delfinV);
		getline(archivo2, delfinV);
		float dei = std::stof(delfinV);

		KeyFrame[j].delfinX = dex;
		KeyFrame[j].delfinY = dey;
		KeyFrame[j].delfinZ = dez;
		KeyFrame[j].delfinR = der;
		KeyFrame[j].delfinC = dec;
		KeyFrame[j].delfinAD = ded;
		KeyFrame[j].delfinAI = dei;
	}

	//krabs
	string krabsfps = "FPS/krabs.txt";
	ifstream archivo3(krabsfps.c_str());
	string krabsV;

	//Inicialización de KeyFrames
	for (int k = 0; k < MAX_FRAMES; k++)
	{
		getline(archivo3, krabsV);
		float kax = std::stof(krabsV);
		getline(archivo3, krabsV);
		float kay = std::stof(krabsV);
		getline(archivo3, krabsV);
		float kaz = std::stof(krabsV);
		getline(archivo3, krabsV);
		float kar = std::stof(krabsV);
		getline(archivo3, krabsV);
		float kar2 = std::stof(krabsV);
		getline(archivo3, krabsV);
		float kabd = std::stof(krabsV);
		getline(archivo3, krabsV);
		float kabi = std::stof(krabsV);
		getline(archivo3, krabsV);
		float kapd = std::stof(krabsV);
		getline(archivo3, krabsV);
		float kapi = std::stof(krabsV);
		getline(archivo3, krabsV);
		float cox = std::stof(krabsV);
		getline(archivo3, krabsV);
		float coy = std::stof(krabsV);
		getline(archivo3, krabsV);
		float coz = std::stof(krabsV);


		KeyFrame[k].cangrejoX = kax;
		KeyFrame[k].cangrejoY = kay;
		KeyFrame[k].cangrejoZ = kaz;
		KeyFrame[k].cangrejoR = kar;
		KeyFrame[k].cangrejoR2 = kar2;
		KeyFrame[k].cangrejoBD = kabd;
		KeyFrame[k].cangrejoBI = kabi;
		KeyFrame[k].cangrejoPD = kapd;
		KeyFrame[k].cangrejoPI = kapi;
		KeyFrame[k].cofreX = cox;
		KeyFrame[k].cofreY = coy;
		KeyFrame[k].cofreZ = coz;
	}

	//pulpo
	string octopusfps = "FPS/octopus.txt";
	ifstream archivo4(octopusfps.c_str());
	string octopusV;

	//Inicialización de KeyFrames
	for (int l = 0; l < MAX_FRAMES; l++)
	{
		getline(archivo4, octopusV);
		float pux = std::stof(octopusV);
		getline(archivo4, octopusV);
		float puy = std::stof(octopusV);
		getline(archivo4, octopusV);
		float puz = std::stof(octopusV);
		getline(archivo4, octopusV);
		float pur = std::stof(octopusV);
		getline(archivo4, octopusV);
		float te1 = std::stof(octopusV);
		getline(archivo4, octopusV);
		float te2 = std::stof(octopusV);
		getline(archivo4, octopusV);
		float te3 = std::stof(octopusV);
		getline(archivo4, octopusV);
		float te4 = std::stof(octopusV);
		getline(archivo4, octopusV);
		float te5 = std::stof(octopusV);
		getline(archivo4, octopusV);
		float te6 = std::stof(octopusV);
		getline(archivo4, octopusV);
		float te7 = std::stof(octopusV);
		getline(archivo4, octopusV);
		float te8 = std::stof(octopusV);


		KeyFrame[l].pulpoX = pux;
		KeyFrame[l].pulpoY = puy;
		KeyFrame[l].pulpoZ = puz;
		KeyFrame[l].pulpoR = pur;
		KeyFrame[l].ten1 = te1;
		KeyFrame[l].ten2 = te2;
		KeyFrame[l].ten3 = te3;
		KeyFrame[l].ten4 = te4;
		KeyFrame[l].ten5 = te5;
		KeyFrame[l].ten6 = te6;
		KeyFrame[l].ten7 = te7;
		KeyFrame[l].ten8 = te8;
	}
	// draw in wireframe
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// render loop
	// -----------
	while (!glfwWindowShouldClose(window))
	{
		skyboxShader.setInt("skybox", 0);
		
		// per-frame time logic
		// --------------------
		lastFrame = SDL_GetTicks();

		// input
		// -----
		//my_input(window);
		animate();
		ambientmusic();
		// render
		// ------
		glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// don't forget to enable shader before setting uniforms
		staticShader.use();
		//Setup Advanced Lights
		staticShader.setVec3("viewPos", camera.Position);
		staticShader.setVec3("dirLight.direction", lightDirection);		// dirLight
		staticShader.setVec3("dirLight.ambient", glm::vec3(0.5f, 0.5f, 0.5f)); //ambiental sombras / brillo minimo
		staticShader.setVec3("dirLight.diffuse", glm::vec3(1.0f, 1.0f, 1.0f));	//difusa luz/ brillo maximo	
		staticShader.setVec3("dirLight.specular", glm::vec3(0.0f, 0.0f, 0.0f)); // reflrejo 

		staticShader.setVec3("pointLight[0].position", lightPosition);
		staticShader.setVec3("pointLight[0].ambient", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setVec3("pointLight[0].diffuse", glm::vec3(0.0f, 0.0f, 0.0f));//lightColor);
		staticShader.setVec3("pointLight[0].specular", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setFloat("pointLight[0].constant", 0.00008f);  // rango donde afecta con menor intensidad **es inverso
		staticShader.setFloat("pointLight[0].linear", 0.0009f);	// rango del centro iluminado  ** es inverso
		staticShader.setFloat("pointLight[0].quadratic", 0.00032f);	//potencia de la fuente de luz

		staticShader.setVec3("pointLight[1].position", glm::vec3(80.0, 0.0f, 0.0f));
		staticShader.setVec3("pointLight[1].ambient", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setVec3("pointLight[1].diffuse", glm::vec3(0.0f, 0.0f, 0.0f)); //Encendidox, Encendidoy, Encendidoz));
		staticShader.setVec3("pointLight[1].specular", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setFloat("pointLight[1].constant", 1.0f);
		staticShader.setFloat("pointLight[1].linear", 0.000009f);
		staticShader.setFloat("pointLight[1].quadratic", 0.000032f);

		staticShader.setVec3("spotLight[0].position", glm::vec3(0.0f, 20.0f, 0.0f));   //posicion
															//(camera.Position.x, camera.Position.y, camera.Position.z) la luz es la camara
		staticShader.setVec3("spotLight[0].direction", glm::vec3(0.0f, -1.0f, -1.0f));	//hacia donde ilumina
															//camera.Front.x, camera.Front.y, camera.Front.z) apunta hacia el frente de la camara
		staticShader.setVec3("spotLight[0].ambient", glm::vec3(0.0f, 0.0f, 0.0f));		//sombras
		staticShader.setVec3("spotLight[0].diffuse", glm::vec3(0.0f, 0.0f, 0.0f));		//brillo
		staticShader.setVec3("spotLight[0].specular", glm::vec3(0.0f, 0.0f, 0.0f));		//reflejos
		staticShader.setFloat("spotLight[0].cutOff", glm::cos(glm::radians(30.0f)));	// bien iluminado 
		staticShader.setFloat("spotLight[0].outerCutOff", glm::cos(glm::radians(60.0f)));	//iluminado difuso
		staticShader.setFloat("spotLight[0].constant", 1.0f);
		staticShader.setFloat("spotLight[0].linear", 0.0009f);
		staticShader.setFloat("spotLight[0].quadratic", 0.00005f);

		staticShader.setFloat("material_shininess", 32.0f);

		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 dolphin = glm::mat4(1.0f);
		glm::mat4 krabs = glm::mat4(1.0f);
		glm::mat4 chest = glm::mat4(1.0f);
		glm::mat4 pulpo = glm::mat4(1.0f);
		glm::mat4 tmp = glm::mat4(1.0f);
		glm::mat4 tmp2 = glm::mat4(1.0f);
		glm::mat4 tmp3 = glm::mat4(1.0f);
		glm::mat4 tmp4 = glm::mat4(1.0f);
		// view/projection transformations
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 10000.0f);
		glm::mat4 view = camera.GetViewMatrix();
		staticShader.setMat4("projection", projection);
		staticShader.setMat4("view", view);

		//// Light
		glm::vec3 lightColor = glm::vec3(0.6f);
		glm::vec3 diffuseColor = lightColor * glm::vec3(0.5f);
		glm::vec3 ambientColor = diffuseColor * glm::vec3(0.75f);
		

		// -------------------------------------------------------------------------------------------------------------------------
		// Personaje Animacion
		// -------------------------------------------------------------------------------------------------------------------------
		//Remember to activate the shader with the animation
		animShader.use();
		animShader.setMat4("projection", projection);
		animShader.setMat4("view", view);
	
		animShader.setVec3("material.specular", glm::vec3(0.5f));
		animShader.setFloat("material.shininess", 32.0f);
		animShader.setVec3("light.ambient", ambientColor);
		animShader.setVec3("light.diffuse", diffuseColor);
		animShader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);
		animShader.setVec3("light.direction", lightDirection);
		animShader.setVec3("viewPos", camera.Position);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(pezX, pezY, pezZ)); // translate it down so it's at the center of the scene
		model = glm::scale(model, glm::vec3(1.0f));	// it's a bit too big for our scene, so scale it down
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		animShader.setMat4("model", model);
		pez1.Draw(animShader);

		// -------------------------------------------------------------------------------------------------------------------------
		// Segundo Personaje Animacion
		// -------------------------------------------------------------------------------------------------------------------------

		model = glm::translate(glm::mat4(1.0f), glm::vec3(pez1X, pez1Y, pez1Z)); // translate it down so it's at the center of the scene
		model = glm::scale(model, glm::vec3(1.0f));	// it's a bit too big for our scene, so scale it down
		model = glm::rotate(model, glm::radians(pez1R), glm::vec3(0.0f, 1.0f, 0.0f));
		animShader.setMat4("model", model);
		pez1.Draw(animShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(pez2X, pez2Y, pez2Z)); // translate it down so it's at the center of the scene
		model = glm::scale(model, glm::vec3(1.0f));	// it's a bit too big for our scene, so scale it down
		model = glm::rotate(model, glm::radians(pez2R), glm::vec3(0.0f, 1.0f, 0.0f));
		animShader.setMat4("model", model);
		pez2.Draw(animShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(pez3X, pez3Y, pez3Z)); // translate it down so it's at the center of the scene
		model = glm::scale(model, glm::vec3(1.0f));	// it's a bit too big for our scene, so scale it down
		model = glm::rotate(model, glm::radians(pez3R), glm::vec3(0.0f, 1.0f, 0.0f));
		animShader.setMat4("model", model);
		pez3.Draw(animShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(pez4X, pez4Y, pez4Z)); // translate it down so it's at the center of the scene
		model = glm::scale(model, glm::vec3(1.0f));	// it's a bit too big for our scene, so scale it down
		model = glm::rotate(model, glm::radians(pez4R), glm::vec3(0.0f, 1.0f, 0.0f));
		animShader.setMat4("model", model);
		pez4.Draw(animShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(pez5X, pez5Y, pez5Z)); // translate it down so it's at the center of the scene
		model = glm::scale(model, glm::vec3(1.0f));	// it's a bit too big for our scene, so scale it down
		model = glm::rotate(model, glm::radians(pez5R), glm::vec3(0.0f, 1.0f, 0.0f));
		animShader.setMat4("model", model);
		pez4.Draw(animShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(pez6X, pez6Y, pez6Z)); // translate it down so it's at the center of the scene
		model = glm::scale(model, glm::vec3(1.0f));	// it's a bit too big for our scene, so scale it down
		model = glm::rotate(model, glm::radians(pez6R), glm::vec3(0.0f, 1.0f, 0.0f));
		animShader.setMat4("model", model);
		pez3.Draw(animShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(pez7X, pez7Y, pez7Z)); // translate it down so it's at the center of the scene
		model = glm::scale(model, glm::vec3(1.0f));	// it's a bit too big for our scene, so scale it down
		model = glm::rotate(model, glm::radians(pez7R), glm::vec3(0.0f, 1.0f, 0.0f));
		animShader.setMat4("model", model);
		pez2.Draw(animShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(pez8X, pez8Y, pez8Z)); // translate it down so it's at the center of the scene
		model = glm::scale(model, glm::vec3(1.0f));	// it's a bit too big for our scene, so scale it down
		model = glm::rotate(model, glm::radians(pez8R), glm::vec3(0.0f, 1.0f, 0.0f));
		animShader.setMat4("model", model);
		pez1.Draw(animShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(pez9X, pez9Y, pez9Z)); // translate it down so it's at the center of the scene
		model = glm::scale(model, glm::vec3(1.0f));	// it's a bit too big for our scene, so scale it down
		model = glm::rotate(model, glm::radians(pez9R), glm::vec3(0.0f, 1.0f, 0.0f));
		animShader.setMat4("model", model);
		pez1.Draw(animShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(pez10X, pez10Y, pez10Z)); // translate it down so it's at the center of the scene
		model = glm::scale(model, glm::vec3(1.0f));	// it's a bit too big for our scene, so scale it down
		model = glm::rotate(model, glm::radians(pez10R), glm::vec3(0.0f, 1.0f, 0.0f));
		animShader.setMat4("model", model);
		pez3.Draw(animShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(torX, torY, torZ)); // translate it down so it's at the center of the scene
		model = glm::scale(model, glm::vec3(0.05f));	// it's a bit too big for our scene, so scale it down
		model = glm::rotate(model, glm::radians(torR), glm::vec3(0.0f, 1.0f, 0.0f));
		animShader.setMat4("model", model);
		tortuga.Draw(animShader);
		// -------------------------------------------------------------------------------------------------------------------------
		// Escenario
		// -------------------------------------------------------------------------------------------------------------------------
		staticShader.use();
		staticShader.setMat4("projection", projection);
		staticShader.setMat4("view", view);


		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, -1.75f, 0.0f));
		model = glm::scale(model, glm::vec3(0.2f));
		staticShader.setMat4("model", model);
		piso.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.2f));
		staticShader.setMat4("model", model);
		casaC.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.2f));
		staticShader.setMat4("model", model);
		arbol1.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.2f));
		staticShader.setMat4("model", model);
		arbol2.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.2f));
		staticShader.setMat4("model", model);
		arbol3.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.2f));
		staticShader.setMat4("model", model);
		arbol4.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.2f));
		staticShader.setMat4("model", model);
		arbol5.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.2f));
		staticShader.setMat4("model", model);
		pecera.Draw(staticShader);
			
		model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.2f));
		staticShader.setMat4("model", model);
		peceraT.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.2f));
		staticShader.setMat4("model", model);
		peceraT2.Draw(staticShader);
		// -------------------------------------------------------------------------------------------------------------------------
		// Shark
		// -------------------------------------------------------------------------------------------------------------------------
		model = glm::translate(model, glm::vec3(sharkX, sharkY, sharkZ));
		tmp = model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.8f));
		staticShader.setMat4("model", model);
		sharkT.Draw(staticShader);

		model = glm::translate(tmp, glm::vec3(0.0f, -26.0f, 69.0f));
		model = glm::rotate(model, glm::radians(sharkB), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.8f));
		staticShader.setMat4("model", model);
		sharkBoca.Draw(staticShader);

		model = glm::translate(tmp, glm::vec3(-1.0f, 2.0f, -96.0f));
		model = glm::rotate(model, glm::radians(sharkC), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.8f));
		staticShader.setMat4("model", model);
		sharkCola.Draw(staticShader);

		model = glm::translate(tmp, glm::vec3(-36.0f, -28.0f, 9.0f));
		model = glm::rotate(model, glm::radians(sharkD), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.8f));
		staticShader.setMat4("model", model);
		sharkAD.Draw(staticShader);

		model = glm::translate(tmp, glm::vec3(36.0f, -28.0f, 9.0f));
		model = glm::rotate(model, glm::radians(sharkI), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.8f));
		staticShader.setMat4("model", model);
		sharkAI.Draw(staticShader);

		// -------------------------------------------------------------------------------------------------------------------------
		// Delfin
		// -------------------------------------------------------------------------------------------------------------------------
		dolphin = glm::translate(dolphin, glm::vec3(delfinX, delfinY, delfinZ));
		tmp2 = dolphin = glm::rotate(dolphin, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		tmp2 = dolphin = glm::rotate(dolphin, glm::radians(delfinR), glm::vec3(1.0f, 0.0f, 0.0f));
		dolphin = glm::scale(dolphin, glm::vec3(0.5));
		staticShader.setMat4("model", dolphin);
		delfinT.Draw(staticShader);

		dolphin = glm::translate(tmp2, glm::vec3(0.0f, -1.166f, -14.0f));
		dolphin = glm::rotate(dolphin, glm::radians(182.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		dolphin = glm::rotate(dolphin, glm::radians(delfinC), glm::vec3(1.0f, 0.0f, 0.0f));
		dolphin = glm::scale(dolphin, glm::vec3(0.5f));
		staticShader.setMat4("model", dolphin);
		delfinCo.Draw(staticShader);

		dolphin = glm::translate(tmp2, glm::vec3(-2.832f, -4.166f, 4.666f));
		dolphin = glm::rotate(dolphin, glm::radians(delfinAD), glm::vec3(0.0f, 0.0f, 1.0f));
		dolphin = glm::scale(dolphin, glm::vec3(0.5f));
		staticShader.setMat4("model", dolphin);
		delfinD.Draw(staticShader);

		dolphin = glm::translate(tmp2, glm::vec3(2.832f, -4.166f, 5.666f));
		dolphin = glm::rotate(dolphin, glm::radians(delfinAI), glm::vec3(0.0f, 0.0f, 1.0f));
		dolphin = glm::scale(dolphin, glm::vec3(0.5f));
		staticShader.setMat4("model", dolphin);
		delfinI.Draw(staticShader);


		// -------------------------------------------------------------------------------------------------------------------------
		// Mr Krabs
		// -------------------------------------------------------------------------------------------------------------------------
		krabs = glm::translate(krabs, glm::vec3(cangrejoX, cangrejoY, cangrejoZ));
		tmp3 = krabs = glm::rotate(krabs, glm::radians(cangrejoR), glm::vec3(0.0f, 1.0f, 0.0f));
		tmp3 = krabs = glm::rotate(krabs, glm::radians(cangrejoR2), glm::vec3(1.0f, 0.0f, 0.0f));
		krabs = glm::scale(krabs, glm::vec3(0.15f));
		staticShader.setMat4("model", krabs);
		cangrejoT.Draw(staticShader);

		krabs = glm::translate(tmp3, glm::vec3(-4.5f, 2.25f, -0.75f));
		krabs = glm::rotate(krabs, glm::radians(cangrejoBD), glm::vec3(1.0f, 0.0f, 0.0f));
		krabs = glm::scale(krabs, glm::vec3(0.15f));
		staticShader.setMat4("model", krabs);
		cangrejoMD.Draw(staticShader);

		krabs = glm::translate(tmp3, glm::vec3(4.5f, 2.25f, -0.75f));
		krabs = glm::rotate(krabs, glm::radians(cangrejoBI), glm::vec3(1.0f, 0.0f, 0.0f));
		krabs = glm::scale(krabs, glm::vec3(0.15f));
		staticShader.setMat4("model", krabs);
		cangrejoMI.Draw(staticShader);

		krabs = glm::translate(tmp3, glm::vec3(-3.0f, -6.75f, 0.75f));
		krabs = glm::rotate(krabs, glm::radians(cangrejoPD), glm::vec3(1.0f, 0.0f, 0.0f));
		krabs = glm::scale(krabs, glm::vec3(0.15f));
		staticShader.setMat4("model", krabs);
		cangrejoD.Draw(staticShader);

		krabs = glm::translate(tmp3, glm::vec3(3.0f, -6.75f, 0.75f));
		krabs = glm::rotate(krabs, glm::radians(cangrejoPI), glm::vec3(1.0f, 0.0f, 0.0f));
		krabs = glm::scale(krabs, glm::vec3(0.15f));
		staticShader.setMat4("model", krabs);
		cangrejoI.Draw(staticShader);

		chest = glm::translate(chest, glm::vec3(cofreX, cofreY, cofreZ));
		chest = glm::rotate(chest, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		chest = glm::scale(chest, glm::vec3(0.2f));
		staticShader.setMat4("model", chest);
		cofre.Draw(staticShader);

		// -------------------------------------------------------------------------------------------------------------------------
		// Octopus
		// -------------------------------------------------------------------------------------------------------------------------

		pulpo = glm::translate(pulpo, glm::vec3(pulpoX, pulpoY, pulpoZ));
		tmp4 = pulpo = glm::rotate(pulpo, glm::radians(pulpoR), glm::vec3(0.0f, 1.0f, 0.0f));
		pulpo = glm::scale(pulpo, glm::vec3(0.2f));
		staticShader.setMat4("model", pulpo);
		octoT.Draw(staticShader);

		pulpo = glm::translate(tmp4, glm::vec3(-15.0f,-10.0f,28.0f));
		pulpo = glm::rotate(pulpo, glm::radians(ten1), glm::vec3(0.0f, 1.0f, 0.0f));
		pulpo = glm::scale(pulpo, glm::vec3(0.2f));
		staticShader.setMat4("model", pulpo);
		octoT1.Draw(staticShader);

		pulpo = glm::translate(tmp4, glm::vec3(-24.0f, -16.0f, 18.0f));
		pulpo = glm::rotate(pulpo, glm::radians(ten2), glm::vec3(0.0f, 1.0f, 0.0f));
		pulpo = glm::scale(pulpo, glm::vec3(0.2f));
		staticShader.setMat4("model", pulpo);
		octoT2.Draw(staticShader);

		pulpo = glm::translate(tmp4, glm::vec3(-24.0f,-13.0f,0.0f));
		pulpo = glm::rotate(pulpo, glm::radians(ten3), glm::vec3(0.0f, 1.0f, 0.0f));
		pulpo = glm::scale(pulpo, glm::vec3(0.2f));
		staticShader.setMat4("model", pulpo);
		octoT3.Draw(staticShader);

		pulpo = glm::translate(tmp4, glm::vec3(-10.0f,-11.0f,-16.0f));
		pulpo = glm::rotate(pulpo, glm::radians(ten4), glm::vec3(0.0f, 1.0f, 0.0f));
		pulpo = glm::scale(pulpo, glm::vec3(0.2f));
		staticShader.setMat4("model", pulpo);
		octoT4.Draw(staticShader);

		pulpo = glm::translate(tmp4, glm::vec3(10.0f,-10.0f,-16.0f));
		pulpo = glm::rotate(pulpo, glm::radians(ten5), glm::vec3(0.0f, 1.0f, 0.0f));
		pulpo = glm::scale(pulpo, glm::vec3(0.2f));
		staticShader.setMat4("model", pulpo);
		octoT5.Draw(staticShader);

		pulpo = glm::translate(tmp4, glm::vec3(26.0f,-15.0f,0.0f));
		pulpo = glm::rotate(pulpo, glm::radians(ten6), glm::vec3(0.0f, 1.0f, 0.0f));
		pulpo = glm::scale(pulpo, glm::vec3(0.2f));
		staticShader.setMat4("model", pulpo);
		octoT6.Draw(staticShader);

		pulpo = glm::translate(tmp4, glm::vec3(24.0f,-17.0f,20.0f));
		pulpo = glm::rotate(pulpo, glm::radians(ten7), glm::vec3(0.0f, 1.0f, 0.0f));
		pulpo = glm::scale(pulpo, glm::vec3(0.2f));
		staticShader.setMat4("model", pulpo);
		octoT7.Draw(staticShader);

		pulpo = glm::translate(tmp4, glm::vec3(16.0f,-10.0f,29.0f));
		pulpo = glm::rotate(pulpo, glm::radians(ten8), glm::vec3(0.0f, 1.0f, 0.0f));
		pulpo = glm::scale(pulpo, glm::vec3(0.2f));
		staticShader.setMat4("model", pulpo);
		octoT8.Draw(staticShader);

		// -------------------------------------------------------------------------------------------------------------------------
		// Termina Escenario
		// -------------------------------------------------------------------------------------------------------------------------

		//-------------------------------------------------------------------------------------
		// draw skybox as last
		// -------------------
		skyboxShader.use();
		skybox.Draw(skyboxShader, view, projection, camera);

		// Limitar el framerate a 60
		deltaTime = SDL_GetTicks() - lastFrame; // time for full 1 loop
		//std::cout <<"frame time = " << frameTime << " milli sec"<< std::endl;
		if (deltaTime < LOOP_TIME)
		{
			SDL_Delay((int)(LOOP_TIME - deltaTime));
		}

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	skybox.Terminate();

	glfwTerminate();
	return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void my_input(GLFWwindow *window, int key, int scancode, int action, int mode)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, (float)deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, (float)deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, (float)deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, (float)deltaTime);
	//To Configure Model


	//if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS)
	//	pulpoZ++;//posZ++;
	//if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS)
	//	cangrejoZ--;//posZ--;
	//if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS)
	//	pulpoX--;//posX--;
	//if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
	//	pulpoX++;//posX++;
	//if (glfwGetKey(window, GLFW_KEY_F1) == GLFW_PRESS)
	//	pulpoY++;//posY += 2;
	//if (glfwGetKey(window, GLFW_KEY_F2) == GLFW_PRESS)
	//	pulpoY--;//posY -= 2;
	//if (glfwGetKey(window, GLFW_KEY_F3) == GLFW_PRESS)
	//	pulpoR++;//rotR += 2;
	//if (glfwGetKey(window, GLFW_KEY_F4) == GLFW_PRESS)
	//	pulpoR--;//rotR -= 2;
	//if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
	//	ten1++;//rotR += 2;
	//if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
	//	ten1--;//rotR -= 2;
	//if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
	//	ten2++;//rotR += 2;
	//if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS)
	//	ten2--;//rotR -= 2;
	//if (glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS)
	//	ten3++;//rotR += 2;
	//if (glfwGetKey(window, GLFW_KEY_6) == GLFW_PRESS)
	//	ten3--;//rotR -= 2;
	//if (glfwGetKey(window, GLFW_KEY_7) == GLFW_PRESS)
	//	ten4++;//rotR += 2;
	//if (glfwGetKey(window, GLFW_KEY_8) == GLFW_PRESS)
	//	ten4--;//rotR -= 2;
	//if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
	//	ten5++;//rotR += 2;
	//if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
	//	ten5--;//rotR -= 2;

	//if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
	//	ten6++;
	//if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
	//	ten6--;
	//if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)
	//	ten7 ++;
	//if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS)
	//	ten7 --;
	//if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS)
	//	ten8 ++;
	//if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS)
	//	ten8 --;
	//if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS)
	//	rotr++;
	//if (glfwGetKey(window, GLFW_KEY_COMMA) == GLFW_PRESS)
	//	rotr--;

	/*if (glfwGetKey(window, GLFW_KEY_0) == GLFW_PRESS)
	{
		cout << pulpoX << endl;
		cout << pulpoY << endl;
		cout << pulpoZ << endl;
		cout << pulpoR << endl;
		cout << ten1 << endl;
		cout << ten2 << endl;
		cout << ten3 << endl;
		cout << ten4 << endl;
		cout << ten5 << endl;
		cout << ten6 << endl;
		cout << ten7 << endl;
		cout << ten8 << endl;
		cout << posx << endl;
		cout << posy << endl;
		cout << posz << endl;
		cout << rotr << endl;
	}
	if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
	{
		estado = 99;
	}*/

	//Car animation
	if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
		animacion ^= true;

	if (key == GLFW_KEY_F5 && action == GLFW_PRESS)
		sound ^= true;

	//To play KeyFrame animation 
	if (key == GLFW_KEY_P && action == GLFW_PRESS)
	{
		if (play == false && (FrameIndex > 1))
		{
			std::cout << "Play animation" << std::endl;
			resetElements();
			//First Interpolation				
			interpolation();

			play = true;
			playIndex = 0;
			i_curr_steps = 0;
		}
		else
		{
			play = false;
			std::cout << "Not enough Key Frames" << std::endl;
		}
	}

	//To Save a KeyFrame
	if (key == GLFW_KEY_L && action == GLFW_PRESS)
	{
		if (FrameIndex < MAX_FRAMES)
		{
			std::cout << "movBrazoDer = " << rotR << std::endl;
			saveFrame();
		}
	}
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}
// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}