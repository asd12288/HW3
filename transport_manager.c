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

static int matchCompanyByType(SetElement elem, KeyForSetElement key)
{
    TransportCompany *company = (TransportCompany *)elem;
    TransportType type = *(TransportType *)key;

    return company->type == type || type == TRANSPORT_ALL;
}

static int matchCompanyById(SetElement elem, KeyForSetElement key)
{
    TransportCompany *company = (TransportCompany *)elem;
    int id = *(int *)key;

    return company->id == id;
}

static int matchCompanyByLost(SetElement elem, KeyForSetElement key)
{
    TransportCompany *company = (TransportCompany *)elem;

    if (company->expenses >= company->revenue)
    {
        return 1;
    }
    return 0;
}

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

static int compareCompaniesByProfit(SetElement elem1, SetElement elem2)
{
    TransportCompany *company1 = elem1;
    TransportCompany *company2 = elem2;

    double profit1 = company1->revenue - company1->expenses;
    double profit2 = company2->revenue - company2->expenses;

    if ((profit1) > (profit2))
    {
        return -1;
    }

    if ((profit1) < (profit2))
    {
        return 1;
    }
    return 0;
}

static void printCompany(FILE *output, SetElement elem)
{
    TransportCompany *company = (TransportCompany *)elem;
    if (company == NULL)
    {
        return;
    }
    prog2_report_company(company->id, company->name, company->type,
                         company->revenue, company->expenses, output);
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

TransportResult TransportManagerRemove(TransportManager tm, int id)
{

    if (tm == NULL)
    {
        return TRANSPORT_NULL_ARGUMENT;
    }

    if (id <= 0)
    {
        return TRANSPORT_INVALID_ID;
    }

    TransportCompany company_to_remove;
    company_to_remove.id = id;

    SetResult result = setRemove(tm->companies, &company_to_remove);

    if (result == SET_ELEMENT_DOES_NOT_EXIST)
    {
        return TRANSPORT_DOESNT_EXIST;
    }

    if (result == SET_BAD_ARGUMENTS)
    {
        return TRANSPORT_NULL_ARGUMENT;
    }

    return TRANSPORT_SUCCESS;
}

TransportResult TransportManagerUpdateRevenue(TransportManager tm, int id, double revenue)
{

    if (tm == NULL)
    {
        return TRANSPORT_NULL_ARGUMENT;
    }

    if (id <= 0)
    {
        return TRANSPORT_INVALID_ID;
    }

    if (revenue < 0)
    {
        return TRANSPORT_INVALID_REVENUE;
    }

    SetElement found_element = NULL;

    SetResult result = setFind(tm->companies, &found_element, &id, matchCompanyById);

    if (result == SET_ELEMENT_DOES_NOT_EXIST)
    {
        return TRANSPORT_DOESNT_EXIST;
    }

    TransportCompany *company = (TransportCompany *)found_element;

    company->revenue = revenue;

    return TRANSPORT_SUCCESS;
}

TransportResult TransportManagerUpdateExpenses(TransportManager tm, int id,
                                               double expenses)
{
    if (id <= 0)
    {
        return TRANSPORT_INVALID_ID;
    }

    if (expenses < 0)
    {
        return TRANSPORT_INVALID_EXPENSES;
    }

    SetElement element_to_found = NULL;

    SetResult result = setFind(tm->companies, &element_to_found, &id, matchCompanyById);

    if (result == SET_ELEMENT_DOES_NOT_EXIST)
    {
        return TRANSPORT_DOESNT_EXIST;
    }

    if (result == SET_BAD_ARGUMENTS)
    {
        return TRANSPORT_NULL_ARGUMENT;
    }

    TransportCompany *company = (TransportCompany *)element_to_found;
    company->expenses = expenses;

    return TRANSPORT_SUCCESS;
}

TransportResult TransportManagerMerge(TransportManager tm1, TransportManager tm2,
                                      TransportManager *merged)
{

    Set intersection = NULL;
    setIntersection(tm1->companies, tm2->companies, &intersection);

    int n = setGetSize(intersection);
    setDestroy(intersection);
    if (n != 0)
    {
        return TRANSPORT_ALREADY_EXISTS;
    }

    TransportManager temp_tm = TransportManagerCreate();
    if (temp_tm == NULL)
    {
        return TRANSPORT_OUT_OF_MEMORY;
    }

    setDestroy(temp_tm->companies);

    SetResult result = setUnion(tm1->companies, tm2->companies, &temp_tm->companies);

    if (result == SET_OUT_OF_MEMORY)
    {
        TransportManagerDestroy(temp_tm);
        return TRANSPORT_OUT_OF_MEMORY;
    }
    if (result == SET_BAD_ARGUMENTS)
    {
        TransportManagerDestroy(temp_tm);
        return TRANSPORT_NULL_ARGUMENT;
    }

    *merged = temp_tm;

    return TRANSPORT_SUCCESS;
}

TransportResult TransportManagerReportTransportCompanies(TransportManager tm, TransportType type, FILE *outChannel)
{
    if (tm == NULL)
    {
        return TRANSPORT_NULL_ARGUMENT;
    }
    
    Set filtered = NULL;

    SetResult result;

    result = setFilter(tm->companies, &filtered, &type, matchCompanyByType);
    if (setGetSize(filtered) == 0)
    {
        setDestroy(filtered);
        return TRANSPORT_EMPTY;
    }

    if (result == SET_BAD_ARGUMENTS)
    {
        setDestroy(filtered);
        return TRANSPORT_NULL_ARGUMENT;
    }

    result = setPrintSorted(filtered, outChannel, setGetSize(filtered), compareCompaniesById);
    setDestroy(filtered);

    return TRANSPORT_SUCCESS;
}

TransportResult TransportManagerReportUnprofitableCompanies(TransportManager tm, FILE *outChannel)
{

    Set filtered = NULL;

    SetResult result;

    result = setFilter(tm->companies, &filtered, NULL, matchCompanyByLost);
    if (setGetSize(filtered) == 0)
    {
        setDestroy(filtered);
        return TRANSPORT_EMPTY;
    }

    if (result == SET_BAD_ARGUMENTS)
    {
        setDestroy(filtered);
        return TRANSPORT_NULL_ARGUMENT;
    }

    result = setPrintSorted(filtered, outChannel, setGetSize(filtered), compareCompaniesById);
    if (result == SET_BAD_ARGUMENTS)
    {
        setDestroy(filtered);
        return TRANSPORT_NULL_ARGUMENT;
    }
    setDestroy(filtered);

    return TRANSPORT_SUCCESS;
}

TransportResult TransportManagerReportCompaniesByNetIncome(TransportManager tm, FILE *outChannel)
{

    if (setGetSize(tm->companies) == 0)
    {
        return TRANSPORT_EMPTY;
    }

    setPrintSorted(tm->companies, outChannel, setGetSize(tm->companies), compareCompaniesByProfit);

    return TRANSPORT_SUCCESS;
}
