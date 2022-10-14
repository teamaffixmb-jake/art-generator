#include "aurora-v4/aurora.h"
#include "art_models.h"
#include "affix-base/serializable.h"
#include <iostream>
#include <filesystem>
#include "cryptopp/osrng.h"

typedef std::vector<std::vector<std::vector<double>>> training_set_input;

bool import_params(
	aurora::latent::parameter_vector & a_parameter_vector
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
	const aurora::latent::parameter_vector& a_parameter_vector
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
	using namespace latent;

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

	std::vector<affix_base::threading::persistent_thread> l_threads(std::thread::hardware_concurrency() /  2);

	size_t l_export_parameter_vector_interval = 100;
	size_t l_console_out_discriminator_cost_interval = 1;

	CryptoPP::AutoSeededRandomPool l_random;
	
	double l_discriminator_cost_momentum = 1;
	double l_minimum_discriminator_cost_for_beginning_training_set_generation = 0.1;
	double l_minimum_discriminator_cost_for_training_set_acceptance = 0.01;

	for (int epoch = 0; true; epoch++)
	{
		if (epoch == 0 || l_discriminator_cost_momentum < l_minimum_discriminator_cost_for_beginning_training_set_generation)
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

				for (int l_generation_epoch = 0; l_cost > l_minimum_discriminator_cost_for_training_set_acceptance; l_generation_epoch++)
				{
					l_element_vector.fwd();
					l_loss.m_partial_gradient = 1;
					l_cost = l_loss.m_state;
					l_element_vector.bwd();
					l_input_optimizer.update();

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
			randomly_modulate_state(pointers(l_x[i]), -0.01, 0.01);
		}

		l_element_vector.fwd();
		l_loss.m_partial_gradient = 1;
		l_discriminator_cost_momentum = 0.9 * l_discriminator_cost_momentum + 0.1 * l_loss.m_state;
		l_element_vector.bwd();
		l_optimizer.update();

		if (epoch % l_console_out_discriminator_cost_interval == 0)
			std::cout << "DISCRIMINATOR COST: " << l_discriminator_cost_momentum << std::endl;

	}

}

int main(

)
{
	train_model();

	return 0;

}
