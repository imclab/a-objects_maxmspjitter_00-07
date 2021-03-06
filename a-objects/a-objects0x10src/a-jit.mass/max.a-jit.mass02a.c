/* 
		important max.a-jit.human02.c is crashing. 2a is first version that gets all coords..
*/
/* 
		
*/


#include "jit.common.h"
#include "max.jit.mop.h"

typedef struct _max_jit_mass
{
	t_object		ob;
	void			*obex;
		//outlets
	void			*out_mass;
} t_max_jit_mass;




t_jit_err jit_mass_init(void); 
void *max_jit_mass_new(t_symbol *s, long argc, t_atom *argv);
void max_jit_mass_free(t_max_jit_mass*x);
void max_jit_mass_mproc(t_max_jit_mass *x, void *mop);			
void max_jit_mass_assist(t_max_jit_mass *x, void *b, long m, long a, char *s);
void *max_jit_mass_class;
			
t_symbol 		*ps_getmass;
			



void main(void)
{	
	void *p,*q;
	
	jit_mass_init();	
	setup(&max_jit_mass_class, max_jit_mass_new, (method)max_jit_mass_free, (short)sizeof(t_max_jit_mass), 
		0L, A_GIMME, 0);

	p = max_jit_classex_setup(calcoffset(t_max_jit_mass,obex));
	q = jit_class_findbyname(gensym("jit_mass"));    
//    max_jit_classex_mop_wrap(p,q,0); 		
    max_jit_classex_mop_wrap(p,q,MAX_JIT_MOP_FLAGS_OWN_BANG|MAX_JIT_MOP_FLAGS_OWN_OUTPUTMATRIX); //custom bang/outputmatrix 		
    max_jit_classex_mop_mproc(p,q,max_jit_mass_mproc); 	//custom mproc
    max_jit_classex_standard_wrap(p,q,0); 	
    addmess((method)max_jit_mass_assist, "assist", A_CANT,0);  

	ps_getmass = gensym("getmass");


	post("a-jit.mass  - andr� sier - "__DATE__"  "__TIME__);
}

void max_jit_mass_mproc(t_max_jit_mass *x, void *mop)
{
	t_jit_err err;
	long ac=0;
	t_atom *av=NULL;
	void *o;
//	t_atom temp[4];
	
//	av = temp;
	
	
	o=max_jit_obex_jitob_get(x);

	if (err=(t_jit_err) jit_object_method(
		o,
		_jit_sym_matrix_calc,
		jit_object_method(mop,_jit_sym_getinputlist),
		jit_object_method(mop,_jit_sym_getoutputlist))) 
	{
		jit_error_code(x,err); 

	} else {


		jit_object_method(o,ps_getmass,&ac,&av);
		outlet_int(x->out_mass,jit_atom_getlong(av));
		if (av) jit_freebytes(av,(ac)*sizeof(t_atom));
		av=NULL; ac=0;


	}
}


void max_jit_mass_assist(t_max_jit_mass *x, void *b, long m, long a, char *s)
{
	if (m == 1) { //input
		max_jit_mop_assist(x,b,m,a,s);
	} else { //output
		switch (a) {
		case 0:
			sprintf(s,"(int) mass value");
			break;  			
		case 1:
			sprintf(s,"dumpout");
			break; 			
		}
	}
}


void max_jit_mass_free(t_max_jit_mass*x)
{
	max_jit_mop_free(x);
	jit_object_free(max_jit_obex_jitob_get(x));
	max_jit_obex_free(x);
}

void *max_jit_mass_new(t_symbol *s, long argc, t_atom *argv)
{
	t_max_jit_mass*x,*o;

	if (x=(t_max_jit_mass*)max_jit_obex_new(max_jit_mass_class,gensym("jit_mass"))) {
		if (o=jit_object_new(gensym("jit_mass"))) {
			max_jit_mop_setup_simple(x,o,argc,argv);			
			//add additional non-matrix output, right2left
			x->out_mass 	= outlet_new(x,0L);	
			max_jit_attr_args(x,argc,argv);
		} else {
			error("jit.mass: could not allocate object");
			freeobject(x);
		}
	}
	return (x);
}


