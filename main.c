#include "transport_manager.h"
#include "linked_list.h"
#include "set.h"
#include "prog2_ex3.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int main()
{

    TransportManager tm = TransportManagerCreate();
    if (tm == NULL)
    {
        fprintf(stderr, "Failed to create TransportManager\n");
        return 1;
    }

    TransportManagerAdd(tm, 2, "Israel Railways", TRANSPORT_TRAIN, 5000.0, 4200.0);
    TransportManagerAdd(tm, 7, "Egged", TRANSPORT_BUS, 3000.0, 3500.0);
    TransportManagerAdd(tm, 1, "Zim", TRANSPORT_SHIPPING, 9000.0, 6000.0);
    TransportManagerAdd(tm, 5, "El Al", TRANSPORT_FLIGHT, 8000.0, 8000.0);
    TransportManagerAdd(tm, 3, "Gett", TRANSPORT_TAXI, 1200.0, 900.0);

    TransportManagerReportTransportCompanies(tm, TRANSPORT_ALL, stdout);
    printf("----------------------------------------\n");
    TransportManagerRemove(tm, 7);
    TransportManagerRemove(tm, 3);
    TransportManagerRemove(tm, 3);
    TransportManagerReportTransportCompanies(tm, TRANSPORT_ALL, stdout);

    TransportManagerDestroy(tm);
    return 0;
}