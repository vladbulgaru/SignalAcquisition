#include <utility.h>
#include <cvirte.h>		
#include <ansi_c.h>
#include <advanlys.h>
#include <userint.h>
#include "Proiect.h"
#include "Frecv.h"

//static WindowConst winConst;


double *convertedSpectrum;//vectorul utilizat pentru reprezentarea spectrului

int N;//numarul de puncte pe care il folosim pentru a reprezenta spectrul semnalului

int imgHandle;

static WindowConst winConst;

int CVICALLBACK OnFreqExit (int panel, int event, void *callbackData,
						int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_GOT_FOCUS:

			break;
		case EVENT_LOST_FOCUS:

			break;
		case EVENT_CLOSE:
			QuitUserInterface(0);
			break;
	}
	return 0;
}

int CVICALLBACK OnSpectruBtt (int panel, int control, int event,
							  void *callbackData, int eventData1, int eventData2)
{
	double df=0.0; //pasul in domeniul frecventei
	double freqPeak=0.0; //valoarea maxima din spectrul de putere
	double powerPeak=0.0; //frecventa estimata pentru spectrum de putere
	
	
	
	int secunda;
	double *raw;
	int winType;
	int signalType;
	
	
	WindowConst win;
	
	double *powerSpectrum;
	double *frequencyArray;
	
	
	switch (event)
	{
		case EVENT_COMMIT:
			DeleteGraphPlot (freqPanel, FREQ_PANEL_GRAPH_FILTER, -1, VAL_IMMEDIATE_DRAW);
			PlotY(freqPanel, FREQ_PANEL_GRAPH_FILTER, waveData, npoints, VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, VAL_CONNECTED_POINTS, VAL_RED);
			
			
			
			
			
			
			
			
			SetCtrlVal(freqPanel,FREQ_PANEL_IDC_SAMPLERATE,sampleRate);
			SetCtrlVal(freqPanel,FREQ_PANEL_IDC_NOPOINTS,npoints);
			
			//se ia de pe interfata numarul de puncte pentru ferestruire
			GetCtrlVal(panel, FREQ_PANEL_IDC_NR_PCT, &N);
			
			double window[N];
			double final[N];
	
			
			GetCtrlVal(freqPanel, FREQ_PANEL_IDC_SECUNDA, &secunda);
			
			raw=(double*)calloc(N,sizeof(double));	
			
			
			for(int i=0;i<N;i++)
			{
				if(secunda*npoints/6+i<npoints)
				{
					raw[i]=waveData[secunda*npoints/6+i];
				}
				else
				{
					raw[i]=0;
				}
			}
			
			GetCtrlVal(freqPanel,FREQ_PANEL_IDC_WIN_TYPE,&winType);
			
			
			char unit[32]="V";
	
			powerSpectrum=(double*)calloc(N/2,sizeof(double));
			frequencyArray=(double*)calloc(N/2,sizeof(double));
		
			LinEv1D(raw,N,0,1,window); // mai intai se aplica o fereastra liniara pe semnal initial
			
			switch (winType)
			{
				case 1:
					BlkHarrisWin(window,N);
					break;
				case 2:
					ScaledWindow(window,N,WELCH,&win);
					break;
			}
			//se afiseaza fereastra
			DeleteGraphPlot (freqPanel, FREQ_PANEL_GRAPH_WINDOW, -1, VAL_IMMEDIATE_DRAW);
			PlotY (freqPanel, FREQ_PANEL_GRAPH_WINDOW, window, N, VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, VAL_CONNECTED_POINTS, VAL_RED);
			
			//aplicare fereastra si afisare semnal ferestruit (ferestruire pe semnal si afisare) 
			Mul1D(raw,window,N,final);
			DeleteGraphPlot (freqPanel, FREQ_PANEL_GRAPH_RAW_DATA, -1, VAL_IMMEDIATE_DRAW);
			PlotY (freqPanel, FREQ_PANEL_GRAPH_RAW_DATA, final, N, VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, VAL_CONNECTED_POINTS, VAL_RED);
			
			
			
			//se afiseaza spectru dupa ferestruirea semnalului
			AutoPowerSpectrum (final, N, 1.0/sampleRate, powerSpectrum, &df);
			PowerFrequencyEstimate (powerSpectrum, N/2, -1, winConst, df, 7, &freqPeak,&powerPeak);
			SpectrumUnitConversion(powerSpectrum, N/2, 0, SCALING_MODE_LINEAR, DISPLAY_UNIT_VPK, df, winConst,frequencyArray, unit);
			DeleteGraphPlot (freqPanel, FREQ_PANEL_GRAPH_SPECTRU, -1, VAL_IMMEDIATE_DRAW);
			PlotY (freqPanel, FREQ_PANEL_GRAPH_SPECTRU, frequencyArray, N/2, VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, VAL_CONNECTED_POINTS, VAL_RED);
			SetCtrlVal(freqPanel, FREQ_PANEL_IDC_FREQ_PEAK, freqPeak);
			SetCtrlVal(freqPanel, FREQ_PANEL_IDC_POWER_PEAK, powerPeak);
			
			//extragem  de pe interfata tipul de filtru
			GetCtrlVal(freqPanel,FREQ_PANEL_IDC_FILTRU,&signalType);
			//filtarea
			double *semnal_filtrat;
			semnal_filtrat = (double *) calloc(N, sizeof(double));
			
			
			
			//aplicare fereastra si afisare semnal filtrat si ferestruit (se afiseaza pentru semnal pe fereastra)
			Mul1D(semnal_filtrat,window,N,final);
			DeleteGraphPlot (freqPanel, FREQ_PANEL_GRAPH_FILTER, -1, VAL_IMMEDIATE_DRAW);
			PlotY (freqPanel, FREQ_PANEL_GRAPH_FILTER, final, N, VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, VAL_CONNECTED_POINTS, VAL_BLUE);
			
			// se afiseaza spectru dupa ferestruire (pt semnalul filtrat)
			AutoPowerSpectrum (final, N, 1.0/sampleRate, powerSpectrum, &df);
			SpectrumUnitConversion(powerSpectrum, N/2, 0, SCALING_MODE_LINEAR, DISPLAY_UNIT_VPK, df, winConst,frequencyArray, unit);
			DeleteGraphPlot (freqPanel, FREQ_PANEL_GRAPH_SPECTRUM_FILTER, -1, VAL_IMMEDIATE_DRAW);
			PlotY (freqPanel, FREQ_PANEL_GRAPH_SPECTRUM_FILTER, frequencyArray, N/2, VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, VAL_CONNECTED_POINTS, VAL_BLUE);
			break;
	}
	return 0;
}




