#include <libgimp/gimp.h>
#include <libgimp/gimpui.h>
#include <gtk/gtk.h>
#include <glib.h>
#include <stdio.h>
#include <string.h>
#include "norezator.h"

#undef DEFAULT_DIR

void norezator_dialog(gint image)
{
    GtkWidget *file_chooser_dialog;
    gint *layers, layers_count, i;
    gimp_ui_init("norezator", FALSE);
    layers = gimp_image_get_layers(image, &layers_count);

#ifndef DEFAULT_DIR
    file_chooser_dialog = gtk_file_chooser_dialog_new("Choose Destination", NULL,
                                                      GTK_FILE_CHOOSER_ACTION_SELECT_FOLDER,
                                                      GTK_STOCK_CANCEL,
                                                      GTK_RESPONSE_CANCEL,
                                                      GTK_STOCK_OK,
                                                      GTK_RESPONSE_ACCEPT, NULL);
    if (gtk_dialog_run(GTK_DIALOG(file_chooser_dialog)) == GTK_RESPONSE_ACCEPT)
#endif
    {
        gchar buffer[1024];
        gchar *filename, *full_filename;
        filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(file_chooser_dialog));
//        g_mkdir_with_parents(filename, 0777);
        full_filename = g_build_filename(filename, "out.txt", NULL);
        FILE *description = fopen(full_filename, "wt");
        g_free(full_filename);
        for (i = 0; i < layers_count; i++)
        {
            full_filename = g_build_filename(filename, gimp_drawable_get_name(layers[i]), NULL);
            sprintf(buffer, "%s.png", full_filename);
            g_free(full_filename);
            gimp_file_save(GIMP_RUN_NONINTERACTIVE, image, layers[i], buffer, buffer);
            gint x, y;
            gimp_drawable_offsets(layers[i], &x, &y);
            fprintf(description, "%s.png %d %d\n", gimp_drawable_get_name(layers[i]), x - 1366, 1600 - y);
        }
        fclose(description);
        g_free(filename);
    }
#ifndef DEFAULT_DIR
    gtk_widget_destroy(file_chooser_dialog);
#endif
    return;
}

void norezator_run(const gchar *name, gint parameters_count, const GimpParam *parameters, gint *return_values_count, GimpParam **return_values)
{
    fprintf(stderr, "norezator_run: %d\n", parameters_count);
    if (strcmp(name, "norezator") == 0)
    {
        static GimpParam values[1];
        GimpPDBStatusType status = GIMP_PDB_SUCCESS;
        GimpRunMode run_mode;
        *return_values_count = 1;
        *return_values = values;
        values[0].type = GIMP_PDB_STATUS;
        values[0].data.d_status = status;
        run_mode = parameters[0].data.d_int32;
        norezator_dialog(parameters[1].data.d_int32);
    }
    return;
}

void norezator_query(void)
{
    
    static GimpParamDef arguments[] =
    {
        {
            GIMP_PDB_INT32,
            "run-mode",
            "Run Mode"
        },
        {
            GIMP_PDB_IMAGE,
            "image",
            "Input Image"
        }
    };
    gimp_install_procedure("norezator", "Norezator", "Saves layers as PNGs",
                           "Kyrylo V. Polezhaiev", "(c) 2011, 2013, 2015 Kyrylo V. Polezhaiev", "November 19, 2011, February 20, 2013, February 28, 2015",
                           "Norezator 2.1",
                           "RGBA",
                           GIMP_PLUGIN,
                           G_N_ELEMENTS (arguments), 0,
                           arguments, NULL);
    gimp_plugin_menu_register("norezator", "<Image>/Tools");
    return;
}

MAIN()

