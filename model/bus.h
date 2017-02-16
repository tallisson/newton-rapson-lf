#ifndef BUS_H_
#define BUS_H_

#include <stdio.h>

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
};
typedef struct bus Bus_t;

#endif /* BUS_H_ */
