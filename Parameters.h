#ifndef __PARAMETERS_H__
#define __PARAMETERS_H__

namespace SpellChecker
{
	struct Parameters{
		typo_long_num full_size_bonus;
		typo_long_num first_part_bonus;
		typo_long_num middle_part_bonus;
		typo_long_num ending_bonus;
		typo_long_num non_comparable_bonus;
		typo_long_num letterset_difference_coeff;
		typo_long_num same_letterset_bonus;
		int shift_max;
		int substr_length_max;
		int length_difference_coeff;
	};
	//Parameters *_pParams;
};


#endif //__PARAMETERS_H__