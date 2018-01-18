#include "../Texture2D.h"
class Texture2DVulkan
{
public:
	Texture2DVulkan();
	~Texture2DVulkan();

	int loadFromFile(std::string filename);
	void bind(unsigned int slot);
};
