#include "imgui\imgui.h"
#include "imgui\imgui_impl_glfw.h"
#include "imgui\imgui_impl_opengl3.h"

#include "Graphics\window.h"
#include "Camera\camera.h"
#include "Shaders\shader.h"
#include "Model Loading\mesh.h"
#include "Model Loading\texture.h"
#include "Model Loading\meshLoaderObj.h"
#include <stdio.h>
#include <unordered_map>

/*
Ruse Cristian-Andrei
Marinescu Alexia
Ciucu Ovidiu
Feraru Andrei

group: 1232A
*/


void processInput ();


float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

Window window("Game Engine", 1920, 1080);
Camera camera;


glm::vec3 lightColor = glm::vec3(1.0f);
glm::vec3 lightPos = glm::vec3(-200.0f, 200.0f, -200.0f);



double lastX, currentX;
double lastY, currentY;
double deltaX, deltaY;

double settings_sensitivity = 1;
bool showPauseMenu = false;

float rotationAngle = 0;

bool dialogue1, dialogue2, dialogue3, dialogue4 = false;
bool closeDialogue5, closeDialogue6, closeDialogue7, closeDialogue8 = false;

std::string current_objective = "objective1";


bool block_movement = false;
int current_task = 1;




std::unordered_map<std::string, std::string> dialogues = {
	{"dialogue1", "Royal guard: Sir Cedric, due to your most recent mishap, we received a royal decree to exile you outside the castle garison. Of course, as you may expect, this comes with the obvious change in titulature, you will be stripped of your title, sir. From now onwards you will be addressed as Cedric, the peasent. Peasent Cedric, i wish upon you an easy life, well... however long it might be."},
	{"dialogue2", "Peasent Cedric ( to himself ): Damn it, i should've thought twice about doing what i have done. An exile sentence? Its a death sentence in disguise with all the remaiming magical corruption left in the land from the last war...Nowhere other than inside the castle is safe."},
	{"dialogue3", "The only good thing that came from that blasted war is that artificial sun which was created by the mages present for the war effort, after our natural sun randomly exploded.Its a bit weird that it doesn't move, unlike our lost natural sun, but at least it's not leting us freeze."},
	{"dialogue4", "I guess my only option is to try and do something to regain the royal favours...The only things that come to mind is to clear the corruption, it's either that or I'll be dead anyway.... Let's get started, I guess..."},
	{"dialogue5", "I remember hearing stories about a particular potion that can cure anything. It can probably stop this flower from corrupting the realm too."},
	{"dialogue6", "With all these ingredients i was able to create the potion. I should pour it on the Corrupting flower."},
	{"dialogue7",  "The Potion Seems to have worked, the Corrupting flower withered away."},
	{"dialogue8",  "Peasent Cedric, the realm hails your return!The land speaks of your triumph over the corruption.I restore your title of Sir for your unwavering courage and dedication.As a token of gratitude, I grant you the Wisdom of the Ancients, a deep understanding of our realm's secrets. Additionally, the bards will sing of your tale, spreading your legacy far and wide. Rise, Sir Cedric, a true hero returned. May your wisdom and legend inspire generations."},

};


std::unordered_map<std::string, std::string> questObjectives = {
	{"objective1", "Find the Corrupting Flower."},
	{"objective2", "Gather Bark from the first magical tree."},
	{"objective3", "Gather bark from the second magical tree."},
	{"objective4", "Gather mushroom stem fragments."},
	{"objective5", "Pour the potion on the corrupting flower."},
	{"objective6", "The King wants to speak with you, head to the castle."},
	{"objective7", "Speak to the king!"},
};



