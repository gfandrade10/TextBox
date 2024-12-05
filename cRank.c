#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct filtered_events_impl {
    int event_count;
    char** events_list;

    void (*get_event_count)(const char* events, struct filtered_events_impl* obj);
    void (*cleanup)(struct filtered_events_impl* obj);
} filtered_events_t;

void get_event_count_impl_(const char* events, filtered_events_t* obj)
{
    int counter = 0;
    int len = (int)strlen(events);
    char copy[len+1];
    strcpy(copy, events);

    for(int i = 0; i < len; ++i)
    {
        if(events[i] == ' ') counter++;
    }
    
    counter++;
    obj->event_count = counter;
    obj->events_list = (char**)malloc(counter * sizeof(char*));

    char* p = strtok(copy, " ");
    int pos = 0;
    while(p != NULL)
    {
        int tmp = (int)strlen(p);
        obj->events_list[pos] = (char*)malloc((tmp + 1) * sizeof(char));
        snprintf(obj->events_list[pos], tmp + 1, "%s", p);
        pos++;
        p = strtok(NULL, " ");
    }
}

void cleanup_impl_(filtered_events_t* obj)
{
    for(int i = 0; i < obj->event_count; ++i)
    {
        free(obj->events_list[i]);
    }
    free(obj->events_list);
}

#define START_EVENT_OBJ(OBJ) do {                   \
    OBJ.get_event_count = get_event_count_impl_;    \
    OBJ.cleanup = cleanup_impl_;                    \
} while(0);

int champion_rank (int champion_ID, const char *events)
{
    //initial empty check
    if(strlen(events) == 0)
        return champion_ID;

    //object creation settings
    filtered_events_t events_obj;
    START_EVENT_OBJ(events_obj);

    //object startup
    events_obj.get_event_count(events, &events_obj);

    //operations
    int curr_pos = champion_ID;
    printf("%i: ", champion_ID);
    for(int i = 0; i < events_obj.event_count; ++i)
    {
        printf("%s ", events_obj.events_list[i]);
        if(strcmp(events_obj.events_list[i], "I") == 0)
        {
            int temp = atoi(events_obj.events_list[i-1]);
            if(temp < curr_pos)
                curr_pos--;

            else if(temp == champion_ID)
                return -1;
        }

        else if(strcmp(events_obj.events_list[i], "O") == 0)
        {
            int temp = atoi(events_obj.events_list[i-1]);
            if(temp == curr_pos + 1 && temp != champion_ID)
                curr_pos++;

            if(temp == champion_ID)
                curr_pos--;
        }
    }
    puts("");
    events_obj.cleanup(&events_obj);
    return curr_pos;
}

int main(int argc, char *argv[])
{
    printf("%i\n", champion_rank(10, "1 I 10 O 2 I"));
    return 0;
}
