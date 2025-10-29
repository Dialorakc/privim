#include <iostream>
#include <string>
#include "noteVim.hpp"
#include "keys.hpp"

using std::string;

class Funk {
    public:

        int endRule(int mult, string border[ ], bool end){
            int con = 6;
            end == 1 ? std::cout << "╭" : std::cout << "╰";
            for(int i = 0; i < mult - 2; i++){
                std::cout << border[1];
            }
            end == 1 ? std::cout << "╮" : std::cout << "╯";

            return 0;
        }
        int sideRule(int mult, string border[ ], int side){
            for(int i = 0; i < mult - side; i++){
                std::cout << " ";
            }
            std::cout << border[0];

            return 0;
        }
};

int main(){
    int miliSec = 1000;
    string border[] = {"│", "─"};
    char word;
    bool end = 1;
    Funk funky;
    VimBind vim;
    int mult = vim.termWidth();

    /* opens new buffer */
    setupterm(0, STDOUT_FILENO, 0);

    putp(tigetstr("smcup"));
    std::cout << std::flush;

    /* Draw background */
    for(int r = 0; r < vim.termHeight(); r++){
        std::cout << "\033[" << r + 1 << ";1H";
        std::cout << "\033[48;2;206;89;40m";
    }

    /* Draw content on top */
    std::cout << "\033[1;1H";
    std::cout << "\033[38;2;216;213;8m";
    putp(tigetstr("clear"));

    funky.endRule(mult, border, end);
    std::cout << '\n' << border[0] << '\n';
    std::cout << border[0] << "  This is a simple note taking program with nvim integration\n";
    funky.endRule(mult, border, 0);
    std::cout << '\n';

    funky.endRule(mult, border, 1);
    std::cout << '\n' << border[0] << '\n';
    vim.termHeight();
    vim.rightCommand("@Dialore.", 2, 4);

    /* Text handler */
    vim.enableRawMode();
    vim.setThinCursor();

    bool run = 1, writeMode = 1;
    struct timeval timeout;
    std::cout << border[0] << "  " << std::flush;

    while(run){

        fd_set readch;
        FD_ZERO(&readch);
        FD_SET(STDIN_FILENO, &readch);

        timeout.tv_sec = 0;
        timeout.tv_usec = 10 * miliSec;

        int complete = select(STDIN_FILENO + 1, &readch, NULL, NULL, &timeout);

        if (vim.termPosUpdater()){
            putp(tigetstr("clear"));
            mult = vim.termWidth();

            funky.endRule(mult, border, 1);
            std::cout << '\n' << border[0] << '\n';
            std::cout << border[0] << "  This is a simple note taking program with nvim integration\n";
            funky.endRule(mult, border, 0);
            std::cout << '\n';
            funky.endRule(mult, border, 1);
            std::cout << '\n' << border[0] << '\n';
            vim.rightCommand("@Dialore.", 2, 4);
            std::cout << border[0] << "  " << std::flush;
        }

        if (complete > 0){
            int byte = read(STDIN_FILENO, &word, 1);
            std::cout << std::flush;
            // perror("read");

            if (writeMode){
                if (word == '\x7f') {
                    std::cout << "\b \b" << std::flush;  // Move back, write space, move back again
                    continue;
                }
                if (word == '\033'){
                    writeMode = 0;
                    vim.setBlockCursor();
                    continue;
                }
                if (word >= 32 && word <= 126){
                    std::cout << word << std::flush;
                }
                if (word == '\n' || word == '\r'){
                    std::cout << '\n' << border[0] << "  " << std::flush;
                    std::cout << '\n' << border[0] << "  " << std::flush;
                }
            }
            else {
                keyBind(vim, word, &writeMode, &run);
            }
        }
    };

    vim.disableRawMode();

    funky.endRule(mult, border, 0);
    std::cout << '\n';

    usleep(1000 * miliSec);

    putp(tigetstr("clear"));
    putp(tigetstr("rmcup"));
    std::cout << std::flush;

    return 0;
}
