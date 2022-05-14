# include "Data.hpp"
# include "Interface.hpp"

using namespace wordle;

int main()
{
	Data 		dictionary;

	Interface::display(dictionary);
	
	std::cout << dictionary.get_random_word() << std::endl;

	return 0;
}