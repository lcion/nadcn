#include "lclient.h"

//------------------------------- Functii Globale -------------------------------------

/* ----------------------------------------------------------------------
 * Functia primeste ca parametru pointerul la forma fereastra
 * si returneaza pentru respectiva fereastra daca a primit (no_login = 0) sau
 * nu (no_login = 1) accesul de la server. 
 * ----------------------------------------------------------------------
*/
int find_nol(FL_FORM *fbel){
unc_pl *p1u = NULL;
	for(p1u = crt_head; p1u->p_gcom->gccommander != fbel && p1u != NULL; p1u = p1u->next);
	if(p1u != NULL)
		return p1u->no_login;
	return -1;
}

/* ----------------------------------------------------------------------
 * Functia primeste ca parametru pointerul la forma fereastra
 * si returneaza socket-ul de comunicatie asociat ferestrei
 * ----------------------------------------------------------------------
*/
int find_sock(FL_FORM *fbel){
unc_pl *p1u = NULL;
	for(p1u = crt_head; p1u->p_gcom->gccommander != fbel && p1u != NULL; p1u = p1u->next);
	if(p1u != NULL)
		return p1u->sockfdex;
	return -1;
}

/* ----------------------------------------------------------------------
 * Functia primeste ca parametru pointerul la forma fereastra
 * si returneaza un pointer la structura de tipul unc_pl
 * care contine pointer la fereastra, descriptorul de socket
 * si o variabila ce retine daca am sau nu acces la server
 * ----------------------------------------------------------------------
*/
unc_pl *find_unc(FL_FORM *fbel){
unc_pl *p1u = NULL;
	for(p1u = crt_head; p1u->p_gcom->gccommander != fbel && p1u != NULL; p1u = p1u->next);
	if(p1u != NULL)
		return p1u;
	return NULL;
}

/* ----------------------------------------------------------------------
 * Functia se ocupa cu deconectarea unui server de la client inchizand
 * conexiunea si fac update la listele din fereastra principala.
 * ----------------------------------------------------------------------
*/
void err_socket(int sock){
unc_pl *p1u = NULL;
char buf[40];
	for(p1u = crt_head; p1u->sockfdex != sock && p1u != NULL; p1u = p1u->next);
	if(p1u == NULL)
		return;
	erblinec(sock);
	dest_c(p1u->p_gcom->retOB);
	sprintf(buf, "Connection on socket %d lost !", sock);
	fl_show_messages(buf);
	close(sock);
}
/* ----------------------------------------------------------------------
 * Deconecteaza toate server-ele din lista de pointeri unc_pl eliberand 
 * memoria si inchizand conexiunile
 * ----------------------------------------------------------------------
*/
void disconect_all(){
unc_pl *p1u = NULL;
char buf[3];
	p1u = crt_head;
	for( ; p1u != NULL; ){
		buf[0] = '.'; buf[1] = END_SESION; buf[2] = 0;
		smsg(p1u->sockfdex, buf);
		recv_rline(p1u->sockfdex);
		erblinec(p1u->sockfdex);
		dest_c(p1u->p_gcom->retOB);
		close(p1u->sockfdex);
		p1u = crt_head;
	}
}

//------------------------------- inpfo Form -------------------------------------

/* ----------------------------------------------------------------------
 * Functie de callback apelata la completarea campului de editare din
 * cadrul formei de verificare a parolei utilizatorului root
 * ----------------------------------------------------------------------
*/
void cb_inpfo_edit(FL_OBJECT *ob, long user_data){

}

/* ----------------------------------------------------------------------
 * Functie de callback apelata la apasarea butonului OK din fereastra de 
 * verificare a parolei utilizatorului root "Password Check"
 * trimite serverul-ui comanda de autentificare avand ca parametru 
 * parola root.
 * ----------------------------------------------------------------------
*/
void cb_inpfo_ok(FL_OBJECT *ob, long user_data){
char str1[80];
unc_pl *p1;
	FD_inpforocc *fd = ob->form->fdui;
	p1 = (unc_pl*) fd->vdata;
	if(p1 != NULL){
		sprintf(str1, ".%c ", MSG_LOGINOUT);
		strcat(str1, fl_get_input(inpf->inpfo_inpOb));
		if(smsg(p1->sockfdex, str1))
			recv_access(p1->sockfdex ,&p1->no_login);
		if(p1->no_login == 1)
			fl_set_button( p1->p_gcom->nolog_btnOB, 1);
	}
	fl_hide_form(inpf->inpforocc);
	fl_free_form(inpf->inpforocc);
	inpf =  NULL;
}

/* ----------------------------------------------------------------------
 * Functie de callback apelata la apasarea butonului Clear din fereastra de
 * verificare a parolei utilizatorului root "Password Check"
 * Sterge campul de editare este de tipul "Secret imput" iar aceasta
 * functie i-l initializeaza.
 * ----------------------------------------------------------------------
*/
void cb_inpfo_clear(FL_OBJECT *ob, long user_data){
	fl_set_input(inpf->inpfo_inpOb,"");
}

