#ifndef __STATE__H__
# define __STATE__H__

# include <utility>
# include <string>
# include <vector>
# include <cctype>
# include <ncurses.h>

# include "Interface.hpp"

namespace wordle
{

enum e_char_state {
	CS_DEFAULT = 0,
	CS_EMPTY = 1,
	CS_UNCHECKED = 2,
	CS_NOTFOUND = 3,
	CS_BADSPOT = 4,
	CS_FOUND = 5
};

enum e_error {
	ST_NOERROR,
	ST_NOEXIST,
	ST_BADCHAR,
	ST_FAIL,
	ST_WIN
};

class State
{

private:
	State();
	~State() {}

public:
	static State&		instance();

	void				reset();
	void				setWord( std::string const& word );
	std::string const&	getWord() const;
	void				setInput( int input );
	void				clearGuess( unsigned int nguess );
	void				checkWord( unsigned int nguess );

	static size_t const				s_nchar = 5;
	static size_t const				s_nguess = 6;
	e_error							m_error;
	size_t							m_cursor_pos[2];
	int								m_current_input;
	std::pair<char, e_char_state> 	m_char_matrix[State::s_nguess][State::s_nchar];

private:
	std::string const*				m_current_word;
	

};

}	// namespace wordle

#endif