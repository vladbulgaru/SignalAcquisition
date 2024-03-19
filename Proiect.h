/**************************************************************************/
/* LabWindows/CVI User Interface Resource (UIR) Include File              */
/*                                                                        */
/* WARNING: Do not add to, delete from, or otherwise modify the contents  */
/*          of this include file.                                         */
/**************************************************************************/

#include <userint.h>

#ifdef __cplusplus
    extern "C" {
#endif

     /* Panels and Controls: */

#define  FREQ_PANEL                       1       /* callback function: OnFreqExit */
#define  FREQ_PANEL_GRAPH_RAW_DATA        2       /* control type: graph, callback function: (none) */
#define  FREQ_PANEL_GRAPH_FILTER          3       /* control type: graph, callback function: (none) */
#define  FREQ_PANEL_GRAPH_SPECTRU         4       /* control type: graph, callback function: (none) */
#define  FREQ_PANEL_GRAPH_SPECTRUM_FILTER 5       /* control type: graph, callback function: (none) */
#define  FREQ_PANEL_GRAPH_WINDOW          6       /* control type: graph, callback function: (none) */
#define  FREQ_PANEL_IDC_PANEL_SWITCH      7       /* control type: binary, callback function: OnPanelSwitch */
#define  FREQ_PANEL_IDC_WIN_TYPE          8       /* control type: ring, callback function: (none) */
#define  FREQ_PANEL_IDC_SPECTRU           9       /* control type: command, callback function: OnSpectruBtt */
#define  FREQ_PANEL_IDC_SAVE_IMG          10      /* control type: command, callback function: OnFreqSave */
#define  FREQ_PANEL_IDC_FILTRU            11      /* control type: ring, callback function: (none) */
#define  FREQ_PANEL_IDC_NR_PCT            12      /* control type: ring, callback function: (none) */
#define  FREQ_PANEL_IDC_SECUNDA           13      /* control type: numeric, callback function: (none) */
#define  FREQ_PANEL_IDC_FREQ_PEAK         14      /* control type: numeric, callback function: (none) */
#define  FREQ_PANEL_IDC_POWER_PEAK        15      /* control type: numeric, callback function: (none) */
#define  FREQ_PANEL_IDC_SAMPLERATE        16      /* control type: numeric, callback function: (none) */
#define  FREQ_PANEL_IDC_FPASS             17      /* control type: numeric, callback function: (none) */
#define  FREQ_PANEL_IDC_STOP_FREQ         18      /* control type: numeric, callback function: (none) */
#define  FREQ_PANEL_FILTER_BUTTON         19      /* control type: command, callback function: OnFilterButtonCB */
#define  FREQ_PANEL_GRAPH_RAW_WINDOW      20      /* control type: graph, callback function: (none) */
#define  FREQ_PANEL_IDC_NOPOINTS          21      /* control type: numeric, callback function: (none) */

#define  MAIN_PANEL                       2       /* callback function: OnExit */
#define  MAIN_PANEL_GRAPH                 2       /* control type: graph, callback function: (none) */
#define  MAIN_PANEL_IDC_MIN               3       /* control type: numeric, callback function: (none) */
#define  MAIN_PANEL_IDC_MAX               4       /* control type: numeric, callback function: (none) */
#define  MAIN_PANEL_IDC_INDEX_MAX         5       /* control type: numeric, callback function: (none) */
#define  MAIN_PANEL_IDC_INDEX_MIN         6       /* control type: numeric, callback function: (none) */
#define  MAIN_PANEL_IDC_BTT_LOAD          7       /* control type: command, callback function: OnLoadBtt */
#define  MAIN_PANEL_IDC_MEDIE             8       /* control type: numeric, callback function: (none) */
#define  MAIN_PANEL_IDC_DISPERSIE         9       /* control type: numeric, callback function: (none) */
#define  MAIN_PANEL_IDC_MEDIANA           10      /* control type: numeric, callback function: (none) */
#define  MAIN_PANEL_IDC_ZERO_CROSSING     11      /* control type: numeric, callback function: (none) */
#define  MAIN_PANEL_IDC_HIST_GRAPH        12      /* control type: graph, callback function: (none) */
#define  MAIN_PANEL_IDC_MOD_FILTRARE      13      /* control type: ring, callback function: OnSelelectFilter */
#define  MAIN_PANEL_IDC_ALPHA             14      /* control type: numeric, callback function: (none) */
#define  MAIN_PANEL_IDC_GRAPH_FILTRAT     15      /* control type: graph, callback function: (none) */
#define  MAIN_PANEL_IDC_BTT_APPLY         16      /* control type: command, callback function: OnApplyBtt */
#define  MAIN_PANEL_IDC_DIM_FEREASTRA     17      /* control type: ring, callback function: (none) */
#define  MAIN_PANEL_IDC_BTT_PREV          18      /* control type: command, callback function: OnChangeSec */
#define  MAIN_PANEL_IDC_BTT_NEXT          19      /* control type: command, callback function: OnChangeSec */
#define  MAIN_PANEL_IDC_START             20      /* control type: numeric, callback function: (none) */
#define  MAIN_PANEL_IDC_STOP              21      /* control type: numeric, callback function: (none) */
#define  MAIN_PANEL_IDC_ANVELOPA          22      /* control type: graph, callback function: (none) */
#define  MAIN_PANEL_IDC_GEN_ANVELOPA      23      /* control type: command, callback function: OnGenerareAnvelopa */
#define  MAIN_PANEL_IDC_SAVE              24      /* control type: command, callback function: OnSaveImg */
#define  MAIN_PANEL_IDC_PANEL_SWITCH      25      /* control type: binary, callback function: OnPanelSwitch */
#define  MAIN_PANEL_TEXTMSG               26      /* control type: textMsg, callback function: (none) */


     /* Control Arrays: */

          /* (no control arrays in the resource file) */


     /* Menu Bars, Menus, and Menu Items: */

          /* (no menu bars in the resource file) */


     /* Callback Prototypes: */

int  CVICALLBACK OnApplyBtt(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnChangeSec(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnExit(int panel, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnFilterButtonCB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnFreqExit(int panel, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnFreqSave(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnGenerareAnvelopa(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnLoadBtt(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnPanelSwitch(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnSaveImg(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnSelelectFilter(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnSpectruBtt(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif