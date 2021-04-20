/*
 *	  Copyright 2021 vovsenqq <senyukov2001@gmail.com>
 *	  
 *	  This program is free software; you can redistribute it and/or modify it 
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>

#define width 70
#define height 20

int i, j, a, b, n, var, dir;
int ground[height][width];   //information about game field
int game, movementSpeed, currentScore, bestResult;
int x, y, zxc;  //cords
int head, tail, sweet, difficulty;
char userName[20];
int results[6];

int get_rand_range_int(const int min, const int max) {   //inteval random funcrion
    srand(time(NULL));
    return rand() % (max - min + 1) + min;
}

void beforeGameStart() {
    system("clear");
    int clown;
    printf("enter username: ");
    system("stty cooked echo");
    scanf("%s", &userName);
    system("stty cbreak -echo");
    for (i = 0; i <= 6; i++) {
        results[i] = 0;
    }
    printf("choose the difficulty level e-easy n-normal h-hard: ");
    system("clear");
    while (1) {
        difficulty = getch_noblock();
        if (difficulty == 'e' || difficulty == 'n' || difficulty == 'h') {
            break;
        }
    }
    system("clear");
}

void gameInitialization() {
    game = 0;    //game 0 - game on, game 1 - game off
    currentScore = 0;
    if (difficulty == 'e') {
        movementSpeed = 120000;
    } else if (difficulty == 'n') {
        movementSpeed = 100000;
    } else if (difficulty == 'h') {
        movementSpeed == 75000;
    }
    x =  get_rand_range_int(5, 15);
    y =  get_rand_range_int(10, 55);
    zxc = y;
    head = 5;
    tail = 1;
    dir = 'd';
    sweet = 0;
    for (i = 0; i < height; i++) {
        for (j = 0; j < width; j++) {
            ground[i][j] = 0;
        }
    }

    for (i = 0; i < head; i++) {
        zxc++;
        ground[x][zxc - head] = i + 1;
    }
}

void frameOutput() {        
    for (i = 0; i <= width; i++) {
        printf("%c", 254);
    }

    printf("  %s   Current Score: %d  HighScore: %d", userName, currentScore, results[0]);
    printf("\n");

    for (i = 0; i < height; i++) { 
        printf("%c", 219);                            //you can change these values according to the ascii table
        for (j = 0; j < width; j++) {
            if (j == width - 1) {
                printf("%c", 219);
            } else if (ground[i][j] == 0) {
                printf(" ");
            } else if (ground[i][j] > 0 && ground[i][j] != head) {
                printf("%c", 254);
            } else if (ground[i][j] == head) {
                printf("%c", 64);
            } else if (ground[i][j] == -1) {
                printf("%c", 36);
            }
        }
        printf("\n");
    }

    for (i = 0; i <= width; i++) {
        printf("%c", 254);
    }
}

int kbhit (void) {         //khbit from curses for linux
  struct timeval tv;
  fd_set rdfs;
 
  tv.tv_sec = 0;
  tv.tv_usec = 0;
 
  FD_ZERO(&rdfs);
  FD_SET (STDIN_FILENO, &rdfs);
 
  select(STDIN_FILENO+1, &rdfs, NULL, NULL, &tv);
  return FD_ISSET(STDIN_FILENO, &rdfs);
}

int mygetch() {            //getch without enter pressing
    struct termios oldt,
    newt;
    int ch;
    tcgetattr( STDIN_FILENO, &oldt );
    newt = oldt;
    newt.c_lflag &= ~( ICANON | ECHO );
    tcsetattr( STDIN_FILENO, TCSANOW, &newt );
    ch = getchar();
    tcsetattr( STDIN_FILENO, TCSANOW, &oldt );
    return ch;
}

int getch_noblock() {
    int a;
    system("stty cbreak -echo");    //non-canonical terminal
    if (kbhit()) {
        a = mygetch();
        system("stty cooked echo");
        return a;
    } else {
        return -1;   
    }
}

void GameOver() {
    system("clear");
    n = 6;
    results[5] = currentScore;
    for(i = 0 ; i < n - 1; i++) { 
      for(j = 0 ; j < n - i - 1 ; j++) {  
        if(results[j] < results[j+1]) {
          int tmp = results[j];
          results[j] = results[j+1] ;
          results[j+1] = tmp;
        }
      }
    }
    

    usleep(5000);
    system("clear");
    for (i = 0; i < 5; i++) {
    int zxcasd = results[i];
    printf("%d.%s  %d                                                             \n", i, userName, zxcasd);
    }
    printf("\n\n         Game over\n");
    printf("             Score : %d \n\n", currentScore);
    printf("             Press G to play again or ESC to exit ... \n");

    while (1) {
        var = getch_noblock ();
        if (var == 'g') {
            game = 0;
            gameInitialization();
            break;
        } else if (var == 27) {
            game = 1;
            break;
        }
    }
    system("clear");
}

void sweetSpawn() {
    srand(time(NULL));
    a = 1 + rand() % 18;
    b = 1 + rand() % 68;

    if (sweet == 0 && ground[a][b] == 0) {
        ground[a][b]= -1;
        sweet = 1;
        if (movementSpeed > 10000 && currentScore != 0) movementSpeed = movementSpeed - 500;
    }
}

void movement() {
    var = getch_noblock();
    var = tolower(var);

    if (((var == 'd'|| var == 'a') || (var == 'w' || var == 's')) && (abs(dir - var) > 5)) { dir = var; }

    if (dir == 'd') {
        y++;
        if (y == width - 1) { 
            GameOver(); 
            } else if (ground[x][y] != 0 && ground[x][y]!= -1) { 
            GameOver(); 
            } else if (ground[x][y] == -1) {
            sweet = 0;
            currentScore += 5;
            tail -= 1;
        }

        head++;
        ground[x][y] = head;
    }

    if (dir == 'a') {
        y--;
        if (y == -1) { 
            GameOver(); 
            } else if (ground[x][y] != 0 && ground[x][y] != -1) { 
            GameOver(); 
            } else if (ground[x][y] == -1) {
            sweet = 0;
            currentScore += 5;
            tail -= 1;
        }
        head++;
        ground[x][y] = head;
    }

    if (dir == 'w') {
        x--;
        if (x == -1) { 
            GameOver(); 
        } else if (ground[x][y] != 0 && ground[x][y] != -1) { 
            GameOver(); 
            } else if (ground[x][y] == -1) {
            sweet = 0;
            currentScore += 5;
            tail -= 1;
        }
        head++;
        ground[x][y] = head;
    }

    if (dir == 's') {
        x++;
        if (x == height) { 
            GameOver(); 
            } else if (ground[x][y] != 0 && ground[x][y] != -1) { 
            GameOver(); 
            } else if (ground[x][y] == -1) {
            sweet = 0;
            currentScore += 5;
            tail -= 1;
        }
        head++;
        ground[x][y] = head;
    }
}

void tailRemove() {
    for (i = 0; i < height; i++) {
        for (j = 0; j < width; j++) {
            if (ground[i][j] == tail) {
                ground[i][j] = 0;
            }
        }
    }
    tail++;
}

void resetCursorPosition() {
  printf("%c[%d;%df",0x1B, 0,0);
}

void main() {
    beforeGameStart();
    gameInitialization();

    while (game == 0) {
        frameOutput();
        resetCursorPosition();
        sweetSpawn();
        movement();
        tailRemove();
        usleep(movementSpeed);
    }
}
