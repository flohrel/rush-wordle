#ifndef __INTERFACE__H__
# define __INTERFACE__H__

# include <iostream>

# include "Data.hpp"
# include "State.hpp"

namespace wordle {

class Interface
{

public:
	Interface();
	~Interface();

	void 			routine();
	static void		displayRefresh();
	static void		printHeader();
	static void		printBody();
	static void		printFooter();

};

} // wordle

#endif