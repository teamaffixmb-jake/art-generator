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

		// First we add a bias for positional differentiation 
		// of values inputted into the convolution
		auto l_y = hadamard(a_x, parameters(a_x.size(), a_x[0].size(), a_x[0][0].size()));
		l_y = bias(l_y);
		auto l_convolved_3d = convolve(l_y, parameters(3, 50, 50), 50);
		l_convolved_3d = leaky_relu(l_convolved_3d, 0.3);
		
		auto l_flattened = flatten(l_convolved_3d);

		std::vector<size_t> l_tnn_dimensions = { l_flattened.size() / 2 };

		for (const auto& l_dimension : l_tnn_dimensions)
		{
			l_flattened = weight_junction(l_flattened, l_dimension);
			l_flattened = bias(l_flattened);
			l_flattened = leaky_relu(l_flattened, 0.3);
		}

		l_flattened = weight_junction(l_flattened, 1);
		l_flattened = bias(l_flattened);
		l_flattened = sigmoid(l_flattened);

		return l_flattened[0];

	}

}
