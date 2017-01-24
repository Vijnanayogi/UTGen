
/*
gcc UTGen.c -o Sender -w -lpthread -lm
gcc UTGen.c -o Receiver -w -lpthread -lm

./Sender -m 1 -s localhost  -z 256 -t 10  -f 2 -p t -d 0 -r 0
./Receiver -m 0  -z 256 -f 1 -p t -d 0 -r 0

for HTTP
gcc UTGen.c -o HTTPReq  -w
./HTTPReq -h coding.debuntu.org -g index.html -t 10 -p h -f 1


*/

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <time.h>
#include <sys/time.h>
#include <pthread.h>
#include "UTGen.h"
#include <netinet/tcp.h>
#include <assert.h>
#include <sys/ioctl.h> 
#include <math.h>

int  NUM_THREADS;
void run();

void init_port()
{
  int i=0;
   
/*  if(protocol=='m')
   {   
  for(i=0;i<TCP_Thread;i++)
  port[i]=5000+i;
  
  for(i=TCP_Thread;i<UDP_Thread+TCP_Thread;i++)
  port[i]=5000+TCP_Thread+1;
    }
    
    else 
    {*/
      for(i=0;i<NUM_THREADS;i++)
      port[i]=5000+i;
 // }
}


void *Send_http_request()
{
	
      double Loop_time=Sen_Loop_time;
      int port=80;
      int sent=0; 
      int sock,on=1,z,write_size;
      double Sen_Loop_time;
      struct timeval  tv1,tv2;
      int sndbuf=sockbuffer;
      socklen_t optlen;  /* Option length */
      struct sockaddr_in serv_addr;
      
      int packet_size=8960;
      char *buffer; 
      buffer= malloc(sizeof(char) *packet_size);
      int rate_factor=1;    
       
      
      int t=0;
     
       
       if((sock = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP)) < 0)
         {
        perror("Can't create TCP socket");
        exit(1);
         }
     
                

 			if (Buffer_flag==1)
       {
   	
 			 z = setsockopt(sock,SOL_SOCKET,SO_SNDBUF,&sndbuf,sizeof sndbuf);  
 			 if(z)
 				 {
  				perror("SO_SNDBUF");   
  				exit(1);
  			    }
 				 optlen = sizeof sndbuf;
 			 z = getsockopt(sock,SOL_SOCKET,SO_SNDBUF,&sndbuf,&optlen);
 			 if(z)
 				 {
  				perror("SO_SNDBUF");   
  				exit(1);
  			    }
  				assert(optlen == sizeof sndbuf); 
 			 printf("Socket : %d\n",sock);  
 			 printf(" Send buf: %d bytes\n", sndbuf);  
  
       }
    
      
    bzero((char *) &serv_addr, sizeof(serv_addr)); 
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr,(char *)&serv_addr.sin_addr.s_addr, server->h_length);    
    serv_addr.sin_port = htons(port);
    
   
    if (connect(sock,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) 
             {
  				perror("Error Connecting");   
  				exit(1);
  			    }  

      ip =get_ip(host);
      fprintf(stderr, "IP is %s\n", ip);
      get=build_get_query(host,page);
      fprintf(stderr, "Query is:\n<<START>>\n%s<<END>>\n", get);
    		//sleep(2);
    		
   gettimeofday(&tv1, NULL);
 while ((tv2.tv_sec-tv1.tv_sec)<=Loop_time)  
     {
     
      write_size=send(sock,get,strlen(get),0); 
     // nu_pkt++; 
     total_Sent++;
     sent++;  
   printf("\n Bytes Sent %d\n  ", write_size);      
                    
    if (write_size < 0) 
         {
         perror("ERROR writing to socket");
         break;
         }
            
    gettimeofday(&tv2, NULL);
      num_of_pkt--;
     } //(num_of_pkt > 0);//

    Sen_Loop_time=(double) (tv2.tv_usec - tv1.tv_usec) / 1000000 + (double) (tv2.tv_sec - tv1.tv_sec);
    printf("\n Closing Thread No %d\n",port);
    printf("\n Total TCP Packet Sent by  %d is %d\n",port,sent);
    close(sock);
    
    }

   
 char *get_ip(char *host)
    {
      struct hostent *hent;
      int iplen = 15; //XXX.XXX.XXX.XXX
      char *ip = (char *)malloc(iplen+1);
      memset(ip, 0, iplen+1);
      if((hent = gethostbyname(host)) == NULL)
      {
        herror("Can't get IP");
        exit(1);
      }
      if(inet_ntop(AF_INET, (void *)hent->h_addr_list[0], ip, iplen) == NULL)
      {
        perror("Can't resolve host");
        exit(1);
      }
      return ip;
    }
    
    
