#include <stdio.h>
#include <string.h>

typedef struct
{
    int id;
    char title[100];
    int completed; // 0 / 1
} Task;

void print_task(Task task)
{
    printf("id: %d | title: %s | completed: %d\n", task.id, task.title, task.completed);
}

int get_new_id(FILE *fptr)
{
    fptr = fopen("tasks.txt", "r");

    char buffer[256];

    int id;

    while (fgets(buffer, sizeof(buffer), fptr) != NULL)
    {
        sscanf(buffer, "%d", &id);
    }
    fclose(fptr);

    if (id == 0)
    {
        return 0;
    }

    return id + 1;
}

int main()
{
    char command;

    FILE *fptr;

    scanf(" %c", &command);

    if (command == 'n')
    {
        char title[100];

        printf("enter task title: ");
        scanf("%99s", title);

        fptr = fopen("tasks.txt", "a");

        if (!fptr)
        {
            perror("adding new task:");
            return 1;
        }

        int new_id = get_new_id(fptr);

        fprintf(fptr, "%d %s 0\n", new_id, title);

        fclose(fptr);
        return 0;
    }

    printf("unsupported command!\n");

    return 0;
}