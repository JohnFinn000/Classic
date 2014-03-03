/*
 * =====================================================================================
 *
 *       Filename:  snake.cpp
 *
 *    Description:  simple snake game
 *
 *        Version:  1.0
 *        Created:  11/01/2013 07:06:54 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  John Finn (), johnvincentfinn@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */
#include <stdlib.h>
#include <string.h>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include "snake.h"

/*
 *--------------------------------------------------------------------------------------
 *       Class:  Snake
 *      Method:  Snake
 * Description:  constructor
 *--------------------------------------------------------------------------------------
 */

Snake::Snake( int nlines, int ncols, int begin_y, int begin_x ) :
    NCursesPanel( nlines, ncols, begin_y, begin_x ) {
    // initialize the snake
    last_dir = dir = dir_down;
    neck_x = tail_x = head_x = ncols/2;
    neck_y = tail_y = head_y = nlines/2;

    length = 1;
    max_length = 3;

    // initialize window attributes
    set_size( ncols, nlines );
    
    box( 0, 0 );
    refresh( );
    halfdelay( 2 );
    
    // initialize the board
    board = new int*[max_x];
    for( int x = 0; x <= max_x; ++x ) {
        board[x] = new int[max_y];
    }

    for( int x = 0; x <= max_x; ++x ) {
        for( int y = 0; y <= max_y; ++y ) {
            board[x][y] = -1;
        }
    }

    srand( settings.seed );

    int num_food = nlines * ncols * settings.food_rarity;
    for( int i = 0; i < num_food; ++i ) {
        drop_food();
    }

}  /* -----  end of method Snake::Snake  (constructor)  ----- */


void Snake::set_size( int width, int height ) {
    if( width < 3 ) {
        //THROW AN ERROR
        width = 3;
    }
    if( height < 3 ) {
        //THROW AN ERROR
        height = 3;
    }

    max_x = width - 2;
    max_y = height - 2;
    return;
}		/* -----  end of method Snake::set_size  ----- */



void Snake::change_direction( direction dir ) {
    
    switch ( dir ) {
    case dir_up:
        if( this->dir == dir_down ) {
            return;
        }
        break;
    case dir_down:
        if( this->dir == dir_up ) {
            return;
        }
        break;
    case dir_left:
        if( this->dir == dir_right ) {
            return;
        }
        break;
    case dir_right:
        if( this->dir == dir_left ) {
            return;
        }
        break;
    }				/* -----  end switch  ----- */
    last_dir = this->dir;
    this->dir = dir;
    return;
}		/* -----  end of method Snake::change_direction  ----- */

void Snake::drop_food() {
    int x;
    int y;
    do {
        x = (rand() % max_x) + 1;
        y = (rand() % max_y) + 1;
    } while( board[x][y] != -1 );

    drop_food( x, y );
}

void Snake::drop_food( int x, int y ) {
    if( x <= 0 || y <= 0 || x > max_x || y > max_y ) {
        return;
    }
    attrset( COLOR_PAIR(settings.food[0].color) );
    board[x][y] = dir_food;
    addch( y, x, settings.food[0].mark );
}

