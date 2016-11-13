/* Author: D.N.F. Brouwer
 * Date: 11-10-2016
 * Last revision: 13-11-2016
 */

#include <gtk/gtk.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>
#include <structures.h>
#include <stdlib.h> //for system()
#include <gdk/gdkx.h>

//global vars
GtkWidget* window; //main window
GtkWidget* box; //box inside the main window

GtkWidget* buttonArray; //array containing all buttons
uint32_t occupation; //bits indicate if place is free or not
buttonArrayStruct buttons; //struct holding the buttons and occupation

int barHeight; //the height of the dock/bar

//find the index of a button with specific label
int indexOfLabel(char* label)
{
	int i;
	for(i=0;i<buttons.size;i++)
	{
		if((buttons.occupation & (1 << i)) && strcmp((char*)gtk_button_get_label(buttons.buttonArray[i]),label)==0) //check if button with that label exist and it is in occupation status
		{
			return i; //return the index of the found button
		}
	}
	
	printf("label not found!\n");
	return -1; //return an impossible index value
}

//get the window back from the scratchpad!
void getScratchpad(void* button)
{
	char* lbl = gtk_button_get_label((GtkWidget*)button);
	char* idx = strchr(lbl,':')+1; //get id only
	char* tmp = "../bash/getScratchpad ";
	
	char* cmd = malloc(sizeof(char)*(strlen(idx)+strlen(tmp))); //malloc enough to hold the command string :)
	strcpy(cmd, tmp);
	strcat(cmd, idx);
	printf("cmd: %s",cmd);
	system(cmd);
	
	removeButton(indexOfLabel(lbl)); //remove the button
	
	free(cmd); //free the mem
}

void moveScratchpad(void* button)
{
	char* lbl = gtk_button_get_label((GtkWidget*)button);
	char* idx = strchr(lbl,':')+1; //get id only
	char* tmp = "../bash/moveScratchpad ";
	
	char* cmd = malloc(sizeof(char)*(strlen(idx)+strlen(tmp))); //malloc enough to hold the command string :)
	strcpy(cmd, tmp);
	strcat(cmd, idx);
	printf("cmd: %s",cmd);
	system(cmd);
		
	free(cmd); //free the mem
}

//add button to the container
void addContainer(void* button)
{
	//add the button to the container and show the button
	gtk_container_add (GTK_CONTAINER (box), (GtkWidget*)button);
	gtk_widget_show ((GtkWidget*)button);
	
	//callback on click of the button
	g_signal_connect ((GtkWidget*)button, "clicked", G_CALLBACK (getScratchpad), (GtkWidget*)button);
	
	g_idle_remove_by_data (addContainer); //removing myself from the mainloop :D (because it will loop indef otherwise adding the samebutton everytime)
}

//remove button from the container
void removeContainer(void* button)
{
	//remove the button from the container 
	gtk_container_remove (box, (GtkWidget*)button);
	
	//gtk_widget_hide((GtkWidget*)button);
	
	//gtk_widget_hide ((GtkWidget*)button); //hide it
	
	g_idle_remove_by_data (removeContainer); //removing myself from the mainloop :D (because it will loop indef otherwise adding the samebutton everytime)
}

//add button to the array
int addButton(GtkWidget* button)
{
	//gtk_button_set_relief(button,GTK_RELIEF_NONE);
	//gtk_label_set_markup(GTK_LABEL(gtk_button_get_label(button)), "<i>font='3.5'</i>");
	int i;
	for(i=0;i<buttons.size;i++)
	{
		if(!(buttons.occupation & (1 << i))) //if button place i is not yet occupied
		{
			buttons.occupation |= (1<<i); //set place i as occupied
			buttons.buttonArray[i]=button; //add the button pointer to the array
			moveScratchpad(button); //move the program to the scratchpad!
			
			g_idle_add(addContainer,buttons.buttonArray[i]); //schedule a add container in the main loop
			
			//AWESOME!!!
			
			return 0; //succes
		}
	}
	printf("Can't hold any more items!\n");
	return 1; //error}
}

//remove button from the array
void removeButton(int idx)
{
	buttons.occupation &= ~(1 << idx); //set place idx as unoccupied
	g_idle_add(removeContainer, buttons.buttonArray[idx]); //schedule a remove container in the main loop
	return 0;

}

