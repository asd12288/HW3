/******************************************************************************
 * pointers_demo.c  —  one file that demonstrates EVERYTHING about pointers
 *
 * Sections (read them in order, each builds on the previous one):
 *   01  boxes and addresses        (&  and  *)
 *   02  the two meanings of *      (declaration vs action)
 *   03  writing through a pointer
 *   04  NULL - the "points at nothing" value
 *   05  structs and the dot        ( . )
 *   06  struct pointers, the arrow ( -> )
 *   07  stack vs heap, malloc/free, DEEP COPY (this is copyCompany!)
 *   08  out-parameters             (f(&x) = "fill my box", and Thing **)
 *   09  void* - the unlabeled envelope, and casting
 *   10  what a cast REALLY is      (looking at raw bytes)
 *   11  function pointers          (storing "what to do" in a variable)
 *   12  THE GENERIC PATTERN        (void* + callbacks = your homework)
 *
 * Compile:  gcc -Wall -Werror -o pointers_demo pointers_demo.c
 * Run:      ./pointers_demo
 *
 * This file is for LEARNING ONLY - it is not part of HW3, do not submit it.
 * Best way to use it: change values, recompile, predict the output before
 * running. Being wrong and finding out why is the whole point.
 ******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void banner(const char *title)
{
    printf("\n==================== %s ====================\n", title);
}

/******************************************************************************
 * 01 - BOXES AND ADDRESSES
 *
 * Every variable is a box in memory. Every box has an address (its "house
 * number"). A pointer is just a box whose CONTENT is another box's address.
 *
 *   x   = what is inside box x
 *   &x  = the ADDRESS of box x        (& = "address of")
 *   p   = what is inside box p        (an address!)
 *   *p  = follow the address in p     (* = "go there")
 ******************************************************************************/
static void section01_boxes_and_addresses(void)
{
    int x = 7;
    int *p = &x; /* p now stores the address of x */

    banner("01 boxes and addresses");
    printf("x      = %d        (the value in box x)\n", x);
    printf("&x     = %p  (the address of box x)\n", (void *)&x);
    printf("p      = %p  (p stores that same address)\n", (void *)p);
    printf("*p     = %d        (follow p's arrow -> we find 7)\n", *p);
    printf("&p     = %p  (p is itself a box, with its own address)\n",
           (void *)&p);

    /* & and * are opposites: *(&x) is just x again */
    printf("*(&x)  = %d        (address-of then follow = back to x)\n", *(&x));
}

/******************************************************************************
 * 02 - THE TWO MEANINGS OF *
 *
 * The same symbol does two different jobs:
 *
 *   int *p;       <- in a DECLARATION: "p is a pointer to int" (part of type)
 *   y = *p;       <- in an EXPRESSION:  "follow the pointer"   (an action)
 *
 * How to tell them apart: if there is a type name in front (int, char,
 * TransportCompany...), the * is part of the type. Otherwise it's an action.
 ******************************************************************************/
static void section02_two_meanings_of_star(void)
{
    int x = 10;
    int *p = &x;  /* star #1: declares that p is a pointer            */
    int y = *p;   /* star #2: action - go read what p points at (10)  */

    banner("02 the two meanings of *");
    printf("int *p = &x;   the * here only DESCRIBES p's type\n");
    printf("int  y = *p;   the * here ACTS: follow the arrow, y = %d\n", y);
}

/******************************************************************************
 * 03 - WRITING THROUGH A POINTER
 *
 * *p is not read-only. It names the box at the other end of the arrow,
 * so you can also ASSIGN into it. This is how one function can modify
 * another function's variable (see section 08).
 ******************************************************************************/
static void section03_writing_through_a_pointer(void)
{
    int x = 5;
    int *p = &x;

    banner("03 writing through a pointer");
    printf("before: x = %d\n", x);
    *p = 99; /* go to the box p points at, and overwrite it */
    printf("after *p = 99:  x = %d   (we never touched 'x' by name!)\n", x);
}

/******************************************************************************
 * 04 - NULL
 *
 * NULL is the special address "points at nothing". Following a NULL
 * pointer (*p when p is NULL) CRASHES the program. That is why every
 * function in your homework starts with NULL checks - it's the only
 * defense. Rule: a pointer is either valid or NULL, never "uninitialized".
 ******************************************************************************/
static void section04_null(void)
{
    int *p = NULL;

    banner("04 NULL");
    printf("p = %p   (the 'nowhere' address)\n", (void *)p);
    if (p == NULL)
    {
        printf("p is NULL, so we must NOT do *p  (it would crash)\n");
    }
    /* This is exactly TransportManagerAdd's first line:
       if (tm == NULL) return TRANSPORT_NULL_ARGUMENT;                */
}

