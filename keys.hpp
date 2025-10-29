void keyBind(VimBind vim, char word, bool *writeMode, bool *run) {
    switch(word){
        case 'k': {
                      std::cout << vim.command[0] << std::flush;
                      break;
                  }
        case 'j': {
                      std::cout << vim.command[1] << std::flush;
                      break;
                  }
        case 'l': {
                      std::cout << vim.command[2] << std::flush;
                      break;
                  }
        case 'h': {
                      std::cout << vim.command[3] << std::flush;
                      break;
                  }
        case 'D': {
                      std::cout << vim.command[6] << std::flush;
                      break;
                  }
        case 'd': {
                      std::cout << vim.command[9] << std::flush;
                      break;
                  }
        case 'i': *writeMode = 1;
                  vim.setThinCursor();
                  break;
        case 'q': std::cout << '\n';
                  *run = 0;
                  break;
    }
}
