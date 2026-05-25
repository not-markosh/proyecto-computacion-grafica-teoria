#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <fstream>
#include <cmath>

// GLEW
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// Other Libs
#include "stb_image.h"

// GLM Mathematics
#include <glm/gtc/type_ptr.hpp>

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
// Light attributes
glm::vec3 lightPos(0.0f, 0.0f, 0.0f);
bool active;

// Positions of the point lights
glm::vec3 pointLightPositions[] = {
	glm::vec3(0.0f,2.0f, 0.0f),
	glm::vec3(0.0f,0.0f, 0.0f),
	glm::vec3(0.0f,0.0f,  0.0f),
	glm::vec3(0.0f,0.0f, 0.0f)
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


glm::vec3 Light1 = glm::vec3(0);
//Anim
float rotBall = 0.0f;
float rotDog = 0.0f;
int dogAnim = 0;
float FLegs = 0.0f;
float RLegs = 0.0f;
float head = 0.0f;
float tail = 0.0f;
float FRLeg = 0.0f;

//Animacion fbx
bool playFBX = false;
bool playAlex = false;
bool playZombie = false;
bool playGhast = false;
Animation* g_SteveAnim = nullptr;
Animator* g_SteveAnimator = nullptr;
float steveSpeed = 1.0f;
int steveEstado = 0;
float steveDir = 1.0f;
Animation* g_alexAnim = nullptr;
Animator* g_alexAnimator = nullptr;
Animation* g_zombieAnim = nullptr;
Animator* g_zombieAnimator = nullptr;
Animation* g_ghastAnim = nullptr;
Animator* g_ghastAnimator = nullptr;


//KeyFrames
float dogPosX, dogPosY, dogPosZ;

#define MAX_FRAMES 16
int i_max_steps = 190;
int i_curr_steps = 0;
typedef struct _frame {

	float rotDog;
	float rotDogInc;
	float dogPosX;
	float dogPosY;
	float dogPosZ;
	float incX;
	float incY;
	float incZ;
	float head;
	float headInc;
	float FRLeg;
	float FRLegInc;
	float RLegs;
	float RLegsInc;


}FRAME;

FRAME KeyFrame[MAX_FRAMES];
int FrameIndex = 0;			//introducir datos
bool play = false;
int playIndex = 0;

void saveFrame(void)
{

	printf("frameindex %d\n", FrameIndex);

	KeyFrame[FrameIndex].dogPosX = dogPosX;
	KeyFrame[FrameIndex].dogPosY = dogPosY;
	KeyFrame[FrameIndex].dogPosZ = dogPosZ;

	KeyFrame[FrameIndex].rotDog = rotDog;
	KeyFrame[FrameIndex].head = head;
	KeyFrame[FrameIndex].FRLeg = FRLeg;
	KeyFrame[FrameIndex].RLegs = RLegs;


	FrameIndex++;
}

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
		file << KeyFrame[i].dogPosX << " "
			<< KeyFrame[i].dogPosY << " "
			<< KeyFrame[i].dogPosZ << " "
			<< KeyFrame[i].rotDog << " "
			<< KeyFrame[i].head << " "
			<< KeyFrame[i].FRLeg << " "
			<< KeyFrame[i].RLegs << "\n";
	}

	file.close();
	printf("Saved %d frames to %s\n", FrameIndex, filename);
}

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
		KeyFrame[i].rotDogInc = 0;
		KeyFrame[i].headInc = 0;
		KeyFrame[i].FRLegInc = 0;
		KeyFrame[i].RLegsInc = 0;
	}

	for (int i = 0; i < frameCount; i++)
	{
		file >> KeyFrame[i].dogPosX
			>> KeyFrame[i].dogPosY
			>> KeyFrame[i].dogPosZ
			>> KeyFrame[i].rotDog
			>> KeyFrame[i].head
			>> KeyFrame[i].FRLeg
			>> KeyFrame[i].RLegs;
	}

	FrameIndex = frameCount;
	file.close();
	printf("Loaded %d frames from %s\n", frameCount, filename);
}