/******************************************************************************
 * 05 - STRUCTS AND THE DOT
 *
 * A struct is one big box made of smaller named boxes (fields), packed
 * together in memory. When you HOLD the struct itself, you reach a field
 * with the dot:   person.age
 ******************************************************************************/
typedef struct Person_s
{
    char *name; /* a POINTER to text (the text lives elsewhere)  */
    int age;
} Person;

static void section05_structs_and_dot(void)
{
    Person tmp;            /* a real struct, on the stack, right here */
    tmp.name = "Ilan";     /* dot: I hold the struct -> direct access */
    tmp.age = 22;

    banner("05 structs and the dot");
    printf("tmp.name = %s, tmp.age = %d\n", tmp.name, tmp.age);
    printf("the whole struct is %lu bytes, starting at %p\n",
           (unsigned long)sizeof(tmp), (void *)&tmp);
    printf("field 'name' is at %p, field 'age' is at %p\n",
           (void *)&tmp.name, (void *)&tmp.age);
    /* notice: the fields' addresses are right next to each other -
       a struct really is just its fields glued together in memory  */
}

/******************************************************************************
 * 06 - STRUCT POINTERS AND THE ARROW
 *
 * When you hold only the ADDRESS of a struct, you must follow the arrow
 * first, then take the field:
 *
 *   (*p).age     "follow p, then field age"  - correct but ugly
 *   p->age       the exact same thing        - the shortcut everyone uses
 *
 * Rule:  hold the struct -> dot.   Hold its address -> arrow.
 * You never choose freely; the DECLARATION decides for you.
 ******************************************************************************/
static void section06_struct_pointers_and_arrow(void)
{
    Person tmp;
    Person *p = &tmp; /* p is just an address; the struct is tmp */

    banner("06 struct pointers and the arrow");
    p->age = 30;            /* same box as tmp.age! */
    (*p).name = "Dana";     /* the long form of p->name = ...   */

    printf("via the struct : tmp.age = %d, tmp.name = %s\n",
           tmp.age, tmp.name);
    printf("via the pointer: p->age  = %d, p->name  = %s\n",
           p->age, p->name);
    printf("p->age and tmp.age are the SAME box: %p == %p\n",
           (void *)&p->age, (void *)&tmp.age);
}

/******************************************************************************
 * 07 - STACK vs HEAP, AND DEEP COPY  (this section IS copyCompany)
 *
 * Stack: variables die automatically when the function returns.
 * Heap:  malloc'd memory lives until YOU call free. You own it.
 *
 * Deep copy: a Person holds a POINTER to its name. Copying just the
 * struct would copy the pointer - both copies would share ONE name
 * (and freeing both would free that name twice = crash). A deep copy
 * also clones what the pointers point at. Compare with copyCompany
 * in transport_manager.c - it is the same function with other fields.
 ******************************************************************************/
static Person *personCreate(const char *name, int age)
{
    Person *p = malloc(sizeof(Person)); /* box for the struct (heap) */
    if (p == NULL)
    {
        return NULL;
    }
    p->name = malloc(strlen(name) + 1); /* separate box for the text */
    if (p->name == NULL)
    {
        free(p); /* don't leak the half-built person */
        return NULL;
    }
    strcpy(p->name, name);
    p->age = age;
    return p;
}

static void personDestroy(Person *p)
{
    if (p != NULL)
    {
        free(p->name); /* free the inner box FIRST...           */
        free(p);       /* ...then the struct that pointed to it */
    }
}

static Person *personDeepCopy(const Person *src)
{
    if (src == NULL)
    {
        return NULL;
    }
    return personCreate(src->name, src->age); /* clones struct AND name */
}

static void section07_heap_and_deep_copy(void)
{
    Person *original = personCreate("Zim", 80);
    Person *clone = personDeepCopy(original);

    banner("07 stack vs heap + deep copy");
    if (original == NULL || clone == NULL)
    {
        printf("out of memory\n");
        personDestroy(original);
        personDestroy(clone);
        return;
    }

    printf("original: name='%s' at %p, struct at %p\n",
           original->name, (void *)original->name, (void *)original);
    printf("clone   : name='%s' at %p, struct at %p\n",
           clone->name, (void *)clone->name, (void *)clone);
    printf("same TEXT, different ADDRESSES -> truly independent copies\n");

    original->name[0] = 'X'; /* vandalize the original... */
    printf("after changing original: original='%s', clone='%s'\n",
           original->name, clone->name);

    personDestroy(original); /* each copy is freed exactly once */
    personDestroy(clone);

    /* DANGER (do not run, just understand): a SHALLOW copy
     *     Person bad = *original;          // copies the name POINTER
     * would share one name between two structs. personDestroy(both)
     * would then free the same text twice -> crash. That's the whole
     * reason copyCompany mallocs a new name.                          */
}

