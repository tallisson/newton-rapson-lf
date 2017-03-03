#ifndef FLUXO_H_
#define FLUXO_H_

#include "bus.h"
#include <math.h>

#define LINHA  2
#define COLUNA 3

// Prototipos
void calcJ(void);
void Zeros(double * mat, int rows, int cols);
void derivaP(double * jac, int rows, int cols);
void derivarQ(double * jac, int rows, int cols);

void Zeros(double * mat, int rows, int cols) {
	for(int i = 0; i < rows; i ++) {
		for(int j = 0; j < cols; j++) {			
			mat[i * cols + j] = 0.0;
		}
	}
}

void calcJ(void) {
	int size = 2*numPQ + numPV;
	double jac[size][size];
	Zeros(jac[0], size, size);
	derivaP(jac[0], size, size);
	derivarQ(jac[0], size, size);
}

// Definição dos métodos
void derivaP(double * jac, int rows, int cols) {
	int i;
	for(i = 0; i < contBus; i++) {
		Bus_t busK = busesV[i];

		if(busK.m_tipo != SLACK) {
			int j;
			for(j = 0; j < busK.m_numBranches; j++) {
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
				jac[k * cols + k] += -1 * vK * vM * 
								(dataBranch.m_g * sin (theta_km) - dataBranch.m_b * cos (theta_km));
				
				// dPk em relação a 'am' (exceto quando m for a barra slack).								
				if (busM.m_tipo != SLACK)
					{
						int m = busM.m_ord;
						jac[k * cols + m] += vK * vM *
												( dataBranch.m_g * sin (theta_km) - dataBranch.m_b * cos (theta_km) );
					}

				// dPk em relação a 'vk'
				if (busK.m_tipo == PQ || busK.m_tipo == PV_TO_PQ) {
					int index = contBus - 1 + busK.m_ordPQ;
					jac [k * cols + index] += -2 * dataBranch.m_g * vK +
														vM * (dataBranch.m_g * cos (theta_km) + dataBranch.m_b * sin(theta_km));
				}

				// dPk em relação a 'vm'
				if (busM.m_tipo == PQ || busM.m_tipo == PV_TO_PQ) {
						int index = contBus - 1 + busM.m_ordPQ;
						jac [k * cols + index] += vK * ( dataBranch.m_g * cos (theta_km) + dataBranch.m_b * sin (theta_km) );
				}


			}
		}
	}
}

void derivarQ(double * jac, int rows, int cols) {
	int i;
	for(i = 0; i < contBus; i++) {
		Bus_t busK = busesV[i];

		if(busK.m_tipo == PQ || busK.m_tipo == PV_TO_PQ) {
			int indexK = contBus - 1 + busK.m_ordPQ;

			int j;
			for(j = 0; j < busK.m_numBranches; j++) {
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

				// dQk em relação a 'ak'
				jac[indexK * cols + k] += vK * vM *
													( dataBranch.m_b * sin (theta_km) + dataBranch.m_g * cos (theta_km) );

				// dQk em relaçao a 'am' (exceto quando m for a barra slack).
				if (busM.m_tipo != SLACK)
					{
						int m = busM.m_ord;
						jac [indexK * cols + m] += vK * vM *
															( dataBranch.m_g * cos (theta_km) + dataBranch.m_b * sin (theta_km) );
					}

				// dQk em relaçao a 'vk'
				jac [indexK * cols + indexK] += 2 * ( dataBranch.m_b + dataBranch.m_bsh ) *
																			vK - vM *
																			(dataBranch.m_b * cos (theta_km) - dataBranch.m_g * sin(theta_km));

				// dQk em relacao a 'vm'
				if (busM.m_tipo == PQ || busM.m_tipo == PV_TO_PQ)
					{
						int indexM = contBus - 1 + busM.m_ordPQ;
						jac[indexK * cols + indexM] += -1 * vK *
																	( dataBranch.m_b * cos (theta_km) - dataBranch.m_g * sin (theta_km) );															
					}

				// dQk em relaçao a 'vk' (continuação)
				jac[indexK * cols + indexK] += (2 * busK.m_bsh * vK);
			}
		}
	}	
}

