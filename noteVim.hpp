#include <sys/ioctl.h>
#include <csignal>
#include <cstdlib>
#include <term.h>
#include <unistd.h>
#include <termios.h>
#include <vector>

class VimBind {
    private:
        int termwidth;
        int termheight;
        struct termios root, raw;

        static volatile sig_atomic_t winResize;
        static void resizeHandler(int sig){
            winResize = 1;
        }

    public:
        std::string command[10] = {"\033[A", "\033[B", "\033[C", "\033[D", "\033", "\033[20;4H", "\033[K", "\033[s", "\033[u", "\033[2K"};
        /*
         * 0 = k | 1 = j | 2 = h | 3 = l | 4 = esc
         * 5 = ]] | 6 = D | 7 = m | 8 = ' | 9 = dd
         */

        VimBind(){
            struct winsize win;
            ioctl(STDOUT_FILENO, TIOCGWINSZ, &win);

            termheight = win.ws_row;
            termwidth= win.ws_col;
            signal(SIGWINCH, resizeHandler);
        }

        int termWidth(){ return termwidth; }
        int termHeight(){ return termheight; }
        void setThinCursor(){ std::cout << "\033[6 q" << std::flush; }
        void setBlockCursor(){ std::cout << "\033[2 q" << std::flush; }

        bool termPosUpdater(){
            if (winResize){
                struct winsize win;
                ioctl(STDOUT_FILENO, TIOCGWINSZ, &win);

                termheight = win.ws_row;
                termwidth= win.ws_col;
                winResize = 0;

                return 1;
            }
            return 0;
        }
        void enableRawMode(){
            tcgetattr(STDIN_FILENO, &root);

            raw = root;
            raw.c_lflag &= ~(ECHO | ICANON); // disable flags
            raw.c_cc[VMIN] = 1; // minimum char
            raw.c_cc[VTIME] = 0; // time to act
            tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
        }
        void disableRawMode(){
            tcsetattr(STDIN_FILENO, TCSAFLUSH, &root);
        }
        void rightCommand(std::string text, int firNume, int secNume){
            std::cout << command[7];
            std::cout << "\033[" << termheight << ";" << (termwidth / firNume) - secNume << "H";
            std::cout << command[6];
            std::cout << text;
            std::cout << command[8];
            std::cout << std::flush;
        }
        void rightCommandClear(){
            std::cout << command[7];
            std::cout << "\033[" << termheight << ";4H";
            std::cout << command[6];
            std::cout << command[8];
            std::cout << std::flush;
        }
};

volatile sig_atomic_t VimBind::winResize = 0;
