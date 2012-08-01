#include "forms.h"
#include <stdio.h>
#include "forms/lhostanduser.h"
#include "forms/unCeasfrm.h"
#include <stdlib.h>

FD_hostanduser *mainfrm;

typedef struct unc_plist{
	FD_unCeas *p_unceas;
	struct unc_plist *next;
}unc_pl;

unc_pl	*crt_head;

void unc_ret_cb(FL_OBJECT *ob, long user_data){//return din ceas - sterg si obiectul din lista
unc_pl *p1u, *p2u;
	//caut in lista pointerul corespunzatoar formei pe care vreau sa o sterg
	for(p2u = p1u = crt_head; p1u->p_unceas->retOB != ob && p1u != NULL; p2u = p1u, p1u = p1u->next);

	fl_hide_form(p1u->p_unceas->unCeas);
	fl_free_form(p1u->p_unceas->unCeas);

	if(p1u == p2u){//nodul eliberat a fost capul listei
		if(p1u->next == NULL){// a fost singurul nod in lista
			free(p1u);
			crt_head = NULL;
		}
		else{	//nu a fost singurul nod in lista
			crt_head = p1u->next;
			free(p1u);
		}
	}
	else{		//nu a fost capul listei
		if(p1u->next == NULL){	//a fost coada listei
			free(p1u);
			p2u->next = NULL;
		}
		else{
			p2u->next = p1u->next;
			free(p1u);
		}
	}
}
void schimba(int *varza){
	*varza = 3;

}
void cb_hau_acc(FL_OBJECT *ob, long user_data){//incarc inca o forma - si introduc obiectul in lista
unc_pl *p1u, *p2u;
int varza;
	if((p2u = malloc(sizeof(unc_pl))) == NULL){
		printf("Aut of memory\n");
		fl_finish();
		exit(0);
	}
	p2u->p_unceas = create_form_unCeas();
	p2u->next = NULL;
	schimba(&varza);
	printf("%d\n",varza);
	if(crt_head == NULL){
		crt_head = p2u;
	}
	else{
		p1u = crt_head;
		while(p1u->next != NULL)p1u = p1u->next;
		p1u->next = p2u;
	}
	fl_show_form(p2u->p_unceas->unCeas, FL_PLACE_MOUSE,FL_TRANSIENT,"Ceas");
}

void cb_hau_quit(FL_OBJECT *ob, long user_data){ //ies afara
	fl_finish();
	exit(0);
}


void main(int argc, char **argv){
	crt_head = NULL;
	fl_initialize(&argc, argv, "FormDemo", 0, 0);
	mainfrm = create_form_hostanduser();
	fl_show_form(mainfrm->hostanduser, FL_PLACE_MOUSE,FL_TRANSIENT,"LanAdmin");
	fl_do_forms();
}

