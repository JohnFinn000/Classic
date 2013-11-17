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
#include "snake.h"

/*
 *--------------------------------------------------------------------------------------
 *       Class:  Snake
 *      Method:  Snake
 * Description:  constructor
 *--------------------------------------------------------------------------------------
 */
Snake::Snake( ) {

    // initialize the snake
    last_dir = dir = dir_down;
    neck_x = tail_x = head_x = 7;
    neck_y = tail_y = head_y = 7;

    length = 1;
    max_length = 3;

    // initialize window attributes
    window_x = 5;
    window_y = 5;
    set_size( settings.board_size.x, settings.board_size.y );
    
    window = newwin( window_width, window_height, window_x, window_y );
    panel = new_panel( window );
    //wborder( window, 0, 0, 0, 0, 0, 0, 0, 0 );
    box( window, 0, 0 );
    //wrefresh( window );
    
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

    for( int i = 0; i < settings.num_food; ++i ) {
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
    window_width = width;
    window_height = height;

    max_x = window_width - 2;
    max_y = window_height - 2;
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
    wattrset( window, COLOR_PAIR(settings.color.food) );
    board[x][y] = dir_food;
    mvwaddch( window, y, x, settings.food_mark );
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
    wattrset( window, COLOR_PAIR(settings.color.snake) );

    // draw the head
    switch( dir ) {
    case dir_up:
        if( head_y <= 1 ) {
            head_y = max_y;
        } else {
            --head_y;
        }
        mvwaddch( window, head_y, head_x, settings.snake_head_mark.up );
        break;
    case dir_down:
        if( head_y >= max_y ) {
            head_y = 1;
        } else {
            ++head_y;
        }
        mvwaddch( window, head_y, head_x, settings.snake_head_mark.down );
        break;
    case dir_left:
        if( head_x <= 1 ) {
            head_x = max_x;
        } else {
            --head_x;
        }
        mvwaddch( window, head_y, head_x, settings.snake_head_mark.left );
        break;
    case dir_right:
        if( head_x >= max_x ) {
            head_x = 1;
        } else {
            ++head_x;
        }
        mvwaddch( window, head_y, head_x, settings.snake_head_mark.right );
        break;
    }


    // draw the neck (which becomes the body)
    switch( board[neck_x][neck_y] ) {
    case dir_up:
        switch( last_dir ) {
        case dir_left:
            mvwaddch( window, neck_y, neck_x, settings.snake_body_mark.ll );
            break;
        case dir_right:
            mvwaddch( window, neck_y, neck_x, settings.snake_body_mark.lr );
            break;
        default:
            mvwaddch( window, neck_y, neck_x, settings.snake_body_mark.up );
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
            mvwaddch( window, neck_y, neck_x, settings.snake_body_mark.ul );
            break;
        case dir_right:
            mvwaddch( window, neck_y, neck_x, settings.snake_body_mark.ur );
            break;
        default:
            mvwaddch( window, neck_y, neck_x, settings.snake_body_mark.down );
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
            mvwaddch( window, neck_y, neck_x, settings.snake_body_mark.ur );
            break;
        case dir_down:
            mvwaddch( window, neck_y, neck_x, settings.snake_body_mark.lr );
            break;
        default:
            mvwaddch( window, neck_y, neck_x, settings.snake_body_mark.left );
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
            mvwaddch( window, neck_y, neck_x, settings.snake_body_mark.ul );
            break;
        case dir_down:
            mvwaddch( window, neck_y, neck_x, settings.snake_body_mark.ll );
            break;
        default:
            mvwaddch( window, neck_y, neck_x, settings.snake_body_mark.right );
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
            mvwaddch( window, tail_y, tail_x, ' ' );
            board[tail_x][tail_y] = -1;
            if( tail_y <= 1 ) {
                tail_y = max_y;
            } else {
                --tail_y;
            }
            break;
        case dir_down:
            mvwaddch( window, tail_y, tail_x, ' ' );
            board[tail_x][tail_y] = -1;
            if( tail_y >= max_y ) {
                tail_y = 1;
            } else {
                ++tail_y;
            }
            break;
        case dir_left:
            mvwaddch( window, tail_y, tail_x, ' ' );
            board[tail_x][tail_y] = -1;
            if( tail_x <= 1 ) {
                tail_x = max_x;
            } else {
                --tail_x;
            }
            break;
        case dir_right:
            mvwaddch( window, tail_y, tail_x, ' ' );
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
    wrefresh( window );
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
 *       Class:  Menu
 *      Method:  Menu
 * Description:  constructor
 *--------------------------------------------------------------------------------------
 */
Menu::Menu() {
    int num_items;
    // count items
    for( num_items = 0; main_menu_options[num_items][0]; ++num_items );
    // create items
    ITEM **items = (ITEM**) calloc( num_items, sizeof( ITEM* ) );
    for( int i = 0; i < num_items; ++i ) {
        items[i] = new_item( main_menu_options[i][0], main_menu_options[i][1] );
    }
    // create menu
    menu = new_menu( items );
    // create window
    window = newwin( 8, 50, 4, 4 );
    box( window, 0, 0 );
    set_menu_win( menu, window );
    set_menu_sub( menu, derwin( window, 6,48, 2,1 ) );
    set_menu_mark( menu, " * " );
    post_menu( menu );

    // create panel
    panel = new_panel( window );
    hide_panel( panel );
}  /* -----  end of method Menu::Menu  (constructor)  ----- */

Menu::~Menu() {
    unpost_menu( menu );
    free_menu( menu );
}

int Menu::get_input( int input_char ) {

    switch ( input_char ) {
        case KEY_BACKSPACE:
        case KEY_ENTER:
            return TOGGLE_MAIN_MENU;
            break;
        case KEY_DOWN:	
            menu_driver( menu, REQ_DOWN_ITEM );
            break;
        case KEY_UP:	
            menu_driver( menu, REQ_UP_ITEM );
            break;
        case KEY_LEFT:
            {
                ITEM *current;
                current = current_item( menu );
                const char *name = item_name( current );
                int i;
                for( i = 0; main_menu_options[i][0] != NULL; ++i ) {
                    if( strcmp( name, main_menu_options[i][0] ) == 0 ) {
                        break;
                    }
                }
                pos_menu_cursor( menu );
                if( main_menu_options[i][0] == NULL ) {
                    printf("null");
                    break;
                }
                switch( i ) {
                    case 0: //Resume
                        return TOGGLE_MAIN_MENU;
                    case 1: //Options
                        return TOGGLE_MAIN_MENU; // open options menu
                    case 2: //Save
                        return 0; // save the results
                    case 3: //Quit
                        return EXIT;
                        break;
                    default:
                        break;
                }
            }
        default:	
            break;
    }				/* -----  end switch  ----- */
    return 0;
}		/* -----  end of method Menu::get_input  ----- */


void Menu::show( ) {
    show_panel( panel );
    return;
}		/* -----  end of method Menu::show  ----- */

void Menu::hide( ) {
    hide_panel( panel );
    return;
}		/* -----  end of method Menu::hide  ----- */


void Menu::toggle( ) {
    if( panel_hidden( panel ) ) {
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
    delete( menu );
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

    // load default settings
    
    Settings default_settings;

    default_settings.color.food             = 4;
    default_settings.color.snake            = 1;
    default_settings.color.super_food       = 3;

    default_settings.food_mark              = ACS_DIAMOND;
    default_settings.num_food               = 20;
    default_settings.seed                   = 1;

    default_settings.snake_body_mark.ur     = ACS_URCORNER;
    default_settings.snake_body_mark.lr     = ACS_LRCORNER;
    default_settings.snake_body_mark.ul     = ACS_ULCORNER;
    default_settings.snake_body_mark.ll     = ACS_LLCORNER;
    default_settings.snake_body_mark.up     = ACS_VLINE;
    default_settings.snake_body_mark.down   = ACS_VLINE;
    default_settings.snake_body_mark.left   = ACS_HLINE;
    default_settings.snake_body_mark.right  = ACS_HLINE;

    default_settings.snake_head_mark.up     = ACS_UARROW;
    default_settings.snake_head_mark.down   = ACS_DARROW;
    default_settings.snake_head_mark.left   = ACS_LARROW;
    default_settings.snake_head_mark.right  = ACS_RARROW;

    default_settings.commands.up            = KEY_UP;
    default_settings.commands.down          = KEY_DOWN;
    default_settings.commands.left          = KEY_LEFT;
    default_settings.commands.right         = KEY_RIGHT;
    default_settings.commands.toggle_menu   = KEY_BACKSPACE;

    default_settings.board_size.x           = 50;
    default_settings.board_size.y           = 50;

    // set the correct settings for everything
    settings.color.food                     = default_settings.color.food;
    settings.color.snake                    = default_settings.color.snake;
    settings.color.super_food               = default_settings.color.super_food;

    settings.food_mark                      = default_settings.food_mark;
    settings.num_food                       = default_settings.num_food;
    settings.seed                           = default_settings.seed;

    // the characters that make up the body
    settings.snake_body_mark.ur             = default_settings.snake_body_mark.ur;
    settings.snake_body_mark.lr             = default_settings.snake_body_mark.lr;
    settings.snake_body_mark.ul             = default_settings.snake_body_mark.ul;
    settings.snake_body_mark.ll             = default_settings.snake_body_mark.ll;
    settings.snake_body_mark.up             = default_settings.snake_body_mark.up;
    settings.snake_body_mark.down           = default_settings.snake_body_mark.down;
    settings.snake_body_mark.left           = default_settings.snake_body_mark.left;
    settings.snake_body_mark.right          = default_settings.snake_body_mark.right;

    settings.snake_head_mark.up             = default_settings.snake_head_mark.up;
    settings.snake_head_mark.down           = default_settings.snake_head_mark.down;
    settings.snake_head_mark.left           = default_settings.snake_head_mark.left;
    settings.snake_head_mark.right          = default_settings.snake_head_mark.right;

    // key commands
    settings.commands.up                    = default_settings.commands.up;
    settings.commands.down                  = default_settings.commands.down;
    settings.commands.left                  = default_settings.commands.left;
    settings.commands.right                 = default_settings.commands.right;
    settings.commands.toggle_menu           = default_settings.commands.toggle_menu;

    settings.board_size.x                   = default_settings.board_size.x;
    settings.board_size.y                   = default_settings.board_size.y;

    snake    = new Snake();
    controls = new Controls( snake );
    menu     = new Menu();
    

    return ;
}		/* -----  end of method Game::initialize  ----- */

enum game_mode {
    state_play,
    state_menu
};




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
            results = menu->get_input( input_char );
            break;
        default:
            break;
        }

        switch( results ) {
            case TOGGLE_MAIN_MENU:
                menu->toggle();
                if( current_mode == state_play ) {
                    current_mode = state_menu;
                } else {
                    current_mode = state_play;
                }
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



/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  main
 *  Description:  
 * =====================================================================================
 */
int main ( int argc, char *argv[] ) {

    Game *game = new Game();

    return EXIT_SUCCESS;
}				/* ----------  end of function main  ---------- */

