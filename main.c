#include <stdio.h>
#include <string.h>

int get_new_id()
{
    FILE *fptr = fopen("tasks.txt", "r");

    char buffer[256];

    int id = 0;

    while (fgets(buffer, sizeof(buffer), fptr) != NULL)
    {
        if (sscanf(buffer, "%d", &id) != 1)
        {
            printf("failed getting task.\n");
        }
    }
    fclose(fptr);

    return id + 1;
}

int view_tasks()
{
    FILE *fptr = fopen("tasks.txt", "r");

    if (!fptr)
    {
        printf("no tasks are present\n");
        return 1;
    }

    char buffer[256];

    int count = 0;

    while (fgets(buffer, sizeof(buffer), fptr) != NULL)
    {
        printf("%s", buffer);
        count++;
    }

    if (count == 0)
    {
        printf("no tasks are present\n");
        return 1;
    }

    return 0;
}

void create_new_task()
{
    char title[100];

    printf("enter task title\n");
    scanf("%99s", title);

    FILE *fptr = fopen("tasks.txt", "a");

    if (!fptr)
    {
        perror("adding new task:");
    }

    int new_id = get_new_id();

    fprintf(fptr, "%d %s 0\n", new_id, title);

    fclose(fptr);

    printf("added a task with id %d\n", new_id);
}

int delete_task()
{
    int id;
    FILE *temp = fopen("temp.txt", "w");
    FILE *fptr = fopen("tasks.txt", "r");

    printf("enter id of the task to delete\n");
    scanf("%d", &id);

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

        if (sscanf(buffer, "%d", &found_id) != 1)
        {
            printf("failed getting task\n");
        }

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
        printf("no task with id %d\n", id);
        return 1;
    }
    printf("task was deleted\n");

    return 0;
}

int change_status()
{
    int id;
    FILE *temp = fopen("temp.txt", "w");
    FILE *fptr = fopen("tasks.txt", "r");

    printf("enter id of task to change status\n");
    scanf("%d", &id);

    if (!fptr)
    {
        perror("changing status: ");
        return 1;
    }

    char buffer[256];
    int is_changed_status = 0;

    while (fgets(buffer, sizeof(buffer), fptr) != NULL)
    {
        int found_id;
        char found_title[100];
        int found_status;

        if (sscanf(buffer, "%d %s %d", &found_id, found_title, &found_status) != 3)
        {
            printf("failed getting task\n");
        }

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
        printf("no task with id %d found.\n", id);
        return 1;
    }

    printf("changed status of task %d\n", id);

    return 0;
}

void help()
{
    printf("available commands:\nn - create new task\n d - delete task\n s - change status\n a - view all tasks\n q - quit\n");
}

void keyboard_handler(char command)
{
    switch (command)
    {
    case 'n':
        create_new_task();
        break;
    case 'd':
        delete_task();
        break;
    case 's':
        change_status();
        break;
    case 'a':
        view_tasks();
        break;
    case 'h':
        help();
        break;
    case 'q':
        break;
    default:
        printf("unsupported command!\n");
    }
}

void welcoming_message()
{
    printf("welcome to todo-c.\ntype 'h' to view available commands.\n");
}

int main()
{
    welcoming_message();

    char command = 0;

    FILE *fptr;

    while (command != 'q')
    {
        scanf(" %c", &command);
        keyboard_handler(command);
    }

    return 0;
}