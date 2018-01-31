#pragma once
#include "../Mesh.h"
class MeshVulkan : public Mesh
{
public:
	MeshVulkan();
	~MeshVulkan();

	static bool sortMesh(const Mesh* meshA, const Mesh* meshB);

};
