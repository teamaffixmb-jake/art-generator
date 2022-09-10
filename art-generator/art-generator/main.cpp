#include "aurora-v4/aurora.h"
#include "art_models.h"

int main(

)
{
	using namespace aurora;

	auto l_x = input(3, 200, 200);

	element_vector::start();
	parameter_vector::start();

	auto l_y = art_generator::discriminate(pointers(l_x));

	element_vector l_element_vector = element_vector::stop();
	parameter_vector l_parameter_vector = parameter_vector::stop(-1, 1);

	return 0;

}
