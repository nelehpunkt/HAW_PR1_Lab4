//PV-Rechner
//Ein Programm welches einen PV-Rechner simuliert und abhängig vom Ort und der dortigen Globalstrahlung, sowie Neigung und Spitzenleistung der PV-Anlage die monatlichen und jährlichen Energieerträge errechnet.
//Nele Hetzer, Anna Katharina Becker, Finn-Niklas Zymelka
//12.01.2024
//Version 15

#define _CRT_SECURE_NO_DEPRECATE                                                        //Warnungen für Makros werden unterdrückt, veraltet                                      
#define _CRT_SECURE_NO_WARNINGS                                                         //Warnungen werden unterdrückt
#include <stdio.h>                                                                      //Bibliothek



#define AUFGABE_1                                                                       //Makrodeklaration zur Auswahl der Aufgabe, Rest wird auskommentiert
//#define AUFGABE_4                                                                       

double mittlereGlobaleStrahlung(double globalStrahlung[], int anzahlTage);              //ausgelagerte Funktion 
void fill(char zeichen, int anzahl);                                                    //ausgelagerte Funktion um Linie darzustellen

//Beginn Hauptteil
int main(void) {



    FILE* out = NULL;                                                                   //zu Anfang wird Datei geleert (out-output)

    char OutFileName[] = "AusgabePV_Rechner_final.txt";                                       //Name der Datei wird definiert und abgespeichert unter AusgabePV_Rechner.txt



    double monatswerte[] = { 0.52, 1.13, 2.23, 3.55, 4.67, 5.44, 4.82, 4.34, 2.79, 1.49, 0.67, 0.40 }; //durchschnittliche Tagesleistung des jeweiligen Monats

    int tageProMonat[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };                            //Anzahl der tage eines Monats

    double* monate[] = { monatswerte, monatswerte + 1, monatswerte + 2, monatswerte + 3, monatswerte + 4, monatswerte + 5,          //Positionen des Arrays, zuweisen von Zahlen zum Monat
                        monatswerte + 6, monatswerte + 7, monatswerte + 8, monatswerte + 9, monatswerte + 10, monatswerte + 11 };

    double spitzenwert;                                                                //Definieren der Variablen

    int ort, ausrichtung;                                                              //Definieren der Variablen

    double neigungsFaktor[3][12] = {                                                   //Zweidimensionaler Array, 3x 12 Werte, Initialisierung Neigungsfaktor für Aufagbe 1
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},                                          //horizontal
        {1.44, 1.4, 1.17, 1.08, 1.00, 0.96, 0.97, 1.03, 1.17, 1.3, 1.47, 1.42},        //30°
        {1.63, 1.54, 1.15, 0.98, 0.85, 0.81, 0.83, 0.92, 1.14, 1.38, 1.68, 1.61}       //60°
    };

    double neigungsFaktor2[4][12] = {                                                  // Initialisierung NeigungsFaktor für Aufgabe 4, zweidimensionaler Array
       {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},                                           //horizontal
       {1.44, 1.4, 1.17, 1.08, 1.00, 0.96, 0.97, 1.03, 1.17, 1.3, 1.47, 1.42},         //30°
       {1.57, 1.5, 1.19, 1.05, 0.94, 0.9, 0.91, 1, 1.18, 1.37, 1.61, 1.55},            //45°
       {1.63, 1.54, 1.15, 0.98, 0.85, 0.81, 0.83, 0.92, 1.14, 1.38, 1.68, 1.61}        //60°
    };

    double orteWerte[5][12] = {                                                        //Initialisierung für die verschiedenen Orte für Aufgabe 4
        {19, 33, 75, 128, 160, 166, 158, 134, 94, 51, 26, 15},                         //Werte Berlin
        {20, 34, 77, 123, 150, 162, 154, 132, 90, 52, 25, 16},                         //Werte Kassel 
        {29, 45, 85, 130, 153, 174, 164, 140, 99, 62, 36, 24},                         //Stuttgart
        {29, 45, 84, 129, 153, 172, 166, 141, 104, 63, 38, 24},                        //Freiburg
        {16, 32, 69, 107, 145, 163, 149, 135, 84, 46, 20, 12} };                       //Hamburg

    char städte[5][10] = { {"Berlin"}, {"Kassel"}, {"Stuttgart"}, {"Freiburg"}, {"Hamburg"} };                          //zweidimensionaler Array, bezeichnet die Städte(werden definiert)



    char monatsnamen[12][10] = { "Januar", "Februar", "Maerz", "April", "Mai", "Juni", "Juli", "August", "September", "Oktober", "November", "Dezember" };  //Moantsnamen werden definiert 