char *build_get_query(char *host, char *page)
{
  char *query;
  char *getpage = page;
  char *tpl = "GET /%s HTTP/1.0\r\nHost: %s\r\nUser-Agent: %s\r\n\r\n";
  if(getpage[0] == '/'){
    getpage = getpage + 1;
    fprintf(stderr,"Removing leading \"/\", converting %s to %s\n", page, getpage);
  }
  // -5 is to consider the %s %s %s in tpl and the ending \0
  query = (char *)malloc(strlen(host)+strlen(getpage)+strlen(USERAGENT)+strlen(tpl)-5);
  sprintf(query, tpl, getpage, host, USERAGENT);
  return query;
}


	
void *Receive_tcp (void *j)
{
	 
	  // int nu_pkt=0;
     int port= *(int*)j; 
     int sock,newsock;
     int z,i=0;
     socklen_t  optlen;
     socklen_t  clilen;
     struct sockaddr_in serv_addr, cli_addr;
     int rcvbuf=sockbuffer;
     double Rec_Loop_time;
     struct timeval  tv1, tv2;	
     int read_size=1;
     int rcv=0;    
    
     if((sock = socket(AF_INET,SOCK_STREAM, IPPROTO_TCP)) < 0)
         {
        perror("Can't create TCP socket");
        exit(1);
         }
       
      if (Buffer_flag==1)
        {
 			   //rcvbuf =Socket_Buffer;   
  				z = setsockopt(sock,SOL_SOCKET,SO_RCVBUF,&rcvbuf,sizeof rcvbuf);  
 				 if(z)
 				 {
 				 error("SO_RCVBUF"); 
 				 exit(1);
 				 }
 				 optlen = sizeof rcvbuf;  
 				 
 				 z = getsockopt(sock,SOL_SOCKET,SO_RCVBUF,&rcvbuf,&optlen); 
 				 if(z)
 				 {
  				error("SO_RCVBUF");   
  				exit(1);
  			    }
  				assert(optlen == sizeof rcvbuf);      
 			   printf("Socket s : %d\n",sock);  
 			   printf(" Recv buf: %d MB \n",rcvbuf); 
			}  
      
     
    
    bzero((char *) &serv_addr, sizeof(serv_addr)); 
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr=INADDR_ANY;
    clilen = sizeof(cli_addr);
    serv_addr.sin_port = htons(port);
       
    if (bind(sock,(struct sockaddr *) &serv_addr,sizeof(serv_addr))< 0) 
    error("ERROR on binding");

   
  	if ( listen(sock, 20) != 0 )
	{
		perror("socket--listen");
		exit(0);
	}
       
       newsock=accept(sock,( struct sockaddr*) &cli_addr,&clilen);
      // printf("\n Waiting  for Next Thread No %d\n",newsock);
     
      if (newsock < 0)
      {
			fprintf(stderr,"server: accept error\n");
			 exit(0);
	   }
	   	   

			printf("\n Started Receiving Traffic  for Thread No %d\n",port);
			gettimeofday(&tv1, NULL);
   
    while(1) 
   {
   	
     read_size=read(newsock,buffer,packet_size); 
    // printf("\n Bytes Received %d\n by %d port ",read_size,port);
      if(read_size==0)
				{
         puts("Client disconnected");
         break;
				}
				 else if(read_size == -1)
				{
						perror("recv failed");
						exit(-1);
				}  
				total_Rcv++;
				rcv++;
				//printf("Bytes Received: %d\n",read_size);     
     }
   
          gettimeofday(&tv2, NULL);        
  Rec_Loop_time=(double) (tv2.tv_usec - tv1.tv_usec)/1000000+(double)(tv2.tv_sec-tv1.tv_sec);
  printf("\n Closing Thread for %d\n",port);
  printf("\n Total Packet Sent by  %d is %d\n",port,rcv);
  close(newsock);
  close(sock);
        
}
  
  