void resetElements(void)
{
	dogPosX = KeyFrame[0].dogPosX;
	dogPosY = KeyFrame[0].dogPosY;
	dogPosZ = KeyFrame[0].dogPosZ;
	head = KeyFrame[0].head;
	FRLeg = KeyFrame[0].FRLeg;

	rotDog = KeyFrame[0].rotDog;

}
void interpolation(void)
{

	KeyFrame[playIndex].incX = (KeyFrame[playIndex + 1].dogPosX - KeyFrame[playIndex].dogPosX) / i_max_steps;
	KeyFrame[playIndex].incY = (KeyFrame[playIndex + 1].dogPosY - KeyFrame[playIndex].dogPosY) / i_max_steps;
	KeyFrame[playIndex].incZ = (KeyFrame[playIndex + 1].dogPosZ - KeyFrame[playIndex].dogPosZ) / i_max_steps;
	KeyFrame[playIndex].headInc = (KeyFrame[playIndex + 1].head - KeyFrame[playIndex].head) / i_max_steps;
	KeyFrame[playIndex].FRLegInc = (KeyFrame[playIndex + 1].FRLeg - KeyFrame[playIndex].FRLeg) / i_max_steps;
	KeyFrame[playIndex].RLegsInc = (KeyFrame[playIndex + 1].RLegs - KeyFrame[playIndex].RLegs) / i_max_steps;

	KeyFrame[playIndex].rotDogInc = (KeyFrame[playIndex + 1].rotDog - KeyFrame[playIndex].rotDog) / i_max_steps;

}



// Deltatime
GLfloat deltaTime = 0.0f;	// Time between current frame and last frame
GLfloat lastFrame = 0.0f;  	// Time of last frame

