#include "runner.h"

#include <vector>
#include <string>
#include <iostream>

#include <Core/Engine.h>

using namespace std;
runner::runner()
{
}

runner::~runner()
{
}

void runner::Init()
{
	const string textureLoc = "Source/Laboratoare/runner/Textures/";

	// Load textures	
	{
		Texture2D* texture = new Texture2D();
		texture->Load2D((textureLoc + "crate.jpg").c_str(), GL_REPEAT);
		mapTextures["crate"] = texture;
	}
	{
		Texture2D* texture = new Texture2D();
		texture->Load2D((textureLoc + "wind.jpg").c_str(), GL_REPEAT);
		mapTextures["wind"] = texture;
	}
	{
		Texture2D* texture = new Texture2D();
		texture->Load2D((textureLoc + "walls.jpg").c_str(), GL_REPEAT);
		mapTextures["wall"] = texture;
	}
	{
		Texture2D* texture = new Texture2D();
		texture->Load2D((textureLoc + "bricks.jpg").c_str(), GL_REPEAT);
		mapTextures["bricks"] = texture;
	}

	{
		Texture2D* texture = new Texture2D();
		texture->Load2D((textureLoc + "grass_bilboard.png").c_str(), GL_REPEAT);
		mapTextures["grass"] = texture;
	}
	{
		Texture2D* texture = new Texture2D();
		texture->Load2D((textureLoc + "fireball.jpg").c_str(), GL_MIRRORED_REPEAT);
		mapTextures["fireball"] = texture;
	}

	{
		Texture2D* texture = new Texture2D();
		texture->Load2D((textureLoc + "dessert.jpg").c_str(), GL_REPEAT);
		mapTextures["dessert"] = texture;
	}
	{
		Texture2D* texture = new Texture2D();
		texture->Load2D((textureLoc + "snow.jpg").c_str(), GL_REPEAT);
		mapTextures["snow"] = texture;
	}

	// Load meshes
	{
		Mesh* mesh = new Mesh("box");
		mesh->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "box.obj");
		meshes[mesh->GetMeshID()] = mesh;
	}

	{
		Mesh* mesh = new Mesh("sphere");
		mesh->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "sphere.obj");
		meshes[mesh->GetMeshID()] = mesh;
	}

	{
		Mesh* mesh = new Mesh("bamboo");
		mesh->LoadMesh(RESOURCE_PATH::MODELS + "Vegetation/Bamboo/", "bamboo.obj");
		meshes[mesh->GetMeshID()] = mesh;
	}

	// Create a simple quad
	{
		vector<glm::vec3> vertices
		{
			glm::vec3(0.5f,   0.5f, 0.0f),	// Top Right
			glm::vec3(0.5f,  -0.5f, 0.0f),	// Bottom Right
			glm::vec3(-0.5f, -0.5f, 0.0f),	// Bottom Left
			glm::vec3(-0.5f,  0.5f, 0.0f),	// Top Left
		};

		vector<glm::vec3> normals
		{
			glm::vec3(0, 1, 1),
			glm::vec3(1, 0, 1),
			glm::vec3(1, 0, 0),
			glm::vec3(0, 1, 0)
		};

		// TODO : Complete texture coordinates for the square
		vector<glm::vec2> textureCoords
		{
			glm::vec2(1.0f, 0.0f),
			glm::vec2(1.0f, 1.0f),
			glm::vec2(0.0f, 1.0f),
			glm::vec2(0.0f, 0.0f),
		};

		vector<unsigned short> indices =
		{
			0, 1, 3,
			1, 2, 3
		};

		Mesh* mesh = new Mesh("square");
		mesh->InitFromData(vertices, normals, textureCoords, indices);
		meshes[mesh->GetMeshID()] = mesh;
	}
	// Create a shader program for drawing face polygon with the color of the normal
	{
		Shader *shader = new Shader("ShaderLab9");
		shader->AddShader("Source/Laboratoare/runner/Shaders/VertexShader.glsl", GL_VERTEX_SHADER);
		shader->AddShader("Source/Laboratoare/runner/Shaders/FragmentShader.glsl", GL_FRAGMENT_SHADER);
		shader->CreateAndLink();
		shaders[shader->GetName()] = shader;
	}
}