void *Send_tcp(void *i)

{	    
    
      double Loop_time=Sen_Loop_time;
      int port= *(int*)i; 
      int sent=0; 
      int sock,on=1,z,write_size;
      double Sen_Loop_time;
      struct timeval  tv1,tv2;
      int sndbuf=sockbuffer;
      socklen_t optlen;  /* Option length */
      struct sockaddr_in serv_addr;
      
      int packet_size=8960;
      char *buffer; 
      buffer= malloc(sizeof(char) *packet_size);
      int rate_factor=1;    
       
      
      int t=0;
     
       
       if((sock = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP)) < 0)
         {
        perror("Can't create TCP socket");
        exit(1);
         }
     
                

 			if (Buffer_flag==1)
       {
   	
 			 z = setsockopt(sock,SOL_SOCKET,SO_SNDBUF,&sndbuf,sizeof sndbuf);  
 			 if(z)
 				 {
  				perror("SO_SNDBUF");   
  				exit(1);
  			    }
 				 optlen = sizeof sndbuf;
 			 z = getsockopt(sock,SOL_SOCKET,SO_SNDBUF,&sndbuf,&optlen);
 			 if(z)
 				 {
  				perror("SO_SNDBUF");   
  				exit(1);
  			    }
  				assert(optlen == sizeof sndbuf); 
 			 printf("Socket : %d\n",sock);  
 			 printf(" Send buf: %d bytes\n", sndbuf);  
  
       }
    
      
    bzero((char *) &serv_addr, sizeof(serv_addr)); 
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr,(char *)&serv_addr.sin_addr.s_addr, server->h_length);    
    serv_addr.sin_port = htons(port);
    
   
    if (connect(sock,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) 
             {
  				perror("Error Connecting");   
  				exit(1);
  			    }  
   
    
    gettimeofday(&tv1, NULL);
 while ((tv2.tv_sec-tv1.tv_sec)<=Loop_time)  
     {
     	   
  
    switch(Distribution) 
    {
        case 0:
            packet_size=seed;           // Constant
            break;
            
        case 1:
            packet_size= seed*rand();					//Ramdom
            break;
                        
        case 2:
            packet_size=packet_size+seed;					//Incremental
            break;
                      
        case 3:
            packet_size=2*rate_factor*seed;
            rate_factor++;
            break;
           
       default:
           printf("Error! operator is not correct");
            break;  
     } 
     	  
   if(packet_size > 8960) 
              packet_size=packet_size%8960;
     else if(packet_size < 64 ) 
              packet_size=64;  	
      write_size= write(sock,buffer,packet_size);
     // nu_pkt++; 
     total_Sent++;
     sent++;  
  //  printf("\n Bytes Sent %d\n by %d port ", write_size,port);      
                    
    if (write_size < 0) 
         {
         perror("ERROR writing to socket");
         break;
         }
            
    gettimeofday(&tv2, NULL);
      num_of_pkt--;
     } //(num_of_pkt > 0);//

    Sen_Loop_time=(double) (tv2.tv_usec - tv1.tv_usec) / 1000000 + (double) (tv2.tv_sec - tv1.tv_sec);
    printf("\n Closing Thread No %d\n",port);
    printf("\n Total TCP Packet Sent by  %d is %d\n",port,sent);
    close(sock);
   // sleep(10);
    
     }
        

