#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "student.h"

void clear_input_buffer(void) {
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF) { }
}

static int read_line(const char *prompt, char *buffer, size_t size) {
    printf("%s", prompt);
    if (!fgets(buffer, (int)size, stdin)) return 0;

    size_t len = strlen(buffer);
    if (len > 0 && buffer[len - 1] == '\n') {
        buffer[len - 1] = '\0';
    } else {
        clear_input_buffer();
    }
    return 1;
}

static int read_int(const char *prompt, int *value) {
    char line[100], extra;
    if (!read_line(prompt, line, sizeof(line))) return 0;
    return sscanf(line, " %d %c", value, &extra) == 1;
}

static int read_float(const char *prompt, float *value) {
    char line[100], extra;
    if (!read_line(prompt, line, sizeof(line))) return 0;
    return sscanf(line, " %f %c", value, &extra) == 1;
}

static int id_exists(int id) {
    FILE *fp = fopen(DATA_FILE, "rb");
    Student s;
    if (!fp) return 0;

    while (fread(&s, sizeof(Student), 1, fp) == 1) {
        if (s.id == id) {
            fclose(fp);
            return 1;
        }
    }
    fclose(fp);
    return 0;
}

static void print_student(const Student *s) {
    printf("%-8d %-24s %-6d %-20s %7.2f\n",
           s->id, s->name, s->age, s->course, s->marks);
}

void add_student(void) {
    Student s;
    FILE *fp;

    if (!read_int("Enter student ID: ", &s.id) || s.id <= 0) {
        printf("Please enter a valid positive ID.\n");
        return;
    }
    if (id_exists(s.id)) {
        printf("A student with that ID already exists.\n");
        return;
    }
    if (!read_line("Enter student name: ", s.name, sizeof(s.name)) ||
        s.name[0] == '\0') {
        printf("Name cannot be empty.\n");
        return;
    }
    if (!read_int("Enter age: ", &s.age) || s.age < 1 || s.age > 120) {
        printf("Please enter a valid age.\n");
        return;
    }
    if (!read_line("Enter course: ", s.course, sizeof(s.course)) ||
        s.course[0] == '\0') {
        printf("Course cannot be empty.\n");
        return;
    }
    if (!read_float("Enter marks (0-100): ", &s.marks) ||
        s.marks < 0.0f || s.marks > 100.0f) {
        printf("Marks must be between 0 and 100.\n");
        return;
    }

    fp = fopen(DATA_FILE, "ab");
    if (!fp) {
        perror("Unable to open data file");
        return;
    }
    if (fwrite(&s, sizeof(Student), 1, fp) != 1) {
        printf("Could not save the student record.\n");
    } else {
        printf("Student added successfully.\n");
    }
    fclose(fp);
}

void view_students(void) {
    FILE *fp = fopen(DATA_FILE, "rb");
    Student s;
    int found = 0;

    if (!fp) {
        printf("No records found yet.\n");
        return;
    }

    printf("\n%-8s %-24s %-6s %-20s %7s\n",
           "ID", "NAME", "AGE", "COURSE", "MARKS");
    printf("---------------------------------------------------------------------\n");
    while (fread(&s, sizeof(Student), 1, fp) == 1) {
        print_student(&s);
        found = 1;
    }
    if (!found) printf("No records found.\n");
    fclose(fp);
}

void search_student(void) {
    int id, found = 0;
    Student s;
    FILE *fp;

    if (!read_int("Enter student ID to search: ", &id)) {
        printf("Invalid ID.\n");
        return;
    }
    fp = fopen(DATA_FILE, "rb");
    if (!fp) {
        printf("No records found yet.\n");
        return;
    }

    while (fread(&s, sizeof(Student), 1, fp) == 1) {
        if (s.id == id) {
            printf("\n%-8s %-24s %-6s %-20s %7s\n",
                   "ID", "NAME", "AGE", "COURSE", "MARKS");
            print_student(&s);
            found = 1;
            break;
        }
    }
    fclose(fp);
    if (!found) printf("Student ID %d was not found.\n", id);
}

void update_student(void) {
    int id, found = 0;
    Student s;
    FILE *fp;

    if (!read_int("Enter student ID to update: ", &id)) {
        printf("Invalid ID.\n");
        return;
    }
    fp = fopen(DATA_FILE, "rb+");
    if (!fp) {
        printf("No records found yet.\n");
        return;
    }

    while (fread(&s, sizeof(Student), 1, fp) == 1) {
        if (s.id == id) {
            printf("Updating record for %s.\n", s.name);

            if (!read_line("Enter new name: ", s.name, sizeof(s.name)) ||
                s.name[0] == '\0' ||
                !read_int("Enter new age: ", &s.age) ||
                s.age < 1 || s.age > 120 ||
                !read_line("Enter new course: ", s.course, sizeof(s.course)) ||
                s.course[0] == '\0' ||
                !read_float("Enter new marks (0-100): ", &s.marks) ||
                s.marks < 0.0f || s.marks > 100.0f) {
                printf("Invalid details. Record was not updated.\n");
                fclose(fp);
                return;
            }

            if (fseek(fp, -(long)sizeof(Student), SEEK_CUR) != 0 ||
                fwrite(&s, sizeof(Student), 1, fp) != 1) {
                printf("Unable to update the record.\n");
            } else {
                printf("Student record updated successfully.\n");
            }
            found = 1;
            break;
        }
    }
    fclose(fp);
    if (!found) printf("Student ID %d was not found.\n", id);
}

void delete_student(void) {
    int id, found = 0;
    Student s;
    FILE *fp, *temp;

    if (!read_int("Enter student ID to delete: ", &id)) {
        printf("Invalid ID.\n");
        return;
    }
    fp = fopen(DATA_FILE, "rb");
    if (!fp) {
        printf("No records found yet.\n");
        return;
    }
    temp = fopen("students.tmp", "wb");
    if (!temp) {
        perror("Unable to create temporary file");
        fclose(fp);
        return;
    }

    while (fread(&s, sizeof(Student), 1, fp) == 1) {
        if (s.id == id) {
            found = 1;
        } else if (fwrite(&s, sizeof(Student), 1, temp) != 1) {
            printf("Error writing temporary data file.\n");
            fclose(fp);
            fclose(temp);
            remove("students.tmp");
            return;
        }
    }

    fclose(fp);
    fclose(temp);

    if (!found) {
        remove("students.tmp");
        printf("Student ID %d was not found.\n", id);
        return;
    }
    if (remove(DATA_FILE) != 0 || rename("students.tmp", DATA_FILE) != 0) {
        printf("Unable to replace the data file. Check file permissions.\n");
        return;
    }
    printf("Student record deleted successfully.\n");
}
