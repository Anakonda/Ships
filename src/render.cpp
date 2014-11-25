#include <fstream>
#include <sstream>

#include <cmath>

#include <SOIL/SOIL.h>

#include <ft2build.h>
#include FT_FREETYPE_H

#include <allegro5/allegro.h>

#include "render.h"

#include <OGLFT.h>
#include <allegro5/allegro_opengl.h>

ALLEGRO_DISPLAY* Renderer::window;

unsigned int Renderer::screenWidth;
unsigned int Renderer::screenHeight;

struct Model
{
public:
	unsigned int vertexbuffer;
	unsigned int normalbuffer;
	unsigned int elementbuffer;
	unsigned int uvbuffer;
	unsigned int vao;
	unsigned int shaderProgram;
	unsigned int vertexShaderObject;
	unsigned int fragmentShaderObject;

	std::vector<float> vertices;
	std::vector<unsigned short> elements;
	std::vector<float> uvcoords;
	std::vector<float> normals;
};

std::map<std::string, Model> models;
std::map<std::string, unsigned int> textures;

OGLFT::Filled* font;

bool Renderer::Init(int argc, char* argv[])
{
	if(!al_init())
	{
		std::cout << "Failed to initialize allegro." << std::endl;
		return false;
	}

	al_set_new_display_flags(ALLEGRO_WINDOWED | ALLEGRO_RESIZABLE | ALLEGRO_OPENGL | ALLEGRO_OPENGL_3_0);

	Renderer::window = al_create_display(640, 480);

	Renderer::screenWidth = 640;
	Renderer::screenHeight = 480;
	if (!Renderer::window)
	{
		std::cout << "Failed to create window." << std::endl;;
		return false;
	}

	std::string filename = "res/LiberationSans-Regular.ttf";
	font = new OGLFT::Filled(filename.c_str(), 14);

	if (font == nullptr || !font->isValid())
	{
		std::cout << "Could not construct font from " << filename << std::endl;
		return false;
	}
	font->setForegroundColor(1, 1, 1);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	al_set_target_backbuffer(Renderer::window);
	al_set_current_opengl_context(Renderer::window);

	glewExperimental = GL_TRUE;
	glewInit();

	glEnable(GL_TEXTURE_2D);

	float global_ambient[] = {1, 1, 1, 1};
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, global_ambient);

	glEnable(GL_LIGHTING);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	glCullFace(GL_FRONT_AND_BACK);

	glMatrixMode(GL_MODELVIEW);

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);

	return true;
}

void Renderer::deInit(void)
{
	for(auto &texture : textures)
	{
		glDeleteTextures(1, &texture.second);
	}
	for(auto &model : models)
	{
		glDeleteBuffers(1, &model.second.vertexbuffer);
		glDeleteBuffers(1, &model.second.normalbuffer);
		glDeleteBuffers(1, &model.second.elementbuffer);
		glDeleteBuffers(1, &model.second.uvbuffer);
		glDeleteVertexArrays(1, &model.second.vao);
		glDeleteProgram(model.second.shaderProgram);
		glDeleteShader(model.second.vertexShaderObject);
		glDeleteShader(model.second.fragmentShaderObject);
	}

	delete font;
	al_destroy_display(Renderer::window);
}

void Renderer::LoadTexture(std::string path)
{
	unsigned int textureID = SOIL_load_OGL_texture(path.c_str(), SOIL_LOAD_AUTO, 0, SOIL_FLAG_POWER_OF_TWO | SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y);
    textures.insert(std::pair<std::string, unsigned int>(path, textureID));
}