void RenderQuestGUI(const std::string& objectiveKey, int currentTask) {
	// Check if the objective key exists
	auto it = questObjectives.find(objectiveKey);
	if (it != questObjectives.end()) {

		ImVec2 windowSize(window.getWidth() * 0.25f, window.getHeight() * 0.1f);

		ImGui::SetNextWindowSize(windowSize, ImGuiCond_Always);

		ImVec2 centerPos(window.getWidth() * 0.05f, window.getHeight() * 0.05f);
		ImGui::SetNextWindowPos(centerPos, ImGuiCond_Always);

		ImGui::Begin("Quest Information", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);

		// Display task progress
		ImGui::Text("Task %d/%d", currentTask, 7);

		// Display the quest objective text
		ImGui::TextWrapped("%s", it->second.c_str());

		ImGui::End();
	}
	else {

		ImVec2 windowSize(window.getWidth() * 0.25f, window.getHeight() * 0.1f);

		ImGui::SetNextWindowSize(windowSize, ImGuiCond_Always);

		ImVec2 centerPos(window.getWidth() * 0.05f, window.getHeight() * 0.05f);
		ImGui::SetNextWindowPos(centerPos, ImGuiCond_Always);

		ImGui::Begin("Quest Information", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);


		// Display the quest objective text
		ImGui::TextWrapped("Thank you for Playing!");

		ImGui::End();
	}
}



void RenderDialogueMenu(const std::string& dialogueKey) {
	// Check if the dialogue key exists
	auto it = dialogues.find(dialogueKey);
	if (it != dialogues.end()) {
		// Display the dialogue box

		window.enableCursor();
        ImVec2 windowSize(window.getWidth() * 0.8f, window.getHeight() * 0.25f);

        ImGui::SetNextWindowSize(windowSize, ImGuiCond_Always);

		ImVec2 centerPos(window.getWidth() * 0.1f, window.getHeight() * 0.15f);
        ImGui::SetNextWindowPos(centerPos, ImGuiCond_Always);

		ImGui::Begin("Dialogue Box", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);

		ImGui::TextWrapped("%s", it->second.c_str());

		if (ImGui::Button("OK")) {
			ImGui::End();
			if (dialogueKey == "dialogue1") {
				dialogue1 = true;
			}
			if (dialogueKey == "dialogue2") {
				dialogue2 = true;
			}
			if (dialogueKey == "dialogue3") {
				dialogue3 = true;
			}
			if (dialogueKey == "dialogue4") {
				dialogue4 = true;
			}
			if (dialogueKey == "dialogue5") {
				closeDialogue5 = true;
			}

			if (dialogueKey == "dialogue6") {
				closeDialogue6 = true;
			}


			if (dialogueKey == "dialogue7") {
				closeDialogue7 = true;
			}


			if (dialogueKey == "dialogue8") {
				closeDialogue8 = true;
			}

			window.disableCursor();
			block_movement = false;
			return;
		}

		ImGui::End();
	}
	else {
		ImGui::TextWrapped("Dialogue not found for key: %s", dialogueKey.c_str());
	}
}



void RenderInteractButton() {
	// Check if the objective key exists


	ImVec2 windowSize(window.getWidth() * 0.15f, window.getHeight() * 0.1f);

	ImGui::SetNextWindowSize(windowSize, ImGuiCond_Always);

	ImVec2 centerPos(window.getWidth() * 0.45f, window.getHeight() * 0.85f);
	ImGui::SetNextWindowPos(centerPos, ImGuiCond_Always);

	ImGui::Begin("Info", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);

	// Display task progress
	ImGui::Text("Press E To Interact");


	ImGui::End();

}



