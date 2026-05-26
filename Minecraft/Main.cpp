// Proyecto final - Escena inspirada en Minecraft
// Autores: Cecilia Torres y Marco Antonio Sanchez

#define GLM_ENABLE_EXPERIMENTAL

//GLM Mathematics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//C++ standard libraries
#include <iostream>
#include <fstream>
#include <cmath>

// GLEW
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// Load images
#include "stb_image.h"

//Load Models
#include "SOIL2/SOIL2.h"

// Other includes
#include "Shader.h"
#include "Camera.h"
#include "Model.h"
#include "Texture.h"
#include "Animation.h"
#include "Animator.h"

// Function prototypes
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
void MouseCallback(GLFWwindow* window, double xPos, double yPos);
void DoMovement();
void AnimationKeys();

// Window dimensions
const GLuint WIDTH = 800, HEIGHT = 600;
int SCREEN_WIDTH, SCREEN_HEIGHT;

// Camera
Camera  camera(glm::vec3(1.5f, 0.5f,3.0f), glm::vec3(0.0f, 1.0f, 0.0f), 270.0f);
GLfloat lastX = WIDTH / 2.0;
GLfloat lastY = HEIGHT / 2.0;
bool keys[1024];
bool firstMouse = true;

//Luces
glm::vec3 lightPos(0.0f, 0.0f, 0.0f);
bool active;

// Positions of the point lights
glm::vec3 pointLightPositions[] = {
	glm::vec3(0.0f,2.0f, 0.0f),
	//glm::vec3(0.0f,0.0f, 0.0f),
	//glm::vec3(0.0f,0.0f,  0.0f),
	//glm::vec3(0.0f,0.0f, 0.0f)
};

float vertices[] = {
	 -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	   -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	   -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

	   -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
	   -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
	   -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

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
	   -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
};

// === Animacion ciclo dia-noche ===
float sunAngle = 0.0f;
float sunSpeed = 0.2f;
float sunRadius = 10.0f;
bool dayNightCycle = true;
glm::vec3 sunPosition(0.0f);

// Intensidad luz puntual, se cambia con SPACE
glm::vec3 Light1 = glm::vec3(0);

// === Animacion maquina de estados zorro ===
int zorroEstado = 0;
float zorroSit = 0.0f;    // 0 = parado, 1 = sentado
float zorroSitSpeed = 1.5f;
// Angulos para la pose sentado
float sitBody = -18.5f;
float sitHead = 7.5f;
float sitRear = 13.5f;
float sitTail = -12.0f;

// === Animaciones FBX (por esqueleto) ===
bool playFBX = false;
bool playAlex = false;
bool playZombie = false;
bool playGhast = false;
Animation* g_SteveAnim = nullptr;
Animator* g_SteveAnimator = nullptr;
float steveSpeed = 1.0f;
int steveEstado = 0;
float steveDir = 1.0f;
float stevePosX = 0.0f, stevePosY = 0.0f, stevePosZ = 0.0f;
Animation* g_alexAnim = nullptr;
Animator* g_alexAnimator = nullptr;
Animation* g_zombieAnim = nullptr;
Animator* g_zombieAnimator = nullptr;
Animation* g_ghastAnim = nullptr;
Animator* g_ghastAnimator = nullptr;


// === Animacion keyframes gato ===
float gatoPosX = -0.15f, gatoPosY = 0.2f, gatoPosZ = -1.0f;
float rotGato = 0.0f;
float headGato = 0.0f;
float FLegsGato = 0.0f;
float BLegsGato = 0.0f;
float tailGato = 0.0f;
float inclinacionGato = 0.0f;
float inclinacionIncGato = 0.0f;

#define MAX_FRAMES 16
int i_max_steps = 140;
int i_curr_steps = 0;

typedef struct _frame {
	float rotGato;
	float rotGatoInc;
	float gatoPosX;
	float gatoPosY;
	float gatoPosZ;
	float incX;
	float incY;
	float incZ;
	float headGato;
	float headIncGato;
	float FLegsGato;
	float FLegsIncGato;
	float BLegsGato;
	float BLegsIncGato;
	float tailGato;
	float tailIncGato;
	float inclinacionGato;
	float inclinacionIncGato;
} FRAME;

FRAME KeyFrame[MAX_FRAMES];
int FrameIndex = 0;	
bool play = false;
int playIndex = 0;

// Funcion para guardar el estado actual del gato en un nuevo keyframe
void saveFrame(void)
{

	printf("frameindex %d\n", FrameIndex);

	KeyFrame[FrameIndex].gatoPosX = gatoPosX;
	KeyFrame[FrameIndex].gatoPosY = gatoPosY;
	KeyFrame[FrameIndex].gatoPosZ = gatoPosZ;

	KeyFrame[FrameIndex].rotGato = rotGato;
	KeyFrame[FrameIndex].headGato = headGato;
	KeyFrame[FrameIndex].FLegsGato = FLegsGato;
	KeyFrame[FrameIndex].BLegsGato = BLegsGato;
	KeyFrame[FrameIndex].tailGato = tailGato;

	KeyFrame[FrameIndex].inclinacionGato = inclinacionGato;

	FrameIndex++;
}

