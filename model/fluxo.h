#ifndef FLUXO_H_
#define FLUXO_H_

#include "bus.h"
#include <math.h>

#define LINHA  2
#define COLUNA 3

// Prototipos
void calcJ(void);
void Zeros(double * mat, int rows, int cols);
void derivaP(double * jac, int rows, int cols);
void derivarQ(double * jac, int rows, int cols);

void Zeros(double * mat, int rows, int cols) {
	for(int i = 0; i < rows; i ++) {
		for(int j = 0; j < cols; j++) {			
			mat[i * cols + j] = 0.0;
		}
	}
}

void calcJ(void) {
	int size = 2*numPQ + numPV;
	double jac[size][size];
	Zeros(jac[0], size, size);
	derivaP(jac[0], size, size);
	derivarQ(jac[0], size, size);
}

// Definição dos métodos
void derivaP(double * jac, int rows, int cols) {
	int i;
	for(i = 0; i < contBus; i++) {
		Bus_t busK = busesV[i];

		if(busK.m_tipo != SLACK) {
			int j;
			for(j = 0; j < busK.m_numBranches; j++) {
				Branch_t dataBranch = busK.m_branches[j];
				Bus_t busM;

				if(busK.m_nin == dataBranch.m_ni) {
					busM = busesV[dataBranch.m_nf - 1];
				}

				if(busK.m_nin == dataBranch.m_nf) {
					busM = busesV[dataBranch.m_ni - 1];	
				}

				double theta_km = busK.m_ang - busM.m_ang;
				double vK = busK.m_v;
				double vM = busM.m_v;				
				int k = busK.m_ord;

				// dPk em relação a 'ak'
				jac[k * cols + k] += -1 * vK * vM * 
								(dataBranch.m_g * sin (theta_km) - dataBranch.m_b * cos (theta_km));
				
				// dPk em relação a 'am' (exceto quando m for a barra slack).								
				if (busM.m_tipo != SLACK)
					{
						int m = busM.m_ord;
						jac[k * cols + m] += vK * vM *
												( dataBranch.m_g * sin (theta_km) - dataBranch.m_b * cos (theta_km) );
					}

				// dPk em relação a 'vk'
				if (busK.m_tipo == PQ || busK.m_tipo == PV_TO_PQ) {
					int index = contBus - 1 + busK.m_ordPQ;
					jac [k * cols + index] += -2 * dataBranch.m_g * vK +
														vM * (dataBranch.m_g * cos (theta_km) + dataBranch.m_b * sin(theta_km));
				}

				// dPk em relação a 'vm'
				if (busM.m_tipo == PQ || busM.m_tipo == PV_TO_PQ) {
						int index = contBus - 1 + busM.m_ordPQ;
						jac [k * cols + index] += vK * ( dataBranch.m_g * cos (theta_km) + dataBranch.m_b * sin (theta_km) );
				}


			}
		}
	}
}

void derivarQ(double * jac, int rows, int cols) {
	int i;
	for(i = 0; i < contBus; i++) {
		Bus_t busK = busesV[i];

		if(busK.m_tipo == PQ || busK.m_tipo == PV_TO_PQ) {
			int indexK = contBus - 1 + busK.m_ordPQ;

			int j;
			for(j = 0; j < busK.m_numBranches; j++) {
				Branch_t dataBranch = busK.m_branches[j];
				Bus_t busM;

				if(busK.m_nin == dataBranch.m_ni) {
					busM = busesV[dataBranch.m_nf - 1];
				}

				if(busK.m_nin == dataBranch.m_nf) {
					busM = busesV[dataBranch.m_ni - 1];	
				}

				double theta_km = busK.m_ang - busM.m_ang;
				double vK = busK.m_v;
				double vM = busM.m_v;				
				int k = busK.m_ord;

				// dQk em relação a 'ak'
				jac[indexK * cols + k] += vK * vM *
													( dataBranch.m_b * sin (theta_km) + dataBranch.m_g * cos (theta_km) );

				// dQk em relaçao a 'am' (exceto quando m for a barra slack).
				if (busM.m_tipo != SLACK)
					{
						int m = busM.m_ord;
						jac [indexK * cols + m] += vK * vM *
															( dataBranch.m_g * cos (theta_km) + dataBranch.m_b * sin (theta_km) );
					}

				// dQk em relaçao a 'vk'
				jac [indexK * cols + indexK] += 2 * ( dataBranch.m_b + dataBranch.m_bsh ) *
																			vK - vM *
																			(dataBranch.m_b * cos (theta_km) - dataBranch.m_g * sin(theta_km));

				// dQk em relacao a 'vm'
				if (busM.m_tipo == PQ || busM.m_tipo == PV_TO_PQ)
					{
						int indexM = contBus - 1 + busM.m_ordPQ;
						jac[indexK * cols + indexM] += -1 * vK *
																	( dataBranch.m_b * cos (theta_km) - dataBranch.m_g * sin (theta_km) );															
					}

				// dQk em relaçao a 'vk' (continuação)
				jac[indexK * cols + indexK] += (2 * busK.m_bsh * vK);
			}
		}
	}	
}

#endif /* FLUXO_H_ */
