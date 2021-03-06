/*
// Sun Jun 21 14:27:37 WIB 2020
// Tue Jun  9 17:46:47 WIB 2020
 */

// adjust DELAY and akunGitHub
#define  DELAY 15
#define  akunGitHub "GibranBrahmanta"
#include "p00.h"

char*    progs[]={P01, P02, P03, P04, P05, P06};
myshare* mymap;

int init(void) {
    int ssize=sizeof(myshare);
    int fd   =open(SHAREMEM, MYFLAGS, CHMOD);
    fchmod   (fd, CHMOD);
    ftruncate(fd, ssize);
    mymap=mmap(NULL, ssize, MYPROTECTION, MYVISIBILITY, fd, 0);
    close(fd);
  	mymap->entry=0;
  	mymap->mutexctr=0;
  	mymap->state=OPEN;
	sem_init(&(mymap->mutex), 0, 1);
    return BOSS;
}

char tmpStr[256]={};  // a temporary string.
void myprint(char* str1, char* str2) {
    printf("%s[%s]\n", str1, str2);
    // blah blah blah
    // blah blah blah
    // blah blah blah
  
}

int getEntry(void) {
    sem_wait(&(mymap -> mutex));
    int entry = 0;
    entry = mymap -> entry++;
	mymap -> mutexctr++;
    mymap -> progs[entry].stamp = 1;
    sem_post(&(mymap -> mutex));
    return entry;
}

void display(int entry) {
    // display an entry of MMAP. Eg.
    // akunGH2[progs[03] TIME[18] MUTEX[05] MMAP[OPEN] [akunGH1][akunGH3][akunGH0][akunGH2]]
  	int cntMutex;
  	cntMutex = mymap->mutexctr++;
  	int userStamp;
  	userStamp = mymap->progs[entry].stamp++;
  	printf("%s[progs[%02d] TIME[%02d] MUTEX[%02d] MMAP[OPEN] ", akunGitHub, entry, cntMutex, userStamp);
  	
	int cntEntry;
  	cntEntry = mymap->entry;
  	for (int j = 0; j < cntEntry; j++) {
      	printf("[%s]", mymap -> progs[j].akun);
    }
	printf("]\n");
	
	/*
	usrinfo listOfUser[MAXPROGS+1];
    memcpy(listOfUser, mymap->progs, sizeof(listOfUser));
  	int cntEntry;
  	cntEntry = mymap->entry;
  	for (int j = 0; j < cntEntry; j++) {
      	printf("[%s]", listOfUser[j].akun);
    }
  	printf("]\n");
	*/
}

void putInfo(char* akun, int entry) {
    // put "akunGitHub" into akun[] array (MMAP)
  	sem_wait(&mymap->mutex);
	
  	mymap->mutexctr++;
	mymap->progs[entry].stamp++;
	strcpy(mymap->progs[entry].akun, akun);
  	/*
	if (getEntry() == -1) {
        memcpy(mymap->progs[entry].akun, akun, sizeof(mymap->progs[entry].akun));
        mymap->progs[entry].stamp = 0;
        mymap->entry++;
    }
  	else {
      	mymap->progs[getEntry()].stamp++;
    }
	*/
  	sem_post(&mymap->mutex);
}

void checkOpen(void) {
    // exit if MMAP is closed.
  	int isClosed;
  	isClosed = mymap->state;
  	if (isClosed == CLOSED) {
     	exit(0); 
    }
}

int main(void) {
    sprintf(tmpStr, "START PID[%d] PPID[%d]", getpid(), getppid());
    myprint(akunGitHub, tmpStr);
    int boss=init();
    checkOpen();

  	for (int i = 0; i < 6; i++){
    	if (!fork()) {
         	execlp(progs[i], NULL);
        }
    }
	
	sleep (DELAY);
	int entry = getEntry();

	for (int i = 0; i < 3; i++) {
		display (entry);
		if (i == 0) {
			putInfo (akunGitHub, entry);
		}
	}
	
	mymap -> state = CLOSED;
    // blah... blah... blah...
    // blah... blah... blah...
    // blah... blah... blah...
    myprint(akunGitHub, "BYEBYE =====  ===== =====");
}
