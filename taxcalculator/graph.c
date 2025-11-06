#include <gtk/gtk.h>
#include <cairo.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include "libtaxcalculator.h"


void TCConnectorCreateGraph(GtkWidget *widget, CalculatorApp *ca){
	TCCreateGraph(ca);
}

void TCDestroyGraphWindow(GtkWidget *widget, CalculatorApp *ca){
    gtk_widget_destroy(ca->graph.window);
}

int TCGetPointXAbsGraph(CalculatorApp *ca, double x){
	return (ca->graph.width * x);
}

int TCGetPointYAbsGraph(CalculatorApp *ca, double y){
	return (ca->graph.height * y);
}

int TCGetPointXRelGraph(CalculatorApp *ca, double x0, double x){
	int absX0 = TCGetPointXAbsGraph(ca, x0);
	int absX  = TCGetPointXAbsGraph(ca, x);
	int diff  = absX - absX0;
	return absX0 + (diff * ca->graph.scale);
}

int TCGetPointYRelGraph(CalculatorApp *ca, double y0, double y){
	int absY0 = TCGetPointYAbsGraph(ca, y0);
	int absY  = TCGetPointYAbsGraph(ca, y);
	int diff  = absY - absY0;
	return absY0 + (diff * ca->graph.scale);
}

void TCDrawBackgroundGraph(CalculatorApp *ca, cairo_t *cr){
	cairo_set_source_rgb(cr, 1, 1, 1);
	cairo_paint(cr);
}

void TCCalculateViewDimGraph(CalculatorApp *ca, double *expX, double *expY){
	ca->graph.minXView = ca->graph.minX + ca->graph.offsetX;

	if(ca->graph.useMinZero){
		ca->graph.minYView = 0;
	} else {
		ca->graph.minYView = ca->graph.minY + ca->graph.offsetY;
	}

	ca->graph.maxXView = ca->graph.minXView + ((ca->graph.maxX - ca->graph.minXView) * ca->graph.scale);
	ca->graph.maxYView = ca->graph.minYView + ((ca->graph.maxY - ca->graph.minYView) * ca->graph.scale);

	double diffX = ca->graph.maxXView - ca->graph.minXView;
	(*expX) = floor(log10(diffX));

	ca->graph.minXView = floor(ca->graph.minXView * pow(10, -(*expX))) * pow(10, (*expX));
	ca->graph.maxXView = ceil(ca->graph.maxXView * pow(10, -(*expX))) * pow(10, (*expX));
	double diffX2 = ca->graph.maxXView - ca->graph.minXView;

	double diffY = ca->graph.maxYView - ca->graph.minYView;
	(*expY) = floor(log10(diffY));

	ca->graph.minYView = floor(ca->graph.minYView * pow(10, -(*expY))) * pow(10, (*expY));
	ca->graph.maxYView = ceil(ca->graph.maxYView * pow(10, -(*expY))) * pow(10, (*expY));
	double diffY2 = ca->graph.maxYView - ca->graph.minYView;
}

double TCGetPointInBetween(double minAbsValue, double maxAbsValue, double minRelValue, double maxRelValue, double realValue){
	double absDiff = maxAbsValue - minAbsValue;
	double relDiff = maxRelValue - minRelValue;
	return minAbsValue + absDiff * ((realValue - minRelValue) / relDiff);
}

void TCDrawGridGraph(CalculatorApp *ca, cairo_t *cr){
	cairo_set_source_rgb(cr, 0.85, 0.85, 0.85);

	double x;
	double y;
	double expX;
	double expY;
	TCCalculateViewDimGraph(ca, &expX, &expY);

	for(double i = ca->graph.minXView; i < ca->graph.maxXView; i += pow(10, expX)){

		cairo_set_source_rgb(cr, 0.95, 0.95, 0.95);
		for(double j = i; j < i + pow(10, expX); j += pow(10, expX - 1)){
			
			if(j == i) continue;

			x = TCGetPointInBetween(0.1, 0.9, ca->graph.minXView, ca->graph.maxXView, j);

			cairo_move_to(cr,
				TCGetPointXAbsGraph(ca, x),
				TCGetPointYAbsGraph(ca, 0.1)
			);

			cairo_line_to(cr,
				TCGetPointXAbsGraph(ca, x),
				TCGetPointYAbsGraph(ca, 0.9)
			);

			cairo_stroke(cr);
		}

		x = TCGetPointInBetween(0.1, 0.9, ca->graph.minXView, ca->graph.maxXView, i);
		cairo_set_source_rgb(cr, 0.85, 0.85, 0.85);

		cairo_move_to(cr,
			TCGetPointXAbsGraph(ca, x),
			TCGetPointYAbsGraph(ca, 0.1)
		);

		cairo_line_to(cr,
			TCGetPointXAbsGraph(ca, x),
			TCGetPointYAbsGraph(ca, 0.9)
		);

		cairo_stroke(cr);
	}
	
	for(double i = ca->graph.minYView; i < ca->graph.maxYView; i += pow(10, expY)){
		
		cairo_set_source_rgb(cr, 0.95, 0.95, 0.95);
		for(double j = i; j < i + pow(10, expY); j += pow(10, expY - 1)){

			if(j == i) continue;

			y = TCGetPointInBetween(0.1, 0.9, ca->graph.minYView, ca->graph.maxYView, j);

			cairo_move_to(cr,
				TCGetPointXAbsGraph(ca, 0.1),
				TCGetPointYAbsGraph(ca, y)
			);

			cairo_line_to(cr,
				TCGetPointXAbsGraph(ca, 0.9),
				TCGetPointYAbsGraph(ca, y)
			);

			cairo_stroke(cr);
		}

		y = TCGetPointInBetween(0.1, 0.9, ca->graph.minYView, ca->graph.maxYView, i);
		cairo_set_source_rgb(cr, 0.85, 0.85, 0.85);

		cairo_move_to(cr,
			TCGetPointXAbsGraph(ca, 0.1),
			TCGetPointYAbsGraph(ca, y)
		);

		cairo_line_to(cr,
			TCGetPointXAbsGraph(ca, 0.9),
			TCGetPointYAbsGraph(ca, y)
		);

		cairo_stroke(cr);
	}
}

