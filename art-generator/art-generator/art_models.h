#pragma once
#include "aurora-v4/aurora.h"
#include "affix-base/files.h"

namespace art_generator
{
	inline aurora::latent::state_gradient_pair* discriminate(
		const std::vector<std::vector<std::vector<aurora::latent::state_gradient_pair*>>>& a_x
	)
	{
		using namespace aurora;
		using namespace latent;

		// First we add a bias for positional differentiation 
		// of values inputted into the convolution
		//auto l_y = bias(a_x);
		auto l_convolved_3d = convolve(a_x, parameters(3, 50, 50), 50);
		l_convolved_3d = bias(l_convolved_3d);
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
