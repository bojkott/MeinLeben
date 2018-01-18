#include "../Sampler2D.h"

class Sampler2DVulkan
{
public:
	Sampler2DVulkan();
	~Sampler2DVulkan();

	void setMagFilter(FILTER filter);
	void setMinFilter(FILTER filter);
	void setWrap(WRAPPING s, WRAPPING t);
};
