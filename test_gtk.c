/* going to use gtk3 instead since have more documentation*/
#include <gtk/gtk.h>
#include <math.h>
#include <stdlib.h>

// Function declaration
int checkWin(int board[9]);
void placeX(int a);
void placeO(int b);

// Global Variables
int board[] = {0, 0, 0, 0, 0, 0, 0, 0, 0};

void updateLabel(GtkLabel *label, int num)
{
    gchar *display;
    display = g_strdup_printf("%d", num);         //convert num to str
    gtk_label_set_text (GTK_LABEL(label), display); //set label to "display"
    g_free(display);                              //free display
}

// function to update the grid when the player selects a space on the grid
static void place_move (GtkButton *button, GtkBuilder *builder, int position)
{
    GtkLabel *label = gtk_builder_get_object (builder, "turn");
    GtkLabel *a_label = gtk_builder_get_object (builder, "announcement");
    int check;

    int label_turn = atoi(gtk_label_get_text(label));
    const char * const text = gtk_button_get_label (button); // get the label of the button clicked

    if (strlen(text) == 0) { // no label means splace is clear
        if (label_turn % 2 == 0){ // if the turn is even is player O turn

            gtk_button_set_label (button, "O");
            placeO(position);
            // check if player O win
            check = checkWin(board);
            if (check == 1){ // if O wins
                gtk_label_set_text (GTK_LABEL(a_label), "O WINS! You may now exit the game"); // set for the next turn which is player X
                // reset board
                for (int i = 0; i < 9; i++){
                    board[i] = 0;
                }
            } else {
                gtk_label_set_text (GTK_LABEL(a_label), "Player X Turn"); // set for the next turn which is player X
            }


        } else { // else is player X turn

            gtk_button_set_label (button, "X");
            placeX(position);
            // check if player X win
            check = checkWin(board);
            if (check == 1){ // if O wins
                gtk_label_set_text (GTK_LABEL(a_label), "X WINS! You may now exit the game"); // set for the next turn which is player X
                // reset board
                for (int i = 0; i < 9; i++){
                    board[i] = 0;
                }
            } else {
                gtk_label_set_text (GTK_LABEL(a_label), "Player O Turn"); // set for the next turn which is player X
            }

        }

        label_turn += 1;
        if (label_turn == 10){ // after user has made their final move and it is a draw, since turn starts from 1 as well
            gtk_label_set_text (GTK_LABEL(a_label), "DRAW! You may now exit the game"); 
            // reset board
            for (int i = 0; i < 9; i++){
                board[i] = 0;
            }
        } else {
            updateLabel(label, label_turn);
        }

    } else {
        // update error message
        gtk_label_set_text (GTK_LABEL(a_label), "Space taken! Please select another space...");
        g_print("\nSpace taken! Please select another space...");
    }

}

// Identify which position the player has clicked
static void position_1 (GtkButton *button, GtkBuilder *builder) {
    int position = 1;
    place_move (button, builder, position);
}
static void position_2 (GtkButton *button, GtkBuilder *builder) {
    int position = 2;
    place_move (button, builder, position);
}
static void position_3 (GtkButton *button, GtkBuilder *builder) {
    int position = 3;
    place_move (button, builder, position);
}
static void position_4 (GtkButton *button, GtkBuilder *builder) {
    int position = 4;
    place_move (button, builder, position);
}
static void position_5 (GtkButton *button, GtkBuilder *builder) {
    int position = 5;
    place_move (button, builder, position);
}
static void position_6 (GtkButton *button, GtkBuilder *builder) {
    int position = 6;
    place_move (button, builder, position);
}
static void position_7 (GtkButton *button, GtkBuilder *builder) {
    int position = 7;
    place_move (button, builder, position);
}
static void position_8 (GtkButton *button, GtkBuilder *builder) {
    int position = 8;
    place_move (button, builder, position);
}
static void position_9 (GtkButton *button, GtkBuilder *builder) {
    int position = 9;
    place_move (button, builder, position);
}

void single_player(GtkWidget *p_widget, gpointer user_data)
{
    gtk_button_released(p_widget);
    GtkWidget *p_window;
    GtkWidget *p_v_box;
    GtkWidget *p_entry;

    g_print("single player selected!\n");

    p_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(p_window), "Single Player");
    gtk_window_set_default_size(GTK_WINDOW(p_window), 300, 300);

    gtk_widget_show_all(p_window);
}

