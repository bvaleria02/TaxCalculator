#include <gtk/gtk.h>
#include <stdbool.h>
#include <string.h>
#include "libtaxcalculator.h"

TCErrorCode TCCreateMenu(CalculatorApp *ca){
	if(ca == NULL){
		return TC_ERROR_NULLPTR;
	}

	ca->menu.menubar = gtk_menu_bar_new();
	ca->menu.fileItem = gtk_menu_item_new_with_label("Archivo");
//	gtk_container_add(GTK_CONTAINER(ca->vbox), ca->menu.menubar);
	gtk_container_add(GTK_CONTAINER(ca->menu.menubar), ca->menu.fileItem);

	ca->menu.menuFile = gtk_menu_new();
	gtk_menu_item_set_submenu(GTK_MENU_ITEM(ca->menu.fileItem), ca->menu.menuFile);

	ca->menu.blank = gtk_menu_item_new_with_label("Nuevo");
	ca->menu.load = gtk_menu_item_new_with_label("Abrir");
	ca->menu.save = gtk_menu_item_new_with_label("Guardar");
	ca->menu.quit = gtk_menu_item_new_with_label("Salir");

	gtk_menu_shell_append(GTK_MENU_SHELL(ca->menu.menuFile), ca->menu.blank);
	gtk_menu_shell_append(GTK_MENU_SHELL(ca->menu.menuFile), ca->menu.load);
	gtk_menu_shell_append(GTK_MENU_SHELL(ca->menu.menuFile), ca->menu.save);
	gtk_menu_shell_append(GTK_MENU_SHELL(ca->menu.menuFile), ca->menu.quit);

	gtk_menu_shell_append(GTK_MENU_SHELL(ca->menu.menubar), ca->menu.fileItem);

	gtk_box_pack_start(GTK_BOX(ca->vbox), ca->menu.menubar, false, false, 0);
	return TC_NO_ERROR;
}

void TCBlankAdapter(GtkWidget *widget, CalculatorApp *ca){
	TCResponse response;	

	if(ca->spreadsheet.hasChanged){
		response = TCAskSaveBeforeLoadDialog(ca);
	}

	if(response == TC_RESPONSE_YES){
		TCSaveData(ca);
	}

	TCResetAllEntries(ca);
	deleteEverythingTree(NULL, &(ca->spreadsheet));
	ca->spreadsheet.hasChanged = 0;
}

void TCSaveAdapter(GtkWidget *widget, CalculatorApp *ca){
	TCSaveData(ca);
}

void TCLoadAdapter(GtkWidget *widget, CalculatorApp *ca){
	TCResponse response;	

	if(ca->spreadsheet.hasChanged){
		response = TCAskSaveBeforeLoadDialog(ca);
	}

	if(response == TC_RESPONSE_YES){
		TCSaveData(ca);
	}

	TCLoadData(ca);
}


TCErrorCode TCAddActionsMenu(CalculatorApp *ca){
	if(ca == NULL){
		return TC_ERROR_NULLPTR;
	}

	g_signal_connect(GTK_OBJECT(ca->menu.blank), "activate", G_CALLBACK(TCBlankAdapter), ca);
	g_signal_connect(GTK_OBJECT(ca->menu.save), "activate", G_CALLBACK(TCSaveAdapter), ca);
	g_signal_connect(GTK_OBJECT(ca->menu.load), "activate", G_CALLBACK(TCLoadAdapter), ca);
	g_signal_connect(GTK_OBJECT(ca->menu.quit), "activate", G_CALLBACK(TCDestroyWindow), ca);


	return TC_NO_ERROR;
}