void TCDrawCoordinatesGraph(CalculatorApp *ca, cairo_t *cr){
	cairo_set_source_rgb(cr, 0.7, 0.7, 0.7);
	cairo_set_line_width(cr, 1);

	cairo_rectangle(cr,
		TCGetPointXAbsGraph(ca, 0.1),
		TCGetPointYAbsGraph(ca, 0.1),
		TCGetPointXAbsGraph(ca, 0.8),
		TCGetPointYAbsGraph(ca, 0.8)
	);
	cairo_stroke(cr);

	cairo_set_source_rgb(cr, 0.2, 0.2, 0.2);

	cairo_move_to(cr,
		TCGetPointXAbsGraph(ca, 0.1),
		TCGetPointYAbsGraph(ca, 0.9)
	);

	cairo_line_to(cr,
		TCGetPointXAbsGraph(ca, 0.1),
		TCGetPointYAbsGraph(ca, 0.1)
	);

	cairo_move_to(cr,
		TCGetPointXAbsGraph(ca, 0.1),
		TCGetPointYAbsGraph(ca, 0.9)
	);

	cairo_line_to(cr,
		TCGetPointXAbsGraph(ca, 0.9),
		TCGetPointYAbsGraph(ca, 0.9)
	);

	cairo_stroke(cr);
}

void TCDrawLineValuesGraph(CalculatorApp *ca, cairo_t *cr){
	cairo_set_source_rgb(cr, 0, 0, 0);

	double x;
	double y;
	double expX;
	double expY;
	TCCalculateViewDimGraph(ca, &expX, &expY);

	char buffer[MAX_BUFFER_SIZE];

	for(double i = ca->graph.minXView; i <= ca->graph.maxXView; i += pow(10, expX)){
		x = TCGetPointInBetween(0.1, 0.9, ca->graph.minXView, ca->graph.maxXView, i);

		cairo_move_to(cr,
			TCGetPointXAbsGraph(ca, x),
			TCGetPointYAbsGraph(ca, 0.94)
		);

		convertDoubleToString(buffer, MAX_BUFFER_SIZE, i);
		cairo_show_text(cr, buffer);

		cairo_stroke(cr);

		cairo_move_to(cr,
			TCGetPointXAbsGraph(ca, x),
			TCGetPointYAbsGraph(ca, 0.89)
		);

		cairo_line_to(cr,
			TCGetPointXAbsGraph(ca, x),
			TCGetPointYAbsGraph(ca, 0.91)
		);

		cairo_stroke(cr);

	}
	
	for(double i = ca->graph.minYView; i <= ca->graph.maxYView; i += pow(10, expY)){
		y = TCGetPointInBetween(0.9, 0.1, ca->graph.minYView, ca->graph.maxYView, i);
		cairo_stroke(cr);

		cairo_move_to(cr,
			TCGetPointXAbsGraph(ca, 0.06),
			TCGetPointYAbsGraph(ca, y)
		);

		convertDoubleToString(buffer, MAX_BUFFER_SIZE, i);
		cairo_show_text(cr, buffer);

		cairo_stroke(cr);

		cairo_move_to(cr,
			TCGetPointXAbsGraph(ca, 0.09),
			TCGetPointYAbsGraph(ca, y)
		);

		cairo_line_to(cr,
			TCGetPointXAbsGraph(ca, 0.11),
			TCGetPointYAbsGraph(ca, y)
		);

		cairo_stroke(cr);
	}
}