//Globalstrahlung auf horizontalen Flächen in Hamburg
#ifdef AUFGABE_1                                                                        //aufrufen des Makros

    out = fopen(OutFileName, "wt");

    printf("\nIn diesem Teil wird monatsweise die mittlere monatliche Globalstrahlung auf horizontalen Flaechen in Hamburg berechnet.\n\n");
    fprintf(out,"\nIn diesem Teil wird monatsweise die mittlere monatliche Globalstrahlung auf horizontalen Flaechen in Hamburg berechnet.\n\n");


    for (int neigung = 0; neigung <= 2; ++neigung) {                                    //alle drei Neigunngswinkel werden abgerufen, 0°, 30°,60°
        printf("\nPV-Rechner fuer Hamburg bei einer Neigung von %d Grad:\n\n", neigung * 30);             //0*30, 1*30, 2*30 =>Neigungswinkel Überschrift
        fprintf(out,"\nPV-Rechner fuer Hamburg bei einer Neigung von %d Grad:\n\n", neigung * 30);

        double summeMonatsmittel = 0;                                                   //die summe wird am anfang auf null gesetzt, da noch keine werte errechnet wurden

        printf("Monat:  ");                                                             //ausgabe an den nutzer als beschriftung der tabelle
        fprintf(out,"Monat:  ");
        
        for (int monat = 0; monat < 12; ++monat) {
            printf("%10s |", monatsnamen[monat]);                                       //ausgabe der monate
            fprintf(out, "%10s |", monatsnamen[monat]);                                 //ausgabe der monate zum speichern in der zuvor eröffneten datei
        }
        printf("\n");                                                                   //Absatz 
        fprintf(out,"\n");

        printf("Ertrag: ");                                                             //ausgabe an den nutzer als andere beschriftung der tabelle
        fprintf(out,"Ertrag: ");
        
        for (int monat = 0; monat < 12; ++monat) {
            int anzahlTage = tageProMonat[monat];                                       //abspeicherung in einer neuen variable wie viele tage der jeweilige monat hat
            double wert = monatswerte[monat] * anzahlTage * neigungsFaktor[neigung][monat];             //berechnung für den Monatswert durch multiplizieren des jeweiligen Mittelwertes mit der anzahl der tage und dem neigungsfaktor des dazugehörigen monats


            double mittelwertMonat = mittlereGlobaleStrahlung(monate[monat], anzahlTage);               //mittlere globale strahlung ist der mittelwert über die bestrahlung der erde an einem bestimmten ort über einen bestimmten zeitraum
            summeMonatsmittel += wert;                                                  //die summe des monatsmittel wird errechnet durch stetiges addieren der einzelnen tage
            printf("%10.2lf |", wert);                                                  //ausgabe an den nutzer über die ertragssumme in form einer tabelle durch welche der zugehörige monat klar erkennbar ist
            fprintf(out, "%10.2lf |", wert);                                            //-"- speicherung in der datei

        }



        double mittelwertJahr = summeMonatsmittel / 12;
        printf("\n\nDas jaehrliche Mittel in kWh betraegt: %.2lf\n", mittelwertJahr);
        fprintf(out,"\n\nDas jaehrliche Mittel in kWh betraegt: %.2lf\n", mittelwertJahr);
        fill('_', 151);
        printf("\n");
        fprintf(out,"\n");
    }
    if (out == NULL)                                                                                                //wenn die datei gleich null entspricht ist ein fehler aufgetreten oder sie konnte nicht geöffnet werden 
        printf("Fehler: AusgabePV_Rechner.txt konnte nicht geoeffnet werden\n");                                    //ausgabe an den nutzer-> fehler bei der datei


    if (out)fclose(out);                                                                                            //überprüfung ob datei nicht null entspricht, wenn dies der fall wird die datei geschlossen 


    if (out)
        printf("\nDie Datei wurde erfolgreich kopiert und kann nun geoeffnet werden\n");                            //ausgabe an den nutzer, dass datei geschlossen wurde und nun genutzt werden kann

#endif

