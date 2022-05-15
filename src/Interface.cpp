# include "Interface.hpp"

namespace wordle
{

namespace {
	Data const&	dictionary = Data::instance();
	State&		state = State::instance();
}

void	Interface::printHeader()
{
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

	for (size_t i = 0; i != 16; i++)
	{
		mvprintw( (LINES / 2) - 20 + i, (COLS / 2) - ((header[i].size() / 2) + header[i].size() % 2), header[i].c_str());
	}
	mvprintw( (LINES / 2) - 2, COLS / 2 - 35, nb_words.c_str());
	attron( A_BOLD | A_DIM );
	printw( std::to_string( dictionary.size() ).c_str() );
	attroff( A_BOLD | A_DIM );
}

void	Interface::printBody()
{
	size_t	cursor_pos[2];

	for (size_t i = 0; i != State::s_nguess; i++)
	{
		for (size_t j = 0; j != State::s_nchar; j++)
		{
			cursor_pos[0] = (LINES / 2) + (i * 2) + 2;
			cursor_pos[1] = (COLS / 2) - 35 + (j * 2);
			attron( COLOR_PAIR( state.m_char_matrix[i][j].second ) );
			switch ( state.m_char_matrix[i][j].second )
			{
				case CS_EMPTY:
					mvprintw( cursor_pos[0], cursor_pos[1], " " );
					break;
				case CS_NOTFOUND:
					attron( A_DIM );
					/* fall through */
				default:
					mvprintw( cursor_pos[0], cursor_pos[1], &state.m_char_matrix[i][j].first );
					break;
			}
			attroff( COLOR_PAIR( state.m_char_matrix[i][j].second ) | A_DIM );
			attron( A_NORMAL );
			printw( " " );
		}
	}
}

void	Interface::printFooter()
{
	move( (LINES / 2) + (State::s_nguess * 2) + 4, (COLS / 2) - 35 );
	printw( "Press");
	attron( A_BOLD | A_DIM );
	printw( " DEL " );
	attroff( A_BOLD | A_DIM );
	printw( "to quit." );
	move( (LINES / 2) + (State::s_nguess * 2) + 6, (COLS / 2) - 35 );
	attron( COLOR_PAIR( 6 ) );
	switch (state.m_error)
	{
		case ST_NOEXIST:
			printw( "NOT in dictionary" );
			break ;
		case ST_BADCHAR:
			printw( "MUST be an alphabetic char" );
			break ;
		case ST_FAIL:
			printw( "FAILED ! The word was: " );
			printw( state.getWord().c_str() );
			break ;
		case ST_WIN:
			printw( "WIN !" );
			break ;
		default:
			break;
	}
	attroff( COLOR_PAIR( 6 ) );
}

void	Interface::routine()
{
	std::string str;

	while ( 42 )
	{
		state.reset();
		state.setWord( dictionary.get_random_word() );
		displayRefresh();
		for (size_t i = 0; i != State::s_nguess; i++)
		{
			for (size_t j = 0; j != State::s_nchar; j++)
			{
				state.m_current_input = 0;
				while ( !isalpha( state.m_current_input ) )
				{
					if ( state.m_current_input != 0 &&
						state.m_current_input != 410 &&
						state.m_current_input != KEY_BACKSPACE )
						state.m_error = ST_BADCHAR;
					state.m_cursor_pos[0] = (LINES / 2) + (i * 2) + 2;
					state.m_cursor_pos[1] = (COLS / 2) - 35 + (j * 2);
					displayRefresh();
					switch (state.m_current_input)
					{
						case KEY_DC :
							return ;
						case KEY_BACKSPACE :
							if (j != 0)
							{
								j--;
								state.m_char_matrix[i][j].first = 0;
							}
							break ;
						default:
							break;
					}
				}
				state.m_char_matrix[i][j].first = toupper( state.m_current_input );
				state.m_char_matrix[i][j].second = CS_UNCHECKED;
			}
			str.clear();
			for (size_t j = 0; j != State::s_nchar; j++)
			{
				str += tolower( state.m_char_matrix[i][j].first );
			}
			if ( dictionary.m_words.contains(str) == false ) {
				state.m_error = ST_NOEXIST;
				state.clearGuess(i);
				i--;
			} else {
				state.checkWord(i);
				if ( str == state.getWord() ) {
					state.m_error = ST_WIN;
					break ;
				}
			}
			if ( i == (State::s_nguess - 1) && state.m_error != ST_WIN ) {
				state.m_error = ST_FAIL;
			}
		}
	}
}

void	Interface::displayRefresh()
{
	clear();
	printHeader();
	printBody();
	printFooter();
	state.setInput( mvgetch( state.m_cursor_pos[0], state.m_cursor_pos[1] ) );
	refresh();
}

Interface::Interface()
{
	WINDOW* stdscr = initscr();
	if (start_color() == ERR)
		exit(EXIT_FAILURE);
	keypad( stdscr, TRUE );
	init_pair(1, COLOR_BLACK, COLOR_WHITE);
	init_pair(2, COLOR_WHITE, COLOR_BLACK);
	init_pair(3, COLOR_WHITE, COLOR_BLACK);
	init_pair(4, COLOR_BLACK, COLOR_YELLOW);
	init_pair(5, COLOR_BLACK, COLOR_GREEN);
	init_pair(6, COLOR_RED, COLOR_BLACK);
	init_pair(7, COLOR_GREEN, COLOR_BLACK);
}

Interface::~Interface()
{
    if (endwin() == ERR)
		exit(EXIT_FAILURE);
}

} // namespace wordle
