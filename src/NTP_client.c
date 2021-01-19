#include "../header/NTP_client.h"
#include <stdio.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <netdb.h>
#include <string.h>
#include <time.h>

/*
 *  don't del commented function
 */


// to test first Byte
//void printBits(unsigned char c){
//    for (int i = 0; i < 8; ++i, c >>= 1) {
//        if (c & 0x1) {
//            printf("BIT %d = ",i);
//            printf("1\n");
//
//        } else {
//            printf("BIT %d = ",i);
//            printf("0\n");
//        }
//    }
//}
//
//unsigned char setBit(unsigned char data,unsigned int pos)
//{
//    return (data|(1 << pos));
//}

//struct timespec find_min(struct timespec * disp){
//    struct timespec min = disp[0];
//    for(int i=1; i<sizeof(disp); i++) {
//        if (disp[i].tv_sec < min.tv_sec) {
//            min = disp[i];
//        }
//        if (disp[i].tv_sec == min.tv_sec ) {
//            if (disp[i].tv_nsec < min.tv_nsec && disp[i].tv_nsec != 0)
//                min = disp[i];
//        }
//    }
//    return min;
//}
//struct timespec find_max(struct timespec * disp){
//    struct timespec max = disp[0];
//    for(int i=1; i<sizeof(disp); i++) {
//        if (disp[i].tv_sec > max.tv_sec) {
//            max = disp[i];
//        }
//        if (disp[i].tv_sec == max.tv_sec ) {
//            if (disp[i].tv_nsec > max.tv_nsec)
//                max = disp[i];
//        }
//    }
//    return max;
//}


void foo(struct ntp_packet packet, int counter, struct timespec t1, struct timespec t4, char *host_name) {

    struct timespec t2, t3;
    packet.Reference_Timestamp_1= ntohl( packet.Reference_Timestamp_1);
    packet.Reference_Timestamp_2= ntohl( packet.Reference_Timestamp_2);
    packet.Origin_Timestamp_1= ntohl( packet.Origin_Timestamp_1);
    packet.Origin_Timestamp_2= ntohl( packet.Origin_Timestamp_2);
    packet.Receive_Timestamp_1= ntohl( packet.Receive_Timestamp_1);
    packet.Receive_Timestamp_2= ntohl( packet.Receive_Timestamp_2);
    packet.Transmit_Timestamp_1 = ntohl( packet.Transmit_Timestamp_1 );
    packet.Transmit_Timestamp_2 = ntohl( packet.Transmit_Timestamp_2 );
    packet.rootDispersion = ntohl(packet.rootDispersion);
    t2.tv_sec = packet.Receive_Timestamp_1 - 2208988800;
    t2.tv_nsec = packet.Receive_Timestamp_2;
    t3.tv_sec = packet.Transmit_Timestamp_1 - 2208988800;
    t3.tv_nsec = packet.Transmit_Timestamp_2;

    struct timespec delay, offset;
    delay.tv_sec = (t4.tv_sec - t1.tv_sec)-(t3.tv_sec - t2.tv_sec);
    delay.tv_nsec = (t4.tv_nsec - t1.tv_nsec) - (t3.tv_nsec - t2.tv_nsec);
    delay.tv_nsec = delay.tv_nsec;

    offset.tv_sec = ((t2.tv_sec - t1.tv_sec) + (t3.tv_sec-t4.tv_sec))/2.0;
    offset.tv_nsec = ((t2.tv_nsec - t1.tv_nsec) + (t3.tv_nsec-t4.tv_nsec))/2.0;

    printf("{%s};{%d};{%d};{disp};{%ld.%ld};{%ld.%ld}\n", host_name, counter, packet.rootDispersion,
            delay.tv_sec, delay.tv_nsec, offset.tv_sec , offset.tv_nsec);
}


int ntp_client(char *host_name, int i) {
    struct timespec t1, t4;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    ntp_packet packet = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    memset(&packet, 0, sizeof(ntp_packet));
    // Set the first byte of Packet_to_send to 00,011,011 for li = 0, vn = 3, and mode = 3
    *((char*)&packet + 0) = 0x23;
    int sock = socket( AF_INET, SOCK_DGRAM, IPPROTO_UDP );
    if (sock < 0)
        perror("[+] error while opening socket");
    server = gethostbyname(host_name);
    if (server == NULL)
        perror("[+] no such Host");
    bzero((char*)&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char*)server->h_addr,( char* ) &serv_addr.sin_addr.s_addr, server->h_length );
    serv_addr.sin_port = htons(PORT);
    if (connect(sock,(struct sockaddr *)&serv_addr, sizeof( serv_addr)) < 0)
        perror("[+] Erorr while conecting to the Server");

    int n = write(sock, (char*)&packet, sizeof(ntp_packet));
    clock_gettime(CLOCK_REALTIME, &t1);
    if (n < 0)
        perror("[+] error while send the packet to server");

    n = read(sock, (char*)&packet, sizeof(ntp_packet));
    clock_gettime(CLOCK_REALTIME, &t4);
    if (n < 0)
        perror("[+] error while receive the packet from server");
    foo(packet, i, t1, t4, host_name);

    close(sock);
    return 0;
}