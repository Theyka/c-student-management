/*
Student Manager:
1. Add user
2. Delete user
3. Show users
4. Update user
5. Exit

Description: Create a console app for this functions. The app should be optimized by using malloc etc.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define FILE_NAME "students.txt"

typedef struct {
    int id;
    char name[50];
    char surname[50];
} Student;

int isValidName(const char *name) {
    if (name[0] == '\0') {
        return 0;
    }

    for (int i = 0; name[i] != '\0'; i++) {
        if (isdigit((unsigned char)name[i])) {
            return 0;
        }
    }
    return 1;
}

void saveToFile(Student *students, int count) {
    FILE *file = fopen(FILE_NAME, "w");
    if (file == NULL) {
        printf("There was problem when saving to the file.\n");
        return;
    }
    for (int i = 0; i < count; i++) {
        fprintf(file, "%d %s %s\n", students[i].id, students[i].name, students[i].surname);
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
            Student *temp = (Student *)realloc(*students, (*capacity) * sizeof(Student));
            if (temp == NULL) {
                break;
            }

            *students = temp;
        }

        if (fscanf(file, "%d %s %s", &(*students)[count].id, (*students)[count].name, (*students)[count].surname) != 3) {
            break;
        }
        count++;
    }

    fclose(file);
    return count;
}

Student *students;

int main() {
    int Capacity = 10;
    students = (Student *)malloc(Capacity * sizeof(Student));
    int StudentCount = loadFromFile(&students, &Capacity);

    if (StudentCount > 0) {
        printf("Loaded %d students from the file.\n", StudentCount);
    }

    while (1) {
        printf("\nStudent Manager\n1. Add student\n2. Delete student\n3. Show students\n4. Update student\n5. Exit\nSelect: ");
        
        int option;
        if (scanf("%d", &option) != 1) {
            printf("Invalid input!\n");
            while (getchar() != '\n');
            continue;
        }

        switch(option) {
            case 1:
                if (StudentCount >= Capacity) {
                    Capacity *= 2;
                    Student *temp = (Student *)realloc(students, Capacity * sizeof(Student));
                    if (temp == NULL) {
                        break;
                    }
                    students = temp;
                }
                
                students[StudentCount].id = StudentCount + 1;
                
                while (1) {
                    printf("Enter name: ");
                    scanf("%s", students[StudentCount].name);
                    if (isValidName(students[StudentCount].name)) {
                        break;
                    }
                    printf("Invalid name.\n");
                }

                while (1) {
                    printf("Enter surname: ");
                    scanf("%s", students[StudentCount].surname);
                    if (isValidName(students[StudentCount].surname)) {
                        break;
                    }
                    printf("Invalid surname.\n");
                }
                
                StudentCount++;
                saveToFile(students, StudentCount);
                printf("Student added successfully.\n");
                break;

            case 2:
                printf("Enter id: ");
                int StudentID_Delete;
                if (scanf("%d", &StudentID_Delete) != 1) {
                    while (getchar() != '\n');
                }

                for (int i = 0; i < StudentCount; i++) {
                    if (students[i].id == StudentID_Delete) {
                        for (int j = i; j < StudentCount - 1; j++) {
                            students[j] = students[j + 1];
                        }
                        
                        StudentCount--;
                        printf("Deleted.\n");

                        saveToFile(students, StudentCount);

                        if (Capacity > 10 && StudentCount <= Capacity / 4) {
                            Capacity /= 2;
                            Student *temp = (Student *)realloc(students, Capacity * sizeof(Student));
                            if (temp != NULL) {
                                students = temp;
                            }
                        }
                        break;
                    }
                }
                break;

            case 3:
                if (StudentCount > 0) {
                    printf("ID | Name | Surname\n");
                    for (int i = 0; i < StudentCount; i++) {
                        printf("%d | %s | %s\n", students[i].id, students[i].name, students[i].surname);
                    }
                } else {
                    printf("No students found.\n");
                }
                break;

            case 4:
                printf("Enter id: ");
                int StudentID_Update;
                if (scanf("%d", &StudentID_Update) != 1) {
                    while (getchar() != '\n');
                }
                for (int i = 0; i < StudentCount; i++) {
                    if (students[i].id == StudentID_Update) {
                        while (1) {
                            printf("New name: ");
                            scanf("%s", students[i].name);
                            if (isValidName(students[i].name)) {
                                break;
                            }
                            printf("Invalid name.\n");
                        }

                        while (1) {
                            printf("New surname: ");
                            scanf("%s", students[i].surname);
                            if (isValidName(students[i].surname)) {
                                break;
                            }
                            printf("Invalid surname.\n");
                        }
                        saveToFile(students, StudentCount);
                        printf("Student updated successfully.\n");
                        break;
                    }
                }
                break;
                
            case 5:
                free(students);
                printf("Goodbye!\n");
                return 0;
        }
    }
    return 0;
}
