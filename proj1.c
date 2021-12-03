#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

/*gcc -g -Wall -Wextra -Werror -ansi -pedantic proj1.c*/
/*zip proj1.zip *.c*/

/*Prints that will be used during the project*/
#define PRINT_INORDER_IDS "%d %d %s\n"
#define PRINT_ACTIVITIES "%s\n"
#define PRINT_USER "%s\n"
#define PRINT_IDS "%d %s #%d %s\n"
#define ID_NO_SUCH_TASK "%d: no such task\n"
#define PRINT_TIME "%d\n"
#define PRINT_DONE "duration=%d slack=%d\n"
#define PRINT_TASKS "task %d\n"
#define TOO_MANY_TASKS "too many tasks\n"
#define DUPLICATE_DESCRIPTION "duplicate description\n"
#define INVALID_DURATION "invalid duration\n"
#define NO_SUCH_TASK "no such task\n"
#define TASK_ALREADY_STARTED "task already started\n"
#define NO_SUCH_USER "no such user\n"
#define NO_SUCH_ACTIVITY "no such activity\n"
#define INVALID_TIME "invalid time\n"
#define USER_ALREADY_EXISTS "user already exists\n"
#define TOO_MANY_USERS "too many users\n"
#define TOO_MANY_ACTIVITIES "too many activities\n"
#define DUPLICATE_ACTIVITY "duplicate activity\n"
#define INVALID_DESCRIPTION "invalid description\n"

/*MAX constants*/
#define MAXTime 1000000
#define MAX_car 1000000
#define MAXusers 50
#define MAXactivities 10
#define MAXTasks 100000
#define MAXT 10000
#define MAXDescr 101
#define MAXUser 101
#define MAXAtiv 101

/*MACROS used in the algorimts*/
typedef int Item;
#define Key(A) (A)
#define less(A, B) (Key(A) < Key(B))
#define equal(A, B) (Key(A) == Key(B))
#define greater(A, B) (Key(A) > Key(B))
#define exch(A, B)  \
    {               \
        Item t = A; \
        A = B;      \
        B = t;      \
    }

/*Global Variables*/
int auxiliar[MAXTasks];
int auxiliar2[MAXactivities];             /*Array used in merge sort algortim*/
int time = 0;                             /*Current time*/
int users = 0, activities = 3, tasks = 0; /* number of users, activities and tasks*/
long count_spaces = 0;                    /*Variable used to count the number os spaces between arguments in the input*/

/*Structs*/
typedef struct Task /*This struct is used to store the tasks*/
{
    int id;
    char description[MAXDescr];
    char user[MAXUser];
    char activity[MAXAtiv];
    int duration;
    int start;
} Task;

typedef struct Activity /*This struct is used to store the activities*/
{
    char name[MAXAtiv];
} Activity;

typedef struct User /*This struct is used to store the users*/
{
    char name[MAXusers];
} User;

Task task[MAXTasks] = {0};

Activity ativ[MAXactivities] = {0};

User person[MAXusers] = {0};

/*This function is used to skip spaces from the input given by the user*/
void skip_spaces(char string[MAX_car])
{
    while (string[count_spaces] == ' ')
    {
        count_spaces += 1;
    }
}

/* This function sorts the ids, firstly by their start times and if start times are
equal it sorts the ids alphabeticly by their description*/
void merge_start_time(int ids[], int left, int m, int right)
{
    int i, j, k;
    for (i = m + 1; i > left; i--)
    {
        auxiliar[i - 1] = ids[i - 1];
    }
    for (j = m; j < right; j++)
    {
        auxiliar[right + m - j] = ids[j + 1];
    }
    for (k = left; k <= right; k++)
    {
        if less (task[auxiliar[j]].start, task[auxiliar[i]].start)
        {
            ids[k] = auxiliar[j--];
        }
        else if greater (task[auxiliar[j]].start, task[auxiliar[i]].start)
        {
            ids[k] = auxiliar[i++];
        }
        else if equal (task[auxiliar[j]].start, task[auxiliar[i]].start)
        {
            if (strcmp(task[auxiliar[j]].description, task[auxiliar[i]].description) < 0)
            {
                ids[k] = auxiliar[j--];
            }
            else
            {
                ids[k] = auxiliar[i++];
            }
        }
    }
}

void mergesort_start(int ids[], int left, int right)
{
    int m = (right + left) / 2;
    if (right == left)
    {
        return;
    }
    mergesort_start(ids, left, m);
    mergesort_start(ids, m + 1, right);
    merge_start_time(ids, left, m, right);
}

