#ifndef RENDERER_H
#define RENDERER_H

#include <string>
#include <iostream>
#include <vector>
#include <map>
#include <tuple>

#include <GL/glew.h>

#include "point3.h"
#include "utils.h"

#include <GLFW/glfw3.h>

#include <allegro5/allegro.h>

namespace Renderer
{

	bool Init(int argc, char* argv[]);
	void deInit(void);
	void LoadModel(std::string path);
	void LoadTexture(std::string path);
	void DrawModel(const std::string &path, const std::string &texture, const Point3 &pos, const Point3 &forward, const Point3 &up, const float &scale);

	void renderText(const std::string &text, float x, float y);

//	static Point3 translate(Point3 point, Point3 angles);

	extern ALLEGRO_DISPLAY* window;

	extern unsigned int screenWidth;
	extern unsigned int screenHeight;


//	extern std::map<std::string, Model> models;
//	extern std::map<std::string, unsigned int> textures;

};

#endif