void runner::FrameStart()
{
	// clears the color buffer (using the previously set color) and depth buffer
	glClearColor(0.45, 0, 0.2, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::ivec2 resolution = window->GetResolution();
	// sets the screen area where to draw
	glViewport(0, 0, resolution.x, resolution.y);	
}
int runner::getRandomNumber(int min, int max) {
	return min + rand() % ((max + 1) - min);
}
void runner::Jump() {
	if (!in_air) {
		upward_speed = JUMP_POWER;
		in_air = true;
	}
}
void runner::GeneratePlayer(float deltaTime) {
	glm::mat4 modelMatrix = glm::mat4(1);
	upward_speed += GRAVITY * 6.0f * deltaTime;
	modelMatrix = glm::translate(modelMatrix, glm::vec3(left_right, upward_speed * deltaTime, 0));
	if (modelMatrix[3][1] < 0) {
		upward_speed = 0;
		in_air = false;
		modelMatrix[3][1] = 0;
	}
	modelMatrix = glm::scale(modelMatrix, glm::vec3(0.75f));
	RenderSimpleMesh(meshes["sphere"], shaders["ShaderLab9"], modelMatrix, mapTextures["fireball"]);
	playerPoints = glm::vec3(modelMatrix[3][0], modelMatrix[3][1], modelMatrix[3][2]);
}
void runner::GeneratePlatform() {

	/* Genereaza prima platforma */
	glm::mat4 fstPlatform = glm::mat4(1);
	fstPlatform = glm::translate(fstPlatform, glm::vec3(0, -0.3f, start_platform1 + speed_platform1));
	fstPlatform = glm::scale(fstPlatform, glm::vec3(2.25f, 0.15f, scale_factor));
	fstplatformPoints = glm::vec3(fstPlatform[3][0], fstPlatform[3][1], fstPlatform[3][2]);

	/* Genereaza a doua platforma */
	glm::mat4 sndPlatform = glm::mat4(1);
	sndPlatform = glm::translate(sndPlatform, glm::vec3(0, -0.3f, start_platform2 + speed_platform2));
	sndPlatform = glm::scale(sndPlatform, glm::vec3(2.25f, 0.15f, scale_factor));
	sndplatformPoints = glm::vec3(sndPlatform[3][0], sndPlatform[3][1], sndPlatform[3][2]);
	RenderSimpleMesh(meshes["box"], shaders["ShaderLab9"], sndPlatform, mapTextures["dessert"]);
	/* Daca prima platform a iesit din vederea camerei sa se reseteze
		la viteza initiala si pozitia platformei nr 2 */
	if (fstplatformPoints[2] > 5.5f) {
		speed_platform1 = 0;
		start_platform1 = start_platform2;
	}
	else {
	/* Sa se faca render la platforma daca aceasta nu a iesit din vederea camerei */
		RenderSimpleMesh(meshes["box"], shaders["ShaderLab9"], fstPlatform, mapTextures["dessert"]);
	}
	/* Analogic ca la platforma nr1 */
	if (sndplatformPoints[2] > 5.5f) {
		speed_platform2 = 0;
		start_platform2 = start_platform1;
	}
	else {
		RenderSimpleMesh(meshes["box"], shaders["ShaderLab9"], sndPlatform, mapTextures["dessert"]);
	}
}
void runner::GenerateWater(float deltaTime, float random_z) {
	{
		if (!flag) {
			this_z = getRandomNumber(1, 5);
			flag = true;
			cout << this_z << endl;
		}
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(0.5f, 0.2, -this_z + start_platform1 + speed_platform1));
		modelMatrix = glm::rotate(modelMatrix, sin(deltaTime), glm::vec3(0, 1, 0));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(0.2f, 0.29f, 0.1f));
		this_water[0] = modelMatrix;
		//RenderSimpleMesh(meshes["sphere"], shaders["ShaderLab9"], modelMatrix, mapTextures["snow"]);
	}
	{
		if (!flag) {
			this_z = getRandomNumber(2, 7);
			flag = true;
			cout << this_z << endl;
		}
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(-0.5f, 0.2, -this_z + start_platform2 + speed_platform2 + 5.8f));
		modelMatrix = glm::rotate(modelMatrix, sin(deltaTime), glm::vec3(0, 1, 0));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(0.2f, 0.29f, 0.1f));
		this_water[1] = modelMatrix;
		//RenderSimpleMesh(meshes["sphere"], shaders["ShaderLab9"], modelMatrix, mapTextures["snow"]);
	}
	{
		if (!flag) {
			this_z = getRandomNumber(2, 7);
			flag = true;
			cout << this_z << endl;
		}
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 0.2, -this_z + start_platform2 + speed_platform2));
		modelMatrix = glm::rotate(modelMatrix, sin(deltaTime), glm::vec3(0, 1, 0));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(0.2f, 0.29f, 0.1f));
		this_water[2] = modelMatrix;
		//RenderSimpleMesh(meshes["sphere"], shaders["ShaderLab9"], modelMatrix, mapTextures["snow"]);
	}
	int colide = CheckAllArray(this_water, 0.29, 3);
	for (int i = 0; i < 3; i++) {
		if (colide == i) {
			cout << "+10 points" << endl;
		} 
		else {
			RenderSimpleMesh(meshes["sphere"], shaders["ShaderLab9"], this_water[i], mapTextures["snow"]);
		}
	}
}
void runner::GenerateGrass() {
	{
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(-0.5f, 0, start_platform1 + speed_platform1));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(0.33f));
		RenderSimpleMesh(meshes["square"], shaders["ShaderLab9"], modelMatrix, mapTextures["grass"]);
	}
	{
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(0.5f, 0, start_platform2 + speed_platform1));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(0.33f));
		RenderSimpleMesh(meshes["square"], shaders["ShaderLab9"], modelMatrix, mapTextures["grass"]);
	}
	{
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(-0.33f, 0, start_platform2 + 2 + speed_platform1));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(0.33f));
		RenderSimpleMesh(meshes["square"], shaders["ShaderLab9"], modelMatrix, mapTextures["grass"]);
	}
	{
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(0.89f, 0, start_platform1 -3  + speed_platform1));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(0.33f));
		RenderSimpleMesh(meshes["square"], shaders["ShaderLab9"], modelMatrix, mapTextures["grass"]);
	}
}
void runner::GenerateBoxes(float delta) {
	{
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(0.58f, 0, speed_platform1 + start_platform1));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(0.5f));
		this_boxes[0] = modelMatrix;
	}
	{
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(-0.69f, 0, speed_platform2 + start_platform2));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(0.5f));
		this_boxes[1] = modelMatrix;
	}
	{
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 0, speed_platform1 + start_platform2 + 2));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(0.5f));
		this_boxes[2] = modelMatrix;
	}
	for (int i = 0; i < 3; i++) {
		/* Daca nu s-a detectat nici o coliziune cu cutie */
		if (!Check4Collision(this_boxes[i], 0.5)) {
			RenderSimpleMesh(meshes["box"], shaders["ShaderLab9"], this_boxes[i], mapTextures["crate"]);
		}
		/* Daca s-a detectat o coliziune cu cutie, las-o sa dispara pe delta timp */
		else {
			//exit(1);
			delay_boxes += delta * 2.0f;
			cout << delay_boxes << endl;
		}
	}
	delay_boxes = 0.0;
}
void runner::GeneratePyramid(int idx) {
	{
		glm::mat4 fstFloor = glm::mat4(1);
		fstFloor = glm::translate(fstFloor, glm::vec3(idx, 0, -5));
		fstFloor = glm::scale(fstFloor, glm::vec3(3.2f, 0.25f, 3.2f));
		RenderSimpleMesh(meshes["box"], shaders["ShaderLab9"], fstFloor, mapTextures["bricks"]);

	}
	{
		glm::mat4 fstFloor = glm::mat4(1);
		fstFloor = glm::translate(fstFloor, glm::vec3(idx, 0.25f, -5));
		fstFloor = glm::scale(fstFloor, glm::vec3(2.88f, 0.25f, 2.88f));
		RenderSimpleMesh(meshes["box"], shaders["ShaderLab9"], fstFloor, mapTextures["bricks"]);
	}
	{
		glm::mat4 fstFloor = glm::mat4(1);
		fstFloor = glm::translate(fstFloor, glm::vec3(idx, 0.5f, -5));
		fstFloor = glm::scale(fstFloor, glm::vec3(2.55f, 0.25f, 2.55f));
		RenderSimpleMesh(meshes["box"], shaders["ShaderLab9"], fstFloor, mapTextures["bricks"]);
	}
	{
		glm::mat4 fstFloor = glm::mat4(1);
		fstFloor = glm::translate(fstFloor, glm::vec3(idx, 0.75f, -5));
		fstFloor = glm::scale(fstFloor, glm::vec3(2, 0.25f, 2));
		RenderSimpleMesh(meshes["box"], shaders["ShaderLab9"], fstFloor, mapTextures["bricks"]);
	}
	{
		glm::mat4 fstFloor = glm::mat4(1);
		fstFloor = glm::translate(fstFloor, glm::vec3(idx, 1, -5));
		fstFloor = glm::scale(fstFloor, glm::vec3(1.5f, 0.25f, 1.5f));
		RenderSimpleMesh(meshes["box"], shaders["ShaderLab9"], fstFloor, mapTextures["bricks"]);
	}
	{
		glm::mat4 fstFloor = glm::mat4(1);
		fstFloor = glm::translate(fstFloor, glm::vec3(idx, 1.25, -5));
		fstFloor = glm::scale(fstFloor, glm::vec3(1.33f, 0.25f, 1.33f));
		RenderSimpleMesh(meshes["box"], shaders["ShaderLab9"], fstFloor, mapTextures["bricks"]);
	}
	{
		glm::mat4 fstFloor = glm::mat4(1);
		fstFloor = glm::translate(fstFloor, glm::vec3(idx, 1.50, -5));
		fstFloor = glm::scale(fstFloor, glm::vec3(1, 0.25f, 1));
		RenderSimpleMesh(meshes["box"], shaders["ShaderLab9"], fstFloor, mapTextures["bricks"]);
	}
	
}
void runner::GenerateTemple() {
	{
		glm::mat4 fstFloor = glm::mat4(1);
		fstFloor = glm::translate(fstFloor, glm::vec3(0, 0, -20));
		fstFloor = glm::scale(fstFloor, glm::vec3(60, 50, 10));
		RenderSimpleMesh(meshes["square"], shaders["ShaderLab9"], fstFloor, mapTextures["wind"]);

	}
}
bool runner::Check4Collision(glm::mat4 box, float scale) {
	// factorul de scalare a cutiei este 0.5
	float x = max(box[3][0] - scale/2, min(playerPoints[0], box[3][0] + scale/2));
	float y = max(box[3][1] - scale/2, min(playerPoints[1], box[3][1] + scale/2));
	float z = max(box[3][2] - scale/2, min(playerPoints[2], box[3][2] + scale/2));

	float distance = sqrt((x - playerPoints[0]) * (x - playerPoints[0]) +
		(y - playerPoints[1]) * (y - playerPoints[1]) +
		(z - playerPoints[2]) * (z - playerPoints[2]));
	return distance < 0.25f;
}
int runner::CheckAllArray(glm::mat4 objs[], float scale_factor, int size) {
	for (int i = 0; i < size; i++) {
		if (Check4Collision(objs[i], scale_factor)) {
			return i;
		}
	}
	return -1;
}
void runner::Update(float deltaTimeSeconds)
{
	rotate_angle += deltaTimeSeconds * 1.5f;
	GeneratePlayer(deltaTimeSeconds);
	GeneratePlatform();
	GenerateWater(rotate_angle, -2);
	GenerateGrass();
	GenerateBoxes(deltaTimeSeconds);
	GeneratePyramid(-5);
	GeneratePyramid(5);
	GenerateTemple();
}