// Función para guardar los keyframes en un archivo de texto
void saveFramesToFile(const char* filename)
{
	std::ofstream file(filename);

	if (!file.is_open())
	{
		printf("Error: Could not open file %s for writing\n", filename);
		return;
	}

	file << FrameIndex << "\n";

	for (int i = 0; i < FrameIndex; i++)
	{
		file << KeyFrame[i].gatoPosX << " "
			<< KeyFrame[i].gatoPosY << " "
			<< KeyFrame[i].gatoPosZ << " "
			<< KeyFrame[i].rotGato << " "
			<< KeyFrame[i].headGato << " "
			<< KeyFrame[i].FLegsGato << " "
			<< KeyFrame[i].BLegsGato << " "
			<< KeyFrame[i].tailGato << " "
			<< KeyFrame[i].inclinacionGato << "\n";
	}

	file.close();
	printf("Saved %d frames to %s\n", FrameIndex, filename);
}

// Función para cargar los keyframes desde un archivo de texto
void loadFramesFromFile(const char* filename)
{
	std::ifstream file(filename);

	if (!file.is_open())
	{
		printf("Error: Could not open file %s for reading\n", filename);
		return;
	}

	int frameCount = 0;
	file >> frameCount;

	if (frameCount > MAX_FRAMES)
	{
		printf("Warning: File contains %d frames, but MAX_FRAMES is %d. Truncating.\n",
			frameCount, MAX_FRAMES);
		frameCount = MAX_FRAMES;
	}

	for (int i = 0; i < MAX_FRAMES; i++)
	{
		KeyFrame[i].incX = 0;
		KeyFrame[i].incY = 0;
		KeyFrame[i].incZ = 0;
		KeyFrame[i].rotGatoInc = 0;
		KeyFrame[i].headIncGato = 0;
		KeyFrame[i].FLegsIncGato = 0;
		KeyFrame[i].BLegsIncGato = 0;
		KeyFrame[i].tailIncGato = 0;
		KeyFrame[i].inclinacionIncGato = 0;
	}

	for (int i = 0; i < frameCount; i++)
	{
		file >> KeyFrame[i].gatoPosX
			>> KeyFrame[i].gatoPosY
			>> KeyFrame[i].gatoPosZ
			>> KeyFrame[i].rotGato
			>> KeyFrame[i].headGato
			>> KeyFrame[i].FLegsGato
			>> KeyFrame[i].BLegsGato
			>> KeyFrame[i].tailGato
			>> KeyFrame[i].inclinacionGato;
	}

	FrameIndex = frameCount;
	file.close();
	printf("Loaded %d frames from %s\n", frameCount, filename);
}

// Función para resetear el estado del gato al primer keyframe
void resetElements(void)
{
	gatoPosX = KeyFrame[0].gatoPosX;
	gatoPosY = KeyFrame[0].gatoPosY;
	gatoPosZ = KeyFrame[0].gatoPosZ;
	headGato = KeyFrame[0].headGato;
	FLegsGato = KeyFrame[0].FLegsGato;
	BLegsGato = KeyFrame[0].BLegsGato;
	tailGato = KeyFrame[0].tailGato;
	rotGato = KeyFrame[0].rotGato;
	inclinacionGato = KeyFrame[0].inclinacionGato;

}

// Función para calcular los incrementos entre el keyframe actual y el siguiente
void interpolation(void)
{

	KeyFrame[playIndex].incX = (KeyFrame[playIndex + 1].gatoPosX - KeyFrame[playIndex].gatoPosX) / i_max_steps;
	KeyFrame[playIndex].incY = (KeyFrame[playIndex + 1].gatoPosY - KeyFrame[playIndex].gatoPosY) / i_max_steps;
	KeyFrame[playIndex].incZ = (KeyFrame[playIndex + 1].gatoPosZ - KeyFrame[playIndex].gatoPosZ) / i_max_steps;
	KeyFrame[playIndex].headIncGato = (KeyFrame[playIndex + 1].headGato - KeyFrame[playIndex].headGato) / i_max_steps;
	KeyFrame[playIndex].FLegsIncGato = (KeyFrame[playIndex + 1].FLegsGato - KeyFrame[playIndex].FLegsGato) / i_max_steps;
	KeyFrame[playIndex].BLegsIncGato = (KeyFrame[playIndex + 1].BLegsGato - KeyFrame[playIndex].BLegsGato) / i_max_steps;
	KeyFrame[playIndex].tailIncGato = (KeyFrame[playIndex + 1].tailGato - KeyFrame[playIndex].tailGato) / i_max_steps;

	KeyFrame[playIndex].rotGatoInc = (KeyFrame[playIndex + 1].rotGato - KeyFrame[playIndex].rotGato) / i_max_steps;
	KeyFrame[playIndex].inclinacionIncGato = (KeyFrame[playIndex + 1].inclinacionGato - KeyFrame[playIndex].inclinacionGato) / i_max_steps;

}

// Deltatime
GLfloat deltaTime = 0.0f;	// Time between current frame and last frame
GLfloat lastFrame = 0.0f;  	// Time of last frame