void execute() {
	/*
	 * Calcular o vetor dos mismatches, a matriz Jacobiana e resolver
	 * o sistema de equações lineares:
	 */
	inicializarBarras();

	InitJ ();

	m_b = m_mismatches->CalcMismatches(m_graph);

	bool execute = false;
	uint32_t nextIter, nextCrt;
	nextCrt = 0;
	do//while (execute)
		{
			nextIter = 0;
			m_iter = 0;
			while (nextIter == 0)
				{
					mat m = m_jac->CalcJac (m_graph);
					vec dx = m_jac->SolveSys (m_b);
					//std::cout << "dx = " << std::endl << dx << std::endl;

					//std::cout << "Dx: \n" << dx << std::endl;
					// Atualizar 'a' e 'V':

					for (uint32_t i = 0; i < m_graph->GetNumBus(); i++)
						{
							Ptr<Bus> bus = m_graph->GetBus(i + 1);
							if (bus->GetType() != Bus::SLACK)
								{
									DoubleValue ang;
									bus->GetAttribute("ACalc", ang);
									double angD = ang.Get();

									angD += dx(bus->GetBus().m_ord);
									//std::cout << "Ângulo Anterior " << ang.Get () << ", Atualização = " << dx (bus->GetBus ().m_ord) << " Novo Ângulo = " << angD << std::endl;
									bus->SetAttribute("ACalc", DoubleValue(angD));
								}

							if (bus->GetType() != Bus::SLACK && bus->GetType() != Bus::GENERATION)
								{
									DoubleValue v;
									bus->GetAttribute("VCalc", v);
									double vD = v.Get();

									uint32_t ind = m_graph->GetNumBus() - 1 + bus->GetBus().m_ordPQ;
									vD += dx(ind);
									//std::cout << "Mag. Tensão Anterior " << v.Get () << ", Atualização = " << dx (ind) << ", Nova Mag. Tensão = " << vD << std::endl;
									bus->SetAttribute("VCalc", DoubleValue(vD));
								}
						}
						bool crt = false;
						if (m_qControl != NULL)
							{
								crt = m_qControl->GetObject<QControl> ()->DoRestore (m_graph);
								if (crt == true)
									{
										InitJ ();
									}
							}


						m_iter++;

						// Qlim
						std::cout << "Iter " << m_iter << std::endl;
						if (m_qControl != NULL)
							{
								crt = m_qControl->GetObject<QControl> ()->DoControl (m_graph);
								if (crt == true)
									{
										InitJ ();
									}
							}

						/*
						 * Cálculo do vetor dos mismatches com os valores corrigidos de
						 * 'a' e 'V':
						 */
						m_b = m_mismatches->CalcMismatches(m_graph);
						//std::cout << "Erros: \n" << b;
						std::cout << "+++++++++++++++++++++++++++++++++++++++++\n";
						for (uint32_t i = 0; i < m_graph->GetNumBus(); i++)
							{
								Ptr<Bus> bus = m_graph->GetBus(i + 1);
								//bus->Print();
							}
						// Teste de convergência:

						double maxB = max(abs(m_b));

						if (maxB <= m_precision)
							{
								nextIter = 1;
							}
						else
							{
								m_jac->Zeros();
								nextIter = 0;

								// Critério de saída do laço:
								if (m_iter == m_maxIter)
									{
										nextIter = 2;
									}
							}
							if(nextIter != 0)
								{
									CalcLosses();
									m_report->StoreData (m_graph, m_sts.m_baseMVA);
									m_report->StoreL (m_totalL);
								}
				}

			if(m_vControl != NULL)
				{
					execute = false;
					if (nextCrt < 20)
						{
							execute = m_vControl->DoControl (m_jac->GetJqv (), m_graph);
							/*std::ostringstream os;
							os << nextCrt;
							std::ofstream fileJ1, fileJ2, fileJ3, fileJ4;

							std::string j1F = "/home/thiago/pwns-ns-3/resultados-fluxo/j1_";
							std::string j2F = "/home/thiago/pwns-ns-3/resultados-fluxo/j2_";
							std::string j3F = "/home/thiago/pwns-ns-3/resultados-fluxo/j3_";
							std::string j4F = "/home/thiago/pwns-ns-3/resultados-fluxo/j4_";

							std::cout << j1F.append(os.str()).append(".txt") << std::endl;

							fileJ1.open(j1F.append(os.str()).append(".txt").c_str());
							fileJ2.open(j2F.append(os.str()).append(".txt").c_str());
							fileJ3.open(j3F.append(os.str()).append(".txt").c_str());
							fileJ4.open(j4F.append(os.str()).append(".txt").c_str());

							fileJ1 << m_jac->GetJ1();
							fileJ2 << m_jac->GetJ2();
							fileJ3 << m_jac->GetJ3();
							fileJ4 << m_jac->GetJ4();

							fileJ1.close();
							fileJ2.close();
							fileJ3.close();
							fileJ4.close();*/
						}
				}
			if (execute == true)
				{
					nextCrt++;
				}
		} while (execute);

	if (nextIter == 1)
		{
			printf("%s\n", "O método de Newton-Raphson convergiu em ");
		}
	else
		{
			printf("%s\n", O número máximo de iterações foi atingido e o método de Newton-Raphson não convergiu...");
		}
}


#endif /* FLUXO_H_ */
