#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_TASKS 100
#define TITLE_LENGTH 100
#define DESC_LENGTH 256
#define FILENAME "tasks.txt"

typedef struct {
    char title[TITLE_LENGTH];
    char description[DESC_LENGTH];
    int completed;
} Task;

Task tasks[MAX_TASKS];
int numTasks = 0;

void loadTasks() {
    FILE *file = fopen(FILENAME, "r");
    if (!file) return;

    numTasks = 0;
    while (numTasks < MAX_TASKS && fscanf(file, "%d\n", &tasks[numTasks].completed) == 1) {
        if (fgets(tasks[numTasks].title, TITLE_LENGTH, file) == NULL)
            break;
        size_t len = strlen(tasks[numTasks].title);
        if (len > 0 && tasks[numTasks].title[len-1] == '\n')
            tasks[numTasks].title[len-1] = '\0';

        if (fgets(tasks[numTasks].description, DESC_LENGTH, file) == NULL)
            break;
        len = strlen(tasks[numTasks].description);
        if (len > 0 && tasks[numTasks].description[len-1] == '\n')
            tasks[numTasks].description[len-1] = '\0';

        numTasks++;
    }
    fclose(file);
}

void saveTasks() {
    FILE *file = fopen(FILENAME, "w");
    if (!file) {
        printf("Error opening file for writing.\n");
        return;
    }
    for (int i = 0; i < numTasks; i++) {
        fprintf(file, "%d\n", tasks[i].completed);
        fprintf(file, "%s\n", tasks[i].title);
        fprintf(file, "%s\n", tasks[i].description);
    }
    fclose(file);
}

void addTask() {
    if (numTasks >= MAX_TASKS) {
        printf("Maximum number of tasks reached.\n");
        return;
    }
    Task newTask;
    printf("Enter task title: ");
    fgets(newTask.title, TITLE_LENGTH, stdin);
    size_t len = strlen(newTask.title);
    if (len > 0 && newTask.title[len-1] == '\n')
        newTask.title[len-1] = '\0';

    printf("Enter task description: ");
    fgets(newTask.description, DESC_LENGTH, stdin);
    len = strlen(newTask.description);
    if (len > 0 && newTask.description[len-1] == '\n')
        newTask.description[len-1] = '\0';

    newTask.completed = 0;
    tasks[numTasks++] = newTask;
    saveTasks();
    printf("Task added successfully.\n");
}

void listTasks() {
    if (numTasks == 0) {
        printf("No tasks found.\n");
        return;
    }
    for (int i = 0; i < numTasks; i++) {
        printf("Task %d:\n", i + 1);
        printf(" Title: %s\n", tasks[i].title);
        printf(" Description: %s\n", tasks[i].description);
        printf(" Status: %s\n", tasks[i].completed ? "Completed" : "Pending");
        printf("-------------------------\n");
    }
}

void markTaskCompleted() {
    int taskNumber;
    printf("Enter task number to mark as completed: ");
    scanf("%d", &taskNumber);
    getchar();
    if (taskNumber < 1 || taskNumber > numTasks) {
        printf("Invalid task number.\n");
        return;
    }
    tasks[taskNumber - 1].completed = 1;
    saveTasks();
    printf("Task marked as completed.\n");
}

void deleteTask() {
    int taskNumber;
    printf("Enter task number to delete: ");
    scanf("%d", &taskNumber);
    getchar();
    if (taskNumber < 1 || taskNumber > numTasks) {
        printf("Invalid task number.\n");
        return;
    }

    for (int i = taskNumber - 1; i < numTasks - 1; i++) {
        tasks[i] = tasks[i + 1];
    }
    numTasks--;
    saveTasks();
    printf("Task deleted successfully.\n");
}

void displayMenu() {
    printf("==== TaskMaster ====\n");
    printf("1. Add Task\n");
    printf("2. List Tasks\n");
    printf("3. Mark Task as Completed\n");
    printf("4. Delete Task\n");
    printf("5. Quit\n");
    printf("Choose an option: ");
}

int main() {
    loadTasks();
    int option;
    while (1) {
        displayMenu();
        scanf("%d", &option);
        getchar();

        switch (option) {
            case 1:
                addTask();
                break;
            case 2:
                listTasks();
                break;
            case 3:
                markTaskCompleted();
                break;
            case 4:
                deleteTask();
                break;
            case 5:
                printf("Exiting TaskMaster.\n");
                return 0;
            default:
                printf("Invalid option. Try again.\n");
        }
        printf("\n");
    }
    return 0;
}
