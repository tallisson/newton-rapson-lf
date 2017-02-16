#ifndef BUS_H_
#define BUS_H_

#include <stdio.h>

struct branch {
  int m_ni;
  int m_nf;
  int m_area;
  int m_zona;
  int m_circ;
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
	double m_pc;
	double m_qc;
	double m_pg;
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
	int m_posPV;
	int m_ordPQ;
	int m_posPQ;
	int m_ord;
	Branch_t * branch;
};
typedef struct bus Bus_t;

#endif /* BUS_H_ */
