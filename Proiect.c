#include <advanlys.h>
#include <utility.h>
#include <formatio.h>
#include <ansi_c.h>
#include <cvirte.h>		
#include <userint.h>
#include "Proiect.h"
#include "toolbox.h"
#include "Frecv.h"



static int mainPanel;
#define SAMPLE_RATE		0
#define NPOINTS			1

int waveInfo[2]; //waveInfo[0] = sampleRate, waveInfo[1] = number of elements

//vectorul ce contine datele semnalului filtrat in domeniul timp
double* filtru=0;

//frecventa de esantionare
double sampleRate = 0.0;

//numarul de esantioane
int npoints = 0;

//vectorul ce contine datele semnalului audio
double *waveData = 0;

double *anvelopa = 0;

int imgHandle;

int N=0;



double *ordinI(double alpha){
	
	for(int i=1;i<npoints;++i){
		
		filtru[i]=(1-alpha)*filtru[i-1]+alpha*waveData[i];
	}
	return filtru;
}

double *mediere(int n){
	
	double sum=0.0;
	for(int i=0;i<n-1;++i){
		
		sum=sum+waveData[i];
		
	}
	
	
	for(int k=0; k < npoints - n; k++){
		
		filtru[k] = sum /n;
		sum = sum - waveData[k] + waveData[k+n];
	
	}
	
	for(int  j = npoints - n; j<npoints; j++){
		
		filtru[j] = sum / n;
	}
	
	return filtru;
}

int main (int argc, char *argv[])
{
	int error = 0;
	
	nullChk (InitCVIRTE (0, argv, 0));
	errChk (mainPanel = LoadPanel (0, "Proiect.uir", MAIN_PANEL));
	errChk (freqPanel = LoadPanel (0, "Proiect.uir", FREQ_PANEL));

	errChk (DisplayPanel (mainPanel));
	errChk (RunUserInterface ());
	
Error:
	/* clean up */
	if (mainPanel > 0)
		DiscardPanel (mainPanel);
	return 0;
}

int CVICALLBACK OnExit (int panel, int event, void *callbackData,
						int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_GOT_FOCUS:

			break;
		case EVENT_LOST_FOCUS:

			break;
		case EVENT_CLOSE:
			QuitUserInterface (0);
			break;
	}
	return 0;
}

//Load wave data
int CVICALLBACK OnLoadBtt (int panel, int control, int event,
						   void *callbackData, int eventData1, int eventData2)
{
	double min = 0.0;
	double max = 0.0;
	int minIndex = 0;
	int maxIndex = 0;
	
	double dispersie = 0.0;
	double medie = 0.0;
	double mediana = 0.0;
	
	int zero = 0;
	
	
	int hist[100];
	int interv=30;
	double axis[100];
	
	switch (event)
	{
		case EVENT_COMMIT:
		
			
			//executa script python pentru conversia unui fisierului .wav in .txt
			LaunchExecutable("python main.py");
			
			//astept sa fie generate cele doua fisiere (modificati timpul daca este necesar
			Delay(4);
			
			//incarc informatiile privind rata de esantionare si numarul de valori
			FileToArray("wafeInfo.txt", waveInfo, VAL_INTEGER, 2, 1, VAL_GROUPS_TOGETHER, VAL_GROUPS_AS_COLUMNS, VAL_ASCII);//se incarca informatiile privind rata de esantionare si numarul de valori
			sampleRate = waveInfo[SAMPLE_RATE];
			npoints = waveInfo[NPOINTS];
			
			//alocare memorie pentru numarul de puncte
			
			waveData =(double*) malloc (npoints*sizeof(double));
			
			filtru =(double*) malloc (npoints*sizeof(double));
			
			//incarcare din fisierul .txt in memorie (vector)
			FileToArray("waveData.txt", waveData, VAL_DOUBLE, npoints, 1, VAL_GROUPS_TOGETHER, VAL_GROUPS_AS_COLUMNS, VAL_ASCII);	
			
			PlotY(panel, MAIN_PANEL_GRAPH, waveData, npoints, VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, VAL_CONNECTED_POINTS, VAL_RED);
			
			//calculare valmin, valmax, medie, dispersie, mediana
			
			MaxMin1D(waveData, npoints, &max,  &maxIndex, &min, &minIndex);//calcularea valorilor min/max (atat valoarea minima si cea maxima cat si indexul acestora)
			Mean(waveData,npoints,&medie);//valoarea medie a semnalului audio, oferind o indicatie despre nivelul sau mediu
			StdDev(waveData,npoints,&medie,&dispersie);//cat de raspandite sunt valorile semnalului in jurul mediei
			Median(waveData,npoints,&mediana);//valoarea din mijloc a unui set de date ordonat
			
			//calculare zero crossing
		
			for(int i=0; i<npoints-1; i++){	
				if( ( (waveData[i]<0) && (waveData[i+1]>0) ) || ( (waveData[i]>0) && (waveData[i+1]<0)) || (waveData[i] == 0)){
					zero++;
				}
			}
			
			//afisarea histogramei
			//reprezentarea grafica a distributiei frecventelor diferitelor valori dintr-un set de date
			Histogram(waveData,npoints,min-1,max+1,hist,axis,interv);
			DeleteGraphPlot (panel,MAIN_PANEL_IDC_HIST_GRAPH, -1, VAL_DELAYED_DRAW);
            PlotXY (panel,MAIN_PANEL_IDC_HIST_GRAPH, axis,  hist, interv, VAL_DOUBLE, VAL_SSIZE_T, VAL_VERTICAL_BAR, VAL_EMPTY_SQUARE, VAL_SOLID, 1, VAL_RED); 
			
			SetCtrlVal(panel, MAIN_PANEL_IDC_MIN, min);
			SetCtrlVal(panel, MAIN_PANEL_IDC_MAX, max);
			SetCtrlVal(panel, MAIN_PANEL_IDC_INDEX_MIN, minIndex);
			SetCtrlVal(panel, MAIN_PANEL_IDC_INDEX_MAX, maxIndex);
			SetCtrlVal(panel, MAIN_PANEL_IDC_MEDIE, medie);
			SetCtrlVal(panel, MAIN_PANEL_IDC_DISPERSIE, dispersie);
			SetCtrlVal(panel, MAIN_PANEL_IDC_MEDIANA, mediana);
			SetCtrlVal(panel, MAIN_PANEL_IDC_ZERO_CROSSING, zero);
			
			break;
	}
	
	return 0;
	
}



//Filtru
int CVICALLBACK OnSelelectFilter (int panel, int control, int event,
								  void *callbackData, int eventData1, int eventData2)
{
	
	int val, dim;
	double alpha;	
	switch (event)
	{
			
		case EVENT_COMMIT:
			GetCtrlVal(mainPanel, MAIN_PANEL_IDC_MOD_FILTRARE, &val);
			
			if(val == 0)
			{
				
				SetCtrlAttribute(mainPanel, MAIN_PANEL_IDC_DIM_FEREASTRA, ATTR_DIMMED, !val);
				SetCtrlAttribute(mainPanel , MAIN_PANEL_IDC_ALPHA, ATTR_DIMMED, val);
				GetCtrlVal(mainPanel, MAIN_PANEL_IDC_ALPHA, &alpha);
				filtru = ordinI(alpha);
			}
			else
			{
				
				SetCtrlAttribute(mainPanel, MAIN_PANEL_IDC_DIM_FEREASTRA, ATTR_DIMMED, !val);			
				SetCtrlAttribute(mainPanel , MAIN_PANEL_IDC_ALPHA, ATTR_DIMMED, val);
				GetCtrlVal(mainPanel, MAIN_PANEL_IDC_DIM_FEREASTRA, &dim);
				filtru = mediere(dim);
			}
			
			break;
	}
	return 0;
}


//Apply
int CVICALLBACK OnApplyBtt (int panel, int control, int event,
							void *callbackData, int eventData1, int eventData2)
{
	
	
	switch (event)
	{
		case EVENT_COMMIT:
			OnSelelectFilter(MAIN_PANEL, MAIN_PANEL_IDC_MOD_FILTRARE, EVENT_COMMIT, 0, 0, 0);
			DeleteGraphPlot(panel, MAIN_PANEL_IDC_GRAPH_FILTRAT, -1, VAL_IMMEDIATE_DRAW);
			PlotY(panel, MAIN_PANEL_IDC_GRAPH_FILTRAT, filtru, npoints, VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, VAL_CONNECTED_POINTS, VAL_BLUE);
			break;
	}
	return 0;
}