/* ----------------------------------------------------------------------
 * Functie de callback apelata la apasarea butonului Cancel din fereastra de
 * verificare a parolei utilizatorului root "Password Check"
 * Inchide fereastra de validare a administratorului. Monitorizarea
 * ramane oprita.
 * ----------------------------------------------------------------------
*/
void cb_inpfo_cancel(FL_OBJECT *ob, long user_data){
unc_pl *p1;
FD_inpforocc *fd = ob->form->fdui;
	p1 = (unc_pl*) fd->vdata;
	fl_set_button( p1->p_gcom->nolog_btnOB, 1);
	fl_hide_form(inpf->inpforocc);
	fl_free_form(inpf->inpforocc);
	inpf  = NULL;
}

//------------------------------- HOST AND USER input form ------------------------------

/* ----------------------------------------------------------------------
 * Functie de callback apelata la apasarea butonului Clear din fereastra de
 * LOGIN, ce sterge campul de editare Password
 * ----------------------------------------------------------------------
*/
void cb_hau_clear(FL_OBJECT *ob, long user_data){
	fl_set_input(hauf->ob_tipasswd,"");
}

/* ----------------------------------------------------------------------
 * Functie apelata de functia callback cb_hau_acc folosita la introducerea
 * in lista de conexiuni a conexiunii nou create.
 * In caz ca autentificarea se termina cu succes este deschisa o noua
 * fereastra de monitorizare. Altfel conexiunea e pierduta.
 * ----------------------------------------------------------------------
*/
FD_gccommander	*createc_form(int sfd, FL_OBJECT *ob, char *titlu){
unc_pl *p1u, *p2u;
char buf[3];
	if((p2u = malloc(sizeof(unc_pl))) == NULL){
		printf("Out of memory\n");
		fl_finish();
		exit(0);
	}
	p2u->p_gcom = create_form_gccommander();
	p2u->next = NULL;
	p2u->sockfdex = sfd;
	p2u->no_login = 1;
	if(crt_head == NULL){
		crt_head = p2u;
	}
	else{
		p1u = crt_head;
		while(p1u->next != NULL)p1u = p1u->next;
		p1u->next = p2u;
	}

	if(recv_access(sfd, &p2u->no_login)){
		fl_set_chart_maxnumb(p2u->p_gcom->load_chartOB, 71);
		fl_insert_chart_value(p2u->p_gcom->load_chartOB, 1, 0, "",0);

		if(p2u->no_login == 0){
			buf[0] = '.'; buf[1] = GRAPH_RAPORT; buf[2] = 0;
			smsg(sfd, buf);
			recv_gupdate(p2u->p_gcom);
			start_timer(p2u->p_gcom->status_timer);
		}else{	//interzic loginarea la host
			//destroy the unbornWindow
			buf[0] = '.'; buf[1] =  END_SESION; buf[2] = 0;
			smsg(sfd, buf);		//disconect
			recv_rline(sfd);
			fl_show_form(p2u->p_gcom->gccommander,FL_PLACE_MOUSE,FL_TRANSIENT, titlu);
			dest_c(p2u->p_gcom->retOB);
			close(sfd);
			return NULL;
		}
		fl_show_form(p2u->p_gcom->gccommander,FL_PLACE_MOUSE,FL_TRANSIENT, titlu);
	return p2u->p_gcom;
	}
	else{
		dest_c(p2u->p_gcom->retOB);
	}
	return NULL;
}

/* ----------------------------------------------------------------------
 * Functia cauta daca exista numele host-ului la care ne-am conectat
 * in lista de host-uri din butonul meniu "Connect to:" din fereastra
 * principala si daca nu exista este adaugat
 * ----------------------------------------------------------------------
*/
void iicinae(const char *hname){	//insert in chioce if does not alredy exist
const char *hn, *ohfc;
int ch_max, i, found = 0;
	hn = hname;
	ch_max = fl_get_choice_maxitems(fland->landnhOB);
	for(i = 1; i <= ch_max; i++){
		ohfc = fl_get_choice_item_text(fland->landnhOB, i);
		if(strcmp(hn, ohfc) == 0)
			found = 1;
	}
	if(found == 0)
		fl_addto_choice(fland->landnhOB, hn);
}

