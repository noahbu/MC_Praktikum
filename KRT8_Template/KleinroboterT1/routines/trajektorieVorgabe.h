#ifndef TRAJEKTORIENVORGABE_H_
#define TRAJEKTORIENVORGABE_H_

/* Initialisierungsfunktion f�r die Solltrajektorienfunktion */
void trajektorie_init(void);
/* Funktion f�r den n�chsten Wert der Solltrajektorie */
void trajektorie_next(float *y1p, float *y2p, float *y1pp, float *y2pp);

#endif /* TRAJEKTORIENVORGABE_H_*/
