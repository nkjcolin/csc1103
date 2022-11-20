/* going to use gtk3 instead since have more documentation*/
#include <gtk/gtk.h>
#include <math.h>
#include <stdlib.h>

#define MIN_SCORE -100
#define MAX_SCORE 100

// Function declaration
int checkWin(int board[9]);
void placeX(int a);
void placeO(int b);
int levelDifficulty(float botWins, float gamesPlayed);

// Global Variables
int board[] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
int player = 0;
float gamesPlayed = 0;
float botWins = 0;

void updateLabel(GtkLabel *label, int num)
{
    gchar *display;
    display = g_strdup_printf("%d", num);          // convert num to str
    gtk_label_set_text(GTK_LABEL(label), display); // set label to "display"
    g_free(display);                               // free display
}

// 2 player mode
// function to update the grid when the player selects a space on the grid for 2 player mode
static void place_move(GtkButton *button, GtkBuilder *builder, int position)
{
    g_print("\n");
    GtkLabel *label = gtk_builder_get_object(builder, "turn");
    GtkLabel *a_label = gtk_builder_get_object(builder, "announcement");
    int check;

    int label_turn = atoi(gtk_label_get_text(label));
    const char *const text = gtk_button_get_label(button); // get the label of the button clicked

    if (strlen(text) == 0)
    { // no label means splace is clear
        if (label_turn % 2 == 0)
        { // if the turn is even is player O turn

            gtk_button_set_label(button, "O");
            placeO(position);
            // check if player O win
            check = checkWin(board);
            if (check == -1)
            { // if O wins
                gtk_label_set_text(GTK_LABEL(a_label), "O won! You may now exit the game.");
                // reset board
                for (int i = 0; i < 9; i++)
                {
                    board[i] = 0;
                }
            }
            else
            {
                gtk_label_set_text(GTK_LABEL(a_label), "X's turn."); // set for the next turn which is player X
            }
        }
        else
        { // else is player X turn

            gtk_button_set_label(button, "X");
            placeX(position);
            // check if player X win
            check = checkWin(board);
            if (check == 1)
            { // if X wins
                gtk_label_set_text(GTK_LABEL(a_label), "X won! You may now exit the game.");
                // reset board
                for (int i = 0; i < 9; i++)
                {
                    board[i] = 0;
                }
            }
            else
            {
                gtk_label_set_text(GTK_LABEL(a_label), "O's Turn"); // set for the next turn which is player X
            }
        }
        label_turn += 1;
        if (label_turn == 10)
        { // after user has made their final move and it is a draw, since turn starts from 1 as well
            gtk_label_set_text(GTK_LABEL(a_label), "It's a draw! You may now exit the game");
            // reset board
            for (int i = 0; i < 9; i++)
            {
                board[i] = 0;
            }
        }
        else
        {
            updateLabel(label, label_turn);
        }
    }
    else
    {
        // update error message
        gtk_label_set_text(GTK_LABEL(a_label), "Space taken, try an empty one!");
        g_print("\nSpace taken, try an empty one!");
    }
}

// Identify which position the player has clicked
static void position_1(GtkButton *button, GtkBuilder *builder)
{
    int position = 1;
    place_move(button, builder, position);
}
static void position_2(GtkButton *button, GtkBuilder *builder)
{
    int position = 2;
    place_move(button, builder, position);
}
static void position_3(GtkButton *button, GtkBuilder *builder)
{
    int position = 3;
    place_move(button, builder, position);
}
static void position_4(GtkButton *button, GtkBuilder *builder)
{
    int position = 4;
    place_move(button, builder, position);
}
static void position_5(GtkButton *button, GtkBuilder *builder)
{
    int position = 5;
    place_move(button, builder, position);
}
static void position_6(GtkButton *button, GtkBuilder *builder)
{
    int position = 6;
    place_move(button, builder, position);
}
static void position_7(GtkButton *button, GtkBuilder *builder)
{
    int position = 7;
    place_move(button, builder, position);
}
static void position_8(GtkButton *button, GtkBuilder *builder)
{
    int position = 8;
    place_move(button, builder, position);
}
static void position_9(GtkButton *button, GtkBuilder *builder)
{
    int position = 9;
    place_move(button, builder, position);
}

