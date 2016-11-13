typedef unsigned long uint32_t;
/*
typedef struct {
	GtkWidget* window;
	GtkWidget* box;
	char* input;
} threadArgs;

typedef struct {
	GtkWidget* box;
	GtkWidget* button;
} containerArgs;
*/


typedef struct { //buttonStruct containing an array of buttons and occupation status
	GtkWidget** buttonArray; //array of GtkWidget* pointers :D
	uint32_t occupation;
	uint32_t size;
} buttonArrayStruct;