void *Receive_udp (void *j)
{
	 
	  // int nu_pkt=0;
     int port= *(int*)j; 
     int sock;
     int z,i=0;
     socklen_t  optlen;
     socklen_t  clilen;
     struct sockaddr_in serv_addr, cli_addr;
     int rcvbuf=sockbuffer;
     double Rec_Loop_time;
     struct timeval  tv1, tv2;	
     int read_size=1;
     int rcv=0;    
     
    int packet_size=8960;
    char *buffer; 
    buffer= malloc(sizeof(char) *packet_size);    
        
     if((sock = socket(AF_INET,SOCK_DGRAM, IPPROTO_UDP)) < 0)
         {
        perror("Can't create UDP socket");
        exit(1);
         }
    
   
      if (Buffer_flag==1)
        {
 			 //rcvbuf =Socket_Buffer;   
  				z = setsockopt(sock,SOL_SOCKET,SO_RCVBUF,&rcvbuf,sizeof rcvbuf);  
 				 if(z)
 				 {
 				 error("SO_RCVBUF"); 
 				 exit(1);
 				 }
 				 optlen = sizeof rcvbuf;  
 				 
 				 z = getsockopt(sock,SOL_SOCKET,SO_RCVBUF,&rcvbuf,&optlen); 
 				 if(z)
 				 {
  				error("SO_RCVBUF");   
  				exit(1);
  			    }
  				assert(optlen == sizeof rcvbuf);      
 			   printf("Socket s : %d\n",sock);  
 			   printf(" Recv buf: %d bytes\n", rcvbuf); 
			}  
          
    bzero((char *) &serv_addr, sizeof(serv_addr)); 
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr=INADDR_ANY;
    clilen = sizeof(cli_addr);
    serv_addr.sin_port = htons(port);
       
    if (bind(sock,(struct sockaddr *) &serv_addr,sizeof(serv_addr))< 0) 
    error("ERROR on binding");
    
   read_size=recvfrom(sock,buffer,packet_size, 0,&cli_addr,&clilen);    
   printf("\n Started Receiving Traffic  for Thread No %d\n",port);
	gettimeofday(&tv1, NULL);
   
    while(read_size!=4) 
   {
   	read_size=recvfrom(sock,buffer,packet_size, 0, &cli_addr,&clilen); 
    /*  if(read_size==0)
				{
         puts("Client disconnected");
         break;
				}
				 else */if(read_size == -1)
				{
						perror("recv failed");
						exit(-1);
				}  
				total_Rcv++;
				rcv++;
			//	printf("Bytes Received: %d\n",read_size);     
   }
   
  gettimeofday(&tv2, NULL);     
  Rec_Loop_time=(double) (tv2.tv_usec - tv1.tv_usec)/1000000+(double)(tv2.tv_sec-tv1.tv_sec);
  printf("\n Closing Thread for %d\n",port);
  printf("\n Total Packet Sent by  %d is %d\n",port,rcv);
  close(sock);     
}
  