//Prev, Next
int CVICALLBACK OnChangeSec (int panel, int control, int event,
							 void *callbackData, int eventData1, int eventData2)
{
	
	double *temp;
	double *temp1;
	int start=0,stop=0;
	switch (event)
	{
		case EVENT_COMMIT:
			
			
			GetCtrlVal(mainPanel, MAIN_PANEL_IDC_START, &start);
			GetCtrlVal(mainPanel, MAIN_PANEL_IDC_STOP, &stop);
			
			temp=(double*)calloc(npoints/6,sizeof(double));
			temp1=(double*)calloc(npoints/6,sizeof(double));
			
			switch(control)
			{
				case MAIN_PANEL_IDC_BTT_NEXT:
					switch(event)
					{
						case EVENT_COMMIT:
							if(stop<6)
							{
								++start;
								++stop;
								SetCtrlVal(mainPanel, MAIN_PANEL_IDC_STOP, stop);
								SetCtrlVal(mainPanel, MAIN_PANEL_IDC_START, start);
								for(int i=0;i<npoints/6;++i){
									
									temp[i]=waveData[start*npoints/6+i];
								}
								
								for(int i=0;i<npoints/6;++i){
									
									temp1[i]=filtru[start*npoints/6+i];
								}
								DeleteGraphPlot (mainPanel, MAIN_PANEL_GRAPH, -1, VAL_IMMEDIATE_DRAW);
								PlotY (mainPanel, MAIN_PANEL_GRAPH, temp, npoints/6, VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, VAL_CONNECTED_POINTS, VAL_RED);
								
								DeleteGraphPlot (mainPanel, MAIN_PANEL_IDC_GRAPH_FILTRAT, -1, VAL_IMMEDIATE_DRAW);
								PlotY (mainPanel, MAIN_PANEL_IDC_GRAPH_FILTRAT, temp1, npoints/6, VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, VAL_CONNECTED_POINTS, VAL_BLUE);
							}
							break;
					}
					break;
				case MAIN_PANEL_IDC_BTT_PREV:
					switch(event)
					{
						case EVENT_COMMIT:
							if(stop>1)
							{
								--stop;
								--start;
								SetCtrlVal(mainPanel, MAIN_PANEL_IDC_STOP, stop);
								SetCtrlVal(mainPanel, MAIN_PANEL_IDC_START, start);
								for(int i=0;i<npoints/6;++i){
									
									temp[i]=waveData[start*npoints/6+i];
								}
								
								for(int i=0;i<npoints/6;++i){
									
									temp1[i]=filtru[start*npoints/6+i];
								}
								DeleteGraphPlot (mainPanel, MAIN_PANEL_GRAPH, -1, VAL_IMMEDIATE_DRAW);
								PlotY (mainPanel, MAIN_PANEL_GRAPH, temp, npoints/6, VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, VAL_CONNECTED_POINTS, VAL_RED);
								
								DeleteGraphPlot (mainPanel, MAIN_PANEL_IDC_GRAPH_FILTRAT, -1, VAL_IMMEDIATE_DRAW);
								PlotY (mainPanel, MAIN_PANEL_IDC_GRAPH_FILTRAT, temp1, npoints/6, VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, VAL_CONNECTED_POINTS, VAL_BLUE);
							}
							break;	
					}
					break;
			}

			break;
	}
	return 0;
}

//Generate Envelope
int CVICALLBACK OnGenerareAnvelopa (int panel, int control, int event,
									void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
	
			LaunchExecutable("python anvelopa.py");
			Delay(4);
			
			anvelopa = (double *) calloc(npoints, sizeof(double));
			
			FileToArray("anvelopa.txt", anvelopa, VAL_DOUBLE, npoints, 1, VAL_GROUPS_TOGETHER, VAL_GROUPS_AS_COLUMNS, VAL_ASCII);
			
			DeleteGraphPlot(panel, MAIN_PANEL_IDC_ANVELOPA, -1, VAL_IMMEDIATE_DRAW);
			PlotY(panel, MAIN_PANEL_IDC_ANVELOPA, anvelopa, npoints, VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, VAL_CONNECTED_POINTS,VAL_BLACK);
			PlotY(panel, MAIN_PANEL_IDC_ANVELOPA, waveData, npoints, VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, VAL_CONNECTED_POINTS, VAL_RED);

			break;
	}
	return 0;
}



