#include "transport_manager.h"
#include "linked_list.h"
#include "set.h"
#include "prog2_ex3.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void)
{

    TransportManager tm = TransportManagerCreate();
    if (tm == NULL)
    {
        fprintf(stderr, "Failed to create TransportManager\n");
        return 1;
    }

    TransportManager tm2 = TransportManagerCreate();
    if (tm2 == NULL)
    {
        fprintf(stderr, "Failed to create TransportManager\n");
        return 1;
    }
    TransportManager merge = NULL;

    TransportManagerAdd(tm, 2, "Israel Railways", TRANSPORT_TRAIN, 500.0, 4200.0);
    TransportManagerAdd(tm, 7, "Egged", TRANSPORT_BUS, 3000.0, 3500.0);
    TransportManagerAdd(tm, 1, "Zim", TRANSPORT_SHIPPING, 9000.0, 6000.0);
    TransportManagerAdd(tm, 3, "Gett", TRANSPORT_TAXI, 1200.0, 900.0);

    TransportManagerAdd(tm2, 5, "El Al", TRANSPORT_FLIGHT, 8000.0, 8000.0);
    TransportManagerAdd(tm2, 4, "Gett", TRANSPORT_TAXI, 1200.0, 900.0);
    TransportManagerMerge(tm, tm2, &merge);

    // TransportManagerReportTransportCompanies(merge, TRANSPORT_ALL, stdout);
    // printf("starting to print merge\n\n");
     TransportManagerReportCompaniesByNetIncome(merge, stdout);
    // printf("finished\n");
    
    //TransportManagerReportUnprofitableCompanies(merge, stdout);
    TransportManagerDestroy(merge);
    return 0;
}