/* ----------------------------------------------------------------------
 * Functie de callback apelata la apasarea "Accept" butonului din fereastra de
 * LOGIN si incearca sa se conecteze cu parola primita la server-ul
 * specificat.
 * ----------------------------------------------------------------------
*/
void cb_hau_acc(FL_OBJECT *ob, long user_data){
char hadr[50];
const char *ho_name;
	int sockfd;
	strcpy(hadr, fl_get_input(hauf->ob_tihost));
	sockfd = connectTCP( hadr, MYPORT);
	while( sockfd < 0 && fl_show_question("Fail to contact server. Retry ?", 0) ){
		sockfd = connectTCP( hadr, MYPORT);
	}		// Am deschis un socket cu server-ul prin care voi comunica.

	if(sockfd >= 0){
		ho_name = fl_get_input(hauf->ob_tihost);
		sprintf(hadr, ".%c ", MSG_LOGINOUT);
		strcat(hadr, fl_get_input(hauf->ob_tipasswd));
		strcat(hadr,"\n");
		if(smsg(sockfd, hadr)){
			strcpy(hadr, "Connected to server ");
			strcat(hadr, ho_name);
			if(createc_form(sockfd, ob, hadr) != NULL){	// Aici introduc in lista de pointeri ....
				sprintf(hadr, "%d %s", sockfd,  ho_name);
				fl_addto_browser(fland->host_listOB, hadr);
				fl_addto_choice(fland->landdhOB, hadr);
				iicinae(ho_name);	//insert in chioce if does not alredy exist
			}
		}
	}
	//Ascund forma de conectare
	fl_hide_form(hauf->hostanduser);
	fl_free_form(hauf->hostanduser);
	fl_activate_form(fland->landmin);
	hauf = NULL;
}

/* ----------------------------------------------------------------------
 * Functie de callback apelata la apasarea butonului Cancel din fereastra de
 * LOGIN.
 * Aceasta are ca efect renuntarea la incercarea de a ne conecta la host.
 * ----------------------------------------------------------------------
*/
void cb_hau_quit(FL_OBJECT *ob, long user_data){
	fl_hide_form(hauf->hostanduser);
	fl_free_form(hauf->hostanduser);
	hauf = NULL;
	fl_activate_form(fland->landmin);
}

/* ----------------------------------------------------------------------
 * Functie callback apelata la completarea campurilor de editare
 * ----------------------------------------------------------------------
*/
void cb_hau_hapinp(FL_OBJECT *ob, long user_data){	//input callback

}
//------------------------------- Gcommander Form -------------------------------------

/* ----------------------------------------------------------------------
 * functie calback apelata la apasarea butonului Change Password din 
 * fereastra de monitorizare a unui sistem server. 
 * Aceasta afiseaza o fereastra de dialog cu trei campuri de editare.
 * De asemenea nu permite prezenta mai multor ferestre de acest tip la
 * un moment dat.
 * ----------------------------------------------------------------------
*/
void cb_gcom_cp(FL_OBJECT *ob, long user_data){	//Change Password
unc_pl *p1;
	p1 = find_unc(ob -> form);
	if( find_nol(ob->form) == 0){
		if(cgpas == NULL){
			cgpas = create_form_cgpassf();
			cgpas->vdata = p1;
        		fl_show_form(cgpas->cgpassf, FL_PLACE_MOUSE, FL_TRANSIENT, "Change User Password");
		}else
			fl_show_messages("Memory Lick");
	}
}

/* ----------------------------------------------------------------------
 * functie callback apelata la apasarea butonului CPU Info din 
 * fereastra de monitorizare a unui sistem server. 
 * Aceasta trimite server-ului o comanda in care i-i spune sa raporteze 
 * informatiile care le detine despre procesoarele din sistemul pe care
 * ruleaza.
 * ----------------------------------------------------------------------
*/
void cb_gcom_cpui(FL_OBJECT *ob, long user_data){	//CPU Info
int sock;
char buf[3];
	if(find_nol(ob->form) == 0){
		sock = find_sock(ob->form);
		buf[0] = '.'; buf[1] = CPU_INFO; buf[2] = 0;
		if(smsg(sock, buf))
			recv_reply(sock);
	}
}

/* ----------------------------------------------------------------------
 * functie callback apelata la apasarea butonului Disk Free din 
 * fereastra de monitorizare a unui sistem server.
 * Aceasta trimite server-ului o comanda in care i-i spune sa raporteze 
 * informatiile care le detine despre sistemele de fisiere montate
 * in acel moment in sistemul pe care ruleaza.
 * ----------------------------------------------------------------------
*/
void cb_gcom_df(FL_OBJECT *ob, long user_data){		//Disk Free
int sock;
char buf[3];
	if(find_nol(ob->form) == 0){
		sock  = find_sock(ob->form);
		buf[0] = '.'; buf[1] = DISK_FREE; buf[2] = 0;
		if(smsg(sock, buf))
			recv_reply(sock);
	}
}

