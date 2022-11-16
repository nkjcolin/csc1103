#include <gtk/gtk.h>
#include <stdlib.h>
static void
print_hello (GtkWidget *widget,
             gpointer   data)
{
  g_print ("Hello World\n");
}


/*void single_player(GtkWidget *p_widget, gpointer user_data)
{
    gtk_button_released(p_widget);
    GtkWidget *p_window;
    GtkWidget *p_v_box;
    GtkWidget *p_entry;

    g_print("single player selected!");

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

    g_print("two player selected!");

    p_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(p_window), "Two Player");
    gtk_window_set_default_size(GTK_WINDOW(p_window), 300, 300);

    gtk_widget_show_all(p_window);
}
*/

int
tic_tac_toe_grid (int   argc,
      char *argv[])
{
  GtkBuilder *builder;
  GObject *window;
  GObject *button;
  GError *error = NULL;

  gtk_init (&argc, &argv);

  /* Construct a GtkBuilder instance and load our UI description */
  
  builder = gtk_builder_new ();
  if (gtk_builder_add_from_file (builder, "builder.ui", &error) == 0)
    {
      g_printerr ("Error loading file: %s\n", error->message);
      g_clear_error (&error);
      return 1;
    }
  
  /* Connect signal handlers to the constructed widgets. */

  window = gtk_builder_get_object (builder, "window");
  g_signal_connect (window, "destroy", G_CALLBACK (gtk_main_quit), NULL);

  button = gtk_builder_get_object (builder, "button1");
  g_signal_connect (button, "clicked", G_CALLBACK (print_hello), NULL);

  button = gtk_builder_get_object (builder, "button2");
  g_signal_connect (button, "clicked", G_CALLBACK (print_hello), NULL);

  button = gtk_builder_get_object (builder, "button3");
  g_signal_connect (button, "clicked", G_CALLBACK (print_hello), NULL);

  button = gtk_builder_get_object (builder, "button4");
  g_signal_connect (button, "clicked", G_CALLBACK (print_hello), NULL);

  button = gtk_builder_get_object (builder, "button5");
  g_signal_connect (button, "clicked", G_CALLBACK (print_hello), NULL);
  
  button = gtk_builder_get_object (builder, "button6");
  g_signal_connect (button, "clicked", G_CALLBACK (print_hello), NULL);

  button = gtk_builder_get_object (builder, "button7");
  g_signal_connect (button, "clicked", G_CALLBACK (print_hello), NULL);
  
  button = gtk_builder_get_object (builder, "button8");
  g_signal_connect (button, "clicked", G_CALLBACK (print_hello), NULL);
  
  button = gtk_builder_get_object (builder, "button9");
  g_signal_connect (button, "clicked", G_CALLBACK (print_hello), NULL);
  
  button = gtk_builder_get_object (builder, "quit");
  g_signal_connect (button, "clicked", G_CALLBACK (gtk_main_quit), NULL);


  gtk_main ();

  return 0;
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

   g_signal_connect(G_OBJECT(button_1), "clicked", G_CALLBACK(tic_tac_toe_grid), NULL);
   g_signal_connect(G_OBJECT(button_2), "clicked", G_CALLBACK(tic_tac_toe_grid), NULL);

   return 0;
}

int
main (int   argc,
      char *argv[])
{
  GtkBuilder *builder;
  GObject *window;
  GObject *button;
  GError *error = NULL;

  gtk_init (&argc, &argv);

  main_window(argc, argv);
  /*tic_tac_toe_grid(argc, argv);*/
  /* Construct a GtkBuilder instance and load our UI description */
  /*
  builder = gtk_builder_new ();
  if (gtk_builder_add_from_file (builder, "builder.ui", &error) == 0)
    {
      g_printerr ("Error loading file: %s\n", error->message);
      g_clear_error (&error);
      return 1;
    }
  */
  /* Connect signal handlers to the constructed widgets. */

  /*
  window = gtk_builder_get_object (builder, "window");
  g_signal_connect (window, "destroy", G_CALLBACK (gtk_main_quit), NULL);

  button = gtk_builder_get_object (builder, "button1");
  g_signal_connect (button, "clicked", G_CALLBACK (print_hello), NULL);

  button = gtk_builder_get_object (builder, "button2");
  g_signal_connect (button, "clicked", G_CALLBACK (print_hello), NULL);

  button = gtk_builder_get_object (builder, "button3");
  g_signal_connect (button, "clicked", G_CALLBACK (print_hello), NULL);

  button = gtk_builder_get_object (builder, "button4");
  g_signal_connect (button, "clicked", G_CALLBACK (print_hello), NULL);

  button = gtk_builder_get_object (builder, "button5");
  g_signal_connect (button, "clicked", G_CALLBACK (print_hello), NULL);
  
  button = gtk_builder_get_object (builder, "button6");
  g_signal_connect (button, "clicked", G_CALLBACK (print_hello), NULL);

  button = gtk_builder_get_object (builder, "button7");
  g_signal_connect (button, "clicked", G_CALLBACK (print_hello), NULL);
  
  button = gtk_builder_get_object (builder, "button8");
  g_signal_connect (button, "clicked", G_CALLBACK (print_hello), NULL);
  
  button = gtk_builder_get_object (builder, "button9");
  g_signal_connect (button, "clicked", G_CALLBACK (print_hello), NULL);
  
  button = gtk_builder_get_object (builder, "quit");
  g_signal_connect (button, "clicked", G_CALLBACK (gtk_main_quit), NULL);

  */

  gtk_main ();

  return 0;
}
