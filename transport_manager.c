#include "transport_manager.h"
#include "linked_list.h"
#include "set.h"
#include "prog2_ex3.h"
#include <stdio.h> 
#include <stdlib.h>
#include <string.h>


typedef struct TransportManager_s{
    
    const char *name;
    int id;
    TransportType type;
    double revenue;
    double expenses;

} *Transport;

Bool cmpSetElemFunc(){

    return 0;
}


TransportManager TransportManagerCreate(void){

    return 0;

}

TransportResult TransportManagerAdd(TransportManager tm, 
const char *name, int id, TransportType type, double revenue, 
double expenses){

    return 0;

}

