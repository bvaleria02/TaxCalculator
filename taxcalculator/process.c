#include <gtk/gtk.h>
#include <stdbool.h>
#include <string.h>
#include "libtaxcalculator.h"

double TCGetTaxes(CalculatorApp *ca){
	const gchar *userValue = gtk_entry_get_text(GTK_ENTRY(ca->config.taxes.entry));
	double taxes;
	if(sscanf(userValue, "%lf", &taxes) != 1){
		return raiseErrorNonNumericTaxes(ca);
	}
	
	return taxes;
}

void TCCalculatePriceFromTaxToNo(GtkWidget *widget, CalculatorApp *ca){
	g_print("[DEBUG] Price from tax entry\n");

	double valueWithTaxes = getDoubleFromTextEntry(&(ca->priceWithTaxes), ca);
	if(valueWithTaxes == VALUE_ERROR) return;

	double taxes = TCGetTaxes(ca);
	if(taxes == TAX_ERROR) return;

	double valueWithoutTaxes = getValueWithoutTaxes(valueWithTaxes, taxes);
	double valueFivePercentWithoutTaxes = valueWithoutTaxes * (double) FIVE_PERCENT;

	TCSetAllEntries(ca, valueWithTaxes, valueWithoutTaxes, valueFivePercentWithoutTaxes);
	TCCreateNewTableRow(&(ca->spreadsheet), valueWithTaxes, valueWithoutTaxes, valueFivePercentWithoutTaxes);
}


void TCCalculatePriceFromNoTaxToTax(GtkWidget *widget, CalculatorApp *ca){
	g_print("[DEBUG] Price from no tax entry\n");

	double valueWithoutTaxes = getDoubleFromTextEntry(&(ca->priceWithoutTaxes), ca);
	if(valueWithoutTaxes == VALUE_ERROR) return;

	double taxes = TCGetTaxes(ca);
	if(taxes == TAX_ERROR) return;

	double valueWithTaxes = getValueWithTaxes(valueWithoutTaxes, taxes);
	double valueFivePercentWithoutTaxes = valueWithoutTaxes * (double) FIVE_PERCENT;

	TCSetAllEntries(ca, valueWithTaxes, valueWithoutTaxes, valueFivePercentWithoutTaxes);
	TCCreateNewTableRow(&(ca->spreadsheet), valueWithTaxes, valueWithoutTaxes, valueFivePercentWithoutTaxes);
}

void TCCalculatePriceFromFivePercent(GtkWidget *widget, CalculatorApp *ca){
	g_print("[DEBUG] Price from five percent no tax entry\n");

	double valueFivePercentWithoutTaxes = getDoubleFromTextEntry(&(ca->priceFivePercent), ca);
	if(valueFivePercentWithoutTaxes == VALUE_ERROR) return;

	double taxes = TCGetTaxes(ca);
	if(taxes == TAX_ERROR) return;

	double valueWithoutTaxes = valueFivePercentWithoutTaxes / FIVE_PERCENT;
	double valueWithTaxes = getValueWithTaxes(valueWithoutTaxes, taxes);

	TCSetAllEntries(ca, valueWithTaxes, valueWithoutTaxes, valueFivePercentWithoutTaxes);
	TCCreateNewTableRow(&(ca->spreadsheet), valueWithTaxes, valueWithoutTaxes, valueFivePercentWithoutTaxes);
}
