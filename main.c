#include <stdio.h>
#include <stdlib.h>
#include "transport_manager.h"
#include "prog2_ex3.h"

#define HANDLE_RESULT(operation)                                                                                              \
    do                                                                                                                        \
    {                                                                                                                         \
        /*fprintf(stdout, "%s():%d [INFO] %s\n", __FUNCTION__, __LINE__, #operation);*/                                           \
        TransportResult res = (operation);                                                                                    \
        if (res != TRANSPORT_SUCCESS)                                                                                         \
        {                                                                                                                     \
            fprintf(stderr, "%s():%d [ERROR] %s failed. %s\n", __func__, __LINE__, #operation, prog2_result_string(res)); \
        }                                                                                                                     \
    } while (0)

void test_1(void)
{
    printf("=== Test 1: Valid Operations ===\n");
    TransportManager tm1 = TransportManagerCreate();
    TransportManager tm2 = TransportManagerCreate();
    TransportManager merged = NULL;

    if (!tm1 || !tm2) {
        fprintf(stderr, "[FATAL] Failed to allocate TransportManagers.\n");
        return;
    }

    // 1. Explicitly add initial companies 
    HANDLE_RESULT(TransportManagerAdd(tm1, 10, "Express Lines", TRANSPORT_BUS, 500000.0, 400000.0));
    HANDLE_RESULT(TransportManagerAdd(tm1, 20, "Oceanic Shipping", TRANSPORT_SHIPPING, 850000.0, 600000.0));
    HANDLE_RESULT(TransportManagerAdd(tm1, 30, "Metro Trains", TRANSPORT_TRAIN, 1200000.0, 900000.0));
    HANDLE_RESULT(TransportManagerAdd(tm1, 40, "Air Connect", TRANSPORT_FLIGHT, 150000.0, 200000.0));
    HANDLE_RESULT(TransportManagerAdd(tm1, 50, "City Cabs", TRANSPORT_TAXI, 95000.0, 50000.0));

    // 2. Bulk additions to hit the volume requirement (100 operations)
    for (int i = 100; i < 200; i++) {
        HANDLE_RESULT(TransportManagerAdd(tm1, i, "Standard Fleet", TRANSPORT_BUS, 10000.0 * i, 8000.0 * i));
    }

    // 3. Bulk updates for existing companies (20 operations)
    for (int i = 150; i < 170; i++) {
        HANDLE_RESULT(TransportManagerUpdateRevenue(tm1, i, 25000.0 * i));
        HANDLE_RESULT(TransportManagerUpdateExpenses(tm1, i, 15000.0 * i));
    }

    // 4. Remove a few valid companies (5 operations)
    for (int i = 195; i < 200; i++) {
        HANDLE_RESULT(TransportManagerRemove(tm1, i));
    }

    // 5. Populate the second manager for merging
    HANDLE_RESULT(TransportManagerAdd(tm2, 60, "Southern Cabs", TRANSPORT_TAXI, 300000.0, 200000.0));
    HANDLE_RESULT(TransportManagerAdd(tm2, 70, "Northern Transport", TRANSPORT_LIGHT_TRAIN, 280000.0, 190000.0));
    HANDLE_RESULT(TransportManagerAdd(tm2, 80, "Eastern Rentals", TRANSPORT_BUS, 320000.0, 210000.0));

    // 6. Merge tm1 and tm2 into a new 'merged' manager
    HANDLE_RESULT(TransportManagerMerge(tm1, tm2, &merged));

    if (merged != NULL) {
        // 7. Test reporting functions (Outputs to a temporary file)
        FILE *out = stdout;
        HANDLE_RESULT(TransportManagerReportTransportCompanies(merged, TRANSPORT_ALL, out));
        HANDLE_RESULT(TransportManagerReportUnprofitableCompanies(merged, out));
        HANDLE_RESULT(TransportManagerReportCompaniesByNetIncome(merged, out));

        // 8. Cleanup
        // tm1 and tm2 are destroyed automatically by TransportManagerMerge upon success.
        TransportManagerDestroy(merged);
    }
}


void test_2(void)
{
    printf("=== Test 2: Error Handling ===\n");
    TransportManager tm1 = TransportManagerCreate();
    TransportManager tm2 = TransportManagerCreate();
    TransportManager merged = NULL;

    if (!tm1 || !tm2) {
        fprintf(stderr, "[FATAL] Failed to allocate TransportManagers.\n");
        return;
    }

    // 1. Setup a valid base of companies (~60 valid operations)
    for (int i = 1; i <= 60; i++) {
        HANDLE_RESULT(TransportManagerAdd(tm1, i, "Valid Co", TRANSPORT_BUS, 1000.0, 500.0));
    }

    // 2. Intentional Errors: Invalid IDs
    HANDLE_RESULT(TransportManagerAdd(tm1, 0, "Zero ID", TRANSPORT_BUS, 100.0, 50.0));
    HANDLE_RESULT(TransportManagerAdd(tm1, -15, "Negative ID", TRANSPORT_BUS, 100.0, 50.0));
    HANDLE_RESULT(TransportManagerRemove(tm1, -5));
    HANDLE_RESULT(TransportManagerUpdateRevenue(tm1, 0, 500.0));

    // 3. Intentional Errors: Invalid Types
    HANDLE_RESULT(TransportManagerAdd(tm1, 101, "Bad Type 1", 0, 100.0, 50.0));
    HANDLE_RESULT(TransportManagerAdd(tm1, 102, "Bad Type 2", 999, 100.0, 50.0));

    // 4. Intentional Errors: Invalid Financials (Negative Revenue/Expenses)
    HANDLE_RESULT(TransportManagerAdd(tm1, 103, "Neg Rev", TRANSPORT_BUS, -10.0, 50.0));
    HANDLE_RESULT(TransportManagerAdd(tm1, 104, "Neg Exp", TRANSPORT_BUS, 100.0, -5.0));
    HANDLE_RESULT(TransportManagerUpdateRevenue(tm1, 1, -500.0));
    HANDLE_RESULT(TransportManagerUpdateExpenses(tm1, 1, -200.0));

    // 5. Intentional Errors: Operating on Non-Existent Entities (20 operations)
    for (int i = 800; i < 810; i++) {
        HANDLE_RESULT(TransportManagerRemove(tm1, i));
        HANDLE_RESULT(TransportManagerUpdateRevenue(tm1, i, 1000.0));
    }

    // 6. Intentional Errors: Duplicate IDs
    HANDLE_RESULT(TransportManagerAdd(tm1, 1, "Duplicate", TRANSPORT_FLIGHT, 500.0, 100.0));
    HANDLE_RESULT(TransportManagerAdd(tm1, 50, "Duplicate2", TRANSPORT_TRAIN, 500.0, 100.0));

    // 7. Intentional Errors: NULL Arguments
    HANDLE_RESULT(TransportManagerAdd(NULL, 200, "Null TM", TRANSPORT_BUS, 100.0, 50.0));
    HANDLE_RESULT(TransportManagerAdd(tm1, 201, NULL, TRANSPORT_BUS, 100.0, 50.0));

    // 8. Intentional Error: Merge Conflict
    // Add ID 1 to tm2 to intentionally cause a duplicate ID conflict during merge
    HANDLE_RESULT(TransportManagerAdd(tm2, 1, "Conflict Co", TRANSPORT_BUS, 1000.0, 500.0));
    HANDLE_RESULT(TransportManagerMerge(tm1, tm2, &merged));

    // 9. Intentional Errors: Reporting
    FILE *out = stdout;
    HANDLE_RESULT(TransportManagerReportTransportCompanies(tm1, 999, out));
    HANDLE_RESULT(TransportManagerReportCompaniesByNetIncome(tm1, out));

    // 10. Intentional Errors: Empty Reports
    TransportManager tm_empty = TransportManagerCreate();
    if (tm_empty) {
        HANDLE_RESULT(TransportManagerReportTransportCompanies(tm_empty, TRANSPORT_ALL, out));
        HANDLE_RESULT(TransportManagerReportUnprofitableCompanies(tm_empty, out));
    }
    
    // 11. Final Cleanup
    if (tm_empty) TransportManagerDestroy(tm_empty);
    
    // Destroy tm1 and tm2 manually because the merge failed and they weren't automatically freed
    TransportManagerDestroy(tm1); 
    TransportManagerDestroy(tm2); 
}

int main(void)
{
    test_1();
    test_2();
    return 0;
}
