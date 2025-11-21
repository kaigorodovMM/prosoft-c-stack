#include "cstack.h"
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#define UNUSED(VAR) (void)(VAR)

struct node
{
    const struct node* prev;
    unsigned int size;
    char data[0];
};

typedef struct node* stack_t;

struct stack_entry
{
    int reserved;
    stack_t stack;
};

typedef struct stack_entry stack_entry_t;

struct stack_entries_table
{
    unsigned int size;
    stack_entry_t* entries;
};
struct stack_entries_table g_table = { 0u, NULL};

hstack_t stack_new(void)
{
    if (g_table.entries == NULL) 
    {
        g_table.entries = malloc(10 * sizeof(stack_entry_t));
        if (g_table.entries == NULL) { //проверка malloc
            return -1;
        }
        for (size_t i = 0; i < 10; ++i) {
            g_table.entries[i].reserved = -1;
            g_table.entries[i].stack = NULL;
        }
    }
    if (g_table.size == 10) 
    {
        return -1;
    }
    else 
    {
        for (hstack_t i = 0; i < 10; ++i)
        {
            if (g_table.entries[i].reserved == -1)
            {
                g_table.entries[i].reserved = 0;
                g_table.entries[i].stack = NULL;
                g_table.size++;
                return i;
            }
        }
    }
    return -1;
}

void stack_free(const hstack_t hstack)
{
    UNUSED(hstack);
    if (stack_valid_handler(hstack) == 0) {
        while (g_table.entries[hstack].reserved > 0) {
            stack_t del = g_table.entries[hstack].stack;
            g_table.entries[hstack].stack = (stack_t)del->prev;
            g_table.entries[hstack].reserved--;
            free(del);
        }
        g_table.entries[hstack].stack = NULL;
        g_table.entries[hstack].reserved = -1;
        g_table.size--;
    }
}

int stack_valid_handler(const hstack_t hstack)
{
    UNUSED(hstack);
    if (g_table.entries != NULL && hstack >= 0 && hstack < 10 && g_table.entries[hstack].reserved != -1) {
        return 0;
    }
    return 1;
}

unsigned int stack_size(const hstack_t hstack)
{
    UNUSED(hstack);
    if (stack_valid_handler(hstack) == 0) {
        return (unsigned int)g_table.entries[hstack].reserved;
    }
    return 0u;
}

void stack_push(const hstack_t hstack, const void* data_in, const unsigned int size)
{
    UNUSED(hstack);
    UNUSED(data_in);
    UNUSED(size);
    if (stack_valid_handler(hstack) == 0 && data_in != NULL && size != 0) {
        
        stack_t new_st = malloc(sizeof(struct node) + size);
        if (new_st != NULL) { // поверка malloc
            //g_table.entries[hstack].reserved += 1;
            new_st->size = size;
            memcpy(new_st->data, (void* )data_in, size);
            new_st->prev = g_table.entries[hstack].stack;
            g_table.entries[hstack].reserved++;
            g_table.entries[hstack].stack = new_st;
        }
        
    }
}

unsigned int stack_pop(const hstack_t hstack, void* data_out, const unsigned int size)
{
    UNUSED(hstack);
    UNUSED(data_out);
    UNUSED(size);
    if (stack_valid_handler(hstack) == 0 && data_out != NULL && size != 0 && size == g_table.entries[hstack].stack->size) {
        memcpy(data_out, g_table.entries[hstack].stack->data, size);
        stack_t del = g_table.entries[hstack].stack;
        g_table.entries[hstack].stack = (stack_t)g_table.entries[hstack].stack->prev;
        g_table.entries[hstack].reserved--;
        free(del);
        return size;
    }
    return 0;
}

