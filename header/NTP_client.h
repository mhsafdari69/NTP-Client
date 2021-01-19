#include <stdint.h>
#ifndef WOCHE_5_NTP_CLIENT_H

#define WOCHE_5_NTP_CLIENT_H
#define PORT 123


typedef struct ntp_packet ntp_packet;
struct ntp_packet{
    uint8_t li_vn_mode,stratum,poll,precision;

    uint32_t rootDelay;
    uint32_t rootDispersion;
    uint32_t refId;
    uint32_t Reference_Timestamp_1;
    uint32_t Reference_Timestamp_2;
    uint32_t Origin_Timestamp_1;
    uint32_t Origin_Timestamp_2;
    uint32_t Receive_Timestamp_1;
    uint32_t Receive_Timestamp_2;
    uint32_t Transmit_Timestamp_1;
    uint32_t Transmit_Timestamp_2;
};

void printBits(unsigned char c);
unsigned char setBit(unsigned char data,unsigned int pos);
int ntp_client(char *host_name, int i);

#endif //WOCHE_5_NTP_CLIENT_H
