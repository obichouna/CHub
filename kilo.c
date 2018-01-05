/*** includes ***/

#define _DEFAULT_SOURCE
#define _BSD_SOURCE
#define _GNU_SOURCE

#include <unistd.h>
#include <termios.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <string.h>


// to start new line must type \r\n

typedef struct erow { //This struct represents one of the rows in the editor
  int size;
  char *chars;
} erow;

/*** data ***/
struct editorConfig{
  int numrows;
  erow *row; //An array of structs of rows
  int coloff;
  int rowoff;
  int cx, cy;
  int screenrows;
  int screencols;
  struct termios orig_termios;
};
struct editorConfig E;

/*** defines ***/
#define KILO_VERSION "0.0.1"

#define CTRL_KEY(k) ((k) & 0x1f) //combines control key

enum editorKey {
  ARROW_LEFT = 1000,
  ARROW_RIGHT,
  ARROW_UP,
  ARROW_DOWN,
  DEL_KEY,
  HOME_KEY,
  END_KEY,
  PAGE_UP,
  PAGE_DOWN
};

/*** terminal ***/
void die(const char *s){
  write(STDOUT_FILENO, "\x1b[2J", 4);
  write(STDOUT_FILENO, "\x1b[H",3);
  //clears screen and repositions cursor
  perror(s);
  exit(1);
}
void disableRawMode(){
  tcsetattr(STDIN_FILENO, TCSAFLUSH, &E.orig_termios);
  die("tcsettattr");
}
void enableRawMode(){
  if(tcgetattr(STDIN_FILENO,&E.orig_termios)== -1) die("tcgetattr");
  //tcgetattr() reads terminal attributes into termios
  atexit(disableRawMode);
  //atexit() calls automatically when programs exits
  struct termios raw = E.orig_termios;
  raw.c_iflag &=~(BRKINT | ICRNL|INPCK |ISTRIP |IXON);
  raw.c_iflag &=~(ICRNL |IXON);
  //IXON turns off ctrl-s and ctrlq
  //ICRNL fixes ctrl-m to read 13 instead of 10
  raw.c_oflag &= ~(OPOST);
  raw.c_cflag |= (CS8);
  raw.c_lflag &= ~(ECHO |ICANON|IEXTEN|ISIG);
  //flipping bits
  //ECHO causes each key you type to be printed to the terminal
  //ISIG turns off ctrl-c and ctrl-z signals
  //ICANON turns off canonical mode allowing us to read byte by byte
  //IEXTEN turns off ctrl-v
  raw.c_cc[VMIN] = 0;
  raw.c_cc[VTIME] =1;
  if( tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw)== -1) die("tcsetattr");
  //tcsetattr allows you to apply attributes after modifying them
  //TCSAFLUSH specifies when to apply the change
  //waits for output to be written then discards input that hasnt been read
  //c_lflag is for "local flags"
}
int editorReadKey() {
  int nread;
  char c;
  while ((nread = read(STDIN_FILENO, &c, 1)) != 1) {
    if (nread == -1 && errno != EAGAIN) die("read");
    //EAGIN is output from read we dont want to exit from it

  }
  if (c == '\x1b') {
    char seq[3];
    if (read(STDIN_FILENO, &seq[0], 1) != 1) return '\x1b';
    if (read(STDIN_FILENO, &seq[1], 1) != 1) return '\x1b';
    if (seq[0] == '[') {
      if (seq[1] >= '0' && seq[1] <= '9') {
        if (read(STDIN_FILENO, &seq[2], 1) != 1) return '\x1b';
        if (seq[2] == '~') {
          switch (seq[1]) {
	  case '1': return HOME_KEY;
	  case '3':return DEL_KEY;
	  case '4': return END_KEY;
	  case '5': return PAGE_UP;
	  case '6': return PAGE_DOWN;
	  case '7': return HOME_KEY;
	  case '8': return END_KEY;
          }
        }
      } else {
	switch (seq[1]) {
        case 'A': return ARROW_UP;
        case 'B': return ARROW_DOWN;
        case 'C': return ARROW_RIGHT;
        case 'D': return ARROW_LEFT;
	case 'H': return HOME_KEY;
        case 'F': return END_KEY;
	  //turns the arrow keys into the wsda keys so they function in the same way
	}
      }
      } else if (seq[0] == 'O') {
      switch (seq[1]) {
        case 'H': return HOME_KEY;
        case 'F': return END_KEY;
      }
    }
    return '\x1b';
  } else {
    return c;
  }
}
int getCursorPosition(int *rows, int *cols) {
  char buf[32];
  unsigned int i = 0;

  if (write(STDOUT_FILENO, "\x1b[6n", 4) != 4) return -1;

  while (i < sizeof(buf) - 1) {
    if (read(STDIN_FILENO, &buf[i], 1) != 1) break;
    if (buf[i] == 'R') break;
    i++;
  }
  if (buf[0] != '\x1b' || buf[1] != '[') return -1;
  if (sscanf(&buf[2], "%d;%d", rows, cols) != 2) return -1;
  return 0;
}

