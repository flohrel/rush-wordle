#ifndef __DATA__H__
# define __DATA__H__

# include <unordered_set>
# include <iostream>
# include <fstream>
# include <string>
# include <vector>
# include <random>

namespace wordle {

class Data
{

private:
	Data();
	~Data();

public:
	static Data&		instance();

	size_t				size() const;

	std::string const&	get_random_word() const;

	std::unordered_set<std::string>		m_words;

private:
	static constexpr const char*		s_default_dict = "ressources/words.txt";
	std::vector<std::string>			m_word_list;

};

}

#endif