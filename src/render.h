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

#include <allegro5/allegro.h>


class iRenderer
{
public:
	virtual ~iRenderer();
	virtual bool Init(int argc, char* argv[]) = 0;
	virtual void deInit(void) = 0;
	virtual void LoadModel(std::string path) = 0;
	virtual void LoadTexture(std::string path) = 0;
	virtual void DrawModel(const std::string &path, const std::string &texture, const Point3 &pos, const Point3 &forward, const Point3 &up, const float &scale) = 0;

	virtual void renderText(const std::string &text, float x, float y) = 0;
};

class Renderer : public iRenderer
{
public:
	virtual ~Renderer();
	bool Init(int argc, char* argv[]) override;
	void deInit(void) override;
	void LoadModel(std::string path) override;
	void LoadTexture(std::string path) override;
	void DrawModel(const std::string &path, const std::string &texture, const Point3 &pos, const Point3 &forward, const Point3 &up, const float &scale) override;

	void renderText(const std::string &text, float x, float y) override;

	ALLEGRO_DISPLAY* window;

	unsigned int screenWidth;
	unsigned int screenHeight;
};

#endif
