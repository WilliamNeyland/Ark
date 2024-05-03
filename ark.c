#include <unistd.h>
#include <termios.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>

struct termios orig_termios;

// Restores terminal canonical mode
void disableRawMode() {
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
}

// Disables terminal canonical mode and enables terminal raw mode
void enableRawMode() {
    tcgetattr(STDIN_FILENO, &orig_termios);
    atexit(disableRawMode); 

    struct termios raw = orig_termios;
    raw.c_lflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON); // Disables Ctrl-S & Ctrl-Q Signals and Carriage returns (Ctrl-M)
    raw.c_lflag &= ~(OPOST);
    raw.c_lflag |= (CS8);
    raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG); //Disable Ctrl-C & Ctrl-Z Signals and Canonical Mode

    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

int main() {
    enableRawMode();

    char c;
    // Reads input signals and outpus their ASCII value
    while (read(STDIN_FILENO, &c, 1) == 1 && c != 'q') {
        // Control Characters 0-31 & 127
        if(iscntrl(c)) {
            printf("%d\r\n", c);
        }
        // All other characters 32-126
        else {
            printf("%d ('%c')\r\n", c, c);
        }
    }
    return 0;
}