#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Student Record System in C

// Define the Student structure
typedef struct {
    int roll_number;
    char name[20];
    float marks;
    char success[10]; // Field to hold "Passed" or "Failed"
} Student;

// 1. Display a welcome message and greet the user by name
void welcome() {
    char user_name[50]; // Array to hold the user's name
    
    printf("Welcome to the Student Record System\n");
    printf("Enter your name: ");
    fgets(user_name, sizeof(user_name), stdin);
    
    // Remove newline character if present
    user_name[strcspn(user_name, "\n")] = '\0';
    
    printf("Hello, %s!\n", user_name);
}

// 2. Input student details and determine pass/fail status
void inputStudent(Student *student) {
    char buffer[100];
    
    printf("Enter roll number: ");
    fgets(buffer, sizeof(buffer), stdin);
    sscanf(buffer, "%d", &student->roll_number);
    
    printf("Enter name: ");
    fgets(student->name, sizeof(student->name), stdin);
    // Remove newline character if present
    student->name[strcspn(student->name, "\n")] = 0;
    
    printf("Enter marks: ");
    fgets(buffer, sizeof(buffer), stdin);
    sscanf(buffer, "%f", &student->marks);
    
    strcpy(student->success, student->marks > 40 ? "Passed" : "Failed");
}

// 3. Display student details
void displayStudent(const Student *student) {
    printf("Roll Number: %d, Name: %s, Marks: %.2f, Success: %s\n",
           student->roll_number, student->name, student->marks, student->success);
}

// 4. Display all students
void displayAllStudents(const Student *students, int count) {
    for (int i = 0; i < count; i++) {
        displayStudent(&students[i]);
    }
}

// 5. Add a student
void addStudent(Student **students, int *count, int *capacity) {
    if (*count >= *capacity) {
        *capacity *= 2;
        *students = (Student *)realloc(*students, *capacity * sizeof(Student));
        if (*students == NULL) {
            printf("Memory reallocation failed\n");
            exit(1);
        }
    }
    inputStudent(&(*students)[*count]);
    (*count)++;
}

// 6. Remove a student by roll number
void removeStudent(Student **students, int *count, int *capacity, int roll_number) {
    int found = 0;
    for (int i = 0; i < *count; i++) {
        if ((*students)[i].roll_number == roll_number) {
            found = 1;
            for (int j = i; j < *count - 1; j++) {
                (*students)[j] = (*students)[j + 1];
            }
            (*count)--;
            break;
        }
    }
    if (!found) {
        printf("Student with roll number %d not found.\n", roll_number);
    } else {
        printf("Student with roll number %d removed.\n", roll_number);
        // Resize array if necessary
        if (*count > 0 && *count <= *capacity / 4) {
            *capacity /= 2;
            *students = (Student *)realloc(*students, *capacity * sizeof(Student));
            if (*students == NULL) {
                printf("Memory reallocation failed\n");
                exit(1);
            }
        }
    }
}

// 7. Modify a student by roll number
void modifyStudent(Student *students, int count, int roll_number) {
    int found = 0;
    for (int i = 0; i < count; i++) {
        if (students[i].roll_number == roll_number) {
            found = 1;
            printf("Modifying student with roll number %d:\n", roll_number);
            inputStudent(&students[i]);
            break;
        }
    }
    if (!found) {
        printf("Student with roll number %d not found.\n", roll_number);
    }
}

// 8. Save students to a file
void saveStudentsToFile(const Student *students, int count, const char *filename) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        printf("Error opening file for writing\n");
        return;
    }
    for (int i = 0; i < count; i++) {
        fprintf(file, "%d\n%s\n%.2f\n%s\n", students[i].roll_number, students[i].name, students[i].marks, students[i].success);
    }
    fclose(file);
    printf("Students saved to %s\n", filename);
}

// 8. Load students from a file
void loadStudentsFromFile(Student **students, int *count, int *capacity, const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error opening file for reading\n");
        return;
    }
    Student temp;
    while (fscanf(file, "%d\n", &temp.roll_number) != EOF) {
        fgets(temp.name, sizeof(temp.name), file);
        temp.name[strcspn(temp.name, "\n")] = 0;
        fscanf(file, "%f\n", &temp.marks);
        fgets(temp.success, sizeof(temp.success), file);
        temp.success[strcspn(temp.success, "\n")] = 0;
        if (*count >= *capacity) {
            *capacity *= 2;
            *students = (Student *)realloc(*students, *capacity * sizeof(Student));
            if (*students == NULL) {
                printf("Memory reallocation failed\n");
                fclose(file);
                return;
            }
        }
        (*students)[*count] = temp;
        (*count)++;
    }
    fclose(file);
    printf("Students loaded from %s\n", filename);
}

// 9. Calculate and display average marks
void calculateAverageMarks(const Student *students, int count) {
    if (count == 0) {
        printf("No students available to calculate average marks.\n");
        return;
    }

    float totalMarks = 0.0;
    for (int i = 0; i < count; i++) {
        totalMarks += students[i].marks;
    }

    float averageMarks = totalMarks / count;
    printf("Average Marks of All Students: %.2f\n", averageMarks);
}

