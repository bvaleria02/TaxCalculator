#include <gtk/gtk.h>
#include <stdbool.h>
#include <string.h>
#include "taxcalculator/libtaxcalculator.h"

int main(int argc, char **argv){
	gtk_init(&argc, &argv);

	CalculatorApp calculatorApp;
	TCCreateWindow(&calculatorApp);
	
	TCCreateEntry(&(calculatorApp.priceWithTaxes), 	"Precio con IVA", DEFAULT_ENTRY_VALUE);
	TCAddButtonsEntry(&(calculatorApp.priceWithTaxes), &calculatorApp, true, true);
	TCSetSizeEntry(&(calculatorApp.priceWithTaxes), 0, 0);

	TCCreateEntry(&(calculatorApp.priceWithoutTaxes), 	"Precio sin IVA", DEFAULT_ENTRY_VALUE);
	TCAddButtonsEntry(&(calculatorApp.priceWithoutTaxes), &calculatorApp,	true, true);
	TCSetSizeEntry(&(calculatorApp.priceWithoutTaxes), 0, 4);

	TCCreateEntry(&(calculatorApp.priceFivePercent), 	"5% sin IVA", DEFAULT_ENTRY_VALUE);
	TCAddButtonsEntry(&(calculatorApp.priceFivePercent), &calculatorApp, true, true);
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