void *Send_udp(void *i)
{	    
    
      double Loop_time=Sen_Loop_time;
      int port= *(int*)i; 
      int sent=0; 
      int sock,on=1,z,write_size;
      double Sen_Loop_time;
      struct timeval  tv1,tv2;
      int sndbuf=sockbuffer;
      socklen_t optlen;  /* Option length */
      struct sockaddr_in serv_addr;
      int t=0;     
      
      
      int packet_size=8960;
      char *buffer;
      buffer= malloc(sizeof(char) *packet_size);
      
            
      int rate_factor=1;  
       
       if((sock = socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP)) < 0)
         {
        perror("Can't create TCP socket");
        exit(1);
         }
     
 			if (Buffer_flag==1)
       {
   	
 			 z = setsockopt(sock,SOL_SOCKET,SO_SNDBUF,&sndbuf,sizeof sndbuf);  
 			 if(z)
 				 {
  				perror("SO_SNDBUF");   
  				exit(1);
  			    }
 				 optlen = sizeof sndbuf;
 			 z = getsockopt(sock,SOL_SOCKET,SO_SNDBUF,&sndbuf,&optlen);
 			 if(z)
 				 {
  				perror("SO_SNDBUF");   
  				exit(1);
  			    }
  				assert(optlen == sizeof sndbuf); 
 			 printf("Socket : %d\n",sock);  
 			 printf(" Send buf: %d bytes\n", sndbuf);  
  
       }
    
     // sleep(2);
    bzero((char *) &serv_addr, sizeof(serv_addr)); 
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr,(char *)&serv_addr.sin_addr.s_addr, server->h_length);    
    serv_addr.sin_port = htons(port);
    int slen=sizeof(serv_addr);
   
    if (connect(sock,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) 
       {
  		perror("Error Connecting");   
  		exit(1);
  		 }  
  
 gettimeofday(&tv1, NULL);
 while((tv2.tv_sec-tv1.tv_sec)<=Loop_time)  
     {
     	
         switch(Distribution) 
    {
        case 0:
            packet_size=seed;           // Constant
            break;
            
        case 1:
            packet_size= seed*rand();					//Ramdom
            break;
                        
        case 2:
            packet_size=packet_size+seed;					//Incremental
            break;
                      
        case 3:
            packet_size=2*rate_factor*seed;
            rate_factor++;
            break;
           
       default:
           printf("Error! operator is not correct");
            break;  
     } 
     	
     	 if(packet_size > 8960) 
              packet_size=packet_size%8960;
     else if(packet_size < 64 ) 
              packet_size=64;  	       	
          	    	
      write_size=sendto(sock,buffer,packet_size,0, &serv_addr,slen);
     // nu_pkt++; 
     total_Sent++;
     sent++;  
   //printf("\n Bytes Sent %d\n by %d port ", write_size,port);                         
 /* if (write_size < 0) 
      {
         perror("ERROR writing to socket");
         break;
      }*/
            
    gettimeofday(&tv2, NULL);
      num_of_pkt--;
     } 
     
    write_size=sendto(sock,"STOP",4,0, &serv_addr, slen);
    Sen_Loop_time=(double) (tv2.tv_usec - tv1.tv_usec) / 1000000 + (double) (tv2.tv_sec - tv1.tv_sec);
    printf("\n Closing Thread No %d\n",port);
    printf("\n Total UDP Packet Sent by  %d is %d\n",port,sent);
    close(sock);
   // sleep(10);   
     }
      

int main (int argc, char *argv[])
{
        
     
      while (argc > 1)
     {
	if (argv[h][0] == '-')
	  {
		switch (argv[h][1])
		{
  case's':
     server = gethostbyname(argv[h+1]);
     break;

  case'h':
     host = argv[h+1];
     server = gethostbyname(argv[h+1]);
     break;
                
  case'f':
       
       NUM_THREADS=atoi(argv[h+1]);
       break;  

  case'B':
       
       sockbuffer=atoi(argv[h+1]);
       sockbuffer=sockbuffer/2;
       sockbuffer=sockbuffer*1000000;
      // rcvbuf=sndbuf/2;
     //  sndbuf=rcvbuf;
       Buffer_flag=1;
       break;         
                                        
                 
  case'g':
      page= argv[h+1];                      		        
		break;

  case'm':
      mode= atoi(argv[h+1]);                      		        
		break;

 case 'z':
      packet_size=atoi(argv[h+1]);
      seed=packet_size;
      buffer= malloc(sizeof(char) *packet_size);                		        
      break;	
       
 case't':
      Sen_Loop_time=atof(argv[h+1]);               		        
		break;
		
 case'j':
      // dev="ethX"
      //Enable Jumbo Frame for interace ethX           		        
		break;		
        	
 case'q':
       // dev="ethX"
      //Modify Transmission Queue Length for ethX         		        
		break;
 case'p':
      protocol=*(argv[h+1]);	             
	   break;	
	   
 case'd':
      Distribution=atoi(argv[h+1]);
      //0 for constant
      //1 for random
      //2 for incremental
      //3 for exponential	             
	   break;

case'r':
      Proportion=atoi(argv[h+1]);
      //0 for 50/50     tcp/udp
      //1 for 25/75
      //2 for 75/25          
	   break;	

 default:
      printf("Wrong Argument: %s\n", argv[0]);
      break;
      }  }
      h ++;
      argc--;
      }   
            
run();        
return 0; 


}


