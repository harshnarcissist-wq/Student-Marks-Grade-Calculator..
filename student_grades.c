/*
 * Student Marks & Average Grade Calculator
 * ----------------------------------------
 * Project: Store student names, marks, and calculate average grades
 *          using arrays and structures.
 *
 * Language : C (standard C99)
 * Compile  : gcc student_grades.c -o student_grades
 * Run      : ./student_grades        (Windows: student_grades.exe)
 */

#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX_STUDENTS 100
#define NAME_LEN     50
#define SUBJECTS     5

/* Subject names stored in an array */
const char *subjectNames[SUBJECTS] = {
    "Maths", "Physics", "Chemistry", "English", "Computer"
};

/* Structure to hold one student's record */
struct Student {
    int   rollNo;
    char  name[NAME_LEN];
    float marks[SUBJECTS];   /* array inside structure */
    float total;
    float average;
    char  grade;
};

/* Array of structures to hold all students */
struct Student students[MAX_STUDENTS];
int studentCount = 0;

/* ---------- Helper functions ---------- */

/* Read a whole line safely and remove the trailing newline */
void readLine(char *buffer, int size) {
    if (fgets(buffer, size, stdin) != NULL) {
        buffer[strcspn(buffer, "\n")] = '\0';
    } else {
        buffer[0] = '\0';
    }
}

/* Read an integer with validation */
int readInt(const char *prompt, int min, int max) {
    char line[100];
    int value;
    while (1) {
        printf("%s", prompt);
        readLine(line, sizeof(line));
        if (sscanf(line, "%d", &value) == 1 && value >= min && value <= max)
            return value;
        printf("  Invalid input. Enter a number between %d and %d.\n", min, max);
    }
}

/* Read a float with validation */
float readFloat(const char *prompt, float min, float max) {
    char line[100];
    float value;
    while (1) {
        printf("%s", prompt);
        readLine(line, sizeof(line));
        if (sscanf(line, "%f", &value) == 1 && value >= min && value <= max)
            return value;
        printf("  Invalid input. Enter a number between %.0f and %.0f.\n", min, max);
    }
}

/* Convert average to a letter grade */
char calculateGrade(float avg) {
    if (avg >= 90) return 'A';
    if (avg >= 75) return 'B';
    if (avg >= 60) return 'C';
    if (avg >= 40) return 'D';
    return 'F';
}

/* Case-insensitive substring search */
int containsIgnoreCase(const char *text, const char *pattern) {
    char t[NAME_LEN], p[NAME_LEN];
    int i;
    for (i = 0; text[i] && i < NAME_LEN - 1; i++) t[i] = (char)tolower((unsigned char)text[i]);
    t[i] = '\0';
    for (i = 0; pattern[i] && i < NAME_LEN - 1; i++) p[i] = (char)tolower((unsigned char)pattern[i]);
    p[i] = '\0';
    return strstr(t, p) != NULL;
}

/* Does this roll number already exist? */
int rollExists(int roll) {
    for (int i = 0; i < studentCount; i++)
        if (students[i].rollNo == roll) return 1;
    return 0;
}

/* ---------- Core features ---------- */

void addStudent(void) {
    if (studentCount >= MAX_STUDENTS) {
        printf("\nStudent list is full (max %d).\n", MAX_STUDENTS);
        return;
    }

    struct Student s;
    printf("\n--- Add New Student ---\n");

    s.rollNo = readInt("Roll number: ", 1, 999999);
    if (rollExists(s.rollNo)) {
        printf("A student with roll number %d already exists.\n", s.rollNo);
        return;
    }

    do {
        printf("Name: ");
        readLine(s.name, NAME_LEN);
        if (s.name[0] == '\0') printf("  Name cannot be empty.\n");
    } while (s.name[0] == '\0');

    s.total = 0;
    for (int i = 0; i < SUBJECTS; i++) {
        char prompt[80];
        snprintf(prompt, sizeof(prompt), "Marks in %s (0-100): ", subjectNames[i]);
        s.marks[i] = readFloat(prompt, 0, 100);
        s.total += s.marks[i];
    }

    s.average = s.total / SUBJECTS;
    s.grade   = calculateGrade(s.average);

    students[studentCount++] = s;
    printf("\nStudent added. Average: %.2f  Grade: %c\n", s.average, s.grade);
}

void printHeader(void) {
    printf("\n%-6s %-20s", "Roll", "Name");
    for (int i = 0; i < SUBJECTS; i++) printf(" %-9.9s", subjectNames[i]);
    printf(" %-8s %-8s %-5s\n", "Total", "Average", "Grade");
    for (int i = 0; i < 6 + 21 + SUBJECTS * 10 + 24; i++) putchar('-');
    putchar('\n');
}

