#include "MeshVulkan.h"
#include "TechniqueVulkan.h"
MeshVulkan::MeshVulkan()
{
}

MeshVulkan::~MeshVulkan()
{
}

bool MeshVulkan::sortMesh(const Mesh * meshA, const Mesh * meshB)
{
	return ((TechniqueVulkan*)meshA->technique)->id	< ((TechniqueVulkan*)meshB->technique)->id;
}
