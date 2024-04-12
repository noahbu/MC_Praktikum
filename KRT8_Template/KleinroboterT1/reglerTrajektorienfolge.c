#include "reglerTrajektorienfolge.h"

/* CODE START */
float x1 = 0;
float x2 = 0;
float x3 = 0; // 

float y1d = 0;
float y2d = 0;

float u1_ctrl = 0;

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

/* Ausf�hrung des Beobachters
	OUTPUT: u1_B - Beobachteter Eingang 1
			u2_B - Beobachteter Eingang 2
			x1_B - Beobachteter Zustand 1
			X2_B - Beobachteter Zustand 2
			x3_B - Beobachteter Zustand 3 */
void reglerTrajektorienfolge_beobachter(float* u1_B, float* u2_B, float* x1_B, float* x2_B, float* x3_B)
{
	/* CODE START */

	// berechnung der Position x, y, der orientierung theta und der geschwindigkeit v

    // Calculate velocities
    //float v = (*u1_B + *u2_B) / 2.0; // Linear velocity
    //float omega = (*u2_B - *u1_B) / D ; // Angular velocity



    // Integrate to update position and orientation
	// möchte die werte von x1_B, x2_B, x3_B sowie u1_B und u2_B berechnen
	float v_l, v_r;
	motor_getVel(&v_l, &v_r);

	float u1 = (v_l + v_r) / 2; // Linear velocity
	float u2 = (v_r - v_l) / D; // Angular velocity
	
	//float u1_dot = u1 / T_SAMPLE; // Acceleration
	//float u2_dot = u2 / T_SAMPLE; // Angular acceleration

    x1 += u1 * cos(x3) * T_SAMPLE; // Change in x-position
    x2 += u1 * sin(x3) * T_SAMPLE; // Change in y-position
    x3 += u2 * T_SAMPLE; // Change in orientation
	//dx3 += atan2(dx1, dx2); // orientation

	//float y1_dd = u1_dot * cos(x3) - u1 * sin(x3*u2); // Acceleration in x-direction
	//float y2_dd = u1_dot * sin(x3) + u1 * cos(x3*u2); // Acceleration in y-direction
    
    *x1_B = x1;
    *x2_B = x2;
    *x3_B = x3;

	*u1_B = u1;
	*u2_B = u2;

	/* CODE END */
}

/*Ausf�hrung der flachheitsbasierten Steuerung */
void reglerTrajektorienfolge_steuerung(void)
{
	/* CODE START */
	float u1_soll = 0;
	float u2_soll = 0;
	float y1p = 0;
	float y2p = 0;
	float y1pp = 0;
	float y2pp = 0;

	trajektorie_next(&y1p, &y2p, &y1pp, &y2pp); // Get the next reference trajectory

	u1_soll = sqrt(y1p*y1p + y2p*y2p); // Linear velocity
	
	if(fabs(y1p*y1p + y2p*y2p) < 0.001) {
		u2_soll = 0;
	}
	else {
		u2_soll = (y2pp * y1p - y2p * y1pp) / (y1p*y1p + y2p*y2p); // Angular velocity
	}

	// Calculate the control inputs
	float vl_soll, vr_soll;

	vl_soll = (2*u1_soll - D*u2_soll)/2;
	vr_soll = (2*u1_soll + D*u2_soll)/2;

	motor_setVel(vl_soll, vr_soll);

	/* CODE END */
}

/* Ausf�hren der flachheitsbasierten Regelung
	INPUT:  u1_B - Beobachteter Eingang 1
			u2_B - Beobachteter Eingang 2
			x1_B - Beobachteter Zustand 1
			X2_B - Beobachteter Zustand 2
			x3_B - Beobachteter Zustand 3 */
void reglerTrajektorienfolge_regelung(float u1_B, float u2_B, float x1_B, float x2_B, float x3_B)
{
	/* CODE START */

	// 1 hz
	// omega = 2 * pi * f
	
	float u1_tilde = 0;
	float u2_tilde = 0;
	float y1p = 0;
	float y2p = 0;
	float y1pp = 0;
	float y2pp = 0;
	float x4 = 0;
	//float x4p = 0;

	trajektorie_next(&y1p, &y2p, &y1pp, &y2pp);
	y1d += y1p * T_SAMPLE;
	y2d += y2p * T_SAMPLE;

	
	float w0 = 1.5*M_PI; //frequency
	float zeta = 1;
	float a1 = 2*zeta*w0; // damping of the system, higher values = more dampin
	float a0 = w0*w0; // stiffness ; natural frequency of system

	float a10 = a0;
	float a21 = a1;
	float a20 = a0;
	float a11 = a1;
 

	x4 = u1_B;

	u1_tilde = cosf(x3_B) * (y1pp + a11 * (y1p - x4 * cosf(x3_B)) + a10 * (y1d - x1_B)) 
             + sinf(x3_B) * (y2pp + a21 * (y2p - x4 * sinf(x3_B)) + a20 * (y2d - x2_B));
			 
	if (fabs(x4) > 0.05){
		u2_tilde = (cosf(x3_B) / x4) * (y2pp + a21 * (y2p - x4 * sinf(x3_B)) + a20 * (y2d - x2_B))
            	 - (sinf(x3_B) / x4) * (y1pp + a11 * (y1p - x4 * cosf(x3_B)) + a10 * (y1d - x1_B));
	}
	else{
		u2_tilde = 0;
	}

	u1_ctrl += u1_tilde * T_SAMPLE;
	float u2 = u2_tilde;

	// Calculate the control inputs
	float vl_soll, vr_soll;

	vl_soll = (2*u1_ctrl - D*u2)/2;
	vr_soll = (2*u1_ctrl + D*u2)/2;

	motor_setVel(vl_soll, vr_soll);

	/* CODE END */	
}