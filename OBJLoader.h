#pragma once

//STD Libs
#include<iostream>
#include<string>
#include<fstream>
#include<vector>
#include<sstream>
//#include<algorithm>

//OpenGL Math libs
#include "Vec3.h"
#include "Mat4.h"

//Own libs
#include"Vertex.h"

static std::vector<Vertex> loadOBJ(const char* file_name)
{
	//Vertex portions
	std::vector<Vec3f> vertex_positions;
	std::vector<Vec3f> vertex_texcoords;
	std::vector<Vec3f> vertex_normals;

	//Face vectors
	std::vector<size_t> vertex_position_indicies;
	std::vector<size_t> vertex_texcoord_indicies;
	std::vector<size_t> vertex_normal_indicies;

	//Vertex array
	std::vector<Vertex> vertices;

	std::stringstream ss;
	std::ifstream in_file(file_name);
	std::string line = "";
	std::string prefix = "";
	Vec3f temp_vec3;
	Vec3f temp_vec2;
	size_t temp_index = 0;

	//File open error check
	if (!in_file.is_open())
	{
		exit(0);
		throw "ERROR::OBJLOADER::Could not open file.";
	}

	//Read one line at a time
	while (std::getline(in_file, line))
	{
		//Get the prefix of the line
		ss.clear();
		ss.str(line);
		ss >> prefix;

		if (prefix == "#" || prefix == "o" || prefix == "use_mtl" || prefix == "s")
		{

		}
		else if (prefix == "v") //Vertex position
		{
			ss >> temp_vec3.x >> temp_vec3.y >> temp_vec3.z;
			vertex_positions.push_back(temp_vec3);
		}
		else if (prefix == "vt")
		{
			ss >> temp_vec2.x >> temp_vec2.y;
			temp_vec2.z = 0.0f;
			vertex_texcoords.push_back(temp_vec2);

		}
		else if (prefix == "vn")
		{
			ss >> temp_vec3.x >> temp_vec3.y >> temp_vec3.z;
			vertex_normals.push_back(temp_vec3);
		}
		else if (prefix == "f")
		{
			int counter = 0;
			while (ss >> temp_index)
			{
				//Pushing indices into correct arrays
				if (counter == 0)
					vertex_position_indicies.push_back(temp_index);
				else if (counter == 1)
					vertex_texcoord_indicies.push_back(temp_index);
				else if (counter == 2)
					vertex_normal_indicies.push_back(temp_index);

				//Handling characters
				if (ss.peek() == '/')
				{
					++counter;
					ss.ignore(1, '/');
				}
				else if (ss.peek() == ' ')
				{
					++counter;
					ss.ignore(1, ' ');
				}

				//Reset the counter
				if (counter > 2)
					counter = 0;
			}
		}
		else
		{

		}
	}

	//Build final vertex array (mesh)
	vertices.resize(vertex_position_indicies.size(), Vertex());

	//Load in all indices
	for (size_t i = 0; i < vertices.size(); ++i)
	{
		vertices[i].position = vertex_positions[vertex_position_indicies[i] - 1];
		//vertices[i].texcoord = vertex_texcoords[vertex_texcoord_indicies[i] - 1];
		vertices[i].normal = vertex_normals[vertex_normal_indicies[i] - 1];
		//vertices[i].color(WHITE);
	}

	//DEBUG
	std::cout << "Nr of vertices: " << vertices.size() << "\n";

	//Loaded success
	std::cout << "OBJ file loaded!" << "\n";
	return vertices;
}