int main()
{
	// Inicializacion de GLFW y creacion de ventana
	glfwInit();
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Proyecto final - Minecraft", nullptr, nullptr);

	if (nullptr == window)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();

		return EXIT_FAILURE;
	}

	glfwMakeContextCurrent(window);

	glfwGetFramebufferSize(window, &SCREEN_WIDTH, &SCREEN_HEIGHT);

	// Set the required callback functions
	glfwSetKeyCallback(window, KeyCallback);
	glfwSetCursorPosCallback(window, MouseCallback);

	// Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
	glewExperimental = GL_TRUE;
	// Initialize GLEW to setup the OpenGL Function pointers
	if (GLEW_OK != glewInit())
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return EXIT_FAILURE;
	}

	// Define the viewport dimensions
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

	// === Carga de shaders ===
	Shader lightingShader("Shader/lighting.vs", "Shader/lighting.frag");
	Shader lampShader("Shader/lamp.vs", "Shader/lamp.frag");
	Shader skyboxShader("Shader/SkyBox.vs", "Shader/SkyBox.frag");


	// === Carga de modelos ===

	//Model Aldea
	Model Aldea((char*)"Models/Aldea/aldea.obj");

	//Model Zorro
	Model ZorroCabeza((char*)"Models/Zorro/cabeza.obj");
	Model ZorroCola((char*)"Models/Zorro/cola.obj");
	Model ZorroCuerpo((char*)"Models/Zorro/cuerpo.obj");
	Model ZorroPataDD((char*)"Models/Zorro/pataDerDelante.obj");
	Model ZorroPataTD((char*)"Models/Zorro/pataDerDetras.obj");
	Model ZorroPataDI((char*)"Models/Zorro/pataIzqDelante.obj");
	Model ZorroPataTI((char*)"Models/Zorro/pataIzqDetras.obj");

	//Model Gato
	Model GatoCara((char*)"Models/Gato/cara.obj");
	Model GatoCola((char*)"Models/Gato/cola.obj");
	Model GatoCuerpo((char*)"Models/Gato/cuerpo.obj");
	Model GatoPataDD((char*)"Models/Gato/pataDerechaAdelante.obj");
	Model GatoPataTD((char*)"Models/Gato/pataDerechaAtras.obj");
	Model GatoPataDI((char*)"Models/Gato/pataIzquierdaAdelante.obj");
	Model GatoPataTI((char*)"Models/Gato/pataIzquierdaAtras.obj");

	//Model Steve
	Model Steve((char*)"Models/Steve/steveAnimated.fbx");
	g_SteveAnim = new Animation("Models/Steve/steveAnimated.fbx", &Steve);
	g_SteveAnimator = new Animator(g_SteveAnim);
	g_SteveAnimator->Reset();

	//Model Alex
	Model Alex((char*)"Models/Alex/alexAnimated.fbx");
	g_alexAnim = new Animation("Models/Alex/alexAnimated.fbx", &Alex);
	g_alexAnimator = new Animator(g_alexAnim);
	g_alexAnimator->Reset();

	//Model Zombie
	Model Zombie((char*)"Models/Zombie/zombieAnimated.fbx");
	g_zombieAnim = new Animation("Models/Zombie/zombieAnimated.fbx", &Zombie);
	g_zombieAnimator = new Animator(g_zombieAnim);
	g_zombieAnimator->Reset();

	//Model Ghast
	Model Ghast((char*)"Models/Ghast/ghast.fbx");
	g_ghastAnim = new Animation("Models/Ghast/ghast.fbx", &Ghast);
	g_ghastAnimator = new Animator(g_ghastAnim);
	g_ghastAnimator->Reset();


	//Inicializacion de keyframes
	for (int i = 0; i < MAX_FRAMES; i++)
	{
		KeyFrame[i].gatoPosX = 0;
		KeyFrame[i].gatoPosY = 0;
		KeyFrame[i].gatoPosZ = 0;
		KeyFrame[i].incX = 0;
		KeyFrame[i].incY = 0;
		KeyFrame[i].incZ = 0;
		KeyFrame[i].rotGato = 0;
		KeyFrame[i].rotGatoInc = 0;
		KeyFrame[i].headGato = 0;
		KeyFrame[i].headIncGato = 0;
		KeyFrame[i].FLegsGato = 0;
		KeyFrame[i].FLegsIncGato = 0;
		KeyFrame[i].BLegsGato = 0;
		KeyFrame[i].BLegsIncGato = 0;
		KeyFrame[i].tailGato = 0;
		KeyFrame[i].tailIncGato = 0;
		KeyFrame[i].inclinacionGato = 0;
		KeyFrame[i].inclinacionIncGato = 0;

	}

	// Vertices skybox
	GLfloat skyboxVertices[] = {
		// Positions
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		1.0f,  1.0f, -1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		1.0f, -1.0f,  1.0f
	};

	GLuint indices[] =
	{  // Note that we start from 0!
		0,1,2,3,
		4,5,6,7,
		8,9,10,11,
		12,13,14,15,
		16,17,18,19,
		20,21,22,23,
		24,25,26,27,
		28,29,30,31,
		32,33,34,35
	};

	// First, set the container's VAO (and VBO)
	GLuint VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);


	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	// normal attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// Set texture units
	lightingShader.Use();
	glUniform1i(glGetUniformLocation(lightingShader.Program, "Material.difuse"), 0);
	glUniform1i(glGetUniformLocation(lightingShader.Program, "Material.specular"), 1);

	//Skybox
	GLuint skyboxVBO, skyboxVAO;
	glGenVertexArrays(1, &skyboxVAO);
	glGenBuffers(1, &skyboxVBO);
	glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);

	// Load textures
	vector < const GLchar*> faces;
	faces.push_back("SkyBox/right.jpg");
	faces.push_back("SkyBox/left.jpg");
	faces.push_back("SkyBox/top.jpg");
	faces.push_back("SkyBox/bottom.jpg");
	faces.push_back("SkyBox/back.jpg");
	faces.push_back("SkyBox/front.jpg");

	GLuint cubemapTexture = TextureLoading::LoadCubemap(faces);


	glm::mat4 projection = glm::perspective(camera.GetZoom(), (GLfloat)SCREEN_WIDTH / (GLfloat)SCREEN_HEIGHT, 0.1f, 100.0f);

	// === Loop para renderizar la escena ===
	while (!glfwWindowShouldClose(window))
	{

		// Calculate deltatime of current frame
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;


		// ==== Ciclo dia/noche ====
		if (dayNightCycle)
		{
			sunAngle += sunSpeed * deltaTime;
			if (sunAngle > glm::two_pi<float>())
				sunAngle -= glm::two_pi<float>();
		}

		// Sun orbits in a vertical circle around the scene center.
		sunPosition.x = sunRadius * cos(sunAngle);
		sunPosition.y = sunRadius * sin(sunAngle);
		sunPosition.z = 0.0f;

		// "Day factor": 1.0 when sun is high, 0.0 when below the horizon
		float dayFactor = glm::clamp(sin(sunAngle), 0.0f, 1.0f);

		// Direction FROM the sun TOWARD the scene (for directional light)
		glm::vec3 sunDir = glm::normalize(-sunPosition);

		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();
		DoMovement();
		AnimationKeys();

		// Maquina de estados del zorro
		if (zorroEstado == 1)        // sentado
		{
			zorroSit += zorroSitSpeed * deltaTime;
			if (zorroSit >= 1.0f) { zorroSit = 1.0f; zorroEstado = 2; }
		}
		else if (zorroEstado == 3)   // parado
		{
			zorroSit -= zorroSitSpeed * deltaTime;
			if (zorroSit <= 0.0f) { zorroSit = 0.0f; zorroEstado = 0; }
		}

		// Clear the colorbuffer
		/*glClearColor(0.1f, 0.1f, 0.1f, 1.0f);*/
		glm::vec3 daySky = glm::vec3(0.5f, 0.7f, 1.0f);
		glm::vec3 nightSky = glm::vec3(0.02f, 0.02f, 0.08f);
		glm::vec3 skyColor = glm::mix(nightSky, daySky, dayFactor);
		glClearColor(skyColor.r, skyColor.g, skyColor.b, 1.0f);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// OpenGL options
		glEnable(GL_DEPTH_TEST);


		glm::mat4 modelTemp = glm::mat4(1.0f); //Temp


		// Use corresponding shader when setting uniforms/drawing objects
		lightingShader.Use();

		glUniform1i(glGetUniformLocation(lightingShader.Program, "diffuse"), 0);
		//glUniform1i(glGetUniformLocation(lightingShader.Program, "specular"),1);

		GLint viewPosLoc = glGetUniformLocation(lightingShader.Program, "viewPos");
		glUniform3f(viewPosLoc, camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);

		// === Directional light (sun/moon) ===
		// Sunlight color shifts: warm orange near horizon, white at noon
		glm::vec3 noonColor = glm::vec3(1.0f, 0.97f, 0.9f);   // bright daylight
		glm::vec3 sunsetColor = glm::vec3(1.0f, 0.5f, 0.2f);    // warm orange/red

		// Blend between sunset and noon colors based on how high the sun is
		float horizonBlend = glm::clamp(sin(sunAngle) * 2.0f, 0.0f, 1.0f);
		glm::vec3 sunColor = glm::mix(sunsetColor, noonColor, horizonBlend);

		// Night ambient/diffuse values (cool, dim moonlight)
		glm::vec3 nightAmbient = glm::vec3(0.05f, 0.05f, 0.12f);
		glm::vec3 nightDiffuse = glm::vec3(0.08f, 0.08f, 0.2f);

		// Interpolate between night and day depending on dayFactor
		glm::vec3 ambient = glm::mix(nightAmbient, sunColor * 0.4f, dayFactor);
		glm::vec3 diffuse = glm::mix(nightDiffuse, sunColor * 0.8f, dayFactor);
		glm::vec3 specular = glm::mix(glm::vec3(0.1f), sunColor * 0.5f, dayFactor);

		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.direction"), sunDir.x, sunDir.y, sunDir.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.ambient"), ambient.x, ambient.y, ambient.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.diffuse"), diffuse.x, diffuse.y, diffuse.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.specular"), specular.x, specular.y, specular.z);


		// Point light 1
		glm::vec3 lightColor;
		lightColor.x = abs(sin(glfwGetTime() * Light1.x));
		lightColor.y = abs(sin(glfwGetTime() * Light1.y));
		lightColor.z = sin(glfwGetTime() * Light1.z);

		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].position"), sunPosition.x, sunPosition.y, sunPosition.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].ambient"), sunColor.x * dayFactor, sunColor.y * dayFactor, sunColor.z * dayFactor);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].diffuse"), sunColor.x * dayFactor, sunColor.y * dayFactor, sunColor.z * dayFactor);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].specular"), 1.0f, 0.9f, 0.7f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].linear"), 0.014f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].quadratic"), 0.0007f);


		// SpotLight
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.position"), camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.direction"), camera.GetFront().x, camera.GetFront().y, camera.GetFront().z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.ambient"), 0.2f, 0.2f, 0.8f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.diffuse"), 0.2f, 0.2f, 0.8f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.specular"), 0.0f, 0.0f, 0.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.linear"), 0.3f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.quadratic"), 0.7f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.cutOff"), glm::cos(glm::radians(12.0f)));
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.outerCutOff"), glm::cos(glm::radians(18.0f)));


		// Set material properties
		glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 5.0f);

		// Create camera transformations
		glm::mat4 view;
		view = camera.GetViewMatrix();

		// Get the uniform locations
		GLint modelLoc = glGetUniformLocation(lightingShader.Program, "model");
		GLint viewLoc = glGetUniformLocation(lightingShader.Program, "view");
		GLint projLoc = glGetUniformLocation(lightingShader.Program, "projection");

		// Pass the matrices to the shader
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));


		glm::mat4 model(1);

		glUniform1i(glGetUniformLocation(lightingShader.Program, "useBones"), 0);

		//Carga de modelos 
		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Aldea.Draw(lightingShader);

		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 0);

		// === Animacion de Steve caminando ===
		model = modelTemp;
		if (playFBX)
		{
			// Maquina estados
			if (steveEstado == 0) // Estado 0: Caminando hacia adelante
			{
				g_SteveAnimator->UpdateAnimation(deltaTime);
				stevePosZ += steveDir * steveSpeed * deltaTime;

				if (stevePosZ >= 2.8f)
				{
					stevePosZ = 2.8f;   
					steveDir = -1.0f;
					steveEstado = 1;    
				}
			}
			else if (steveEstado == 1) // Estado 1: Regresando al origen
			{
				g_SteveAnimator->UpdateAnimation(deltaTime);
				stevePosZ += steveDir * steveSpeed * deltaTime;

				if (stevePosZ <= 0.0f)
				{
					stevePosZ = 0.0f;    
					steveEstado = 2;     
					playFBX = false;
				}
			}

			glUniform1i(glGetUniformLocation(lightingShader.Program, "useBones"), 1);
			model = glm::translate(model, glm::vec3(2.0f + stevePosX, 0.1f + stevePosY, 0.0f + stevePosZ));
			if (steveEstado == 1)
				model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::scale(model, glm::vec3(0.05f));
			const auto& transforms = g_SteveAnimator->GetFinalBoneMatrices();
			for (int i = 0; i < (int)transforms.size(); i++)
			{
				string uniformName = "finalBonesMatrices[" + to_string(i) + "]";
				glUniformMatrix4fv(
					glGetUniformLocation(lightingShader.Program, uniformName.c_str()),
					1, GL_FALSE, glm::value_ptr(transforms[i])
				);
			}
		}
		else
		{
			glUniform1i(glGetUniformLocation(lightingShader.Program, "useBones"), 0);
			model = glm::translate(model, glm::vec3(2.0f + stevePosX, 0.1f + stevePosY, 0.0f + stevePosZ));
			model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
			model = glm::scale(model, glm::vec3(0.05f));
		}
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Steve.Draw(lightingShader);

		glUniform1i(glGetUniformLocation(lightingShader.Program, "useBones"), 0);

		// === Animacion de Alex idle ===
		model = modelTemp;
		if (playAlex)
		{
			glUniform1i(glGetUniformLocation(lightingShader.Program, "useBones"), 1);
			g_alexAnimator->UpdateAnimation(deltaTime);

			model = glm::translate(model, glm::vec3(1.0f, -0.05f, 1.0f));
			model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::scale(model, glm::vec3(0.2f));

			const auto& alexTransforms = g_alexAnimator->GetFinalBoneMatrices();
			for (int i = 0; i < (int)alexTransforms.size(); i++)
			{
				string uniformName = "finalBonesMatrices[" + to_string(i) + "]";
				glUniformMatrix4fv(
					glGetUniformLocation(lightingShader.Program, uniformName.c_str()),
					1, GL_FALSE, glm::value_ptr(alexTransforms[i])
				);
			}
		}
		else
		{
			glUniform1i(glGetUniformLocation(lightingShader.Program, "useBones"), 1);

			const auto& alexTransforms = g_alexAnimator->GetFinalBoneMatrices();
			for (int i = 0; i < (int)alexTransforms.size(); i++)
			{
				string uniformName = "finalBonesMatrices[" + to_string(i) + "]";
				glUniformMatrix4fv(
					glGetUniformLocation(lightingShader.Program, uniformName.c_str()),
					1, GL_FALSE, glm::value_ptr(alexTransforms[i])
				);
			}

			model = glm::translate(model, glm::vec3(1.0f, -0.05f, 1.0f));
			model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::scale(model, glm::vec3(0.2f));
		}
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Alex.Draw(lightingShader);
		glUniform1i(glGetUniformLocation(lightingShader.Program, "useBones"), 0);

		// === Animacion de zombie con pushups ===
		model = modelTemp;
		if (playZombie)
		{
			glUniform1i(glGetUniformLocation(lightingShader.Program, "useBones"), 1);
			g_zombieAnimator->UpdateAnimation(deltaTime);

			model = glm::translate(model, glm::vec3(-0.5f, 0.7f, 0.5f));
			model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::scale(model, glm::vec3(0.05f));

			const auto& zombieTransforms = g_zombieAnimator->GetFinalBoneMatrices();
			for (int i = 0; i < (int)zombieTransforms.size(); i++)
			{
				string uniformName = "finalBonesMatrices[" + to_string(i) + "]";
				glUniformMatrix4fv(
					glGetUniformLocation(lightingShader.Program, uniformName.c_str()),
					1, GL_FALSE, glm::value_ptr(zombieTransforms[i])
				);
			}
		}
		else
		{
			glUniform1i(glGetUniformLocation(lightingShader.Program, "useBones"), 1);

			const auto& zombieTransforms = g_zombieAnimator->GetFinalBoneMatrices();
			for (int i = 0; i < (int)zombieTransforms.size(); i++)
			{
				string uniformName = "finalBonesMatrices[" + to_string(i) + "]";
				glUniformMatrix4fv(
					glGetUniformLocation(lightingShader.Program, uniformName.c_str()),
					1, GL_FALSE, glm::value_ptr(zombieTransforms[i])
				);
			}

			model = glm::translate(model, glm::vec3(-0.5f, 0.7f, 0.5f));
			model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::scale(model, glm::vec3(0.05f));
		}
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Zombie.Draw(lightingShader);
		glUniform1i(glGetUniformLocation(lightingShader.Program, "useBones"), 0);

		// === Animacion de ghast flotando ===
		model = modelTemp;
		if (playGhast)
		{
			glUniform1i(glGetUniformLocation(lightingShader.Program, "useBones"), 1);
			g_ghastAnimator->UpdateAnimation(deltaTime);

			model = glm::translate(model, glm::vec3(0.7f, 1.5f, 0.2f));
			model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::rotate(model, glm::radians(30.0f), glm::vec3(1.0f, 0.0f, 0.0f));
			model = glm::scale(model, glm::vec3(0.2f));

			const auto& ghastTransforms = g_ghastAnimator->GetFinalBoneMatrices();
			for (int i = 0; i < (int)ghastTransforms.size(); i++)
			{
				string uniformName = "finalBonesMatrices[" + to_string(i) + "]";
				glUniformMatrix4fv(
					glGetUniformLocation(lightingShader.Program, uniformName.c_str()),
					1, GL_FALSE, glm::value_ptr(ghastTransforms[i])
				);
			}
		}
		else
		{
			glUniform1i(glGetUniformLocation(lightingShader.Program, "useBones"), 1);

			const auto& ghastTransforms = g_ghastAnimator->GetFinalBoneMatrices();
			for (int i = 0; i < (int)ghastTransforms.size(); i++)
			{
				string uniformName = "finalBonesMatrices[" + to_string(i) + "]";
				glUniformMatrix4fv(
					glGetUniformLocation(lightingShader.Program, uniformName.c_str()),
					1, GL_FALSE, glm::value_ptr(ghastTransforms[i])
				);
			}

			model = glm::translate(model, glm::vec3(0.7f, 1.5f, 0.2f));
			model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::rotate(model, glm::radians(30.0f), glm::vec3(1.0f, 0.0f, 0.0f));
			model = glm::scale(model, glm::vec3(0.2f));
		}
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Ghast.Draw(lightingShader);
		glUniform1i(glGetUniformLocation(lightingShader.Program, "useBones"), 0);

		// == Modelo del zorro ==
		float t = zorroSit;
		float bodyAng = sitBody * t;
		float headAng = sitHead * t;
		float rearAng = sitRear * t;
		float tailAng = sitTail * t;

		// Cabeza
		model = modelTemp;
		model = glm::scale(model, glm::vec3(0.05f));
		model = glm::translate(model, glm::vec3(0.0f, 3.0f, 40.0f));
		model = glm::rotate(model, glm::radians(bodyAng), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(headAng), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		ZorroCabeza.Draw(lightingShader);

		// Cuerpo
		model = modelTemp;
		model = glm::scale(model, glm::vec3(0.05f));
		model = glm::translate(model, glm::vec3(0.0f, 3.0f, 40.0f));
		model = glm::rotate(model, glm::radians(bodyAng), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		ZorroCuerpo.Draw(lightingShader);

		// Cola 
		model = modelTemp;
		model = glm::scale(model, glm::vec3(0.05f));
		model = glm::translate(model, glm::vec3(0.0f, 3.0f, 40.0f));
		model = glm::rotate(model, glm::radians(tailAng), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		ZorroCola.Draw(lightingShader);

		// Pata Delantera Derecha
		model = modelTemp;
		model = glm::scale(model, glm::vec3(0.05f));
		model = glm::translate(model, glm::vec3(0.0f, 3.0f, 40.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		ZorroPataDD.Draw(lightingShader);

		// Pata Delantera Izquierda
		model = modelTemp;
		model = glm::scale(model, glm::vec3(0.05f));
		model = glm::translate(model, glm::vec3(0.0f, 3.0f, 40.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		ZorroPataDI.Draw(lightingShader);

		// Pata Trasera Derecha 
		model = modelTemp;
		model = glm::scale(model, glm::vec3(0.05f));
		model = glm::translate(model, glm::vec3(0.0f, 3.0f, 40.0f));
		model = glm::rotate(model, glm::radians(bodyAng), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(rearAng), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		ZorroPataTD.Draw(lightingShader);

		// Pata Trasera Izquierda 
		model = modelTemp;
		model = glm::scale(model, glm::vec3(0.05f));
		model = glm::translate(model, glm::vec3(0.0f, 3.0f, 40.0f));
		model = glm::rotate(model, glm::radians(bodyAng), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(rearAng), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		ZorroPataTI.Draw(lightingShader);


		// === Modelo del gato ===
		glm::mat4 modelGato = modelTemp;
		modelGato = glm::translate(modelGato, glm::vec3(gatoPosX, gatoPosY, gatoPosZ));
		modelGato = glm::rotate(modelGato, glm::radians(rotGato), glm::vec3(0.0f, 1.0f, 0.0f));
		modelGato = glm::rotate(modelGato, glm::radians(inclinacionGato), glm::vec3(1.0f, 0.0f, 0.0f));
		modelGato = glm::scale(modelGato, glm::vec3(0.05f));

		// Cuerpo (Base)
		model = modelGato;
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		GatoCuerpo.Draw(lightingShader);

		// Cabeza
		model = modelGato;
		model = glm::rotate(model, glm::radians(headGato), glm::vec3(1.0f, 0.0f, 0.0f)); // 2. Rotar
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		GatoCara.Draw(lightingShader);

		// Cola
		model = modelGato;
		model = glm::rotate(model, glm::radians(tailGato), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		GatoCola.Draw(lightingShader);

		// Pata Delantera Derecha
		model = modelGato;
		model = glm::translate(model, glm::vec3(-0.5f, 0.3f, 1.5f));
		model = glm::rotate(model, glm::radians(FLegsGato), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::translate(model, glm::vec3(0.5f, -0.3f, -1.5f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		GatoPataDD.Draw(lightingShader);

		// Pata Delantera Izquierda
		model = modelGato;
		model = glm::translate(model, glm::vec3(0.5f, 0.3f, 1.5f));
		model = glm::rotate(model, glm::radians(FLegsGato), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::translate(model, glm::vec3(-0.5f, -0.3f, -1.5f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		GatoPataDI.Draw(lightingShader);

		// Pata Trasera Derecha
		model = modelGato;
		model = glm::translate(model, glm::vec3(-0.5f, 0.3f, -1.5f));
		model = glm::rotate(model, glm::radians(BLegsGato), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::translate(model, glm::vec3(0.5f, -0.3f, 1.5f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		GatoPataTD.Draw(lightingShader);

		// Pata Trasera Izquierda
		model = modelGato;
		model = glm::translate(model, glm::vec3(0.5f, 0.3f, -1.5f));
		model = glm::rotate(model, glm::radians(BLegsGato), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::translate(model, glm::vec3(-0.5f, -0.3f, 1.5f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		GatoPataTI.Draw(lightingShader);

		// ===Activacion del canal alfa para un modelo ===
		//model = glm::mat4(1);
		//glEnable(GL_BLEND);//Avtiva la funcionalidad para trabajar el canal alfa
		//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		//glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		//glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 1);
		//model = glm::rotate(model, glm::radians(rotBall), glm::vec3(0.0f, 1.0f, 0.0f));
		//glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		//Ball.Draw(lightingShader);
		//glDisable(GL_BLEND);  //Desactiva el canal alfa 
		//glBindVertexArray(0);

		// Also draw the lamp object, again binding the appropriate shader
		lampShader.Use();
		// Get location objects for the matrices on the lamp shader (these could be different on a different shader)
		modelLoc = glGetUniformLocation(lampShader.Program, "model");
		viewLoc = glGetUniformLocation(lampShader.Program, "view");
		projLoc = glGetUniformLocation(lampShader.Program, "projection");

		// Set matrices
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
		model = glm::mat4(1);
		model = glm::translate(model, lightPos);
		model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		// Draw the light object (using light's vertex attributes)

		model = glm::mat4(1);
		model = glm::translate(model, sunPosition);
		model = glm::scale(model, glm::vec3(0.6f)); // Bigger so the sun is visible
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		glBindVertexArray(0);

		// === Dibujo del skybox ===
		glDepthFunc(GL_LEQUAL);
		skyboxShader.Use();
		view = glm::mat4(glm::mat3(camera.GetViewMatrix()));
		glUniformMatrix4fv(glGetUniformLocation(skyboxShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(glGetUniformLocation(skyboxShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

		glBindVertexArray(skyboxVAO);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
		glDepthFunc(GL_LESS);

		// Swap the screen buffers
		glfwSwapBuffers(window);
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteVertexArrays(1, &skyboxVAO);
	glDeleteBuffers(1, &skyboxVAO);
	delete g_SteveAnim;
	delete g_SteveAnimator;

	// Terminate GLFW, clearing any resources allocated by GLFW.
	glfwTerminate();



	return 0;
}

// Moves/alters the camera positions based on user input
void DoMovement()
{
	// Camera controls
	if (keys[GLFW_KEY_W] || keys[GLFW_KEY_UP]) camera.ProcessKeyboard(FORWARD, deltaTime);
	if (keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN]) camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT]) camera.ProcessKeyboard(LEFT, deltaTime);
	if (keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT]) camera.ProcessKeyboard(RIGHT, deltaTime);

	// Sun controls
	if (keys[GLFW_KEY_COMMA]) sunAngle -= 0.01f; // sol atras
	if (keys[GLFW_KEY_PERIOD]) sunAngle += 0.01f; // sol al frente

	// Keyframe animation controls
	if (keys[GLFW_KEY_1]) FLegsGato -= 1.0f; // Patas delanteras adelante
	if (keys[GLFW_KEY_2]) FLegsGato += 1.0f; // Patas delanteras atras
	if (keys[GLFW_KEY_3]) BLegsGato -= 1.0f; // Patas traseras adelante
	if (keys[GLFW_KEY_4]) BLegsGato += 1.0f; // Patas traseras atras
	if (keys[GLFW_KEY_5]) tailGato -= 1.0f;  // Cola derecha
	if (keys[GLFW_KEY_6]) tailGato += 1.0f;  // Cola izquierda
	if (keys[GLFW_KEY_7]) headGato -= 1.0f;  // Cabeza arriba
	if (keys[GLFW_KEY_8]) headGato += 1.0f;  // Cabeza abajo
	if (keys[GLFW_KEY_KP_1]) gatoPosX -= 0.01f; // Izquierda
	if (keys[GLFW_KEY_KP_2]) gatoPosX += 0.01f; // Derecha
	if (keys[GLFW_KEY_KP_3]) gatoPosY -= 0.01f; // Abajo
	if (keys[GLFW_KEY_KP_4]) gatoPosY += 0.01f; // Arriba
	if (keys[GLFW_KEY_KP_5]) gatoPosZ -= 0.01f;  // Atras
	if (keys[GLFW_KEY_KP_6]) gatoPosZ += 0.01f;  // Adelante
	if (keys[GLFW_KEY_KP_7]) rotGato -= 0.1f; // Girar izq
	if (keys[GLFW_KEY_KP_8]) rotGato += 0.1f; // Girar der
	if (keys[GLFW_KEY_KP_9]) inclinacionGato += 0.1f; // Inclinar abajo
	if (keys[GLFW_KEY_KP_0]) inclinacionGato -= 0.1f; // Inclinar arriba


	// Keyframe save to file controls
	if (keys[GLFW_KEY_O]) 
	{
		if (FrameIndex > 0)
		{
			saveFramesToFile("animation.txt");
		}
		else
		{
			printf("No frames to save\n");
		}
	}

	// Keyframe load from file controls
	if (keys[GLFW_KEY_P]) 
	{
		loadFramesFromFile("animation.txt");
	}
}

// Is called whenever a key is pressed/released via GLFW
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{

	if (keys[GLFW_KEY_L])
	{
		if (play == false && (FrameIndex > 1))
		{

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
		}

	}

	// Guardar frame
	if (keys[GLFW_KEY_K])
	{
		if (FrameIndex < MAX_FRAMES)
		{
			saveFrame();
		}

	}

	// Cerrar ventana
	if (GLFW_KEY_ESCAPE == key && GLFW_PRESS == action)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
		{
			keys[key] = true;
		}
		else if (action == GLFW_RELEASE)
		{
			keys[key] = false;
		}
	}

	// Alternar luz puntual
	if (keys[GLFW_KEY_SPACE])
	{
		active = !active;
		if (active)
		{
			Light1 = glm::vec3(0.2f, 0.8f, 1.0f);

		}
		else
		{
			Light1 = glm::vec3(0);//Cuado es solo un valor en los 3 vectores pueden dejar solo una componente
		}
	}

	// Inicio de animacion de Steve caminando
	if (key == GLFW_KEY_B && action == GLFW_PRESS)
	{
		stevePosZ = 0.0f;
		steveEstado = 0;
		steveDir = 1.0f;

		g_SteveAnimator->PlayAnimation(g_SteveAnim);
		playFBX = true;

		printf("Animación de Steve iniciada\n");

	}

	// Inicio de animacion de Alex idle, alternando entre play y pause
	if (key == GLFW_KEY_V && action == GLFW_PRESS)
	{
		playAlex = !playAlex;
		if (playAlex) {
			g_alexAnimator->PlayAnimation(g_alexAnim);
			printf("Animacion de Alex activada\n");
		}
		else {
			g_alexAnimator->Reset();
			printf("Animacion de Alex pausada\n");
		}
	}

	// Inicio de animacion de zombie pushups, alternando entre play y pause
	if (key == GLFW_KEY_C && action == GLFW_PRESS)
	{
		playZombie = !playZombie;
		if (playZombie) {
			g_zombieAnimator->PlayAnimation(g_zombieAnim);
			printf("Animacion de Zombie activada\n");
		}
		else {
			g_zombieAnimator->Reset();
			printf("Animacion de Zombie pausada\n");
		}
	}

	// Inicio de animacion de ghast flotando, alternando entre play y pause
	if (key == GLFW_KEY_X && action == GLFW_PRESS)
	{
		playGhast = !playGhast;
		if (playGhast) {
			g_ghastAnimator->PlayAnimation(g_ghastAnim);
			printf("Animacion de Ghast activada\n");
		}
		else {
			g_ghastAnimator->Reset();
			printf("Animacion de Ghast pausada\n");
		}
	}

	// Inicio de animcion de zorro
	if (key == GLFW_KEY_Z && action == GLFW_PRESS)
	{
		if (zorroEstado == 0 || zorroEstado == 3)
		{
			zorroEstado = 1;   // sentarse
			printf("Animacion de zorro sentado\n");
		}
		else
		{
			zorroEstado = 3;   // levantarse
			printf("Animacion de zorro parado\n");
		}
	}

	// Iniciar o detener ciclo día/noche
	if (key == GLFW_KEY_N && action == GLFW_PRESS)
	{
		dayNightCycle = !dayNightCycle;
		printf("Ciclo del dia/noche: %s\n", dayNightCycle ? "ON" : "OFF");
	}


}

// Function to handle keyframe animation logic
void AnimationKeys() {

	if (play)
	{
		if (i_curr_steps >= i_max_steps) //end of animation between frames?
		{
			playIndex++;
			if (playIndex > FrameIndex - 2)	//end of total animation?
			{
				printf("termina anim\n");
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
			gatoPosX += KeyFrame[playIndex].incX;
			gatoPosY += KeyFrame[playIndex].incY;
			gatoPosZ += KeyFrame[playIndex].incZ;
			headGato += KeyFrame[playIndex].headIncGato;
			FLegsGato += KeyFrame[playIndex].FLegsIncGato;
			BLegsGato += KeyFrame[playIndex].BLegsIncGato;
			tailGato += KeyFrame[playIndex].tailIncGato;
			rotGato += KeyFrame[playIndex].rotGatoInc;
			inclinacionGato += KeyFrame[playIndex].inclinacionIncGato;

			i_curr_steps++;
		}

	}

}

// Is called whenever the mouse moves
void MouseCallback(GLFWwindow* window, double xPos, double yPos)
{
	if (firstMouse)
	{
		lastX = xPos;
		lastY = yPos;
		firstMouse = false;
	}

	GLfloat xOffset = xPos - lastX;
	GLfloat yOffset = lastY - yPos;  // Reversed since y-coordinates go from bottom to left

	lastX = xPos;
	lastY = yPos;

	camera.ProcessMouseMovement(xOffset, yOffset);
}