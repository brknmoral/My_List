#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#define DATA_SIZE 10
#define POISON_VALUE -17

#define MAKEPROBLEM(ret_value, elem_index, next, prev)   {                          \
    printf(RED "ERROR, err = %d, in %s:%d\n" RESET, ret_value, __FILE__, __LINE__); \
    printf("Elem index = <%d>, next = %d; prev = %d ;",elem_index, next, prev);     \
    printf("\n");                                                                   \
    return ret_value;                                                               \
}

#define MAKEPROBLEM_FUNC(ret_value, ...)   {                                        \
    printf(RED "ERROR, err = %d, in %s:%d\n" RESET, ret_value, __FILE__, __LINE__); \
    printf("\n");                                                                   \
    return ret_value;                                                               \
}

#define MAKEPROBLEM_BYPASS(ret_value, ...){                      \
    printf(RED "ERROR, err = %d, in %s:%d\n" RESET, ret_value, __FILE__, __LINE__); \
    printf("\n");                                                                   \
    return ret_value;                                                               \
}

#define SAY_FUNC(   func_name)   {printf("Im in %s\n", func_name);}
#define DUMP_PEREM( name_x, x)   {printf("%s == <%d>\n"), name_x, x;}

#define RED     "\033[1;31m"
#define GREEN   "\033[1;32m"
#define YELLOW  "\033[1;33m"
#define BLUE    "\033[1;34m"
#define PINK    "\033[1;35m"
#define RESET   "\033[0m"

enum List_Errors
{
    LIST_OK               = 0,
    LIST_ERR_NULL_PTR     = EINVAL,
    LIST_ERR_NO_MEM       = ENOMEM,
    LIST_ERR_NO_ELEM      = ENOENT,
    LIST_ERR_FULL         = ENOSPC,
    LIST_ERR_FILE_OPEN    = EIO,
    LIST_ERR_DOT          = -1000,

    LIST_ERR_FICT_ELEM        = -1001,
    LIST_ERR_HALF_FREE        = -1002,
    LIST_ERR_OUT_OF_RANGE     = -1003,
    LIST_ERR_ASYMMETRIC       = -1004,
    LIST_ERR_CYCLE            = -1005,
    LIST_ERR_FWD_BWD          = -1006,   // длина прямого и обратного обхода разная
    LIST_ERR_UNREACHABLE      = -1007    // занятые ячейки, недостижимые из data[0]
};

const char* PREV_COLOR = "#4960c8";
const char* NEXT_COLOR = "#63c59e";

int List_Dump(struct List_Elem* list);
int Insert_New_Elem(struct List_Elem* data, int index_elem, double number);
int Del_Elem(struct List_Elem* data, int index_del_elem);
struct List_Elem* List_Init(void);
int List_Verify(struct List_Elem* list);




//typedef double list_elem;


struct List_Head{
    struct List_Elem* data;
    int capacity;
    int current_size;

    //
};

struct List_Elem{
    double x;
    int next;
    int prev;
};

int main(void)
{
    struct List_Elem* data = (struct List_Elem*)calloc(DATA_SIZE, sizeof(data[0]));

    data[0].x    = -17;
    data[0].next = 0;
    data[0].prev = 0;

    data[1].x = 10;
    data[1].next = 2;
    data[1].prev = 0;

    data[2].x = 20;
    data[2].next = 3;
    data[2].prev = 1;

    data[3].x = 30;
    data[3].next = 4;
    data[3].prev = 2;

    data[4].x = 40;
    data[4].next = 5;
    data[4].prev = 3;

    data[5].x = 50;
    data[5].next = 0;
    data[5].prev = 4;

    data[6].x = 0;
    data[6].next = -1;
    data[6].prev = -1;

    data[7].next = -1;
    data[7].prev = -1;

    data[8].next = -1;
    data[8].prev = -1;

    data[9].next = -1;
    data[9].prev = -1;

    Insert_New_Elem(data, 3, 3.5);
    Insert_New_Elem(data, 3, 3.25);
    Del_Elem(data,2);

    List_Dump(data);
}

