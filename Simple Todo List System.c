#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// Define a structure for tasks
typedef struct {
    char name[100];
    char description[255];
    char addingDate[20];
    char dueDate[20];
    bool completed;
} Task;

// Function declarations
void addTask(Task **tasks, int *taskCount);
void viewTasks(Task *tasks, int taskCount);
void markTaskAsCompleted(Task *tasks, int taskCount);
void deleteCompletedTasks(Task **tasks, int *taskCount);
void saveTasksToFile(Task *tasks, int taskCount);
void loadTasksFromFile(Task **tasks, int *taskCount);

int main() {
    Task *tasks = NULL;
    int taskCount = 0;
    int choice;

    loadTasksFromFile(&tasks, &taskCount);

    while (1) {
        printf("\n==== To-Do List Menu ====");
        printf("\n1. Add Task");
        printf("\n2. View Tasks");
        printf("\n3. Mark Task as Completed");
        printf("\n4. Delete Completed Tasks");
        printf("\n5. Save and Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                addTask(&tasks, &taskCount);
                saveTasksToFile(tasks, taskCount);  // Save after adding a task
                break;
            case 2:
                viewTasks(tasks, taskCount);
                break;
            case 3:
                markTaskAsCompleted(tasks, taskCount);
                saveTasksToFile(tasks, taskCount);  // Save after marking a task as completed
                break;
            case 4:
                deleteCompletedTasks(&tasks, &taskCount);
                saveTasksToFile(tasks, taskCount);  // Save after deleting completed tasks
                break;
            case 5:
                saveTasksToFile(tasks, taskCount);
                free(tasks);
                printf("Exiting...\n");
                return 0;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }
}

void addTask(Task **tasks, int *taskCount) {
    *tasks = realloc(*tasks, (*taskCount + 1) * sizeof(Task));
    if (*tasks == NULL) {
        printf("Memory allocation failed.\n");
        return;
    }

    Task *newTask = &(*tasks)[*taskCount];

    printf("Enter task name: ");
    getchar();
    fgets(newTask->name, sizeof(newTask->name), stdin);
    newTask->name[strcspn(newTask->name, "\n")] = 0;

    printf("Enter task description: ");
    fgets(newTask->description, sizeof(newTask->description), stdin);
    newTask->description[strcspn(newTask->description, "\n")] = 0;

    printf("Enter adding date (YYYY-MM-DD): ");
    fgets(newTask->addingDate, sizeof(newTask->addingDate), stdin);
    newTask->addingDate[strcspn(newTask->addingDate, "\n")] = 0;

    printf("Enter due date (YYYY-MM-DD): ");
    fgets(newTask->dueDate, sizeof(newTask->dueDate), stdin);
    newTask->dueDate[strcspn(newTask->dueDate, "\n")] = 0;

    newTask->completed = false;

    (*taskCount)++;
    printf("Task added successfully as Task No. %d.\n", *taskCount);
}

void viewTasks(Task *tasks, int taskCount) {
    if (taskCount == 0) {
        printf("No tasks available.\n");
        return;
    }

    for (int i = 0; i < taskCount; i++) {
        printf("\nTask No. %d\n", i + 1);
        printf("Task Name: %s\n", tasks[i].name);
        printf("Description: %s\n", tasks[i].description);
        printf("Adding Date: %s\n", tasks[i].addingDate);
        printf("Due Date: %s\n", tasks[i].dueDate);
        printf("Status: %s\n", tasks[i].completed ? "Complete" : "Incomplete");
    }
}

void markTaskAsCompleted(Task *tasks, int taskCount) {
    if (taskCount == 0) {
        printf("No tasks available to mark as completed.\n");
        return;
    }

    int taskNumber;
    printf("Enter task number to mark as completed: ");
    scanf("%d", &taskNumber);

    if (taskNumber < 1 || taskNumber > taskCount) {
        printf("Invalid task number.\n");
        return;
    }

    tasks[taskNumber - 1].completed = true;
    printf("Task No. %d Task Name:\"%s\" marked as completed.\n", taskNumber, tasks[taskNumber - 1].name);
}

void deleteCompletedTasks(Task **tasks, int *taskCount) {
    if (*taskCount == 0) {
        printf("No tasks to delete.\n");
        return;
    }

    int completedCount = 0;
    for (int i = 0; i < *taskCount; i++) {
        if ((*tasks)[i].completed) {
            completedCount++;
        }
    }

    if (completedCount == 0) {
        printf("No completed tasks to delete.\n");
        return;
    }

    Task *newTasks = malloc((*taskCount - completedCount) * sizeof(Task));
    if (newTasks == NULL) {
        printf("Memory allocation failed.\n");
        return;
    }

    int newIndex = 0;
    for (int i = 0; i < *taskCount; i++) {
        if (!(*tasks)[i].completed) {
            newTasks[newIndex++] = (*tasks)[i];
        }
    }

    free(*tasks);
    *tasks = newTasks;
    *taskCount -= completedCount;

    printf("Deleted %d completed task(s). Remaining tasks: %d.\n", completedCount, *taskCount);
}

void saveTasksToFile(Task *tasks, int taskCount) {
    FILE *file = fopen("todo.txt", "w");
    if (file == NULL) {
        printf("Error saving tasks to file.\n");
        return;
    }

    fprintf(file, "Loaded file: %d\n", taskCount); 
    for (int i = 0; i < taskCount; i++) {
        fprintf(file, "Task Name: %s\n", tasks[i].name);
        fprintf(file, "Task Description: %s\n", tasks[i].description);
        fprintf(file, "Task Adding Date: %s\n", tasks[i].addingDate);
        fprintf(file, "Task Due Date: %s\n", tasks[i].dueDate);

        // Print status as "Complete" or "Incomplete"
        fprintf(file, "Task Status: %s\n", tasks[i].completed ? "Complete" : "Incomplete");
    }

    fclose(file);
    printf("Tasks saved to file successfully.\n");
}

void loadTasksFromFile(Task **tasks, int *taskCount) {
    FILE *file = fopen("todo.txt", "r");
    if (file == NULL) {
        printf("No saved tasks found. Starting with an empty list.\n");
        return;
    }

    fscanf(file, "%d\n", taskCount); 
    *tasks = malloc(*taskCount * sizeof(Task));
    if (*tasks == NULL) {
        printf("Memory allocation failed.\n");
        fclose(file);
        return;
    }

    for (int i = 0; i < *taskCount; i++) {
        fgets((*tasks)[i].name, sizeof((*tasks)[i].name), file);
        (*tasks)[i].name[strcspn((*tasks)[i].name, "\n")] = 0; 

        fgets((*tasks)[i].description, sizeof((*tasks)[i].description), file);
        (*tasks)[i].description[strcspn((*tasks)[i].description, "\n")] = 0;

        fgets((*tasks)[i].addingDate, sizeof((*tasks)[i].addingDate), file);
        (*tasks)[i].addingDate[strcspn((*tasks)[i].addingDate, "\n")] = 0;

        fgets((*tasks)[i].dueDate, sizeof((*tasks)[i].dueDate), file);
        (*tasks)[i].dueDate[strcspn((*tasks)[i].dueDate, "\n")] = 0;

        // Read the status as "Complete" or "Incomplete"
        char status[20];
        fgets(status, sizeof(status), file);
        status[strcspn(status, "\n")] = 0;  // Remove newline character

        (*tasks)[i].completed = (strcmp(status, "Complete") == 0);
    }

    fclose(file);
    printf("Loaded %d task(s) from file.\n", *taskCount);
}
