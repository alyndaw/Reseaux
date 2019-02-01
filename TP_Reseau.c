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




int main (int argc, char **argv)
{

//---------------------------------Déclatation variables

  int c; //variable du getopt
  extern char *optarg;
  extern int optind;

  int blettre=0 ; //variable associé à la BAL vaut 1 si on est en BAL, 0 sinon
  int source = -1 ; // 0=puits, 1=source
  int udp = 0 ;// 0=TCP, 1=UDP
  int nb_message = 10; // Nb de messages à envoyer ou à recevoir, par défaut : 10 en émission, infini en réception. Associé au -n
  int longueur_max=35; //longueur max des messages. Associé au -l
  int max_message = 10; //nombre max de messages


  char message[35] ;  //variable du message
  char stocking_message[35]; //variable permettant de stocker les messages reçus
  int  longueur_message_stocked = 0; //variable contenant la taille du message reçu


  int sock1 = -1, sock2 = -1 ,sock4 = -1, sock3 = -1 ; // variables du socket
  struct sockaddr_in addr_sock1, addr_sock2, addr_sock4, addr_sock3;  //structures contenant les adresses des socket


  struct sockaddr *addr_sockemetteur ; //structure contenant le socket emetteur
  int longueur_struct_stocked = sizeof(addr_sockemetteur) ; //taille de la structure contenant le socket emetteur


  int longueur=0 ; //variable pour le read
  int message_recu = -1 ; //variable du receivfrom
  int el_bind = -1 ;//variable du bind
  int acc = -1 ; //variable du accept
  int list = -1; //variable du listen
  int conn = -1 ; //variable du connect
  struct hostent *hp; //variable du gethostbyname


  //-----------------------------------------------------------------------variables utilisées uniquement en BAL
  int dest; //variable contenant le numéro destinataire donné en paramètre
  struct BAL debut_liste; //notre structure BAL de base
  char *utilisateur = NULL ; //variable contenant le nom de la machine passé en paramètre
  int lecteur = 0 ; //Variable associée au -r vaut 1 si on est en -r, 0 sinon
  int emetteur = 0 ; //Variable associée au -e vaut 1 si on est en -e, 0 sinon

  int i=0; //variable compteur

//----------------------------------------Gestion des options du programme

  while ((c = getopt(argc, argv, "psun:l:be:r:")) != -1) {
    switch (c) {
    case 'p':
      source = 0;
      break;

    case 's':
      source = 1;
      break;

    case 'u':
      udp=1;
      break;

    case 'n':
      nb_message = atoi(optarg);
      break;

    case 'l':
      longueur_max = atoi(optarg);
      break;

    case 'b':
      blettre=1;
      source=2;
      break;

    case 'e':
      dest = atoi(optarg);
      emetteur = 1 ;
      source = 1;
      break;

    case 'r':
      lecteur = 1;
      source = 1;
      dest = atoi(optarg);
      break ;




    default:
      printf("Erreur, aucune option n'a été choisie");
      break;
    }
  }

 if (source == -1 )
    {
    printf("Erreur nous ne sommes ni en source ni en puit");
    exit(1) ;
    }



  if (source == 1)
    {


      //----------------------------------------------UDP CLIENT---------------------------------------//


      if (udp==1)
	{

	  printf("on est en UDP dans la source\n");

	  if ((sock1=socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP)) == -1)
	    {

	      printf("échec de création du socket");
	      exit(1);

	    }



	  memset((char *)&addr_sock1,0,sizeof(addr_sock1));
	  addr_sock1.sin_family = AF_INET ;
	  addr_sock1.sin_port =htons(atoi(argv[argc-1]));

	  if((hp= gethostbyname(argv[argc-2])) == NULL)
	    {

	      printf("erreur gethostbyname");
	      exit(1);
	    }


	  memcpy((char*)&(addr_sock1.sin_addr.s_addr), hp->h_addr, hp->h_length);

	  for ( i=0;i<nb_message;i++)
	    {
	      construire_message(message,(char)97+(i%26),35);
	      afficher_message(message,20);
	      sendto(sock1,message,(sizeof(message)),0,(struct sockaddr*)&addr_sock1,sizeof(struct sockaddr));

	    }

	}  // fermeture mode udp




      //----------------------------------------------tcp CLIENT---------------------------------------//


      else
	{

	  printf("on est en TCP dans la source\n");

	  if ((sock3=socket(AF_INET,SOCK_STREAM,0)) == -1)
	    {

	      printf("échec de création du socket");
	      exit(1);

	    }




	  memset((char *)&addr_sock3,0,sizeof(addr_sock3));
	  addr_sock3.sin_family = AF_INET ;
	  addr_sock3.sin_port =htons(atoi(argv[argc-1]));

	  if((hp= gethostbyname(argv[argc-2])) == NULL)
	    {

	      printf("erreur gethostbyname");
	      exit(1);
	    }



	  memcpy((char*)&(addr_sock3.sin_addr.s_addr), hp->h_addr, hp->h_length);

	  while( (conn=connect(sock3,(struct sockaddr*)&addr_sock3,sizeof(struct sockaddr))) == -1)
	    {
	      NULL ;

	    }

	  if(lecteur==1) //Gestion du cas où le client veut lire sa boite
	    {
	      char message[5];
	      sprintf(message, "%d", dest);
	      write(sock3,message,sizeof(message));

	      while ((message_recu=(read(sock3,stocking_message,longueur_max))) != 0)
		{
		  printf("Lecture de la boite %d (30) [", dest);
		  afficher_message(stocking_message,message_recu);

		}
	      printf("c'est fini\n");
	      close(sock3);

	    }

	  else if(emetteur==1) //Gestion du cas où le client veut envoyer un message
	    {
	      char message_final[35];
	      char message[30];
	     
	      for (i=0;i<nb_message;i++)
		{
		  construire_message(message,'a'+(i%26),33);
		  sprintf(message_final, "%d", dest); //obtention du numéro de destinataire
		  strcat(message_final,message);      //concaténation du message émis et du numéro de destinataire
		  write(sock3,message_final,(sizeof(message_final)));
		}
	    }

	  else
	    {

	      for (i=0;i<nb_message;i++)
		{
		  construire_message(message,(char)97+(i%26),35);
		  afficher_message(message,20);
		  write(sock3,message,(sizeof(message)));


		}
	    }

	  shutdown(sock3,2);
	  close(sock3);

	}





    } //fermeture mode source

  //----------------------------------------------UDP PUIT---------------------------------------//
  else if (source==0)
    {  //mode puit

      if (udp==1)
	{

	  printf("on est en UDP dans le puits\n");

	  if ((sock2=(socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP))) == -1)
	    {

	      printf("échec de création du socket");
	      exit(1);

	    }


	  memset((char *)&addr_sock2,0,sizeof(addr_sock2));
	  addr_sock2.sin_family = AF_INET ;
	  addr_sock2.sin_port =htons(atoi(argv[argc-1]));
	  addr_sock2.sin_addr.s_addr = INADDR_ANY ;

	  if (el_bind=(bind(sock2,(struct sockaddr*)&addr_sock2,sizeof(struct sockaddr))) == -1 )
	    {
	      printf("echec du bind\n") ;
	      perror("");
	      exit(1);
	    }

	  for( i=0; i<max_message;i++)
	    {
	      if ((message_recu=recvfrom(sock2,stocking_message,longueur_max,0, (struct sockaddr*)&addr_sockemetteur, &longueur_struct_stocked  ))== -1 )
		{
		  printf("echec du receivefrom\n") ;
		  perror("");
		  exit(1) ;
		}


	      afficher_message(stocking_message,message_recu);

	    }








	}  // fermeture mode udp

      //----------------------------------------------TCP PUIT---------------------------------------//




      else
	{





	      printf("on est en TCP dans le puits\n");


	      if ((sock4=(socket(AF_INET,SOCK_STREAM,0))) == -1)
		{

		  printf("echec de création du socket");
		  perror("");
		  exit(1);

		}
	      memset((char *)&addr_sock4,0,sizeof(addr_sock4));
	      addr_sock4.sin_family = AF_INET ;
	      addr_sock4.sin_port =htons(atoi(argv[argc-1]));
	      addr_sock4.sin_addr.s_addr = INADDR_ANY ;

	      if (el_bind=(bind(sock4,(struct sockaddr*)&addr_sock4,sizeof(struct sockaddr))) == -1 )
		{
		  printf("echec du bind\n") ;
		  perror("");
		  exit(1);
		}

	      if (list=(listen(sock4,max_message))==-1)
		{
		  printf("echec de listen\n");
		  perror("");
		  exit(1);
		}

	      if ((acc=accept(sock4,(struct sockaddr*)&addr_sockemetteur,&longueur_struct_stocked)) == -1 )
		{
		  printf("echec de accept\n");
		  perror("");
		  exit(1);
		}

	      printf("valeur acc %d \n",acc);
	      for(i=0;i<max_message;i++)
		{
		  if ((longueur_message_stocked=read(acc,stocking_message,longueur_max))<0)
		    {
		      printf("echec du read\n");
		      perror("");
		      exit(1);
		    }


		  afficher_message(stocking_message,longueur_message_stocked);
		}







	}
    }


  if (blettre == 1)
    {
      //mode BAL

      printf("on est en BAL \n");

      struct BAL* debut_liste=(struct BAL*)malloc(sizeof(struct BAL)); //Création et initialisation de notre liste de BAL
      debut_liste->bal_1=NULL;
      struct bal_i* boite_visee=(struct bal_i*)malloc(sizeof(struct bal_i));//boite avec laquelle on veut interagir
      boite_visee->destinataire=-1;
      boite_visee->lettre_1=NULL;
      boite_visee->next=NULL;


      utilisateur = argv[argc-2];

      if ((sock1=socket(AF_INET,SOCK_STREAM,0)) == -1)
        {
          printf("échec de création du socket");
          exit(1);
        }


      struct sockaddr_in adr_local;
      int port=(atoi(argv[argc-1]));
      memset((char*)& adr_local, 0, sizeof(adr_local));
      adr_local.sin_family = AF_INET;
      adr_local.sin_port =htons(port) ;
      adr_local.sin_addr.s_addr = INADDR_ANY;

      //liaison entre socket et adresse
      if  (bind(sock1,(struct sockaddr *)&adr_local, sizeof(adr_local)) == -1)
        {
          printf("Echec du bind\n");
          perror("");
          exit(1);
        }



      int list;
      int sock_client;


      char message[35];
      int * longueur_psock;


      if ((list = listen(sock1,5)) == -1)
        {
          printf("Echec lors de l'appel de listen()\n");
          perror("");
          exit(1);
        }


      fd_set rfds;
      struct timeval tv;
      int retval = 0;

    while(1)
	{
        FD_ZERO(&rfds);
        FD_SET(sock1, &rfds);
        tv.tv_sec = 5;
        tv.tv_usec = 0;


        if ((retval = select(sock1+1, &rfds, NULL, NULL, &tv)) < 0)
          {
            printf("Erreur select()");
            perror("");
            exit(1);
          }
        else if (retval > 0)


          {

            if(FD_ISSET(sock1, &rfds))


              {

                printf("Donnees disponibles\n");
                if((sock_client = accept(sock1, (struct sockaddr*)&addr_sockemetteur,&longueur_struct_stocked)) == -1)
                  {
                    printf("Echec lors du accept\n");
                    perror("");
                    exit(1);
                  }

                for (i=0; i<nb_message; i++)
                  {

		    message_recu=read(sock_client, stocking_message, longueur_max);

		    if (message_recu == -1)
		    {
		    	    printf("Echec du read\n");
		    	    perror("");
		    	    exit(1);
		    }

		    if (message_recu > 0 )
		    {
		    int num_dest = Recuperer_num(stocking_message);
		  
		     if (Rechercher_dest(debut_liste, num_dest, boite_visee) == 0)
		     	  {
			    // Si la BAL correspondante n'a pas été trouvée, alors on la crée
		     	    Ajouter_Destinataire(debut_liste, num_dest);
			   
		     	  }

		     
		     Rechercher_dest(debut_liste, num_dest, boite_visee);


                        if (message_recu == 5)
                          {
			    struct bal_i* ptr_Courant=debut_liste->bal_1 ;         //Recherche de la boite ciblée
			    while (ptr_Courant->next != 0 && ptr_Courant->destinataire != num_dest){
			      ptr_Courant=ptr_Courant->next ;
			    }
			    if (ptr_Courant->destinataire==num_dest)
			      {
				printf("Récupération de ses lettres par le destinataire  %d \n", ptr_Courant->destinataire);
				Envoi_Lettres(sock_client,ptr_Courant); //Emission des lettres de la BAL au lecteur
				Vider_bal(ptr_Courant);                 //La boite est vidée une fois lue
			      }
                          }


                        else
                          {
		     
			    struct bal_i* ptr_Courant=debut_liste->bal_1 ;         //Recherche de la boite ciblée
			    while (ptr_Courant->next != 0 && ptr_Courant->destinataire != num_dest)
			      {
				ptr_Courant=ptr_Courant->next ;
			      }
			    if (ptr_Courant->destinataire==num_dest)
			      {
				Ajouter_Lettre(ptr_Courant,stocking_message);      //Ajout de la lettre à la boite ciblée     
				afficher_message(stocking_message, message_recu);  //Affichage du message émis
			      }
                          }


		    }

		  }

              }



          }

        else
          {
            printf("Pas de donnees disponibles, on recommence\n");
          }

	close (sock_client);
	}

    close(sock1);
   
    }
  return 0;
}