/* ----------------------------------------------------------------------
 * functie callback apelata la apasarea butonului Disk Usage din 
 * fereastra de monitorizare a unui sistem server. 
 * Aceasta trimite server-ului o comanda in care i-i spune sa raporteze 
 * spatiul ocupat de directorul specificat ca parametru sau directorul
 * local in acel moment in sistemul pe care ruleaza.
 * ----------------------------------------------------------------------
*/
void cb_gcom_du(FL_OBJECT *ob, long user_data){		//Disk Usage
char str1[100];
int sock;
const char *s;
	if(find_nol(ob->form) == 0){

		sprintf(str1, ".%c ", DISK_USAGE);
		strcat(str1, (s=fl_show_input("Path to count size:","")) ? s:"");
		sock = find_sock(ob->form);
		if(smsg(sock, str1))
			recv_reply(sock);
	}
}

/* ----------------------------------------------------------------------
 * functie callback apelata la apasarea butonului Halt din 
 * fereastra de monitorizare a unui sistem server.
 * Aceasta are ca efect aparitia unei ferestre de confirmare pentru a
 * evita suspendarea din greseala a sistemului la distanta monitorizat.
 * Daca in aceasta fereastra este apasat OK se trimite server-ului 
 * comanda de a suspenda sistemul la distanta
 * ----------------------------------------------------------------------
*/
void cb_gcom_halt(FL_OBJECT *ob, long user_data){	//suspenda masina la distanta
int sock;
char buf[3];
	if(find_nol(ob->form) == 0){
		sock = find_sock(ob->form);
		buf[0] = '.'; buf[1] = SYS_HALT; buf[2] = 0;
		if( fl_show_question("Esti sigur ca vrei sa suspenzi host-ul server-ului ?", 0)){
			smsg(sock, buf);
			err_socket(sock);
		}
	}
}

/* ----------------------------------------------------------------------
 * functie callback apelata la apasarea butonului Interface Config din 
 * fereastra de monitorizare a unui sistem server. 
 * Aceasta trimite server-ului o comanda in care i-i spune sa trimita
 * ca raspuns informatii despre interfetele de retea active in acel moment
 * in sistemul pe care ruleaza.
 * ----------------------------------------------------------------------
*/
void cb_gcom_ifconfig(FL_OBJECT *ob, long user_data){
int sock;
char buf[3];
	if(find_nol(ob->form) == 0){
		sock = find_sock(ob->form);
		buf[0] = '.'; buf[1] = INTERFACE_CONF; buf[2] = 0;
		if(smsg(sock, buf))
			recv_reply(sock);
	}
}

/* ----------------------------------------------------------------------
 * functie callback apelata la apasarea butonului Last din 
 * fereastra de monitorizare a unui sistem server. 
 * Aceasta trimite server-ului o comanda in care i-i spune sa trimita
 * lista ultimilor utilizatori ce s-au loginat la sistem.
 * ----------------------------------------------------------------------
*/
void cb_gcom_last(FL_OBJECT *ob, long user_data){
int sock;
char buf[3];
	if(find_nol(ob->form) == 0){
		sock = find_sock(ob->form);
		buf[0] = '.'; buf[1] = LOG_HISTORY; buf[2] = 0;
		if(smsg(sock, buf))
			recv_reply(sock);
	}
}

/* ----------------------------------------------------------------------
 * functie callback apelata la apasarea butonului View mtab din 
 * fereastra de monitorizare a unui sistem server. 
 * Aceasta trimite server-ului o comanda in care i-i spune sa trimita
 * informatii despre sitemele de fisiere montate pe respectivul sistem.
 * ----------------------------------------------------------------------
*/
void cb_gcom_mtab(FL_OBJECT *ob, long user_data){
int sock;
char buf[3];
	if(find_nol(ob->form) == 0){
		sock = find_sock(ob->form);
		buf[0] = '.'; buf[1] = MOUNTED_FS; buf[2] = 0;
		if(smsg(sock, buf))
			recv_reply(sock);
	}
}

/* ----------------------------------------------------------------------
 * functie callback apelata la apasarea butonului CPU Info din 
 * fereastra de monitorizare a unui sistem server. 
 * Aceasta trimite server-ului o comanda in care i-i spune sa trimita
 * starea actuala a conexiunilor stabilite pe sistemul respectiv pe 
 * toate protocoalele suprtate de sistem.
 * ----------------------------------------------------------------------
*/
void cb_gcom_netstat(FL_OBJECT *ob, long user_data){
int sock;
char buf[3];
	if(find_nol(ob->form) == 0){
		sock = find_sock(ob->form);
		buf[0] = '.'; buf[1] = NET_STATUS; buf[2] = 0;
		if(smsg(sock, buf))
			recv_reply(sock);
	}
}

