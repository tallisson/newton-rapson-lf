#include "../model/bus.h"

int main(int argc, char * argv[]) {
	criarBarra(1, SLACK, 1, 0, 0, 0, 0, 0);	
	printf("Bus %d foi criada", busesV[0].m_nin);
	return 0;
}
