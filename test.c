#include <curses.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <math.h> 
#include <time.h>

#define MAX(a,b) (a>b)?a:b
#define MIN(a,b) (a<b>?a:b

#define CATCHMARKER A_BOLD
enum {
	UP,
	DOWN,
	LEFT,
	RIGHT
	} ;
static void sighandler(int sig) {
	endwin() ;
	exit(0) ;
}

void movement (int *oldx, int *oldy, int dir) {
	if (dir == LEFT) {
		if (*oldy == 10 && *oldx == 1) {
			*oldx = 77 ;
			return ;
		}
		if (*oldx == 1)
			return ;
		if (*oldy <= 18 && *oldx == 40)
			return ;
		(*oldx)-- ;
	}
	else if (dir == RIGHT) {
		if (*oldy == 10 && *oldx == 77) {
			*oldx = 1 ;
			return ;
		}
		if (*oldx == 77)
			return ;
		if (*oldy <= 18 && *oldx == 38)
			return ;
		(*oldx)++ ;
	}
	else if (dir == UP) {
		if (*oldy == 1)
			return ;
		if (*oldx == 39 && *oldy == 18)
			return ;
		(*oldy)-- ;
	}
	else if (dir == DOWN) {
		if (*oldy == 22) 
			return ;
		(*oldy)++ ;
	}
}

int main() {
	unsigned int p1x=20,p1y=12,p2x=60,p2y=12,p1p=0,p2p=0,p1d=UP,p2d=UP,i,*catch=&p1p, endtime=time(NULL)+30, timenow=time(NULL) ;
	char buf[80] ;
	WINDOW *g ;
	signal(SIGINT,sighandler) ;
	initscr() ;
	keypad(stdscr, TRUE);
	nonl() ;
	cbreak() ;
	noecho() ;
	nodelay(stdscr,TRUE) ;
	if(COLS < 80 || LINES < 25) {
		printf("Fenster zu klein!") ;
		return 1 ;
	}

	g = subwin(stdscr,24,79,0,0) ;
	box(g,0,0) ;
	mvwaddch(g,0,39,ACS_TTEE) ;
	mvwaddch(g,10,78,ACS_RTEE) ;
	mvwaddch(g,10,0,ACS_LTEE) ;

	srand(23) ;
	for(i = 1;i<18;i++) 
		mvwaddch(g,i,39,ACS_VLINE) ;
	
	while(TRUE) {
		int c = getch() ;
		if ((abs(p1x - p2x) < 2 && abs(p1y - p2y) < 2) || endtime - timenow == 0) {
			mvwaddch(g,p1y,p1x,' ') ;
			mvwaddch(g,p2y,p2x,' ') ;
			if(catch == &p1p) {
				(endtime - timenow != 0) ? p1p++:0 ;
				catch=&p2p ;
			}
			else {
				(endtime - timenow != 0) ? p2p++:0 ;
				catch=&p1p ;
			}
			if(endtime - timenow != 0) {
				p1x = 20; p1y = 12 ;
				p2x = 60; p2y = 12 ;
				p1d=UP ; p2d = UP ;
			}
			endtime=time(NULL)+30 ;
		}
		sprintf(buf,"%02d : %02d",p1p,p2p) ;
		mvprintw (24,(80-strlen(buf))/2,buf) ;
		timenow = time(NULL) ;
		mvprintw(24,0,"0:%02d",endtime-timenow) ;
		mvwaddch(g,p1y,p1x,' ') ;
		mvwaddch(g,p2y,p2x,' ') ;
		
		switch(c) {
			case KEY_RIGHT:
				p2d = RIGHT ;
				break ;
			case KEY_LEFT:
				p2d = LEFT ;
				break ;
			case KEY_UP:
				p2d = UP ;
				break ;
			case KEY_DOWN:
				p2d = DOWN ;
				break ;
			case 'w':
				p1d = UP ;
				break ;
			case 'a':
				p1d = LEFT ;
				break ;
			case 's':
				p1d = DOWN ;
				break ;
			case 'd':
				p1d = RIGHT ;
				break ;
		}
		movement(&p1x,&p1y,p1d) ;
		movement(&p2x,&p2y,p2d) ;
		wattroff(g,CATCHMARKER) ;
		if (catch==&p1p) 
			wattron(g,CATCHMARKER) ;
		mvwaddch(g,p1y,p1x,'O') ;
		wattroff(g,CATCHMARKER) ;
		if (catch==&p2p) 
			wattron(g,CATCHMARKER) ;
		mvwaddch(g,p2y,p2x,'O') ;
		wattroff(g,CATCHMARKER) ;
		wrefresh(g) ;
		refresh() ;
		usleep(40000) ;
	}


	finish (0) ;

}