void run()
{
      if(protocol=='m') 
          {
 			 if (NUM_THREADS<4)
 				{
 					printf("\n Atleast four Thread are required to run\n");   
 					exit(-1);
 				}
 				}


	 init_port();
	 
if (protocol=='h')

 			{  
 			
 			
 			int t,rc,z;
  
    pthread_t tid[NUM_THREADS];
    pthread_attr_t attr;
    void *status;
    for (t=0;t<packet_size;t++)
    buffer[t]='A';
     
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE); 
    
    
 			
         printf("\n Number of HTTP  Thread going to Create %d",NUM_THREADS);
         for (t=0;t<NUM_THREADS;t++)
         
         {
    	 printf("\n Started Thread %d\n",t);
    	 rc=pthread_create(&tid[t], NULL,Send_http_request,NULL);
       if (rc) 
         {
         printf("ERROR; return code from pthread_create() is %d\n", rc);
         exit(-1);
         }
             printf("\n Started %d");
         }    
         
              	
     	pthread_attr_destroy(&attr);
     for(t=0; t<NUM_THREADS; t++)
        {
      rc = pthread_join(tid[t], &status);
      if (rc) {
         printf("ERROR; return code from pthread_join() is %d\n", rc);
         exit(-1);
         }
       }   
     
     printf("Main: completed join with thread %ld having a status of %ld\n",t,(long)status);  
     printf("\n Total Packet Sent is %d\n",total_Sent); 
     printf("Main: program completed. Exiting.\n");
     pthread_exit(NULL);
                
 				return 0;
 				
 			}		 
     
      if (mode==1)
{
	   	
    int t,rc,z;
  
    pthread_t tid[NUM_THREADS];
    pthread_attr_t attr;
    void *status;
    for (t=0;t<packet_size;t++)
    buffer[t]='A';
     
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);    
    
    
switch (protocol) 
       
   {   
      
           
 case 'u':
  		 {		
      printf("\n Number of UDP Thread going to Create %d",NUM_THREADS);
      for (t=0;t<NUM_THREADS;t++)
         {
    	  printf("\n Started Thread %d\n",t);
    	 rc=pthread_create(&tid[t], NULL,Send_udp,(void*)&port[t]);
       if (rc) 
         {
         printf("ERROR; return code from pthread_create() is %d\n", rc);
         exit(-1);
         }
            printf("\n Started Sending for Port No %d",port[t]);
         }  
    		break;
    		}
    		
case 't':
 			{  
         printf("\n Number of TCP Thread going to Create %d",NUM_THREADS);
         for (t=0;t<NUM_THREADS;t++)
         {
    	 printf("\n Started Thread %d\n",t);
    	 rc=pthread_create(&tid[t], NULL,Send_tcp,(void*)&port[t]);
       if (rc) 
         {
         printf("ERROR; return code from pthread_create() is %d\n", rc);
         exit(-1);
         }
             printf("\n Started Sending for Port No %d",port[t]);
         }    
 				break;
 				
 			}	
 			
 	case 'm':
 			 {	
 			 
 				if (NUM_THREADS<2)
 				{
 					printf("\n Atleast Two Thread are required to run\n");   
 					exit(-1);
 				}
 				
 		
   switch(Proportion) 
    {
        case 0:
           	if (NUM_THREADS%2==0)
 		      {
 		 UDP_Thread=NUM_THREADS/2;
       TCP_Thread=NUM_THREADS/2;
 		      }	 	
 		
 		   else 
 		      {
 		 UDP_Thread=NUM_THREADS/2;
       TCP_Thread=NUM_THREADS/2+1;
 		        }	        // 50/50
            break;
            
        case 1:
           if (NUM_THREADS%2==0)
 		      {
 		 UDP_Thread=NUM_THREADS/4;
       TCP_Thread=NUM_THREADS-UDP_Thread;
 		      }	 	
 		
 		   else 
 		      {
 		 UDP_Thread=NUM_THREADS/4;
       TCP_Thread=NUM_THREADS-UDP_Thread+1;
 		        }	        // 25/75				
            break;
                        
        case 2:
            if (NUM_THREADS%2==0)
 		      {
 		 TCP_Thread=NUM_THREADS/4;
       UDP_Thread=NUM_THREADS-TCP_Thread;
 		      }	 	
 		
 		   else 
 		      {
 		 TCP_Thread=NUM_THREADS/4;
       UDP_Thread=NUM_THREADS-TCP_Thread+1;
 		        }	        // 25/75					//75/25
            break;
                      
         default:
           printf("Error! Proportion operator is not correct");
            break;  
     }  		
 		
 		printf("\n Number of TCP Thread going to Create %d",TCP_Thread);  
     for (t=0;t<TCP_Thread;t++)
         {
    	  printf("\n Started Thread %d\n",t);
    	  rc=pthread_create(&tid[t], NULL,Send_tcp,(void*)&port[t]);
    	  if (rc) 
         {
         printf("ERROR; return code from pthread_create() is %d\n", rc);
         exit(-1);
         }
            printf("\n Started Sending for Port No %d",port[t]);
         } 	
    
    sleep(2); 
        
    printf("\n Number of UDP Thread going to Create %d",UDP_Thread);   
    for (t=0;t<UDP_Thread;t++)
         {
    	  printf("\n Started Thread %d\n",TCP_Thread+t);
    	  rc=pthread_create(&tid[t], NULL,Send_udp,(void*)&port[TCP_Thread+t]);
    	  if (rc) 
         {
         printf("ERROR; return code from pthread_create() is %d\n", rc);
         exit(-1);
         }
            printf("\n Started Sending for Port No %d",port[TCP_Thread+t]);
         }  
         
       
         
      
    		break;
    		
    		}	
 			
default:
  		      printf("Wrong Protocol");
  				break;
         }
         
   
     
          	
     	pthread_attr_destroy(&attr);
     for(t=0; t<NUM_THREADS; t++)
        {
      rc = pthread_join(tid[t], &status);
      if (rc) {
         printf("ERROR; return code from pthread_join() is %d\n", rc);
         exit(-1);
         }
       }   
     
     printf("Main: completed join with thread %ld having a status of %ld\n",t,(long)status);  
     printf("\n Total Packet Sent is %d\n",total_Sent); 
     printf("Main: program completed. Exiting.\n");
     pthread_exit(NULL);
    
  
}

else if (mode==0)
{
    
      packet_size=8960;
      buffer= malloc(sizeof(char) *packet_size);  	
	
       int t,rc,z;
       pthread_t tid[NUM_THREADS];
       pthread_attr_t attr;
       void *status;
    
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);     
    
    
    
    switch (protocol) 
        {       	
   
        
case 'u':

  		{		
      printf("\n Number of Thread going to Create %d",NUM_THREADS);
      for (t=0;t<NUM_THREADS;t++)
         {
    	 printf("\n Waiting for for Port No %d",port[t]);
    	 rc=pthread_create(&tid[t], NULL,Receive_udp,(void*)&port[t]);
       if (rc) 
         {
         printf("ERROR; return code from pthread_create() is %d\n", rc);
         exit(-1);
         }
            printf("\n Started Thread %d\n",t);
         }  
    		break;
    		}
    		
case 't':

 		{	 
         printf("\n Number of Thread going to Create %d",NUM_THREADS);
         for (t=0;t<NUM_THREADS;t++)
         {
    	 printf("\n Waiting for for Port No %d",port[t]);
    	 rc=pthread_create(&tid[t], NULL,Receive_tcp,(void*)&port[t]);
       if (rc) 
         {
         printf("ERROR; return code from pthread_create() is %d\n", rc);
         exit(-1);
         }
            printf("\n Started Thread %d\n",t);
         }    
 				break;
 		}
 		
case 'm':
 			 {	
 			 
 			 
 				if (NUM_THREADS<4)
 				{
 					printf("\n Atleast four Thread are required to run\n");   
 					exit(-1);
 				}


   switch(Proportion) 
    {
        case 0:
           	if (NUM_THREADS%2==0)
 		      {
 		 UDP_Thread=NUM_THREADS/2;
       TCP_Thread=NUM_THREADS/2;
 		      }	 	
 		
 		   else 
 		      {
 		 UDP_Thread=NUM_THREADS/2;
       TCP_Thread=NUM_THREADS/2+1;
 		        }	        // 50/50
            break;
            
        case 1:
           if (NUM_THREADS%2==0)
 		      {
 		 UDP_Thread=NUM_THREADS/4;
       TCP_Thread=NUM_THREADS-UDP_Thread;
 		      }	 	
 		
 		   else 
 		      {
 		 UDP_Thread=NUM_THREADS/4;
       TCP_Thread=NUM_THREADS-UDP_Thread+1;
 		        }	        // 25/75				
            break;
                        
        case 2:
            if (NUM_THREADS%2==0)
 		      {
 		 TCP_Thread=NUM_THREADS/4;
       UDP_Thread=NUM_THREADS-TCP_Thread;
 		      }	 	
 		
 		   else 
 		      {
 		 TCP_Thread=NUM_THREADS/4;
       UDP_Thread=NUM_THREADS-TCP_Thread+1;
 		        }	        // 25/75					//75/25
            break;
                      
         default:
           printf("Error! Proportion operator is not correct");
            break;  
     }  		
 		
 		 	
    printf("\n Number of Thread going to Create %d\n",NUM_THREADS); 	
 	 printf("\n Number of TCP Thread going to Create %d",TCP_Thread);  
     for (t=0;t<TCP_Thread;t++)
        {
    	 printf("\n Waiting for for Port No %d",port[t]);
    	 rc=pthread_create(&tid[t], NULL,Receive_tcp,(void*)&port[t]);
       if (rc) 
         {
         printf("ERROR; return code from pthread_create() is %d\n", rc);
         exit(-1);
         }
            printf("\n Started Thread %d\n",t);
         }  
         
          	sleep(2);
 	 
    printf("\n Number of UDP Thread going to Create %d",UDP_Thread);   
   
         for (t=0;t<UDP_Thread;t++)
         {
    	 printf("\n Waiting for for Port No %d",port[TCP_Thread+t]);
    	 rc=pthread_create(&tid[t], NULL,Receive_udp,(void*)&port[TCP_Thread+t]);
       if (rc) 
         {
         printf("ERROR; return code from pthread_create() is %d\n", rc);
         exit(-1);
         }
            printf("\n Started Thread %d\n",TCP_Thread+t);
         }  
          
         
    		break;
    		
    		}			
 				
default:
  		      printf("Wrong Protocol");
  				break;
         }
    
   
   
   
     	pthread_attr_destroy(&attr);
     for(t=0; t<NUM_THREADS; t++) {
      rc = pthread_join(tid[t], &status);
      if (rc)
        {
         printf("ERROR; return code from pthread_join() is %d\n", rc);
         exit(-1);
         }
      printf("Main: completed join with thread %ld having a status of %ld\n",t,(long)status);
      }
     printf("\n Total Packet Received is %d\n",total_Rcv);
     printf("Main: program completed. Exiting.\n");
     pthread_exit(NULL);
        

}
}



