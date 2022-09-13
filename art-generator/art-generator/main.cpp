#include "aurora-v4/aurora.h"
#include "art_models.h"
#include "affix-base/serializable.h"
#include <iostream>
#include <filesystem>

typedef std::vector<std::vector<std::vector<double>>> training_set_input;

//bool try_get_training_set_from_file(
//	const std::string& a_file_path
//)
//{
//
//}

//std::vector<training_set> get_training_sets(
//
//)
//{
//
//}

bool import_params(
	aurora::parameter_vector& a_parameter_vector
)
{
	std::vector<double> l_exported_params;
	if (!affix_base::files::file_read("parameter_vector.bin", l_exported_params))
		return false;
	if (a_parameter_vector.size() != l_exported_params.size())
		return false;
	for (int i = 0; i < a_parameter_vector.size(); i++)
		a_parameter_vector[i]->m_state = l_exported_params[i];
	return true;
}

bool export_params(
	const aurora::parameter_vector& a_parameter_vector
)
{
	std::vector<double> l_exported_params;
	for (int i = 0; i < a_parameter_vector.size(); i++)
		l_exported_params.push_back(a_parameter_vector[i]->m_state);
	return affix_base::files::file_write("parameter_vector.bin", l_exported_params);
}

void train_model(

)
{
	using namespace aurora;

	std::vector<training_set_input> l_human_training_set_inputs;
	std::vector<training_set_input> l_machine_training_set_inputs;

	for (auto l_entry : std::filesystem::directory_iterator("../../processed-data/"))
	{
		training_set_input l_training_set_input;
		if (!affix_base::files::file_read(l_entry.path().u8string(), l_training_set_input))
			throw std::exception();
		l_human_training_set_inputs.push_back(l_training_set_input);
		std::cout << "IMPORTED TRAINING SET: " << l_entry.path().u8string() << std::endl;
	}

	element_vector::start();
	parameter_vector::start();

	std::vector<affix_base::threading::persistent_thread> l_threads(std::thread::hardware_concurrency() / 2);

	parallel_executor::startup(l_threads);

	std::vector<std::vector<std::vector<std::vector<state_gradient_pair>>>> l_x(l_threads.size());
	std::vector<state_gradient_pair*> l_y(l_threads.size());

	join_threads();

	size_t l_parameter_index = parameter_vector::next_index();

	for (int i = 0; i < l_threads.size(); i++)
	{
		parameter_vector::next_index(l_parameter_index);
		element_vector::start();
		l_x[i] = input(3, 400, 400);
		l_y[i] = art_generator::discriminate(pointers(l_x[i]));
		parallel_branch(l_threads[i], element_vector::stop());
		std::cout << "THREAD LOADED: " << i << std::endl;
	}

	join_threads();

	element_vector l_element_vector = element_vector::stop();
	parameter_vector l_parameter_vector = parameter_vector::stop(-1, 1);

	if (import_params(l_parameter_vector))
		std::cout << "FAILED TO IMPORT PARAMETER VECTOR FROM FILE." << std::endl;

	size_t l_machine_training_set_generation_interval = 100;
	size_t l_export_parameter_vector_interval = 100;

	for (int epoch = 0; true; epoch++)
	{
		if (epoch % l_machine_training_set_generation_interval == 0)
		{

		}
		if (epoch % l_export_parameter_vector_interval == 0)
		{
			if (!export_params(l_parameter_vector))
				std::cout << "FAILED TO EXPORT PARAMETER VECTOR FROM FILE." << std::endl;
		}
	}


}

int main(

)
{
	train_model();

	return 0;

}
