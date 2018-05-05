#include "model.hpp"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <regex>

#include <GL/glew.h>

//Just an allocation constructor
int Model::loadFromFile(std::string filename, bool verbose)
{
	std::ifstream objfile(filename, std::ios::in);
	std::string line;

	//Tokenizer variables
	std::string token_type;
	std::string token_data;

	//Temporary data buffers
	std::vector <glm::vec3> vertices;
	std::vector <glm::vec2> uvs;
	std::vector <glm::vec3> normals;

	//Indices of vertices, UVs and normals (face data)
	std::vector <int> vids;
	std::vector <int> uvids;
	std::vector <int> nids;
	int face_count = 0;

	if (verbose) std::cout << "Loading model '" << filename << "'\n";

	//Read line by line
	while (std::getline(objfile, line))
	{
		//Remove leading and trailing spaces
		line = std::regex_replace(line, std::regex("^ +| +$|( ) +"), "$1");

		//Skip comments
		if (line[0] == '#') continue;

		size_t sep = line.find(" ");
		if (sep == std::string::npos) continue;
		token_type = line.substr(0, sep);
		token_data = line.substr(sep);
		token_data = std::regex_replace(token_data, std::regex("^ +| +$|( ) +"), "$1");

		if (token_type == "v") //Vertex
		{
			glm::vec3 v;
			std::sscanf(token_data.c_str(), "%f %f %f", &v.x, &v.y, &v.z);
			vertices.push_back(v);
		}
		else if (token_type == "vt") //UV
		{
			glm::vec2 uv;
			std::sscanf(token_data.c_str(), "%f %f", &uv.x, &uv.y);
			uvs.push_back(uv);
		}
		else if (token_type == "vn") //Normal
		{
			glm::vec3 norm;
			std::sscanf(token_data.c_str(), "%f %f %f", &norm.x, &norm.y, &norm.y);
			normals.push_back(norm);
		}
		else if (token_type == "f") //Faces
		{
			int vid[3], uvid[3], nid[3];

			for (int i = 0; i < 9; i++)
			{
				int n;
				
				//If ID is missing, default to 1
				try
				{
					n = std::stoi(token_data);
				}
				catch(std::invalid_argument)
				{
					n = 1;
				}

				switch (i)
				{
					case 0:
						vid[0] = n;
						break;
					case 1:
						uvid[0] = n;
						break;
					case 2:
						nid[0] = n;
						break;

					case 3:
						vid[1] = n;
						break;
					case 4:
						uvid[1] = n;
						break;
					case 5:
						nid[1] = n;
						break;

					case 6:
						vid[2] = n;
						break;
					case 7:
						uvid[2] = n;
						break;
					case 8:
						nid[2] = n;
						break;

					default:
						std::cout << "\tdummy data?\n";

				}

				size_t delimpos = std::min(token_data.find(" "), token_data.find("/"));
				if (delimpos == std::string::npos) break;
				token_data = token_data.substr(delimpos + 1);
			}

			vids.push_back(vid[0]);
			vids.push_back(vid[2]);
			vids.push_back(vid[1]);
			uvids.push_back(uvid[0]);
			uvids.push_back(uvid[1]);
			uvids.push_back(uvid[2]);
			nids.push_back(nid[0]);
			nids.push_back(nid[1]);
			nids.push_back(nid[2]);
			face_count++;
		}

	}

	objfile.close();

	if (verbose) std::cout << "\tvertices: " << vertices.size() << "\n";
	if (verbose) std::cout << "\tuvs: " << uvs.size() << "\n";
	if (verbose) std::cout << "\tnormals: " << normals.size() << "\n";

	//'Unpack' the data
	if (verbose) std::cout << "Unpacking the data\n";
	int missing_vertices = 0, missing_uvs = 0, missing_normals = 0;
	for (int i = 0; i < vids.size(); i++)
	{
		int vid = vids[i] - 1;
		int uvid = uvids[i] - 1;
		int nid = nids[i] - 1;

		//std::cout << vid << " " << uvid << " " << nid << "\n";

		if (vid >= vertices.size())
		{
			missing_vertices++;
			this->vertices.push_back(glm::vec3(0.0));
		}
		else this->vertices.push_back(glm::vec3(vertices[vid]));

		if (uvid >= uvs.size())
		{
			missing_uvs++;
			this->uvs.push_back(glm::vec2(0.0));
		}
		else this->uvs.push_back(glm::vec2(uvs[uvid]));
		
		if (nid >= normals.size())
		{
			missing_normals++;
			this->normals.push_back(glm::vec3(0.0));
		}
		else this->normals.push_back(glm::vec3(normals[nid]));
		
	}

	if (verbose) std::cout << "\tmissing vertices: " << missing_vertices << "\n";
	if (verbose) std::cout << "\tmissing uvs: " << missing_uvs << "\n";
	if (verbose) std::cout << "\tmissing normals: " << missing_normals << "\n";
	if (verbose) std::cout << "\t faces: " << face_count << "\n";
	
	if (verbose) std::cout << "Done loading model\n";
	return 0;
}

void Model::init_buffers()
{
	//Create GL buffer
	glGenBuffers(1, &this->vertex_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, this->vertex_buffer);

	//Load vertex buffer with data
	glBufferData(GL_ARRAY_BUFFER, sizeof(this->vertices[0]) * this->vertices.size(), &this->vertices[0], GL_STATIC_DRAW);

	this->buffer_loaded = true;
}

void Model::free_buffers()
{
	if (this->buffer_loaded)
	{
		glDeleteBuffers(1, &this->vertex_buffer);
		this->buffer_loaded = false;
	}
}

//Just draw the model
void Model::draw()
{
	if (!this->buffer_loaded) return;
	
	glEnableVertexAttribArray(0); //Vertex data

	//Attribute 0 - vertex position
	glBindBuffer(GL_ARRAY_BUFFER, this->vertex_buffer);
	glVertexAttribPointer(
		0, //Attribute ID
		3, //Size
		GL_FLOAT, //Type
		GL_FALSE, //Normalized
		0, //Stride
		NULL //Array buffer offset
	);

	glDrawArrays(GL_TRIANGLES, 0, this->vertices.size());

	glDisableVertexAttribArray(0); //Vertex data
}

//Loader constructor
Model::Model(std::string filename)
{
	this->loadFromFile(filename, true);
	this->init_buffers();
}

//Destructor
Model::~Model()
{
	this->free_buffers();
}