// 10. Compare students by marks in ascending order
int compareMarksAscending(const void *a, const void *b) {
    float markA = ((Student *)a)->marks;
    float markB = ((Student *)b)->marks;
    return (markA > markB) - (markA < markB);
}

// 10. Compare students by marks in descending order
int compareMarksDescending(const void *a, const void *b) {
    float markA = ((Student *)a)->marks;
    float markB = ((Student *)b)->marks;
    return (markB > markA) - (markB < markA);
}

// 10. Sort students by marks
void sortStudentsByMarks(Student *students, int count, int ascending) {
    if (ascending) {
        qsort(students, count, sizeof(Student), compareMarksAscending);
    } else {
        qsort(students, count, sizeof(Student), compareMarksDescending);
    }
    printf("Students sorted by marks.\n");
}

// 11. Search for a student by roll number and display their information
void searchStudent(const Student *students, int count, int roll_number) {
    int found = 0;
    for (int i = 0; i < count; i++) {
        if (students[i].roll_number == roll_number) {
            displayStudent(&students[i]);
            found = 1;
            break;
        }
    }
    if (!found) {
        printf("Student with roll number %d not found.\n", roll_number);
    }
}

int main() {
    Student *students = NULL;
    int count = 0;
    int capacity = 2; // Initial capacity for dynamic array
    int choice, roll_number;
    char filename[100];

    // Allocate initial memory for students array
    students = (Student *)malloc(capacity * sizeof(Student));
    if (students == NULL) {
        printf("Memory allocation failed\n");
        return 1;
    }

    welcome();

    while (1) {
        printf("\nMenu:\n");
        printf("1. Add Student\n");
        printf("2. Remove Student\n");
        printf("3. Modify Student\n");
        printf("4. Display All Students\n");
        printf("5. Save Students to File\n");
        printf("6. Load Students from File\n");
        printf("7. Calculate Average Marks\n");
        printf("8. Sort Students by Marks\n");
        printf("9. Search Student by Roll Number\n");
        printf("10. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar(); // Clear newline character from buffer

        switch (choice) {
            case 1:
                addStudent(&students, &count, &capacity);
                break;
            case 2:
                printf("Enter roll number of student to remove: ");
                scanf("%d", &roll_number);
                getchar(); // Clear newline character from buffer
                removeStudent(&students, &count, &capacity, roll_number);
                break;
            case 3:
                printf("Enter roll number of student to modify: ");
                scanf("%d", &roll_number);
                getchar(); // Clear newline character from buffer
                modifyStudent(students, count, roll_number);
                break;
            case 4:
                displayAllStudents(students, count);
                break;
            case 5:
                printf("Enter filename to save to: ");
                fgets(filename, sizeof(filename), stdin);
                filename[strcspn(filename, "\n")] = 0;
                saveStudentsToFile(students, count, filename);
                break;
            case 6:
                printf("Enter filename to load from: ");
                fgets(filename, sizeof(filename), stdin);
                filename[strcspn(filename, "\n")] = 0;
                loadStudentsFromFile(&students, &count, &capacity, filename);
                break;
            case 7:
                calculateAverageMarks(students, count);
                break;
            case 8:
                printf("Enter 1 to sort in ascending order, 0 for descending: ");
                int ascending;
                scanf("%d", &ascending);
                getchar(); // Clear newline character from buffer
                sortStudentsByMarks(students, count, ascending);
                break;
            case 9:
                printf("Enter roll number of student to search: ");
                scanf("%d", &roll_number);
                getchar(); // Clear newline character from buffer
                searchStudent(students, count, roll_number);
                break;
            case 10:
                free(students);
                return 0;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }

    return 0;
}


// Create a simple Student Record System in C that allows users to manage student information. The program should include the following functionality:

// Initial Setup:
//    - Create a new C project using JetBrains Clion (or your preferred development environment)
//    - Write a program that displays a welcome message and prompts the user to enter their name. The program should greet the user by their name.

// Data Storage:
//    - Implement storage for student information, including name, roll number, and marks, using appropriate data types.

// Input and Output:
//    - Add a feature to input a student's marks and display whether the student has passed or failed based on a passing threshold (e.g., marks above 40 are considered passing).

// Student Records Management:
//    - Implement a loop that allows the user to input information for multiple students until the user decides to exit.

//    - Create an array to store information for multiple students, with the ability to add, remove, and modify student records.

//    - Implement functions for adding, modifying, and displaying student records. Ensure these functions take appropriate arguments and return relevant information.

// Memory Management:
//    - Use dynamic memory allocation for student records and ensure memory is freed when no longer needed.

// File Operations:
//    - Implement the ability to save student records to a text file and load student records from a text file, storing information such as name, roll number, and marks.

// Search Functionality:
//    - Allow users to search for a student by their roll number and display the relevant information.

// Calculations and Sorting:
//    - Calculate and display the average marks for all students.

//    - Provide an option to sort student records based on marks in ascending or descending order.