/******************************************************************************
 * 08 - OUT-PARAMETERS:  f(&x) means "here is my box, fill it in"
 *
 * A function's return value is often busy carrying an error code
 * (like SetResult / TransportResult). So results travel back through
 * pointers the CALLER provides.
 *
 * And if the result is itself a pointer (a Person*), the caller must
 * pass the address of their pointer -> a pointer to a pointer (Person**).
 * That's why setCreate takes  Set*  : your Set variable is a pointer,
 * and setCreate needs its address to fill it.
 ******************************************************************************/
typedef enum
{
    DEMO_SUCCESS,
    DEMO_OUT_OF_MEMORY
} DemoResult;

static void fillNumber(int *out) /* gets the ADDRESS of caller's int */
{
    *out = 42; /* write through it -> caller's variable changes */
}

static DemoResult personCreateSafe(Person **out, const char *name, int age)
{
    *out = personCreate(name, age); /* fill the caller's POINTER box */
    if (*out == NULL)
    {
        return DEMO_OUT_OF_MEMORY;
    }
    return DEMO_SUCCESS; /* return value stays free for the error code */
}

static void section08_out_parameters(void)
{
    int n = 0;
    Person *who = NULL;
    DemoResult res;

    banner("08 out-parameters (& = fill my box)");
    fillNumber(&n);
    printf("after fillNumber(&n):  n = %d\n", n);

    res = personCreateSafe(&who, "Egged", 90); /* note the & on a pointer! */
    printf("personCreateSafe filled 'who': %s, age %d (result code %d)\n",
           who->name, who->age, (int)res);
    personDestroy(who);
}

/******************************************************************************
 * 09 - void*  THE UNLABELED ENVELOPE, AND CASTING
 *
 * A normal pointer = address + type ("an int lives there").
 * A void*          = address only. The type label was peeled off.
 *
 * You CANNOT open a void* (*v is a compile error - how many bytes?
 * interpreted how?). To open it you must put the label back: a CAST.
 *
 *   void *v = &n;          pack   (any pointer fits, no cast needed)
 *   (int *)v               label  (same address, now typed)
 *   *(int *)v              open   (read the int)
 *
 * The cast is a PROMISE, not a check. Lie, and you get garbage.
 ******************************************************************************/
static void section09_void_pointer_and_cast(void)
{
    int n = 7;
    double d = 3.14;
    void *envelope;

    banner("09 void* + casting");

    envelope = &n; /* pack an int's address */
    printf("packed   &n  : envelope = %p\n", envelope);
    printf("unpacked     : *(int *)envelope    = %d\n", *(int *)envelope);

    envelope = &d; /* the SAME void* can carry any type */
    printf("packed   &d  : envelope = %p\n", envelope);
    printf("unpacked     : *(double *)envelope = %.2f\n",
           *(double *)envelope);

    /* This is exactly your homework's key trick:
     *   caller:    setFind(set, &found, &id, matchCompanyById);  <- packs
     *   callback:  int id = *(int *)key;                         <- unpacks
     * The pair must agree on the type. The compiler cannot check it.  */
}

/******************************************************************************
 * 10 - WHAT A CAST REALLY IS: just different glasses, same bytes
 *
 * A cast never moves or converts memory. It only changes how the
 * compiler INTERPRETS the bytes at that address. Proof: let's look at
 * the raw bytes of an int through 'unsigned char*' glasses (reading
 * bytes via char* is always legal).
 ******************************************************************************/
static void section10_bytes_view(void)
{
    int x = 7;
    unsigned char *bytes = (unsigned char *)&x; /* byte-glasses on x */
    unsigned long i;

    banner("10 a cast = different glasses, same bytes");
    printf("int x = 7 occupies %lu bytes at %p:  ",
           (unsigned long)sizeof(int), (void *)&x);
    for (i = 0; i < sizeof(int); i++)
    {
        printf("%02x ", bytes[i]);
    }
    printf("\n(the 07 byte plus padding zeros - the memory did not change,\n");
    printf(" only the TYPE we used to look at it changed)\n");
}

/******************************************************************************
 * 11 - FUNCTION POINTERS: storing "what to do" in a variable
 *
 * Functions live in memory too, so they have addresses. A function
 * pointer stores such an address; calling through it runs the function.
 *
 *   int (*op)(int, int);    "op points to a function (int,int)->int"
 *
 * This is how setCreate works: you hand it the addresses of YOUR
 * compare/copy/free/print functions, it stores them, and calls them later.
 ******************************************************************************/