// One player - minimax
// function to update the grid when the player selects a space on the grid for 1 player mode minimax
static void place_move_minimax(GtkButton *button, GtkBuilder *builder, int position)
{
    g_print("\n");

    // Get buttons so the bot can make their move
    GObject *button_1, *button_2, *button_3, *button_4, *button_5, *button_6, *button_7, *button_8, *button_9;
    button_1 = gtk_builder_get_object(builder, "button1");
    button_2 = gtk_builder_get_object(builder, "button2");
    button_3 = gtk_builder_get_object(builder, "button3");
    button_4 = gtk_builder_get_object(builder, "button4");
    button_5 = gtk_builder_get_object(builder, "button5");
    button_6 = gtk_builder_get_object(builder, "button6");
    button_7 = gtk_builder_get_object(builder, "button7");
    button_8 = gtk_builder_get_object(builder, "button8");
    button_9 = gtk_builder_get_object(builder, "button9");

    GtkLabel *label = gtk_builder_get_object(builder, "turn");
    GtkLabel *a_label = gtk_builder_get_object(builder, "announcement");
    int check;

    int label_turn = atoi(gtk_label_get_text(label));
    const char *const text = gtk_button_get_label(button); // get the label of the button clicked

    if (strlen(text) == 0)
    { // no label means splace is clear

        // player O turn
        gtk_button_set_label(button, "O");
        placeO(position);
        // check if player O win
        check = checkWin(board);
        if (check == -1)
        { // if O wins
            gtk_label_set_text(GTK_LABEL(a_label), "You won! You may now exit the game.");
            // reset board
            for (int i = 0; i < 9; i++)
            {
                board[i] = 0;
            }
        }
        else
        {
            // let AI make the play first
            int move = AImove(board) + 1;
            g_print("%d", move);

            if (move == 1)
            {
                gtk_button_set_label(button_1, "X");
            }
            else if (move == 2)
            {
                gtk_button_set_label(button_2, "X");
            }
            else if (move == 3)
            {
                gtk_button_set_label(button_3, "X");
            }
            else if (move == 4)
            {
                gtk_button_set_label(button_4, "X");
            }
            else if (move == 5)
            {
                gtk_button_set_label(button_5, "X");
            }
            else if (move == 6)
            {
                gtk_button_set_label(button_6, "X");
            }
            else if (move == 7)
            {
                gtk_button_set_label(button_7, "X");
            }
            else if (move == 8)
            {
                gtk_button_set_label(button_8, "X");
            }
            else if (move == 9)
            {
                gtk_button_set_label(button_9, "X");
            }

            // check if bot win
            check = checkWin(board);
            if (check == 1)
            { // if X wins
                gtk_label_set_text(GTK_LABEL(a_label), "Bot A won! You may now exit the game.");
                // reset board
                for (int i = 0; i < 9; i++)
                {
                    board[i] = 0;
                }
            }
            else
            {
                // increase turn after the bot has made their move
                int label_turn = atoi(gtk_label_get_text(label));
                label_turn += 2;

                if (label_turn >= 10)
                {                          // after user has made their final move and it is a draw, since turn starts from 1 as well
                    updateLabel(label, 9); // for when the bot starts first
                    gtk_label_set_text(GTK_LABEL(a_label), "It's a draw! You may now exit the game.");
                    // reset board
                    for (int i = 0; i < 9; i++)
                    {
                        board[i] = 0;
                    }
                }
                else
                { // if game has not end
                    updateLabel(label, label_turn);
                    gtk_label_set_text(GTK_LABEL(a_label), "It's your turn."); // set for the next turn after the bot has made their move
                }
            }
        }
    }
    else
    {
        // update error message
        gtk_label_set_text(GTK_LABEL(a_label), "Space taken, try an empty one!");
        g_print("\nSpace taken, try an empty one!");
    }
}

