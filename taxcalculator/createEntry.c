#include <gtk/gtk.h>
#include <stdbool.h>
#include <string.h>
#include "libtaxcalculator.h"

void greet(GtkWidget *widget, gpointer data){
	static int counter = 1;

	g_print("Welcome to GTK!\n");
	g_print("This button has been pressed %i times\n", counter);

	counter++;
}

TCErrorCode TCCreateEntry(TextEntry *te, const char *label, double defaultValue){
	if(te == NULL || label == NULL){
		return TC_ERROR_NULLPTR;
	}

	char buffer[MAX_BUFFER_SIZE];
	convertDoubleToString(buffer, MAX_BUFFER_SIZE, defaultValue);

	te->label = gtk_label_new(label);

	te->entry = gtk_entry_new();
	gtk_entry_set_text(GTK_ENTRY(te->entry), buffer);
	gtk_entry_set_alignment(GTK_ENTRY(te->entry), RIGHT_ALIGN);

	te->reset = NULL;
	te->tenInc = NULL;
	te->tenDec = NULL;
	te->defaultValue = defaultValue;

	return TC_NO_ERROR;
}

void incrementTenFunc(GtkWidget *widget, gpointer pcp){
	CallbackPointers cp = *(CallbackPointers *)pcp;
	CalculatorApp 	*ca = cp.ca;
	TextEntry 		*te = cp.te;

	double value = getDoubleFromTextEntry(te, ca);
	value *= 10;
	char buffer[MAX_BUFFER_SIZE];
	convertDoubleToString(buffer, MAX_BUFFER_SIZE, value);
	gtk_entry_set_text(GTK_ENTRY(te->entry), buffer);
}

void decrementTenFunc(GtkWidget *widget, gpointer pcp){
	CallbackPointers cp = *(CallbackPointers *)pcp;
	CalculatorApp 	*ca = cp.ca;
	TextEntry 		*te = cp.te;

	double value = getDoubleFromEntry(te->entry, ca);
	value /= 10.0f;
	char buffer[MAX_BUFFER_SIZE];
	convertDoubleToString(buffer, MAX_BUFFER_SIZE, value);
	gtk_entry_set_text(GTK_ENTRY(te->entry), buffer);
}

void resetFunc(GtkWidget *widget, TextEntry *te){
	char buffer[MAX_BUFFER_SIZE];
	convertDoubleToString(buffer, MAX_BUFFER_SIZE, te->defaultValue);
	gtk_entry_set_text(GTK_ENTRY(te->entry), buffer);
}

TCErrorCode TCAddButtonsEntry(TextEntry *te, CallbackPointers *pcp, bool reset, bool incdec){
	if(te == NULL || pcp == NULL){
		return TC_ERROR_NULLPTR;
	}

	if(reset){
		te->reset = gtk_button_new_with_label("Reiniciar");
		g_signal_connect(GTK_OBJECT(te->reset), "clicked", G_CALLBACK(resetFunc), te);
	}

	if(incdec){
		te->tenInc = gtk_button_new_with_label("* 10");
		te->tenDec = gtk_button_new_with_label("/ 10");

		g_signal_connect(GTK_OBJECT(te->tenInc), "clicked", G_CALLBACK(incrementTenFunc), pcp);
		g_signal_connect(GTK_OBJECT(te->tenDec), "clicked", G_CALLBACK(decrementTenFunc), pcp);
	}

	return TC_NO_ERROR;
}

TCErrorCode TCSetSizeEntry(TextEntry *te, int x, int y){
	if(te == NULL){
		return TC_ERROR_NULLPTR;
	}

	te->coordinates.x = x;
	te->coordinates.y = y;
	return TC_NO_ERROR;
}

TCErrorCode TCPlaceEntry(TextEntry *te, CalculatorApp *ca){
	if(te == NULL || ca == NULL){
		return TC_ERROR_NULLPTR;
	}

	gtk_table_attach_defaults(GTK_TABLE(ca->mainTable), te->label, te->coordinates.x, te->coordinates.x + 3, te->coordinates.y,   te->coordinates.y + 1);

	if(te->reset != NULL){
		gtk_table_attach_defaults(GTK_TABLE(ca->mainTable), te->entry, te->coordinates.x, te->coordinates.x + 2, te->coordinates.y+1, te->coordinates.y + 2);
		gtk_table_attach_defaults(GTK_TABLE(ca->mainTable), te->reset, te->coordinates.x+2, te->coordinates.x+3, te->coordinates.y+1, te->coordinates.y + 2);
	} else {
		gtk_table_attach_defaults(GTK_TABLE(ca->mainTable), te->entry, te->coordinates.x, te->coordinates.x + 3, te->coordinates.y+1, te->coordinates.y + 2);
	}
	
	if(te->tenInc != NULL){
		gtk_table_attach_defaults(GTK_TABLE(ca->mainTable), te->tenInc, te->coordinates.x+2, te->coordinates.x+3, te->coordinates.y+2, te->coordinates.y + 3);
	}

	if(te->tenDec != NULL){
		gtk_table_attach_defaults(GTK_TABLE(ca->mainTable), te->tenDec, te->coordinates.x+1, te->coordinates.x+2, te->coordinates.y+2, te->coordinates.y + 3);
	}

	return TC_NO_ERROR;
}

TCErrorCode TCSetCallbackEntry(TextEntry *te, CalculatorApp *ca, GCallback callback){
	if(te == NULL){
		return TC_ERROR_NULLPTR;
	}

	g_signal_connect(GTK_OBJECT(te->entry), "activate", G_CALLBACK(callback), ca);

	return TC_NO_ERROR;
}

TCErrorCode TCSetCallbackEntries(CalculatorApp *ca){
	if(ca == NULL){
		return TC_ERROR_NULLPTR;
	}


	TCSetCallbackEntry(&(ca->priceWithTaxes), 	 ca, G_CALLBACK(TCCalculatePriceFromTaxToNo));
	TCSetCallbackEntry(&(ca->priceWithoutTaxes), ca, G_CALLBACK(TCCalculatePriceFromNoTaxToTax));
	TCSetCallbackEntry(&(ca->priceFivePercent),  ca, G_CALLBACK(TCCalculatePriceFromFivePercent));

	return TC_NO_ERROR;
}

TCErrorCode TCResetAllEntries(CalculatorApp *ca){
	if(ca == NULL){
		return TC_ERROR_NULLPTR;
	}

	resetFunc(NULL, &(ca->priceWithTaxes));
	resetFunc(NULL, &(ca->priceWithoutTaxes));
	resetFunc(NULL, &(ca->priceFivePercent));
	resetFunc(NULL, &(ca->config.taxes));
	
	return TC_NO_ERROR;
}
