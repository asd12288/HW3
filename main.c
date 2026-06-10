#include "transport_manager.h"
#include "linked_list.h"
#include "set.h"
#include "prog2_ex3.h"
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

    return 0;
}