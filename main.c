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

typedef struct {
    int id;
    char name[50];
    char surname[50];
} Student;

Student *students;

int main() {
    int Capacity = 10;
    int StudentCount = 0;
    students = (Student *)malloc(Capacity * sizeof(Student));

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
                printf("Enter name: ");
                scanf("%s", students[StudentCount].name);
                printf("Enter surname: ");
                scanf("%s", students[StudentCount].surname);
                StudentCount++;
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
                        printf("New name: ");
                        scanf("%s", students[i].name);
                        printf("New surname: ");
                        scanf("%s", students[i].surname);
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