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
