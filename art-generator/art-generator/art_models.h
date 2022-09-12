#pragma once
#include "aurora-v4/aurora.h"
#include "affix-base/files.h"

namespace art_generator
{
	inline aurora::state_gradient_pair* discriminate(
		const std::vector<std::vector<std::vector<aurora::state_gradient_pair*>>>& a_x
	)
	{
		using namespace aurora;

		auto l_y = convolve(a_x, parameters(3, 5, 5), 5);
		l_y = average_pool(l_y, 5, 5);
		l_y = leaky_relu(l_y, 0.3);
		l_y = convolve({ l_y }, parameters(1, 3, 3), 3);
		l_y = average_pool(l_y, 2, 2);
		l_y = leaky_relu(l_y, 0.3);
		
		auto l_flattened = flatten(l_y);

		std::vector<size_t> l_tnn_dimensions = { l_flattened.size() / 2, 1 };

		for (const auto& l_dimension : l_tnn_dimensions)
		{
			l_flattened = weight_junction(l_flattened, l_dimension);
			l_flattened = bias(l_flattened);
			l_flattened = leaky_relu(l_flattened, 0.3);
		}

		return l_flattened[0];

	}

}