int CVICALLBACK OnFilterButtonCB (int panel, int control, int event,
										void *callbackData, int eventData1, int eventData2)
{
	
	int secunda;
	double *raw;
	int winType;
	double final[npoints/6];
	double window[npoints/6];

	double fcut = 250;
	
	int fstop;
	
	int fpass;
	
	int signalType;
	static WindowConst winConst;
	
	
	double *powerSpectrum;
	double *frequencyArray;
	
	char unit[32]="V";
	double df=0.0; //pasul in domeniul frecventei
	
	double coef[55];
		
	powerSpectrum=(double*)calloc(npoints/12,sizeof(double));
	frequencyArray=(double*)calloc(npoints/12,sizeof(double));
	
		switch (event)
	{
			
		case EVENT_COMMIT:
			
			//ferestruirea
			DeleteGraphPlot (freqPanel,FREQ_PANEL_GRAPH_WINDOW, -1, VAL_IMMEDIATE_DRAW);
			GetCtrlVal(freqPanel, FREQ_PANEL_IDC_NR_PCT, &N);
			
			GetCtrlVal(freqPanel,FREQ_PANEL_IDC_SECUNDA,&secunda);
			
			GetCtrlVal(freqPanel,FREQ_PANEL_IDC_STOP_FREQ,&fstop);
			
			GetCtrlVal(freqPanel,FREQ_PANEL_IDC_FPASS,&fpass);
			
			raw=(double*)calloc(npoints/6,sizeof(double));
			for(int i=0;i<npoints/6;i++)
			{
				raw[i]=waveData[secunda*npoints/6+i];
			}	 
			
			GetCtrlVal(freqPanel,FREQ_PANEL_IDC_WIN_TYPE,&winType);
			
			
			//se afiseaza semnalul pe secunde
			DeleteGraphPlot (freqPanel, FREQ_PANEL_GRAPH_RAW_DATA, -1, VAL_IMMEDIATE_DRAW);//Semnal Nefiltrat
			PlotY (freqPanel, FREQ_PANEL_GRAPH_RAW_DATA, raw, npoints/6, VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, VAL_CONNECTED_POINTS, VAL_RED);
			
			
			LinEv1D(raw,npoints/6,0,1,window);	   //// mai intai se aplica o fereastra liniara pe semnal initial
			
			switch (winType)
			{
				case 0:
					BkmanWin(window,npoints/6);
					break;
				case 1:
					HamWin(window,npoints/6);
					break;
			}
			DeleteGraphPlot (freqPanel, FREQ_PANEL_GRAPH_WINDOW, -1, VAL_IMMEDIATE_DRAW);//Window
			PlotY (freqPanel,FREQ_PANEL_GRAPH_WINDOW, window, npoints/6, VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, VAL_CONNECTED_POINTS, VAL_RED);
			
			//ferestruirea pe semnal si afisare
			Mul1D(raw,window,npoints/6,final);//pe aceasta iesire aplic filtru; mult. elem. cu elem. val. din semnal+fereastra; rezulta vectorul final
			DeleteGraphPlot (freqPanel, FREQ_PANEL_GRAPH_RAW_WINDOW, -1, VAL_IMMEDIATE_DRAW);
			PlotY (freqPanel,FREQ_PANEL_GRAPH_RAW_WINDOW, final, npoints/6, VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, VAL_CONNECTED_POINTS, VAL_RED);
			
			
			
			GetCtrlVal(freqPanel,FREQ_PANEL_IDC_FILTRU, &signalType);
			
			double *semnal_filtrat;
			semnal_filtrat = (double *) calloc(npoints, sizeof(double));
			
			switch(signalType)
			{
				case 0:
				
					Ksr_HPF (sampleRate, fcut, 40, coef, 4.5);
					Convolve(final,npoints/6, coef, 40, semnal_filtrat);
			
					break;
					
				case 1:
 				
					Ch_LPF(final,npoints/6,npoints/6, fcut, fstop, 14, semnal_filtrat);
				   	break;

						   
			}
			
			DeleteGraphPlot (freqPanel, FREQ_PANEL_GRAPH_FILTER, -1, VAL_IMMEDIATE_DRAW);
			PlotY (freqPanel, FREQ_PANEL_GRAPH_FILTER, final, npoints/6, VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, VAL_CONNECTED_POINTS, VAL_RED);
			
				
			//se afiseaza spectrul semnalului dupa ce am aplicat filtrul
			DeleteGraphPlot (freqPanel,FREQ_PANEL_GRAPH_SPECTRUM_FILTER, -1, VAL_IMMEDIATE_DRAW);
        	AutoPowerSpectrum (semnal_filtrat, npoints/6, 1.0/sampleRate, powerSpectrum, &df);
			SpectrumUnitConversion(powerSpectrum, npoints/12, 0, SCALING_MODE_LINEAR, DISPLAY_UNIT_VPK, df, winConst,frequencyArray, unit);
			PlotY (freqPanel,FREQ_PANEL_GRAPH_SPECTRUM_FILTER, frequencyArray, npoints/12, VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, VAL_CONNECTED_POINTS, VAL_RED);
				
	
			
			
		
			
	}
	return 0;

			
	
}







