#ifndef _UT_GEN_H
#define _UT_GEN_H



char *get_ip(char *host);
char *build_get_query(char *host, char *page);
void check_protocol();
#define PORT 80
#define USERAGENT "HTMLGET 1.0"


     double Sen_Loop_time;
     int num_of_pkt;
    int h=0;
    static int port[100];
    int mode=0; //Sender=0 and Receiver =1 
      int nu_pkt=0;
    struct hostent *server;
    int packet_size=0;
    int seed;
    char protocol;
   char *buffer;
    int sockbuffer=0;
    int Buffer_flag=0;
    char *host,*page, *ip,*get;
    int total_Sent=0;
    int total_Rcv=0;
    int min_pkt_size=64;
    int max_pkt_size=8950;
    int Distribution=0;
    int Proportion=0;
    int UDP_Thread=0;
    int TCP_Thread=0;


#endif
