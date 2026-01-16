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

    char buffer[100];
    while (1) {
        printf("Enter Name: ");
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            clearInputBuffer();
            continue;
        }

        size_t len = strlen(buffer);
        if (len > 0 && buffer[len - 1] == '\n') {
            buffer[len - 1] = '\0';
        }

        if (buffer[0] == '\0') {
            printf("Wrong input!\n");
            continue;
        }

        if (isNameValid(buffer)) {
            fixCase(buffer);
            strcpy((*students)[*count].name, buffer);
            break;
        } else {
            printf("Error: Only letters allowed!\n");
        }
    }

    while (1) {
        printf("Enter Surname: ");
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
             clearInputBuffer();
             continue;
        }

        size_t len = strlen(buffer);
        if (len > 0 && buffer[len - 1] == '\n') {
            buffer[len - 1] = '\0';
        }

        if (buffer[0] == '\0') {
            printf("Wrong input!\n");
            continue;
        }

        if (isNameValid(buffer)) {
            fixCase(buffer);
            strcpy((*students)[*count].surname, buffer);
            break;
        } else {
            printf("Error: Only letters allowed!\n");
        }
    }

    while (1) {
        printf("Enter Age: ");
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            clearInputBuffer();
            continue;
        }

        size_t len = strlen(buffer);
        if (len > 0 && buffer[len - 1] == '\n') {
            buffer[len - 1] = '\0';
        }
        
        if (buffer[0] == '\0') {
            printf("Wrong input!\n");
            continue;
        }

        int isNum = 1;
        for (int i = 0; buffer[i] != '\0'; i++) {
            if (!isdigit(buffer[i])) {
                isNum = 0;
                break;
            }
        }

        if (!isNum) {
            printf("Error: Age must be a number!\n");
            continue;
        }

        int val = atoi(buffer);
        if (val >= 6 && val <= 100) {
            (*students)[*count].age = val;
            break;
        } else {
            printf("Error: Age must be between 6 and 100!\n");
        }
    }

    (*count)++;
    saveToFile(*students, *count);
    printf("Successfully added!\n");
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
    char buffer[50];
    printf("Enter ID to delete: ");
    
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        return;
    }
    
    size_t len = strlen(buffer);
    if (len > 0 && buffer[len - 1] == '\n') {
        buffer[len - 1] = '\0';
    }
    
    if (buffer[0] == '\0') {
         printf("Wrong input!\n");
         return;
    }
    
    if (sscanf(buffer, "%d", &id) != 1) {
        printf("Invalid ID!\n");
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
    char buffer[100];

    printf("Enter ID to update: ");
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        return;
    }

    size_t len = strlen(buffer);
    if (len > 0 && buffer[len - 1] == '\n') {
        buffer[len - 1] = '\0';
    }

    if (buffer[0] == '\0') {
         printf("Wrong input!\n");
         return;
    }

    if (sscanf(buffer, "%d", &id) != 1) {
        printf("Invalid ID!\n");
        return;
    }

    int found = 0;
    for (int i = 0; i < count; i++) {
        if (students[i].id == id) {
            found = 1;

            while (1) {
                printf("New Name: ");
                if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
                    break;
                }
                
                size_t len = strlen(buffer);
                if (len > 0 && buffer[len - 1] == '\n') {
                    buffer[len - 1] = '\0';
                }

                if (buffer[0] == '\0') {
                    break;
                }

                if (isNameValid(buffer)) {
                    fixCase(buffer);
                    strcpy(students[i].name, buffer);
                    break;
                } else {
                    printf("Error: Only letters allowed!\n");
                }
            }

            while (1) {
                printf("New Surname: ");
                if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
                    break;
                }
                
                size_t len = strlen(buffer);
                if (len > 0 && buffer[len - 1] == '\n') {
                    buffer[len - 1] = '\0';
                }

                if (buffer[0] == '\0') {
                    break;
                }

                if (isNameValid(buffer)) {
                    fixCase(buffer);
                    strcpy(students[i].surname, buffer);
                    break;
                } else {
                    printf("Error: Only letters allowed!\n");
                }
            }

            while (1) {
                printf("New Age: ");
                if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
                    break;
                }

                size_t len = strlen(buffer);
                if (len > 0 && buffer[len - 1] == '\n') {
                    buffer[len - 1] = '\0';
                }

                if (buffer[0] == '\0') {
                    break;
                }

                int isNum = 1;
                for (int k = 0; buffer[k] != '\0'; k++) {
                    if (!isdigit(buffer[k])) {
                        isNum = 0;
                        break;
                    }
                }

                if (isNum) {
                    int val = atoi(buffer);
                    if (val >= 6 && val <= 100) {
                        students[i].age = val;
                        break;
                    } else {
                        printf("Error: Age must be between 6 and 100!\n");
                    }
                } else {
                    printf("Error: Enter a valid number!\n");
                }
            }

            saveToFile(students, count);
            printf("Successfully updated!\n");
            break;
        }
    }

    if (!found) {
        printf("Student not found!\n");
    }
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

        char buffer[50];
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            continue;
        }
        
        size_t len = strlen(buffer);
        if (len > 0 && buffer[len - 1] == '\n') {
            buffer[len - 1] = '\0';
        }

        if (buffer[0] == '\0') {
             printf("Wrong input!\n");
             continue;
        }

        if (sscanf(buffer, "%d", &choice) != 1) {
            printf("Please enter a number (1-5)!\n");
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
