#include "../model/fluxo.h"

int main(int argc, char * argv[]) {
	criarBarra(1, SLACK, 1.06, 0.0, 0, 0, 0, 0);	
	criarBarra(2, PV, 1.02, 0, 0, 0, 0.2, 0);
	criarBarra(3, PQ, 0, 0, 45, 15, 0, 0);
	criarBarra(4, PQ, 0, 0, 40, 5, 0, 0);
	criarBarra(5, PQ, 0, 0, 60, 10, 0, 0);

	int i;
	for(i = 0; i < 2; i++) {
		printf("Bus %d foi criada\n", busesV[i].m_nin);	
	}

	associarBarras(1, 2, LT, 1, 1, 0);
	printf("Branch criado %d foi criada\n", busesV[0].m_branches[0].m_nf);

	int size = 2*numPQ + numPV;
	double jac[size][size];
	calcJ();

	return 0;
}
