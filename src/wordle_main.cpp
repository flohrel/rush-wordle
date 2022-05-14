# include "Data.hpp"
# include "Interface.hpp"

using namespace wordle;

int main()
{
	Data 		dictionary;

	Interface::printHeader();
	
	std::cout << "Total words available: " << dictionary.size() << std::endl;
	std::cout << dictionary.get_random_word() << std::endl;

	return 0;
}