#ifndef __PROG2_EX3_H__
#define __PROG2_EX3_H__

typedef enum {FALSE, TRUE} Bool;

 /* Result type and values for Cbitbank module */
typedef enum {
    TRANSPORT_SUCCESS,
    TRANSPORT_OUT_OF_MEMORY,
    TRANSPORT_NULL_ARGUMENT,
    TRANSPORT_INVALID_ID,
    TRANSPORT_INVALID_TYPE,
    TRANSPORT_INVALID_REVENUE,
    TRANSPORT_INVALID_EXPENSES,
    TRANSPORT_ALREADY_EXISTS,
    TRANSPORT_DOESNT_EXIST,
    TRANSPORT_EMPTY
} TransportResult;

/* Cbitbank branches types */
typedef enum {
    TRANSPORT_SHIPPING = 1,
    TRANSPORT_TRAIN = 2,
    TRANSPORT_LIGHT_TRAIN = 4,
    TRANSPORT_BUS = 8,
    TRANSPORT_FLIGHT = 16,
    TRANSPORT_TAXI = 32,
    TRANSPORT_ALL = 63,
} TransportType;

void prog2_report_company(int id, const char *name, TransportType type,
    double revenue, double expenses, FILE* outChannel);

const char * prog2_result_string(TransportResult res);

#endif