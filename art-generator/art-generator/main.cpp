#include "aurora-v4/aurora.h"
#include "art_models.h"
#include "affix-base/serializable.h"

struct training_set : public affix_base::data::serializable
{
	std::vector<std::vector<std::vector<double>>> m_x;
	double m_y = 0;

	training_set(

	) :
		affix_base::data::serializable(m_x, m_y)
	{

	}

	training_set(
		const std::vector<std::vector<std::vector<double>>>& a_x,
		const double& a_y
	) :
		affix_base::data::serializable(m_x, m_y),
		m_x(a_x),
		m_y(a_y)
	{

	}

};

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

void train_model(

)
{

}

void generate_art(

)
{

}

int main(

)
{
	using namespace aurora;

	training_set l_training_set({ { { 1, 1 }, { 2, 2 } }, { { 3, 3 }, { 4, 4 } } }, 1);

	affix_base::files::file_write("test.bin", l_training_set);

	return 0;

}
