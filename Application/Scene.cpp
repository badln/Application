/*

#include "Scene.h"
#include "main.h"
#include "EngineResources/Console.h"

//TODO: Create a scene class, when scene class's "Save()" function is run, take all objects and write them to a Scene.sc file.

WindowConsole SceneConsole;

std::string currentScene = "Null";
std::string parameter(std::string str)
{
	return str.substr(0, str.find_first_of(" "));
}
std::string thisLineValue(std::string scene)
{
	return scene.substr(scene.find_first_of(" ") + 1, scene.size());
}
bool str_bool(std::string string)
{
	if (string == "true" || string == "True") {
		return true;
	}
	return false;
}

int ParseInt(std::string string)
{
	std::istringstream str(string);
	int rVal;
	str >> rVal;
	return rVal;
}
float ParseFloat(std::string string)
{
	std::istringstream str(string);
	float rVal;
	str >> rVal;
	return rVal;
}
vec4 ParseVector(std::string vector)
{
	float x, y, z, w;
	std::string x_s, y_s, z_s, w_s = "1.0";

	x_s = vector.substr(0, vector.find_first_of("f"));

	vector = vector.substr(vector.find_first_of("f") + 3, vector.size());
	y_s = vector.substr(0, vector.find_first_of("f"));

	vector = vector.substr(vector.find_first_of("f") + 3, vector.size());
	z_s = vector.substr(0, vector.find_first_of("f"));

	try {
		vector = vector.substr(vector.find_first_of("f") + 3, vector.size());
		w_s = vector.substr(0, vector.find_first_of("f"));
	}
	catch (std::exception e) {}

	std::istringstream strx(x_s);
	strx >> x;
	std::istringstream stry(y_s);
	stry >> y;
	std::istringstream strz(z_s);
	strz >> z;
	std::istringstream strw(w_s);
	strw >> w;

	return vec4(x, y, z, w);
}
void ReadScene(std::string scenePath)
{
	std::ifstream file(scenePath);
	std::string scene;
	int objects = 1;
	if (file)
	{
		std::string modelPath = "n";
		ObjContainer* newObj = NULL;
		Camera* camera = NULL;

		bool CamCreator = false;

		while (std::getline(file, scene))
		{
			try {
				std::string par = parameter(scene);
				std::string tlv = thisLineValue(scene);
				// TODO : must be a better way of doing this
				if (scene.substr(1, 6) == "CAMERA")
				{
					camera = new Camera;
					worldCameras.push_back(camera);
				}
				else if (par == "cm")
					camera->name(tlv);

				else if (par == "cmp")
					camera->Position = ParseVector(tlv);

				else if (par == "cmcc")
					camera->clearColour = ParseVector(tlv);

				else if (par == "cmfov")
					camera->fov = ParseFloat(tlv);

				else if (par == "cmy") 
					camera->yaw = ParseFloat(tlv); 

				else if (par == "cmm" && str_bool(tlv) == true)
					camera->SetMainCamera();

				else if (par == "cmpt")
					camera->pitch = ParseFloat(tlv);

				else if (par == "cmfcp")
					camera->farClipPlane = ParseFloat(tlv);

				else if (par == "cmncp")
					camera->nearClipPlane = ParseFloat(tlv);

				else if (scene.substr(1, 6) == "OBJECT") {
					newObj = new ObjContainer;
					SceneConsole.Log("Creating object #" + std::to_string(objects) + ", '" + tlv + "'");
					objects++;
				}
				else if (par == "o")
					newObj->name(tlv.c_str());

				else if (par == "m")
					modelPath = tlv;
				else if (par == "c")
					newObj->renderer.material.culling = ParseInt(tlv);

				else if (par == "ma")
					newObj->renderer.material.ambient = ParseVector(tlv);

				else if (par == "md")
					newObj->renderer.material.diffuse = ParseVector(tlv);

				else if (par == "mdt") {
					std::string str = tlv;
					if (str != "none")
						newObj->renderer.material.diffuseTex = Texture(str, TextureType::Diffuse);
				}

				else if (par == "ms")
					newObj->renderer.material.specular = ParseVector(tlv);

				else if (par == "mst") {
					std::string str = tlv;
					if (str != "none")
						newObj->renderer.material.specularTex = Texture(str, TextureType::Specular);
				}

				else if (par == "me")
					newObj->renderer.material.emissive = ParseInt(tlv);

				else if (par == "met") {
					std::string str = tlv;
					if (str != "none")
						newObj->renderer.material.emissiveTex = Texture(str, TextureType::Emissive);
				}

				else if (par == "msh")
					newObj->renderer.material.shininess = ParseFloat(tlv);

				else if (par == "mc") 
					newObj->renderer.material.colour = ParseVector(tlv);

				else if (par == "mt") {
					std::string str = tlv;
					if (str != "none")
						newObj->renderer.material.texture = Texture(str, TextureType::Texture2D);
				}
				else if (par == "mtf" && modelPath != "n")
					newObj->SetModel(modelPath, str_bool(scene.substr(scene.find_first_of(" ") + 1, scene.size())));

				else if (par == "p")
					newObj->transform.position = ParseVector(tlv);

				else if (par == "s")
					newObj->transform.scale = ParseVector(tlv);

				else if (par == "r")
					newObj->transform.rotation = ParseVector(tlv);

				else if (par == "l")
					newObj->light.enabled = str_bool(tlv);

				else if (par == "rq")
					newObj->light.renderQueue = ParseInt(tlv);

				else if (par == "lin")
					newObj->light.linear = ParseFloat(tlv);

				else if (par == "q")
					newObj->light.quadratic = ParseFloat(tlv);

				else if (par == "cnst")
					newObj->light.constant = ParseFloat(tlv);

				else if (par == "co")
					newObj->light.cutoff = ParseFloat(tlv);

				else if (par == "oco")
					newObj->light.outerCutoff = ParseFloat(tlv);

				else if (par == "t")
					newObj->light.type = ParseInt(tlv);

				else if (par == "d")
					newObj->light.direction = ParseVector(tlv);

				else if (par == "lc")
					newObj->light.colour = ParseVector(tlv);

				else if (par == "lp")
					newObj->light.position = ParseVector(tlv);

				else if (par == "la")
					newObj->light.ambient = ParseVector(tlv);

				else if (par == "ld")
					newObj->light.diffuse = ParseVector(tlv);

				else if (par == "ls")
					newObj->light.specular = ParseVector(tlv);

				else if (par == "sh") {
					for (int i = 0; i < shaders.size(); i++)
					{
						if (shaders[i]->name() == tlv) {
							newObj->renderer.material.shader = shaders[i];
						}
					}
				}
				else if (scene == "##END") {
					SceneConsole.Log("Scene loaded");
					break;
				}
			}
			catch (std::exception e) {}
		}

	}
	file.close();

}
void SetActiveScene(std::string scenePath)
{
	currentScene = scenePath.substr(scenePath.find_last_of("/") + 1, scenePath.substr().length());
	currentScene = currentScene.substr(0, currentScene.find_last_of("."));
	Scene.name(currentScene.c_str());
	for (int i = 1; i < objects.size(); i++)
	{
		objects[i]->Destroy;
	}
	ReadScene(scenePath);
	for (int i = 0; i < worldCameras.size(); i++)
		worldCameras[i]->SetCameraDir();
}*/