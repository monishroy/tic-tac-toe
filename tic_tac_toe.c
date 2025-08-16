// tic_tac_toe.c
// Build: gcc tic_tac_toe.c -o ttt && ./ttt
#include <stdio.h>
#include <stdlib.h>

#ifdef _WIN32
#include <windows.h>
void enableVT() {
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hOut != INVALID_HANDLE_VALUE) {
        DWORD dwMode = 0;
        if (GetConsoleMode(hOut, &dwMode)) {
            dwMode |= 0x0004; // ENABLE_VIRTUAL_TERMINAL_PROCESSING
            SetConsoleMode(hOut, dwMode);
        }
    }
}
#else
void enableVT() {}
#endif

static void cls(void) {
    // ANSI clear screen + move cursor home
    printf("\033[2J\033[H");
}

static void draw_header(void) {
    printf("^^^^^^^^^^^^^^^^TIC TAC TOE^^^^^^^^^^^^^^^^\n");
}

static void draw_board(const char b[9]) {
    // Looks similar to your screenshot
    puts("-------------------------");
    printf("| %c | %c | %c |\n", b[0], b[1], b[2]);
    puts("-------------------------");
    printf("| %c | %c | %c |\n", b[3], b[4], b[5]);
    puts("-------------------------");
    printf("| %c | %c | %c |\n", b[6], b[7], b[8]);
    puts("-------------------------");
}

static int check_winner(const char b[9], char *who) {
    const int winLines[8][3] = {
        {0,1,2},{3,4,5},{6,7,8}, // rows
        {0,3,6},{1,4,7},{2,5,8}, // cols
        {0,4,8},{2,4,6}          // diagonals
    };
    for (int i = 0; i < 8; ++i) {
        int a = winLines[i][0], c = winLines[i][1], d = winLines[i][2];
        if (b[a] == b[c] && b[c] == b[d]) {
            *who = b[a];
            return 1; // someone won
        }
    }
    return 0;
}

static int board_full(const char b[9]) {
    for (int i = 0; i < 9; ++i)
        if (b[i] >= '1' && b[i] <= '9') return 0;
    return 1;
}

int main(void) {
    enableVT();

    for (;;) { // replay loop
        char board[9];
        for (int i = 0; i < 9; ++i) board[i] = '1' + i;
        char player = 'X';
        char winner = 0;

        while (1) {
            cls();
            draw_header();
            draw_board(board);
            printf("\nYour step (%c): ", player);

            int choice;
            if (scanf("%d", &choice) != 1) {
                // flush invalid input
                int ch;
                while ((ch = getchar()) != '\n' && ch != EOF) {}
                continue;
            }

            if (choice < 1 || choice > 9) continue;
            int idx = choice - 1;
            if (board[idx] == 'X' || board[idx] == 'O') continue;

            board[idx] = player;

            if (check_winner(board, &winner)) {
                cls(); draw_header(); draw_board(board);
                printf("\nWinner: %c 🎉\n", winner);
                break;
            }
            if (board_full(board)) {
                cls(); draw_header(); draw_board(board);
                printf("\nIt's a draw.\n");
                break;
            }

            player = (player == 'X') ? 'O' : 'X';
        }

        printf("\nPlay again? (y/n): ");
        int c = 0, ch;
        // consume trailing newline
        while ((ch = getchar()) != '\n' && ch != EOF) {}
        c = getchar();
        if (c != 'y' && c != 'Y') {
            printf("Goodbye!\n");
            break;
        }
    }
    return 0;
}
