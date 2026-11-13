#include <stdio.h>
#include <stdlib.h>

#define DATA_SIZE 10

const char* PREV_COLOR = "#4960c8";
const char* NEXT_COLOR = "#63c59e";

int List_Dump(struct List_Elem* list);
int Insert_New_Elem(struct List_Elem* data, int index_elem, double number);

//typedef double list_elem;


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

    List_Dump(data);
}

int List_Init(struct List_Elem* data)
{
    return -1;
}

int Insert_New_Elem(struct List_Elem* data, int index_elem, double number)
{
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

int List_Dump(struct List_Elem* list)
{
    for (int i = 1; i < DATA_SIZE; i++)
    {
        printf("data[%d] = <%lg>; next = (%d); prev = {%d}\n"
                   , i, list[i].x, list[i].next, list[i].prev);

    }

    FILE* f = fopen("ListDump.dot", "w");
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


