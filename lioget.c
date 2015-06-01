#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <netdb.h>
#include <errno.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>



int main(int argc, char* argv[])

{
    int socklio;
    int bytesRcvd;
    int bufsize = 1024;
    struct hostent *dest;
    struct sockaddr_in servidor;

    
    if (argc != 4)
    {
       fprintf(stderr, "Error! Inserir: Host de destino, arquivo e nome.\n");
       exit(1);
    }
   
    if ((dest = gethostbyname(argv[1]))== NULL)
    {  
       perror("gethostbyname Error");
       exit (1);
    }

    if ((socklio = socket(PF_INET, SOCK_STREAM, 0)) == -1)
    {
       perror(" Socket Error");
       exit(1);   
    }
    puts(" \n Socket, ok!\n");
   
/*estrutura de endereço socket*/ 

    servidor.sin_family = AF_INET;
    servidor.sin_port = htons(80);
    servidor.sin_addr = *((struct in_addr *)dest);
    bzero(&(servidor.sin_zero), 8);//zerar o resto da estrutura

/*connecting the client to the socket*/
    if (connect (socklio, (struct sockaddr*) &servidor, sizeof(servidor)) == -1)
    {
       perror("connect Error");
       exit(1);
    }
       puts(" Conexao com o servidor, ok!\n");


    char request[100];
      
    strcpy (request,"GET /");
    strcat (request, argv[2]);   
    strcat (request," HTTP/1.0\r\n\r\n");
         
    printf (" send: %s", request);
       

    if (send (socklio, request, strlen(request), 0) < 0)
    {
       perror(" send failed\n");
       exit (1);
    }
       puts (" Send, ok!\n");

    
    char buf_recev [1024]; 
    
    FILE *datafile;    

    datafile = fopen (argv[3], "w"); 
    
    if (datafile == NULL)
    {
       puts(" FILE cant open!\n"); 
       exit (1);
    }
       puts (" Arquivo criado, ok!\n");
       

/*Receber arquivo do servidor*/ 

    memset(buf_recev, '\0', bufsize);
    
    bytesRcvd = recv (socklio, buf_recev, bufsize, 0);
    fwrite (buf_recev, sizeof (char), bytesRcvd, datafile);

    if (bytesRcvd < 0)
    {
      perror(" recv\n");
      exit (1);
    }  
      puts(" DATA recv, Ok!\n");

/*Loop para arquivo*/       
    while (bytesRcvd)
    { 
     
    if (bytesRcvd < 0)
    {
       perror (" Cant read server");
       exit(1);
    }

    memset (buf_recev, '\0', bufsize);  
    bytesRcvd = recv (socklio, buf_recev, bufsize, 0);

    }
      puts (" DATA save, OK!\n");

/*Fechar o socket e o arquivo FILE.*/
      close (socklio);
      fclose (datafile);  

  return 0;
}
