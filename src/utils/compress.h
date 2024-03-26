#pragma once

#include "filestruct.h"

namespace Compress{
	void CompFile(ContentFiles& Files, unsigned int& threads);
	void CompReset(ContentFiles& Files);
}