
#define ROWS 9
#define COLS 9

#define UNK   -1    // Empty sudoku cell ('?')
#define UNK_C '?'

/*
 * Color definitions for the terminal. Credits:
 * https://gitlab.com/greggink/youtube_episode_terminal_control_2/-/blob/main/tc.h
 */

// Bold colors
#define TC_B_NRM "\x1B[1m"
#define TC_B_GRY "\x1B[1;30m"
#define TC_B_RED "\x1B[1;31m"
#define TC_B_GRN "\x1B[1;32m"
#define TC_B_YEL "\x1B[1;33m"
#define TC_B_BLU "\x1B[1;34m"
#define TC_B_MAG "\x1B[1;35m"
#define TC_B_CYN "\x1B[1;36m"
#define TC_B_WHT "\x1B[1;37m"

// Normal colors
#define TC_NRM "\x1B[0m"
#define TC_BLK "\x1B[0;31m"
#define TC_RED "\x1B[0;31m"
#define TC_GRN "\x1B[0;32m"
#define TC_YEL "\x1B[0;33m"
#define TC_BLU "\x1B[0;34m"
#define TC_MAG "\x1B[0;35m"
#define TC_CYN "\x1B[0;36m"
#define TC_WHT "\x1B[0;37m"

#define BOLD  TC_B_BLU
#define NORM  TC_NRM
#define SOFT  TC_GRN
#define FCOL  TC_B_RED
#define NFCOL TC_B_GRY