/* ----------------------------------------------------------------------
 * functie callback apelata la apasarea butonului No Login din 
 * fereastra de monitorizare a unui sistem server. 
 * are ca efect invalidarea/validarea monitorizarii si actiunii tuturor
 * butoanelor in afara de butonul No Login si Disconect from server.
 * Daca monitorizarea este activa si se apasa acest buton functia face
 * ca monitorizarea calculatorului host respectiv este intrerupta iar
 * butoanele de comanda  interfata grafica pastreaza ultimile informatii 
 * de stare pentru calculatorul monitorizat. Altfel va aparea o fereastra
 * de autentificare.
 * ----------------------------------------------------------------------
*/
void cb_gcom_login(FL_OBJECT *ob, long user_data){
unc_pl *p1;
char buf[3];
	p1 = find_unc(ob -> form);
	if(p1 != NULL){
		if(find_nol(ob->form) == 1){
			if(inpf == NULL){
				inpf = create_form_inpforocc();
				inpf->vdata = p1;
				fl_show_form(inpf->inpforocc, FL_PLACE_MOUSE,FL_TRANSIENT,"Passsword Check");
			}else
				fl_show_messages("Memory Lick");
			}
		else{
			buf[0] = '.'; buf[1] = MSG_LOGINOUT; buf[2] = 0;
			if(smsg(find_sock(ob->form), buf))
				recv_access(p1->sockfdex, &p1->no_login);
		}
		start_timer(p1->p_gcom->status_timer);
	}
}

/* ----------------------------------------------------------------------
 * functie callback apelata la apasarea butonului ps din 
 * fereastra de monitorizare a unui sistem server. 
 * Aceasta trimite server-ului o comanda in care i-i spune sa trimita
 * lista de procese care ruleaza la acel moment pe respectivul sistem.
 * ----------------------------------------------------------------------
*/
void cb_gcom_ps(FL_OBJECT *ob, long user_data){
int sock;
char buf[3];
	if(find_nol(ob->form) == 0){
		sock = find_sock(ob->form);
		buf[0] = '.'; buf[1] = PROCESS_VIEW; buf[2] = 0;
		if(smsg(sock, buf))
			recv_reply(sock);
	}
}
/* ----------------------------------------------------------------------
 * functie callback apelata la apasarea butonului reboot din 
 * fereastra de monitorizare a unui sistem server. 
 * Aceasta are ca efect aparitia unei ferestre de confirmare pentru a
 * evita rebootarea din greseala a sistemului la distanta monitorizat.
 * Daca in aceasta fereastra este apasat OK se trimite server-ului 
 * comanda de a reboota sistemul la distanta
 * ----------------------------------------------------------------------
*/

void cb_gcom_reboot(FL_OBJECT *ob, long user_data){	// !!!Rebuteaza masina la distanta
int sock;
char buf[3];
	if(find_nol(ob->form) == 0){
		sock = find_sock(ob->form);
		buf[0] = '.'; buf[1] =  SYS_REBOOT; buf[2] = 0;
		if( fl_show_question("Esti sigur ca vrei sa rebootezi host-ul server-ului ?", 0)){
			smsg(sock, buf);
			err_socket(sock);
		}
	}
}

/* ----------------------------------------------------------------------
 * functie callback apelata la apasarea butonului  din 
 * fereastra de monitorizare a unui sistem server.
 * Aceasta trimite server-ului o comanda in care va trimite procesului 
 * server o cerere pentru a returna un raport despre folosirea spatiului
 * pe dissk si limitele de spatiu pentru toate sistemele de fisiere montate. 
 * ----------------------------------------------------------------------
*/
void cb_gcom_repq(FL_OBJECT *ob, long user_data){
int sock;
char buf[3];
	if(find_nol(ob->form) == 0){
		sock = find_sock(ob->form);
		buf[0] = '.'; buf[1] =  REP_QUOTA; buf[2] = 0;
		if(smsg(sock, buf))
			recv_reply(sock);
	}
}

/* ----------------------------------------------------------------------
 * functie callback apelata la apasarea butonului route din 
 * fereastra de monitorizare a unui sistem server.
 * Aceasta trimite server-ului o comanda in care va trimite procesului 
 * server o cerere pentru ca acesta sa trimita tabela de rutatre a
 * kernel-ului de pe masina administrata.
 * ----------------------------------------------------------------------
*/
void cb_gcom_route(FL_OBJECT *ob, long user_data){
int sock;
char buf[3];
	if(find_nol(ob->form) == 0){
		sock = find_sock(ob->form);
		buf[0] = '.'; buf[1] =  ROUTE; buf[2] = 0;
		if(smsg(sock, buf))
			recv_reply(sock);
	}
}

/* ----------------------------------------------------------------------
 * functie callback apelata la apasarea butonului Wall din 
 * fereastra de monitorizare a unui sistem server. 
 * Aceasta trimite un mesaj tuturor persoanelor ce au deschis un terminal 
 * pe calculatorul unde ruleaza server-ul, si au mesg setat pe yes.
 * ----------------------------------------------------------------------
*/
void cb_gcom_wall(FL_OBJECT *ob, long user_data){
char str1[100];
const char *s;
int sock;
	if(find_nol(ob->form) == 0){

		sprintf(str1, ".%c ", LUSERS_MSG);
		s=fl_show_input("Send message to all: ","");
		if(s == NULL)
			return;
		strcat(str1, s);

		sock = find_sock(ob->form);
		if(smsg(sock, str1 ))
			recv_reply(sock);
	}
}

