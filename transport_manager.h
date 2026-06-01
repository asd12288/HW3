#ifndef __TRANSPORT_MANAGER_H__
#define __TRANSPORT_MANAGER_H__

#include <stdio.h>
#include "prog2_ex3.h"


/******************************************************************************/
/*                                                                            
 * File: transport_manager.h
 * Description: Header file for the TransportManager module
 * Author: David Avdat 
 * Copyright (C) 2026 David Avdat
 */
/*******************************************************************************/

/*
 * An ADT module interface for managing transportation companies.
 *
 * Below you'll find the functions you need to implement for this module.
 * Each function has a description of what it should do and what its input and output are.
 * 
 * You should not change the function prototypes or the defined types.
 * 
 * *** NOTE! ***
 * Each function that returns TransportResult may return one of the following in addition to the description:
 * TRANSPORT_SUCCESS, TRANSPORT_OUT_OF_MEMORY or TRANSPORT_NULL_ARGUMENT
 * 
 */

/* The ADT main structure type definition */
typedef struct TransportManager_s *TransportManager;

/*
 * Function: TransportManagerCreate
 * Parameters:
 *           None
 * Returns:
 *           A new TransportManager instance
 * Description:
 *           Creates a new TransportMaanger instance.
 */
TransportManager TransportManagerCreate(void);
/******************************************************************************/

/*
 * Function: TransportManagerDestroy
 * Parameters:
 *           tm - a pointer to the TransportManager instance to destroy
 * Returns:
 *           None
 * Description:
 *           Destroys the given TransportManager instance and frees all of its allocations
 */
void TransportManagerDestroy(TransportManager tm);
/******************************************************************************/

/*
 * Function: TransportManagerAdd
 * Parameters:
 *           tm - a pointer to the TransportManager instance
 *           id - the unique ID of the transport company to add
 *           name - the name of the company
 *           type - the type of the transportation company (defined in prog2_ex3.h)
 *           revenue - the annual revenue of the company
 *           expenses - the annual expenses of the company
 * Returns:
 *           TRANSPORT_INVALID_ID if the ID is invalid (negative or zero).
 *           TRANSPORT_INVALID_TYPE if the type is invalid.
 *           TRANSPORT_INVALID_REVENUE if the revenue is negative.
 *           TRANSPORT_INVALID_EXPENSES if the expenses are negative.
 *           TRANSPORT_ALREADY_EXISTS if a company with the same ID already exists.
 * Description:
 *           Adds a new transportation company to the given TransportManager instance.
 */
TransportResult TransportManagerAdd(TransportManager tm, int id, const char *name,
    TransportType type, double revenue, double expenses);
/******************************************************************************/


/*
 * Function: TransportManagerRemove
 * Parameters:
 *           tm - a pointer to the TransportManager instance
 *           id - the ID of the transportation company to remove
 * Returns:
 *           TRANSPORT_INVALID_ID if the ID is invalid (negative or zero).
 *           TRANSPORT_DOESNT_EXIST if a company with the given ID does not exist.
 * Description:
 *           Removes a transportation company from the given TransportManager instance and free
 *           all of its allocations
 */
TransportResult TransportManagerRemove(TransportManager tm, int id);
/******************************************************************************/


/*
 * Function: TransportManagerUpdateRevenue
 * Parameters:
 *           tm - a pointer to the TransportManager instance
 *           id - the ID of the transportation company to get
 *           revenue - the new revenue of the transportation company
 * Returns:
 *           TRANSPORT_INVALID_ID if the ID is invalid (negative or zero).
 *           TRANSPORT_DOESNT_EXIST if a company with the given ID does not exist.
 *           TRANSPORT_INVALID_REVENUE if the revenue is negative.
 * Description:
 *          Updates the revenue of a transportation company in the given TransportManager instance.
 */
TransportResult TransportManagerUpdateRevenue(TransportManager tm, int id, double revenue);
/******************************************************************************/


/*
 * Function: TransportManagerUpdateExpenses
 * Parameters:
 *           tm - a pointer to the TransportManager instance
 *           id - the ID of the transportation company to get
 *          expenses - the new expenses of the transportation
 * Returns:
 *           TRANSPORT_INVALID_ID if the ID is invalid (negative or zero).
 *           TRANSPORT_DOESNT_EXIST if a company with the given ID does not exist.
 *           TRANSPORT_INVALID_EXPENSES if the expenses are negative.
 * Description:
 *          Updates the expenses of a transportation company in the given TransportManager instance.
 */
TransportResult TransportManagerUpdateExpenses(TransportManager tm, int id, double expenses);
/******************************************************************************/


/*
 * Function: TransportManagerMerge
 * Parameters:
 *           tm1 - a pointer to the first TransportManager instance
 *           tm2 - a pointer to the second TransportManager instance
 *           merged - [out] a pointer to the merged TransportManager instance
 * Returns:
 *           TRANSPORT_ALREADY_EXISTS if a company with the same ID exists
 *           in both tm1 and tm2.
 * Description:
 *           Merges two TransportManager instances into a new one.
 *           The merged instance will contain all companies from both tm.
 * 
 * ** NOTE! **
 * The merged instance will be created using the TransportManagerCreate function.
 * The caller is responsible for destroying the merged instance using
 * TransportManagerDestroy.
 * In case this function fails (e.g., returned TRANSPORT_ALREADY_EXISTS) both tm1 and tm2
 *  remains as they were. Otherwise, on success, both are destroyed.
 */
 TransportResult TransportManagerMerge(TransportManager tm1, TransportManager tm2, /* OUT */ TransportManager *merged);
 /******************************************************************************/


/*
 * Function: TransportManagerReportTransportCompanies
 * Parameters:
 *           tm - a pointer to the TransportManager instance
 *           type - the type of the transportation companies to report (can be TRANSPORT_ALL)
 * 
 * Returns:
 *           TRANSPORT_INVALID_TYPE if the type is invalid.
 *           TRANSPORT_EMPTY if there are no companies in the tm.
 * Description:
 *          Reports all companies in the tm from the requested type in an id ascending order
 */
TransportResult TransportManagerReportTransportCompanies(TransportManager tm, TransportType type, FILE *outChannel);
/******************************************************************************/

/*
 * Function: TransportManagerReportUnprofitableCompanies
 * Parameters:
 *           tm - a pointer to the TransportManager instance
 *           outChannel - the output channel to write the report to
 * 
 * Returns:
 *           TRANSPORT_EMPTY if there are no companies in the tm.
 * Description:
 *           Reports all the transportation companies in which the revenue <= expenses
 */
TransportResult TransportManagerReportUnprofitableCompanies(TransportManager tm, FILE *outChannel);
/******************************************************************************/


/*
 * Function: TransportManagerReportCompaniesByIncome
 * Parameters:
 *           tm - a pointer to the TransportManager instance
 *           outChannel - the output channel to write the report to
 * 
 * Returns:
 *           TRANSPORT_EMPTY if there are no companies in the tm.
 * Description:
 *          Reports all the compannies int the tm ordered in an ascending order by net income (revenue - expenses)
 */
TransportResult TransportManagerReportCompaniesByNetIncome(TransportManager tm, FILE *outChannel);
/******************************************************************************/


#endif /* __TRANSPORT_MANAGER_H__ */