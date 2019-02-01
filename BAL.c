#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <netinet/ip.h>
#include <netinet/in.h>
#include <errno.h>
#include <unistd.h>

#include "FonctionsMessage.h"
#include "BAL.h"


  void Ajouter_Destinataire(struct BAL* boites, int dest) { //creer une boite associée au numéro destinataire donné en paramètre

    struct bal_i* nouveau = (struct bal_i*)malloc(sizeof(struct bal_i)) ;

    nouveau->next= NULL;
    nouveau->lettre_1 =NULL;
    nouveau->destinataire = dest;


    if(boites->bal_1==NULL)
      {
	boites->bal_1=nouveau;

      }

    else
      {
    struct bal_i *ptr_Courant = boites->bal_1 ;
	while( ptr_Courant->next != NULL) {
	  ptr_Courant = ptr_Courant->next;
	}

	ptr_Courant->next=nouveau;
	ptr_Courant->next->next=NULL;

      }

  }


  void Ajouter_Lettre(struct bal_i* boite_destinataire, char mess_recu[35]) { //crée une structure lettre associée au message et l'ajoute à la boite
    struct lettre_i* nouvelle=(struct lettre_i*)malloc(sizeof(struct lettre_i));
    nouvelle->next=NULL;
    strcpy(nouvelle->message, mess_recu);

    if (boite_destinataire->lettre_1 == NULL)
      {
	
	boite_destinataire->lettre_1=nouvelle;

      }

    else if (boite_destinataire->lettre_1->next == NULL)
      {

	boite_destinataire->lettre_1->next=nouvelle;
      }
    else
      {
	struct lettre_i* ptr_Courant=boite_destinataire->lettre_1;

	while( ptr_Courant->next != NULL)
	  {
	    
	    ptr_Courant = ptr_Courant->next;
	  }
	ptr_Courant->next=nouvelle;
      }
  }






  void Vider_bal(struct bal_i *boite_cible) //permet de vider entièrement une boite après lecture des messages
  {

    struct lettre_i *ptr_Courant = boite_cible->lettre_1;
    
    if(ptr_Courant== NULL)
      {
	NULL;
      }

    else
      {
	while(ptr_Courant!=NULL)
	  {
	    struct lettre_i *ptr_Suppr = ptr_Courant;
	    ptr_Courant=ptr_Courant->next;
	    free(ptr_Suppr);
	  }

      }

    boite_cible->lettre_1=NULL;

  }






int Rechercher_dest(struct BAL* liste, int num_dest, struct bal_i *une_boite ) //permet de trouver une boite à l'aide du numéro destinataire
{
  int result=0;
  if (liste->bal_1 != NULL)
    {
      
      struct bal_i *ptr_Courant = liste->bal_1;
      
      if (ptr_Courant->next == NULL )
	{
	  if(num_dest==ptr_Courant->destinataire) {
            result=1;
	  }

	  *une_boite=*ptr_Courant;

	}
      else
	{
	  
	  
	  while( ptr_Courant->next != NULL )
	    {
	      if(ptr_Courant->destinataire==num_dest)
		{
		  break;
		}
	      
	      ptr_Courant = ptr_Courant->next;
	      
	    }
	  
	  
	  if(ptr_Courant->next != NULL)
	    {
	      result=1;
	    }
	  
	  *une_boite=*ptr_Courant;
	}
    }
  
  return result;   //La fonction renvoie 0 si la BAL correspondant au numéro n'existe pas, 1 si elle existe
  
}


  void Envoi_Lettres(int sock, struct bal_i* boite) //fonction qui envoie toutes les lettres d'une boite
  {

    if (boite->lettre_1 != NULL)
      {
	struct lettre_i *ptr_Courant = boite->lettre_1;
      
	
	while(ptr_Courant != NULL)
	  {
	    
	    write(sock, ptr_Courant->message, sizeof(ptr_Courant->message));
	    afficher_message(ptr_Courant->message,sizeof(ptr_Courant->message));
	    ptr_Courant = ptr_Courant->next;
      
	  }
      }
    else
      {
	printf("vous n'avez pas de lettres\n");   //Il n'y a pas de lettre à lire dans cette BAL
      }
  }

  int Recuperer_num(char message[35]) //permet de récuperer le numéro de la boite avec laquelle on veut intéragir
  {
    int i=0;
    char num[3];
    int num_dest;
    for(i=0;i<2; i++)
      {
	num[i]= message[i];
      }
    num_dest=atoi(num);

    return num_dest;
  }