//wait for input from i3 through tmux!
void* wfinput(void* args)
{	
	char* c_input = malloc(64 * sizeof(char)); // max 64 character string
	char* p_name = malloc(64 * sizeof(char)); //name of the process (only use for checking i3Minimize!)
	
	while(1){
		printf("input:\n");
		scanf("%s", c_input);
		printf("%s\n", c_input);
		
		//only used in case that the user wants to hide i3Minimize itself:
		strncpy(p_name, c_input, 10); //get first 10 char of the input
		if(strcmp(p_name,"i3Minimize")==0)//if user wants to hide the i3Minimize in itself, stop it!
		{
			printf("can't hide myself!\n");
		}else
		{
			int idx=indexOfLabel(c_input);//get the index of the button
			if(idx<0) //check if button with that label does not exist
			{
				//if it does not exist try to add it
				addButton(gtk_button_new_with_label(c_input)); //create button and add it to the buttons struct
			}else
			{
				removeButton(idx); //else remove it!
			}
		}
	}
	//the function must return something 
	return NULL;
}

//prompt on exit
void closeDialog(void* args)
{
	GtkWidget * dialog;
	GtkDialogFlags flags = GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT;
	
	//create a new dialog with confirmation buttons
	dialog = gtk_dialog_new_with_buttons
	(	"Quit?",
		window,
		flags,
		("_OK"),
		GTK_RESPONSE_ACCEPT,
		("_Cancel"),
		GTK_RESPONSE_REJECT,
		NULL
	);
	
	//spawn the dialog and ask if user really wants to quit the application
	gint result = gtk_dialog_run (GTK_DIALOG (dialog)); //run the dialog and read the input
	switch (result)
	{
	case GTK_RESPONSE_ACCEPT: //if the user wants to quit
		gtk_main_quit(); //quit the app
		break;
    default:
		gtk_widget_destroy (dialog); //destroy the dialog widget
		gtk_widget_show(window); //show main window again
		break;
  }
}

int main(int argc, char* argv[])
{
	int status; //status indicator
	barHeight = 0; // height of the dock/bar
	//initialize the global vars:
	buttonArray = malloc(sizeof(GtkWidget*)*32); //array containing all button pointers
	occupation = 0x00; //bits indicate if place is free or not
	
	//fill the struct holding the buttons and occupation:
	buttons.buttonArray = buttonArray;
	buttons.occupation = occupation;
	buttons.size = 32;
    
	gtk_init(&argc, &argv);

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	box = gtk_button_box_new (GTK_ORIENTATION_HORIZONTAL);

	int width = gdk_screen_get_width (gdk_screen_get_default ()); //get screen width
	gtk_window_set_default_size(window, width, barHeight); //set dimensions
	gtk_widget_set_size_request(window, width, barHeight); //set dimensions (hard)
	gtk_window_set_type_hint(GTK_WINDOW (window), GDK_WINDOW_TYPE_HINT_DOCK); //set window as a dock
	gtk_window_set_title (GTK_WINDOW (window), "i3Minimize");
	gtk_window_set_resizable (GTK_WINDOW (window), false);
    gtk_window_set_decorated(window, false);
		
	//gtk_widget_modify_font(window, pango_font_description_from_string("DejaVu Sans 8.0")); //change overall font size

	//window color settings:
	const GdkRGBA windowColor = {0.5,0.5,0.5,0.5}; //RGBA
	
	GtkCssProvider* cssProvider = gtk_css_provider_new();
    GError* error = NULL; //important!

	char* file = "../bin/style.css";
	gtk_css_provider_load_from_path(cssProvider, (gchar*)file, &error);
	
	if( error != NULL )
	{
		printf("Error: %s\n",error->message);
		g_clear_error (&error);
	}

	gtk_style_context_add_provider_for_screen(gdk_screen_get_default(),GTK_STYLE_PROVIDER(cssProvider),GTK_STYLE_PROVIDER_PRIORITY_USER);
	
	
	
	gtk_widget_override_background_color (window,GTK_STATE_FLAG_NORMAL,&windowColor);
	

	g_signal_connect(window, "delete_event", closeDialog, NULL); //call function closeDialog if user tries to exit the program
	
	gtk_container_add(GTK_CONTAINER(window),box); //add the box to the window
	
	gtk_window_stick(window); //window visible on all workspaces yeah!
	
	//show the window
	gtk_widget_show(window);
	
	//show the box
	gtk_widget_show(box);
	
	gtk_button_box_set_layout (box,GTK_BUTTONBOX_EXPAND); //let the buttons expand the buttonbox

    pthread_t inputThread; // reference to the thread
    
    if(pthread_create(&inputThread, NULL, wfinput, NULL)) {
		fprintf(stderr, "Error creating thread\n");
		return 1;
    }
	
	//gtk_widget_show_all (window);

    gtk_main();

    return 0;
}