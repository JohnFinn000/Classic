/*
 * =====================================================================================
 *
 *       Filename:  snake.h
 *
 *    Description:  simple snake game
 *
 *        Version:  1.0
 *        Created:  11/01/2013 07:07:36 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  John Finn (), johnvincentfinn@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */

#include <curses.h>
#include <cursesw.h>
#include <cursesp.h>
#include <cursesm.h>
#include <signal.h>
#include <panel.h>
#include <menu.h>
#include <form.h>

#define DEFAULT 0
#define TOGGLE_MAIN_MENU 1
#define GAME_OVER 2
#define TOGGLE_OPTIONS_MENU 3
#define EXIT 4

enum direction {
    dir_up,
    dir_down,
    dir_left,
    dir_right,
    dir_food,
    dir_super_food
};

struct Settings {
    int seed;

    struct {
        int color;
        int mark;
        int points;
        int ratio;
    } food[5];

    float food_rarity;

    struct {
        int color;
        struct {
            int ur;
            int lr;
            int ul;
            int ll;
            int up;
            int down;
            int left;
            int right;
        } body_mark;

        struct {
            int up;
            int down;
            int left;
            int right;
        } head_mark;
    } snake;

    struct {
        int up;
        int down;
        int left;
        int right;
        int toggle_menu;
    } commands;

    struct {
        int x;
        int y;
    } board_size;
};

Settings settings;


/*
 * =====================================================================================
 *        Class:  Snake
 *  Description:  The implementation for the game of snake
 * =====================================================================================
 */
class Snake : public NCursesPanel {
    public:
        /* ====================  LIFECYCLE     ======================================= */
        Snake();                             /* constructor */
        Snake( int nlines, int ncols, int begin_y, int begin_x );
        ~Snake() { }

        /* ====================  ACCESSORS     ======================================= */

        /* ====================  MUTATORS      ======================================= */
        void change_direction( direction dir );
        int tick();

        /* ====================  OPERATORS     ======================================= */

    protected:
        /* ====================  DATA MEMBERS  ======================================= */

    private:
        /* ====================  DATA MEMBERS  ======================================= */
        void drop_food();
        void drop_food( int x, int y );
        void set_size( int x, int y );

        direction dir;
        direction last_dir;
        int head_x, head_y;
        int neck_x, neck_y;
        int tail_x, tail_y;

        int max_x, max_y;

        int length;
        int max_length;

        int **board;

        int score;
        // timer

        //Settings *settings; 

}; /* -----  end of class Snake  ----- */


/*
 * =====================================================================================
 *        Class:  Controls
 *  Description:  handles control from the user
 * =====================================================================================
 */
class Controls
{
    public:
        /* ====================  LIFECYCLE     ======================================= */
        Controls( Snake *snake );                             /* constructor */

        /* ====================  ACCESSORS     ======================================= */
        int get_input( int input_char );

    private:
        /* ====================  DATA MEMBERS  ======================================= */
        Snake *snake;

        void up();
        void down();
        void right();
        void left();

}; /* -----  end of class Controls  ----- */


/*
 * =====================================================================================
 *        Class:  Main_Menu
 *  Description:  This controls the user input to the games main menu
 * =====================================================================================
 */
class Main_menu : public NCursesMenu {
    public:
        /* ====================  LIFECYCLE     ======================================= */
        Main_menu( int nlines, int ncols, int begin_y, int begin_x );   /* constructor */
        ~Main_menu();

        /* ====================  MUTATORS      ======================================= */
        //int get_input( int input_char );
        void toggle();

    private:
        /* ====================  DATA MEMBERS  ======================================= */
        //NCursesMenuItem menu_item_array[5];
        NCursesPanel *panel;
        NCursesMenuItem **menu_item_array;

        static bool resume_callback( NCursesMenuItem &item );
        static bool save_callback( NCursesMenuItem &item );
        static bool quit_callback( NCursesMenuItem &item );
        static bool options_callback( NCursesMenuItem &item );

}; /* -----  end of class Menu  ----- */

char *options_menu_options[][2] = {
    { "Speed", NULL },
    { "Food Rate", NULL },
    { "Back", NULL },
    { NULL, NULL }
};

char *game_over_menu_options[][2] = {
    {"Restart", "Play again" },
    {"Quit", "Leave the game" },
    {NULL, NULL}
};

/*
 * =====================================================================================
 *        Class:  Game
 *  Description:  
 * =====================================================================================
 */
class Game
{
    public:
        /* ====================  LIFECYCLE     ======================================= */
        Game();                             /* constructor */
        ~Game();

        void load_configuration( const std::string &filename );


        /* ====================  MUTATORS      ======================================= */
        static void finish( int sig );
        static void resize( int sig );

    private:
        /* ====================  DATA MEMBERS  ======================================= */
        void run();
        void initialize();

        Snake *snake;
        Controls *controls;
        //NCursesMenu *main_menu;
        Main_menu *main_menu;

        enum game_mode {
            state_play,
            state_menu
        };
}; /* -----  end of class Game  ----- */

