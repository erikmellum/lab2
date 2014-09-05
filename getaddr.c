/*
** showip.c -- show IP addresses for a host given on the command line
*/

#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <ifaddrs.h>
#include <netinet/ether.h>

int main(int argc, char *argv[])
{
	struct ifaddrs *res, *p;
	int status;
	char ipstr[INET6_ADDRSTRLEN];


	if ((status = getifaddrs(&res)) != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
		return 2;
	}

	for(p = res;p != NULL; p = p->ifa_next) {
		void *addr;
		char *ipver;

		// get the pointer to the address itself,
		// different fields in IPv4 and IPv6:
		if (p->ifa_addr->sa_family == AF_INET) { // IPv4
			struct sockaddr_in *ipv4 = (struct sockaddr_in *)p->ifa_addr;
			addr = &(ipv4->sin_addr);
			ipver = "AF_INET";
		        inet_ntop(p->ifa_addr->sa_family, addr, ipstr, sizeof ipstr);
		} else if(p->ifa_addr->sa_family == AF_PACKET){
			struct sockaddr_in *ipv4 = (struct sockaddr_in *)p->ifa_addr;
			addr = &(ipv4->sin_addr);
			ipver = "AF_PACKET";
		        //ipstr=ether_ntoa(p->ifa_addr);
		        ipstr=ether_ntoa(p->ifa_addr);
		} 
		else { // IPv6
			struct sockaddr_in6 *ipv6 = (struct sockaddr_in6 *)p->ifa_addr;
			addr = &(ipv6->sin6_addr);
			ipver = "AF_INET6";
		        inet_ntop(p->ifa_addr->sa_family, addr, ipstr, sizeof ipstr);
		}

		// convert the IP to a string and print it:
		if(p->ifa_addr->sa_family == AF_PACKET){
			printf("%5s   %12s  %30s\n", p->ifa_name, ipver, ipstr);
  		} else{
			printf("%5s   %12s  %30s\n", p->ifa_name, ipver, ipstr);
		}
	}

	freeifaddrs(res); // free the linked list

	return 0;
}