TCErrorCode TCRetrieveDataFromTable(CalculatorApp *ca){
	TCDestroyGraphData(ca);

	ca->graph.dataSize = 0;
	gtk_tree_model_foreach(GTK_TREE_MODEL(ca->spreadsheet.store), G_CALLBACK(TCCountElementsTable), ca);

	if(ca->graph.dataSize == 0){
		TC_NO_ERROR;
	}

	ca->graph.priceWithTaxes 	= (long *)malloc(sizeof(long) * ca->graph.dataSize);
	if(ca->graph.priceWithTaxes == NULL) goto cleanup;

	ca->graph.priceWithoutTaxes = (long *)malloc(sizeof(long) * ca->graph.dataSize);
	if(ca->graph.priceWithoutTaxes == NULL) goto cleanup;

	ca->graph.priceFivePercent 	= (long *)malloc(sizeof(long) * ca->graph.dataSize);
	if(ca->graph.priceFivePercent == NULL) goto cleanup;

	ca->graph.sizeCounter = 0;
	gtk_tree_model_foreach(GTK_TREE_MODEL(ca->spreadsheet.store), G_CALLBACK(TCCopyElementsFromTable), ca);

	return TC_NO_ERROR;

cleanup:
	TCDestroyGraphData(ca);
	return TC_NO_ERROR;
}

TCErrorCode TCGetLimitsTable(CalculatorApp *ca){
	if(ca->graph.dataSize == 0){
		TC_NO_ERROR;
	}

	ca->graph.minX = 0;
	ca->graph.maxX = ca->graph.dataSize - 1;

	for(long i = 0; i < ca->graph.dataSize; i++){
		if(i == 0){
			if(ca->graph.showPriceWithTaxes){
				ca->graph.minY = ca->graph.priceWithTaxes[i];
				ca->graph.maxY = ca->graph.priceWithTaxes[i];
			} else if(ca->graph.showPriceWithoutTaxes){
				ca->graph.minY = ca->graph.priceWithoutTaxes[i];
				ca->graph.maxY = ca->graph.priceWithoutTaxes[i];
			} else if(ca->graph.showPriceFivePercent){
				ca->graph.minY = ca->graph.priceFivePercent[i];
				ca->graph.maxY = ca->graph.priceFivePercent[i];
			}
		}

		if(ca->graph.maxY < ca->graph.priceWithTaxes[i] && ca->graph.showPriceWithTaxes){
			ca->graph.maxY = ca->graph.priceWithTaxes[i];
		}

		if(ca->graph.maxY < ca->graph.priceWithoutTaxes[i] && ca->graph.showPriceWithoutTaxes){
			ca->graph.maxY = ca->graph.priceWithoutTaxes[i];
		}

		if(ca->graph.maxY < ca->graph.priceFivePercent[i] && ca->graph.showPriceFivePercent){
			ca->graph.maxY = ca->graph.priceFivePercent[i];
		}

		if(ca->graph.minY > ca->graph.priceWithTaxes[i] && ca->graph.showPriceWithTaxes){
			ca->graph.minY = ca->graph.priceWithTaxes[i];
		}

		if(ca->graph.minY > ca->graph.priceWithoutTaxes[i] && ca->graph.showPriceWithoutTaxes){
			ca->graph.minY = ca->graph.priceWithoutTaxes[i];
		}

		if(ca->graph.minY > ca->graph.priceFivePercent[i] && ca->graph.showPriceFivePercent){
			ca->graph.minY = ca->graph.priceFivePercent[i];
		}
	}

	return TC_NO_ERROR;
}

void TCDrawingPipelineGraph(GtkWidget *widget, cairo_t *cr, CalculatorApp *ca){
	TCDrawBackgroundGraph(ca, cr);
	TCDrawGridGraph(ca, cr);
	TCDrawCoordinatesGraph(ca, cr);
	TCDrawLineValuesGraph(ca, cr);

	if(ca->graph.showPriceWithTaxes){
		cairo_set_source_rgb(cr, 0.367, 0.582, 0.945);
		TCDrawDataGraph(ca, cr, ca->graph.priceWithTaxes);
	}

	if(ca->graph.showPriceWithoutTaxes){
		cairo_set_source_rgb(cr, 0.945, 0.813, 0.321);
		TCDrawDataGraph(ca, cr, ca->graph.priceWithoutTaxes);
	}

	if(ca->graph.showPriceFivePercent){
		cairo_set_source_rgb(cr, 0.656, 0.363, 0.828);
		TCDrawDataGraph(ca, cr, ca->graph.priceFivePercent);
	}

	TCDrawLegendGraph(ca, cr);
}

gboolean TCOnDrawGraph(GtkWidget *widget, GdkEventExpose *event, CalculatorApp *ca){

	cairo_t *cr = gdk_cairo_create(widget->window);
	GtkAllocation allocation;
	gtk_widget_get_allocation(ca->graph.area, &allocation);
	ca->graph.width = allocation.width;
	ca->graph.height = allocation.height;

	TCDrawingPipelineGraph(widget, cr, ca);

	cairo_destroy(cr);
	return false;
}

void TCSubOffsetXGraph(GtkWidget *widget, CalculatorApp *ca){
	ca->graph.offsetX -= DEFAULT_OFFSET_X_INCREMENT;
	gtk_widget_queue_draw(ca->graph.area);
}

void TCAddOffsetXGraph(GtkWidget *widget, CalculatorApp *ca){
	ca->graph.offsetX += DEFAULT_OFFSET_X_INCREMENT;
	gtk_widget_queue_draw(ca->graph.area);
}

void TCSubOffsetYGraph(GtkWidget *widget, CalculatorApp *ca){
	ca->graph.offsetY -= DEFAULT_OFFSET_Y_INCREMENT;
	gtk_widget_queue_draw(ca->graph.area);
}

void TCAddOffsetYGraph(GtkWidget *widget, CalculatorApp *ca){
	ca->graph.offsetY += DEFAULT_OFFSET_Y_INCREMENT;
	gtk_widget_queue_draw(ca->graph.area);
}

void TCSubScaleGraph(GtkWidget *widget, CalculatorApp *ca){
	ca->graph.scale /= DEFAULT_SCALE_INCREMENT;
	gtk_widget_queue_draw(ca->graph.area);
}

void TCAddScaleGraph(GtkWidget *widget, CalculatorApp *ca){
	ca->graph.scale *= DEFAULT_SCALE_INCREMENT;
	gtk_widget_queue_draw(ca->graph.area);
}

void TCResetViewGraph(GtkWidget *widget, CalculatorApp *ca){
	ca->graph.scale 	= DEFAULT_SCALE;
	ca->graph.offsetX 	= DEFAULT_OFFSET_X;
	ca->graph.offsetY 	= DEFAULT_OFFSET_Y;
	TCRetrieveDataFromTable(ca);
	TCGetLimitsTable(ca);
	ca->graph.useMinZero = 1;
	gtk_widget_queue_draw(ca->graph.area);
}

void TCSubMinXGraph(GtkWidget *widget, CalculatorApp *ca){
	ca->graph.minX -= 1;
	gtk_widget_queue_draw(ca->graph.area);
}

void TCAddMinXGraph(GtkWidget *widget, CalculatorApp *ca){
	ca->graph.minX += 1;
	gtk_widget_queue_draw(ca->graph.area);
}

void TCSubMinYGraph(GtkWidget *widget, CalculatorApp *ca){
	ca->graph.minY -= 1;
	gtk_widget_queue_draw(ca->graph.area);
}

void TCAddMinYGraph(GtkWidget *widget, CalculatorApp *ca){
	ca->graph.minY += 1;
	gtk_widget_queue_draw(ca->graph.area);
}

void TCSubMaxXGraph(GtkWidget *widget, CalculatorApp *ca){
	ca->graph.maxX -= 1;
	gtk_widget_queue_draw(ca->graph.area);
}

void TCAddMaxXGraph(GtkWidget *widget, CalculatorApp *ca){
	ca->graph.maxX += 1;
	gtk_widget_queue_draw(ca->graph.area);
}

void TCSubMaxYGraph(GtkWidget *widget, CalculatorApp *ca){
	ca->graph.maxY -= 1;
	gtk_widget_queue_draw(ca->graph.area);
}

void TCAddMaxYGraph(GtkWidget *widget, CalculatorApp *ca){
	ca->graph.maxY += 1;
	gtk_widget_queue_draw(ca->graph.area);
}

void TCChangeMinimumToDataGraph(GtkWidget *widget, CalculatorApp *ca){
	ca->graph.useMinZero = 0;
	gtk_widget_queue_draw(ca->graph.area);
}

void TCChangeMinimumToZeroGraph(GtkWidget *widget, CalculatorApp *ca){
	ca->graph.useMinZero = 1;
	gtk_widget_queue_draw(ca->graph.area);
}

void TCTogglePriceWithTaxes(GtkWidget *widget, CalculatorApp *ca){
	ca->graph.showPriceWithTaxes = !(ca->graph.showPriceWithTaxes);
	TCGetLimitsTable(ca);
	gtk_widget_queue_draw(ca->graph.area);
}

void TCTogglePriceWithoutTaxes(GtkWidget *widget, CalculatorApp *ca){
	ca->graph.showPriceWithoutTaxes = !(ca->graph.showPriceWithoutTaxes);
	TCGetLimitsTable(ca);
	gtk_widget_queue_draw(ca->graph.area);
}

void TCTogglePriceFivePercent(GtkWidget *widget, CalculatorApp *ca){
	ca->graph.showPriceFivePercent = !(ca->graph.showPriceFivePercent);
	TCGetLimitsTable(ca);
	gtk_widget_queue_draw(ca->graph.area);
}

