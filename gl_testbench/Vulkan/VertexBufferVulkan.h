#include "../VertexBuffer.h"
#include <GL/glew.h>

class VertexBufferVulkan : public VertexBuffer
{
public:
	VertexBufferVulkan(size_t size, VertexBuffer::DATA_USAGE usage);
	~VertexBufferVulkan();

	void setData(const void* data, size_t size, size_t offset);
	void bind(size_t offset, size_t size, unsigned int location);
	void unbind();
	size_t getSize();


};