void printStudent(const struct Student *s) {
    printf("%-6d %-20.20s", s->rollNo, s->name);
    for (int i = 0; i < SUBJECTS; i++) printf(" %-9.1f", s->marks[i]);
    printf(" %-8.1f %-8.2f %-5c\n", s->total, s->average, s->grade);
}

void displayAll(void) {
    if (studentCount == 0) {
        printf("\nNo students recorded yet.\n");
        return;
    }
    printf("\n--- All Students (%d) ---", studentCount);
    printHeader();
    for (int i = 0; i < studentCount; i++) printStudent(&students[i]);
}

void searchStudent(void) {
    if (studentCount == 0) {
        printf("\nNo students recorded yet.\n");
        return;
    }
    char query[NAME_LEN];
    printf("\nEnter name (or part of name) to search: ");
    readLine(query, NAME_LEN);

    int found = 0;
    for (int i = 0; i < studentCount; i++) {
        if (containsIgnoreCase(students[i].name, query)) {
            if (!found) printHeader();
            printStudent(&students[i]);
            found++;
        }
    }
    if (!found) printf("No student found matching \"%s\".\n", query);
}

void classStatistics(void) {
    if (studentCount == 0) {
        printf("\nNo students recorded yet.\n");
        return;
    }

    float sumAvg = 0;
    int topper = 0, lowest = 0;
    int gradeCount[5] = {0};   /* A, B, C, D, F */
    float subjectSum[SUBJECTS] = {0};

    for (int i = 0; i < studentCount; i++) {
        sumAvg += students[i].average;
        if (students[i].average > students[topper].average) topper = i;
        if (students[i].average < students[lowest].average) lowest = i;

        switch (students[i].grade) {
            case 'A': gradeCount[0]++; break;
            case 'B': gradeCount[1]++; break;
            case 'C': gradeCount[2]++; break;
            case 'D': gradeCount[3]++; break;
            default:  gradeCount[4]++; break;
        }
        for (int j = 0; j < SUBJECTS; j++) subjectSum[j] += students[i].marks[j];
    }

    printf("\n--- Class Statistics ---\n");
    printf("Total students : %d\n", studentCount);
    printf("Class average  : %.2f\n", sumAvg / studentCount);
    printf("Topper         : %s (%.2f, Grade %c)\n",
           students[topper].name, students[topper].average, students[topper].grade);
    printf("Lowest         : %s (%.2f, Grade %c)\n",
           students[lowest].name, students[lowest].average, students[lowest].grade);

    printf("\nSubject-wise class average:\n");
    for (int j = 0; j < SUBJECTS; j++)
        printf("  %-10s %.2f\n", subjectNames[j], subjectSum[j] / studentCount);

    printf("\nGrade distribution:\n");
    printf("  A: %d   B: %d   C: %d   D: %d   F: %d\n",
           gradeCount[0], gradeCount[1], gradeCount[2], gradeCount[3], gradeCount[4]);
}

void sortByAverage(void) {
    if (studentCount == 0) {
        printf("\nNo students recorded yet.\n");
        return;
    }
    /* Bubble sort on a copy so the original order is preserved */
    struct Student sorted[MAX_STUDENTS];
    for (int i = 0; i < studentCount; i++) sorted[i] = students[i];

    for (int i = 0; i < studentCount - 1; i++) {
        for (int j = 0; j < studentCount - i - 1; j++) {
            if (sorted[j].average < sorted[j + 1].average) {
                struct Student tmp = sorted[j];
                sorted[j] = sorted[j + 1];
                sorted[j + 1] = tmp;
            }
        }
    }

    printf("\n--- Ranking (highest average first) ---");
    printHeader();
    for (int i = 0; i < studentCount; i++) printStudent(&sorted[i]);
}

/* ---------- Main menu ---------- */

int main(void) {
    int choice;

    printf("==========================================\n");
    printf("   STUDENT MARKS & GRADE MANAGEMENT\n");
    printf("==========================================\n");
    printf("Grading: A>=90, B>=75, C>=60, D>=40, else F\n");

    do {
        printf("\n------------- MENU -------------\n");
        printf("1. Add student\n");
        printf("2. Display all students\n");
        printf("3. Search student by name\n");
        printf("4. Class statistics\n");
        printf("5. Rank students by average\n");
        printf("6. Exit\n");
        choice = readInt("Enter your choice (1-6): ", 1, 6);

        switch (choice) {
            case 1: addStudent();      break;
            case 2: displayAll();      break;
            case 3: searchStudent();   break;
            case 4: classStatistics(); break;
            case 5: sortByAverage();   break;
            case 6: printf("\nGoodbye!\n"); break;
        }
    } while (choice != 6);

    return 0;
}