void Renderer::LoadModel(std::string path)
{
	std::vector<Point3> tempuv;
	std::vector<Point3> tempnormals;
	std::vector<Point3> tempvertices;

	std::ifstream infile(path);

	if(!infile.good())
	{
		std::cout << "can not open file "<<path<<std::endl;
		return;
	}

	Model model;

	std::string line;
	while (std::getline(infile, line))
	{
		if (line.substr(0,2) == "v ")
		{
			std::istringstream s(line.substr(2));
			Point3 v;
			s >> v.x;
			s >> v.y;
			s >> v.z;
			tempvertices.push_back(v);
		}
		else if(line.substr(0,3) == "vn ")
		{
			std::istringstream s(line.substr(3));
			Point3 point;
			s >> point.x;
			s >> point.y;
			s >> point.z;
			tempnormals.push_back(point);
		}
		else if(line.substr(0,3) == "vt ")
		{
			std::istringstream s(line.substr(3));
			Point3 point;
			s >> point.x;
			s >> point.y;
			s >> point.z;
			tempuv.push_back(point);
		}
		else if(line.substr(0,2) == "f ")
		{
			auto points = utils::splitString(line.substr(2), ' ');
			//Face face;
			for(auto &point : points)
			{
				unsigned int i = utils::countCharacters(point, '/');
				if(i == 2)
				{
					auto parts = utils::splitString(point, '/');
					if(parts.size() == 3)
					{
						unsigned int verticeID = utils::toInt(parts.at(0)) - 1;
						unsigned int texCoordID = utils::toInt(parts.at(1)) - 1;
						unsigned int normalID = utils::toInt(parts.at(2)) - 1;

						model.vertices.push_back(tempvertices.at(verticeID).x);
						model.vertices.push_back(tempvertices.at(verticeID).y);
						model.vertices.push_back(tempvertices.at(verticeID).z);

						model.uvcoords.push_back(tempuv.at(texCoordID).x);
						model.uvcoords.push_back(tempuv.at(texCoordID).y);

						model.normals.push_back(tempnormals.at(normalID).x);
						model.normals.push_back(tempnormals.at(normalID).y);
						model.normals.push_back(tempnormals.at(normalID).z);

						//model.uvcoords.push_back(tempuv.at(utils::toInt(parts.at(1)) - 1).z);
					}
				}
			}
		}
	}

	model.vertexShaderObject = glCreateShader(GL_VERTEX_SHADER);
	model.fragmentShaderObject = glCreateShader(GL_FRAGMENT_SHADER);

	std::string vertexString = utils::readFile("shaders/vertexshader.txt");
	std::string fragmentString = utils::readFile("shaders/fragmentshader.txt");
	const char* vertexCString = vertexString.c_str();
	int vertexStringLen = vertexString.length();
	const char* fragmentCString = fragmentString.c_str();
	int fragmentStringLen = fragmentString.length();

	glShaderSource(model.vertexShaderObject, 1, &vertexCString, &vertexStringLen);
	glShaderSource(model.fragmentShaderObject, 1, &fragmentCString, &fragmentStringLen);

	glCompileShader(model.vertexShaderObject);
	glCompileShader(model.fragmentShaderObject);

	model.shaderProgram = glCreateProgram();

	glAttachShader(model.shaderProgram, model.vertexShaderObject);
	glAttachShader(model.shaderProgram, model.fragmentShaderObject);

	glLinkProgram(model.shaderProgram);

	glGenVertexArrays(1, &model.vao);
	glBindVertexArray(model.vao);

	glUseProgram(model.shaderProgram);

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);

	for(unsigned short i = 0; i < model.vertices.size(); i++)
	{
		model.elements.push_back(i);
	}
	glGenBuffers(1, &model.vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, model.vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, model.vertices.size() * sizeof(float), &model.vertices[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(0);

	//normals
	glGenBuffers(1, &model.normalbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, model.normalbuffer);
	glBufferData(GL_ARRAY_BUFFER, model.normals.size() * sizeof(float), &model.normals[0], GL_STATIC_DRAW);
	glNormalPointer(GL_FLOAT, 0, (void*)0 );


	glGenBuffers(1, &model.uvbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, model.uvbuffer);
	glBufferData(GL_ARRAY_BUFFER, model.uvcoords.size() * sizeof(float), &model.uvcoords[0], GL_STATIC_DRAW);
	glTexCoordPointer(2, GL_FLOAT, 0, 0);
	glEnableVertexAttribArray(0);

	//indices
	glGenBuffers(1, &model.elementbuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, model.elementbuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, model.elements.size() * sizeof(short), &model.elements[0], GL_STATIC_DRAW);

	glBindVertexArray(0);

	models.insert(std::pair<std::string, Model>(path, model));
}

void Renderer::DrawModel(const std::string &path, const std::string &texture, const Point3 &pos, const Point3 &forward, const Point3 &up, const float &scale)
{
	//Todo Try to load it
	auto modelIT = models.find(path);
	if(modelIT == models.end())
	{
		return;
	}
	auto textureIT = textures.find(texture);
	if(textureIT == textures.end())
	{
		return;
	}

	double originalMatrix[16];
	glGetDoublev(GL_MODELVIEW_MATRIX, originalMatrix);

	glTranslatef(pos.x, pos.y, pos.z);

	Point3 z(0,0,1);
	Point3 y(0,1,0);

	Point3 axis = z.crossProduct(forward);
	float angle = z.angleBetween(forward);

	glRotated(angle, axis.x, axis.y, axis.z);

	Point3 temp = up.cpy().rotate(axis, 360 - angle);

	float anglez = y.angleBetween(temp);
	if(temp.x > 0)
	{
		anglez = -anglez;
	}

	glRotated(anglez, 0, 0, 1);

	glScaled(scale, scale, scale);

	Model model = modelIT->second;

	glBindTexture(GL_TEXTURE_2D, textureIT->second);
	glColor3d(1,1,1);

	glBindVertexArray(model.vao);

	//Draw the mesh
	glDrawArrays(GL_TRIANGLES, 0, model.elements.size());

	glBindVertexArray(0);

	glLoadMatrixd(originalMatrix);
}

void Renderer::renderText(const std::string &text, float x, float y)
{
	font->draw(x, y, text.c_str());
}
