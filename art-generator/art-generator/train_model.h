#pragma once
#include "art_models.h"

namespace art_generator
{
	struct training_set
	{
		std::vector<std::vector<std::vector<double>>> m_x;
		double m_y = 0;
	};

	class trainer
	{
	private:
		std::vector<training_set> m_training_sets;

	};
}
