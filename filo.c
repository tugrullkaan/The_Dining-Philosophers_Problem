#include <pthread.h>   //ataman
#include <semaphore.h>
#include <stdio.h>
#include <unistd.h>

#define N 5 // Filozof Sayısı
#define Dusunuyor 2
#define Ac 1
#define YemekYiyor 0
#define Sol (phnum + 4) % N
#define Sag (phnum + 1) % N

int state[N];
int Filozof[N] = {0,1,2,3,4};

sem_t mutex;
sem_t S[N];

void* FilozofIslem(void* num)
{
    while(1)
    {
        int* i = num;
        sleep(1);
        take_stick(*i);
        sleep(0);
        put_stick(*i);
    }
}

void put_stick(int phnum) //ataman
{
	sem_wait(&mutex);
	
	state[phnum]=Dusunuyor;
	
	drawState();
	printf("%d. Filozof %d. ve %d. cubugu masaya koydu.\n\n",
		phnum + 1, Sol + 1, phnum + 1);

	eat_test(Sol);
	eat_test(Sag);
	
	sem_post(&mutex);
}

void take_stick(int phnum) //burak
{
	sem_wait(&mutex);
	state[phnum]=Ac;
	drawState();
	eat_test(phnum);

	sem_post(&mutex);
	sem_wait(&S[phnum]);

	sleep(1);
}

void eat_test(int phnum) //burak
{
 if(state[phnum] == Ac
  && state[Sol] != YemekYiyor
  && state[Sag] != YemekYiyor) {
//durum yemek yiyor
   state[phnum] = YemekYiyor ;

sleep(2);

drawState();
printf("%d. Filozof %d. ve %d. cubugu masadan aldı. \n",
        phnum + 1, Sol + 1, phnum + 1) ;

sem_post(&S[phnum]);
  }
}


void drawState(){ //osman
printf("-------------------------------------------\n\n");
printf("%5d. Filozof\t %5s\n",1, getStatusText(state[0]));
printf("%5d. Filozof\t %5s\n",2, getStatusText(state[1]));
printf("%5d. Filozof\t %5s\n",3, getStatusText(state[2]));
printf("%5d. Filozof\t %5s\n",4, getStatusText(state[3]));
printf("%5d. Filozof\t %5s\n",5, getStatusText(state[4]));
}

char* getStatusText(int stateValue){  //osman
    char *text ="Flavio";
    if(stateValue == YemekYiyor)
        return "YemekYiyor";
    else if(stateValue == Ac)
        return "Ac";
    else 
        return "Dusunuyor";
}



int main() // tugrul
{
    int i;
    pthread_t thread_id[N];
    
    sem_init(&mutex, 0, 1);
    
    for (i = 0; i < N; i++)
        sem_init(&S[i], 0, 0);
    
    for (i = 0; i < N; i++) {
        pthread_create(&thread_id[i], NULL, FilozofIslem, &Filozof[i]);
        drawState();
        
    }
    
    for (i = 0; i < N; i++)
        pthread_join(thread_id[i], NULL);

    
}
