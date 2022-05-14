# include "Interface.hpp"

namespace wordle
{

void Interface::display( Data const& dictionary )
{
	initscr();
	std::string	header[16] = {
		"                                                        ,---,                 ",
		"                    ,----..                           ,---.'|              ",
		"           .---.   /   /   \\  ,-.----.       ,---,    |   | :       ,---,. ",
		"          /. ./|  /   .     : \\    /  \\    .'  .' `\\  :   : |     ,'  .' | ",
		"      .--'.  ' ; .   /   ;.  \\;   :    \\ ,---.'     \\ |   ' :   ,---.'   | ",
		"     /__./ \\ : |.   ;   /  ` ;|   | .\\ : |   |  .`\\  |;   ; '   |   |   .' ",
		" .--'.  '   \' .;    |  ; \\ ; |.   : |: | :   : |  '  |'   | |__ :   :  |-, ",
		"/___/ \\ |    ' '|   :  | ; | '|   |  \\ : |   ' '  ;  :|   | :.'|:   |  ;/| ",
		";   \\  \\;      :.   |  ' ' ' :|   : .  / '   | ;  .  |'   :    ;|   :   .' ",
		" \\   ;  `      |'   ;  \\; /  |;   | |  \\ |   | :  |  '|   |  ./ |   |  |-, ",
		"  .   \\    .\\  ; \\   \\  ',  / |   | ;\\  \\'   : | /  ; ;   : ;   '   :  ;/| ",
		"   \\   \\   ' \\ |  ;   :    /  :   ' | \\.'|   | '` ,/  |   ,/    |   |    | ",
		"    :   '  |--\"    \\   \\ .'   :   : :-'  ;   :  .'    '---'     |   :   .' ",
		"     \\   \\ ;        `---`     |   |.'    |   ,.'                |   | ,'   ",
		"      '---\"                   `---'      '---'                  `----'     ",
		"                                                                           "
	};
	std::string	nb_words = "Total words available: ";

	start_color();
	init_pair(1, COLOR_WHITE, COLOR_BLACK);
	init_pair(2, COLOR_BLACK, COLOR_WHITE);
	while (1)
	{
        clear();
		for (size_t i = 0; i != 16; i++)
		{
        	mvprintw( (LINES / 2) - 20 + i, (COLS / 2) - ((header[i].size() / 2) + header[i].size() % 2), header[i].c_str());
		}
		mvprintw( (LINES / 2) - 2, COLS / 2 - 35, nb_words.c_str());
		attron(A_BOLD);
		printw( std::to_string( dictionary.size() ).c_str() );
   		attroff(A_BOLD);
		for (size_t i = 0; i != 6; i++)
		{
			move((LINES / 2) + (i * 2), COLS / 2 - 35);
			for (size_t j = 0; j != 5; j++)
			{
				attron(COLOR_PAIR(1));
				printw( " " );
				attron(COLOR_PAIR(2));
				printw( " " );
				attroff(COLOR_PAIR(2));
			}
		}
        refresh();
        if (getch() != 410)
		{
            break ;
		}
	}
    endwin();
}

} // namespace wordle