TCErrorCode TCCreateButtonsGraph(CalculatorApp *ca){
	//ca->graph.buttonsHBox = gtk_hbox_new(false, 0);
	ca->graph.buttonsHBox = gtk_table_new(3, 8, true);

	ca->graph.subOffsetX = gtk_button_new_with_label("L");
	ca->graph.addOffsetX = gtk_button_new_with_label("R");
	ca->graph.subOffsetY = gtk_button_new_with_label("U");
	ca->graph.addOffsetY = gtk_button_new_with_label("D");
	ca->graph.addScale   = gtk_button_new_with_label("Z+");
	ca->graph.subScale   = gtk_button_new_with_label("Z-");
	ca->graph.reset   	 = gtk_button_new_with_label("Reiniciar");
	ca->graph.useAbsMin	 = gtk_button_new_with_label("Usar mínimo de datos");

	ca->graph.subMinX = gtk_button_new_with_label("-mx");
	ca->graph.addMinX = gtk_button_new_with_label("+mx");
	ca->graph.subMinY = gtk_button_new_with_label("-my");
	ca->graph.addMinY = gtk_button_new_with_label("+my");

	ca->graph.subMaxX = gtk_button_new_with_label("-Mx");
	ca->graph.addMaxX = gtk_button_new_with_label("+Mx");
	ca->graph.subMaxY = gtk_button_new_with_label("-My");
	ca->graph.addMaxY = gtk_button_new_with_label("+My");

/*
	gtk_container_add(GTK_CONTAINER(ca->graph.buttonsHBox), ca->graph.subOffsetX);
	gtk_container_add(GTK_CONTAINER(ca->graph.buttonsHBox), ca->graph.addOffsetX);
	gtk_container_add(GTK_CONTAINER(ca->graph.buttonsHBox), ca->graph.subOffsetY);
	gtk_container_add(GTK_CONTAINER(ca->graph.buttonsHBox), ca->graph.addOffsetY);
*/	
/*
	gtk_box_pack_start(GTK_BOX(ca->graph.buttonsHBox), ca->graph.subOffsetX, false, false, 0);
	gtk_box_pack_start(GTK_BOX(ca->graph.buttonsHBox), ca->graph.addOffsetX, false, false, 0);
	gtk_box_pack_start(GTK_BOX(ca->graph.buttonsHBox), ca->graph.subOffsetY, false, false, 0);
	gtk_box_pack_start(GTK_BOX(ca->graph.buttonsHBox), ca->graph.addOffsetY, false, false, 0);
	*/

	gtk_table_attach_defaults(GTK_TABLE(ca->graph.buttonsHBox), ca->graph.subOffsetX, 0, 1, 0, 1);
	gtk_table_attach_defaults(GTK_TABLE(ca->graph.buttonsHBox), ca->graph.addOffsetX, 1, 2, 0, 1);
	gtk_table_attach_defaults(GTK_TABLE(ca->graph.buttonsHBox), ca->graph.subOffsetY, 3, 4, 0, 1);
	gtk_table_attach_defaults(GTK_TABLE(ca->graph.buttonsHBox), ca->graph.addOffsetY, 4, 5, 0, 1);
	gtk_table_attach_defaults(GTK_TABLE(ca->graph.buttonsHBox), ca->graph.subScale,   6 ,7, 0, 1);
	gtk_table_attach_defaults(GTK_TABLE(ca->graph.buttonsHBox), ca->graph.addScale,   7, 8, 0, 1);
	gtk_table_attach_defaults(GTK_TABLE(ca->graph.buttonsHBox), ca->graph.reset,   	  6, 8, 1, 2);
	gtk_table_attach_defaults(GTK_TABLE(ca->graph.buttonsHBox), ca->graph.useAbsMin,  6, 8, 2, 3);

	gtk_table_attach_defaults(GTK_TABLE(ca->graph.buttonsHBox), ca->graph.subMinX, 0, 1, 1, 2);
	gtk_table_attach_defaults(GTK_TABLE(ca->graph.buttonsHBox), ca->graph.addMinX, 1, 2, 1, 2);
	gtk_table_attach_defaults(GTK_TABLE(ca->graph.buttonsHBox), ca->graph.subMinY, 3, 4, 1, 2);
	gtk_table_attach_defaults(GTK_TABLE(ca->graph.buttonsHBox), ca->graph.addMinY, 4, 5, 1, 2);
	gtk_table_attach_defaults(GTK_TABLE(ca->graph.buttonsHBox), ca->graph.subMaxX, 0, 1, 2, 3);
	gtk_table_attach_defaults(GTK_TABLE(ca->graph.buttonsHBox), ca->graph.addMaxX, 1, 2, 2, 3);
	gtk_table_attach_defaults(GTK_TABLE(ca->graph.buttonsHBox), ca->graph.subMaxY, 3, 4, 2, 3);
	gtk_table_attach_defaults(GTK_TABLE(ca->graph.buttonsHBox), ca->graph.addMaxY, 4, 5, 2, 3);

	g_signal_connect(GTK_OBJECT(ca->graph.subOffsetX), "clicked", G_CALLBACK(TCSubOffsetXGraph), ca);
	g_signal_connect(GTK_OBJECT(ca->graph.addOffsetX), "clicked", G_CALLBACK(TCAddOffsetXGraph), ca);
	g_signal_connect(GTK_OBJECT(ca->graph.subOffsetY), "clicked", G_CALLBACK(TCSubOffsetYGraph), ca);
	g_signal_connect(GTK_OBJECT(ca->graph.addOffsetY), "clicked", G_CALLBACK(TCAddOffsetYGraph), ca);
	g_signal_connect(GTK_OBJECT(ca->graph.subScale  ), "clicked", G_CALLBACK(TCSubScaleGraph), ca);
	g_signal_connect(GTK_OBJECT(ca->graph.addScale  ), "clicked", G_CALLBACK(TCAddScaleGraph), ca);
	g_signal_connect(GTK_OBJECT(ca->graph.reset     ), "clicked", G_CALLBACK(TCResetViewGraph), ca);
	g_signal_connect(GTK_OBJECT(ca->graph.useAbsMin ), "clicked", G_CALLBACK(TCChangeMinimumToDataGraph), ca);

	g_signal_connect(GTK_OBJECT(ca->graph.subMinX), "clicked", G_CALLBACK(TCSubMinXGraph), ca);
	g_signal_connect(GTK_OBJECT(ca->graph.addMinX), "clicked", G_CALLBACK(TCAddMinXGraph), ca);
	g_signal_connect(GTK_OBJECT(ca->graph.subMinY), "clicked", G_CALLBACK(TCSubMinYGraph), ca);
	g_signal_connect(GTK_OBJECT(ca->graph.addMinY), "clicked", G_CALLBACK(TCAddMinYGraph), ca);
	g_signal_connect(GTK_OBJECT(ca->graph.subMaxX), "clicked", G_CALLBACK(TCSubMaxXGraph), ca);
	g_signal_connect(GTK_OBJECT(ca->graph.addMaxX), "clicked", G_CALLBACK(TCAddMaxXGraph), ca);
	g_signal_connect(GTK_OBJECT(ca->graph.subMaxY), "clicked", G_CALLBACK(TCSubMaxYGraph), ca);
	g_signal_connect(GTK_OBJECT(ca->graph.addMaxY), "clicked", G_CALLBACK(TCAddMaxYGraph), ca);
	//gtk_container_add(GTK_CONTAINER(ca->graph.vbox), ca->graph.buttonsHBox);
	gtk_box_pack_start(GTK_BOX(ca->graph.vbox), ca->graph.buttonsHBox, false, false, 0);
	gtk_container_set_border_width(GTK_CONTAINER(ca->graph.buttonsHBox), 16);

	return TC_NO_ERROR;
}

