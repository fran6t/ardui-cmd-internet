Début d'un projet de gestion domotique, le coeur du système est Jeedom

Dans l'immédiat il s'agit de faire un petit programme de gestion d'un arduino ethernet afin de pouvoir lui passer des commandes de mises en route de relais, mais aussi de lire l'etat de ces relais.
 

ardui-cmd-internet.ino
Passer ou recevoir des infos avec un arduino internet

Le script lorsqu'il reçoit une requete http :
  - analyse celle-ci pour voir si c'est un simple appel ou si c'est pour passer des commandes
 
  Sans parametre, le script renvoi l'état de la patte 2 sous forme 0 ou 1
  Avec parametre, le script cherche la presence du ? puis recherche la chaine R1=0 ou R1=1 pour mettre la patte 3 dans ce meme etat
 
  exemple : http://192.168.0.55?R1=0 provoquera la mise a 0 de la patte 3
