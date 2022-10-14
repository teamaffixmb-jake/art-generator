#pragma once
#include "aurora-v4/aurora.h"
#include "affix-base/files.h"

namespace art_generator
{
	inline double discriminate(
		const std::vector<std::vector<std::vector<double>>>& a_x,
		aurora::oneshot::parameter_vector& a_parameter_vector
	)
	{
		using namespace aurora;
		using namespace oneshot;
		
		auto l_convolved_3d = convolve(a_x, a_parameter_vector.next(3, 50, 50), 50);
		l_convolved_3d = add(l_convolved_3d, a_parameter_vector.next(l_convolved_3d.size(), l_convolved_3d[0].size()));
		l_convolved_3d = leaky_relu(l_convolved_3d, 0.3);
		
		auto l_flattened = flatten(l_convolved_3d);

		std::vector<size_t> l_tnn_dimensions = { l_flattened.size() / 2, l_flattened.size() / 5 };

		for (const auto& l_dimension : l_tnn_dimensions)
		{
			l_flattened = multiply(a_parameter_vector.next(l_dimension, l_flattened.size()), l_flattened);
			l_flattened = add(a_parameter_vector.next(l_dimension), l_flattened);
			l_flattened = leaky_relu(l_flattened, 0.3);
		}

		l_flattened = multiply(a_parameter_vector.next(1, l_flattened.size()), l_flattened);
		l_flattened = add(a_parameter_vector.next(1), l_flattened);
		l_flattened = sigmoid(l_flattened);

		return l_flattened[0];

	}

}
