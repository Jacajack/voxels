#include "model.hpp"

#include <string>
#include <cstdio>
#include <fstream>
#include <sstream>
#include <iostream>
#include <regex>

#include <libpng16/png.h>
#include <GL/glew.h>

//This is awfully slow because... guess what...
//It uses regex!
int Model::load_obj(std::string filename, bool verbose)
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

	if (verbose) std::cerr << "Loading model '" << filename << "'\n";

	if (objfile.is_open())
	{
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
				std::sscanf(token_data.c_str(), "%f %f %f", &norm.x, &norm.y, &norm.z);
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
							if (verbose) std::cerr << "\tdummy data?\n";

					}

					size_t delimpos = std::min(token_data.find(" "), token_data.find("/"));
					if (delimpos == std::string::npos) break;
					token_data = token_data.substr(delimpos + 1);
				}

				vids.push_back(vid[0]);
				vids.push_back(vid[1]);
				vids.push_back(vid[2]);
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
	}
	else
	{
		std::cerr << "cannot load model file - " << filename << "\n";
		return 1;
	}

	if (verbose) std::cerr << "\tvertices: " << vertices.size() << "\n";
	if (verbose) std::cerr << "\tuvs: " << uvs.size() << "\n";
	if (verbose) std::cerr << "\tnormals: " << normals.size() << "\n";

	//'Unpack' the data
	if (verbose) std::cerr << "Unpacking the data\n";
	int missing_vertices = 0, missing_uvs = 0, missing_normals = 0;
	for (unsigned int i = 0; i < vids.size(); i++)
	{
		unsigned int vid = vids[i] - 1;
		unsigned int uvid = uvids[i] - 1;
		unsigned int nid = nids[i] - 1;

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

	if (verbose) std::cerr << "\tmissing vertices: " << missing_vertices << "\n";
	if (verbose) std::cerr << "\tmissing uvs: " << missing_uvs << "\n";
	if (verbose) std::cerr << "\tmissing normals: " << missing_normals << "\n";
	if (verbose) std::cerr << "\tfaces: " << face_count << "\n";
	
	if (verbose) std::cerr << "Done loading model\n";
	return 0;
}

int Model::load_texture(std::string filename, bool verbose)
{
	if (verbose) std::cerr << "Loading texture file - " << filename << "\n";

	//Open texture file
	std::FILE *texfile;
	texfile = std::fopen(filename.c_str(), "rb");
	if (texfile == NULL)
	{
		std::cerr << "cannot load texture file - " << filename << "\n";
		return 1;
	}

	//Libpng init
	png_structp png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (png == NULL)
	{
		std::cerr << "libpng init failed\n";
		return 1;
	}

	//Get PNG info
	png_infop pnginfo = png_create_info_struct(png);
	if (pnginfo == NULL)
	{
		std::cerr << "cannot get png info\n";
		return 1;
	}

	//Libpng error handler
	if (setjmp(png_jmpbuf(png)))
	{
		png_destroy_read_struct(&png, &pnginfo, NULL);
		std::cerr << "libpng error\n";
		return 1;
	}

	//Read info
	png_init_io(png, texfile);
	png_read_info(png, pnginfo);
	this->texture_width = png_get_image_width(png, pnginfo);
	this->texture_height = png_get_image_height(png, pnginfo);
	png_byte color_type = png_get_color_type(png, pnginfo);
	png_byte bit_depth = png_get_bit_depth(png, pnginfo);

	//Force 8-bit RGB space
	if (bit_depth == 16) png_set_strip_16(png);
	if (color_type == PNG_COLOR_TYPE_PALETTE) png_set_palette_to_rgb(png);
	if (color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8) png_set_expand_gray_1_2_4_to_8(png);
	if (png_get_valid(png, pnginfo, PNG_INFO_tRNS)) png_set_tRNS_to_alpha(png);
	if (color_type == PNG_COLOR_TYPE_GRAY
		|| color_type == PNG_COLOR_TYPE_GRAY_ALPHA)
    		png_set_gray_to_rgb(png);
	png_set_interlace_handling(png);
	png_read_update_info(png, pnginfo);

	//This is linear texture buffer
	this->texture = new unsigned char[this->texture_width * this->texture_height * 3];

	//And this tricks libpng to read data into my linear bufer buahaha
	//Note: BMPs are upside down while PNGs are not
	png_bytep *rows = new png_bytep[this->texture_height];
	unsigned char *p = this->texture;
	for (int i = this->texture_height - 1; i >= 0; i--)
	{
		rows[i] = p;
		p += this->texture_width * 3;
	}

	//Read data
	png_read_image(png, rows);

	//Cleanup
	png_read_end(png, NULL);
	delete rows;
	png_destroy_info_struct(png, &pnginfo);
	png_destroy_read_struct(&png, &pnginfo, NULL);	
	std::fclose(texfile);

	if (verbose) std::cerr << "Done texture file (" << this->texture_width << "x" << this->texture_height << ")\n";

	return 0;
}