/* ----------------------------------------------------------------------
 * functie callback apelata la apasarea butonului Run Command din 
 * fereastra de monitorizare a unui sistem server. 
 * Aceasta functie trimite server-ului o comanda de la administrator
 * pentru a fi executata pe masina la distanta si iesirea acelui proces
 * redirectata spre fereastra de Log.
 * ----------------------------------------------------------------------
*/
void cb_gcom_command(FL_OBJECT *ob, long user_data){
char str1[100];
const char *s;
int sock;
	if(find_nol(ob->form) == 0){

		sprintf(str1, ".%c ",  FREE_COMMAND);
		s=fl_show_input("Input Command Line: ","");
		if(s == NULL)
			return;
		strcat(str1, s);
		strcat(str1, "\n");
		sock = find_sock(ob->form);
		if(smsg(sock, str1 ))
			recv_reply(sock);
	}
}

/* ----------------------------------------------------------------------
 * sterg liniile ce reprezinta conexiunea pe socket-ul sock din lista
 * aflata in fereastra principala si butonul de deconectare tot din
 * fereastra principala.
 * ----------------------------------------------------------------------
*/
void erblinec(int sock){	//sterg din browser clientul
int i, n, numb;
const char *cbrowl;

	n = fl_get_browser_maxline(fland->host_listOB);
	for(i = 1; i <= n; i++){
		cbrowl = fl_get_browser_line(fland->host_listOB, i);
		sscanf(cbrowl, "%d", &numb);
		if(numb == sock){
			fl_delete_browser_line(fland->host_listOB, i);
			i--;
			n--;
		}
	}
	numb = -1;
	n = fl_get_choice_maxitems(fland->landdhOB);
	for(i = 1; i <= n; i++){
		cbrowl = fl_get_choice_item_text(fland->landdhOB, i);
		sscanf(cbrowl, "%d", &numb);
		if(numb == sock){
			fl_delete_choice(fland->landdhOB, i);
			fl_set_choice(fland->landdhOB, 1);
			return;
		}
	}
}

/* ----------------------------------------------------------------------
 * functie callback apelata la apasarea butonului Disconect from server din 
 * fereastra de monitorizare a unui sistem server. 
 * Aceast apel trimite server-ului comanda de deconectare, inchide fereastra
 * de monitorizare, face update la listele din fereastra principala si 
 * inchide descriptorul de socket alocat conexiunii.
 * ----------------------------------------------------------------------
*/
void cb_gcom_disconnect(FL_OBJECT *ob, long user_data){
int sock;
char buf[3];
	sock = find_sock(ob->form);
	buf[0] = '.'; buf[1] =  END_SESION; buf[2] = 0;
	smsg(sock, buf);
	erblinec(sock);
	dest_c(ob);
	recv_rline(sock);
	close(sock);
}

/* ----------------------------------------------------------------------
 * functie de callback 
 * ----------------------------------------------------------------------
*/
void cb_gcom_cddon(FL_OBJECT *ob, long user_data){ //change and do noething
}

/* ----------------------------------------------------------------------
 * Functie de callback (nu se apeleaza)
 * ----------------------------------------------------------------------
*/
void cb_gcom_wclk(FL_OBJECT *ob, long user_data){
	printf("Click on w\n");
}

// Timer 

/* ----------------------------------------------------------------------
 * Functie apelata prima data la pornirea temporizatorului folosit
 * la monitorizarea sistemelor server la distanta
 * Timer-ul da o comanda automata server-ului corespondent pentru ca
 * acesta sa trimita informatiile de stare sistemului client care
 * se ocupa cu monitorizarea sa.
 * ----------------------------------------------------------------------
*/
void start_timer(FL_OBJECT *ob){
	FD_gccommander *fd = ob->form->fdui;
	fl_set_timer(fd->status_timer, TIMER_SEC_DELAY);
}

/* ----------------------------------------------------------------------
 * functie callback ce este apelata cand contorul timer-ului ajunge
 * la zero. Aceasta functie reinitializeaza timer-ul, trimite  o noua
 * comanda server-ului pentru a-i cere informatii de stare, si apeleaza
 * functia care transforma informatiile primite in grafice si campuri in
 * fereastra de monitorizare.
 * ----------------------------------------------------------------------
*/
void cb_gcom_timer(FL_OBJECT *ob, long data){
unc_pl *p1;
char buf[3];
	start_timer(ob);
	p1 = find_unc(ob->form);
	buf[0] = '.'; buf[1] =  GRAPH_RAPORT; buf[2] = 0;
	if(p1 != NULL)
		if(p1->no_login  == 0)
			if(smsg(p1->sockfdex, buf))
				recv_gupdate(p1->p_gcom);
}

