#ifndef STUDENT_H
#define STUDENT_H

#define DATA_FILE "students.dat"
#define NAME_LENGTH 80
#define COURSE_LENGTH 60

typedef struct {
    int id;
    char name[NAME_LENGTH];
    int age;
    char course[COURSE_LENGTH];
    float marks;
} Student;

void add_student(void);
void view_students(void);
void search_student(void);
void update_student(void);
void delete_student(void);
void clear_input_buffer(void);

#endif
