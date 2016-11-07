#include <algorithm>
#include <math.h>
#include <iostream> 
#include <stdlib.h> 
#include <string.h>

using namespace std;

int converted_timestamp(timestamp t){                     
int time_calculations;
time_calculations = 0;

time_calculations = time_calculations + t.hh * 3600;                          
time_calculations = time_calculations + t.mm * 60;                            
time_calculations = time_calculations + t.ss;                                 

return time_calculations;	
}

timestamp converted_time(int finished_time){                       
timestamp t;

t.hh = finished_time / 3600;
finished_time = finished_time % 3600;
t.mm = finished_time / 60;
finished_time = finished_time % 60;
t.ss = finished_time;

return t;
}

int getPrio (uri* max, request* ongoing){           

while(strcmp(ongoing -> name, max -> name) != 0){      
	max = max -> next;
	}

return max -> priority;                              
}

float getCalcDelay (uri* max, request* ongoing){            
	
while(strcmp(ongoing -> name, max -> name) != 0){      
	max = max -> next;
	}
	
int calc_delay;
calc_delay = max -> size * 0.8;

return calc_delay;                                       
}

struct request_queue {                                  

float calc_delay;                               		   
int prio;
int time_caluclations;                                 
request_queue* next;
};

void queue_constructor (uri* max, request* ongoing, request_queue *& counted){  

request_queue* a = new request_queue;
a -> calc_delay = getCalcDelay(max, ongoing);                  
a -> prio = getPrio(max, ongoing);           
a -> time_caluclations = converted_timestamp(ongoing -> t);             
a -> next = counted;                                  
counted = a;
}

void prio_sorter (request_queue *& header){

if (header == NULL){
	return;
}
	
bool not_sorted = false;
int counter;
float other_counter;

while(not_sorted){
	not_sorted = true;
	request_queue* ongoing = header;

	while(ongoing -> next != NULL){
		if(ongoing -> prio < ongoing -> next -> prio){
			counter = ongoing -> prio;
			ongoing -> prio = ongoing -> next -> prio;
			ongoing -> next -> prio = counter;	
			other_counter = ongoing -> calc_delay;
			ongoing -> calc_delay = ongoing -> next -> calc_delay;
			ongoing -> next -> calc_delay = counter;		
			counter = ongoing -> time_caluclations;
			ongoing -> time_caluclations = ongoing -> next -> time_caluclations;
			ongoing -> next -> time_caluclations = counter;
				
			not_sorted = false;
		}
	
	ongoing = ongoing -> next;
	}
}
}

timestamp maxClientDelay (uri* max, request* header){      
int time_gone = 0;                               
time_gone = converted_timestamp(header -> t);           	
int count = 0;                                       
request_queue* another = 0;                        
int waitingtime = 0;
while (header != NULL or another != NULL){
	for (int i = 0; i <= count; i++){                
		while(header != NULL and (converted_timestamp(header -> t) - time_gone) == i){     
				queue_constructor (max, header, another);
				header = header -> next;                      
		}
	}
		
	prio_sorter(another);                          
	time_gone = time_gone + count;             

	if (waitingtime < (time_gone - another -> time_caluclations)){  
		waitingtime = time_gone - another -> time_caluclations;
	}
		
	if (another == NULL and header != NULL){           
		time_gone = converted_timestamp(header -> t);   
		count = 0;                                  
	}                       
	
	if (another != NULL){
		count = ceil(another -> calc_delay);                
		another = another -> next;                    
	}		
}

timestamp waitingtimestamp = converted_time(waitingtime); 
    
return waitingtimestamp;
}