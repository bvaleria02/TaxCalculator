#include <gtk/gtk.h>
#include <stdbool.h>
#include <string.h>
#include "libtaxcalculator.h"

void TCTerminateWindow(GtkWidget *widget, CalculatorApp *ca){
	gtk_main_quit();
}

void TCDestroyWindow(GtkWidget *widget, CalculatorApp *ca){
	TCResponse response;	

	if(ca->spreadsheet.hasChanged){
		response = TCAskSaveDialog(ca);
	}

	g_print("response: %i\n", response);
	if(response == TC_RESPONSE_YES){
		TCSaveData(ca);
	}

	g_print("Bye\n");
	gtk_main_quit();
}

TCErrorCode TCCreateWindow(CalculatorApp *calculatorApp){
	if(calculatorApp == NULL){
		return TC_ERROR_NULLPTR;
	}

	calculatorApp->window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(calculatorApp->window), "Calculadora de IVA");
	g_signal_connect(calculatorApp->window, "destroy", G_CALLBACK(TCDestroyWindow), calculatorApp);
	gtk_container_set_border_width(GTK_CONTAINER(calculatorApp->window), 0);

	return TC_NO_ERROR;
}

TCErrorCode TCRunWindow(CalculatorApp *calculatorApp){
	if(calculatorApp == NULL){
		return TC_ERROR_NULLPTR;
	}

	gtk_widget_show_all(calculatorApp->window);
	gtk_main();

	return TC_NO_ERROR;
}

TCErrorCode TCPlaceElements(CalculatorApp *ca){
	if(ca == NULL){
		return TC_ERROR_NULLPTR;
	}

	ca->vbox = gtk_vbox_new(FALSE, 0);
	ca->mainTable = gtk_table_new(DEFAULT_TABLE_HEIGHT, DEFAULT_TABLE_WIDTH, true);

	TCPlaceEntry(&(ca->priceWithTaxes), ca);
	TCPlaceEntry(&(ca->priceWithoutTaxes), ca);
	TCPlaceEntry(&(ca->priceFivePercent), ca);

	TCPlaceConfig(ca);
	TCPlaceDataTable(ca);

	TCCreateMenu(ca);
	TCAddActionsMenu(ca);

	gtk_container_add(GTK_CONTAINER(ca->vbox), ca->mainTable);
	gtk_container_add(GTK_CONTAINER(ca->window), ca->vbox);
	gtk_container_set_border_width(GTK_CONTAINER(ca->mainTable), 32);
	return TC_NO_ERROR;
}