static int add(int a, int b) { return a + b; }
static int mul(int a, int b) { return a * b; }

static void section11_function_pointers(void)
{
    int (*op)(int, int); /* a box that holds a FUNCTION's address */

    banner("11 function pointers");
    op = add; /* a function's name is its address */
    printf("op = add:  op(3, 4) = %d\n", op(3, 4));
    op = mul;
    printf("op = mul:  op(3, 4) = %d\n", op(3, 4));
    /* same call site, different behavior - decided by DATA at runtime.
       That is the entire magic behind the generic Set.                */
}

/******************************************************************************
 * 12 - THE FULL GENERIC PATTERN  (void* + function pointers together)
 *
 * A generic function works on elements it knows NOTHING about:
 *  - elements travel as void*            (section 09)
 *  - behavior arrives as function ptrs   (section 11)
 *
 * Below: ONE sort function + ONE print function that work for ints AND
 * for Persons, never knowing either type exists. This is set.h in
 * miniature: compare genericSort's 'cmp' to setCreate's cmpSetElemFunc.
 ******************************************************************************/
typedef void *Element;                          /* like SetElement      */
typedef int (*CompareFunc)(Element, Element);   /* like cmpSetElemFunc  */
typedef void (*PrintFunc)(Element);             /* like printSetElemFunc */

static void genericSort(Element arr[], int count, CompareFunc cmp)
{
    int i, j;
    for (i = 0; i < count - 1; i++)
    {
        for (j = 0; j < count - 1 - i; j++)
        {
            if (cmp(arr[j], arr[j + 1]) > 0) /* calls back into YOUR code */
            {
                Element t = arr[j]; /* we swap the void* arrows, */
                arr[j] = arr[j + 1]; /* not the data they point at */
                arr[j + 1] = t;
            }
        }
    }
}

static void genericPrintAll(Element arr[], int count, PrintFunc print)
{
    int i;
    for (i = 0; i < count; i++)
    {
        print(arr[i]); /* the generic code has no idea what it prints */
    }
    printf("\n");
}

/* --- the "personality" for ints (like your company callbacks) --- */
static int compareInts(Element a, Element b)
{
    return *(int *)a - *(int *)b; /* unpack both envelopes, compare */
}
static void printInt(Element e)
{
    printf("%d ", *(int *)e);
}

/* --- the "personality" for Persons --- */
static int comparePersonsByAge(Element a, Element b)
{
    Person *p1 = a; /* void* converts to Person* implicitly */
    Person *p2 = b;
    return p1->age - p2->age;
}
static void printPerson(Element e)
{
    Person *p = e;
    printf("[%s, %d] ", p->name, p->age);
}

static void section12_the_generic_pattern(void)
{
    int v0 = 30, v1 = 10, v2 = 20;
    Element numbers[3];
    Person *people[3];
    Element persons[3];
    int i;

    banner("12 the generic pattern (= your homework)");

    numbers[0] = &v0; /* pack each int's address into a void* */
    numbers[1] = &v1;
    numbers[2] = &v2;
    genericSort(numbers, 3, compareInts);
    printf("sorted ints   : ");
    genericPrintAll(numbers, 3, printInt);

    people[0] = personCreate("Zim", 80);
    people[1] = personCreate("Gett", 12);
    people[2] = personCreate("ElAl", 75);
    for (i = 0; i < 3; i++)
    {
        persons[i] = people[i]; /* Person* -> void*, no cast needed */
    }
    genericSort(persons, 3, comparePersonsByAge);
    printf("sorted persons: ");
    genericPrintAll(persons, 3, printPerson);

    for (i = 0; i < 3; i++)
    {
        personDestroy(people[i]); /* we own them -> we free them */
    }

    /* Map this to set.h:
     *   Element            <-> SetElement (void*)
     *   compareInts        <-> compareCompaniesById
     *   printPerson        <-> printCompany
     *   genericSort's cmp  <-> the cmpFunc you gave setCreate
     * The Set adds one more idea on top: it COPIES elements in
     * (copyCompany) and frees them on destroy (freeCompany).          */
}

int main(void)
{
    section01_boxes_and_addresses();
    section02_two_meanings_of_star();
    section03_writing_through_a_pointer();
    section04_null();
    section05_structs_and_dot();
    section06_struct_pointers_and_arrow();
    section07_heap_and_deep_copy();
    section08_out_parameters();
    section09_void_pointer_and_cast();
    section10_bytes_view();
    section11_function_pointers();
    section12_the_generic_pattern();
    printf("\nDone. Now change something, predict the output, recompile!\n");
    return 0;
}
