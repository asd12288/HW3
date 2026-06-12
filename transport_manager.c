#include "transport_manager.h"
#include "transport_company.h"
#include "set.h"
#include <stdlib.h>

struct TransportManager_s
{
    Set companies;
};

TransportManager TransportManagerCreate(void)
{

    TransportManager new_tm = (TransportManager)malloc(sizeof(struct TransportManager_s));
    if (new_tm == NULL)
    {
        free(new_tm);
        return NULL;
    }

    SetResult result = setCreate(&new_tm->companies, compareCompaniesById, copyCompany, freeCompany, printCompany);

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

TransportResult TransportManagerAdd(TransportManager tm, int id, const char *name,
                                    TransportType type, double revenue, double expenses)
{
    if (tm == NULL || name == NULL)
    {
        return TRANSPORT_NULL_ARGUMENT;
    }

    if (id <= 0)
    {
        return TRANSPORT_INVALID_ID;
    }

    if (!typeCheck(type) || type == TRANSPORT_ALL)
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
    if (tm == NULL)
    {
        return TRANSPORT_NULL_ARGUMENT;
    }

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
    if (tm1 == NULL || tm2 == NULL || merged == NULL)
    {
        return TRANSPORT_NULL_ARGUMENT;
    }

    Set intersection = NULL;
    SetResult inter_result = setIntersection(tm1->companies, tm2->companies, &intersection);

    if (inter_result == SET_OUT_OF_MEMORY)
    {
        setDestroy(intersection);
        return TRANSPORT_OUT_OF_MEMORY;
    }

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

    Set union_set = NULL;
    SetResult result = setUnion(tm1->companies, tm2->companies, &union_set);

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

    setDestroy(temp_tm->companies);
    temp_tm->companies = union_set;

    TransportManagerDestroy(tm1);
    TransportManagerDestroy(tm2);

    *merged = temp_tm;

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

    Set filtered = NULL;

    SetResult result;

    result = setFilter(tm->companies, &filtered, &type, matchCompanyByType);
    if (result == SET_OUT_OF_MEMORY)
    {
        setDestroy(filtered);
        return TRANSPORT_OUT_OF_MEMORY;
    }

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
    if (tm == NULL || outChannel == NULL)
    {
        return TRANSPORT_NULL_ARGUMENT;
    }

    Set filtered = NULL;

    SetResult result;

    result = setFilter(tm->companies, &filtered, NULL, matchCompanyByLost);
    if (result == SET_OUT_OF_MEMORY)
    {
        setDestroy(filtered);
        return TRANSPORT_OUT_OF_MEMORY;
    }

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
    if (tm == NULL || outChannel == NULL)
    {
        return TRANSPORT_NULL_ARGUMENT;
    }

    if (setGetSize(tm->companies) == 0)
    {
        return TRANSPORT_EMPTY;
    }

    setPrintSorted(tm->companies, outChannel, setGetSize(tm->companies), compareCompaniesByProfit);

    return TRANSPORT_SUCCESS;
}
