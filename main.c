#include <malloc.h>
#include "console_user_interface.h"
#include "game_logic.h"
#include "console_user_interface.h"

int main() {
    init_game();
    print_board(game_board);
    play_game();
    return 0;
}