int List_Verify(struct List_Elem* list)
{
    int used_elements = 0;
    if (list[0].next != 0 && list[0].prev != 0 && list[0].x != 0) 
    {
        MAKEPROBLEM(LIST_ERR_FICT_ELEM, 0, list[0].next, list[0].prev);
    }
    
    if ((list[0].next < 0 || list[0].next >= DATA_SIZE) || (list[0].prev < 0 || list[0].prev >= DATA_SIZE))
    {
        MAKEPROBLEM(LIST_ERR_FICT_ELEM, 0, list[0].next, list[0].prev);
    }

    for (int index_elem = 0; index_elem < DATA_SIZE; index_elem++)
    {
        int n = list[index_elem].next;
        int p = list[index_elem].prev;
        
        if (n == -1 && p == -1)
        {
            if (index_elem == 0)
            {
                MAKEPROBLEM(LIST_ERR_FICT_ELEM, index_elem, n, p);
            }
            continue;
        }
        if ((n == -1 && p != -1) || (n != -1 && p == -1))
        {
            MAKEPROBLEM(LIST_ERR_HALF_FREE, index_elem, n, p);
        }
        if (n >= DATA_SIZE || n < 0 || p < 0 || p >= DATA_SIZE)
        {
            MAKEPROBLEM(LIST_ERR_OUT_OF_RANGE, index_elem, n, p);
        }
        if ((list[n].prev != index_elem) || (list[p].next != index_elem))
        {
            MAKEPROBLEM(LIST_ERR_ASYMMETRIC, index_elem, n, p);
        }

        if (n != -1 && p != -1) {used_elements++;}
    }
    int fwd = 0;
    int bwd = 0;

    for (int cur = list[0].next; cur != 0; cur = list[cur].next)
    {
        fwd++;
        if (fwd > DATA_SIZE - 1)
        {
            MAKEPROBLEM_BYPASS(LIST_ERR_CYCLE);
        }
    }
    for (int cur = list[0].prev; cur !=0; cur = list[cur].prev)
    {
        bwd++;
        if (bwd > DATA_SIZE - 1)
        {
            MAKEPROBLEM_BYPASS(LIST_ERR_CYCLE);
        }
    }

    if (fwd != bwd)
    {
        MAKEPROBLEM_BYPASS(LIST_ERR_FWD_BWD);
    }
    if (fwd != used_elements)
        MAKEPROBLEM_BYPASS(LIST_ERR_UNREACHABLE);

    return LIST_OK;
}

struct List_Elem* List_Init(void)
{
    struct List_Elem* list = (struct List_Elem*)calloc(DATA_SIZE, sizeof(list[0]));

    if (list == NULL) {MAKEPROBLEM_FUNC((struct List_Elem*)-ENOMEM);}

    list[0].prev = 0;
    list[0].next = 0;
    list[0].x    = POISON_VALUE;

    for (int i = 1; i < DATA_SIZE; i ++)
    {
        list[i].x = 0;
        list[i].prev = -1;
        list[i].next = -1;
    }
    return list;
}

int Insert_New_Elem(struct List_Elem* data, int index_elem, double number)
{
    if (data[index_elem].prev == 0 || data[index_elem].next == 0) {return LIST_ERR_NO_ELEM;}
    int free_elem = 0;
    for (free_elem = 0; free_elem < DATA_SIZE; free_elem++)
    {
        if (data[free_elem].next == -1 && data[free_elem].prev == -1)
        {
            break;
        }
    }
    data[free_elem].x = number;
    data[free_elem].prev = index_elem;
    data[free_elem].next = data[index_elem].next;
    data[data[index_elem].next].prev = free_elem;
    data[index_elem].next = free_elem;
   
    return 0;
}
int Del_Elem(struct List_Elem* data, int index_del_elem)
{
    if (index_del_elem <= 0 && index_del_elem > DATA_SIZE) {return LIST_ERR_NO_ELEM;}

    if (data[index_del_elem].prev == -1 || data[index_del_elem].next == -1) {return LIST_ERR_NO_ELEM;}

    data[data[index_del_elem].prev].next = data[index_del_elem].next;
    data[data[index_del_elem].next].prev = data[index_del_elem].prev;

    data[index_del_elem].x = 0;
    data[index_del_elem].prev = -1;
    data[index_del_elem].next = -1;

    return 0;
}

int List_Dump(struct List_Elem* list)
{
    for (int i = 1; i < DATA_SIZE; i++)
    {
        printf("data[%d] = <%lg>; next = (%d); prev = {%d}\n"
                   , i, list[i].x, list[i].next, list[i].prev);

    }

    FILE* f = fopen("ListDump.dot", "w");
    if (f == NULL){MAKEPROBLEM_FUNC(LIST_ERR_FILE_OPEN);}
    fprintf(f, "digraph structs {\n");
    
    fprintf(f, "    rankdir=LR;\n");
    for (int i = 0; i < DATA_SIZE; i++)
    {
        fprintf(f, "Elem%d [shape=Mrecord,label=\"data[%d] | x = %lg | next = %d | prev = %d}\" ]\n", i, i, list[i].x, list[i].next, list[i].prev);
    }

    fprintf(f, "\n");

    for (int i = 0; i < DATA_SIZE - 1; i++)
    {
        fprintf(f, "Elem%d ->Elem%d\n", i, i+1);
        fprintf(f, "[color=white]\n");
     }

    for (int i = 0; i < DATA_SIZE - 1; i++)
    {
        if (list[i].next != -1)
        {
            fprintf(f, "Elem%d ->Elem%d\n", i, list[i].next);
            fprintf(f, "[color=\"%s\"]\n", NEXT_COLOR);
        }

        if (list[i].prev != -1)
        {
            fprintf(f, "Elem%d ->Elem%d\n", i, list[i].prev);
            fprintf(f, "[color=\"%s\"]\n", PREV_COLOR);
        }
    }

    fprintf(f,"}");
    fclose(f);
    system("dot -Tpng -o ListDump.png ListDump.dot");
    return 0;
}