// One player - imperfect minimax
// function to update the grid when the player selects a space on the grid for 1 player mode imperfect minimax
static void place_move_imp_minimax(GtkButton *button, GtkBuilder *builder, int position)
{
    g_print("\n");

    // Get buttons so the bot can make their move
    GObject *button_1, *button_2, *button_3, *button_4, *button_5, *button_6, *button_7, *button_8, *button_9;
    button_1 = gtk_builder_get_object(builder, "button1");
    button_2 = gtk_builder_get_object(builder, "button2");
    button_3 = gtk_builder_get_object(builder, "button3");
    button_4 = gtk_builder_get_object(builder, "button4");
    button_5 = gtk_builder_get_object(builder, "button5");
    button_6 = gtk_builder_get_object(builder, "button6");
    button_7 = gtk_builder_get_object(builder, "button7");
    button_8 = gtk_builder_get_object(builder, "button8");
    button_9 = gtk_builder_get_object(builder, "button9");

    GtkLabel *label = gtk_builder_get_object(builder, "turn");
    GtkLabel *a_label = gtk_builder_get_object(builder, "announcement");
    int check;

    int label_turn = atoi(gtk_label_get_text(label));
    const char *const text = gtk_button_get_label(button); // get the label of the button clicked

    if (strlen(text) == 0)
    { // no label means splace is clear

        // player O turn
        gtk_button_set_label(button, "O");
        placeO(position);
        // check if player O win
        check = checkWin(board);
        if (check == -1)
        { // if O wins
            gtk_label_set_text(GTK_LABEL(a_label), "You won! You may now exit the game.");
            gamesPlayed++;
            // reset board
            for (int i = 0; i < 9; i++)
            {
                board[i] = 0;
            }
        }
        else
        {
            // let AI make the play first
            int move = moveAB(board) + 1;
            g_print("%d", move);

            if (move == 1)
            {
                gtk_button_set_label(button_1, "X");
            }
            else if (move == 2)
            {
                gtk_button_set_label(button_2, "X");
            }
            else if (move == 3)
            {
                gtk_button_set_label(button_3, "X");
            }
            else if (move == 4)
            {
                gtk_button_set_label(button_4, "X");
            }
            else if (move == 5)
            {
                gtk_button_set_label(button_5, "X");
            }
            else if (move == 6)
            {
                gtk_button_set_label(button_6, "X");
            }
            else if (move == 7)
            {
                gtk_button_set_label(button_7, "X");
            }
            else if (move == 8)
            {
                gtk_button_set_label(button_8, "X");
            }
            else if (move == 9)
            {
                gtk_button_set_label(button_9, "X");
            }

            // check if bot win
            check = checkWin(board);
            if (check == 1)
            { // if X wins
                gtk_label_set_text(GTK_LABEL(a_label), "Bot B won! You may now exit the game.");
                gamesPlayed++;
                botWins++;
                // reset board
                for (int i = 0; i < 9; i++)
                {
                    board[i] = 0;
                }
            }
            else
            {
                // increase turn after the bot has made their move
                int label_turn = atoi(gtk_label_get_text(label));
                label_turn += 2;

                if (label_turn >= 10)
                {                          // after user has made their final move and it is a draw, since turn starts from 1 as well
                    updateLabel(label, 9); // for when the bot starts first
                    gtk_label_set_text(GTK_LABEL(a_label), "It's a draw! You may now exit the game");
                    gamesPlayed++;
                    botWins += 0.5;
                    // reset board
                    for (int i = 0; i < 9; i++)
                    {
                        board[i] = 0;
                    }
                }
                else
                { // if game has not end
                    updateLabel(label, label_turn);
                    gtk_label_set_text(GTK_LABEL(a_label), "It's your turn."); // set for the next turn after the bot has made their move
                }
            }
        }
    }
    else
    {
        // update error message
        gtk_label_set_text(GTK_LABEL(a_label), "Space taken, try an empty one!");
        g_print("\nSpace taken, try an empty one!");
    }
}

// Identify which position the player has clicked - imperfect minimax
static void position_1_imp_minimax(GtkButton *button, GtkBuilder *builder)
{
    int position = 1;
    place_move_imp_minimax(button, builder, position);
}
static void position_2_imp_minimax(GtkButton *button, GtkBuilder *builder)
{
    int position = 2;
    place_move_imp_minimax(button, builder, position);
}
static void position_3_imp_minimax(GtkButton *button, GtkBuilder *builder)
{
    int position = 3;
    place_move_imp_minimax(button, builder, position);
}
static void position_4_imp_minimax(GtkButton *button, GtkBuilder *builder)
{
    int position = 4;
    place_move_imp_minimax(button, builder, position);
}
static void position_5_imp_minimax(GtkButton *button, GtkBuilder *builder)
{
    int position = 5;
    place_move_imp_minimax(button, builder, position);
}
static void position_6_imp_minimax(GtkButton *button, GtkBuilder *builder)
{
    int position = 6;
    place_move_imp_minimax(button, builder, position);
}
static void position_7_imp_minimax(GtkButton *button, GtkBuilder *builder)
{
    int position = 7;
    place_move_imp_minimax(button, builder, position);
}
static void position_8_imp_minimax(GtkButton *button, GtkBuilder *builder)
{
    int position = 8;
    place_move_imp_minimax(button, builder, position);
}
static void position_9_imp_minimax(GtkButton *button, GtkBuilder *builder)
{
    int position = 9;
    place_move_imp_minimax(button, builder, position);
}

