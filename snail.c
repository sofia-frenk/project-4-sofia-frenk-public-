#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>
#include<string.h>
#include<sys/time.h>
#include<assert.h>

#include "snail.h"

/*
 * author: burt rosenberg
 * last update:
 *    20 september 2022
 *
 */

int is_verbose_g = 0 ; 
struct Board board_g ;

void * draw_thread(void * the_args) {
	struct T_arg * t_arg = (struct T_arg *) the_args ;
	struct timeval tv;
    struct timespec ts;
    
    char * board_string ;
    
	board_string = (char *) malloc(BOARD_SIZE+1) ;
	assert(board_string) ;

	while (1) {
	
// ***
// stuff to do 
// ***	
        
		pthread_mutex_lock(t_arg->mutex) ;
		{
// ***
// stuff to do 
// ***	
          
            gettimeofday(&tv, NULL) ;
            ts.tv_sec = tv.tv_sec + SLEEP_TIME ;
            ts.tv_nsec = 0 ;
            pthread_cond_timedwait (t_arg->cond, t_arg->mutex, &ts) ;
			
            //if (board_g.gate_key == GATE_IS_OPEN)
            //{
            //       board_g.gate_state = GATE_IS_OPEN;
            //}
            //else {
            //    board_g.gate_state = GATE_IS_CLOSED;
            //}
            board_g.snail_loc++;
            board_draw(board_string, &board_g);
            printf("%s\n", board_string);
            if(snail_collide(&board_g) || board_gameover(&board_g))
            {
                if(snail_collide(&board_g) && board_g.gate_state == GATE_IS_CLOSED)
                {
                    printf("The snail got smushed!\n");
                    exit(0);
                }
                else if (board_gameover(&board_g))
                {
                    printf("The snail has arrived!\n");
                    exit(0);
                }
            }
		}
		pthread_mutex_unlock(t_arg->mutex) ;
	}

	pthread_exit(NULL) ;
	assert(0==1) ; /* never gets here */
}


int main(int argc, char * argv[]) {
	int ch ;
	pthread_t thread_id ;
	char buf[LINE_MAX] ;
	struct T_arg t_arg ;
	pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
	pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
    t_arg.req_exit = 0 ;
    t_arg.mutex = &mutex ;
    t_arg.cond = &cond ;
	
	while ((ch = getopt(argc, argv, "v")) != -1) {
		switch(ch) {
			case 'v':
				is_verbose_g += 1 ;
				break ;
			default:
				printf(USAGE_MSG, PROG_NAME) ;
				return 0 ;
		}
	}
	argc -= optind;
	argv += optind;
	
	init_board( &board_g ) ;
	printf(INSTRUCTIONS, QUIT_STRING) ;
	
	if (is_verbose_g) {
		printf("(%s,%d): key %c\n",__FILE__,__LINE__,board_g.gate_key) ;
	}

// ***
// stuff to do 
// ***	
    
    if (pthread_create( &thread_id, NULL, draw_thread, (void *) &t_arg )) {
            perror("pthread_create") ;
            exit(-1) ;
    }

    //gateNumber = (rand() % (0 - 9 + 1)) + 0;
    board_g.gate_state = GATE_IS_CLOSED;
	while (fgets(buf,LINE_MAX,stdin) != NULL ) {
	
// ***
// stuff to do 
// ***	
        buf[strlen(buf)-1] = '\0' ;
        if (!strcmp(buf,QUIT_STRING) ) {
                    break ;
                }
       
        
        //char = int +'0
        
        //char gateNumberAsChar = gateNumber + '0';
        pthread_mutex_lock(&mutex); //I made this
            board_g.guess = buf[0];
        //    int compare = (char)board_g.guess;
            if (board_g.guess == board_g.gate_key)
            {
                    board_g.gate_state = GATE_IS_OPEN;
            }
            pthread_cond_signal(&cond) ;
        pthread_mutex_unlock(&mutex); //I made this
	}
	
	if (is_verbose_g) {
		printf("(%s,%d): requesting thread to exit ...\n", __FILE__,__LINE__) ;
	}

// ***
// stuff to do 
// ***	
    pthread_mutex_lock(&mutex) ;
            t_arg.req_exit = 1 ; /* tell thread to exit */
            pthread_cond_signal(&cond) ;
    pthread_mutex_unlock(&mutex) ;
	
	pthread_exit(NULL) ; /* wait for the draw thread to exit */
	assert(0==1) ; /* never gets here */
}
