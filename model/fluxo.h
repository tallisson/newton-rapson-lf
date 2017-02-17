#ifndef FLUXO_H_
#define FLUXO_H_

#include "bus.h"
#include <math.h>

// Prototipos
void calcP(void);
void derivaP(double ** jac);
void derivarQ(double ** jac);

void calcJac(void) {
	double jacobiana[2*numPQ + numPV][2*numPQ + numPV];

}

// Definição dos métodos
void derivaP(double ** jac) {
	for(int i = 0; i < contBus; i++) {
		Bus_t busK = busesV[i];

		if(busK.m_tipo != SLACK) {

			for(int j = 0; j < busK.m_numBranches; j++) {
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

				// Calcular Dp[k,k]
				jac[k][k] += -1 * vK * vM * 
								(dataBranch.m_g * sin (theta_km) - dataBranch.m_b * cos (theta_km));
			}
		}
	}
}

void derivarQ(double ** jac) {
	
}

#endif /* FLUXO_H_ */