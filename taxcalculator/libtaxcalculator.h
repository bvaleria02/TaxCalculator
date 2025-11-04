#ifndef LIB_TAX_CALCULATOR_H
#define LIB_TAX_CALCULATOR_H

#include <gtk/gtk.h>

#define DEFAULT_TAX_VALUE 19.0
#define DEFAULT_TAX_STRING "19.0"
#define DEFAULT_ENTRY_VALUE 0
#define RIGHT_ALIGN 1
#define TAX_ERROR -100
#define VALUE_ERROR -1
#define FIVE_PERCENT (0.05)
#define MAX_BUFFER_SIZE 128
#define DEFAULT_TABLE_HEIGHT 17
#define DEFAULT_TABLE_WIDTH 8

typedef struct{
	int x;
	int y;
} Coordinates;


typedef struct{
	Coordinates coordinates;
	double defaultValue;
	GtkWidget  *entry;
	GtkWidget *reset;
	GtkWidget *tenInc;
	GtkWidget *tenDec;
	GtkWidget *label;
} TextEntry;

typedef struct{
	Coordinates coordinates;
	GtkWidget *buttonFromTaxesToNoTaxes;
	GtkWidget *buttonFromNoTaxesToTaxes;
	TextEntry taxes;
} ConfigApp;

typedef struct{
	Coordinates coordinates;
	GtkWidget *store;
	GtkTreeIter iter;
	GtkWidget *model;
	GtkWidget *renderer;
	GtkWidget *view;
	GtkWidget *container;
	GtkWidget *clear;
} DataTable;

typedef struct{
	TextEntry priceWithTaxes;
	TextEntry priceWithoutTaxes;
	TextEntry priceFivePercent;
	ConfigApp config;
	DataTable spreadsheet;
	GtkWidget *mainTable;
	GtkWidget *window;
} CalculatorApp;

typedef struct{
	CalculatorApp *ca;
	TextEntry *te;
} CallbackPointers;

typedef enum{
	TC_NO_ERROR 		= 0,
	TC_ERROR_NULLPTR	= 1,
} TCErrorCode;


//extern CalculatorApp calculatorApp;

// utils.c
double getTaxesScale(double taxes);
double getValueWithTaxes(double value, double taxes);
double getValueWithoutTaxes(double value, double taxes);
void convertDoubleToString(char *buffer, size_t size, double value);
double getDoubleFromEntry(GtkWidget *entry, CalculatorApp *ca);
double getDoubleFromTextEntry(TextEntry *te, CalculatorApp *ca);
TCErrorCode TCSetAllEntries(CalculatorApp *ca, double priceWithTaxes, double priceWithoutTaxes, double priceFivePercent);

// createEntry.c
TCErrorCode TCCreateEntry(TextEntry *te, const char *label, double defaultValue);
TCErrorCode TCAddButtonsEntry(TextEntry *te, CalculatorApp *ca, bool reset, bool incdec);
TCErrorCode TCSetSizeEntry(TextEntry *te, int x, int y);
TCErrorCode TCPlaceEntry(TextEntry *te, CalculatorApp *ca);
TCErrorCode TCSetCallbackEntry(TextEntry *te, CalculatorApp *ca, GCallback callback);
TCErrorCode TCSetCallbackEntries(CalculatorApp *ca);

// config.c
TCErrorCode TCCreateConfig(CalculatorApp *ca);
TCErrorCode TCSetSizeConfig(CalculatorApp *ca, int x, int y);
TCErrorCode TCPlaceConfig(CalculatorApp *ca);

// dialogs.c
double raiseErrorNonNumericTaxes(CalculatorApp *ca);
double raiseErrorNonNumeric(CalculatorApp *ca);

// process.c
double TCGetTaxes(CalculatorApp *ca);
void TCCalculatePriceFromTaxToNo(GtkWidget *widget, CalculatorApp *ca);
void TCCalculatePriceFromNoTaxToTax(GtkWidget *widget, CalculatorApp *ca);
void TCCalculatePriceFromFivePercent(GtkWidget *widget, CalculatorApp *ca);

// window.c
TCErrorCode TCCreateWindow(CalculatorApp *calculatorApp);
TCErrorCode TCRunWindow(CalculatorApp *calculatorApp);
TCErrorCode TCPlaceElements(CalculatorApp *calculatorApp);

// spreadsheet.c
TCErrorCode TCCreateDataTable(DataTable *dt);
void TCCreateNewTableRow(DataTable *dt, double valueWithTaxes, double valueWithoutTaxes, double valueFivePercentWithoutTaxes);
TCErrorCode TCSetSizeDataTable(DataTable *dt, int x, int y);
TCErrorCode TCPlaceDataTable(CalculatorApp *ca);

#endif // LIB_TAX_CALCULATOR_H
