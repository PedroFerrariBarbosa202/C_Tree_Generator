#include <stdio.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <stdlib.h>
#include "random.h"
#include "config.h"

typedef enum{
  LIGHTGREEN,
  GREEN,
  BROWN,
  YELLOW,
} Color;

void get_terminal_size(int *rows, int *cols) {
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    *rows = w.ws_row;
    *cols = w.ws_col;
}

void draw_char(char c, Color color){
  switch (color) {
      case LIGHTGREEN:
        printf("\033[92m");
        break;
      case GREEN:
        printf("\033[32m");
        break;
      case BROWN:
        printf("\033[38;5;94m");
        break;
      case YELLOW:
        printf("\033[93m");
        break;
  }
  printf("%c", c);
}

void move_cursor(int row, int col){
  printf("\033[%d;%dH", row, col);
}

void draw_leafs(int row, int col){
    int curr_row = row, curr_col = col;

    for(int i = 0; i < LEAF_DENSITY; i++){
        curr_row += get_random_1_0_m1();
        curr_col += get_random_1_m1();

        int color_rand = get_random_1_m1();

        // cap the distance leaves can grow
        if(curr_row > row + LEAF_SPREAD || curr_row < row - LEAF_SPREAD)
            curr_row = row;

        if(curr_col > col + LEAF_SPREAD || curr_col < col - LEAF_SPREAD)
            curr_col = col;

        move_cursor(curr_row, curr_col);
        draw_char('&', color_rand == 1 ? LIGHTGREEN : GREEN);

        fflush(stdout);
        usleep(SLEEP_TIME);
    }
}

void draw_trunk(int row, int col, int curr, int prev){
  move_cursor(row, col);
  int color_rand = get_random_1_m1();
  int char_rand = get_random_1_m1();
  
  Color trunk_color = (color_rand  == 1 ? BROWN : YELLOW);

  if(prev == 0 || curr - prev == 0){
    draw_char('|', trunk_color);
    for(int i = 1; i < THICKNESS + 1; i++){
      move_cursor(row, col + i); 
      draw_char(char_rand == 1 ? '#' : '~', trunk_color);
    } 
    draw_char('|', trunk_color);
  }else if(curr - prev < 0){
    draw_char('\\', trunk_color);
    for(int i = 1; i < THICKNESS + 1; i++){
      move_cursor(row, col + i);    
      draw_char(char_rand == 1 ? '#' : '~', trunk_color);
    } 
    draw_char('\\', trunk_color);
  }else if(curr - prev > 0){
    draw_char('/', trunk_color);
    for(int i = 1; i < THICKNESS + 1; i++){
      move_cursor(row, col + i); 
      draw_char(char_rand == 1 ? '#' : '~', trunk_color);
    }
    draw_char('/', trunk_color);
  }
  
  usleep(SLEEP_TIME);
  // for a smoother animation
  fflush(stdout);
}


void check_for_branch(int row, int col){
  int rand_num = get_random_range(CHANCE_FOR_BRANCH);
  int direction = get_random_1_m1();
  
  // 1 in CHANCE_FOR_BRANCH to create branch
  if(rand_num == 0){
    int curr_row = row, curr_col = col, prev = 0;
    int growth_time = GROWTH_TIME_BRANCH * (int)f_get_random_range(1.);
    
    for(int i = 0; i < GROWTH_TIME_BRANCH; i++){
      int color_rand = get_random_1_m1(); 
      Color branch_color = (color_rand  == 1 ? BROWN : YELLOW);

      move_cursor(curr_row, curr_col);
      if(prev == 0 || curr_row - prev == 0){
        draw_char('_', branch_color);
        for(int i = 1; i < THICKNESS_BRANCH + 1; i++){
          move_cursor(curr_row + i, curr_col);
          draw_char('#', branch_color);
        }
        draw_char('_', branch_color);
      }else if(curr_row - prev < 0){
        draw_char('/', branch_color);
        for(int i = 1; i < THICKNESS_BRANCH + 1; i++){
         move_cursor(curr_row + i, curr_col);
         draw_char('#', branch_color);
        }
        draw_char('/', branch_color);
      }else if(curr_row - prev > 0){ 
        draw_char('\\', branch_color);
        for(int i = 1; i < THICKNESS_BRANCH + 1; i++){
         move_cursor(curr_row + i, curr_col);
         draw_char('#', branch_color);
        } 
        draw_char('\\', branch_color);
      }

      prev = curr_row;
      curr_row += get_random_1_0_m1();
      curr_col += direction;
      fflush(stdout);
      usleep(SLEEP_TIME);
    }

    // grow leafs on place growth stopped
    draw_leafs(curr_row, curr_col);
  }
}

void draw_base(int row, int col){
  move_cursor(row, col - 1);
  printf("\033[33m:_______________________________:\033[0m");

  move_cursor(row + 1, col);
  printf("\033[33m\\_____________________________/\033[0m");

  move_cursor(row + 2, col);
  printf("\033[33m \\                           /\033[0m");

  move_cursor(row + 3, col);
  printf("\033[33m  \\_________________________/\033[0m");

  move_cursor(row + 4, col);
  printf("\033[32m  (_)                     (_)\033[0m");
}

void create_tree() {
    // start random nums seed
    init_rand_seed();

    int rows, cols;
    get_terminal_size(&rows, &cols);
    int curr = cols/2, prev = 0;

    // clear screen
    printf("\033[2J");
    int y = rows - 3;

    int base_col = cols/2 - 14;
    int base_row = rows - 4;

    draw_base(base_row, base_col);
    for (int i = 0; i < MAX_Y; i++) {
        draw_trunk(y, curr, curr, prev);

        if(i > BASE_TRUNK_SIZE)
          check_for_branch(y, curr);

        prev = curr;
        curr += get_random_1_0_m1();
        y--;
    }
    draw_leafs(y, curr);
    move_cursor(0, 0);
}