TCErrorCode TCCreateGraph(CalculatorApp *ca){
	TCInitGraph(ca);
	ca->graph.window = gtk_window_new(GTK_WINDOW_TOPLEVEL);

	gtk_window_set_title(GTK_WINDOW(ca->graph.window), "Gráfica");
	g_signal_connect(ca->graph.window, "destroy", G_CALLBACK(TCDestroyGraphWindow), ca);
	gtk_container_set_border_width(GTK_CONTAINER(ca->graph.window), 0);

	ca->graph.area = gtk_drawing_area_new();
	gtk_widget_set_size_request(ca->graph.area, 400, 400);
	g_signal_connect(G_OBJECT(ca->graph.area), "expose-event", G_CALLBACK(TCOnDrawGraph), ca);

	ca->graph.container = gtk_vbox_new(false, 0);
	gtk_container_set_border_width(GTK_CONTAINER(ca->graph.container), 16);

	ca->graph.vbox = gtk_vbox_new(false, 0);
	TCCreateMenuGraph(ca);
	TCAddActionsMenuGraph(ca);
	gtk_container_add(GTK_CONTAINER(ca->graph.container), ca->graph.area);

	TCCreateButtonsGraph(ca);

	TCRetrieveDataFromTable(ca);
	TCGetLimitsTable(ca);

	gtk_container_add(GTK_CONTAINER(ca->graph.vbox), ca->graph.container);
	//gtk_box_pack_start(GTK_BOX(ca->graph.vbox), ca->graph.container, false, false, 0);
	gtk_container_add(GTK_CONTAINER(ca->graph.window), ca->graph.vbox);

	gtk_widget_show_all(ca->graph.window);

	return TC_NO_ERROR;
}

TCErrorCode TCInitGraph(CalculatorApp *ca){
	ca->graph.priceWithTaxes = NULL;
	ca->graph.priceWithoutTaxes = NULL;
	ca->graph.priceFivePercent = NULL;
	ca->graph.dataSize = 0;

	ca->graph.useMinZero = 1;
	ca->graph.showPriceWithTaxes = 1;
	ca->graph.showPriceWithoutTaxes = 1;
	ca->graph.showPriceFivePercent = 1;

	ca->graph.offsetX = DEFAULT_OFFSET_X;
	ca->graph.offsetY = DEFAULT_OFFSET_Y;
	ca->graph.scale = DEFAULT_SCALE;

	return TC_NO_ERROR;
}

TCErrorCode TCDestroyGraphData(CalculatorApp *ca){
	if(ca->graph.priceWithTaxes != NULL){
		free(ca->graph.priceWithTaxes);
		ca->graph.priceWithTaxes = NULL;
	}

	if(ca->graph.priceWithoutTaxes != NULL){
		free(ca->graph.priceWithoutTaxes);
		ca->graph.priceWithoutTaxes = NULL;
	}

	if(ca->graph.priceFivePercent != NULL){
		free(ca->graph.priceFivePercent);
		ca->graph.priceFivePercent = NULL;
	}

	return TC_NO_ERROR;
}