//----------------------------------- CgPassw Input Form --------------------------------


/* ----------------------------------------------------------------------
 * functie callback apelata la apasarea butonului Ok printr-un click de mouse
 * sau apasarea tastei Enter in fereastra "Change User Password". Aceasta
 * fereastra apare la apasarea butonului Change Password din fereastra
 * de monitorizare a server-ului respectiv.
 * Inainte de a trimite server-ului comanda de schimbare a parolei
 * aceasta functie face o minima verificare a validitatii parolei punand
 * conditia ca aceasta sa nu fie de lungime 0 sau campul de editare 
 * Password diferit de campul de editare Change Password.
 * ----------------------------------------------------------------------
*/
void cb_cgp_change(FL_OBJECT *ob, long user_data){
char user[80], password[20];

unc_pl *p1;
	FD_inpforocc *fd = ob->form->fdui;
	p1 = (unc_pl*) fd->vdata;
	if(p1 != NULL){
		if(p1->no_login == 0){
			strcpy(password, fl_get_input(cgpas->cgp_pass0Ob));
			if(strcmp(password, fl_get_input(cgpas->cgp_pass1Ob)) != 0 || password[0] == 0){
				fl_show_message("ERROR", "", "Wrong Password. The two fields must be the same or length greater than zero");
				return;
			}

			sprintf(user, ".%c ",  CH_USER_PASS);
			strcat(user, fl_get_input(cgpas->cgp_userOb));
			strcat(user, " ");
			strcat(user, password);
			fl_hide_form(cgpas->cgpassf);
			fl_free_form(cgpas->cgpassf);
			cgpas = NULL;
			if(smsg(p1->sockfdex, user))
				recv_rline(p1->sockfdex);
		}
	}
}

/* ----------------------------------------------------------------------
 * functie callback apelata la apasarea butonului Clear printr-un click de mouse
 * sau apasarea tastelor Alt+l in fereastra "Change User Password". Aceasta
 * fereastra apare la apasarea butonului Change Password din fereastra
 * de monitorizare a server-ului respectiv.
 * functia are ca efect stergerea campurilor de editare Password si 
 * Change Password.
 * ----------------------------------------------------------------------
*/
void cb_cgp_clear(FL_OBJECT *ob, long user_data){
	fl_set_input(cgpas->cgp_pass0Ob, "");
	fl_set_input(cgpas->cgp_pass1Ob, "");
}

/* ----------------------------------------------------------------------
 * functie callback apelata la apasarea butonului Cancel printr-un click de mouse
 * sau apasarea tastelor Alt+l in fereastra "Change User Password". Aceasta
 * fereastra apare la apasarea butonului Change Password din fereastra
 * de monitorizare a server-ului respectiv.
 * functia are ca efect inchiderea ferestrei "Change User Password" fara
 * a trimite procesului server nici un fel de comanda.
 * ----------------------------------------------------------------------
*/
void cb_cgp_nochange(FL_OBJECT *ob, long user_data){
	fl_hide_form(cgpas->cgpassf);
	fl_free_form(cgpas->cgpassf);
	cgpas = NULL;
}

/* ----------------------------------------------------------------------
 * functie callback apelata la modificarea campurilor de editatere din 
 * fereastra "Change User Password"
 * ----------------------------------------------------------------------
*/
void cb_cgp_upcinp(FL_OBJECT *ob, long user_data){	//callback pentru campurile de editare

}


//------------------------------- Main Form -------------------------------------
/* ----------------------------------------------------------------------
 * functia este apelata la modificarea continutului browser-ului din 
 * fereastra principala a aplicatiei "LanAdmin", cand adaugam sau
 * stergem conexiuni din aceasta.
 * 
 * ----------------------------------------------------------------------
*/
void cb_landseld_hst(FL_OBJECT *ob, long data){	// host_listOB

}

/* ----------------------------------------------------------------------
 * functie callback apelata la apasarea butonului "new host" printr-un click 
 * de mouse sau selectarea unei entitati din meniul ce se deschide cand tinem
 * apasat butonul din stanga al mousului fiind pozitionati deasupra butonului.
 * Daca a fost ales un camp din acel meniu campul de editare "Host" din 
 * fereastra "Login" va fi precompletat cu optiunea aleasa
 * Daca fereastra de login este deja deschisa se semnaleaza un mesaj de eroare.
 * Activarea ferestrei de Login determina dezactivarea ferestrei principale a
 * aplicatiei. 
 * 
 * ----------------------------------------------------------------------
*/
void cb_land_nh(FL_OBJECT *ob, long data){	//login into new host
const char *cchoice;
	if(hauf == NULL){

		cchoice = fl_get_choice_text(fland->landnhOB);
		fl_deactivate_form(fland->landmin);
		hauf = create_form_hostanduser();
		if(strcmp("new host", cchoice) != 0)
			fl_set_input(hauf->ob_tihost, cchoice);
		fl_show_form(hauf->hostanduser,FL_PLACE_MOUSE,FL_TRANSIENT,"LOGIN");
	}else
		fl_show_messages("Memory Lick detected");

}