/*Quick Sort - This algortim sorts the ids alphabeticly*/
int partition(int ids[], int left, int right)
{
    int i = left - 1;
    int j = right;
    Item v = ids[right];
    while (i < j)
    {
        while (strcmp(task[ids[++i]].description, task[v].description) < 0)
            ;
        while (strcmp(task[v].description, task[ids[--j]].description) < 0)
        {
            if (j == left)
            {
                break;
            }
        }
        if (i < j)
        {
            exch(ids[i], ids[j]);
        }
    }
    exch(ids[i], ids[right]);
    return i;
}

void quicksort(int ids[], int left, int right)
{
    int i;
    if (right <= left)
    {
        return;
    }
    i = partition(ids, left, right);
    quicksort(ids, left, i - 1);
    quicksort(ids, i + 1, right);
}

/*This function receives a duration and a description and saves them in the 
corresponding task id*/
void save_tasks(int duration, char description[MAXDescr])
{
    printf(PRINT_TASKS, (tasks + 1));
    task[tasks].id = tasks;
    strcpy(task[tasks].description, description);
    strcpy(task[tasks].user, "");
    strcpy(task[tasks].activity, "TO DO");
    task[tasks].duration = duration;
    task[tasks].start = 0;
    tasks += 1;
}

/*This function receives checks for errors related to tasks and then calls
a function which will save all the given parameters in the task structure*/
void write_tasks(int dur, char description[MAXDescr])
{
    int i = 0;
    if (tasks == MAXT)
    {
        printf(TOO_MANY_TASKS);
        return;
    }
    for (i = 0; i < tasks; i += 1)
    {
        if (strcmp(task[i].description, description) == 0)
        {
            printf(DUPLICATE_DESCRIPTION);
            return;
        }
    }
    if (dur <= 0)
    {
        printf(INVALID_DURATION);
        return;
    }
    save_tasks(dur, description);
}

/* This function receives the input from the terminal and separates it into 2
parameters, which are duration and description*/
void read_tasks(char string[MAX_car])
{
    int j = 0, k = 0, dur;
    char duration[MAXTime] = {0}, description[MAXDescr] = {0};
    count_spaces = 1;
    skip_spaces(string);
    for (j = 0; string[count_spaces] != ' '; count_spaces += 1, j += 1)
    {
        duration[j] = string[count_spaces];
    }
    skip_spaces(string);
    for (k = 0; string[count_spaces] != '\0'; count_spaces += 1, k += 1)
    {
        description[k] = string[count_spaces];
    }
    dur = atoi(duration);
    write_tasks(dur, description);
}

/*This function is called when tasks activity is moved to DONE. It will
print the tasks duration and slack*/
void print_activity_times(int id)
{
    int start_time = 0, wasted_time = 0, slack = 0;
    start_time = task[id - 1].start;
    wasted_time = time - start_time;
    slack = wasted_time - task[id - 1].duration;
    printf(PRINT_DONE, wasted_time, slack);
}

/*This function receives an id, a user and an activity and moves the activity and the user
to the task*/
void change_activity(int id, char user[MAXUser], char activity[MAXAtiv])
{
    int j = 0, k = 0, valid_id = 0, valid_user = 0, valid_activity = 0;
    if (id <= tasks && id > 0)
    {
        valid_id = 1;
    }
    for (j = 0; j < users; j += 1)
    {
        if (strcmp(person[j].name, user) == 0)
        {
            valid_user = 1;
        }
    }
    for (k = 0; k < activities; k += 1)
    {
        if (strcmp(ativ[k].name, activity) == 0)
        {
            valid_activity = 1;
        }
    }
    if (valid_id != 1)
    {
        printf(NO_SUCH_TASK);
        return;
    }
    if (strcmp(activity, "TO DO") == 0 && strcmp(task[id - 1].activity, "TO DO") != 0)
    {
        printf(TASK_ALREADY_STARTED);
        return;
    }
    if (strcmp(activity, "DONE") == 0 && strcmp(task[id - 1].activity, "DONE") == 0)
    {
        return;
    }
    if (valid_user != 1)
    {
        printf(NO_SUCH_USER);
        return;
    }
    if (valid_activity != 1)
    {
        printf(NO_SUCH_ACTIVITY);
        return;
    }
    if (strcmp(task[id - 1].activity, "TO DO") == 0)
    {
        task[id - 1].start = time;
    }
    strcpy(task[id - 1].activity, activity);
    strcpy(task[id - 1].user, user);
    if (strcmp(activity, "DONE") == 0)
    {
        print_activity_times(id);
    }
}

