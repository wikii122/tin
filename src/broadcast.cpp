#include <stdio.h>
#include <net/if.h>
#include <netinet/in.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netdb.h>
#include <unistd.h>
#include <iostream>
#include <stdlib.h>
#include <cstring>

#include "helloPacket.h"


#define INT_TO_ADDR(_addr) \
(_addr & 0xFF), \
(_addr >> 8 & 0xFF), \
(_addr >> 16 & 0xFF), \
(_addr >> 24 & 0xFF)

#define MAXBUF 65536

sockaddr getBroadcastAddress()
{
    struct ifconf ifc;
    struct ifreq ifr[10];
    int sd, ifc_num, bcast, i;

    //Create a socket so we can use ioctl on the file
     //descriptor to retrieve the interface info.


    sd = socket(PF_INET, SOCK_DGRAM, 0);
    if (sd > 0)
    {
        ifc.ifc_len = sizeof(ifr);
        ifc.ifc_ifcu.ifcu_buf = (caddr_t)ifr;

        if (ioctl(sd, SIOCGIFCONF, &ifc) == 0)
        {
            ifc_num = ifc.ifc_len / sizeof(struct ifreq);
            //printf("%d interfaces found\n", ifc_num);

            for (i = 0; i < ifc_num; ++i)
            {
                if (ifr[i].ifr_addr.sa_family != AF_INET)
                {
                    continue;
                }

                //display the interface name
                //printf("%d) interface: %s\n", i+1, ifr[i].ifr_name);

                if(ioctl(sd,SIOCGIFFLAGS,(char*) ifr)<0){
                	printf("Kijowo");
                    break;
                }

                if ((ifr->ifr_flags & IFF_UP) == 0 ||
                   	    (ifr->ifr_flags & IFF_LOOPBACK) ||
                   	    (ifr->ifr_flags &
                   	    (IFF_BROADCAST | IFF_POINTOPOINT)) == 0){
                	continue;
                }

                if (ioctl(sd, SIOCGIFBRDADDR, &ifr[i]) == 0)
                {
                    bcast = ((struct sockaddr_in *)(&ifr[i].ifr_broadaddr))->sin_addr.s_addr;
                    printf("%d) broadcast: %d.%d.%d.%d\n", i+1, INT_TO_ADDR(bcast));
                    close(sd);
                    return ifr[i].ifr_broadaddr;
                }
            }
        }
        perror("Broadcast not available");
        close(sd);
        exit(1);

    }

}

void sendBroadcast(int port, char* buffer, size_t buflen){
	int sock, status, sinlen;
	  struct sockaddr_in sock_in;
	  int yes = 1;

	  sinlen = sizeof(struct sockaddr_in);
	  memset(&sock_in, 0, sinlen);

	  sock = socket (PF_INET, SOCK_DGRAM, IPPROTO_UDP);

	  sock_in.sin_addr.s_addr = htonl(INADDR_ANY);
	  sock_in.sin_port = htons(0);
	  sock_in.sin_family = PF_INET;

	  if(bind(sock, (struct sockaddr *)&sock_in, sinlen)==-1){
		  perror("Bind");
	  }

	  setsockopt(sock, SOL_SOCKET, SO_BROADCAST, &yes, sizeof(int) );

	  /* -1 = 255.255.255.255 this is a BROADCAST address,
	     a local broadcast address could also be used.
	     you can comput the local broadcat using NIC address and its NETMASK
	  */

	  sock_in.sin_addr.s_addr=htonl(-1); /* send message to 255.255.255.255 */
	  sock_in.sin_port = htons(port); /* port number */
	  sock_in.sin_family = PF_INET;

	  if(sendto(sock, buffer, buflen, 0, (struct sockaddr *)&sock_in, sinlen)=-1){
		  perror("Sendto");
	  }

	  shutdown(sock, 2);
	  close(sock);
}

void listenBroadcast(){
	int sock, status, buflen;
  	unsigned sinlen;
  	char buffer[MAXBUF];
	struct sockaddr_in sock_in;
	int yes = 1;

	sinlen = sizeof(struct sockaddr_in);
	memset(&sock_in, 0, sinlen);

	sock = socket (PF_INET, SOCK_DGRAM, IPPROTO_UDP);

	sock_in.sin_addr.s_addr = htonl(INADDR_ANY);
  	sock_in.sin_port = htons(0);
  	sock_in.sin_family = PF_INET;

	if(bind(sock, (struct sockaddr *)&sock_in, sinlen)==-1){
		perror("Bind");
	}

	if(getsockname(sock, (struct sockaddr *)&sock_in, &sinlen)==-1){
		perror("getsockname");
	}

	buflen = MAXBUF;
	memset(buffer, 0, buflen);
	status = recvfrom(sock, buffer, buflen, 0, (struct sockaddr *)&sock_in, &sinlen);
	printf("sendto Status = %d\n", status);

	shutdown(sock, 2);
	close(sock);
}


