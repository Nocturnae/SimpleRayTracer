//
// Created by Fatih on 10/30/15.
//

#include "Scene.h"
#include <fstream>
#include <cmath>
#include <algorithm>

std::istream &operator>>(std::istream &stream, Scene &scene)
{
	stream >> scene._rayReflect;
	stream >> scene._background;
	stream >> scene._ambient;

	unsigned long tmpCount;
	unsigned long i;
	std::string dummy;

	stream >> tmpCount;
	scene._lights.resize(tmpCount);
	for (i = 0; i < tmpCount; ++i) {
		stream >> scene._lights[i];
	}

	stream >> tmpCount;
	getline(stream, dummy);
	scene._materials.resize(tmpCount);
	for (i = 0; i < tmpCount; ++i) {
		getline(stream, dummy);
		stream >> scene._materials[i] >> std::ws;
	}
    
    // textures
    getline(stream, dummy);
    stream >> tmpCount;
    scene._textures.resize(tmpCount);
    for (i = 0; i < tmpCount; i++) {
        stream >> scene._textures[i] >> std::ws;
    }
    
    // translation
    getline(stream, dummy);
    stream >> tmpCount;
    scene._translation.resize(tmpCount);
    // TODO: where to realy store new data
    for (i = 0; i < tmpCount; i++) {
        stream >> scene._translation[i] >> std::ws;
    }
    
    // scaling
    getline(stream, dummy);
    stream >> tmpCount;
    scene._scaling.resize(tmpCount);
    // TODO: where to realy store new data
    for (i = 0; i < tmpCount; i++) {
        stream >> scene._scaling[i] >> std::ws;
    }
    
    // rotation
    getline(stream, dummy);
    stream >> tmpCount;
    scene._rotation.resize(tmpCount);
    // TODO: where to realy store new data
    // hmmm!! rotation not vec3
    for (i = 0; i < tmpCount; i++) {
        stream >> scene._rotation[i] >> std::ws;
        float w_value;
        stream >> w_value;
        scene._rotation[i].W(w_value);
    }
    
    float maxDist = sqrtf(2);
    size_t depth = 2;
    scene._vertices.push_back(Vertex(Vector3::Zero));
    scene._vertices.push_back(Vertex(Vector3(0, 0, 1)));
    scene._vertices.push_back(Vertex(Vector3(0, 1, 0)));
    scene._vertices.push_back(Vertex(Vector3(1, 0, 0)));
    scene._vertices.push_back(Vertex(Vector3(0, 1, 1)));
    scene._vertices.push_back(Vertex(Vector3(1, 0, 1)));
    scene._vertices.push_back(Vertex(Vector3(1, 1, 0)));
    scene._vertices.push_back(Vertex(Vector3(1, 1, 1)));
    
    stream >> tmpCount;
    std::string type;
    
    for (i = 0; i < tmpCount; ++i) {
        stream >> type;
        getline(stream, dummy);
        
        if (type == "#CubeInstance") {
            Mesh tmp;
            stream >> tmp >> std::ws;
            tmp.SetScene(&scene);
            scene._meshes.push_back(tmp);
        }
        else {
            // don't have the correct attributes :/?
            MaterialId mid;
            TextureId tid;
            stream >> mid >> tid;
            Sphere tmp(0, 1.0, ;
            stream >> tmp >> std::ws;
            tmp.SetScene(&scene);
            scene._spheres.push_back(tmp);
        }
    }
/*
	stream >> tmpCount >> std::ws;
	getline(stream, dummy);
	scene._vertices.resize(tmpCount);
	float maxDist = 0;
	for (i = 0; i < tmpCount; ++i) {
		stream >> scene._vertices[i] >> std::ws;
		maxDist = std::max(
				{
						maxDist,
						std::abs(scene._vertices[i].Position().X()),
						std::abs(scene._vertices[i].Position().Y()),
						std::abs(scene._vertices[i].Position().Z())
				});
	  }
 
	size_t depth = 0;
	maxDist = std::abs(maxDist);
	if (maxDist > 4) {
		depth = static_cast<size_t>(std::ceil(std::log2(maxDist)));
		maxDist = std::pow(2, depth);
	}
	else
	{
		depth = 2;
	}*/

	scene._root = Octree(Vector3(-maxDist, -maxDist, -maxDist), maxDist * 2);
	scene._root.Partition(depth);
	scene._root.SetScene(&scene);

	for (const auto& mesh : scene._meshes)
	{
		const auto& tris = mesh.GetTris();
		for (const auto& tri : tris)
		{
			scene._root.Insert(tri);
		}
	}

	scene._root.CountObjects();
	scene._root.Optimize();
	scene._root.SetScene(&scene);

	return stream;
}

void Scene::AddCameras(std::istream &stream)
{
	std::string dummy;
	unsigned long camCount;
	stream >> camCount >> std::ws;

	_cameras.resize(camCount);
	for (unsigned long i = 0; i < camCount; ++i) {
		getline(stream, dummy);
		stream >> _cameras[i];
		_cameras[i].SetScene(this);
	}
}

Scene Scene::GetMockScene()
{
	Scene scene;
	scene._vertices.push_back(Vertex(Vector3(0, 0, 0)));
	scene._vertices.push_back(Vertex(Vector3(3, 0, 0)));
	scene._vertices.push_back(Vertex(Vector3(-3, 0, 0)));
	scene._vertices.push_back(Vertex(Vector3(0, 3, 0)));
	scene._vertices.push_back(Vertex(Vector3(0, -3, 0)));

	scene._materials.push_back(Material::Diffuse(Color(255, 0, 0)));
	scene._materials.push_back(Material::Diffuse(Color(255, 255, 0)));
	scene._materials.push_back(Material::Diffuse(Color(255, 255, 255)));
	scene._materials.push_back(Material::Diffuse(Color(0, 255, 255)));
	scene._materials.push_back(Material::Diffuse(Color(0, 0, 255)));

	scene._spheres.push_back(Sphere(1, 1, 1));
	scene._spheres.push_back(Sphere(2, 1, 2));
	scene._spheres.push_back(Sphere(3, 1, 3));
	scene._spheres.push_back(Sphere(4, 1, 4));
	scene._spheres.push_back(Sphere(5, 1, 5));

	scene._spheres[0].SetScene(&scene);
	scene._spheres[1].SetScene(&scene);
	scene._spheres[2].SetScene(&scene);
	scene._spheres[3].SetScene(&scene);
	scene._spheres[4].SetScene(&scene);

	scene._cameras.push_back(Camera(Vector3(0, 0, -5), Vector3(0, 1, 0), Vector3(0, 0, 1), -5, 5, -5, 5, 1, 256, 256));
	scene._cameras[0].SetScene(&scene);
	
	scene._background = Color(128, 0, 96);

	return scene;
}

Scene::Scene(const Scene& rhs)
{
	*this = rhs;
}

Scene& Scene::operator=(const Scene& rhs)
{
	_vertices = rhs._vertices;

	_cameras = rhs._cameras;
	_spheres = rhs._spheres;
	_meshes = rhs._meshes;

	_materials = rhs._materials;

	_lights = rhs._lights;

	_rayReflect = rhs._rayReflect;
	_background = rhs._background;
	_ambient = rhs._ambient;

	_root.SetScene(this);

	for (auto& i : _cameras)
	{
		i.SetScene(this);
	}

	for (auto& i : _spheres)
	{
		i.SetScene(this);
	}

	for (auto& i : _meshes)
	{
		i.SetScene(this);
	}

	return *this;
}
