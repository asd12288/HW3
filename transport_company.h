#ifndef __TRANSPORT_COMPANY_H__
#define __TRANSPORT_COMPANY_H__

#include <stdio.h>
#include "prog2_ex3.h"
#include "set.h"

/******************************************************************************/
/*
 * File: transport_company.h
 * Description: Header file for the TransportCompany module.
 *              Holds the company data structure and all the company-level
 *              logic (copy/free/print, comparisons, matchers and type check)
 *              used by the TransportManager module through the Set ADT.
 */
/******************************************************************************/

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

/* Set ADT callbacks - matchers (matchSetElemFunc) */
int matchCompanyByType(SetElement elem, KeyForSetElement key);
int matchCompanyById(SetElement elem, KeyForSetElement key);
int matchCompanyByLost(SetElement elem, KeyForSetElement key);

/* Set ADT callbacks - comparators (cmpSetElemFunc) */
int compareCompaniesById(SetElement elem1, SetElement elem2);
int compareCompaniesByProfit(SetElement elem1, SetElement elem2);

/* Set ADT callbacks - element management */
void printCompany(FILE *output, SetElement elem);
void freeCompany(SetElement elem);
SetElement copyCompany(SetElement elem);

#endif /* __TRANSPORT_COMPANY_H__ */