TCErrorCode TCCreateMenuGraph(CalculatorApp *ca){
	ca->graph.menubar = gtk_menu_bar_new();

	ca->graph.fileItem = gtk_menu_item_new_with_label("Archivo");
	gtk_container_add(GTK_CONTAINER(ca->graph.menubar), ca->graph.fileItem);

	ca->graph.menuFile = gtk_menu_new();
	gtk_menu_item_set_submenu(GTK_MENU_ITEM(ca->graph.fileItem), ca->graph.menuFile);

	ca->graph.save = gtk_menu_item_new_with_label("Guardar");
	ca->graph.quit = gtk_menu_item_new_with_label("Salir");

	gtk_menu_shell_append(GTK_MENU_SHELL(ca->graph.menuFile), ca->graph.save);
	gtk_menu_shell_append(GTK_MENU_SHELL(ca->graph.menuFile), ca->graph.quit);

	ca->graph.canvasItem = gtk_menu_item_new_with_label("Lienzo");
	gtk_container_add(GTK_CONTAINER(ca->graph.menubar), ca->graph.canvasItem);

	ca->graph.menuCanvas = gtk_menu_new();
	gtk_menu_item_set_submenu(GTK_MENU_ITEM(ca->graph.canvasItem), ca->graph.menuCanvas);

	ca->graph.resetMenu  	  = gtk_menu_item_new_with_label("Reiniciar vista");
	ca->graph.useAbsZeroMenu  = gtk_menu_item_new_with_label("Usar mínimo desde datos");
	ca->graph.useTrueZeroMenu = gtk_menu_item_new_with_label("Usar mínimo cero");
	ca->graph.togglePriceWithTaxes 		= gtk_menu_item_new_with_label("Activar/Desactivar precio con IVA");
	ca->graph.togglePriceWithoutTaxes 	= gtk_menu_item_new_with_label("Activar/Desactivar precio sin IVA");
	ca->graph.togglePriceFivePercent 	= gtk_menu_item_new_with_label("Activar/Desactivar 5% sin IVA");

	gtk_menu_shell_append(GTK_MENU_SHELL(ca->graph.menuCanvas), ca->graph.resetMenu);
	gtk_menu_shell_append(GTK_MENU_SHELL(ca->graph.menuCanvas), ca->graph.useAbsZeroMenu);
	gtk_menu_shell_append(GTK_MENU_SHELL(ca->graph.menuCanvas), ca->graph.useTrueZeroMenu);
	gtk_menu_shell_append(GTK_MENU_SHELL(ca->graph.menuCanvas), ca->graph.togglePriceWithTaxes);
	gtk_menu_shell_append(GTK_MENU_SHELL(ca->graph.menuCanvas), ca->graph.togglePriceWithoutTaxes);
	gtk_menu_shell_append(GTK_MENU_SHELL(ca->graph.menuCanvas), ca->graph.togglePriceFivePercent);

	gtk_box_pack_start(GTK_BOX(ca->graph.vbox), ca->graph.menubar, false, false, 0);
	return TC_NO_ERROR;
}

TCErrorCode TCAddActionsMenuGraph(CalculatorApp *ca){
	if(ca == NULL){
		return TC_ERROR_NULLPTR;
	}

	g_signal_connect(GTK_OBJECT(ca->graph.save), "activate", G_CALLBACK(TCSaveGraph), ca);
	g_signal_connect(GTK_OBJECT(ca->graph.quit), "activate", G_CALLBACK(TCDestroyGraphWindow), ca);

	g_signal_connect(GTK_OBJECT(ca->graph.resetMenu), 		"activate", G_CALLBACK(TCResetViewGraph), ca);
	g_signal_connect(GTK_OBJECT(ca->graph.useAbsZeroMenu),  "activate", G_CALLBACK(TCChangeMinimumToDataGraph), ca);
	g_signal_connect(GTK_OBJECT(ca->graph.useTrueZeroMenu), "activate", G_CALLBACK(TCChangeMinimumToZeroGraph), ca);

	g_signal_connect(GTK_OBJECT(ca->graph.togglePriceWithTaxes), "activate", G_CALLBACK(TCTogglePriceWithTaxes), ca);
	g_signal_connect(GTK_OBJECT(ca->graph.togglePriceWithoutTaxes), "activate", G_CALLBACK(TCTogglePriceWithoutTaxes), ca);
	g_signal_connect(GTK_OBJECT(ca->graph.togglePriceFivePercent), "activate", G_CALLBACK(TCTogglePriceFivePercent), ca);

	return TC_NO_ERROR;
}

