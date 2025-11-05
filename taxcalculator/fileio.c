#include <gtk/gtk.h>
#include <stdbool.h>
#include <string.h>
#include "libtaxcalculator.h"

void TCExitFilenameFileSelection(GtkWidget *widget, CalculatorApp *ca){
	gtk_widget_destroy(ca->fileMenu.fileSelection);
}

void TCGetFilenameFromFileSelection(GtkWidget *widget, CalculatorApp *ca){
	ca->fileMenu.filename = gtk_file_selection_get_filename(GTK_FILE_SELECTION(ca->fileMenu.fileSelection));
	g_print("Name: %s\n", ca->fileMenu.filename);
	gtk_widget_destroy(ca->fileMenu.fileSelection);
}


gboolean TCWriteFileRowValues(GtkTreeModel *model, GtkTreePath *path, GtkTreeIter *iter, FILE *fp){
	long int priceWithTaxes;
	long int priceWithoutTaxes;
	long int priceFivePercent;

	gtk_tree_model_get(
			model, iter,
			0, &priceWithTaxes,
			1, &priceWithoutTaxes,
			2, &priceFivePercent,
			-1
	);

	fprintf(fp, "%li,%li,%li\n", priceWithTaxes, priceWithoutTaxes, priceFivePercent);

	return false;
}


TCErrorCode TCSaveData(CalculatorApp *ca){
	if(ca == NULL){
		return TC_ERROR_NULLPTR;
	}
	
	TCResponse response = TCGetFileDialog(ca);
	if(response != TC_RESPONSE_YES){
		return TC_NO_ERROR;
	}
		
	FILE *fp = fopen(ca->fileMenu.filename, "w+");
	if(fp == NULL){
		TCErrorFileReadDialog(ca, ca->fileMenu.filename);
		return TC_ERROR_NULLPTR;
	}

	fprintf(fp, "PrecioConIVA,PrecioSinIVA,5pSinIVA\n");
	gtk_tree_model_foreach(GTK_TREE_MODEL(ca->spreadsheet.store), G_CALLBACK(TCWriteFileRowValues), fp);

	fclose(fp);
	ca->spreadsheet.hasChanged = 0;

	return TC_NO_ERROR;
}

TCErrorCode TCLoadData(CalculatorApp *ca){
	if(ca == NULL){
		return TC_ERROR_NULLPTR;
	}

	TCResponse response;	

	response = TCGetFileDialog(ca);
	if(response != TC_RESPONSE_YES){
		return TC_NO_ERROR;
	}

	FILE *fp = fopen(ca->fileMenu.filename, "r");
	if(fp == NULL){
		TCErrorFileReadDialog(ca, ca->fileMenu.filename);
		return TC_ERROR_NULLPTR;
	}

	deleteEverythingTree(NULL, &(ca->spreadsheet));
	char buffer[256];
	fgets(buffer, 256, fp);

	int scanfResponse;
	long int priceWithTaxes;
    long int priceWithoutTaxes;
    long int priceFivePercent;

	do{
		scanfResponse = fscanf(fp, "%li,%li,%li\n", &priceWithTaxes, &priceWithoutTaxes, &priceFivePercent);
		if(scanfResponse != 3){
			break;
		}

		TCCreateNewTableRow(&(ca->spreadsheet), priceWithTaxes, priceWithoutTaxes, priceFivePercent);

	} while(1);

	fclose(fp);
	ca->spreadsheet.hasChanged = 0;

	return TC_NO_ERROR;
}