int getWindowSize(int *rows, int *cols) {
  struct winsize ws;
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == -1 || ws.ws_col == 0) {
    //ioctl() will place the number of columns wide and the number of rows high
    //the terminal is into the given winsize struct
  if (write(STDOUT_FILENO, "\x1b[999C\x1b[999B", 12) != 12) return -1;
  return getCursorPosition(rows, cols);
  } else {
    *cols = ws.ws_col;
    *rows = ws.ws_row;
    return 0;
  }
}
/*** append buffer ***/
struct abuf {
  char *b;
  int len;
};
#define ABUF_INIT {NULL, 0}
void abAppend(struct abuf *ab, const char *s, int len) {
  char *new = realloc(ab->b, ab->len + len);
  if (new == NULL) return;
  memcpy(&new[ab->len], s, len);
  ab->b = new;
  ab->len += len;
}
void abFree(struct abuf *ab) {
  free(ab->b);
}

//*** output **/
void editorDrawRows(struct abuf *ab) {
  int y;
  for (y = 0; y < E.screenrows; y++) {
    int filerow = y + E.rowoff;
    if (filerow >= E.numrows){
      if (E.numrows == 0 && y == E.screenrows / 3) {
        char welcome[80];
        int welcomelen = snprintf(welcome, sizeof(welcome),
        "CHub editor -- version %s", KILO_VERSION);
        if (welcomelen > E.screencols) welcomelen = E.screencols;
        //use the welcome buffer and snprintf to interpolate our KILO_VERsSION string into th
        //e welcome message and truncate length of string in case its too tiny to fit into welcome message

        int padding = (E.screencols - welcomelen) / 2;
        if (padding) {
          abAppend(ab, "~", 1);
          padding--;
        }
        while (padding--) abAppend(ab, " ", 1);
        //centers welcome message
        abAppend(ab, welcome, welcomelen);
      } else {
        abAppend(ab, "~", 1);
      }
    } else {
      int len = E.row[filerow].size - E.coloff;
      if (len < 0) len = 0;
      if (len > E.screencols) len = E.screencols;
      abAppend(ab, &E.row[filerow].chars[E.coloff], len);
    }
    abAppend(ab, "\x1b[K", 3);
    //k erases parts of current line

    if (y < E.screenrows - 1) {
      abAppend(ab, "\r\n", 2);
    }
  }
  //draws ~ at start of every row
}


void editorAppendRow(char *s, size_t len) {
  E.row = realloc(E.row, sizeof(erow) * (E.numrows + 1));
  int at = E.numrows;
  E.row[at].size = len;
  E.row[at].chars = malloc(len + 1);
  memcpy(E.row[at].chars, s, len);
  E.row[at].chars[len] = '\0';
  E.numrows++;
}

