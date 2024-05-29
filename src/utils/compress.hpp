#ifndef COMPRESS_HPP
#define COMPRESS_HPP

#include "filestruct.hpp"

namespace Compress{
	void CompFile(ContentFiles& Files, unsigned int& threads);
	void CompReset(ContentFiles& Files);
}

#endif