#pragma once
#include <Component/SimpleScene.h>
#include <Component/Transform/Transform.h>
#include <Core/GPU/Mesh.h>

class runner : public SimpleScene
{
	public:
		runner();
		~runner();

		void Init() override;

	private:
		void FrameStart() override;
		void Update(float deltaTimeSeconds) override;
		void FrameEnd() override;

		void RenderSimpleMesh(Mesh *mesh, Shader *shader, const glm::mat4 &modelMatrix, Texture2D* texture1 = NULL, Texture2D* texture2 = NULL);
		void GeneratePlayer(float d);
		void Jump();
		void GeneratePlatform();
		void GenerateWater(float d, float r);
		void GenerateGrass();
		void GenerateBoxes(float delta);
		void GeneratePyramid(int i);
		void GenerateTemple();
		bool Check4Collision(glm::mat4 s, float scale);
		int CheckAllArray(glm::mat4 objs[], float scale_factor, int size);
		int getRandomNumber(int min, int max);
		void OnInputUpdate(float deltaTime, int mods) override;
		void OnKeyPress(int key, int mods) override;
		void OnKeyRelease(int key, int mods) override;
		void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
		void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
		void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
		void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
		void OnWindowResize(int width, int height) override;

		glm::vec3 playerPoints, fstplatformPoints, sndplatformPoints;
		glm::mat4 this_boxes[3];
		glm::mat4 this_water[3];
		std::unordered_map<std::string, Texture2D*> mapTextures;
		GLuint randomTextureID;
		int mix_flag = 0;
		float scale_factor = 7.5f;
		float press_space = false;
		float press_m = false;
		float left_right = 0.0;
		int playersLife = 5;
		float render_water = 0.0;
		float speed_platform1 = 0;
		float speed_platform2 = 0;
		float start_platform1 = -(scale_factor / 2);
		float start_platform2 = -scale_factor - (scale_factor/2 + 1);
		float delay_boxes = 0.0;
		float delay_water = 0.0;
		bool in_air = false;
		float rotate_angle = 0;
		float upward_speed = 0;
		int GRAVITY = -20;
		int JUMP_POWER = 40;
		float this_z = 0.0;
		bool flag = false;
};
