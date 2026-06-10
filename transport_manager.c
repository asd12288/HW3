#include "transport_manager.h"
#include "linked_list.h"
#include "set.h"
#include "prog2_ex3.h"
#include <stdlib.h>
#include <string.h>

typedef struct TransportCompany_s
{
    int id;
    char *name;
    TransportType type;
    double revenue;
    double expenses;

} TransportCompany;

struct TransportManager_s
{
    Set companies;
};

static int compareCompaniesById(SetElement elem1, SetElement elem2)
{
    TransportCompany *company1 = elem1;
    TransportCompany *company2 = elem2;

    if (company1->id < company2->id)
    {
        return -1;
    }

    if (company1->id > company2->id)
    {
        return 1;
    }
    return 0;
}

void printCompany(FILE *output, SetElement elem)
{

    TransportCompany *company = (TransportCompany *)elem;
    if (company == NULL)
    {
        return;
    }
    prog2_report_company(company->id, company->name, company->type,
                         company->revenue, company->expenses, output);
}

static int matchCompanyByType(SetElement elem, KeyForSetElement key)
{
    TransportCompany *company = elem;
    TransportType type = *(TransportType *)key;
    return (company->type & type) != 0;
}

static void freeCompany(SetElement elem)
{
    TransportCompany *company = (TransportCompany *)elem;
    if (company != NULL)
    {
        if (company->name != NULL)
        {
            free(company->name);
        }
        free(company);
    }
}

static SetElement copyComapny(SetElement elem)
{

    TransportCompany *company = (TransportCompany *)elem;
    TransportCompany *company_copy = NULL;

    if (company == NULL)
    {
        return NULL;
    }

    company_copy = (TransportCompany *)malloc(sizeof(TransportCompany));
    if (company_copy == NULL)
    {
        return NULL;
    }

    company_copy->name = (char *)malloc(strlen(company->name) + 1);
    if (company_copy->name == NULL)
    {
        free(company_copy);
        return NULL;
    }

    strcpy(company_copy->name, company->name);

    company_copy->expenses = company->expenses;
    company_copy->revenue = company->revenue;
    company_copy->id = company->id;
    company_copy->type = company->type;

    return company_copy;
}

TransportManager TransportManagerCreate(void)
{

    TransportManager new_tm = (TransportManager)malloc(sizeof(struct TransportManager_s));
    if (new_tm == NULL)
    {
        free(new_tm);
        return NULL;
    }

    SetResult result = setCreate(&new_tm->companies, compareCompaniesById, copyComapny, freeCompany, printCompany);

    if (result != SET_SUCCESS)
    {
        free(new_tm);
        return NULL;
    }

    return new_tm;
}

void TransportManagerDestroy(TransportManager tm)
{
    if (tm == NULL)
    {
        return;
    }
    setDestroy(tm->companies);
    free(tm);
}

int typeCheck(TransportType type)
{
    switch (type)
    {
    case 1:
        return 1;
        break;
    case 2:
        return 1;
        break;
    case 4:
        return 1;
        break;
    case 8:
        return 1;
        break;
    case 16:
        return 1;
        break;
    case 32:
        return 1;
        break;
    case 63:
        return 1;
        break;
    default:
        return 0;
        break;
    }
}

TransportResult TransportManagerAdd(TransportManager tm, int id, const char *name,
                                    TransportType type, double revenue, double expenses)
{

    if (id <= 0)
    {
        return TRANSPORT_INVALID_ID;
    }

    if (!typeCheck(type))
    {
        return TRANSPORT_INVALID_TYPE;
    }

    if (revenue < 0)
    {
        return TRANSPORT_INVALID_REVENUE;
    }

    if (expenses < 0)
    {
        return TRANSPORT_INVALID_EXPENSES;
    }

    TransportCompany company;

    company.id = id;
    company.type = type;
    company.revenue = revenue;
    company.expenses = expenses;
    company.name = (char *)name;

    SetResult result = setAdd(tm->companies, &company);

    if (result == SET_ELEMENT_EXISTS)
    {
        return TRANSPORT_ALREADY_EXISTS;
    }

    if (result == SET_OUT_OF_MEMORY)
    {
        return TRANSPORT_OUT_OF_MEMORY;
    }

    return TRANSPORT_SUCCESS;
}

TransportResult TransportManagerReportTransportCompanies(TransportManager tm, TransportType type, FILE *outChannel)
{
    if (tm == NULL || outChannel == NULL)
    {
        return TRANSPORT_NULL_ARGUMENT;
    }

    if (!typeCheck(type))
    {
        return TRANSPORT_INVALID_TYPE;
    }

    if (setGetSize(tm->companies) == 0)
    {
        return TRANSPORT_EMPTY;
    }

    Set filtered;
    if (setFilter(tm->companies, &filtered, &type, matchCompanyByType) != SET_SUCCESS)
    {
        return TRANSPORT_OUT_OF_MEMORY;
    }

    setPrintSorted(filtered, outChannel, setGetSize(filtered), compareCompaniesById);
    setDestroy(filtered);

    return TRANSPORT_SUCCESS;
}

TransportResult TransportManagerReportUnprofitableCompanies(TransportManager tm, FILE *outChannel) {
    /* TODO: not implemented yet */
    return TRANSPORT_SUCCESS;
}