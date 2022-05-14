#ifndef __INTERFACE__H__
# define __INTERFACE__H__

# include <iostream>
# include <ncurses.h>

# include "Data.hpp"

namespace wordle {

class Interface
{

public:
	Interface( void ) {}
	~Interface( void ) {}

	static void display( Data const& dictionary );

private:
	

};

} // wordle

#endif