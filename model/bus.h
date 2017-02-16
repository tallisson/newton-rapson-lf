#ifndef BUS_H_
#define BUS_H_

#include <stdio.h>

#define MAX 10
#define NUM_BUS 50
#define SLACK 3
#define PV 2
#define PQ 0

// Branch => linha de transmissão
#define LT 0

struct branch {
	// Barra de saida
  int m_ni;
  // Barra de chegada
  int m_nf;
  int m_area;
  int m_zona;
  int m_circ;
  //Tipo de branches   
  int m_tipo;
  int m_ordtap;
  int m_postap;
  double m_r;
  double m_x;
  double m_bsh;
  double m_line_rating1;
  double m_line_rating2;
  double m_line_rating3;
  double m_t_ctrl;
  double m_side;
  double m_tap;
  double m_def;
  double m_tapmin;
  double m_tapmax;
  double m_passo;
  double m_ctrl_min;
  double m_ctrl_max;
  double m_g;
  double m_b;
};
typedef struct branch Branch_t;

struct bus {
	int m_nex;
	// Identificador da barra
	int m_nin;
	char * m_nome;
	double m_area;
	double m_zona;
	/* Tipo de barras
	 * 3 = Slack
   	 * 2 = PV
   	 * 0 = PQ
   	 */
	int m_tipo;
	double m_v;
	double m_ang;
	// Potência ativa consumida
	double m_pc;
	// Potência reativa consumida
	double m_qc;
	// Potência ativa gerada
	double m_pg;
	// Potência reativa gerada
	double m_qg;
	double m_base_kV;
	double m_vg_o;
	double m_qgmax;
	double m_qgmin;
	double m_vmax;
	double m_vmin;
	double m_gsh;
	double m_bsh;
	double m_crt;
	int m_ctrl_rem;
	int m_ordPV;
	// Índice de barra PV
	int m_posPV;
	int m_ordPQ;
	// Índice de barra PQ
	int m_posPQ;
	// Indice associada a todas as barras que não são slack
	int m_ord;
	Branch_t m_branches[MAX];
	int m_numBranches;
};
typedef struct bus Bus_t;

Bus_t busesV[NUM_BUS];
int contBus = 0;
int ord = 0;
int posPQ = 0;
int posPV = 0;

// Protótipos
void criarBarra(int nin, int tipo, double v, double ang,
		double pc, double qc, double pg, double qg);

void associarBarras(int indBusk, int indBusM, int tipo, int r, int x, int bsh);

// Definição de métodos
void criarBarra(int nin, int tipo, double v, double ang,
		double pc, double qc, double pg, double qg) {
	Bus_t bus;

	bus.m_nin = nin;
	bus.m_tipo = tipo;
	bus.m_v = v;
	bus.m_ang = ang;
	bus.m_pc = pc;
	bus.m_qc = qc;
	bus.m_pg = pg;
	bus.m_qg = qg;
	bus.m_numBranches = 0;

	if(tipo != SLACK) {
		bus.m_ord = ord++;
	}

	if(tipo == PQ) {
		bus.m_posPQ = posPQ++;
	}

	if(tipo == PV) {
		bus.m_posPV = posPV++;
	}

	busesV[contBus++] = bus;
}

void associarBarras(int indBusk, int indBusM, int tipo, int r, int x, int bsh) {
	Branch_t branch;
	branch.m_ni = indBusk;
	branch.m_nf = indBusM;
	branch.m_tipo = tipo;
	branch.m_r = r;
	branch.m_x = x;
	branch.m_g = r/(r*r+x*x);
	branch.m_b = -x/(r*r+x*x);
	branch.m_bsh = bsh;

	Bus_t busK = busesV[indBusk-1];
	busK.m_branches[busK.m_numBranches++] = branch;
	busesV[indBusk-1] = busK;

	Bus_t busM = (busesV[indBusM-1]);	
	busM.m_branches[busM.m_numBranches++] = branch;
	busesV[indBusM-1] = busM;
}

#endif /* BUS_H_ */
