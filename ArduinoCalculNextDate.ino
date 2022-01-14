/**

Réalisation d'un programme en C vérifiant si une date saisie par l'utilisateur est valide.
Si c'est bien la cas, le programme affichera alors la date du jour suivant.

Exemple: Le jour apres la date du 31/12/1582 est le 1/1/1583

Voici les conditions pour qu'une date soit valide :
- Une année est supérieure ou égale à 1582.
- Les règles utilisées pour les années bissextiles sont celles du calendrier Gregorien instauré en 1582. L'année sera bissextile si :
  . l'année est divisible par 4 et non divisible par 100, ou
  . l'année est divisible par 400.
- Les mois de numéro 1,3,5,7,8,10 et 12 comportent 31 jours.
- Les mois de numéro 4,6,9 et 11 comportent 30 jours
- Le mois numéro 2 (février) comporte :
  . 29 jours si l'année est bissextile
  . 28 jours sinon

Liens externes :
https://fr.wikipedia.org/wiki/Calcul_canonique_de_la_date_de_P%C3%A2ques_gr%C3%A9gorienne#:~:text=M%C3%A9thode%20de%20calcul%20de%20l'%C3%A9pacte%20gr%C3%A9gorienne,-Voici%20un%20algorithme&text=c%20%3D%20QUOTIENT%20(A%20%2F%20100,10%2C%20alors%20%C3%89pacte%20%3D%2026.
http://lwh.free.fr/pages/algo/calendriers/calendrier_gregorien.htm
https://ressources.unisciel.fr/algoprog/s81dates/emodules/dt06mexerc1/res/dt06exerc1-enonce-cpp-xxx.pdf

*/

#include <Arduino.h>

/**
 * @brief L'année sera bissextile si :
 *  - l'année est divisible par 4 et non divisible par 100, ou
 *  - l'année est divisible par 400.
 * @param annee
 * @return true si l'année passée en argument est bissextile
 * @return false sinon
 */
bool estCeQueAnneeEstBissextile(int annee)
{
    bool annee_bissextile = false;

    if (((0 == (annee % 4)) && (0 != (annee % 100))) || (0 == (annee % 400)))
    {
        annee_bissextile = true;
    }

    return annee_bissextile;
}

void setup()
{
    Serial.begin(9600);
    Serial.setTimeout(5000);
    delay(1000);
}

void loop()
{
    // Variables locales
    int Jour = 0;
    int NbrDeJourDansLeMois = 31;
    int Mois = 0;
    int Annee = 0;
    bool Annee_Bissextile = false;

    // Présentation du programme
    Serial.println("\r\nCe programme va vous permettre de trouver le jour suivant la date de votre saisie,");
    Serial.println("a partir du debut du calendrier Gregorien instaure en 1582.");

    // Saisie des données de la date par l'utilisateur.
    // Vérification de la validité de la saisie,
    // si hors limite, alors l'utilisateur doit recommencer la saisie.

    // 1 - Saisie de l'Annee
    while (1582 > Annee)
    {
        Serial.println("\r\nVeillez entrer une valeur d'annee a partir de l'an 1582:");
        // Si le buffer série reçoit des données
        // on lit le buffer série après le delais permettant de recevoir tout les caractères
        while (0 == Serial.available())
            ;
        delay(100); // Attendre la fin de transmission
        Annee = Serial.parseInt();
        Annee_Bissextile = estCeQueAnneeEstBissextile(Annee);
        Serial.print("Vous avez saisi l'annee ");
        if (true == Annee_Bissextile)
        {
            Serial.print("bissextile ");
        }
        Serial.println(Annee);
    }

    // 2 - Saisie du Mois
    while ((1 > Mois) || (12 < Mois))
    {
        Serial.println("\r\nVeillez entrer une valeur de mois entre 1 et 12:");
        while (0 == Serial.available())
            ;
        delay(100); // Attendre la fin de transmission
        Mois = Serial.parseInt();
        Serial.print("Vous avez saisi le mois ");
        Serial.println(Mois);
    }

    // 3 - Saisie du Jour
    // - les mois de numéro 1, 3, 5, 7, 8, 10 et 12 comportent 31 jours,
    // - les mois de numéro 4, 6, 9 et 11 comportent 30 jours.
    if ((4 == Mois) || (6 == Mois) || (9 == Mois) || (11 == Mois))
    {
        NbrDeJourDansLeMois = 30;
    }
    else if (2 == Mois)
    {
        // - Le mois numéro 2 (février) comporte :
        //   . 29 jours si l'année est bissextile
        //   . 28 jours sinon
        NbrDeJourDansLeMois = 28;
        if (true == Annee_Bissextile)
        {
            NbrDeJourDansLeMois = 29;
        }
    }

    while ((1 > Jour) || (NbrDeJourDansLeMois < Jour))
    {
        Serial.print("\r\nVeillez entrer une valeur de jour entre 1 et ");
        Serial.print(NbrDeJourDansLeMois);
        Serial.println(":");
        while (0 == Serial.available())
            ;
        delay(100); // Attendre la fin de transmission
        Jour = Serial.parseInt();
        Serial.print("Vous avez saisi le jour ");
        Serial.println(Jour);
    }

    // Calcul de la date suivant
    // Serial.print("Le jour apres la date du %02d/%02d/%04d ", Jour, Mois, Annee);
    // http://www.cplusplus.com/reference/cstdio/printf/
    Serial.print("\r\nLe jour apres la date du ");
    Serial.print(Jour);
    Serial.print("/");
    Serial.print(Mois);
    Serial.print("/");
    Serial.print(Annee);
    ++Jour; // idem à Jour++; voir http://ressources.unit.eu/cours/Cfacile/co/ch4_p2_5.html
    if (2 == Mois)
    {
        // Test si l'on a un débordement vers le mois suivant
        if (((29 == Jour) && (false == Annee_Bissextile)) || ((30 == Jour) && (true == Annee_Bissextile)))
        {
            // En cas de débordement on passe à la date du mois suivant
            Jour = 1;
            ++Mois;
            if (12 < Mois)
            {
                Mois = 1;
                ++Annee;
            }
        }
    }
    else if ((4 == Mois) || (6 == Mois) || (9 == Mois) || (11 == Mois))
    {
        // Les mois de numéro 4, 6, 9 et 11 comportent 30 jours
        if (31 == Jour)
        {
            // En cas de débordement on passe à la date du mois suivant
            Jour = 1;
            ++Mois;
            if (12 < Mois)
            {
                Mois = 1;
                ++Annee;
            }
        }
    }
    else
    {
        // Les mois de numéro 1, 3, 5, 7, 8, 10 et 12 comportent 31 jours
        if (32 == Jour)
        {
            // En cas de débordement on passe à la date du mois suivant
            Jour = 1;
            ++Mois;
            if (12 < Mois)
            {
                Mois = 1;
                ++Annee;
            }
        }
    }

    // Affichage de la date suivant
    // Serial.println("est le %02d/%02d/%04d.\n", Jour, Mois, Annee);
    Serial.print(" est le ");
    Serial.print(Jour);
    Serial.print("/");
    Serial.print(Mois);
    Serial.print("/");
    Serial.println(Annee);
    Serial.println("\r\nFIN...\r\n"); // sauter 2 lignes avant de recommencer le déroulement du programme
}
