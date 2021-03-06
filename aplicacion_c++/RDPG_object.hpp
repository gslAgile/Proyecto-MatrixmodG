/*---------------------------------------------------------------------------------------------------------------------------------------------------------
 * Libreria RDPG_object.hpp/.cpp (archivo cabecera - header file).
 * 
 * 
 * Esta libreria contiene las declaraciones y definiciones de la clase RDPG y sus metodos asociados. Esta clase permite
 * gestionar las RDPG como objetos. 
 * 
 * 
 * La libreria fue migrada desde codigo C del driver MatrixmodG, es el codigo replica en C++, la lubreria de C fue testeada por un conjunto de pruebas unitarias
 * e integrales mediante el framework Kernel Test Framework (KTF). Pero no se repitieron las pruebas unitarias e integrales desde el espacio usuario, en caso de
 * ser realizado se recomienda hacerlo con el framework de pruebas GTEST.
 * 
 *---------------------------------------------------------------------------------------------------------------------------------------------------------*/

#ifndef RDPG_ULIB_H
#define RDPG_ULIB_H 			/* ULIB: por biblioteca de espacio usuario. */

#include <fstream>
#include <sstream>				/* stringstream: include <istringstram> and <ostringstream> (lectura y escritura de string). */
#include <iomanip>				/* incluye funciones: setw(), setfill(), etc. */
#include "MV_object.hpp"


#define RDPGO_DB_MSG    0		/* Macro de mensajes de debug lib RDPG_object. 0: Deshailitada, 1: Habilitada. */
#define MAX_PLACES		1000	/* Numero maximo de plazas soportadas por aplicacion. */
#define MAX_TRANSITIONS 1000	/* Numero maximo de transiciones soportadas por aplicacion. */
#define MAX_VDIM		50		/* Numero entero maximo de plazas y transiciones a visualizar en una lectura de componentes de una RDPG_o. */
#define MIN_VDIM		10		/* Numero entero maximo de plazas y transiciones a visualizar en una lectura de componentes de una RDPG_o. */

/*---------------------------------------------------------------------------------------------------------------------------------------------------------
 * ID_MCOMPONENT: Enumerador de los identificadores de los componentes matrices (matrix_o) de una RDPG_o.
 *
 * @note 	Los componentes de una RDPG_o pueden ser vectores o matrices. Se normaliza los identificadores de componentes matriz que 
 * comienzan con mX donde:
 * m: es una letra fija que indica en el nombre de la enumeracion que se asocia a una matriz (matrix_o).
 * X: puede ser cualquier nombre para completar el nombre del componente matrix_o.
 *---------------------------------------------------------------------------------------------------------------------------------------------------------*/
enum ID_MCOMPONENT{
	ID_MC_INIT = 0,				/**< Numero que indica el inicio de las enumeraciones en ID_MCOMPONENT. */
	_mII = 0,					/**< Identificador de la matriz de incidencia I. */
	_mIH,						/**< Identificador de la matriz de incidencia H. */
	_mIR,						/**< Identificador de la matriz de incidencia R. */
	_mIRe,						/**< Identificador de la matriz de incidencia Re. */
	_mD, 						/**< Identificador de la matriz de disparos D. */
	ID_MC_END					/**< Numero que indica el fin de las enumeraciones en ID_MCOMPONENT. */
};

const string NAME_MCOMP[] = {
	"< Matriz de incidencia I >",
	"< Matriz de brazos inhibidores H >",
	"< Matriz de brazos lectores R >",
	"< Matriz de brazos reset Re >",
	"< Matriz de disparos disponibles D >"
};

/*---------------------------------------------------------------------------------------------------------------------------------------------------------
 * ID_VCOMPONENT: Enumerador de los identificadores de los componentes vectores (vector_o) de una RDPG_o.
 *
 * @note 	Los componentes pueden ser vectores o matrices, Se normaliza los identificadores de componentes vector que 
 * comienzan con vX donde:
 * v: es una letra fija que indica en el nombre de la enumeracion que se asocia a un vector.
 * X: puede ser cualquier nombre para completar el nombre del componente vector_o.
 *---------------------------------------------------------------------------------------------------------------------------------------------------------*/