void Model::init_buffers()
{
	if (this->buffers_loaded) return;

	//Create vertex buffer
	glGenBuffers(1, &this->vertex_buffer_id);
	glBindBuffer(GL_ARRAY_BUFFER, this->vertex_buffer_id);

	//Load vertex buffer with data
	glBufferData(GL_ARRAY_BUFFER, sizeof(this->vertices[0]) * this->vertices.size(), &this->vertices[0], GL_STATIC_DRAW);

	//Create UV buffer
	glGenBuffers(1, &this->uv_buffer_id);
	glBindBuffer(GL_ARRAY_BUFFER, this->uv_buffer_id);

	//Load UV buffer with data
	glBufferData(GL_ARRAY_BUFFER, sizeof(this->uvs[0]) * this->uvs.size(), &this->uvs[0], GL_STATIC_DRAW);

	//Create texture
	glGenTextures(1, &this->texture_id);
	glBindTexture(GL_TEXTURE_2D, this->texture_id);

	//Pass texture data to OpenGL
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, this->texture_width, this->texture_height, 0, GL_RGB, GL_UNSIGNED_BYTE, this->texture);

	//Texture settings
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glGenerateMipmap(GL_TEXTURE_2D);

	this->buffers_loaded = true;
}

void Model::free_buffers()
{
	if (this->buffers_loaded)
	{
		glDeleteBuffers(1, &this->vertex_buffer_id);
		glDeleteBuffers(1, &this->uv_buffer_id);
		glDeleteTextures(1, &this->texture_id);

		this->buffers_loaded = false;
	}
}

//Just draw the model
void Model::draw(GLuint texture_uniform_id)
{
	if (!this->buffers_loaded) return;
	
	//Activate texturing unit
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, this->texture_id);
	glUniform1i(texture_uniform_id, 0);

	glEnableVertexAttribArray(0); //Vertex data
	glEnableVertexAttribArray(1); //UV

	//Attribute 0 - vertex position
	glBindBuffer(GL_ARRAY_BUFFER, this->vertex_buffer_id);
	glVertexAttribPointer(
		0, //Attribute ID
		3, //Size
		GL_FLOAT, //Type
		GL_FALSE, //Normalized
		0, //Stride
		NULL //Array buffer offset
	);

	//Attribute 1 - UV
	glBindBuffer(GL_ARRAY_BUFFER, this->uv_buffer_id);
	glVertexAttribPointer(
		1, //Attribute ID
		2, //Size
		GL_FLOAT, //Type
		GL_FALSE, //Normalized
		0, //Stride
		NULL //Array buffer offset
	);

	glDrawArrays(GL_TRIANGLES, 0, this->vertices.size());

	glDisableVertexAttribArray(0); //Vertex data
	glDisableVertexAttribArray(1); //UV

	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_TEXTURE_2D);
}

//Loader constructor
Model::Model(std::string obj_filename, std::string texture_filename)
{
	this->buffers_loaded = false;


	if ( !this->load_obj(obj_filename, true) )
	{
		if ( !this->load_texture( texture_filename, true ) )
		{
			this->init_buffers();
		}
	}
}

//Destructor
Model::~Model()
{
	this->free_buffers();
	delete this->texture;
}