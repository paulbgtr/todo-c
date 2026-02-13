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
    int is_deleted = 0;

    while (fgets(buffer, sizeof(buffer), fptr) != NULL)
    {
        int found_id;

        sscanf(buffer, "%d", &found_id);

        if (found_id == id)
        {
            is_deleted = 1;

            continue;
        }
        fputs(buffer, temp);
    }

    fclose(fptr);
    fclose(temp);

    remove("tasks.txt");
    rename("temp.txt", "tasks.txt");

    if (is_deleted == 0)
    {
        return 1;
    }

    return 0;
}

int change_status(FILE *fptr)
{

    int id;
    FILE *temp = fopen("temp.txt", "w");

    printf("enter id of task to change status\n");
    scanf("%d", &id);

    fptr = fopen("tasks.txt", "r");

    char buffer[256];
    int is_changed_status = 0;

    while (fgets(buffer, sizeof(buffer), fptr) != NULL)
    {
        int found_id;
        char found_title[100];
        int found_status;

        sscanf(buffer, "%d %s %d", &found_id, found_title, &found_status);

        if (found_id != id)
        {
            fputs(buffer, temp);
            continue;
        }

        if (found_status == 0)
        {
            fprintf(temp, "%d %s 1\n", found_id, found_title);
            is_changed_status = 1;
        }
        else if (found_status == 1)
        {
            fprintf(temp, "%d %s 0\n", found_id, found_title);
            is_changed_status = 1;
        }
    }

    fclose(fptr);
    fclose(temp);

    remove("tasks.txt");
    rename("temp.txt", "tasks.txt");

    if (is_changed_status != 1)
    {
        return 1;
    }

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
    case 's':
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