enum ID_VCOMPONENT{
	ID_VC_INIT = 0, 			/**< Numero que indica el fin de las enumeraciones en ID_VCOMPONENT.*/
	_vMI = 0,					/**< Identificador del vector de marcado inicial. */
	_vMA,						/**< Identificador del vector de marcado actual. */
	_vMN,						/**< Identificador del vector de marcado nuevo. */
	_vE, 						/**< Identificador del vector E. */
	_vQ,						/**< Identificador del vector Q. */
	_vW,						/**< Identificador del vector W. */
	_vB,						/**< Identificador del vector B. */
	_vL,						/**< Identificador del vector L. */
	_vG,						/**< Identificador del vector G. */
	_vA,						/**< Identificador del vector A. */
	_vUDT,						/**< Identificador del vector UDT. */
	_vEx, 						/**< Identificador del vector Ex. */
	_vHQCV,						/**< Identificador del vector vHQCV . */
	_vHD,						/**< Identificador del vector vHD. */
	ID_VC_END					/**< Numero que indica el fin de las enumeraciones en ID_VCOMPONENT.*/
};	

const string NAME_VCOMP[] = {
	"< Vector de marcado inicial vMI >",
	"< Vector de marcado de marcado actual vMA >",
	"< Vector de marcado nuevo vMN >",
	"< Vector de transiciones sensibilizadas E >",
	"< Vector de funcion Q >",
	"< Vector de funcion W >",
	"< Vector de transiciones des-sensibilizadas B >",
	"< Vector de transiciones des-sensibilizadas L >",
	"< Vector de Transiciones des-sensibilizadas por guardas G >",
	"< Vector de transiciones reset A >",
	"< Vector de resultado de ultimo disparo de transicion UDT >",
	"< Vector de transiciones sensibilizadas generalizado Ex >",
	"< Vector de hilos en cola de variables de condicion HQCV >",
	"< Vector de hilos a despertar HD >"
};
/**---------------------------------------------------------------------------------------------------------------------------------------------------------
 * RDPG_SHOT_RESULT: Enumerador de los resultados que puede devolver el ultimo disparo realizado de una RDPG_o.
 *---------------------------------------------------------------------------------------------------------------------------------------------------------*/
enum RDPG_SHOT_RESULT{
	SHOT_INIT=0,				/* Resultado cuando no se realizaron disparos en una RDPG_o. */
	SHOT_OK = 1, 				/* Resultado cuando el ultimo disparo realizado de una RDPG_o fue exitoso. */
	SHOT_FAILED=-1				/* Resultado cuando el ultimo disparo realizado de una RDPG_o no fue exitoso. */
} ;


/**---------------------------------------------------------------------------------------------------------------------------------------------------------
 * SHOT_MODE: Enumerador de los modos en que se puede disparar las transiciones de una red RDPG_o afectada.
 *---------------------------------------------------------------------------------------------------------------------------------------------------------*/
enum SHOT_MODE{
	SHOT_MODE_E = 1, 			/* Modo de disparo explicito, con este modo seleccionado en la red RDPG_o afectada, impactan los cambios de un disparo. Los vectores de maracado MN y MA de la red se veran afectados.*/
	SHOT_MODE_I					/* Modo de disparo implicito, con este modo seleccionado en la red RDPG_o afectada, no impactan los cambios de un disparo. Se conoce que sucede al disparar una transicion de la red pero los vectores de maracado MN y MA no se veran afectados.*/
} ;


/**---------------------------------------------------------------------------------------------------------------------------------------------------------
 * ST_TRANSITION: Enumerador de los estados en los que puede estar una transicion de una RDPG_o.
 *---------------------------------------------------------------------------------------------------------------------------------------------------------*/
enum ST_TRANSITION{
	ST_NO_SENSITIZED=0,			/* Estado de transicion no sensibilizada.*/	
	ST_SENSITIZED = 1 			/* Estado de transicion sensibilizada.*/	
} ;

