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
#include <signal.h>
#include <panel.h>
#include <menu.h>
#include <form.h>

#define DEFAULT 0
#define OPEN_MENU 1
#define GAME_OVER 2

#define DEFAULT_SNAKE_COLOR 1
#define DEFAULT_FOOD_COLOR 2
#define DEFAULT_SUPER_FOOD_COLOR 2

enum direction {
    dir_up,
    dir_down,
    dir_left,
    dir_right,
    food,
    super_food
};

struct Settings {
    struct {
        int snake_color;
        int food_color;
        int super_food_color;
    } color;

    int food_mark;

    struct {
        int up;
        int down;
        int right;
        int left;
        int ur;
        int lr;
        int ul;
        int ll;
    } snake_body_mark;

    struct {
        int up;
        int down;
        int right;
        int left;
    } snake_head_mark;

    struct {
        int down;
        int up;
        int right;
        int left;
        int toggle_menu;
    } commands;
}

/*
 * =====================================================================================
 *        Class:  Snake
 *  Description:  The implementation for the game of snake
 * =====================================================================================
 */
class Snake
{
    public:
        /* ====================  LIFECYCLE     ======================================= */
        Snake();                             /* constructor */

        /* ====================  ACCESSORS     ======================================= */

        /* ====================  MUTATORS      ======================================= */
        void change_direction( direction dir );
        void drop_food();
        void drop_food( int x, int y );
        void set_size( int x, int y );
        int tick();

        /* ====================  OPERATORS     ======================================= */

    protected:
        /* ====================  DATA MEMBERS  ======================================= */

    private:
        /* ====================  DATA MEMBERS  ======================================= */

        direction dir;
        direction last_dir;
        int head_x, head_y;
        int neck_x, neck_y;
        int tail_x, tail_y;

        int max_x, max_y;

        int length;
        int max_length;

        int **board;

        Settings *settings; 
    
        int window_x, window_y;
        int window_width, window_height;
        WINDOW *window;
        PANEL  *panel;

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

        /* ====================  MUTATORS      ======================================= */

        /* ====================  OPERATORS     ======================================= */

    protected:
        /* ====================  DATA MEMBERS  ======================================= */

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
 *        Class:  Menu
 *  Description:  This controls the user input to the games main menu
 * =====================================================================================
 */
class Menu {
    public:
        /* ====================  LIFECYCLE     ======================================= */
        Menu();                             /* constructor */
        ~Menu();

        /* ====================  ACCESSORS     ======================================= */

        /* ====================  MUTATORS      ======================================= */
        int get_input( int input_char );
        void show();
        void hide();
        void toggle();

        /* ====================  OPERATORS     ======================================= */

    protected:
        /* ====================  DATA MEMBERS  ======================================= */

    private:
        /* ====================  DATA MEMBERS  ======================================= */
        WINDOW *window;
        PANEL *panel;
        MENU *menu;
}; /* -----  end of class Menu  ----- */


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
        void initialize();

        static void finish( int sig );
        static void resize( int sig );

        /* ====================  ACCESSORS     ======================================= */

        /* ====================  MUTATORS      ======================================= */

        /* ====================  OPERATORS     ======================================= */

    protected:
        /* ====================  DATA MEMBERS  ======================================= */

    private:
        /* ====================  DATA MEMBERS  ======================================= */
        void run();
        Snake *snake;
        Controls *controls;
        Menu *menu;

}; /* -----  end of class Game  ----- */

