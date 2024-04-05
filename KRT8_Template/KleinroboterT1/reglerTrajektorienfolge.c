#include "reglerTrajektorienfolge.h"

/* CODE START */
/* CODE END */

/* Initialisierung des Beobachters und der Steuerung und des Reglers zur Trajektorienfolge */
void reglerTrajektorienfolge_init(void)
{
	/* Initialisierung Solltrajektorienroutinen */
	trajektorie_init();
	
	/* Initialisierung der Variablen des Beobachters und der Steuerung und des Reglers */
	/* CODE START */
	/* CODE END */
}

/* Ausführung des Beobachters
	OUTPUT: u1_B - Beobachteter Eingang 1
			u2_B - Beobachteter Eingang 2
			x1_B - Beobachteter Zustand 1
			X2_B - Beobachteter Zustand 2
			x3_B - Beobachteter Zustand 3 */
void reglerTrajektorienfolge_beobachter(float* u1_B, float* u2_B, float* x1_B, float* x2_B, float* x3_B)
{
	/* CODE START */
	/* CODE END */
}

/*Ausführung der flachheitsbasierten Steuerung */
void reglerTrajektorienfolge_steuerung(void)
{
	/* CODE START */
	/* CODE END */
}

/* Ausführen der flachheitsbasierten Regelung
	INPUT:  u1_B - Beobachteter Eingang 1
			u2_B - Beobachteter Eingang 2
			x1_B - Beobachteter Zustand 1
			X2_B - Beobachteter Zustand 2
			x3_B - Beobachteter Zustand 3 */
void reglerTrajektorienfolge_regelung(float u1_B, float u2_B, float x1_B, float x2_B, float x3_B)
{
	/* CODE START */
	/* CODE END */	
}