/* ----------------------------------------------------------------------
 * functia distruge o fereastra care are obiectul de return ob din lista
 * de pointeri de tipul unc_pl in cele trei situatii:
 * - nodul reprezinta prima conexiune a clientului
 * - nodul este una din conexiunile clientului dar nu prima sau ultima
 * - nodul este ultima conexiune a clientului
 * ----------------------------------------------------------------------
*/
void dest_c(FL_OBJECT *ob){
unc_pl *p1u, *p2u;
	//caut in lista pointerul corespunzatoar formei pe care vreau sa o sterg
	for(p2u = p1u = crt_head; p1u->p_gcom->retOB != ob && p1u != NULL; p2u = p1u, p1u = p1u->next);

	fl_hide_form(p1u->p_gcom->gccommander);
	fl_free_form(p1u->p_gcom->gccommander);
	p1u->p_gcom = NULL;
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

/* ----------------------------------------------------------------------
 * functia callback este apelata la apasarea butonului de deconectare
 * situat in fereastra principala a aplicatiei. Acest buton de deconectare
 * este dotat cu un meniu ca si butonul de stabilire a unei conexiuni.
 * daca alegem o adresa din acel meniu in afara de "host" (prima entitate 
 * din meniu ) acel server va fi deconectat iar fereastra corespunzatoare
 * lui inchisa automat. 
 * ----------------------------------------------------------------------
*/
void cb_land_dh(FL_OBJECT *ob, long data){	//destroy the host
const char *cchoice;
char	buf[3];
int sock;
	cchoice = fl_get_choice_text(fland->landdhOB);
	if(strcmp(cchoice, "host") == 0)
		return;
	sscanf(cchoice, "%d", &sock);
	buf[0] = '.'; buf[1] =  END_SESION; buf[2] = 0;
	smsg(sock, buf);
	recv_rline(sock);
	err_socket(sock);
}

/* ----------------------------------------------------------------------
 * salvez toate adresele din lista meniului de conectare in fisierul
 * .lacli din directorul HOME
 * ----------------------------------------------------------------------
*/
void save_hosts(){
int i, ch_max;
const char *ohfc;
char buff[100];
FILE *fp;
	strcpy(buff, getenv("HOME"));
	strcat(buff, "/.lacli");
	if((fp = fopen(buff, "wt")) == NULL){
		printf("Can't save %s\n", buff);
		return;
	}
	ch_max = fl_get_choice_maxitems(fland->landnhOB);
	for(i = 2; i <= ch_max; i++){
		ohfc = fl_get_choice_item_text(fland->landnhOB, i);
		fprintf(fp, "%s\n", ohfc);
	}
	fclose(fp);
}

/* ----------------------------------------------------------------------
 * incarca lista host-urilor din fisierul .lacli din directorul HOME
 * al utilizatorului curent.
 * 
 * ----------------------------------------------------------------------
*/
void load_hosts(){
char ohfc[200];
FILE *fp;
	strcpy(ohfc, getenv("HOME"));
	strcat(ohfc, "/.lacli");
	if((fp = fopen(ohfc, "rt")) == NULL){
		printf("Can't save %s\n", ohfc);
		return;
	}
	fscanf(fp, "%s", ohfc);
	while(!feof(fp)){
		fl_addto_choice(fland->landnhOB, ohfc);
		fscanf(fp, "%s", ohfc);
	}
	fclose(fp);
}

/* ----------------------------------------------------------------------
 * functie callback apelata la apasarea butonului quit din fereastra
 * principala "LanAdmin". Functia salveaza host-urile in fiser apeland
 * save_hosts(), deconecteaza toate serverele, inchiand socket-urile
 * de comunicatie, si inchide toate ferestrele parasind apoi programul.
 * 
 * ----------------------------------------------------------------------
*/
void cb_land_quitb(FL_OBJECT *ob, long data){
	save_hosts();
	disconect_all();
	fl_finish();
	exit(0);
}

/* ----------------------------------------------------------------------
 * functia principala a programului
 *  - initializeaza sistemul grafic
 *  - creeaza fereastra principala
 *  - completeaza lista host-urilor la care ne putem conecta cu
 *    ceea ce gaseste in fisierul .lacli din directorul radacina
 *  - intra in bucla de tratare a mesajelor
 * ----------------------------------------------------------------------
*/
void main(int argc, char *argv[]){
	fl_initialize(&argc, argv, "LanAdmin", 0, 0);
	fland = create_form_landmin();
	load_hosts();
	fl_show_form(fland->landmin, FL_PLACE_MOUSE, FL_TRANSIENT, "LanAdmin");
	fl_do_forms();
}
