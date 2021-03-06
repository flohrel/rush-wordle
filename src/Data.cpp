# include "Data.hpp"

namespace wordle {

Data::Data()
: m_words()
{
	std::ifstream	dict_file(Data::s_default_dict);
	std::string		word;

	if ( !dict_file.is_open() )
	{
		std::cerr << "wordle: " << Data::s_default_dict << ": Cannot open file." << std::endl;
		exit(EXIT_FAILURE);
	}
	while ( std::getline(dict_file, word) )
	{
		if ( word.empty() )
			continue ;
		m_words.insert(word);
	}
	if ( m_words.empty() )
	{
		std::cerr << "wordle: Empty dictionary." << std::endl;
		exit(EXIT_FAILURE);
	}
	m_word_list.reserve(m_words.size());
	for ( auto it = m_words.begin(); it != m_words.end(); it++ )
	{
		m_word_list.push_back(*it);
	}
	return ;
}

Data::~Data()
{
	return ;
}

Data&	Data::instance()
{
	static Data the_inst;
	return the_inst;
}

size_t	Data::size() const
{
	return ( m_words.size() );
}

std::string const&	Data::get_random_word() const
{
	std::random_device											dev;
    std::mt19937												rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type>	dist(0, m_word_list.size());

	return ( *m_words.find( m_word_list[dist(rng)] ) );
}

}	// namespace wordle