/*This function receives the input from the terminal and separates it in different 
parameters, which are id, user and activity*/
void read_m_command(char string[MAX_car])
{
    int j = 0, k = 0, m = 0, identifier = 0;
    char id[MAXTasks] = {0}, user[MAXUser] = {0}, activity[MAXAtiv] = {0};
    count_spaces = 1;
    skip_spaces(string);
    for (j = 0; string[count_spaces] != ' '; count_spaces += 1, j += 1)
    {
        id[j] = string[count_spaces];
    }
    skip_spaces(string);
    for (k = 0; string[count_spaces] != ' '; count_spaces += 1, k += 1)
    {
        user[k] = string[count_spaces];
    }
    skip_spaces(string);
    for (m = 0; string[count_spaces] != '\0'; m += 1, count_spaces += 1)
    {
        activity[m] = string[count_spaces];
    }
    identifier = atoi(id);
    change_activity(identifier, user, activity);
}

/*This function receives a integer, which corresponds to a space of time and
then it increments a global variable with that parameter and prints the current time*/
void time_func(char string[MAXTime])
{
    int j = 0, integer_time;
    char receive_time[MAXTime] = {0};
    count_spaces = 1;
    skip_spaces(string);
    for (j = 0; string[count_spaces] != '\0' && string[count_spaces] != ' '; count_spaces += 1, j += 1)
    {
        receive_time[j] = string[count_spaces];
    }
    integer_time = atoi(receive_time);
    if (integer_time < 0)
    {
        printf(INVALID_TIME);
        return;
    }
    time += integer_time;
    printf(PRINT_TIME, time);
}

/*This function receives the ids given by the user and prints the corresponding
id, duration and description*/
void print_ids(char *tokens[])
{
    int i = 0, valid_id = 0, identifier = 0;
    for (i = 0; tokens[i]; i += 1)
    {
        identifier = atoi(tokens[i]);
        if (identifier <= tasks && identifier > 0)
        {
            valid_id = 1;
        }
        if (valid_id == 0)
        {
            printf(ID_NO_SUCH_TASK, identifier);
        }
        else
        {
            printf(PRINT_IDS, identifier, task[identifier - 1].activity,
                   task[identifier - 1].duration, task[identifier - 1].description);
            valid_id = 0;
        }
    }
}

/*This function creates an array with all the task ids that are in the struct "task"
and calls an algoritm that will sort them alphabetacly*/
void sort_ids()
{
    int i = 0, ids[MAXTasks], j = 0;
    if (tasks == 0)
    {
        return;
    }
    for (i = 0; i < tasks; i += 1)
    {
        ids[i] = task[i].id;
    }
    quicksort(ids, 0, i - 1);
    for (j = 0; j < tasks; j += 1)
    {
        printf(PRINT_IDS, ids[j] + 1, task[ids[j]].activity,
               task[ids[j]].duration, task[ids[j]].description);
    }
}

/*This function receives the input from the terminal and, using strtok, breaks
it into pieces, separated by spaces.
If the first argument is an empty string, then it will get all the task ids and
call an algortim that will organize them.
Otherwise it will call a function that will print the information of all the task ids
given by the user*/
void read_ids(char string[MAX_car], char *tokens[])
{
    int x = 0;
    strtok(string, " ");
    for (x = 0; x < MAXTasks; x += 1)
    {
        tokens[x] = strtok(NULL, " ");
    }
    if (tokens[0])
    {
        print_ids(tokens);
    }
    else
    {
        sort_ids();
    }
}

/*This function receives no arguments or a user.
If it receives a user, then it chekcs if the user already exists in the structure
and if it doesn't, the function will add it.
If it receives no arguments, the function prints all the users contained by the struct "person"*/
void add_users(char user[MAXUser])
{
    int i = 0, j = 0, size = strlen(user);
    for (i = 0; i < users; i += 1)
    {
        if (strcmp(person[i].name, user) == 0 && size != 0)
        {
            printf(USER_ALREADY_EXISTS);
            return;
        }
    }
    if (users == MAXusers && size != 0)
    {
        printf(TOO_MANY_USERS);
        return;
    }
    if (size != 0)
    {
        strcpy(person[users].name, user);
        users += 1;
    }
    else
    {
        for (j = 0; j < users; j += 1)
        {
            printf(PRINT_USER, person[j].name);
        }
    }
}

