/*
// Sun Jun 21 14:27:37 WIB 2020
// Tue Jun  9 17:46:47 WIB 2020
 */

// adjust DELAY and akunGitHub
#define  DELAY 5
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
    sem_init(&(mymap->mutex), 1, 1);
  	mymap->entry=0;
  	mymap->mutexctr=0;
  	mymap->state=OPEN;
    //  more INIT STUFFs...
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
    int entry = -1;
    // get an entry number
    int cntEntry;
    cntEntry = mymap->entry;
    usrinfo listOfUser[MAXPROGS+1];
    memcpy(listOfUser, mymap->progs, sizeof(listOfUser));
  	for (int j = 0; j < cntEntry; j++) {
      	if (listOfUser[j].akun[0] != '\0') {
        	if (strcmp(listOfUser[j].akun, akunGitHub) == 0) {
                entry = j;
                break;
        	}
        }
      	else {
        	break;
        }
    }
    return entry;
}

void display(int entry) {
    // display an entry of MMAP. Eg.
    // akunGH2[progs[03] TIME[18] MUTEX[05] MMAP[OPEN] [akunGH1][akunGH3][akunGH0][akunGH2]]
  	int cntMutex;
  	cntMutex = mymap->mutexctr;
  	int userStamp;
  	userStamp = mymap->progs[entry].stamp;
  	printf("%s[progs[%02d] TIME[%02d] MUTEX[%02d] MMAP[OPEN] ", akunGitHub, entry, cntMutex, userStamp);
  	usrinfo listOfUser[MAXPROGS+1];
    memcpy(listOfUser, mymap->progs, sizeof(listOfUser));
  	int cntEntry;
  	cntEntry = mymap->entry;
  	for (int j = 0; j < cntEntry; j++) {
      	if (listOfUser[j].akun[0] == '\0') {
        	printf("[]");
        }
      	else {
        	printf("[%s]", listOfUser[j].akun);
        }
    }
  	printf("]\n");
}

void putInfo(char* akun, int entry) {
    // put "akunGitHub" into akun[] array (MMAP)
  	sem_wait(&(mymap->mutex));
  	mymap->mutexctr++;
  	if (getEntry() == -1) {
        memcpy(mymap->progs[entry].akun, akun, sizeof(mymap->progs[entry].akun));
        mymap->progs[entry].stamp = 0;
        mymap->entry++;
    }
  	else {
      	mymap->progs[getEntry()].stamp++;
    }
  	sem_post(&(mymap->mutex));
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
    sleep (DELAY);
  	int size = sizeof(progs)/sizeof(progs[0]);
  	for (int i = 0; i < size; i++){
    	if (!fork()) {
         	execlp(progs[i], NULL);
        }
        else {
            wait(NULL);
        }
    }
  	int availableEntry;
  	availableEntry = mymap->entry;
  	putInfo(akunGitHub, availableEntry);
    // blah... blah... blah...
    // blah... blah... blah...
    // blah... blah... blah...
    myprint(akunGitHub, "BYEBYE =====  ===== =====");
}
