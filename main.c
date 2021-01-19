#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "header/NTP_client.h"

typedef struct timespce timespce;

int main(int argc, char *argv[]) {
    int n = atoi(argv[1]);
    for (int i = 2; i<argc; i++){
        for (int j=0; j<n ;j++){
            ntp_client(argv[i], j);
            if (j != n)
                sleep(8);
        }
    }
    return 0;
}
