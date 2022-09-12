#pragma once
#include "art_models.h"
#include "affix-base/serializable.h"

namespace art_generator
{
	struct training_set
	{
		std::vector<std::vector<std::vector<double>>> m_x;
		double m_y = 0;
	};

}