/**---------------------------------------------------------------------------------------------------------------------------------------------------------
 * ERRORS_CODES: Enumerador de los codigos de eerores posibles de una RDPG_o.
 *---------------------------------------------------------------------------------------------------------------------------------------------------------*/
enum ERRORS_CODES{
	EC_NULL = 0,				/* Codigo cuando no se produjo ningun error. */
	EC_dobleCreacionDeRDPG,		/* Error que se ocaciona cuando se intenta crear dos o mas veces una misma RDPG_o.*/	
	EC_dobleEliminacionDeRDPG,	/* Error que se ocaciona cuando se produce la eliminacion de una RDPG que ya se elimino.*/	
	EC_transicionInexistente,
	EC_falloPosVP,
	EC_falloPosVT,
	EC_falloVdim,
	EC_extraccionDato,
	EC_datoIncorrecto,
	EC_componenteNoCreado,
	EC_componeteNoValido,
	EC_CodigoCatComp,
	EC_posicionIncorrecta,
	EC_referenciaNula,
	EC_fileReadFail
};


/*---------------------------------------------------------------------------------------------------------------------------------------------------------
 * DEFINICION DE CLASE RDPG: Clase de objeto RDPG gestionado por libreria.
 *---------------------------------------------------------------------------------------------------------------------------------------------------------*/
class RDPG
{
private:
  	/* Variables de objeto RDPG: identificaciones de RDPG_o y variables de estados de objeto RDPG_o */
	string name;				/* Nombre del objeto RDPG. */
	int obj_id;					/* Identificador del objeto RDPG. */
	int shot_result;			/* Resultado de ultimo disparo realizado en la RDPG_o. Ver enumeracion RDPG_SHOT_RESULT. */
	int error_code;				/* Codigo del ultimo error sucedido. */
	size_t posVP;				/* Posicion de la vista de plazas. */
	size_t posVT;				/* Posicion de la vista de transiciones. */
	size_t vdim;				/* Dimension de visualizacion de componentes. Es el numero de plazas y transiciones a visualizar en una lectura (read) al driver. */
	size_t plazas;				/* Numero de plazas de la RDP en caracteres. */
	size_t transiciones;		/* Numero de transiciones de la RDPG. */
	size_t size;				/* Numero de bytes reservados por la RDPG. */


	/* Variables de objeto RDPG: componentes matrices de RDPG */
	matrix_o mII;    			/* Matriz de incidencia I. */
	matrix_o mIH;    			/* Matriz de incidencia H asociada a los brazos inhibidores. */
	matrix_o mIR;    			/* Matriz de incidencia R asociada a los brazos lectores. */
	matrix_o mIRe;   			/* Matriz de incidencia Re asociada a los arcos reset. */
	matrix_o mD;     			/* Matriz con cada uno de los vectores disparos. */
	

	/* Variables de objeto RDPG: componentes vector de RDPG */
	vector_o vMI;  				/* Vector de marcado inicial. */
	vector_o vMA;  				/* Vector de marcado actual. */
	vector_o vMN;  				/* Vector de marcado nuevo. */
	vector_o vE;   				/* Vector de transiciones sensibilizadas. */
	vector_o vQ;   				/* Vector asociado a la funcion cero. */
	vector_o vW;  				/* Vector asociado a la funcion uno. */
	vector_o vL;   				/* Vector de transiciones inhibidas por arco lector L. */
	vector_o vB;   				/* Vector de transiciones inhibidas por arco inhibidor B. */
	vector_o vG;   				/* Vector de transiciones des-sensibilizadas por guarda G. */
	vector_o vA;   				/* Vector de transiciones reset A. */
	vector_o vUDT; 				/* Vector de resultado de ultimo disparo de transiciones. */
	vector_o vEx;  				/* Vector de transiciones sensibilizadas extendido Ex. */
	vector_o vHQCV;				/* Vector de Hilos en Cola de Variables de Condicion (Threads in condition variables queue). */
	vector_o vHD;				/* Vector de hilos a despertar. */


/*---------------------------------------------------------------------------------------------------------------------------------------------------------
 * DECLARACION DE METODOS PRIVADOS DE Objeto RDPG: Conjunto de metodos gestionados solo por libreria RDPG_object.hpp/cpp.
 *---------------------------------------------------------------------------------------------------------------------------------------------------------*/
	void import_matrix(matrix_o&, string p_fname);
	void import_vector(vector_o&, string p_fname);
	void update_mII(matrix_o&);
	void update_vE();
	void update_vW();
	void update_vQ();
	void update_vB();
	void update_vL();
	void update_vA();
	void update_vEx();
	void update_vG();
	void update_vHD();
	int shoot(int, SHOT_MODE);
	void print_headerT();
	void print_headerP();
	void print_line(const char p_c, int p_tam=80) const {cout << setw( p_tam ) << setfill( p_c ) << '\n' << setfill( ' ' ); }