//This function opens the editor
//If there is no filename, the editor will open a blank file
void editorOpen(char *filename) {
  FILE *fp = fopen(filename, "r");
  if (!fp) die("fopen");
  char *line = NULL;
  size_t linecap = 0;
  ssize_t linelen;
  while ((linelen = getline(&line, &linecap, fp)) != -1) {
    while (linelen > 0 && (line[linelen - 1] == '\n' ||
                           line[linelen - 1] == '\r'))
      linelen--;
    editorAppendRow(line, linelen);
  }
  free(line);
  fclose(fp);
}

//This function will move around the cursor
void editorScroll() {
  if (E.cy < E.rowoff) {
    E.rowoff = E.cy;
  }
  if (E.cy >= E.rowoff + E.screenrows) {
    E.rowoff = E.cy - E.screenrows + 1;
  }
  if (E.cx < E.coloff) {
    E.coloff = E.cx;
  }
  if (E.cx >= E.coloff + E.screencols) {
    E.coloff = E.cx - E.screencols + 1;
  }
}

void editorRefreshScreen(){
  editorScroll();
  struct abuf ab = ABUF_INIT;
  abAppend(&ab, "\x1b[?25l", 6);
  //hids cursor
  abAppend(&ab, "\x1b[H", 3);
  //escape sequence to the terminal
  //\x1b is escape character
  // H repositions cursors
  editorDrawRows(&ab);

  char buf[32];
  snprintf(buf, sizeof(buf), "\x1b[%d;%dH", (E.cy - E.rowoff) + 1, E.cx + 1);
  abAppend(&ab, buf, strlen(buf)); abAppend(&ab,"\x1b[H",3);
  //moves the cursor to the position stored in E.cx and E.cy
  abAppend(&ab, "\x1b[?25h", 6);

  write(STDOUT_FILENO, ab.b, ab.len);
  abFree(&ab);
}

/*** input ***/
void editorMoveCursor(int key) {
 switch (key) {
    case ARROW_LEFT:
      if (E.cx != 0) {
        E.cx--;
      }
      break;
    case ARROW_RIGHT:
        E.cx++;
        break;
    case ARROW_UP:
      if (E.cy != 0) {
        E.cy--;
      }
      break;
    case ARROW_DOWN:
      if (E.cy < E.numrows) {
        E.cy++;
      }
      break;
  }
  // allows you to use these keys to move the cursor left right up or down
}

void editorProcessKeypress(){
  int c = editorReadKey();
  switch(c){
  case CTRL_KEY('q'):
    write(STDOUT_FILENO, "\x1b[2J", 4);
    write(STDOUT_FILENO, "\x1b[H",3);
    //clears screen and repositions cursor
    exit(0);
    break;
    //control q quits
  case HOME_KEY:
    E.cx = 0;
    break;
  case END_KEY:
    E.cx = E.screencols - 1;
    break;
  case PAGE_UP:
  case PAGE_DOWN:
    {
      int times = E.screenrows;
      while (times--)
	editorMoveCursor(c == PAGE_UP ? ARROW_UP : ARROW_DOWN);
      }
      break;
      //helps with scrolling
    case ARROW_UP:
    case ARROW_DOWN:
    case ARROW_LEFT:
    case ARROW_RIGHT:
      editorMoveCursor(c);
      break;
  }
}

/*** init ***/
void initEditor(){
  E.numrows = 0;
  E.rowoff = 0;
  E.coloff = 0;
  E.cx = 0;
  E.cy = 0;
  E.row = NULL;

  if(getWindowSize(&E.screenrows, &E.screencols)== -1) die("getWindowSize");
}

int main(int argc, char *argv[]) {
  enableRawMode();
  initEditor();
  if (argc >= 2) {
   editorOpen(argv[1]);
 }
  while(1){
    editorRefreshScreen();
    editorProcessKeypress();
  }
  //press q to quit

  return 0;
}