void imp_minimax(GtkWidget *p_widget, int player)
{

    gtk_button_released(p_widget);
    GtkWidget *p_window;
    GtkWidget *p_v_box;
    GtkWidget *p_entry;
    GtkBuilder *builder;
    GObject *button_1, *button_2, *button_3, *button_4, *button_5, *button_6, *button_7, *button_8, *button_9, *label, *announcement_label;
    GError *error = NULL;

    // reset board for every new game played
    for (int i = 0; i < 9; i++)
    {
        board[i] = 0;
    }

    /* Construct a GtkBuilder instance and load our UI description */

    builder = gtk_builder_new();
    if (gtk_builder_add_from_file(builder, "builder.ui", &error) == 0)
    {
        g_printerr("Error loading file: %s\n", error->message);
        g_clear_error(&error);
    }

    g_print("\nsingle player minimax gameplay\n");

    p_window = gtk_builder_get_object(builder, "window");
    // g_signal_connect (p_window, "destroy", G_CALLBACK (gtk_main_quit), NULL);

    // Set buttons
    button_1 = gtk_builder_get_object(builder, "button1");
    button_2 = gtk_builder_get_object(builder, "button2");
    button_3 = gtk_builder_get_object(builder, "button3");
    button_4 = gtk_builder_get_object(builder, "button4");
    button_5 = gtk_builder_get_object(builder, "button5");
    button_6 = gtk_builder_get_object(builder, "button6");
    button_7 = gtk_builder_get_object(builder, "button7");
    button_8 = gtk_builder_get_object(builder, "button8");
    button_9 = gtk_builder_get_object(builder, "button9");

    // Label
    label = gtk_builder_get_object(builder, "turn");
    announcement_label = gtk_builder_get_object(builder, "announcement");

    // If AI goes first update the board
    if (player == 2)
    {

        // let AI make the play first
        int move = moveAB(board) + 1;
        g_print("%d", move);

        if (move == 1)
        {
            gtk_button_set_label(button_1, "X");
        }
        else if (move == 2)
        {
            gtk_button_set_label(button_2, "X");
        }
        else if (move == 3)
        {
            gtk_button_set_label(button_3, "X");
        }
        else if (move == 4)
        {
            gtk_button_set_label(button_4, "X");
        }
        else if (move == 5)
        {
            gtk_button_set_label(button_5, "X");
        }
        else if (move == 6)
        {
            gtk_button_set_label(button_6, "X");
        }
        else if (move == 7)
        {
            gtk_button_set_label(button_7, "X");
        }
        else if (move == 8)
        {
            gtk_button_set_label(button_8, "X");
        }
        else if (move == 9)
        {
            gtk_button_set_label(button_9, "X");
        }

        // increase turn after the bot has made their move
        int label_turn = atoi(gtk_label_get_text(label));
        label_turn += 1;
        updateLabel(label, label_turn);
        gtk_label_set_text(GTK_LABEL(announcement_label), "It's your turn."); // set for the next turn after the bot has made their move
    }
    else if (player == 2)
    {
        gtk_label_set_text(GTK_LABEL(announcement_label), "It's your turn.");
    }

    // button is clicked, check with update_button (pass in label, placement and error)
    g_signal_connect(button_1, "clicked", G_CALLBACK(position_1_imp_minimax), builder);
    g_signal_connect(button_2, "clicked", G_CALLBACK(position_2_imp_minimax), builder);
    g_signal_connect(button_3, "clicked", G_CALLBACK(position_3_imp_minimax), builder);
    g_signal_connect(button_4, "clicked", G_CALLBACK(position_4_imp_minimax), builder);
    g_signal_connect(button_5, "clicked", G_CALLBACK(position_5_imp_minimax), builder);
    g_signal_connect(button_6, "clicked", G_CALLBACK(position_6_imp_minimax), builder);
    g_signal_connect(button_7, "clicked", G_CALLBACK(position_7_imp_minimax), builder);
    g_signal_connect(button_8, "clicked", G_CALLBACK(position_8_imp_minimax), builder);
    g_signal_connect(button_9, "clicked", G_CALLBACK(position_9_imp_minimax), builder);

    // cannot loop just whenever the player clicks on the button that is valid the turn will increase
    // cannot loop since the funtion will run all thats in the function

    gtk_widget_show_all(p_window);
}

// Identify which position the player has clicked - minimax
static void position_1_minimax(GtkButton *button, GtkBuilder *builder)
{
    int position = 1;
    place_move_minimax(button, builder, position);
}
static void position_2_minimax(GtkButton *button, GtkBuilder *builder)
{
    int position = 2;
    place_move_minimax(button, builder, position);
}
static void position_3_minimax(GtkButton *button, GtkBuilder *builder)
{
    int position = 3;
    place_move_minimax(button, builder, position);
}
static void position_4_minimax(GtkButton *button, GtkBuilder *builder)
{
    int position = 4;
    place_move_minimax(button, builder, position);
}
static void position_5_minimax(GtkButton *button, GtkBuilder *builder)
{
    int position = 5;
    place_move_minimax(button, builder, position);
}
static void position_6_minimax(GtkButton *button, GtkBuilder *builder)
{
    int position = 6;
    place_move_minimax(button, builder, position);
}
static void position_7_minimax(GtkButton *button, GtkBuilder *builder)
{
    int position = 7;
    place_move_minimax(button, builder, position);
}
static void position_8_minimax(GtkButton *button, GtkBuilder *builder)
{
    int position = 8;
    place_move_minimax(button, builder, position);
}
static void position_9_minimax(GtkButton *button, GtkBuilder *builder)
{
    int position = 9;
    place_move_minimax(button, builder, position);
}