//Save
int CVICALLBACK OnSaveImg (int panel, int control, int event,
						   void *callbackData, int eventData1, int eventData2)
{
	int year = 0, month = 0, day = 0, hour = 0, minute = 0, second = 0;
	int imgHandle;
	char fileName[256] = {0},fileName1[256]={0},fileName2[256]={0},fileName3[256]={0},fileName4[256]={0},fileName5[256]={0},fileName6[256]={0};
	switch (event)
	{
		case EVENT_COMMIT:
			
			GetSystemDate(&month, &day, &year);
			GetSystemTime(&hour, &minute, &second);
			
			
			
			
			sprintf(fileName, "Signal_%4d.%2d.%2d_%2d-%2d-%2d.jpg", year, month, day, hour, minute, second);
			GetCtrlDisplayBitmap(panel, MAIN_PANEL_GRAPH, 1, &imgHandle);
			
			
			SaveBitmapToJPEGFile(imgHandle, fileName, JPEG_DCTFAST, 100);
			
			sprintf(fileName1, "SignalFiltered_%4d.%2d.%2d_%2d-%2d-%2d.jpg", year, month, day, hour, minute, second);
			GetCtrlDisplayBitmap(panel, MAIN_PANEL_IDC_GRAPH_FILTRAT, 1, &imgHandle);
			SaveBitmapToJPEGFile(imgHandle, fileName1, JPEG_DCTFAST, 100);
			
			
			
			//setam numele cu care salvam imaginea
			sprintf(fileName2, "SPECTRUM_%4d.%2d.%2d_%2d-%2d-%2d.jpg", year, month, day, hour, minute, second);
			//salvam imaginea
			GetCtrlDisplayBitmap(freqPanel,FREQ_PANEL_GRAPH_SPECTRU,1,&imgHandle);
			SaveBitmapToJPEGFile(imgHandle,fileName2,JPEG_PROGRESSIVE,100);
			
			//setam numele cu care salvam imaginea
			sprintf(fileName3, "Window_%4d.%2d.%2d_%2d-%2d-%2d.jpg", year, month, day, hour, minute, second);
			//salvam imaginea
			GetCtrlDisplayBitmap(freqPanel,FREQ_PANEL_GRAPH_WINDOW, 1,&imgHandle);
			SaveBitmapToJPEGFile(imgHandle,fileName3,JPEG_PROGRESSIVE,100);
			
			//setam numele cu care salvam imaginea
			sprintf(fileName4, "Raw_Window%4d.%2d.%2d_%2d-%2d-%2d.jpg", year, month, day, hour, minute, second);
			//salvam imaginea
			GetCtrlDisplayBitmap(freqPanel,FREQ_PANEL_GRAPH_WINDOW, 1,&imgHandle);
			SaveBitmapToJPEGFile(imgHandle,fileName4,JPEG_PROGRESSIVE,100);
			
				//setam numele cu care salvam imaginea
			sprintf(fileName5, "Filtered_Window%4d.%2d.%2d_%2d-%2d-%2d.jpg", year, month, day, hour, minute, second);
			//salvam imaginea
			GetCtrlDisplayBitmap(freqPanel,FREQ_PANEL_GRAPH_WINDOW,1,&imgHandle);
			SaveBitmapToJPEGFile(imgHandle,fileName5,JPEG_PROGRESSIVE,100);
			
					//setam numele cu care salvam imaginea
			sprintf(fileName6, "Spectrum_of_filter%4d.%2d.%2d_%2d-%2d-%2d.jpg", year, month, day, hour, minute, second);
			//salvam imaginea
			GetCtrlDisplayBitmap(freqPanel,FREQ_PANEL_GRAPH_WINDOW,1,&imgHandle);
			SaveBitmapToJPEGFile(imgHandle,fileName6,JPEG_PROGRESSIVE,100);
			
		
			DiscardBitmap(imgHandle);
			
			break;
	}
	
	
	
	return 0;
}
int CVICALLBACK OnPanelSwitch (int panel, int control, int event,
							   void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			if(panel == mainPanel)
			{
				SetCtrlVal(freqPanel, FREQ_PANEL_IDC_PANEL_SWITCH, 1);
				DisplayPanel(freqPanel);
				HidePanel(panel);
			}
			else
			{
				SetCtrlVal(mainPanel, MAIN_PANEL_IDC_PANEL_SWITCH, 0);
				DisplayPanel(mainPanel);
				HidePanel(panel);
			}

			break;
	}
	return 0;
}
