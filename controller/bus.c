#include "../model/bus.h"

int main(int argc, char * argv[]) {
	criarBarra(1, SLACK, 1, 0, 0, 0, 0, 0);	
	criarBarra(2, PV, 1, 0, 0, 0, 0, 0);
	int i;
	for(i = 0; i < 2; i++) {
		printf("Bus %d foi criada\n", busesV[i].m_nin);	
	}

	associarBarras(1, 2, LT, 1, 1, 0);
	printf("Branch criado %d foi criada\n", busesV[0].m_branches[0].m_nf);
	
	return 0;
}