void minimax(GtkWidget *p_widget, int player)
{

    gtk_button_released(p_widget);
    GtkWidget *p_window;
    GtkWidget *p_v_box;
    GtkWidget *p_entry;
    GtkBuilder *builder;
    GObject *button_1, *button_2, *button_3, *button_4, *button_5, *button_6, *button_7, *button_8, *button_9, *label, *announcement_label;
    GError *error = NULL;

    // reset board for every new game played
    for (int i = 0; i < 9; i++)
    {
        board[i] = 0;
    }

    /* Construct a GtkBuilder instance and load our UI description */

    builder = gtk_builder_new();
    if (gtk_builder_add_from_file(builder, "builder.ui", &error) == 0)
    {
        g_printerr("Error loading file: %s\n", error->message);
        g_clear_error(&error);
    }

    g_print("\nsingle player minimax gameplay\n");

    p_window = gtk_builder_get_object(builder, "window");
    // g_signal_connect (p_window, "destroy", G_CALLBACK (gtk_main_quit), NULL);

    // Set buttons
    button_1 = gtk_builder_get_object(builder, "button1");
    button_2 = gtk_builder_get_object(builder, "button2");
    button_3 = gtk_builder_get_object(builder, "button3");
    button_4 = gtk_builder_get_object(builder, "button4");
    button_5 = gtk_builder_get_object(builder, "button5");
    button_6 = gtk_builder_get_object(builder, "button6");
    button_7 = gtk_builder_get_object(builder, "button7");
    button_8 = gtk_builder_get_object(builder, "button8");
    button_9 = gtk_builder_get_object(builder, "button9");

    // Label
    label = gtk_builder_get_object(builder, "turn");
    announcement_label = gtk_builder_get_object(builder, "announcement");

    // If AI goes first update the board
    if (player == 2)
    {

        // let AI make the play first
        int move = AImove(board) + 1;
        g_print("%d", move);

        if (move == 1)
        {
            gtk_button_set_label(button_1, "X");
        }
        else if (move == 2)
        {
            gtk_button_set_label(button_2, "X");
        }
        else if (move == 3)
        {
            gtk_button_set_label(button_3, "X");
        }
        else if (move == 4)
        {
            gtk_button_set_label(button_4, "X");
        }
        else if (move == 5)
        {
            gtk_button_set_label(button_5, "X");
        }
        else if (move == 6)
        {
            gtk_button_set_label(button_6, "X");
        }
        else if (move == 7)
        {
            gtk_button_set_label(button_7, "X");
        }
        else if (move == 8)
        {
            gtk_button_set_label(button_8, "X");
        }
        else if (move == 9)
        {
            gtk_button_set_label(button_9, "X");
        }

        // increase turn after the bot has made their move
        int label_turn = atoi(gtk_label_get_text(label));
        label_turn += 1;
        updateLabel(label, label_turn);
    }

    gtk_label_set_text(GTK_LABEL(announcement_label), "It's your turn."); // set for the next turn after the bot has made their move or when the user goes first


    // button is clicked, check with update_button (pass in label, placement and error)
    g_signal_connect(button_1, "clicked", G_CALLBACK(position_1_minimax), builder);
    g_signal_connect(button_2, "clicked", G_CALLBACK(position_2_minimax), builder);
    g_signal_connect(button_3, "clicked", G_CALLBACK(position_3_minimax), builder);
    g_signal_connect(button_4, "clicked", G_CALLBACK(position_4_minimax), builder);
    g_signal_connect(button_5, "clicked", G_CALLBACK(position_5_minimax), builder);
    g_signal_connect(button_6, "clicked", G_CALLBACK(position_6_minimax), builder);
    g_signal_connect(button_7, "clicked", G_CALLBACK(position_7_minimax), builder);
    g_signal_connect(button_8, "clicked", G_CALLBACK(position_8_minimax), builder);
    g_signal_connect(button_9, "clicked", G_CALLBACK(position_9_minimax), builder);

    // cannot loop just whenever the player clicks on the button that is valid the turn will increase
    // cannot loop since the funtion will run all thats in the function

    gtk_widget_show_all(p_window);
}