int CVICALLBACK OnFreqSave (int panel, int control, int event,
							void *callbackData, int eventData1, int eventData2)
{
	int fereastra, filtru, s;
	char nume_fereastra[50]={0}, nume_filtru[50]={0};
	char fileName[256] ={0};
	switch (event)
	{
		case EVENT_COMMIT:
			GetCtrlVal(freqPanel, FREQ_PANEL_IDC_WIN_TYPE, &fereastra);
			GetCtrlVal(freqPanel, FREQ_PANEL_IDC_FILTRU, &filtru);
			GetCtrlVal(freqPanel, FREQ_PANEL_IDC_SECUNDA, &s);
			
			switch(fereastra)
			{
				case 1:
					strcpy(nume_fereastra,"Blackman");
					break;
				case 2:
					strcpy(nume_fereastra, "Hamming");
					break;
			}
			
			switch(filtru)
			{
				case 1:
					strcpy(nume_filtru,"FIR Ksr_HPF");
					break;
				case 2:
					strcpy(nume_filtru, "ChebyshevITreceJos");
					break;
		
			}
			
			
			//salvare semnal nefiltrat
			sprintf(fileName, "Nefiltrat_%s_interval_%d-%d.jpeg", nume_fereastra,s, s+1);
			GetCtrlDisplayBitmap(freqPanel,FREQ_PANEL_GRAPH_RAW_DATA, 1, &imgHandle);
			SaveBitmapToJPEGFile(imgHandle, fileName, JPEG_DCTFAST, 100);
			DiscardBitmap(imgHandle);
			
			//salvare semnal filtrat
			sprintf(fileName, "%s_%s_interval_%d-%d.jpeg",nume_filtru, nume_fereastra,s, s+1);
			GetCtrlDisplayBitmap(freqPanel,FREQ_PANEL_GRAPH_FILTER, 1, &imgHandle);
			SaveBitmapToJPEGFile(imgHandle, fileName, JPEG_DCTFAST, 100);
			DiscardBitmap(imgHandle);
			
			break;
	}
	return 0;
}