void two_player(GtkWidget *p_widget, gpointer user_data)
{
    gtk_button_released(p_widget);
    GtkWidget *p_window;
    GtkWidget *p_v_box;
    GtkWidget *p_entry;
    GtkBuilder *builder;
    GObject *button_1, *button_2, *button_3, *button_4, *button_5, *button_6, *button_7, *button_8, *button_9, *turn_label, *announcement_label;
    GError *error = NULL;

      /* Construct a GtkBuilder instance and load our UI description */
  
    builder = gtk_builder_new ();
    if (gtk_builder_add_from_file (builder, "builder.ui", &error) == 0)
    {
        g_printerr ("Error loading file: %s\n", error->message);
        g_clear_error (&error);
        return 1;
    }

    g_print("\ntwo player selected!\n");

    // p_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    // gtk_window_set_title(GTK_WINDOW(p_window), "Two Player");
    // gtk_window_set_default_size(GTK_WINDOW(p_window), 300, 300);

    p_window = gtk_builder_get_object (builder, "window");
    // g_signal_connect (p_window, "destroy", G_CALLBACK (gtk_main_quit), NULL);
    

    // Set buttons
    button_1 = gtk_builder_get_object (builder, "button1");
    button_2 = gtk_builder_get_object (builder, "button2");
    button_3 = gtk_builder_get_object (builder, "button3");
    button_4 = gtk_builder_get_object (builder, "button4");
    button_5 = gtk_builder_get_object (builder, "button5");    
    button_6 = gtk_builder_get_object (builder, "button6");
    button_7 = gtk_builder_get_object (builder, "button7");
    button_8 = gtk_builder_get_object (builder, "button8");
    button_9 = gtk_builder_get_object (builder, "button9");

    // button is clicked, check with update_button (pass in label, placement and error)
    g_signal_connect (button_1, "clicked", G_CALLBACK (position_1), builder);
    g_signal_connect (button_2, "clicked", G_CALLBACK (position_2), builder);
    g_signal_connect (button_3, "clicked", G_CALLBACK (position_3), builder);
    g_signal_connect (button_4, "clicked", G_CALLBACK (position_4), builder);
    g_signal_connect (button_5, "clicked", G_CALLBACK (position_5), builder);
    g_signal_connect (button_6, "clicked", G_CALLBACK (position_6), builder);
    g_signal_connect (button_7, "clicked", G_CALLBACK (position_7), builder);
    g_signal_connect (button_8, "clicked", G_CALLBACK (position_8), builder);
    g_signal_connect (button_9, "clicked", G_CALLBACK (position_9), builder);

    // cannot loop just whenever the player clicks on the button that is valid the turn will increase
    // cannot loop since the funtion will run all thats in the function


    gtk_widget_show_all(p_window);
}

int main_window(int argc, char **argv){
   GtkWidget *window;
   GtkWidget *button_1;
   GtkWidget *button_2;
   GtkWidget *label;
   GtkWidget *grid;

   gtk_init (&argc, &argv);

   //Declaration
   window = gtk_window_new(GTK_WINDOW_TOPLEVEL); // Create window
   label = gtk_label_new("\n#################\n   Tic-Tac-Toe  \n#################\n=== New Game ===\n");
   gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_CENTER);
   grid = gtk_grid_new();
   button_1 = gtk_button_new_with_label("1. Single Player");
   button_2 = gtk_button_new_with_label("2. Two Player");

   // Set Properties
   gtk_window_set_title(GTK_WINDOW(window), "Tic Tac Toe");
   gtk_window_set_default_size(GTK_WINDOW(window), 300, 300);
   gtk_grid_set_row_spacing       (GTK_GRID(grid), 4);
   gtk_grid_set_column_spacing    (GTK_GRID(grid), 4);
   gtk_container_add              (GTK_CONTAINER(window), grid);

   // Fill grid
   gtk_grid_attach(GTK_GRID(grid), label, 1, 0, 2, 1);
   gtk_grid_attach(GTK_GRID(grid), button_1, 1, 1, 2, 1);
   gtk_grid_attach(GTK_GRID(grid), button_2, 1, 2, 2, 1);

   gtk_widget_show_all(window);

   g_signal_connect(G_OBJECT(button_1), "clicked", G_CALLBACK(single_player), NULL);
   g_signal_connect(G_OBJECT(button_2), "clicked", G_CALLBACK(two_player), NULL);

   return 0;
}

int main (int argc, char *argv[]) {
   main_window(argc, argv);
   gtk_main ();
   return 0;
}

// Functions used taken from main - some may have been updated slightly to fit the gui

// Checks board inputs if there are any win conditions detected
int checkWin(int board[9])
{
    // Check Rows for Horizontal wins
    for (int i = 0; i < 3; i++)
    {
        if (board[3 * i] == board[3 * i + 1] && board[3 * i + 1] == board[3 * i + 2])
        {
            return board[3 * i];
        }
    }

    // Check Columns for Vertical wins
    for (int j = 0; j < 3; j++)
    {
        if (board[j] == board[3 + j] && board[3 + j] == board[6 + j])
        {
            return board[j];
        }
    }

    // Check for Diagonal wins
    if (board[0] == board[4] && board[0] == board[8])
    {
        return board[0];
    }

    if (board[2] == board[4] && board[2] == board[6])
    {
        return board[2];
    }

    return 0;
}

// Put 1 which translates to X on the board
void placeX(int a) // changed to fit gui
{
    if (a >= 1 && a <= 9)
    {
        if (board[a - 1] == 0)
        {
            board[a - 1] = 1;
        }
    }
}

// Put -1 which translates to O on the board
void placeO(int b) // changed to fit gui
{

    if (b >= 1 && b <= 9)
    {
        if (board[b - 1] == 0)
        {
            board[b - 1] = -1;
        }
    }

}