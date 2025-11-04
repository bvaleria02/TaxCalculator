#include <gtk/gtk.h>
#include <stdbool.h>
#include <string.h>
#include "libtaxcalculator.h"

double getTaxesScale(double taxes){
	return 1 + (taxes / (double) 100.0);
}

double getValueWithTaxes(double value, double taxes){
	return value * getTaxesScale(taxes);
}

double getValueWithoutTaxes(double value, double taxes){
	return value / getTaxesScale(taxes);
}

void convertDoubleToString(char *buffer, size_t size, double value){
	memset(buffer,'\0', size);
	snprintf(buffer, size, "%.1lf", value);
}

double getDoubleFromEntry(GtkWidget *entry, CalculatorApp *ca){
	const gchar *userValue = gtk_entry_get_text(GTK_ENTRY(entry));
	double numericValue;
	if(sscanf(userValue, "%lf", &numericValue) != 1){
		return raiseErrorNonNumeric(ca);
	}
	
	return numericValue;
}

double getDoubleFromTextEntry(TextEntry *te, CalculatorApp *ca){
	return getDoubleFromEntry(te->entry, ca);
}

TCErrorCode TCSetAllEntries(CalculatorApp *ca, double priceWithTaxes, double priceWithoutTaxes, double priceFivePercent){
	char buffer[128];

	convertDoubleToString(buffer, 128, priceWithTaxes);
	gtk_entry_set_text(GTK_ENTRY(ca->priceWithTaxes.entry), buffer);

	convertDoubleToString(buffer, 128, priceWithoutTaxes);
	gtk_entry_set_text(GTK_ENTRY(ca->priceWithoutTaxes.entry), buffer);

	convertDoubleToString(buffer, 128, priceFivePercent);
	gtk_entry_set_text(GTK_ENTRY(ca->priceFivePercent.entry), buffer);

	return TC_NO_ERROR;
}