/*This function receives no arguments or an activity.
If it receives an activity, then it chekcs if the activity already exists in the struct
and if it doesn't, the function will add it.
If it receives no arguments, the function prints all the activities contained by the struct
"ativ*/
void add_activities(char activity[MAXAtiv])
{
    int i = 0, j = 0, size = 0, k = 0;
    size = strlen(activity);
    if (size == 0)
    {
        for (k = 0; k < activities; k += 1)
        {
            printf(PRINT_ACTIVITIES, ativ[k].name);
        }
        return;
    }
    if (activities == MAXactivities)
    {
        printf(TOO_MANY_ACTIVITIES);
        return;
    }
    for (i = 0; i < activities; i += 1)
    {
        if (strcmp(ativ[i].name, activity) == 0)
        {
            printf(DUPLICATE_ACTIVITY);
            return;
        }
    }
    for (j = 0; j != size; j += 1)
    {
        if (islower(activity[j]))
        {
            printf(INVALID_DESCRIPTION);
            return;
        }
    }
    strcpy(ativ[activities].name, activity);
    activities += 1;
}

/*This function receives an activity and creates an array which contains all the tasks
with the given activity.
Then it calls an algoritm to organize the ids*/
void d_command(char activity[MAXAtiv])
{
    int j = 0, count = 0, k = 0;
    int ids[MAXTasks];
    for (j = 0; j < tasks; j += 1)
    {
        if (strcmp(task[j].activity, activity) == 0)
        {
            ids[count] = task[j].id;
            count += 1;
        }
    }
    if (count == 0)
    {
        return;
    }
    mergesort_start(ids, 0, count - 1);
    for (k = 0; k < count; k += 1)
    {
        printf(PRINT_INORDER_IDS, ids[k] + 1, task[ids[k]].start, task[ids[k]].description);
    }
}

/*This function checks to see if the activity provided exists*/
void sort_activities(char activity[MAXAtiv])
{
    int i = 0, valid_activity = 0;
    for (i = 0; i < activities; i += 1)
    {
        if (strcmp(ativ[i].name, activity) == 0)
        {
            valid_activity = 1;
        }
    }
    if (valid_activity != 1)
    {
        printf(NO_SUCH_ACTIVITY);
        return;
    }
    else
    {
        d_command(activity);
    }
}

void funcao_g(char string[MAX_car], char *tokens[])
{
    int x = 0, i = 0, j = 1, a = 0, b = 1, c = 1;
    strtok(string, " ");
    for (x = 0; x < 10; x += 1)
    {
        tokens[x] = strtok(NULL, " ");
    }
    for (i = 0; i < users; i++)
    {
        if (strcmp(person[i].name, tokens[0]) == 0)
        {
            printf("user already exists\n");
            return;
        }
    }
    if (users == MAXusers)
    {
        printf("too many users\n");
    }
    while (tokens[j])
    {
        for (a = 0; a < users; a++)
        {
            if (strcmp(person[a].name, tokens[j]) == 0)
            {
                printf("no such user\n");
                return;
            }
            j++;
        }
    }
    while (tokens[b])
    {
        for (c = 1; tokens[c]; c++)
        {
            if (strcmp(tokens[b], tokens[c]) == 0)
            {
                printf("repeated user\n");
                return;
            }
        }
        b++;
    }
    strcpy(person[users].name, tokens[0]);
    users += 1;
}

/* This function receives the input passed by the user and gets the parameter, which 
can be a user or an activity*/
void read_func(char string[MAX_car], char letter)
{
    char parameter[MAXAtiv] = {0};
    int j = 0;
    count_spaces = 1;
    skip_spaces(string);
    for (j = 0; string[count_spaces] != '\0'; count_spaces += 1, j += 1)
    {
        parameter[j] = string[count_spaces];
    }
    if (letter == 'a')
    {
        add_activities(parameter);
    }
    if (letter == 'd')
    {
        sort_activities(parameter);
    }
    if (letter == 'u')
    {
        add_users(parameter);
    }
}

/*This function receives a letter and calls the appropried command*/
void main_cycle(char letter, char string[], char *tokens[])
{
    if (letter == 't')
    {
        read_tasks(string);
    }
    if (letter == 'l')
    {
        read_ids(string, tokens);
    }
    if (letter == 'n')
    {
        time_func(string);
    }
    if (letter == 'u')
    {
        read_func(string, letter);
    }
    if (letter == 'm')
    {
        read_m_command(string);
    }
    if (letter == 'd')
    {
        read_func(string, letter);
    }
    if (letter == 'a')
    {
        read_func(string, letter);
    }
    if (letter == 'g')
    {
        funcao_g(string, tokens);
    }
}

/* This function gets the input from the terminal and sends it to another function
that will choose the command to use*/

int main()
{
    char letter, string[MAX_car], *tokens[MAXTasks];
    strcpy(ativ[0].name, "TO DO");
    strcpy(ativ[1].name, "IN PROGRESS");
    strcpy(ativ[2].name, "DONE");
    while (scanf(" %[^\n]", string) == 1)
    {
        letter = string[0];
        if (letter == 'q')
        {
            break;
        }
        main_cycle(letter, string, tokens);
    }
    return 0;
}