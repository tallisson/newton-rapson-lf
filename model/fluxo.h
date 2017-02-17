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

				// dPk em relação a 'ak'
				jac[k][k] += -1 * vK * vM * 
								(dataBranch.m_g * sin (theta_km) - dataBranch.m_b * cos (theta_km));
				
				// dPk em relação a 'am' (exceto quando m for a barra slack).								
				if (busM->GetType () != Bus::SLACK)
					{
						int m = busM.m_ord;
						jac[k][m] += vK * vM *
												( dataBranch.m_g * sin (theta_km) - dataBranch.m_b * cos (theta_km) )
					}

				// dPk em relação a 'vk'
				if (busK.m_tipo == PQ || busK.m_tipo == PV_TO_PQ) {
					int index = contBus - 1 + busK.m_ordPQ;
					jac [k][index] += -2 * dataBranch.m_g * vK.Get () +
														vM.Get () *
														(dataBranch.m_g * cos (theta_km) + dataBranch.m_b * sin(theta_km));
				}

				// dPk em relação a 'vm'
				if (busM.m_tipo == PQ || busM.m_tipo == PV_TO_PQ) {
						int index = contBus - 1 + busM.m_ordPQ;
						jac [k][index] += vK * ( dataBranch.m_g * cos (theta_km) + dataBranch.m_b * sin (theta_km) );
				}


			}
		}
	}
}

void derivarQ(double ** jac) {
	
}

#endif /* FLUXO_H_ */