int Snake::tick() {
    
    // handle collisions
    switch( board[head_x][head_y] ) {
    case dir_food:
        ++max_length;
        drop_food();
        break;
    case dir_super_food:
        max_length += 5;
        drop_food();
        break;
    case dir_up:
    case dir_down:
    case dir_left:
    case dir_right:
        return GAME_OVER;
        break;
    }

    ++length;

    // leave directions for the rest of the snakes body
    board[head_x][head_y] = dir;

    // change to snake body color
    attrset( COLOR_PAIR(settings.snake.color) );

    // draw the head
    switch( dir ) {
    case dir_up:
        if( head_y <= 1 ) {
            head_y = max_y;
        } else {
            --head_y;
        }
        addch( head_y, head_x, settings.snake.head_mark.up );
        break;
    case dir_down:
        if( head_y >= max_y ) {
            head_y = 1;
        } else {
            ++head_y;
        }
        addch( head_y, head_x, settings.snake.head_mark.down );
        break;
    case dir_left:
        if( head_x <= 1 ) {
            head_x = max_x;
        } else {
            --head_x;
        }
        addch( head_y, head_x, settings.snake.head_mark.left );
        break;
    case dir_right:
        if( head_x >= max_x ) {
            head_x = 1;
        } else {
            ++head_x;
        }
        addch( head_y, head_x, settings.snake.head_mark.right );
        break;
    }


    // draw the neck (which becomes the body)
    switch( board[neck_x][neck_y] ) {
    case dir_up:
        switch( last_dir ) {
        case dir_left:
            addch( neck_y, neck_x, settings.snake.body_mark.ll );
            break;
        case dir_right:
            addch( neck_y, neck_x, settings.snake.body_mark.lr );
            break;
        default:
            addch( neck_y, neck_x, settings.snake.body_mark.up );
        }
        if( neck_y <= 1 ) {
            neck_y = max_y;
        } else {
            --neck_y;
        }
        break;
    case dir_down:
        switch( last_dir ) {
        case dir_left:
            addch( neck_y, neck_x, settings.snake.body_mark.ul );
            break;
        case dir_right:
            addch( neck_y, neck_x, settings.snake.body_mark.ur );
            break;
        default:
            addch( neck_y, neck_x, settings.snake.body_mark.down );
        }
        if( neck_y >= max_y ) {
            neck_y = 1;
        } else {
            ++neck_y;
        }
        break;
    case dir_left:
        switch( last_dir ) {
        case dir_up:
            addch( neck_y, neck_x, settings.snake.body_mark.ur );
            break;
        case dir_down:
            addch( neck_y, neck_x, settings.snake.body_mark.lr );
            break;
        default:
            addch( neck_y, neck_x, settings.snake.body_mark.left );
        }
        if( neck_x <= 1 ) {
            neck_x = max_x;
        } else {
            --neck_x;
        }
        break;
    case dir_right:
        switch( last_dir ) {
        case dir_up:
            addch( neck_y, neck_x, settings.snake.body_mark.ul );
            break;
        case dir_down:
            addch( neck_y, neck_x, settings.snake.body_mark.ll );
            break;
        default:
            addch( neck_y, neck_x, settings.snake.body_mark.right );
        }
        if( neck_x >= max_x ) {
            neck_x = 1;
        } else {
            ++neck_x;
        }
        break;
    }
    last_dir = dir;

    // remove any tail segments
    while( length > max_length ) {
        switch( board[tail_x][tail_y] ) {
        case dir_up:
            addch( tail_y, tail_x, ' ' );
            board[tail_x][tail_y] = -1;
            if( tail_y <= 1 ) {
                tail_y = max_y;
            } else {
                --tail_y;
            }
            break;
        case dir_down:
            addch( tail_y, tail_x, ' ' );
            board[tail_x][tail_y] = -1;
            if( tail_y >= max_y ) {
                tail_y = 1;
            } else {
                ++tail_y;
            }
            break;
        case dir_left:
            addch( tail_y, tail_x, ' ' );
            board[tail_x][tail_y] = -1;
            if( tail_x <= 1 ) {
                tail_x = max_x;
            } else {
                --tail_x;
            }
            break;
        case dir_right:
            addch( tail_y, tail_x, ' ' );
            board[tail_x][tail_y] = -1;
            if( tail_x >= max_x ) {
                tail_x = 1;
            } else {
                ++tail_x;
            }
            break;
        }
        --length;
    }
    refresh();
    return DEFAULT;
}		/* -----  end of method Snake::tick  ----- */


/*
 *--------------------------------------------------------------------------------------
 *       Class:  Controls
 *      Method:  Controls
 * Description:  constructor
 *--------------------------------------------------------------------------------------
 */
Controls::Controls( Snake *snake ) {
    this->snake = snake;
}  /* -----  end of method Controls::Controls  (constructor)  ----- */


