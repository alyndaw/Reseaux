README TP RESEAU

Ce document permet de prendre en main notre programme afin de pouvoir l'exécuter. 

Pour le compiler: 

- Dans un terminal se mettre dans le dossier contenant les scripts
- Effectuer la commande : make


Pour envoyer un message à un destinataire @xxx sur le numéro de port #np : 
on exécute le programme avec l'option -s comme ci-dessous: 

./TP_Darguy_Ndaw_Vincent -s @xxx #np

Le message s'envoie par défaut en TCP, rajouter l'option -u pour le mode UDP. 


Pour se mettre en mode puit sur le numéro de port #np : 
on exécute le programme avec l'option -p comme ci-dessous: 

./TP_Darguy_Ndaw_Vincent -p #np

De la même manière on est par défaut en TCP et l'option -u permet de passer en UDP. 

Attention, la gestion des messages en TCP à la reception n'a pas été traité il y a donc
décalage dans les messages receptionnés. 


Autres options : 

Voici les autres options qui ont été implémentées: 

-n #nm
définit le nombre de message à envoyer

-l #lm
définit la longueur max des messages


BAL: 

Pour se mettre en mode BAL sur le numéro de port #np il faut entrer la commande suivante: 

./TP_Darguy_Ndaw_Vincent -b #np
La boite est alors en attente. 

Si on veut envoyer un message à la BAL il faut utiliser l'option -e comme suit: 

./TP_Darguy_Ndaw_Vincent -e#nd @xxx #np

#nd correspond au numéro du destinataire à qui on veut envoyer le message.

@xxx est la machine qui est en BAL. 


Si on veut lire une boite il faut utiliser l'option -r: 

./TP_Darguy_Ndaw_Vincent -r#nb @xxx #np

#nb correspond au numéro de la boite qu'on souhaite lire. 

@xxx est la machine qui est en BAL. 

Notre programme n'arrive cependant pas à gérer les deux en même temps. La boite ne peut donc pas être sollicité en même temps par plusieurs utilisateurs.