int main()
{
	// Init GLFW
	glfwInit();
	// Set all the required options for GLFW
	/*glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);*/

	// Create a GLFWwindow object that we can use for GLFW's functions
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

	// GLFW Options
	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

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



	Shader lightingShader("Shader/lighting.vs", "Shader/lighting.frag");
	Shader lampShader("Shader/lamp.vs", "Shader/lamp.frag");
	Shader skyboxShader("Shader/SkyBox.vs", "Shader/SkyBox.frag");


	//models
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

	Model Steve((char*)"Models/Steve/steveAnimated.fbx");
	g_SteveAnim = new Animation("Models/Steve/steveAnimated.fbx", &Steve);
	g_SteveAnimator = new Animator(g_SteveAnim);
	g_SteveAnimator->Reset();

	Model Alex((char*)"Models/Alex/alexAnimated.fbx");
	g_alexAnim = new Animation("Models/Alex/alexAnimated.fbx", &Alex);
	g_alexAnimator = new Animator(g_alexAnim);
	g_alexAnimator->Reset();

	Model Zombie((char*)"Models/Zombie/zombieAnimated.fbx");
	g_zombieAnim = new Animation("Models/Zombie/zombieAnimated.fbx", &Zombie);
	g_zombieAnimator = new Animator(g_zombieAnim);
	g_zombieAnimator->Reset();

	Model Ghast((char*)"Models/Ghast/ghast.fbx");
	g_ghastAnim = new Animation("Models/Ghast/ghast.fbx", &Ghast);
	g_ghastAnimator = new Animator(g_ghastAnim);
	g_ghastAnimator->Reset();


	//KeyFrames
	for (int i = 0; i < MAX_FRAMES; i++)
	{
		KeyFrame[i].dogPosX = 0;
		KeyFrame[i].dogPosY = 0;
		KeyFrame[i].dogPosZ = 0;
		KeyFrame[i].incX = 0;
		KeyFrame[i].incY = 0;
		KeyFrame[i].incZ = 0;
		KeyFrame[i].rotDog = 0;
		KeyFrame[i].rotDogInc = 0;
		KeyFrame[i].head = 0;
		KeyFrame[i].headInc = 0;
		KeyFrame[i].FRLeg = 0;
		KeyFrame[i].FRLegInc = 0;
		KeyFrame[i].RLegs = 0;
		KeyFrame[i].RLegsInc = 0;
	}

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

	// Game loop
	while (!glfwWindowShouldClose(window))
	{

		// Calculate deltatime of current frame
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();
		DoMovement();
		AnimationKeys();

		// Clear the colorbuffer
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// OpenGL options
		glEnable(GL_DEPTH_TEST);


		glm::mat4 modelTemp = glm::mat4(1.0f); //Temp


		// Use cooresponding shader when setting uniforms/drawing objects
		lightingShader.Use();

		glUniform1i(glGetUniformLocation(lightingShader.Program, "diffuse"), 0);
		//glUniform1i(glGetUniformLocation(lightingShader.Program, "specular"),1);

		GLint viewPosLoc = glGetUniformLocation(lightingShader.Program, "viewPos");
		glUniform3f(viewPosLoc, camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);


		// Directional light
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.direction"), -0.2f, -1.0f, -0.3f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.ambient"), 0.6f, 0.6f, 0.6f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.diffuse"), 0.6f, 0.6f, 0.6f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.specular"), 0.3f, 0.3f, 0.3f);


		// Point light 1
		glm::vec3 lightColor;
		lightColor.x = abs(sin(glfwGetTime() * Light1.x));
		lightColor.y = abs(sin(glfwGetTime() * Light1.y));
		lightColor.z = sin(glfwGetTime() * Light1.z);


		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].position"), pointLightPositions[0].x, pointLightPositions[0].y, pointLightPositions[0].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].ambient"), lightColor.x, lightColor.y, lightColor.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].diffuse"), lightColor.x, lightColor.y, lightColor.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].specular"), 1.0f, 0.2f, 0.2f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].linear"), 0.045f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].quadratic"), 0.075f);


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

		//Steve (Herobrine)
		model = modelTemp;
		if (playFBX)
		{
			// Maquina estados
			if (steveEstado == 0) // Estado 0: Caminando hacia adelante
			{
				g_SteveAnimator->UpdateAnimation(deltaTime);
				dogPosZ += steveDir * steveSpeed * deltaTime;

				if (dogPosZ >= 2.8f)
				{
					dogPosZ = 2.8f;   
					steveDir = -1.0f;
					steveEstado = 1;    
				}
			}
			else if (steveEstado == 1) // Estado 1: Regresando al origen
			{
				g_SteveAnimator->UpdateAnimation(deltaTime);
				dogPosZ += steveDir * steveSpeed * deltaTime;

				if (dogPosZ <= 0.0f)
				{
					dogPosZ = 0.0f;    
					steveEstado = 2;     
					playFBX = false;
				}
			}

			glUniform1i(glGetUniformLocation(lightingShader.Program, "useBones"), 1);
			model = glm::translate(model, glm::vec3(2.0f + dogPosX, 0.1f + dogPosY, 0.0f + dogPosZ));
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
			model = glm::translate(model, glm::vec3(2.0f + dogPosX, 0.1f + dogPosY, 0.0f + dogPosZ));
			model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
			model = glm::scale(model, glm::vec3(0.05f));
		}
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Steve.Draw(lightingShader);

		glUniform1i(glGetUniformLocation(lightingShader.Program, "useBones"), 0);

		//Alex
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

		//Zombie
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

		//Ghast
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


		////////////////////////////////
		/* Inicia Dibujo Modelo Zorro */
		////////////////////////////////

		// Cabeza
		model = modelTemp;
		model = glm::scale(model, glm::vec3(0.05f));
		model = glm::translate(model, glm::vec3(0.0f, 3.0f, 40.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		ZorroCabeza.Draw(lightingShader);

		// Cuerpo
		model = modelTemp;
		model = glm::scale(model, glm::vec3(0.05f));
		model = glm::translate(model, glm::vec3(0.0f, 3.0f, 40.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		ZorroCuerpo.Draw(lightingShader);

		// Cola
		model = modelTemp;
		model = glm::scale(model, glm::vec3(0.05f));
		model = glm::translate(model, glm::vec3(0.0f, 3.0f, 40.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		ZorroCola.Draw(lightingShader);

		// Pata Delantera Derecha
		model = modelTemp;
		model = glm::scale(model, glm::vec3(0.05f));
		model = glm::translate(model, glm::vec3(0.0f, 3.0f, 40.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		ZorroPataDD.Draw(lightingShader);

		// Pata Delantera Izquiera
		model = modelTemp;
		model = glm::scale(model, glm::vec3(0.05f));
		model = glm::translate(model, glm::vec3(0.0f, 3.0f, 40.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		ZorroPataDI.Draw(lightingShader);

		// Pata Trasera Derecha
		model = modelTemp;
		model = glm::scale(model, glm::vec3(0.05f));
		model = glm::translate(model, glm::vec3(0.0f, 3.0f, 40.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		ZorroPataTD.Draw(lightingShader);
		
		// Pata Trasera Izquierda
		model = modelTemp;
		model = glm::scale(model, glm::vec3(0.05f));
		model = glm::translate(model, glm::vec3(0.0f, 3.0f, 40.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		ZorroPataTI.Draw(lightingShader);

		
		/////////////////////////////////
		/* Termina Dibujo Modelo Zorro */
		/////////////////////////////////


		///////////////////////////////
		/* Inicia Dibujo Modelo Gato */
		///////////////////////////////

		// Cabeza
		model = modelTemp;
		model = glm::scale(model, glm::vec3(0.05f));
		model = glm::translate(model, glm::vec3(0.0f, 3.6f, -25.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		GatoCuerpo.Draw(lightingShader);

		model = modelTemp;
		model = glm::scale(model, glm::vec3(0.05f));
		model = glm::translate(model, glm::vec3(0.0f, 3.6f, -25.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		GatoCara.Draw(lightingShader);

		model = modelTemp;
		model = glm::scale(model, glm::vec3(0.05f));
		model = glm::translate(model, glm::vec3(0.0f, 3.6f, -25.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		GatoCola.Draw(lightingShader);

		model = modelTemp;
		model = glm::scale(model, glm::vec3(0.05f));
		model = glm::translate(model, glm::vec3(0.0f, 3.6f, -25.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		GatoPataDD.Draw(lightingShader);

		model = modelTemp;
		model = glm::scale(model, glm::vec3(0.05f));
		model = glm::translate(model, glm::vec3(0.0f, 3.6f, -25.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		GatoPataDI.Draw(lightingShader);

		model = modelTemp;
		model = glm::scale(model, glm::vec3(0.05f));
		model = glm::translate(model, glm::vec3(0.0f, 3.6f, -25.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		GatoPataTD.Draw(lightingShader);

		model = modelTemp;
		model = glm::scale(model, glm::vec3(0.05f));
		model = glm::translate(model, glm::vec3(0.0f, 3.6f, -25.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		GatoPataTI.Draw(lightingShader);

		/////////////////////////////////
		/* Termina Dibujo Modelo Gato */
		////////////////////////////////


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
		model = glm::translate(model, pointLightPositions[0]);
		model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		glBindVertexArray(0);

		// Draw SkyBox
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
	//Dog Controls

	if (keys[GLFW_KEY_2])
	{
		rotDog += 1.0f;
	}

	if (keys[GLFW_KEY_3])
	{
		rotDog -= 1.0f;
	}

	if (keys[GLFW_KEY_4])
	{
		head += 1.0f;
	}

	if (keys[GLFW_KEY_5])
	{
		head -= 1.0f;
	}

	if (keys[GLFW_KEY_6])
	{
		FRLeg += 1.0f;
	}

	if (keys[GLFW_KEY_7])
	{
		FRLeg -= 1.0f;
	}

	if (keys[GLFW_KEY_8])
	{
		RLegs += 1.0f;
	}

	if (keys[GLFW_KEY_9])
	{
		RLegs -= 1.0f;
	}

	if (keys[GLFW_KEY_H])
	{
		dogPosZ += 0.01;
	}

	if (keys[GLFW_KEY_Y])
	{
		dogPosZ -= 0.01;
	}

	if (keys[GLFW_KEY_G])
	{
		dogPosX -= 0.01;
	}

	if (keys[GLFW_KEY_J])
	{
		dogPosX += 0.01;
	}

	// Camera controls
	if (keys[GLFW_KEY_W] || keys[GLFW_KEY_UP])
	{
		camera.ProcessKeyboard(FORWARD, deltaTime);

	}

	if (keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN])
	{
		camera.ProcessKeyboard(BACKWARD, deltaTime);


	}

	if (keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT])
	{
		camera.ProcessKeyboard(LEFT, deltaTime);


	}

	if (keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT])
	{
		camera.ProcessKeyboard(RIGHT, deltaTime);


	}

	if (keys[GLFW_KEY_T])
	{
		pointLightPositions[0].x += 0.01f;
	}
	if (keys[GLFW_KEY_G])
	{
		pointLightPositions[0].x -= 0.01f;
	}

	if (keys[GLFW_KEY_Y])
	{
		pointLightPositions[0].y += 0.01f;
	}

	if (keys[GLFW_KEY_H])
	{
		pointLightPositions[0].y -= 0.01f;
	}
	if (keys[GLFW_KEY_U])
	{
		pointLightPositions[0].z -= 0.1f;
	}
	if (keys[GLFW_KEY_J])
	{
		pointLightPositions[0].z += 0.01f;
	}
	if (keys[GLFW_KEY_O])  // 'O' for Output (save to file)
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

	if (keys[GLFW_KEY_P])  // 'P' for loP... pickup (load from file)
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

	if (keys[GLFW_KEY_K])
	{
		if (FrameIndex < MAX_FRAMES)
		{
			saveFrame();
		}

	}



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

	if (key == GLFW_KEY_B && action == GLFW_PRESS)
	{
		dogPosZ = 0.0f;
		steveEstado = 0;
		steveDir = 1.0f;

		g_SteveAnimator->PlayAnimation(g_SteveAnim);
		playFBX = true;

		printf("Animación de Steve reiniciada desde el origen\n");

	}

	if (key == GLFW_KEY_V && action == GLFW_PRESS)
	{
		playAlex = !playAlex;
		if (playAlex) {
			g_alexAnimator->PlayAnimation(g_alexAnim);
			printf("Animacion Alex Activada\n");
		}
		else {
			g_alexAnimator->Reset();
			printf("Animacion Alex Pausada\n");
		}
	}

	if (key == GLFW_KEY_C && action == GLFW_PRESS)
	{
		playZombie = !playZombie;
		if (playZombie) {
			g_zombieAnimator->PlayAnimation(g_zombieAnim);
			printf("Animacion Zombie Activada\n");
		}
		else {
			g_zombieAnimator->Reset();
			printf("Animacion Zombie Pausada\n");
		}
	}

	if (key == GLFW_KEY_X && action == GLFW_PRESS)
	{
		playGhast = !playGhast;
		if (playGhast) {
			g_ghastAnimator->PlayAnimation(g_ghastAnim);
			printf("Animacion Ghast Activada\n");
		}
		else {
			g_ghastAnimator->Reset();
			printf("Animacion Ghast Pausada\n");
		}
	}


}
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
			dogPosX += KeyFrame[playIndex].incX;
			dogPosY += KeyFrame[playIndex].incY;
			dogPosZ += KeyFrame[playIndex].incZ;
			head += KeyFrame[playIndex].headInc;
			FRLeg += KeyFrame[playIndex].FRLegInc;
			RLegs += KeyFrame[playIndex].RLegsInc;

			rotDog += KeyFrame[playIndex].rotDogInc;

			i_curr_steps++;
		}

	}

}

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