int Controls::get_input( int input_char ) {
    switch( input_char ) {
    case KEY_BACKSPACE:
    case KEY_ENTER:
        return TOGGLE_MAIN_MENU;
        break;
    case KEY_UP:
        up();
        break;
    case KEY_DOWN:
        down();
        break;
    case KEY_LEFT:
        left();
        break;
    case KEY_RIGHT:
        right();
        break;
    case ERR:
    default:
        break;
    }
    return 0;
}		/* -----  end of method Controls::grab  ----- */


void Controls::up() {
    snake->change_direction( dir_up );
    return ;
}		/* -----  end of method Controls::up  ----- */


void Controls::down() {
    snake->change_direction( dir_down );
    return ;
}		/* -----  end of method Controls::down  ----- */


void Controls::left() {
    snake->change_direction( dir_left );
    return ;
}		/* -----  end of method left::left  ----- */


void Controls::right() {
    snake->change_direction( dir_right );
    return ;
}		/* -----  end of method Controls::right  ----- */

/*
 *--------------------------------------------------------------------------------------
 *       Class:  Main_menu
 *      Method:  Main_menu
 * Description:  constructor
 *--------------------------------------------------------------------------------------
 */

Main_menu::Main_menu( int nlines, int ncols, int begin_y, int begin_x ) : 
    NCursesMenu( nlines, ncols, begin_y, begin_x ),
    panel(0),

    menu_item_array(0) {
    menu_item_array = new NCursesMenuItem*[5];

    menu_item_array[0] = new NCursesMenuCallbackItem( resume_callback,  "Resume",   "Continue playing the game" );
    menu_item_array[1] = new NCursesMenuCallbackItem( options_callback, "Options",  "Edit the game options" );
    menu_item_array[2] = new NCursesMenuCallbackItem( save_callback,    "Save",     "Save this game for future play" );
    menu_item_array[3] = new NCursesMenuCallbackItem( quit_callback,    "Quit",     "Leave the game" );
    menu_item_array[4] = new NCursesMenuItem( );


    InitMenu( menu_item_array, TRUE, FALSE );


    //panel = new NCursesPanel( 1, 5, 10, 1 );
    //panel->show();
    
    //setItems( menu_item_array );

}  /* -----  end of method Menu::Menu  (constructor)  ----- */

Main_menu::~Main_menu() {

}

bool Main_menu::resume_callback( NCursesMenuItem &item ) {
    
}

bool Main_menu::save_callback( NCursesMenuItem &item ) {

}

bool Main_menu::quit_callback( NCursesMenuItem &item ) {

}

bool Main_menu::options_callback( NCursesMenuItem &item ) {

}



void Main_menu::toggle( ) {
    if( hidden( ) ) {
        show();
    } else {
        hide();
    }
    return ;
}		/* -----  end of method Menu::toggle  ----- */

/*
 *--------------------------------------------------------------------------------------
 *       Class:  Game
 *      Method:  Game
 * Description:  constructor
 *--------------------------------------------------------------------------------------
 */
Game::Game() {
    initialize();
    run();
    finish(0);
}  /* -----  end of method Game::Game  (constructor)  ----- */

Game::~Game() {
    delete( snake );
    delete( controls );
    delete( main_menu );
}

void Game::initialize () {
    int num = 0;

    signal( SIGINT, finish );
    signal( SIGWINCH, resize );

    // initializing stuff
    initscr();
    keypad(stdscr, TRUE);
    nonl();
    cbreak();
    halfdelay(2);
    noecho();
    curs_set( 0 );

    if( has_colors() ) {
        start_color();

        init_pair( 1, COLOR_RED, COLOR_BLACK );
        init_pair( 2, COLOR_GREEN, COLOR_BLACK );
        init_pair( 3, COLOR_YELLOW, COLOR_BLACK );
        init_pair( 4, COLOR_BLUE, COLOR_BLACK );
        init_pair( 5, COLOR_WHITE, COLOR_BLACK );
    }


    load_configuration("config.xml");
    
    main_menu     = new Main_menu(  8, 50, 4, 4 );
    snake         = new Snake( settings.board_size.y, settings.board_size.x, 5, 5 );
    controls      = new Controls( snake );

    main_menu->top();
    
    

    return ;
}		/* -----  end of method Game::initialize  ----- */

