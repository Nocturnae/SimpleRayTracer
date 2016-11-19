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
    for (i = 0; i < tmpCount; i++) {
        float t_x, t_y, t_z;
        stream >> t_x >> t_y >> t_z;
        Vector3 r1(1, 0, 0, t_x), r2(0, 1, 0, t_y), r3(0, 0, 1, t_z), r4(0, 0, 0, 1);
        (scene._translation[i])[0] = r1;
        (scene._translation[i])[1] = r2;
        (scene._translation[i])[2] = r3;
        (scene._translation[i])[3] = r4;
    }
    
    // scaling
    getline(stream, dummy);
    getline(stream, dummy);
    stream >> tmpCount;
    scene._scaling.resize(tmpCount);
    for (i = 0; i < tmpCount; i++) {
        float s_x, s_y, s_z;
        stream >> s_x >> s_y >> s_z;
        Vector3 r1(s_x, 0, 0, 0), r2(0, s_y, 0, 0), r3(0, 0, s_z, 0), r4(0, 0, 0, 1);
        (scene._scaling[i])[0] = r1;
        (scene._scaling[i])[1] = r2;
        (scene._scaling[i])[2] = r3;
        (scene._scaling[i])[3] = r4;
    }
    
    // rotation
    getline(stream, dummy);
    getline(stream, dummy);
    stream >> tmpCount;
    scene._rotation.resize(tmpCount);
    for (i = 0; i < tmpCount; i++) {
        float alpha;
        int u_x, u_y, u_z;
        stream >> alpha >> u_x >> u_y >> u_z;
        
        Vector3 rot_axis(u_x, u_y, u_z);
        // normalize?
        
        float length = rot_axis.Magnitude();
        
        Matrix t_inv(Vector3(1, 0, 0, -u_x), Vector3(0, 1, 0, -u_y), Vector3(0, 0, 1, -u_z), Vector3(0, 0, 0, 1));
        Matrix r_x(Vector3(1, 0, 0, 0),
                   Vector3(0, u_z / length, -u_y / length, 0),
                   Vector3(0, u_y / length, u_z / length, 0),
                   Vector3(0, 0, 0, 1));
        
        
        Matrix r_z(Vector3(cos(alpha), -sin(alpha), 0, 0),
                   Vector3(sin(alpha), cos(alpha), 0, 0),
                   Vector3(0, 0, 1, 0),
                   Vector3(0, 0, 0 ,1));
        
        
    }
    
    getline(stream, dummy);
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
            Vector3 s_center(0, 0, 0, 1);
            float radius = 1.0;
            
            MaterialId mid;
            TextureId tid;
            stream >> mid >> tid;
            
            int t_count, t_id;
            char t_type;
            stream >> t_count;
            
            for (int j = 0; j < t_count; j++) {
                stream >> t_type >> t_id;
                if (t_type == 's') {
                    radius *= scene._scaling[t_id - 1][0][0];
                }
                else if (t_type == 't') {
                    s_center = scene._translation[t_id - 1] * s_center;
                }
            }
            
            Vertex centerVertex(s_center);
            
            VertexId vid;

            
            // check if already exits
            std::vector<Vertex>::iterator it = find(scene._vertices.begin(), scene._vertices.end(), centerVertex);
            if (it == scene._vertices.end()) {
                vid = scene._vertices.size();
                scene._vertices.push_back(centerVertex);
            }
            else {
                vid = it - scene._vertices.begin();
            }
            
            Sphere tmp(vid, radius, mid, tid);
            tmp.SetScene(&scene);
            scene._spheres.push_back(tmp);
        }
    }
    
    float maxDist = sqrtf(2);
    size_t depth = 2;
	for (i = 0; i < scene._vertices.size(); ++i) {
		maxDist = std::max(
				{
						maxDist,
						std::abs(scene._vertices[i].Position().X()),
						std::abs(scene._vertices[i].Position().Y()),
						std::abs(scene._vertices[i].Position().Z())
				});
	  }
 
	maxDist = std::abs(maxDist);
	if (maxDist > 4) {
		depth = static_cast<size_t>(std::ceil(std::log2(maxDist)));
		maxDist = std::pow(2, depth);
	}
	else
	{
		depth = 2;
	}

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
	Scene scene;/*
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
	
	scene._background = Color(128, 0, 96);*/

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