void imp_minimax_selectplayer(GtkWidget *p_widget, gpointer user_data)
{
    gtk_button_released(p_widget);
    GtkWidget *p_window, *label, *grid, *button_1, *button_2;

    g_print("Imperfect minimax selected!\n");

    int difficulty = levelDifficulty(botWins, gamesPlayed);

    // Declaration
    p_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);

    switch (difficulty)
    {
    case 0:
        label = gtk_label_new("\nBot: X \t You: O\n=== Who goes first? ===");
        break;
    case 1:
        label = gtk_label_new("\nDificulty: Easy\nBot: X \t You: O\n=== Who goes first? ===");
        break;
    case 2:
        label = gtk_label_new("\nDificulty: Intermediate\nBot: X \t You: O\n=== Who goes first? ===");
        break;
    case 3:
        label = gtk_label_new("\nDificulty: Hard\nBot: X \t You: O\n=== Who goes first? ===");
        break;
    }

    gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_CENTER);
    grid = gtk_grid_new();
    button_1 = gtk_button_new_with_label("You");
    button_2 = gtk_button_new_with_label("Bot B");

    // properties
    gtk_window_set_title(GTK_WINDOW(p_window), "You vs Bot B (Imperfect minimax)");
    gtk_window_set_default_size(GTK_WINDOW(p_window), 300, 300);
    gtk_grid_set_row_spacing(GTK_GRID(grid), 4);
    gtk_grid_set_column_spacing(GTK_GRID(grid), 4);
    gtk_container_add(GTK_CONTAINER(p_window), grid);

    // Fill grid
    gtk_grid_attach(GTK_GRID(grid), label, 1, 0, 2, 1);
    gtk_grid_attach(GTK_GRID(grid), button_1, 1, 1, 2, 1);
    gtk_grid_attach(GTK_GRID(grid), button_2, 1, 2, 2, 1);

    gtk_widget_show_all(p_window);

    g_signal_connect(G_OBJECT(button_1), "clicked", G_CALLBACK(imp_minimax), 1);
    g_signal_connect(G_OBJECT(button_2), "clicked", G_CALLBACK(imp_minimax), 2);
}

void minimax_selectplayer(GtkWidget *p_widget, gpointer user_data)
{
    gtk_button_released(p_widget);
    GtkWidget *p_window, *label, *grid, *button_1, *button_2;

    g_print("Minimax selected!\n");

    // Declaration
    p_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    label = gtk_label_new("\nBot: X \t You: O\n=== Who goes first? ===");
    gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_CENTER);
    grid = gtk_grid_new();
    button_1 = gtk_button_new_with_label("You");
    button_2 = gtk_button_new_with_label("Bot A");

    // properties
    gtk_window_set_title(GTK_WINDOW(p_window), "You vs Bot A (Minimax)");
    gtk_window_set_default_size(GTK_WINDOW(p_window), 300, 300);
    gtk_grid_set_row_spacing(GTK_GRID(grid), 4);
    gtk_grid_set_column_spacing(GTK_GRID(grid), 4);
    gtk_container_add(GTK_CONTAINER(p_window), grid);

    // Fill grid
    gtk_grid_attach(GTK_GRID(grid), label, 1, 0, 2, 1);
    gtk_grid_attach(GTK_GRID(grid), button_1, 1, 1, 2, 1);
    gtk_grid_attach(GTK_GRID(grid), button_2, 1, 2, 2, 1);

    gtk_widget_show_all(p_window);

    g_signal_connect(G_OBJECT(button_1), "clicked", G_CALLBACK(minimax), 1);
    g_signal_connect(G_OBJECT(button_2), "clicked", G_CALLBACK(minimax), 2);
}

void single_player(GtkWidget *p_widget, gpointer user_data)
{
    gtk_button_released(p_widget);
    GtkWidget *p_window, *label, *grid, *button_1, *button_2;

    g_print("You vs Bot selected!\n");

    // Declaration
    p_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    label = gtk_label_new("=== Opponents ===");
    gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_CENTER);
    grid = gtk_grid_new();
    button_1 = gtk_button_new_with_label("Bot A (minimax)");
    button_2 = gtk_button_new_with_label("Bot B (imperfect minimax)");

    // properties
    gtk_window_set_title(GTK_WINDOW(p_window), "You vs Bot");
    gtk_window_set_default_size(GTK_WINDOW(p_window), 300, 300);
    gtk_grid_set_row_spacing(GTK_GRID(grid), 4);
    gtk_grid_set_column_spacing(GTK_GRID(grid), 4);
    gtk_container_add(GTK_CONTAINER(p_window), grid);

    // Fill grid
    gtk_grid_attach(GTK_GRID(grid), label, 1, 0, 2, 1);
    gtk_grid_attach(GTK_GRID(grid), button_1, 1, 1, 2, 1);
    gtk_grid_attach(GTK_GRID(grid), button_2, 1, 2, 2, 1);

    gtk_widget_show_all(p_window);

    g_signal_connect(G_OBJECT(button_1), "clicked", G_CALLBACK(minimax_selectplayer), NULL);
    g_signal_connect(G_OBJECT(button_2), "clicked", G_CALLBACK(imp_minimax_selectplayer), NULL);
}

// Two player mode

