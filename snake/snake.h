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
    struct {
        int snake;
        int food;
        int super_food;
    } color;

    int food_mark;
    int num_food;
    int seed;

    struct {
        int ur;
        int lr;
        int ul;
        int ll;
        int up;
        int down;
        int left;
        int right;
    } snake_body_mark;

    struct {
        int up;
        int down;
        int left;
        int right;
    } snake_head_mark;

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


/*
class Config {
    static char names[] = {
        "super_food_color".

        "seed".

        "food.color".
        "food.num".
        "food.mark".

        "snake.body_mark.ur".
        "snake.body_mark.lr".
        "snake.body_mark.ul".
        "snake.body_mark.ll".
        "snake.body_mark.up".
        "snake.body_mark.down".
        "snake.body_mark.left".
        "snake.body_mark.right".
        "snake.head_mark.up".
        "snake.head_mark.down".
        "snake.head_mark.left".
        "snake.head_mark.right".

        "snake.color".

        "commands.up";
        "commands.down";
        "commands.left";
        "commands.right";
        "commands.toggle_menu";

        "board.size.x";
        "board.size.y";
    }
    union Settings {
        int index[];
        struct {
            int super_food_color;

            int seed;

            struct {
                int color;
                int num;
                int mark;
                int score;
                int nutrition;
            } food;

            struct {
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

                int color
            } snake;

            struct {
                int up;
                int down;
                int left;
                int right;
                int toggle_menu;
            } commands;

            struct {
                struct {
                    int x;
                    int y;
                } size;
            } board;
        }
    }
};
*/

Settings settings;

char *main_menu_options[][2] = {
    {"Resume", "Continue playing the game" },
    {"Options", "Edit the game options" },
    {"Save", "Save this game for future play" },
    {"Quit", "Leave the game" },
    {NULL, NULL}
};

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

        int score;
        // timer

        //Settings *settings; 
    
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

