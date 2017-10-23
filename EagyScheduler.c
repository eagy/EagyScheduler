////////////////////////////////////////////////////////////////////////////////
//INCLUDES
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
////////////////////////////////////////////////////////////////////////////////
//MACROS: CONSTANTS


////////////////////////////////////////////////////////////////////////////////
//DATA STRUCTURES
typedef enum {
    SER = 0, EGR, CSE, EEE
} Subject;

struct Course {
    Subject subject;
    int number;
    char teacher[1024];
    int creditHours;
};
////////////////////////////////////////////////////////////////////////////////
//GLOBAL VARIABLES
int totalCredits = 0;

//place to store course information
struct Course* CourseCollection = NULL;

//number of courses in the collection. also the index of the next empty element.
int courseCount = 0;

////////////////////////////////////////////////////////////////////////////////
//FORWARD DECLARATION
int contains(Subject sub, int num); 
void swap(struct Course *a, struct Course *b);
void course_sort();
void course_insert();
void schedule_print();
void course_drop();
void branching(char option);


//main entry point. Starts the program by displaying a welcome and beginning an 
//input loop that displays a menu and processes user input. Pressing q quits.      

int main() {
    char input_buffer;

    printf("\n\nWelcome to ASU Class Schedule\n");

    //menu and input loop
    do {
        printf("\nMenu Options\n");
        printf("------------------------------------------------------\n");
        printf("a: Add a class\n");
        printf("d: Drop a class\n");
        printf("s: Show your classes\n");
        printf("q: Quit\n");
        printf("\nTotal Credits: %d\n\n", totalCredits);
        printf("Please enter a choice ---> ");

        scanf(" %c", &input_buffer);

        branching(input_buffer);
    } while (input_buffer != 'q');

    free(CourseCollection);

    return 0;
}

// contains returns the index of the element. 
// returns -1 if element is not contained
int contains(Subject sub, int num) {
    int i;
    
    for(i = 0; i < courseCount; i++) {
        if(CourseCollection[i].subject == sub &&
                CourseCollection[i].number == num)
            return i;
           
    }
    
    return -1;
}

void swap(struct Course *a, struct Course *b) {
    //temp struct to hold data
    struct Course tmp;
    
    tmp.creditHours=a->creditHours;
    tmp.number=a->number;
    tmp.subject=a->subject;
    strcpy(tmp.teacher, a->teacher);
    
    a->creditHours=b->creditHours;
    a->number=b->number;
    a->subject=b->subject;
    strcpy(a->teacher, b->teacher);
    
    b->creditHours=tmp.creditHours;
    b->number=tmp.number;
    b->subject=tmp.subject;
    strcpy(b->teacher, tmp.teacher);
}

void course_sort() {
    int i, j, min;
    for (i = 0; i < courseCount; i++) {
        min = i;
        for (j = i+1; j < courseCount; j++) {
            if(CourseCollection[i].number > CourseCollection[j].number)
                min = j;
        }
        swap(&CourseCollection[min], &CourseCollection[i]);
    }
}

void course_insert() {
    int intInput;
    char charInput[1024];

    //if CourseCollection is empty, then it allocates enough memory for one Course
    //if CourseCollection has elements in it, then it allocates enough memory to
    //add another course.
    if (CourseCollection == NULL)
        CourseCollection = (struct Course*) malloc(++courseCount * sizeof (struct Course));
    else
        CourseCollection = (struct Course*) realloc(CourseCollection, ++courseCount * sizeof (struct Course));

    printf("\nWhat is the subject? (SER=0, EGR=1, CSE=2, EEE=3)\n");
    scanf("%d", &intInput);
    CourseCollection[courseCount-1].subject = intInput;

    printf("What is the number (e.g. 240)?\n");
    scanf("%d", &intInput);
    CourseCollection[courseCount-1].number = intInput;

    printf("How many credits is the class (e.g. 3)?\n");
    scanf("%d", &intInput);
    CourseCollection[courseCount-1].creditHours = intInput;

    printf("What is the name of the teacher?\n");
    scanf("%s", charInput);
    strcpy(CourseCollection[courseCount-1].teacher, charInput);

    totalCredits += CourseCollection[courseCount-1].creditHours;
    
    course_sort();
}

void schedule_print() {
    printf("Class Schedule:\n");
    int i;
    char *tmp;
    
    for (i = 0; i < courseCount; i++) {
        switch ((CourseCollection + i)->subject) {
            case 0:
                tmp = "SER";
                break;
            case 1:
                tmp = "EGR";
                break;
            case 2:
                tmp = "CSE";
                break;
            case 3:
                tmp = "EEE";
                break;
            default:
                printf("Error: Invalid Course Code.");
                break;
        }

        printf("%s%d\t%d\t%s\n", tmp,
                (CourseCollection + i)->number,
                (CourseCollection + i)->creditHours,
                (CourseCollection + i)->teacher);
    }
}

void course_drop() {
    int num;
    Subject sub;
    
    
    printf("\nWhat is the subject? (SER=0, EGR=1, CSE=2, EEE=3)\n");
    scanf("%d", &sub);

    printf("What is the number (e.g. 240)?\n");
    scanf("%d", &num);
    
    int index = contains(sub, num);
    if(index >= 0) {  
        totalCredits -= CourseCollection[index].creditHours;
        swap(&CourseCollection[index], &CourseCollection[courseCount-1]);
        struct Course temp[--courseCount]; \

        int i;
        for (i = 0; i < courseCount; i++) {
            temp[i].creditHours = CourseCollection[i].creditHours;
            temp[i].number = CourseCollection[i].number;
            temp[i].subject = CourseCollection[i].subject;
            strcpy(temp[i].teacher, CourseCollection[i].teacher);
        }
        
        free(CourseCollection);
        CourseCollection = (struct Course*) malloc(courseCount * sizeof (struct Course));
        
        for (i = 0; i < courseCount; i++) {
            CourseCollection[i].creditHours = temp[i].creditHours;
            CourseCollection[i].number = temp[i].number;
            CourseCollection[i].subject = temp[i].subject;
            strcpy(CourseCollection[i].teacher, temp[i].teacher);
        }
        

        //CourseCollection = (struct Course*) realloc(CourseCollection, --courseCount * sizeof (struct Course));
        
        course_sort();
    }
    else 
        printf("ERROR: This course does not exist.");
}




//takes a character representing an inputs menu choice and calls the appropriate
//function to fulfill that choice. display an error message if the character is
//not recognized.

void branching(char option) {


    switch (option) {
        case 'a':
            //TODO
            course_insert();
            break;

        case 'd':
            //TODO
            course_drop();
            break;

        case 's':
            //TODO
            schedule_print();
            break;

        case 'q':
            // main loop will take care of this.
            break;

        default:
            printf("\nError: Invalid Input.  Please try again...");
            break;
    }

}