void two_player(GtkWidget *p_widget, gpointer user_data)
{
    gtk_button_released(p_widget);
    GtkWidget *p_window;
    GtkWidget *p_v_box;
    GtkWidget *p_entry;
    GtkBuilder *builder;
    GObject *button_1, *button_2, *button_3, *button_4, *button_5, *button_6, *button_7, *button_8, *button_9, *announcement_label;
    GError *error = NULL;

    // reset board for every new game played
    for (int i = 0; i < 9; i++)
    {
        board[i] = 0;
    }

    /* Construct a GtkBuilder instance and load our UI description */

    builder = gtk_builder_new();
    if (gtk_builder_add_from_file(builder, "builder.ui", &error) == 0)
    {
        g_printerr("Error loading file: %s\n", error->message);
        g_clear_error(&error);
        return 1;
    }

    g_print("\nYou vs Friend selected!\n");

    // p_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    // gtk_window_set_title(GTK_WINDOW(p_window), "Two Player");
    // gtk_window_set_default_size(GTK_WINDOW(p_window), 300, 300);

    p_window = gtk_builder_get_object(builder, "window");
    // g_signal_connect (p_window, "destroy", G_CALLBACK (gtk_main_quit), NULL);

    // Set buttons
    button_1 = gtk_builder_get_object(builder, "button1");
    button_2 = gtk_builder_get_object(builder, "button2");
    button_3 = gtk_builder_get_object(builder, "button3");
    button_4 = gtk_builder_get_object(builder, "button4");
    button_5 = gtk_builder_get_object(builder, "button5");
    button_6 = gtk_builder_get_object(builder, "button6");
    button_7 = gtk_builder_get_object(builder, "button7");
    button_8 = gtk_builder_get_object(builder, "button8");
    button_9 = gtk_builder_get_object(builder, "button9");

    // Set label
    announcement_label = gtk_builder_get_object(builder, "announcement");
    gtk_label_set_text(GTK_LABEL(announcement_label), "X's Turn");

    // button is clicked, check with update_button (pass in label, placement and error)
    g_signal_connect(button_1, "clicked", G_CALLBACK(position_1), builder);
    g_signal_connect(button_2, "clicked", G_CALLBACK(position_2), builder);
    g_signal_connect(button_3, "clicked", G_CALLBACK(position_3), builder);
    g_signal_connect(button_4, "clicked", G_CALLBACK(position_4), builder);
    g_signal_connect(button_5, "clicked", G_CALLBACK(position_5), builder);
    g_signal_connect(button_6, "clicked", G_CALLBACK(position_6), builder);
    g_signal_connect(button_7, "clicked", G_CALLBACK(position_7), builder);
    g_signal_connect(button_8, "clicked", G_CALLBACK(position_8), builder);
    g_signal_connect(button_9, "clicked", G_CALLBACK(position_9), builder);

    // cannot loop just whenever the player clicks on the button that is valid the turn will increase
    // cannot loop since the funtion will run all thats in the function

    gtk_widget_show_all(p_window);
}

int main_window(int argc, char **argv)
{
    GtkWidget *window;
    GtkWidget *button_1;
    GtkWidget *button_2;
    GtkWidget *label;
    GtkWidget *grid;

    gtk_init(&argc, &argv);

    // Declaration
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL); // Create window
    label = gtk_label_new("\n   Tic-Tac-Toe  \n\n=== New Game ===\n");
    gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_CENTER);
    grid = gtk_grid_new();
    button_1 = gtk_button_new_with_label("You vs Bot");
    button_2 = gtk_button_new_with_label("You vs Friend");

    // Set Properties
    gtk_window_set_title(GTK_WINDOW(window), "Tic Tac Toe");
    gtk_window_set_default_size(GTK_WINDOW(window), 300, 300);
    gtk_grid_set_row_spacing(GTK_GRID(grid), 4);
    gtk_grid_set_column_spacing(GTK_GRID(grid), 4);
    gtk_container_add(GTK_CONTAINER(window), grid);

    // Fill grid
    gtk_grid_attach(GTK_GRID(grid), label, 1, 0, 2, 1);
    gtk_grid_attach(GTK_GRID(grid), button_1, 1, 1, 2, 1);
    gtk_grid_attach(GTK_GRID(grid), button_2, 1, 2, 2, 1);

    gtk_widget_show_all(window);

    g_signal_connect(G_OBJECT(button_1), "clicked", G_CALLBACK(single_player), NULL);
    g_signal_connect(G_OBJECT(button_2), "clicked", G_CALLBACK(two_player), NULL);

    return 0;
}

