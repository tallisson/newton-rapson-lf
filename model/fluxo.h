#ifndef FLUXO_H_
#define FLUXO_H_

#include "bus.h"

// Prototipos
void derivaP(void);
void derivarQ(void);

// Definição dos métodos
void derivaP(void) {
	for(int k = 0; k < contBus; k++) {
		Bus_t busK = busesV[k];

		if(busK.m_tipo != SLACK) {

			for(int m = 0; m < busK.m_numBranches; m++) {
				Branch_t branch = busK.m_branches[m];
				Bus_t busM;

				if(busK.m_nin == branch.m_ni) {
					busM = busesV[branch.m_nf - 1];
				}

				if(busK.m_nin == branch.m_nf) {
					busM = busesV[branch.m_ni - 1];	
				}

				double theta = busK.m_ang - busM.m_ang;
			}
		}
	}
}

void derivarQ(void) {
	
}

#endif /* FLUXO_H_ */