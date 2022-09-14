#include "aurora-v4/aurora.h"
#include "art_models.h"
#include "affix-base/serializable.h"
#include <iostream>
#include <filesystem>
#include "cryptopp/osrng.h"

typedef std::vector<std::vector<std::vector<double>>> training_set_input;

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

	size_t l_acceptable_training_set_height = 400;
	size_t l_acceptable_training_set_width = 400;

	for (auto l_entry : std::filesystem::directory_iterator("../../processed-data/"))
	{
		training_set_input l_training_set_input;
		if (!affix_base::files::file_read(l_entry.path().u8string(), l_training_set_input))
			throw std::exception();
		if (l_training_set_input[0].size() != l_acceptable_training_set_height || l_training_set_input[0][0].size() != l_acceptable_training_set_width)
			throw std::exception();
		l_human_training_set_inputs.push_back(l_training_set_input);
		std::cout << "IMPORTED TRAINING SET: " << l_entry.path().u8string() << std::endl;
	}

	element_vector::start();
	parameter_vector::start();

	std::vector<affix_base::threading::persistent_thread> l_threads(2);

	parallel_executor::startup(l_threads);

	std::vector<std::vector<std::vector<std::vector<state_gradient_pair>>>> l_x(l_threads.size());
	std::vector<state_gradient_pair*> l_y(l_threads.size());

	join_threads();

	size_t l_parameter_index = parameter_vector::next_index();

	for (int i = 0; i < l_threads.size(); i++)
	{
		parameter_vector::next_index(l_parameter_index);
		element_vector::start();
		l_x[i] = input(3, l_acceptable_training_set_height, l_acceptable_training_set_width);
		l_y[i] = art_generator::discriminate(pointers(l_x[i]));
		parallel_branch(l_threads[i], element_vector::stop());
		std::cout << "THREAD LOADED: " << i << std::endl;
	}

	join_threads();

	auto l_desired_y = input(l_threads.size());

	std::vector<state_gradient_pair*> l_cross_entropy_ys(l_threads.size());

	for (int i = 0; i < l_cross_entropy_ys.size(); i++)
		l_cross_entropy_ys[i] = cross_entropy(l_y[i], &l_desired_y[i]);

	auto l_loss = average(l_cross_entropy_ys)->depend();

	element_vector l_element_vector = element_vector::stop();
	parameter_vector l_parameter_vector = parameter_vector::stop(-0.1, 0.1);

	if (import_params(l_parameter_vector))
		std::cout << "IMPORTED PARAMETER VECTOR FROM FILE." << std::endl;
	else
		std::cout << "FAILED TO IMPORT PARAMETER VECTOR FROM FILE." << std::endl;

	gradient_descent_with_momentum l_optimizer(l_parameter_vector, false, 0.002, 0.9);

	std::vector<state_gradient_pair*> l_flattened_input;

	for (auto& l_thread_specific_x : l_x)
		l_flattened_input = concat(l_flattened_input, flatten(pointers(l_thread_specific_x)));

	gradient_descent_with_momentum l_input_optimizer(l_flattened_input, true, 0.2, 0.9);

	size_t l_machine_training_set_generation_interval = 100;
	size_t l_export_parameter_vector_interval = 100;
	size_t l_console_out_discriminator_cost_interval = 1;

	CryptoPP::AutoSeededRandomPool l_random;

	for (int epoch = 0; true; epoch++)
	{
		if (epoch % l_machine_training_set_generation_interval == 0)
		{
			// Generate entirely new machine-made training sets.

			l_machine_training_set_inputs.clear();

			while (l_machine_training_set_inputs.size() < l_human_training_set_inputs.size())
			{
				for (int i = 0; i < l_threads.size(); i++)
				{
					randomize_state(pointers(l_x[i]), 0, 255);
					l_desired_y[i].m_state = 0; // SIGNAL THAT WE WANT IT TO LOOK LIKE A HUMAN MADE THE ART
				}

				double l_cost = 10000;

				for (int l_generation_epoch = 0; l_cost > 0.1; l_generation_epoch++)
				{
					l_element_vector.fwd();
					l_loss.m_partial_gradient = 1;
					l_cost = l_loss.m_state;
					l_element_vector.bwd();
					l_input_optimizer.update();
					l_input_optimizer.m_learn_rate = 200000 * std::log(l_cost + 1);
					std::cout << "MACHINE TRAINING SET GENERATION COST: " << l_cost << std::endl;
				}

				for (auto& l_thread_specific_x : l_x)
					l_machine_training_set_inputs.push_back(get_state(pointers(l_thread_specific_x)));

			}
		}
		if (epoch % l_export_parameter_vector_interval == 0)
		{
			if (export_params(l_parameter_vector))
				std::cout << "EXPORTED PARAMETER VECTOR TO FILE." << std::endl;
			else
				std::cout << "FAILED TO EXPORT PARAMETER VECTOR TO FILE." << std::endl;
		}
		for (int i = 0; i < l_threads.size(); i++)
		{
			bool l_is_machine_training_set = l_random.GenerateBit();
			l_desired_y[i].m_state = l_is_machine_training_set;
			if (l_is_machine_training_set)
			{
				size_t l_training_set_index = l_random.GenerateWord32(0, l_machine_training_set_inputs.size() - 1);
				set_state(pointers(l_x[i]), l_machine_training_set_inputs[l_training_set_index]);
			}
			else
			{
				size_t l_training_set_index = l_random.GenerateWord32(0, l_human_training_set_inputs.size() - 1);
				set_state(pointers(l_x[i]), l_human_training_set_inputs[l_training_set_index]);
			}
		}

		l_element_vector.fwd();
		l_loss.m_partial_gradient = 1;
		l_element_vector.bwd();
		//l_optimizer.m_learn_rate = std::log(l_loss.m_state + 1);
		l_optimizer.update();

		if (epoch % l_console_out_discriminator_cost_interval == 0)
			std::cout << "DISCRIMINATOR COST: " << l_loss.m_state << std::endl;

	}

}

int main(

)
{
	train_model();

	return 0;

}
