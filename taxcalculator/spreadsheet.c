#include <gtk/gtk.h>
#include <stdbool.h>
#include <string.h>
#include "libtaxcalculator.h"

void deleteEverythingTree(GtkWidget *widget, DataTable *dt){
	g_print("Deleted everything\n");
	gtk_list_store_clear(GTK_LIST_STORE(dt->store));
}

TCErrorCode TCCreateDataTable(DataTable *dt){
	if(dt == NULL){
		return TC_ERROR_NULLPTR;
	}

	dt->view = gtk_tree_view_new();

	//store = gtk_list_store_new(3, G_TYPE_DOUBLE, G_TYPE_DOUBLE, G_TYPE_DOUBLE);
	dt->store = gtk_list_store_new(3, G_TYPE_INT, G_TYPE_INT, G_TYPE_INT);

  	dt->renderer = gtk_cell_renderer_text_new();


	gtk_tree_view_insert_column_with_attributes(GTK_TREE_VIEW (dt->view),
                                               -1,      
                                               "Valor con IVA",  
                                               dt->renderer,
                                               "text", 0,
                                               NULL);

  	dt->renderer = gtk_cell_renderer_text_new();
  	gtk_tree_view_insert_column_with_attributes(GTK_TREE_VIEW (dt->view),
                                               -1,      
                                               "Valor sin IVA",  
                                               dt->renderer,
                                               "text", 1,
                                               NULL);

  	dt->renderer = gtk_cell_renderer_text_new();
  	gtk_tree_view_insert_column_with_attributes(GTK_TREE_VIEW (dt->view),
                                               -1,      
                                               "5% sin IVA",  
                                               dt->renderer,
                                               "text", 2,
                                               NULL);

  	gtk_tree_view_set_model(GTK_TREE_VIEW(dt->view), dt->store);
	g_object_unref(dt->store);

	dt->container = gtk_scrolled_window_new(NULL, NULL);
	gtk_container_add(GTK_CONTAINER(dt->container), dt->view);
	gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(dt->container),
                                   GTK_POLICY_AUTOMATIC, GTK_POLICY_ALWAYS);


	dt->clear = gtk_button_new_with_label("Borrar");
	g_signal_connect(GTK_OBJECT(dt->clear), "clicked", G_CALLBACK(deleteEverythingTree), dt);
	return TC_NO_ERROR;
}

void TCCreateNewTableRow(DataTable *dt, double valueWithTaxes, double valueWithoutTaxes, double valueFivePercentWithoutTaxes){
 	gtk_list_store_append(dt->store, &(dt->iter));
 	gtk_list_store_set(dt->store, &(dt->iter),
                      0, (int) valueWithTaxes,
                      1, (int) valueWithoutTaxes,
                      2, (int) valueFivePercentWithoutTaxes,
                      -1);
}

TCErrorCode TCSetSizeDataTable(DataTable *dt, int x, int y){
	if(dt == NULL){
		return TC_ERROR_NULLPTR;
	}

	dt->coordinates.x = x;
	dt->coordinates.y = y;
	return TC_NO_ERROR;
}

TCErrorCode TCPlaceDataTable(CalculatorApp *ca){
	if(ca == NULL){
		return TC_ERROR_NULLPTR;
	}

	gtk_table_attach_defaults(GTK_TABLE(ca->mainTable), ca->spreadsheet.container,
		ca->spreadsheet.coordinates.x,
		DEFAULT_TABLE_WIDTH,
		ca->spreadsheet.coordinates.y,
		DEFAULT_TABLE_HEIGHT - 1
	);

	gtk_table_attach_defaults(GTK_TABLE(ca->mainTable), ca->spreadsheet.clear,
		ca->spreadsheet.coordinates.x,
		DEFAULT_TABLE_WIDTH,
		DEFAULT_TABLE_HEIGHT - 1,
		DEFAULT_TABLE_HEIGHT
	);


	return TC_NO_ERROR;
}