	/* Metodos privados para componentes matrix_o de RDPG. */
	void identity_matrix(ID_MCOMPONENT);
	void clean_matrix(ID_MCOMPONENT);
	void reload_matrix(ID_MCOMPONENT, int);
	void resize_allmatrixs();
	void resize_matrix(ID_MCOMPONENT, size_t, size_t);

	/* Metodos privados para componentes vector_o de RDPG. */
	void clean_vector(ID_VCOMPONENT);
	void reload_vector(ID_VCOMPONENT, int);
	void resize_allvectors();
	void cpy_vector(vector_o&, const vector_o&);
	
/*---------------------------------------------------------------------------------------------------------------------------------------------------------
 * DECLARACION DE METODOS PUBLICOS DE Objeto RDPG
 *---------------------------------------------------------------------------------------------------------------------------------------------------------*/
public:
	/* Variables globales. */
	static int n_objects;									/* Numero de todos los objetos instanciados. */
	
	/* Constructores */
	RDPG(string , size_t, size_t);							/* Constructor personalizado version 1. */
	RDPG(string, string, string, string, string, string);	/* Constructor personalizado version 2. */

	/* Destructor */
	~RDPG() { n_objects--; }

	/* Getters: Metodos inline */
	size_t get_fileLines(string);
	size_t get_lineElements(string);
	string get_name() const { return name; }
	int get_objID() const { return obj_id; }
	int get_errorCode() const { return error_code; }
	size_t get_posVP() const { return posVP; }
	size_t get_posVT() const { return posVT; }
	size_t get_vdim() const { return vdim; }
	size_t get_size() const { return size; }
	size_t get_plazas() const { return plazas; }
	size_t get_transiciones() const  { return transiciones; }
	int get_TokensPlace(size_t);
	size_t get_mcompSize(ID_MCOMPONENT) const;
	size_t get_vcompSize(ID_VCOMPONENT) const;
	size_t get_RDPGSize() const;
	int get_sensitized(size_t);
	bool empty();

	/* Setters */
	void set_posVP(size_t);
	void set_posVT(size_t);
	void set_vdim(size_t);

	/* Metodos de objeto RDPG */
	void import_RDPG(string p_mII, string p_mIH, string p_mIR, string p_mIRe, string p_vMI);
	void add_values_mcomp(matrix_o&, string, size_t);
	void add_values_vcomp(vector_o&, string);
	void add_value_vG(size_t, int);
	void update_work_components();
	
	int shoot_rdpg(int, SHOT_MODE);
	void print_allComp() const;
	void print_RDPGinfo() const;

	/* Metodos publicos para componentes matrix_o de RDPG. */
	matrix_o& ref_mcomp(ID_MCOMPONENT);
	void print_mcomp(ID_MCOMPONENT);
	//void print_mcompInfo(ID_MCOMPONENT);

	/* Metodos publicos para componentes vector_o de RDPG. */
	vector_o& ref_vcomp(ID_VCOMPONENT);
	void print_vcomp(ID_VCOMPONENT);
	//void print_vcompInfo(ID_VCOMPONENT);

};


#endif /* RDPG_ULIB_H */