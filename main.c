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

        if (fptr == NULL)
        {
            perror("adding new task:");
            return 1;
        }

        fprintf(fptr, "0 %99s 0\n", title);

        fclose(fptr);
        return 0;
    }

    printf("unsupported command!\n");

    return 0;
}