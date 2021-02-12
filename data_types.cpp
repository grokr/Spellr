#include "data_types.h"

typo_long_num ato_typo_long_num(const typo_char *s)
{	
#ifdef _DOUBLE
	return atof(s);
#endif

#ifdef _INT
	return atoi(s);
#endif
	return atoi(s);
}


