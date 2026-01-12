#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define FILE_NAME "students.txt"

typedef struct {
    int id;
    char name[50];
    char surname[50];
    int age;
} Student;

void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {
        ;
    }
}

void fixCase(char *str) {
    if (str[0] == '\0') {
        return;
    }

    str[0] = toupper((unsigned char)str[0]);

    for (int i = 1; str[i] != '\0'; i++) {
        str[i] = tolower((unsigned char)str[i]);
    }
}

int isNameValid(const char *name) {
    if (name[0] == '\0') {
        return 0;
    }
    
    for (int i = 0; name[i] != '\0'; i++) {
        if (!isalpha((unsigned char)name[i])) {
            return 0;
        }
    }
    return 1;
}

void saveToFile(Student *students, int count) {
    FILE *file = fopen(FILE_NAME, "w");
    if (file == NULL) {
        printf("Error: Could not save to file!\n");
        return;
    }

    for (int i = 0; i < count; i++) {
        fprintf(file, "%d %s %s %d\n", students[i].id, students[i].name, students[i].surname, students[i].age);
    }
    fclose(file);
}

int loadFromFile(Student **students, int *capacity) {
    FILE *file = fopen(FILE_NAME, "r");
    if (file == NULL) {
        return 0;
    }

    int count = 0;
    while (1) {
        if (count >= *capacity) {
            *capacity *= 2;
            *students = (Student *)realloc(*students, (*capacity) * sizeof(Student));
        }

        if (fscanf(file, "%d %s %s %d", &(*students)[count].id, (*students)[count].name, (*students)[count].surname, &(*students)[count].age) != 4) {
            break;
        }
        count++;
    }

    fclose(file);
    return count;
}

void addStudent(Student **students, int *count, int *capacity) {
    if (*count >= *capacity) {
        *capacity *= 2;
        *students = (Student *)realloc(*students, (*capacity) * sizeof(Student));
    }

    if (*count == 0) {
        (*students)[*count].id = 1;
    } else {
        (*students)[*count].id = (*students)[*count - 1].id + 1;
    }

    while (1) {
        printf("Enter Name: ");
        if (scanf("%s", (*students)[*count].name) != 1) {
            printf("Wrong input!\n");
            clearInputBuffer();
            continue;
        }
        if (isNameValid((*students)[*count].name)) {
            fixCase((*students)[*count].name);
            break;
        } else {
            printf("Error: Only letters allowed!\n");
        }
    }

    while (1) {
        printf("Enter Surname: ");
        if (scanf("%s", (*students)[*count].surname) != 1) {
            printf("Wrong input!\n");
            clearInputBuffer();
            continue;
        }

        if (isNameValid((*students)[*count].surname)) {
            fixCase((*students)[*count].surname);
            break;
        } else {
            printf("Error: Only letters allowed!\n");
        }
    }

    while (1) {
        printf("Enter Age: ");
        if (scanf("%d", &(*students)[*count].age) != 1) {
            printf("Error: Enter a number for age!\n");
            clearInputBuffer();
            continue;
        }
        if ((*students)[*count].age >= 6 && (*students)[*count].age <= 100) {
            break;
        } else {
            printf("Error: Age must be between 6 and 100!\n");
        }
    }

    (*count)++;
    saveToFile(*students, *count);
    printf("Successfully added!\n");
    clearInputBuffer();
}

void showStudents(Student *students, int count) {
    if (count == 0) {
        printf("\nNo students found.\n");
        return;
    }

    printf("\n%-5s | %-15s | %-15s | %-5s\n", "ID", "Name", "Surname", "Age");
    printf("--------------------------------------------------\n");
    for (int i = 0; i < count; i++) {
        printf("%-5d | %-15s | %-15s | %-5d\n", students[i].id, students[i].name, students[i].surname, students[i].age);
    }
}

void deleteStudent(Student *students, int *count) {
    int id;
    printf("Enter ID to delete: ");
    if (scanf("%d", &id) != 1) {
        printf("Invalid ID!\n");
        clearInputBuffer();
        return;
    }

    int found = 0;
    for (int i = 0; i < *count; i++) {
        if (students[i].id == id) {
            found = 1;
            for (int j = i; j < *count - 1; j++) {
                students[j] = students[j + 1];
            }
            (*count)--;
            printf("ID %d deleted.\n", id);
            saveToFile(students, *count);
            break;
        }
    }

    if (!found) {
        printf("Student not found!\n");
    }
    clearInputBuffer();
}

void updateStudent(Student *students, int count) {
    int id;
    printf("Enter ID to update: ");
    if (scanf("%d", &id) != 1) {
        printf("Invalid ID!\n");
        clearInputBuffer();
        return;
    }

    int found = 0;
    for (int i = 0; i < count; i++) {
        if (students[i].id == id) {
            found = 1;

            while (1) {
                printf("New Name: ");
                scanf("%s", students[i].name);
                if (isNameValid(students[i].name)) {
                    fixCase(students[i].name);
                    break;
                }
                printf("Error: Only letters allowed!\n");
            }

            while (1) {
                printf("New Surname: ");
                scanf("%s", students[i].surname);
                if (isNameValid(students[i].surname)) {
                    fixCase(students[i].surname);
                    break;
                }
                printf("Error: Only letters allowed!\n");
            }

            while (1) {
                printf("New Age: ");
                if (scanf("%d", &students[i].age) == 1 && students[i].age >= 6 && students[i].age <= 100) {
                    break;
                }
                printf("Error: Age must be a number between 6 and 100!\n");
                clearInputBuffer();
            }

            saveToFile(students, count);
            printf("Successfully updated!\n");
            break;
        }
    }

    if (!found) {
        printf("Student not found!\n");
    }

    clearInputBuffer();
}

int main() {
    int capacity = 10;
    int count = 0;
    Student *students = (Student *)malloc(capacity * sizeof(Student));

    if (students == NULL) {
        printf("Memory error!\n");
        return 1;
    }

    count = loadFromFile(&students, &capacity);
    if (count > 0) {
        printf("Loaded %d students.\n", count);
    }

    int choice;
    while (1) {
        printf("\n--- Student Manager ---\n");
        printf("1. Add Student\n");
        printf("2. Delete Student\n");
        printf("3. Show Students\n");
        printf("4. Update Student\n");
        printf("5. Exit\n");
        printf("Select: ");

        if (scanf("%d", &choice) != 1) {
            printf("Please enter a number (1-5)!\n");
            clearInputBuffer();
            continue;
        }

        if (choice == 1) {
            addStudent(&students, &count, &capacity);
        } else if (choice == 2) {
            deleteStudent(students, &count);
        } else if (choice == 3) {
            showStudents(students, count);
        } else if (choice == 4) {
            updateStudent(students, count);
        } else if (choice == 5) {
            free(students);
            printf("Goodbye!\n");
            break;
        } else {
            printf("Wrong choice (1-5)!\n");
        }
    }

    return 0;
}