void runner::FrameEnd()
{
	//DrawCoordinatSystem();
}

void runner::RenderSimpleMesh(Mesh *mesh, Shader *shader, const glm::mat4 & modelMatrix, Texture2D* texture1, Texture2D* texture2)
{
	if (!mesh || !shader || !shader->GetProgramID())
		return;

	// render an object using the specified shader and the specified position
	glUseProgram(shader->program);

	// Bind model matrix
	GLint loc_model_matrix = glGetUniformLocation(shader->program, "Model");
	glUniformMatrix4fv(loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

	// Bind view matrix
	glm::mat4 viewMatrix = GetSceneCamera()->GetViewMatrix();
	int loc_view_matrix = glGetUniformLocation(shader->program, "View");
	glUniformMatrix4fv(loc_view_matrix, 1, GL_FALSE, glm::value_ptr(viewMatrix));

	// Bind projection matrix
	glm::mat4 projectionMatrix = GetSceneCamera()->GetProjectionMatrix();
	int loc_projection_matrix = glGetUniformLocation(shader->program, "Projection");
	glUniformMatrix4fv(loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

	if (texture1)
	{
		//TODO : activate texture location 0
		//TODO : Bind the texture1 ID
		//TODO : Send texture uniform value
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1->GetTextureID());
		GLint obj_text_0 = glGetUniformLocation(shader->program, "texture_1");
		glUniform1i(obj_text_0, 0);
	}

	if (texture2)
	{
		//TODO : activate texture location 1
		//TODO : Bind the texture2 ID
		//TODO : Send texture uniform value
		glActiveTexture(GL_TEXTURE1);

		glBindTexture(GL_TEXTURE_2D, texture2->GetTextureID());
		GLint obj_text_1 = glGetUniformLocation(shader->program, "texture_2");
		glUniform1i(obj_text_1, 1);
	}
	GLint mix_location = glGetUniformLocation(shader->program, "mix_quad");
	glUniform1i(mix_location, mix_flag);

	// Draw the object
	glBindVertexArray(mesh->GetBuffers()->VAO);
	glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_SHORT, 0);
}

