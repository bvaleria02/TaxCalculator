#include <gtk/gtk.h>
#include <stdbool.h>
#include <string.h>
#include "taxcalculator/libtaxcalculator.h"

int main(int argc, char **argv){
	gtk_init(&argc, &argv);

	CalculatorApp calculatorApp;
	TCCreateWindow(&calculatorApp);
	
	CallbackPointers cpPriceWithTaxes;
	cpPriceWithTaxes.ca = &calculatorApp;
	cpPriceWithTaxes.te = &(calculatorApp.priceWithTaxes);
	TCCreateEntry(&(calculatorApp.priceWithTaxes), 	"Precio con IVA", DEFAULT_ENTRY_VALUE);
	TCAddButtonsEntry(&(calculatorApp.priceWithTaxes), &cpPriceWithTaxes, true, true);
	TCSetSizeEntry(&(calculatorApp.priceWithTaxes), 0, 0);

	CallbackPointers cpPriceWithoutTaxes;
	cpPriceWithoutTaxes.ca = &calculatorApp;
	cpPriceWithoutTaxes.te = &(calculatorApp.priceWithoutTaxes);
	TCCreateEntry(&(calculatorApp.priceWithoutTaxes), 	"Precio sin IVA", DEFAULT_ENTRY_VALUE);
	TCAddButtonsEntry(&(calculatorApp.priceWithoutTaxes), &cpPriceWithoutTaxes,	true, true);
	TCSetSizeEntry(&(calculatorApp.priceWithoutTaxes), 0, 4);

	CallbackPointers cpPriceFivePercent;
	cpPriceFivePercent.ca = &calculatorApp;
	cpPriceFivePercent.te = &(calculatorApp.priceFivePercent);
	TCCreateEntry(&(calculatorApp.priceFivePercent), 	"5% sin IVA", DEFAULT_ENTRY_VALUE);
	TCAddButtonsEntry(&(calculatorApp.priceFivePercent), &cpPriceFivePercent, true, true);
	TCSetSizeEntry(&(calculatorApp.priceFivePercent), 0, 8);

	TCCreateConfig(&calculatorApp);
	TCSetSizeConfig(&calculatorApp, 0, 12);

	TCCreateDataTable(&(calculatorApp.spreadsheet));
	TCSetSizeDataTable(&(calculatorApp.spreadsheet), 4, 0);

	TCSetCallbackEntries(&calculatorApp);
	TCPlaceElements(&calculatorApp);
	TCRunWindow(&calculatorApp);

	return 0;
}
