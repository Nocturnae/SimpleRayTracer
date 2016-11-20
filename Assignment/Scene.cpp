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

    // materials
	stream >> tmpCount;
	//getline(stream, dummy);
	scene._materials.resize(tmpCount);
	for (i = 0; i < tmpCount; ++i) {
		//getline(stream, dummy);
        stream >> dummy >> dummy;
		stream >> scene._materials[i] >> std::ws;
	}
    
    // textures
    //getline(stream, dummy);
    stream >> dummy;
    stream >> tmpCount;
    for (i = 0; i < tmpCount; i++) {
        std::string filename;
        stream >> filename;
        scene._textures.push_back(Texture(filename.c_str()));
    }

    // translation
    //getline(stream, dummy);
    //getline(stream, dummy);
    stream >> dummy;
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
    //getline(stream, dummy);
    //getline(stream, dummy);
    stream >> dummy;
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
    //getline(stream, dummy);
    //getline(stream, dummy);
    stream >> dummy;
    stream >> tmpCount;
    scene._rotation.resize(tmpCount);
    for (i = 0; i < tmpCount; i++) {
        int alpha;
        float u_x, u_y, u_z;
        //stream >> alpha >> u_x >> u_y >> u_z;
        stream >> alpha;
        stream >> u_x;
        stream >> u_y;
        stream >> u_z;
        
        Vector3 rotationAxis(u_x, u_y, u_z);
        rotationAxis.Normalize();
        
        float c = cos(alpha), s = sin(alpha);
        Vector3 r1(pow(rotationAxis[0], 2) * (1 - c) + c,
                   rotationAxis[0] * rotationAxis[1] * (1 - c) - rotationAxis[2] * s,
                   rotationAxis[0] * rotationAxis[2] * (1 - c) + rotationAxis[1] * s,
                   0);
        Vector3 r2(rotationAxis[0] * rotationAxis[1] * (1 - c) + rotationAxis[2] * s,
                   pow(rotationAxis[1], 2) * (1 - c) + c,
                   rotationAxis[1] * rotationAxis[2] * (1 - c) - rotationAxis[0] * s,
                   0);
        Vector3 r3(rotationAxis[0] * rotationAxis[2] * (1 - c) - rotationAxis[1] * s,
                   rotationAxis[1] * rotationAxis[2] * (1 - c) + rotationAxis[0] * s,
                   pow(rotationAxis[2], 2) * (1 - c) + c,
                   0);
        Vector3 r4(0, 0, 0, 1);
        (scene._rotation[i])[0] = r1;
        (scene._rotation[i])[1] = r2;
        (scene._rotation[i])[2] = r3;
        (scene._rotation[i])[3] = r4;
    }

    //getline(stream, dummy);
	stream >> tmpCount;
	std::string type;
    
    MaterialId mid; TextureId tid; int t_count;
    char t_type; int t_id;

	for (i = 0; i < tmpCount; ++i) {
		/*stream >> type;
		getline(stream, dummy);*/
        
        //getline(stream, type);
        stream >> type;

		if (type == "#CubeInstance") {
            
            std::vector<Vector3> vertexList = {Vector3(0.5f, 0.5f, 0.5f), Vector3(0.5f, 0.5f, -0.5f),
                Vector3(-0.5f, 0.5f, -0.5f), Vector3(-0.5f, 0.5f, 0.5f),
                Vector3(0.5f, -0.5f, 0.5f), Vector3(0.5f, -0.5f, -0.5f),
                Vector3(-0.5f, -0.5f, -0.5f), Vector3(-0.5f, -0.5f, 0.5f)};
            
            stream >> mid >> tid >> t_count;
            
            for (int j = 0; j < t_count; j++) {
                stream >> t_type >> t_id;
                if (t_type == 's') {
                    for (int k = 0; k < vertexList.size(); k++) {
                        vertexList[k] = scene._scaling[t_id - 1] * vertexList[k];
                    }
                }
                else if (t_type == 't') {
                    for (int k = 0; k < vertexList.size(); k++) {
                        vertexList[k] = scene._translation[t_id - 1] * vertexList[k];
                    }
                }
                else if (t_type == 'r') {
                    for (int k = 0; k < vertexList.size(); k++) {
                        vertexList[k] = scene._rotation[t_id - 1] * vertexList[k];
                    }
                }
            }
            
            std::vector<VertexId> vidList;
            for (int k = 0; k < vertexList.size(); k++) {
                Vertex vertex(vertexList[k]);
                VertexId vid;
                std::vector<Vertex>::iterator it = find(scene._vertices.begin(), scene._vertices.end(), vertex);
                if (it == scene._vertices.end()) {
                    // TODO: check
                    vid = scene._vertices.size() + 1;
                    scene._vertices.push_back(vertex);
                }
                else {
                    // TODO: check
                    vid = it - scene._vertices.begin();
                }
                
                vidList.push_back(vid);
            }
            
            /*
            std::vector<VertexId> vidList;
            for (int k = 0; k < vertexList.size(); k++) {
                Vertex vertex(vertexList[k]);
                VertexId vid = scene._vertices.size() + 1;
                scene._vertices.push_back(vertex);
                vidList.push_back(vid);
            }*/
            
            Mesh temp(mid, tid, vidList);
            temp.SetScene(&scene);
            scene._meshes.push_back(temp);
            
		}
		else {
            
            Vector3 s_center;
            float radius = 1.0f;
            
            stream >> mid >> tid >> t_count;
            
            for (int j = 0; j < t_count; j++) {
                stream >> t_type >> t_id;
                if (t_type == 's') {
                    radius *= scene._scaling[t_id - 1][0][0];
                }
                else if (t_type == 't') {
                    s_center = scene._translation[t_id - 1] * s_center;
                }
                else if (t_type == 'r') {
                    
                }
            }
            
            Vertex center(s_center);
            VertexId vid;
            
            //scene._vertices.push_back(center);
            
            std::vector<Vertex>::iterator it = find(scene._vertices.begin(), scene._vertices.end(), center);
            if (it == scene._vertices.end()) {
                // TODO: check
                vid = scene._vertices.size() + 1;
                scene._vertices.push_back(center);
            }
            else {
                // TODO: check
                vid = it - scene._vertices.begin();
            }
            
            Sphere tmp(vid, radius, mid, tid);
            tmp.SetScene(&scene);
            scene._spheres.push_back(tmp);
            
		}
	}
    
    float maxDist = 0;
    for (i = 0; i < scene._vertices.size(); i++) {
        maxDist = std::max({maxDist,
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
/*
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
*/
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
