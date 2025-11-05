#include <gtk/gtk.h>
#include <stdbool.h>
#include <string.h>
#include "libtaxcalculator.h"

double raiseErrorNonNumericTaxes(CalculatorApp *calculatorApp){
	GtkWidget *dialog;
	GtkDialogFlags flags = GTK_DIALOG_DESTROY_WITH_PARENT | GTK_DIALOG_MODAL;

    dialog = gtk_message_dialog_new(GTK_WINDOW(calculatorApp->window),
                                    flags,
                                    GTK_MESSAGE_ERROR,
                                    GTK_BUTTONS_CLOSE,
                                    "El valor para el IVA debe ser un número (por ejemplo 19.0)."
								);

    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
	return TAX_ERROR;
}

double raiseErrorNonNumeric(CalculatorApp *calculatorApp){
	GtkWidget *dialog;
	GtkDialogFlags flags = GTK_DIALOG_DESTROY_WITH_PARENT | GTK_DIALOG_MODAL;

    dialog = gtk_message_dialog_new(GTK_WINDOW(calculatorApp->window),
                                    flags,
                                    GTK_MESSAGE_ERROR,
                                    GTK_BUTTONS_CLOSE,
                                    "El valor ingresado no es numérico (debe ingresarlo como 1234 o 125.9)"
								);

    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);

	return VALUE_ERROR;
}

TCResponse TCAskSaveDialog(CalculatorApp *ca){
	GtkWidget *dialog;
	GtkDialogFlags flags = GTK_DIALOG_DESTROY_WITH_PARENT | GTK_DIALOG_MODAL;

	ca->lastResponse = TC_NO_RESPONSE;

    dialog = gtk_message_dialog_new(GTK_WINDOW(ca->window),
                                    flags,
                                    GTK_MESSAGE_QUESTION,
                                    GTK_BUTTONS_YES_NO,
                                    "¿Desea guardar los cambios antes de salir?"
								);

    ca->lastResponse = gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);

	return ca->lastResponse;
}

TCResponse TCGetFileDialog(CalculatorApp *ca){
	if(ca == NULL){
		return TC_NO_RESPONSE;
	}

	ca->fileMenu.fileSelection = gtk_file_selection_new("Guardar como");
	ca->fileMenu.okButton = GTK_FILE_SELECTION(ca->fileMenu.fileSelection)->ok_button;
	ca->fileMenu.cancelButton = GTK_FILE_SELECTION(ca->fileMenu.fileSelection)->cancel_button;

	g_signal_connect(G_OBJECT(ca->fileMenu.okButton), "clicked", G_CALLBACK(TCGetFilenameFromFileSelection), ca);
	g_signal_connect(G_OBJECT(ca->fileMenu.cancelButton), "clicked", G_CALLBACK(TCExitFilenameFileSelection), ca);

	gtk_widget_show_all(ca->fileMenu.fileSelection);
	gtk_dialog_run(ca->fileMenu.fileSelection);

	return TC_RESPONSE_YES;
}

TCErrorCode TCErrorFileReadDialog(CalculatorApp *calculatorApp, const gchar *filename){
	GtkWidget *dialog;
	GtkDialogFlags flags = GTK_DIALOG_DESTROY_WITH_PARENT | GTK_DIALOG_MODAL;

    dialog = gtk_message_dialog_new(GTK_WINDOW(calculatorApp->window),
                                    flags,
                                    GTK_MESSAGE_ERROR,
                                    GTK_BUTTONS_CLOSE,
                                    "No es posible abrir %s",
									filename
								);

    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);

	return VALUE_ERROR;
}

TCResponse TCAskSaveBeforeLoadDialog(CalculatorApp *ca){
	GtkWidget *dialog;
	GtkDialogFlags flags = GTK_DIALOG_DESTROY_WITH_PARENT | GTK_DIALOG_MODAL;

	ca->lastResponse = TC_NO_RESPONSE;

    dialog = gtk_message_dialog_new(GTK_WINDOW(ca->window),
                                    flags,
                                    GTK_MESSAGE_QUESTION,
                                    GTK_BUTTONS_YES_NO,
                                    "¿Desea guardar los cambios antes de cargar otro archivo?"
								);

    ca->lastResponse = gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);

	return ca->lastResponse;
}