// Documentation for the input functions can be found in: "/Source/Core/Window/InputController.h" or
// https://github.com/UPB-Graphics/Framework-EGC/blob/master/Source/Core/Window/InputController.h

void runner::OnInputUpdate(float deltaTime, int mods)
{
	float speed = 2;
	if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
	{
		glm::vec3 up = glm::vec3(0, 1, 0);
		glm::vec3 right = GetSceneCamera()->transform->GetLocalOXVector();
		glm::vec3 forward = GetSceneCamera()->transform->GetLocalOZVector();
		forward = glm::normalize(glm::vec3(forward.x, 0, forward.z));
	}
	if (press_m) {
		speed_platform1 += speed * deltaTime;
		speed_platform2 += speed * deltaTime;

	}
	if (press_space) {
		Jump();
	}
	if (window->KeyHold(GLFW_KEY_A)) {
		left_right -= speed * deltaTime;
	}
	if (window->KeyHold(GLFW_KEY_D)) {
		left_right += speed * deltaTime;
	}
}

void runner::OnKeyPress(int key, int mods)
{
	// add key press event
	if (key == GLFW_KEY_M) {
		press_m = true;
	}
	if (key == GLFW_KEY_SPACE) {
		press_space = true;
	}
}

void runner::OnKeyRelease(int key, int mods)
{
	// add key release event
	if (key == GLFW_KEY_SPACE) {
		press_space = false;
	}
}

void runner::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
	// add mouse move event
}

void runner::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button press event
}

void runner::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button release event
}

void runner::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}

void runner::OnWindowResize(int width, int height)
{
}
