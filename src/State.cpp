# include "State.hpp"

namespace wordle
{

State::State() : m_error(ST_NOERROR)
{
	reset();
	return ;
}

State&	State::instance()
{
	static State the_inst;
	return the_inst;
}

void	State::reset()
{
	m_current_input = -1;
	m_current_word = nullptr;
	m_cursor_pos[0] = 0;
	m_cursor_pos[1] = 0;
	for (size_t i = 0; i < State::s_nguess; i++)
	{
		for (size_t j = 0; j < State::s_nchar; j++)
		{
			m_char_matrix[i][j] = std::make_pair(0, CS_EMPTY);
		}
	}
}

void	State::setWord( std::string const& word )
{
	m_current_word = &word;
}

std::string const&	State::getWord() const
{
	return (*m_current_word);
}

void	State::setInput( int input )
{
	m_current_input = input;
}

void	State::clearGuess( unsigned int nguess )
{
	for (size_t i = 0; i != State::s_nchar; i++)
	{
		m_char_matrix[nguess][i].first = 0;
		m_char_matrix[nguess][i].second = CS_EMPTY;
	}
}

void	State::checkWord( unsigned int nguess )
{
	for (size_t i = 0; i != State::s_nchar; i++)
	{
		if ( m_current_word->find( tolower( m_char_matrix[nguess][i].first ) ) != std::string::npos ) {
			if ( (*m_current_word)[i] == tolower( m_char_matrix[nguess][i].first ) ) {
				m_char_matrix[nguess][i].second = CS_FOUND;
			} else {
				m_char_matrix[nguess][i].second = CS_BADSPOT;
			}
		} else {
			m_char_matrix[nguess][i].second = CS_NOTFOUND;
		}
	}
}

}	// namespace wordle