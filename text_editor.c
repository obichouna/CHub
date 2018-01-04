#include <unistd.h>
#include <termios.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

struct termios orig_termios;

void disableRawMode(){
  tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
}
void enableRawMode(){
  tcgetattr(STDIN_FILENO,&orig_termios);
  //tcgetattr() reads terminal attributes into termios
  atexit(disableRawMode);
  //atexit() calls automatically when programs exits 
  struct termios raw = orig_termios;
  raw.c_lflag &= ~(ECHO |ICANON);
  //flipping bits
  //ECHO causes each key you type to be printed to the terminal
  tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
  //tcsetattr allows you to apply attributes after modifying them
  //TCSAFLUSH specifies when to apply the change
  //waits for output to be written then discards input that hasnt been read
  //c_lflag is for "local flags" 
}
int main(){
  enableRawMode();
  char c;
  while(read(STDIN_FILENO, &c,1) ==1 && c!='q'){
    if(iscntrl(c)){
      //iscntrl checks whether character is a control character 
      printf("%d\n", c);
    }else {
      printf("%d('%c')\n", c,c);
    }
  }
  //press q to quit
  
  return 0;
}