TCErrorCode TCDrawDataGraph(CalculatorApp *ca, cairo_t *cr, long *data){
	double x;
	double y;
	double expX;
	double expY;
	TCCalculateViewDimGraph(ca, &expX, &expY);
	cairo_set_line_width(cr, 1.5);

	double xl;
	double yl;

	for(long i = 0; i < ca->graph.dataSize; i++){

		x = TCGetPointInBetween(0.1, 0.9,
					ca->graph.minXView, ca->graph.maxXView, i);

		y = TCGetPointInBetween(0.9, 0.1,
					ca->graph.minYView,ca->graph.maxYView, data[i]);

		xl = TCGetPointXAbsGraph(ca, x);
		yl = TCGetPointYAbsGraph(ca, y);

		if(i == 0){
			cairo_move_to(cr, x, y);
		} else {
			cairo_line_to(cr, xl, yl);
			cairo_stroke(cr);
		}


		cairo_arc(cr, xl, yl, 4, 0, 2 * M_PI);
		cairo_fill(cr);
		cairo_stroke(cr);

		cairo_move_to(cr, xl, yl);
	}
	cairo_stroke(cr);

	return TC_NO_ERROR;
}

TCErrorCode TCDrawLegendGraph(CalculatorApp *ca, cairo_t *cr){
	cairo_set_line_width(cr, 1);
	cairo_set_source_rgb(cr, 1, 1, 1);
	cairo_rectangle(cr,
		TCGetPointXAbsGraph(ca, 0.73),
		TCGetPointYAbsGraph(ca, 0.12),
		TCGetPointXAbsGraph(ca, 0.15),
		TCGetPointYAbsGraph(ca, 0.1)
	);
	cairo_fill(cr);
	cairo_stroke(cr);

	cairo_set_source_rgb(cr, 0, 0, 0);
	cairo_rectangle(cr,
		TCGetPointXAbsGraph(ca, 0.73),
		TCGetPointYAbsGraph(ca, 0.12),
		TCGetPointXAbsGraph(ca, 0.15),
		TCGetPointYAbsGraph(ca, 0.1)
	);
	cairo_stroke(cr);

	cairo_set_line_width(cr, 4);

	if(ca->graph.showPriceWithTaxes){
		cairo_set_source_rgb(cr, 0.367, 0.582, 0.945);
		cairo_move_to(cr,
			TCGetPointXAbsGraph(ca, 0.75),
			TCGetPointYAbsGraph(ca, 0.145)
		);

		cairo_line_to(cr,
			TCGetPointXAbsGraph(ca, 0.78),
			TCGetPointYAbsGraph(ca, 0.145)
		);

		cairo_stroke(cr);

		cairo_move_to(cr,
			TCGetPointXAbsGraph(ca, 0.8),
			TCGetPointYAbsGraph(ca, 0.145)
		);

		cairo_set_source_rgb(cr, 0.0, 0.0, 0.0);
		cairo_show_text(cr, "Precio con IVA");
		cairo_stroke(cr);
	}

	if(ca->graph.showPriceWithoutTaxes){
		cairo_set_source_rgb(cr, 0.945, 0.813, 0.321);
		cairo_move_to(cr,
			TCGetPointXAbsGraph(ca, 0.75),
			TCGetPointYAbsGraph(ca, 0.17)
		);

		cairo_line_to(cr,
			TCGetPointXAbsGraph(ca, 0.78),
			TCGetPointYAbsGraph(ca, 0.17)
		);

		cairo_stroke(cr);

		cairo_move_to(cr,
			TCGetPointXAbsGraph(ca, 0.8),
			TCGetPointYAbsGraph(ca, 0.17)
		);

		cairo_set_source_rgb(cr, 0.0, 0.0, 0.0);
		cairo_show_text(cr, "Precio sin IVA");
		cairo_stroke(cr);
	}

	if(ca->graph.showPriceFivePercent){
		cairo_set_source_rgb(cr, 0.656, 0.363, 0.828);
		cairo_move_to(cr,
			TCGetPointXAbsGraph(ca, 0.75),
			TCGetPointYAbsGraph(ca, 0.195)
		);

		cairo_line_to(cr,
			TCGetPointXAbsGraph(ca, 0.78),
			TCGetPointYAbsGraph(ca, 0.195)
		);

		cairo_stroke(cr);

		cairo_move_to(cr,
			TCGetPointXAbsGraph(ca, 0.8),
			TCGetPointYAbsGraph(ca, 0.195)
		);

		cairo_set_source_rgb(cr, 0.0, 0.0, 0.0);
		cairo_show_text(cr, "5% sin IVA");
		cairo_stroke(cr);
	}

	return TC_NO_ERROR;
}

void TCSaveGraph(GtkWidget *widget, CalculatorApp *ca){
	TCResponse response = TCGetFileDialog(ca);
	
	if(response != TC_RESPONSE_YES){
		return;
	}

	cairo_surface_t *surface = cairo_image_surface_create(
									CAIRO_FORMAT_RGB24,
									ca->graph.width,
									ca->graph.height
								);

	cairo_t *cr = cairo_create(surface);

	TCDrawingPipelineGraph(ca->graph.area, cr, ca);
	
	cairo_surface_write_to_png(surface, ca->fileMenu.filename);
	cairo_destroy(cr);
	cairo_surface_destroy(surface);
}
