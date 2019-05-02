
#include <iostream>
#include <fstream>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
// To get timestamp _____
#include <iostream>
#include <iomanip>
#include <ctime>
#include <sstream>
//_______________________
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <signal.h>
#include <unistd.h>

#define MAXLINE 4096 /*max text line length*/
// #define SERV_PORT 3000 /*port*/
#define LISTENQ 8 /*maximum number of client connections*/
using namespace std; 
// char toSend[1000000];

std::string getTimestamp();
int execServer(int argc, char **argv);
void writeToFile(char* fileLoc, char* str);
void clearFile(char* fileLoc);

static int timer_expired = 0;
static void alarm_handler(int sig)
{
  timer_expired=30;
}

int main (int argc, char **argv){
  // Set up a signal hander to call alarm_handler() 
  // when SIGALRM is raised
  // sigaction(SIGALRM, &(struct sigaction){.sa_handler = alarm_handler}, NULL);
 
  // time_t t;
  // srand((unsigned) time(&t));
 
  // timer_expired = 0;
  // alarm(1); // raise SIGALRM in 1 seconds

  // while (!timer_expired) {
  //   int s=(rand()%4)+1;
  //   printf("%d", s);
  // }
  

  execServer(argc, argv);
  // clearFile("server_output.txt");

  // system(" date >> server_output.txt");
  // system(" ls >> server_output.txt");
  // system("cat server_output.txt");

  return 0;
}	

int execServer(int argc, char **argv){
  int SERV_PORT = atoi(argv[1]);

  int listenfd, connfd, n;
  pid_t childpid;
  socklen_t clilen;
  char buf[MAXLINE];
  struct sockaddr_in cliaddr, servaddr;

  //Create a socket for the socket
  //If sockfd<0 there was an error in the creation of the socket
  if ((listenfd = socket (AF_INET, SOCK_STREAM, 0)) <0) {
    perror("Problem in creating the socket");
    exit(2);
  }

  //preparation of the socket address
  servaddr.sin_family = AF_INET;
  servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
  servaddr.sin_port = htons(SERV_PORT);

  //bind the socket
  bind (listenfd, (struct sockaddr *) &servaddr, sizeof(servaddr));

  //listen to the socket by creating a connection queue, then wait for clients
  listen (listenfd, LISTENQ);

  // printf("%s\n","Server running...waiting for connections.");
  cout << getTimestamp() << "\tpid is [" << getpid() << "]\tport is (" << SERV_PORT << ")" << "\t...waiting for connections.\n" << endl;

  for ( ; ; ) {
  clilen = sizeof(cliaddr);
  //accept a connection

  connfd = accept (listenfd, (struct sockaddr *) &cliaddr, &clilen);

  cout << getTimestamp() << "\tpid is [" << getpid() << "]\t server is connected." << endl;
  // printf("%s\n","Received request...");

   if ( (childpid = fork ()) == 0 ) { //if it’s 0, it’s child process
      cout << getTimestamp() << "\tpid is [" << getpid() << "]\tChild created for dealing with client requests\n" << endl;
      // printf ("%s\n","Child created for dealing with client requests");
      //close listening socket
      close (listenfd);

      //clear buf
      for(int p = 0; p < MAXLINE; p++){
        buf[p] = '\0';
      }
      clearFile("server_output.txt");
      // writeToFile("server_output.txt", "WRITTING TO FILE BRUH!!!\n");

      while ( (n = recv(connfd, buf, MAXLINE,0)) > 0)  {
        cout << getTimestamp() + "\tserver\tpid : [" <<  getpid() << "]\t\t recieved:\t" << buf << endl;
        // printf("%s","String received from and resent to the client:");
        // puts(buf);

        
        // writeToFile("server_output.txt", "WRITTING TO FILE BRUH!!!");
        //put output of system call in file
        //convert buf which is a char array
        char charAry[n];
        for(int l = 0; l < n; l++){
          charAry[l] =  buf[l];
        }
        
        std::string cmdStr = "\0";

        for(int g = 0; g < n; g++){
          cmdStr = cmdStr + charAry[g];
        }

        if(cmdStr == "end"){
          // cout << "COMMAND STR IS --> [" << cmdStr.c_str() << "]" << endl;

          char toSend[10000];
          const char* cmdStrAry = cmdStr.c_str();
          for(int u = 0; u < strlen(cmdStr.c_str()); u++){
            toSend[u] = cmdStrAry[u];
          }

          send(connfd, toSend, n, 0);
          cout << getTimestamp() + "\tserver\tpid : [" <<  getpid() << "]\t\t closing connection.\t" << endl;
          close(connfd);
          // exit(5);
        }else{
          cmdStr = cmdStr + " >> server_output.txt";
          // cout << "COMMAND STR IS --> [" << cmdStr.c_str() << "]" << endl;

          system(cmdStr.c_str());
          // writeToFile("server_output.txt", "_____\n");

          string item_name;
          ifstream nameFileout;
          nameFileout.open("server_output.txt");
          string line = "";
          string readAll = "";
          while(std::getline(nameFileout, line))
          {
            readAll = readAll + line + "\n";
          }
          //convert cmdStr to char array with fixed length

          char toSend[1000000];
          //clear toSend
          // for(int t = 0; t < 1000000; t++){
          //   toSend[t] = '\0';
          // }

          const char* readAllAry = readAll.c_str();
          for(int u = 0; u < strlen(readAll.c_str()); u++){
            toSend[u] = readAllAry[u];
          }

          cout << getTimestamp() + "\tserver\tpid : [" <<  getpid() << "]\t\t sending:\t" << toSend << endl;
          send(connfd, toSend, n, 0);

          clearFile("server_output.txt");

          if (n < 0){
            printf("%s\n", "Read error");
          }
        }


      }

        
        exit(0);
    }
      //close socket of the server
      close(connfd);
  }
}





std::string getTimestamp(){  
    std::time_t time_t_var = std::time(nullptr);
    wchar_t buffer[30] = L"";
    
    if ( time_t_var > 0 )
    {
        wcsftime( buffer , 30 , L"%y:%m:%d %X" , localtime( &time_t_var ) );
    } 
    wstring ws(buffer);
    std::string s( ws.begin(), ws.end() );
    return s;
}


void clearFile(char* fileLoc){
  std::ofstream ofs;
  ofs.open(fileLoc, std::ofstream::out | std::ofstream::trunc);
  ofs.close();
}

void writeToFile(char* fileLoc, char* str){

  fstream f(fileLoc, f.out | f.app);
  f << str;

}