#ifdef AUFGABE_4                                                                                                     //Aufrufen des Makros für Aufgabe 4

    int retVal;                                                                                                      //Rückgabewert
    out = fopen(OutFileName, "wt");                                                                                  //Datei wird geöffnet, wt= write, update (vorlesung 8, Folie 32)
    
    printf("\nDies ist ein Programm, in dem ein PV-Rechner simuliert wird. Dafuer wird der Ort und die dortige Globalstrahlung,\nsowie die Neigung und Spitzenleistung der PV-Anlage genutzt.\n");
    printf("Die Werte werden dabei von Ihnen gewaehlt.\n\n\n");
    printf("PV-Rechner fuer den PV-Ertrag von PV-Modulen in verschiedenen Staedten.\nNeigungswinkel und Spitzenleistung werden ebenfalls mit einberechnet.\n\n\n");            //Ausgabe an Nutzer, kurze Erklärung
    fprintf(out,"PV-Rechner fuer den PV-Ertrag von PV-Modulen in verschiedenen Staedten.\nNeigungswinkel und Spitzenleistung werden ebenfalls mit einberechnet.\n\n\n");


    printf("Bitte geben Sie den Ort der PV-Anlage ein (1 - Berlin, 2 - Kassel, 3 - Stuttgart, 4 - Freiburg, 5 - Hamburg): ");      //Aufforderung Zahl zur Eingabe der Stadt
    retVal = scanf("%d", &ort);                                                                                        //einlesen der Benutzereingabe, Ort wird gespeichert
    printf("\n");

    printf("Bitte geben Sie die Ausrichtung der Anlage ein (1 - Horizontal, 2 - 30 Grad, 3 - 45 Grad, 4 - 60 Grad): ");        //Aufforderung Winkel eingeben
    retVal = scanf("%d", &ausrichtung);                                                                               //einlesen der Benutzereingabe
    printf("\n");


    printf("Bitte geben Sie die Spitzenleistung der Anlage ein (1 bis 30): ");                                      //Aufforderung Spitzenleistung eingeben
    retVal = scanf("%lf", &spitzenwert);                                                                              //Rückgabewert wird eingelesen (lf = double)
    printf("\n");
    fill('_', 151);
    printf("\n");

    printf("\nPV-Rechner fuer %s bei einer Neigung von %d Grad und einem  Spitzenwert von %.2lf\n\n", städte[ort - 1], (ausrichtung) * 15, spitzenwert);     //array von 0-4, 4+15=60...Gradzahl bestimmen
    
    fprintf(out, "PV-Rechner fuer %s, Neigung= %d Grad, Spitzenwert= %.2lf\n\n", städte[ort - 1], (ausrichtung) * 15, spitzenwert); //speichern in der Datei


    double summeMonatsmittel = 0;                                                                                   //Mittelwert wird zu Anfang null gesetzt                 

    printf("Monat: ");
    fprintf(out,"Monat: ");
    for (int monat = 0; monat < 12; ++monat) {                                                                      //nur 12 Monate, beginn bei 0, variable wird erhöht
        printf("%10s |", monatsnamen[monat]);                                                                     //Monate werden geprintet
        fprintf(out, "%10s |", monatsnamen[monat]);
    }
    printf("\n");                                                                                                   //absatz
    fprintf(out, "\n");

    printf("Ertrag:");                                                                                              //Ertrag wird geprintet
    fprintf(out,"Ertrag:");
    for (int monat = 0; monat < 12; ++monat) {                                                                      //Moante werden durchlaufen
        int anzahlTage = tageProMonat[monat];                                                                      //wie viele Tage pro Monat
        double wert = monatswerte[monat] * anzahlTage * neigungsFaktor2[ausrichtung - 1][monat] * spitzenwert;      //monatswert wird ausgerechnet, -1 wegen eingabe

        double mittelwertMonat = mittlereGlobaleStrahlung(monate[monat], anzahlTage);                               //
        summeMonatsmittel += wert;                                                                                  //die werte werden einer nach dem anderen einander aufaddiert
        fprintf(out, "%10.2lf |", wert);
        printf("%10.2lf |", wert);
    }


    printf("\n\n");                                                                                                 //Absätze
    fprintf(out,"\n\n");
    fill('_', 151);                                                                                                 //aufrufen der fill funktion für eine einfachere Trennung von ausgaben an den nutzer
    printf("\n");                                                                                                   //Absatz



    double mittelwertJahr = summeMonatsmittel;                                                                      //die summe aller monatsmittel entspricht dem mittel des gesamten Jahres
    fprintf(out, "\nDer Gesamtertrag pro Jahr in kWh betraegt: %.2lf\n", mittelwertJahr);                             //in Datei einspeichern
    printf("\nDer Gesamtertrag pro Jahr in kWh betraegt: %.2lf\n", mittelwertJahr);                                 //Ausgabe an Nutzer Jahresmittelwert



    if (out == NULL)                                                                                                //wenn die datei gleich null entspricht ist ein fehler aufgetreten oder sie konnte nicht geöffnet werden 
        printf("Fehler: AusgabePV_Rechner.txt konnte nicht geoeffnet werden\n");                                    //ausgabe an den nutzer-> fehler bei der datei


    if (out)fclose(out);                                                                                            //überprüfung ob datei nicht null entspricht, wenn dies der fall wird die datei geschlossen 


    if (out)
        printf("\nDie Datei wurde erfolgreich kopiert und kann nun geoeffnet werden\n");                            //ausgabe an den nutzer, dass datei geschlossen wurde und nun genutzt werden kann
#endif



    return 0;
}

double mittlereGlobaleStrahlung(double globalStrahlung[], int anzahlTage)               // Definition der Funktion zur Berechnung vom mittelwert der globalen Strahlung des Monats Aufgabe 1
{
    double summe = 0;                                                                   //zu anfang wird die summe auf null gesetzt, da noch keine werte errechnet wurden

    for (int i = 0; i < anzahlTage; ++i) {
        summe += globalStrahlung[i];                                                    //Summe + Golbalstrahlung = neue Summe
    }

    double mittelwert = summe / anzahlTage;                                             //mittelwert ist gleich die summe aller tage durch die jeweilige anzahl an tagen
    return mittelwert;                                                                  //der mittelwert wird als rückgabewert zurück an die funktion übergeben
}

void fill(char zeichen, int anzahl)                                                     //um Linie zu printen 
{
    for (int i = 0; i < anzahl; i++)
        printf("%c", zeichen);
}