int main()
{
	// Initialize ImGui
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	const char* fontPath = "imgui/finalf.ttf";
	ImGuiIO& io = ImGui::GetIO();
	io.Fonts->AddFontFromFileTTF(fontPath, 24);
	ImGui::StyleColorsDark();

	// Setup Platform/Renderer bindings
	ImGui_ImplGlfw_InitForOpenGL(window.getWindow(), true);
	ImGui_ImplOpenGL3_Init("#version 330");


	glClearColor(0.2f, 0.8f, 1.0f, 1.0f);

	//building and compiling shader program
	Shader shader("Shaders/vertex_shader.glsl", "Shaders/fragment_shader.glsl");
	Shader sunShader("Shaders/sun_vertex_shader.glsl", "Shaders/sun_fragment_shader.glsl");
	Shader corruptionShader("Shaders/vertex_shader.glsl", "Shaders/post_processing_shader.glsl");





	//Textures
	GLuint tex = loadBMP("Resources/Textures/orange.bmp");
	GLuint tex5 = loadBMP("Resources/Textures/tree_texture.bmp");
	GLuint tex6 = loadBMP("Resources/Textures/mountain_texture.bmp");
	GLuint tex7 = loadBMP("Resources/Textures/tree_texture.bmp");
	GLuint tex8 = loadBMP("Resources/Textures/castle_texture.bmp");
	GLuint tex9 = loadBMP("Resources/Textures/magic_Mushroom_texture.bmp");
	GLuint tex10 = loadBMP("Resources/Textures/magic_tree_1_texture.bmp");
	GLuint tex11 = loadBMP("Resources/Textures/magic_tree_2_texture.bmp");
	GLuint tex12 = loadBMP("Resources/Textures/corrupted_flower_texture.bmp");
	GLuint tex13 = loadBMP("Resources/Textures/corrupted_flower_texture.bmp");
	GLuint tex14 = loadBMP("Resources/Textures/king_texture.bmp");
	GLuint skysphereTexture = loadBMP("Resources/Textures/skysphere.bmp");



	glEnable(GL_DEPTH_TEST);

	std::vector<Texture> textures;
	textures.push_back(Texture());
	textures[0].id = tex;
	textures[0].type = "texture_diffuse";

	std::vector<Texture> textures1;
	textures1.push_back(Texture());
	textures1[0].id = skysphereTexture;
	textures1[0].type = "texture_diffuse";

	std::vector<Texture> textures5;
	textures5.push_back(Texture());
	textures5[0].id = tex5;
	textures5[0].type = "texture_diffuse";

	std::vector<Texture> textures6;
	textures6.push_back(Texture());
	textures6[0].id = tex6;
	textures6[0].type = "texture_diffuse";


	std::vector<Texture> textures7;
	textures7.push_back(Texture());
	textures7[0].id = tex7;
	textures7[0].type = "texture_diffuse";


	std::vector<Texture> textures8;
	textures8.push_back(Texture());
	textures8[0].id = tex8;
	textures8[0].type = "texture_diffuse";


	std::vector<Texture> textures9;
	textures9.push_back(Texture());
	textures9[0].id = tex9;
	textures9[0].type = "texture_diffuse";


	std::vector<Texture> textures10;
	textures10.push_back(Texture());
	textures10[0].id = tex10;
	textures10[0].type = "texture_diffuse";


	std::vector<Texture> textures11;
	textures11.push_back(Texture());
	textures11[0].id = tex11;
	textures11[0].type = "texture_diffuse";




	std::vector<Texture> textures12;
	textures12.push_back(Texture());
	textures12[0].id = tex12;
	textures12[0].type = "texture_diffuse";



	std::vector<Texture> textures13;
	textures13.push_back(Texture());
	textures13[0].id = tex13;
	textures13[0].type = "texture_diffuse";



	std::vector<Texture> textures14;
	textures14.push_back(Texture());
	textures14[0].id = tex14;
	textures14[0].type = "texture_diffuse";


	MeshLoaderObj loader;
	Mesh sun = loader.loadObj("Resources/Models/sphere.obj");
	Mesh skySphere = loader.loadObj("Resources/Models/sphere.obj", textures1);
	Mesh trees = loader.loadObj("Resources/Models/trees.obj", textures5);
	Mesh mountain = loader.loadObj("Resources/Models/mountain.obj", textures6);
	Mesh grass_plane = loader.loadObj("Resources/Models/grass_plane.obj", textures7);
	Mesh castle = loader.loadObj("Resources/Models/castle.obj", textures8);
	Mesh castle_interior = loader.loadObj("Resources/Models/castle_interior.obj", textures8);
	Mesh magic_mushroom = loader.loadObj("Resources/Models/magic_mushroom.obj", textures9);
	Mesh magic_tree_1 = loader.loadObj("Resources/Models/magic_tree_1.obj", textures10);
	Mesh magic_tree_2 = loader.loadObj("Resources/Models/magic_tree_2.obj", textures11);
	Mesh corrupted_flower = loader.loadObj("Resources/Models/corrupted_flower.obj", textures12);
	Mesh castle_door = loader.loadObj("Resources/Models/castle_door.obj", textures13);
	Mesh king = loader.loadObj("Resources/Models/king.obj", textures14);

	while (!window.isPressed(GLFW_KEY_ESCAPE) &&
		glfwWindowShouldClose(window.getWindow()) == 0)
	{
		
		window.clear();
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		currentX = window.getxPos();
		currentY = window.getyPos();
		processInput();



		rotationAngle = rotationAngle + 0.005f;



		//render sun object
		sunShader.use();
		glm::mat4 ProjectionMatrix = glm::perspective(90.0f, window.getWidth() * 1.0f / window.getHeight(), 0.1f, 10000.0f);
		glm::mat4 ViewMatrix = glm::lookAt(camera.getCameraPosition(), camera.getCameraPosition() + camera.getCameraViewDirection(), camera.getCameraUp());
		GLuint MatrixID = glGetUniformLocation(sunShader.getId(), "MVP");
		glm::mat4 ModelMatrix = glm::mat4(1.0);
		ModelMatrix = glm::translate(ModelMatrix, lightPos);
		glm::mat4 MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
		sun.draw(sunShader);
		shader.use();



		//render skysphere object
		GLuint MatrixID2 = glGetUniformLocation(shader.getId(), "MVP");
		GLuint ModelMatrixID = glGetUniformLocation(shader.getId(), "model");
		ModelMatrix = glm::mat4(1.0);
		ModelMatrix = glm::translate(ModelMatrix, glm::vec3(0.0f, 0.0f, 0.0f));
		ModelMatrix = glm::scale(ModelMatrix, glm::vec3(150.0f, 150.0f, 150.0f));
		ModelMatrix = glm::rotate(ModelMatrix, rotationAngle, glm::vec3(1.0f, 0.0f, 0.0));
		MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
		glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
		glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
		skySphere.draw(shader);



		ModelMatrix = glm::mat4(1.0);
		ModelMatrix = glm::translate(ModelMatrix, glm::vec3(0.0f, 0.0f, 0.0f));
		MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
		glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
		glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
		glUniform3f(glGetUniformLocation(shader.getId(), "lightColor"), lightColor.x, lightColor.y, lightColor.z);
		glUniform3f(glGetUniformLocation(shader.getId(), "lightPos"), lightPos.x, lightPos.y, lightPos.z);
		glUniform3f(glGetUniformLocation(shader.getId(), "viewPos"), camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);



		ModelMatrix = glm::mat4(1.0);
		ModelMatrix = glm::translate(ModelMatrix, glm::vec3(0.0f, -10.0f, 0.0f));
		ModelMatrix = glm::scale(ModelMatrix, glm::vec3(60.0f));  // Adjust the scale accordingly
		MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
		glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
		glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
		trees.draw(shader);



		ModelMatrix = glm::mat4(1.0);
		ModelMatrix = glm::translate(ModelMatrix, glm::vec3(0.0f, -10.0f, 0.0f));
		ModelMatrix = glm::scale(ModelMatrix, glm::vec3(60.0f));  // Adjust the scale accordingly
		MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
		glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
		glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
		mountain.draw(shader);



		ModelMatrix = glm::mat4(1.0);
		ModelMatrix = glm::translate(ModelMatrix, glm::vec3(0.0f, -10.0f, 0.0f));
		ModelMatrix = glm::scale(ModelMatrix, glm::vec3(60.0f));  // Adjust the scale accordingly
		MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
		glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
		glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
		grass_plane.draw(shader);


		ModelMatrix = glm::mat4(1.0);
		ModelMatrix = glm::translate(ModelMatrix, glm::vec3(0.0f, -10.0f, 0.0f));
		ModelMatrix = glm::scale(ModelMatrix, glm::vec3(60.0f));  // Adjust the scale accordingly
		MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
		glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
		glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
		castle.draw(shader);


		ModelMatrix = glm::mat4(1.0);
		ModelMatrix = glm::translate(ModelMatrix, glm::vec3(0.0f, -10.0f, 0.0f));
		ModelMatrix = glm::scale(ModelMatrix, glm::vec3(60.0f));  // Adjust the scale accordingly
		MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
		glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
		glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
		magic_mushroom.draw(shader);




		ModelMatrix = glm::mat4(1.0);
		ModelMatrix = glm::translate(ModelMatrix, glm::vec3(0.0f, -10.0f, 0.0f));
		ModelMatrix = glm::scale(ModelMatrix, glm::vec3(60.0f));  // Adjust the scale accordingly
		MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
		glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
		glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
		magic_tree_1.draw(shader);



		ModelMatrix = glm::mat4(1.0);
		ModelMatrix = glm::translate(ModelMatrix, glm::vec3(0.0f, -10.0f, 0.0f));
		ModelMatrix = glm::scale(ModelMatrix, glm::vec3(60.0f));  // Adjust the scale accordingly
		MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
		glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
		glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
		magic_tree_2.draw(shader);


		ModelMatrix = glm::mat4(1.0);
		ModelMatrix = glm::translate(ModelMatrix, glm::vec3(0.0f, -10.0f, 0.0f));
		ModelMatrix = glm::scale(ModelMatrix, glm::vec3(60.0f));  // Adjust the scale accordingly
		MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
		glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
		glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
		
		if (current_objective != "objective6" && current_objective != "objective7") {
			corrupted_flower.draw(shader);
		}

		ModelMatrix = glm::mat4(1.0);
		ModelMatrix = glm::translate(ModelMatrix, glm::vec3(0.0f, -10.0f, 0.0f));
		ModelMatrix = glm::scale(ModelMatrix, glm::vec3(60.0f));  // Adjust the scale accordingly
		MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
		glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
		glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
		king.draw(shader);


		ModelMatrix = glm::mat4(1.0);
		ModelMatrix = glm::translate(ModelMatrix, glm::vec3(0.0f, -10.0f, 0.0f));
		ModelMatrix = glm::scale(ModelMatrix, glm::vec3(60.0f));  // Adjust the scale accordingly
		MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
		glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
		glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
		castle_interior.draw(shader);

		
		/*Information Dialogues + Quest Task Start*/
		if (dialogue1 != true) {
			block_movement = true;
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();
			RenderDialogueMenu("dialogue1");
			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		}

		else if (dialogue1 == true && dialogue2 == false && dialogue3 == false) {
			block_movement = true;
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();
			RenderDialogueMenu("dialogue2");
			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		}

		else if (dialogue1 == true && dialogue2 == true && dialogue3 == false) {
			block_movement = true;
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();
			RenderDialogueMenu("dialogue3");
			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		}
		else if (dialogue1 == true && dialogue2 == true && dialogue3 == true && dialogue4 == false) {
			block_movement = true;
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();
			RenderDialogueMenu("dialogue4");
			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		}
		else if (dialogue1 == true && dialogue2 == true && dialogue3 == true && dialogue4 == true) {
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();
			RenderQuestGUI(current_objective, current_task);

			// Corrupting Flower
			glm::vec3 cameraPosition = camera.getCameraPosition();
			if ((cameraPosition.x >= 20.0f && cameraPosition.x <= 50.0f) &&
				(cameraPosition.z <= -90.0f && cameraPosition.z >= -120.0f && (current_task == 1 || current_task == 5))) {
				RenderInteractButton();
				if (window.isPressed(GLFW_KEY_E))
				{
					if (current_task == 1) {
						current_task++;
						current_objective = "objective2";
					}
					else {
						current_task++;
						current_objective = "objective6";
					}

				}
			}

			//Magical Tree 1
			if ((cameraPosition.x <= -180.0f && cameraPosition.x >= -240.0f) &&
				(cameraPosition.z <= -180.0f && cameraPosition.z >= -230.0f && current_task == 2)) {
				RenderInteractButton();
				if (window.isPressed(GLFW_KEY_E))
				{
					current_task++;
					current_objective = "objective3";


				}
			}


			//Magical Tree 2
			if ((cameraPosition.x >= 250.0f && cameraPosition.x <= 280.0f) &&
				(cameraPosition.z <= -230.0f && cameraPosition.z >= -270.0f && current_task == 3)) {
				RenderInteractButton();
				if (window.isPressed(GLFW_KEY_E))
				{
					current_task++;
					current_objective = "objective4";


				}
			}

			//Magical Mushroom	
			if ((cameraPosition.x >= 0.0f && cameraPosition.x <= 40.0f) &&
				(cameraPosition.z >= 120.0f && cameraPosition.z <= 150.0f && current_task == 4)) {
				RenderInteractButton();
				if (window.isPressed(GLFW_KEY_E))
				{
					current_objective = "objective5";
					current_task++;


				}
			}

			//Castle Door
			if ((cameraPosition.x >= 180.0f && cameraPosition.x <= 200.0f) &&
				(cameraPosition.z >= 44.0f && cameraPosition.z <= 55.0f && current_task == 6)) {
				RenderInteractButton();
				if (window.isPressed(GLFW_KEY_E))
				{
					current_task++;
					current_objective = "objective7";
					camera.setCameraPosition(glm::vec3(-1970.0f, 0.0f, 50.0f));
					//camera.setFront(glm::vec3(-0.02f, -0.03f, -0.9f));


				}
			}


			//King	
			if ((cameraPosition.x <= -1940.0f && cameraPosition.x >= -1970.0f) &&
				(cameraPosition.z <= -20.0f && cameraPosition.z >= -50.0f && current_task == 7)) {
				RenderInteractButton();
				if (window.isPressed(GLFW_KEY_E))
				{
					current_objective = "objective8";
					current_task++;


				}
			}


			if (closeDialogue5 != true && current_objective == "objective2") {
				block_movement = true;
				RenderDialogueMenu("dialogue5");

			}

			if (closeDialogue6 != true && current_objective == "objective5") {
				block_movement = true;
				RenderDialogueMenu("dialogue6");

			}

			if (closeDialogue7 != true && current_objective == "objective6") {
				block_movement = true;
				RenderDialogueMenu("dialogue7");

			}

			if (closeDialogue8 != true && current_objective == "objective8") {
				block_movement = true;
				RenderDialogueMenu("dialogue8");

			}

			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		}
		
		window.update();
	}


	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

void processInput()
{
	float cameraSpeed = 100 * deltaTime;
	

	if (block_movement == false){



		//translation
		if (window.isPressed(GLFW_KEY_W))
			camera.keyboardMoveFront(cameraSpeed);
		if (window.isPressed(GLFW_KEY_S))
			camera.keyboardMoveBack(cameraSpeed);
		if (window.isPressed(GLFW_KEY_A))
			camera.keyboardMoveLeft(cameraSpeed);
		if (window.isPressed(GLFW_KEY_D))
			camera.keyboardMoveRight(cameraSpeed);
		if (window.isPressed(GLFW_KEY_R))
			camera.keyboardMoveUp(cameraSpeed);
		if (window.isPressed(GLFW_KEY_F))
			camera.keyboardMoveDown(cameraSpeed);


		float cameraSpeed = 100 * deltaTime * settings_sensitivity;
		float originalCameraSpeed = 50 * deltaTime * settings_sensitivity;


		if (lastX == NULL) {
			lastX = currentX;
		}
		if (lastY == NULL) {
			lastY = currentY;
		}
		double deltaX = currentX - lastX;
		double deltaY = currentY - lastY;


		if (deltaX > 0) {
			camera.rotateOy(-cameraSpeed);
		}
		else if (deltaX < 0) {
			camera.rotateOy(cameraSpeed);
		}
		if (deltaY > 0) {
			camera.rotateOx(-cameraSpeed);
		}
		else if (deltaY < 0) {
			camera.rotateOx(cameraSpeed);
		}

		lastX = currentX;
		lastY = currentY;





		camera.update(deltaTime * 5);
	}
}

