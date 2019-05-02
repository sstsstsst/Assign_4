#include <iostream> 

#include <fstream>
#include <unistd.h>

#include <cstdlib>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
// To get timestamp _____

#include <iostream>
#include <iomanip>
#include <ctime>
#include <sstream>
//_______________________

#define MAXLINE 4096 /*max text line length*/
// #define SERV_PORT 30000 /*port*/
using namespace std; 



  std::string getTimestamp();
  int execClient(int argc, char **argv);
  void writeToFile(char* fileLoc, char* str);
  void clearFile(char* fileLoc);



int main(int argc, char** argv) { 

  execClient(argc, argv);


  exit(0);
} 


int
execClient(int argc, char **argv) {

  // servaddr.sin_addr.s_addr = inet_addr(argv[1]);
  int SERV_PORT = atoi(argv[2]);
  char* commandFileLoc = argv[3];


  //create socket, and connect to server
  int sockfd;
  struct sockaddr_in servaddr;
  char sendline[MAXLINE], recvline[MAXLINE];
  
   //basic check of the arguments
   //additional checks can be inserted
   if (argc != 4) {
    perror("Usage: TCPClient <IP address of the server"); 
    exit(1);
   }
    
   //Create a socket for the client
   //If sockfd<0 there was an error in the creation of the socket
   if ((sockfd = socket (AF_INET, SOCK_STREAM, 0)) <0) {
    perror("Problem in creating the socket");
    exit(2);
   }
    
   //Creation of the socket
   memset(&servaddr, 0, sizeof(servaddr));
   servaddr.sin_family = AF_INET;
   servaddr.sin_addr.s_addr= inet_addr(argv[1]);
   servaddr.sin_port =  htons(SERV_PORT); //convert to big-endian order
    
   //Connection of the client to the socket 
   if (connect(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr))<0) {
    perror("Problem in connecting to the server");
    exit(3);
   }


   //client connected
   cout << getTimestamp() + "\tclient\tpid is [" <<  getpid() << "]\t\t :\tconnected." << endl;

   //read from commandFile
  string item_name;
  ifstream nameFileout;
  nameFileout.open(commandFileLoc);
  string line;
  while(std::getline(nameFileout, line))
  {
    //line is a line in the commandFile

   
      //output to console
      //cout << getTimestamp() + "\tclient\tpid : [" <<  getpid() << "]\t\t :\t" << line << endl;

      //sent to Server
    

        //put the the string line into the char array sendline
        const char* charAry = line.c_str();
        for (int i = 0; i < MAXLINE; i++){
            if(i < strlen(line.c_str())){
              //add to char array
              sendline[i] = charAry[i];
            }else{
              //add null character at end
              sendline[i] = '\0';
            }
          }

        cout << getTimestamp() + "\tclient\tpid : [" <<  getpid() << "]\t\t sending:\t" << sendline << endl;

        send(sockfd, sendline, strlen(sendline), 0);
        
        if (recv(sockfd, recvline, MAXLINE,0) == 0){

           //error: server terminated prematurely
          perror("The server terminated prematurely"); 
          exit(4);

        }else{
          if(recvline == "end"){
            //END PROGRAM
            cout << "ENDING" << endl;
            // cout << "YOU WANT ME TO END BRUH!" << endl;
            exit(5);
          }
          //recieve output
          cout << getTimestamp() + "\tclient\tpid : [" <<  getpid() << "]\t\t recieved:\t" << recvline << endl;
          // printf("%s", "String received from the server: ");
          //fputs(recvline, stdout);
        }

      //write to log file

      //client PID ???

      //to check what line holds
      // std::cout << line << std::endl;
    
     
      
   
  }


   

   // exit(0);



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



// while (fgets(sendline, MAXLINE, stdin) != NULL) {
    
//     send(sockfd, sendline, strlen(sendline), 0);
      
//     if (recv(sockfd, recvline, MAXLINE,0) == 0){
//      //error: server terminated prematurely
//      perror("The server terminated prematurely"); 
//      exit(4);
//     }
//     printf("%s", "String received from the server: ");
//     fputs(recvline, stdout);
//    }