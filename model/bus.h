#ifndef BUS_H_
#define BUS_H_

#include <stdio.h>

struct branch {
	// Barra de saida
  int m_ni;
  // Barra de chegada
  int m_nf;
  int m_area;
  int m_zona;
  int m_circ;
  /* Tipo de barras
   * 3 = Slack
   * 2 = PV
   * 0 = PQ
   */
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
	int m_nin;
	char * m_nome;
	double m_area;
	double m_zona;
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
	Branch_t * branch;
};
typedef struct bus Bus_t;

#endif /* BUS_H_ */
