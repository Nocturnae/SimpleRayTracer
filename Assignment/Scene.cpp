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
	scene._materials.resize(tmpCount);
	for (i = 0; i < tmpCount; ++i) {
        stream >> dummy >> dummy;
		stream >> scene._materials[i] >> std::ws;
	}
    
    // textures
    stream >> dummy;
    stream >> tmpCount;
    for (i = 0; i < tmpCount; i++) {
        std::string filename;
        stream >> filename;
        scene._textures.push_back(Texture(filename.c_str()));
    }

    // translation
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
    
    stream >> dummy;
    stream >> tmpCount;
    scene._rotation.resize(tmpCount);
    scene._reverse.resize(tmpCount);
    for (i = 0; i < tmpCount; i++) {
        int alpha;
        float u_x, u_y, u_z;
        stream >> alpha;
        stream >> u_x;
        stream >> u_y;
        stream >> u_z;
        
        Vector3 rotationAxis(u_x, u_y, u_z);
        rotationAxis.Normalize();
        
        float c = cos(alpha * M_PI / 180.0), s = sin(alpha * M_PI / 180.0);
        
        Vector3 r1((powf(rotationAxis[0], 2.0f) * (1 - c)) + c,
                   (rotationAxis[0] * rotationAxis[1] * (1 - c)) - (rotationAxis[2] * s),
                   (rotationAxis[0] * rotationAxis[2] * (1 - c)) + (rotationAxis[1] * s),
                   0);
        Vector3 r2((rotationAxis[0] * rotationAxis[1] * (1 - c)) + (rotationAxis[2] * s),
                   (powf(rotationAxis[1], 2.0f) * (1 - c)) + c,
                   (rotationAxis[1] * rotationAxis[2] * (1 - c)) - (rotationAxis[0] * s),
                   0);
        Vector3 r3((rotationAxis[0] * rotationAxis[2] * (1 - c)) - (rotationAxis[1] * s),
                   (rotationAxis[1] * rotationAxis[2] * (1 - c)) + (rotationAxis[0] * s),
                   (powf(rotationAxis[2], 2.0f) * (1 - c)) + c,
                   0);
        Vector3 r4(Vector3::Zero);
        
        (scene._rotation[i])[0] = r1;
        (scene._rotation[i])[1] = r2;
        (scene._rotation[i])[2] = r3;
        (scene._rotation[i])[3] = r4;
        
        float c_r = cosf(2 * M_PI - (alpha * M_PI / 180.0)), s_r = sinf(2 * M_PI - (alpha * M_PI / 180.0));
        
        Vector3 r1_r((powf(rotationAxis[0], 2.0f) * (1 - c_r)) + c_r,
                   (rotationAxis[0] * rotationAxis[1] * (1 - c_r)) - (rotationAxis[2] * s_r),
                   (rotationAxis[0] * rotationAxis[2] * (1 - c_r)) + (rotationAxis[1] * s_r),
                   0);
        Vector3 r2_r((rotationAxis[0] * rotationAxis[1] * (1 - c_r)) + (rotationAxis[2] * s_r),
                   (powf(rotationAxis[1], 2.0f) * (1 - c_r)) + c_r,
                   (rotationAxis[1] * rotationAxis[2] * (1 - c_r)) - (rotationAxis[0] * s_r),
                   0);
        Vector3 r3_r((rotationAxis[0] * rotationAxis[2] * (1 - c_r)) - (rotationAxis[1] * s_r),
                   (rotationAxis[1] * rotationAxis[2] * (1 - c_r)) + (rotationAxis[0] * s_r),
                   (powf(rotationAxis[2], 2.0f) * (1 - c_r)) + c_r,
                   0);
        Vector3 r4_r(Vector3::Zero);
        
        (scene._reverse[i])[0] = r1_r;
        (scene._reverse[i])[1] = r2_r;
        (scene._reverse[i])[2] = r3_r;
        (scene._reverse[i])[3] = r4_r;
        
    }

	stream >> tmpCount;
	std::string type;
    
    MaterialId mid; TextureId tid; int t_count;
    char t_type; int t_id;

	for (i = 0; i < tmpCount; ++i) {
        stream >> type;

		if (type == "#CubeInstance") {
            
            Matrix transMatrix;
            
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
                    vid = scene._vertices.size() + 1;
                    scene._vertices.push_back(vertex);
                }
                else {
                    vid = it - scene._vertices.begin();
                }
                
                vidList.push_back(vid);
            }
            
            Mesh temp(mid, tid, vidList);
            temp.SetScene(&scene);
            scene._meshes.push_back(temp);
            
		}
		else {
            
            Vector3 s_center;
            Matrix rotMatrix;
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
                    /*std::cout << "**" << scene._reverse[t_id - 1][0] << " "
                    << scene._reverse[t_id - 1][1] << " "
                    << scene._reverse[t_id - 1][2] << "**" << std::endl;*/
                    rotMatrix = scene._reverse[t_id - 1] * rotMatrix;
                    //std::cout << rotMatrix[0] << " " << rotMatrix[1] << " " << rotMatrix[2] << std::endl;
                }
            }
            
            Vertex center(s_center);
            VertexId vid;
            
            std::vector<Vertex>::iterator it = find(scene._vertices.begin(), scene._vertices.end(), center);
            if (it == scene._vertices.end()) {
                vid = scene._vertices.size() + 1;
                scene._vertices.push_back(center);
            }
            else {
                vid = it - scene._vertices.begin();
            }
            
            Sphere tmp(vid, radius, mid, tid, rotMatrix);
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
