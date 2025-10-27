#include <sys/ioctl.h>
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

    public:
        std::vector<std::string> command = {"\033[A", "\033[B", "\033[C", "\033[D", "\033", "\033[20;4H", "\033[K", "\033[s", "\033[u"};
        std::vector<char> characters = {'j', 'k', 'l', 'h', 'D', 'g'};
        /*
         * 0 = k | 1 = j | 2 = h | 3 = l | 4 = esc
         * 5 = gg | 6 = D | 7 = m | 8 = '
         */

        VimBind(){
            struct winsize win;
            ioctl(STDOUT_FILENO, TIOCGWINSZ, &win);

            termheight = win.ws_row;
            termwidth= win.ws_col;
        }

        int termWidth(){ return termwidth; }
        int termHeight(){ return termheight; }

        void enableRawMode(){
            tcgetattr(STDIN_FILENO, &root);

            raw = root;
            raw.c_lflag &= ~(ICANON); // disable flags
            raw.c_cc[VMIN] = 0; // minimum char
            raw.c_cc[VTIME] = 1; // time to act
            tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
        }
        void disableRawMode(){
            tcsetattr(STDIN_FILENO, TCSAFLUSH, &root);
        }
        void rightCommand(std::string text){
            std::cout << command[7];
            std::cout << "\033[" << termWidth() << ";" << (termwidth / 2) - 4 << "H";
            std::cout << command[6];
            std::cout << text;
            std::cout << command[8];
            std::cout << std::flush;
        }
        void rightCommandClear(){
            std::cout << command[7];
            std::cout << "\033[" << termWidth() << ";4H";
            std::cout << command[6];
            std::cout << command[8];
            std::cout << std::flush;
        }
};