int main(int argc, char *argv[])
{
    main_window(argc, argv);
    gtk_main();
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

// AI calculates best move using miniMax function and performs optimal move
int AImove(int board[9]) // update to return position of where to put the move
{
    printf("\nPlaying with Bot A...\n");
    int score = MIN_SCORE;
    int move = 0;

    for (int i = 0; i < 9; i++)
    {
        if (board[i] == 0)
        {
            board[i] = 1;
            int tempScore = miniMax(board, 0, 0);
            board[i] = 0;
            if (tempScore > score)
            {
                score = tempScore;
                move = i;
            }
        }
    }
    board[move] = 1;
    return move;
}

// Checks all possible move in board and return value of board
// If 10, player wins. If -10, opponent wins. If 0, tie. Otherwise, calculate move values
int miniMax(int board[9], int depth, int maxTurn)
{
    int gameState = checkWin(board);
    int checkDraw = movesLeft(board);

    if (gameState == 1)
    {
        return 10;
    }

    if (gameState == -1)
    {
        return -10;
    }

    if (checkDraw == 0) // Tie
    {
        return 0;
    }

    if (maxTurn) // If maximizer's turn, get the highest score as possible recursively
    {
        int bestScore = MIN_SCORE;
        for (int i = 0; i < 9; i++) // Checks every row and column if empty box exists
        {
            if (board[i] == 0)
            {
                board[i] = 1;
                int tempScore = miniMax(board, depth + 1, 0);
                board[i] = 0; // Resets back box
                bestScore = max(tempScore, bestScore);
            }
        }
        return bestScore;
    }

    else // If minimizer's turn, get the lowest score as possible recursively
    {
        int bestScore = MAX_SCORE;
        for (int i = 0; i < 9; i++) // Checks every row and column if empty box exists
        {
            if (board[i] == 0)
            {
                board[i] = -1;
                int tempScore = miniMax(board, depth + 1, 1);
                board[i] = 0; // Resets back box
                bestScore = min(tempScore, bestScore);
            }
        }
        return bestScore;
    }
}

// Check if board has any more space left to fill
int movesLeft(int board[9])
{
    for (int i = 0; i < 9; i++)
    {
        if (board[i] == 0)
        {
            return 1;
        }
    }
    return 0;
}

// Returns higher value
int max(int a, int b)
{
    return a > b ? a : b;
}

// Returns lower value
int min(int a, int b)
{
    return a < b ? a : b;
}

// minimax using alpha-beta pruning (fail-hard)
int minimaxAB(int board[9], int depth, int alpha, int beta, int maxTurn)
{
    int gameState = checkWin(board);
    int checkDraw = movesLeft(board);

    if (gameState == 1)
    {
        return 10;
    }

    if (gameState == -1)
    {
        return -10;
    }

    if (checkDraw == 0) // Tie
    {
        return 0;
    }

    if (maxTurn)
    {
        // If maximizer's turn, get the highest score as possible recursively
        int bestScore = MIN_SCORE;
        // Checks every row and column if empty box exists
        for (int i = 0; i < 9; i++)
        {
            if (board[i] == 0)
            {
                if (depth == 2) // setting a depth limit uses less memory but it cannot travers into the tree deeper to find a more optimal result
                {
                    break;
                }
                board[i] = 1;
                int tempScore = minimaxAB(board, depth + 1, alpha, beta, 0);
                board[i] = 0; // resets back box
                bestScore = max(bestScore, tempScore);
                if (bestScore >= beta)
                    break;
                alpha = max(alpha, bestScore);
            }
        }
        return bestScore; // bot will win or get draw
    }

    else
    {
        // If minimizer's turn, get the lowest score as possible recursively
        int bestScore = MAX_SCORE;
        for (int i = 0; i < 9; i++) // Checks every row and column if empty box exists
        {
            if (board[i] == 0)
            {
                if (depth == 2) // setting a depth limit uses less memory but it cannot travers into the tree deeper to find a more optimal result
                {
                    break;
                }
                board[i] = -1;
                int tempScore = minimaxAB(board, depth + 1, alpha, beta, 1);
                board[i] = 0; // resets back box
                bestScore = min(tempScore, bestScore);
                if (bestScore <= alpha)
                    break;
                beta = min(beta, bestScore);
            }
        }
        return bestScore;
    }
}

// next best move minimax ab pruning
int moveAB(int board[9]) // update to return position of where to put the move
{
    printf("\nPlaying with Bot B...\n");
    int score = MIN_SCORE;
    int move = 0;

    for (int i = 0; i < 9; i++)
    {
        if (board[i] == 0)
        {
            board[i] = 1;
            int tempScore = minimaxAB(board, 0, MIN_SCORE, MAX_SCORE, 0);
            board[i] = 0;
            if (tempScore > score)
            {
                score = tempScore;
                move = i;
            }
        }
    }
    board[move] = 1;
    return move;
}

int levelDifficulty(float botWins, float gamesPlayed)
{
    // if draw botWins + 0.5, if bot wins botWins + 1
    float difficulty = botWins / gamesPlayed;
    if (gamesPlayed != 0) // played at least 1 game
    {
        if (difficulty > 0.6)
        {
            // Difficulty: Hard
            return 3;
        }
        else if (difficulty > 0.4)
        {
            // Difficulty: Intermediate
            return 2;
        }
        else
        {
            // Difficulty: Easy
            return 1;
        }
    }
    return 0;
}