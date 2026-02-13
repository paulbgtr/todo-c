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

    int id = 0;

    while (fgets(buffer, sizeof(buffer), fptr) != NULL)
    {
        sscanf(buffer, "%d", &id);
    }
    fclose(fptr);

    return id + 1;
}

int create_new_task(FILE *fptr)
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

int delete_task(FILE *fptr)
{
    int id;
    FILE *temp = fopen("temp.txt", "w");

    printf("enter id of the task to delete:\n");
    scanf("%d", &id);

    fptr = fopen("tasks.txt", "r");

    if (!fptr)
    {
        perror("deleting task: ");
        return 1;
    }

    char buffer[256];

    int deleted_id = -1;

    while (fgets(buffer, sizeof(buffer), fptr) != NULL)
    {
        int found_id;

        sscanf(buffer, "%d", &found_id);

        if (found_id == id)
        {
            deleted_id = id;

            continue;
        }
        fputs(buffer, temp);
    }

    fclose(fptr);
    fclose(temp);

    if (deleted_id == -1)
    {
        return 1;
    }

    remove("tasks.txt");
    rename("temp.txt", "tasks.txt");

    return 0;
}

int main()
{
    char command;

    FILE *fptr;

    scanf(" %c", &command);

    switch (command)
    {
    case 'n':
        if (create_new_task(fptr) == 1)
        {
            return 1;
        }
        break;
    case 'd':
        if (delete_task(fptr) == 1)
        {
            printf("task not found!\n");
            return 1;
        }

        printf("task was deleted\n");

        break;
    default:
        printf("unsupported command!\n");
        return 0;
    }
}