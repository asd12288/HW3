#ifndef __TRANSPORT_COMPANY_H__
#define __TRANSPORT_COMPANY_H__

#include <stdio.h>
#include "prog2_ex3.h"
#include "set.h"



typedef struct TransportCompany_s
{
    int id;
    char *name;
    TransportType type;
    double revenue;
    double expenses;

} TransportCompany;

/* Returns 1 if the given type is a valid TransportType, 0 otherwise */
int typeCheck(TransportType type);

// matchers
int matchCompanyByType(SetElement elem, KeyForSetElement key);
int matchCompanyById(SetElement elem, KeyForSetElement key);
int matchCompanyByLost(SetElement elem, KeyForSetElement key);

// comparaters
int compareCompaniesById(SetElement elem1, SetElement elem2);
int compareCompaniesByProfit(SetElement elem1, SetElement elem2);

// setcreate funcs
void printCompany(FILE *output, SetElement elem);
void freeCompany(SetElement elem);
SetElement copyCompany(SetElement elem);

#endif