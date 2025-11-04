#include <gtk/gtk.h>
#include <stdbool.h>
#include <string.h>
#include "libtaxcalculator.h"

TCErrorCode TCCreateConfig(CalculatorApp *ca){
	if(ca == NULL){
		return TC_ERROR_NULLPTR;
	}

	ca->config.buttonFromTaxesToNoTaxes = gtk_button_new_with_label("Calcular desde precio con IVA.");
	g_signal_connect(GTK_OBJECT(ca->config.buttonFromTaxesToNoTaxes), "clicked", G_CALLBACK(TCCalculatePriceFromTaxToNo), ca);

	ca->config.buttonFromNoTaxesToTaxes = gtk_button_new_with_label("Calcular desde precio sin IVA.");
	g_signal_connect(GTK_OBJECT(ca->config.buttonFromNoTaxesToTaxes), "clicked", G_CALLBACK(TCCalculatePriceFromNoTaxToTax), ca);

	TCCreateEntry(&(ca->config.taxes),	"IVA", DEFAULT_TAX_VALUE);
	TCAddButtonsEntry(&(ca->config.taxes), ca, true, false);

	return TC_NO_ERROR;
}

TCErrorCode TCSetSizeConfig(CalculatorApp *ca, int x, int y){
	if(ca == NULL){
		return TC_ERROR_NULLPTR;
	}

	ca->config.coordinates.x = x;
	ca->config.coordinates.y = y;

	TCSetSizeEntry(&(ca->config.taxes), x, y+3);
	
	return TC_NO_ERROR;
}

TCErrorCode TCPlaceConfig(CalculatorApp *ca){
	if(ca == NULL){
		return TC_ERROR_NULLPTR;
	}

	gtk_table_attach_defaults(GTK_TABLE(ca->mainTable), ca->config.buttonFromTaxesToNoTaxes,
			ca->config.coordinates.x,
			ca->config.coordinates.x + 3,
			ca->config.coordinates.y,
			ca->config.coordinates.y+1
		);

	gtk_table_attach_defaults(GTK_TABLE(ca->mainTable), ca->config.buttonFromNoTaxesToTaxes,
			ca->config.coordinates.x,
			ca->config.coordinates.x + 3,
			ca->config.coordinates.y+1,
			ca->config.coordinates.y+2
		);

	TCPlaceEntry(&(ca->config.taxes), ca);


	return TC_NO_ERROR;
}