void Game::load_configuration( const std::string &filename ) {

    using boost::property_tree::ptree;
    ptree configuration;
    //read_xml( filename, configuration );
    settings.seed                   = configuration.get("settings.seed", 1 );

    settings.food[0].color          = configuration.get("settings.food.color", 4 );
    settings.food[0].mark           = configuration.get("settings.food.mark", ACS_DIAMOND );
    settings.food[0].points         = configuration.get("settings.food.points", 1 );
    settings.food[0].ratio          = configuration.get("settings.food.ratio", 5 );

    settings.food_rarity            = configuration.get("settings.food_rarity", .1 );

    settings.snake.color            = configuration.get("settings.snake.color", 1 );

    settings.snake.body_mark.ur     = configuration.get("settings.snake.body_mark.ur", ACS_URCORNER );
    settings.snake.body_mark.lr     = configuration.get("settings.snake.body_mark.lr", ACS_LRCORNER );
    settings.snake.body_mark.ul     = configuration.get("settings.snake.body_mark.ul", ACS_ULCORNER );
    settings.snake.body_mark.ll     = configuration.get("settings.snake.body_mark.ll", ACS_LLCORNER );
    settings.snake.body_mark.up     = configuration.get("settings.snake.body_mark.up", ACS_VLINE );
    settings.snake.body_mark.down   = configuration.get("settings.snake.body_mark.down", ACS_VLINE );
    settings.snake.body_mark.left   = configuration.get("settings.snake.body_mark.left", ACS_HLINE );
    settings.snake.body_mark.right  = configuration.get("settings.snake.body_mark.right", ACS_HLINE );

    settings.snake.head_mark.up     = configuration.get("settings.snake.head_mark.up", ACS_UARROW );
    settings.snake.head_mark.down   = configuration.get("settings.snake.head_mark.down", ACS_DARROW );
    settings.snake.head_mark.left   = configuration.get("settings.snake.head_mark.left", ACS_LARROW );
    settings.snake.head_mark.right  = configuration.get("settings.snake.head_mark.right", ACS_RARROW );

    settings.commands.up            = configuration.get("settings.commands.up", KEY_UP );
    settings.commands.down          = configuration.get("settings.commands.down", KEY_DOWN );
    settings.commands.left          = configuration.get("settings.commands.left", KEY_LEFT );
    settings.commands.right         = configuration.get("settings.commands.right", KEY_RIGHT );
    settings.commands.toggle_menu   = configuration.get("settings.commands.toggle_menu", KEY_BACKSPACE );

    settings.board_size.x           = configuration.get("settings.board_size.x", 100 );
    settings.board_size.y           = configuration.get("settings.board_size.y", 30 );
}

void Game::run() {

    int results;
    game_mode current_mode = state_play;
    for(;;) {
        int input_char = getch();
        switch( current_mode ) {
        case state_play:
            results = controls->get_input( input_char );
            snake->tick();
            break;
        case state_menu:
            main_menu->virtualize( input_char );
            break;
        default:
            break;
        }

        switch( results ) {
            case TOGGLE_MAIN_MENU:
                if( current_mode == state_play ) {
                    main_menu->post();
                    current_mode = state_menu;
                } else {
                    main_menu->unpost();
                    current_mode = state_play;
                }
                results = 0;
                break;
            case EXIT:
                return;
            default:
                break;
        }
        
        update_panels();
        doupdate();
        
    }
    
    return ;
}		/* -----  end of method Game::run  ----- */



void Game::finish( int sig ) {

    endwin();
    exit( 0 );
    return;
}		/* -----  end of method Game::finish  ----- */

void Game::resize( int sig ) {
    
    int new_height;
    int new_width;
    getmaxyx( stdscr, new_height, new_width );

    return;
}		/* -----  end of method Game::resize  ----- */


Game *game = new Game();

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  main
 *  Description:  
 * =====================================================================================
 */
int main ( int argc, char *argv[] ) {


    return EXIT_SUCCESS;
}				/* ----------  end of function main  ---------- */

