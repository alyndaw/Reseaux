README TP RESEAU

Ce document permet de prendre en main notre programme afin de pouvoir l'ex�cuter. 

Pour le compiler: 

- Dans un terminal se mettre dans le dossier contenant les scripts
- Effectuer la commande : make


Pour envoyer un message � un destinataire @xxx sur le num�ro de port #np : 
on ex�cute le programme avec l'option -s comme ci-dessous: 

./TP_Darguy_Ndaw_Vincent -s @xxx #np

Le message s'envoie par d�faut en TCP, rajouter l'option -u pour le mode UDP. 


Pour se mettre en mode puit sur le num�ro de port #np : 
on ex�cute le programme avec l'option -p comme ci-dessous: 

./TP_Darguy_Ndaw_Vincent -p #np

De la m�me mani�re on est par d�faut en TCP et l'option -u permet de passer en UDP. 

Attention, la gestion des messages en TCP � la reception n'a pas �t� trait� il y a donc
d�calage dans les messages receptionn�s. 


Autres options : 

Voici les autres options qui ont �t� impl�ment�es: 

-n #nm
d�finit le nombre de message � envoyer

-l #lm
d�finit la longueur max des messages


BAL: 

Pour se mettre en mode BAL sur le num�ro de port #np il faut entrer la commande suivante: 

./TP_Darguy_Ndaw_Vincent -b #np
La boite est alors en attente. 

Si on veut envoyer un message � la BAL il faut utiliser l'option -e comme suit: 

./TP_Darguy_Ndaw_Vincent -e#nd @xxx #np

#nd correspond au num�ro du destinataire � qui on veut envoyer le message.

@xxx est la machine qui est en BAL. 


Si on veut lire une boite il faut utiliser l'option -r: 

./TP_Darguy_Ndaw_Vincent -r#nb @xxx #np

#nb correspond au num�ro de la boite qu'on souhaite lire. 

@xxx est la machine qui est en BAL. 

Notre programme n'arrive cependant pas � g�rer les deux en m�me temps. La boite ne peut donc pas �tre sollicit� en m�me temps par plusieurs utilisateurs.

