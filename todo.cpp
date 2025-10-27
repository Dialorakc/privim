#include <iostream>
#include <string>
#include "noteVim.hpp"

using std::string;

class Funk {
    public:

        int endRule(int mult, string border[ ], bool end){
            int con = 6;
            end == 1 ? std::cout << "╭" : std::cout << "╰";
            for(int i = 0; i < mult - 1; i++){
                std::cout << border[1];
            }

            return 0;
        }
        void debug(string &text){
            std::cout << text;
        }
};

int main(){
    int miliSec = 1000;
    string border[] = {"│", "─"}, text;
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
    end = 0;
    funky.endRule(mult, border, end);
    std::cout << '\n';

    end = 1;
    funky.endRule(mult, border, end);
    std::cout << '\n' << border[0] << '\n';
    vim.termHeight();
    vim.rightCommand("@Dialore.");

    /* Text handler */
    vim.enableRawMode();

    char cmd;
    bool run = 1;

    while(run){
        int byte = read(STDIN_FILENO, &word, 1);
        std::cout << std::flush;
        // perror("read");

        if (word == '\033'){
            if (read(STDIN_FILENO, &cmd, 1) == 1){
                switch(cmd){
                    case 'k': {
                                  std::cout << vim.command[0] << std::flush;
                                  break;
                              }
                    case 'j': {
                                  std::cout << vim.command[1] << std::flush;
                                  break;
                              }
                    case 'h': {
                                  std::cout << vim.command[2] << std::flush;
                                  break;
                              }
                    case 'l': {
                                  std::cout << vim.command[3] << std::flush;
                                  break;
                              }
                    case 'q': run = 0;
                              break;
                    default: std::cout << "[Error on Esc handler]" << std::flush;
                             break;
                }
            }
        }

        usleep(10000);
    };

    vim.disableRawMode();

    end = 0;
    funky.endRule(mult, border, end);
    std::cout << '\n';

    usleep(1000 * miliSec);

    putp(tigetstr("clear"));
    putp(tigetstr("rmcup"));
    std::cout << std::flush;

    return 0;
}
