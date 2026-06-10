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

    SetResult res = TransportManagerAdd(tm, 2, "ilam", 1, 123, 123);

    printf("%d", res);
    return 0;
}