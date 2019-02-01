
  struct lettre_i {
    char message[35];
    struct lettre_i *next;
  };


  struct bal_i {
    struct lettre_i *lettre_1;
    struct bal_i *next;
    int destinataire;
  };

  struct BAL {
    struct bal_i *bal_1;
  };




  void Ajouter_Destinataire(struct BAL* boites, int dest);
  void Ajouter_Lettre(struct bal_i* boite_destinataire, char mess_recu[35]);
  void Vider_bal(struct bal_i* boite_cible);
  int Rechercher_dest(struct BAL* liste, int num_dest, struct bal_i *une_boite );
  void Envoi_Lettres(int sock, struct bal_i* boite);
  int Recuperer_num(char message[35]);

