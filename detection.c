/*  Detection de contours dans une image en utilisant les filtres:
       - negatif
       - roberts
       - sobel
       - prewitt
   
   Programme fait par semaj_james
   mail: florentjustin@hotmail.com   */
   

#include<stdio.h>
#include<math.h>


char menu();
FILE * OpenFile(char *nom_fic, char *mode);
void c38(FILE *fic, FILE *fic2);
void bin(FILE *fic2, FILE *fic3,char men);
void negatif(FILE *fic, FILE *fic2);
void roberts(FILE *fic, FILE *fic2);
void sobel(FILE *fic, FILE *fic2);
void prewitt(FILE *fic, FILE *fic2);


int main(void)
{
FILE *fic,*fic2,*fic3;
char men;

do
   {
   //le fichier 99C.pgm doit etre dans le repertoire courant au programme
   fic=OpenFile("99C.pgm","rb"); 
   men=menu();
   switch(men)
      {
      case '1': fic2=OpenFile("negatif.pgm","w+b");    
	      c38(fic,fic2);
	      negatif(fic,fic2);
	      break;
       case '2': fic2=OpenFile("roberts.pgm","w+b");
	       c38(fic,fic2);
	       roberts(fic,fic2);
	       fclose(fic2);
	       fic2=OpenFile("roberts.pgm","rb");// sert pour la binarisation
	       fic3=OpenFile("broberts.pgm","wb");
	       break;
       case '3': fic2=OpenFile("sobel.pgm","w+b");
	       c38(fic,fic2);
	       sobel(fic,fic2);
	       fclose(fic2);
	       fic2=OpenFile("sobel.pgm","rb");
	       fic3=OpenFile("bsobel.pgm","wb");
	       break;
       case '4': fic2=OpenFile("prewitt.pgm","w+b");
	       c38(fic,fic2);
	       prewitt(fic,fic2);
	       fclose(fic2);
	       fic2=OpenFile("prewitt.pgm","rb");
	       fic3=OpenFile("bprewitt.pgm","wb");
	       break;
       case '5': break;
       }
       
   if (men!='5')
      {
      if(men!='1')
         {
         bin(fic2,fic3,men);
         fclose(fic3);
         }
      fclose(fic);   
      fclose(fic2);
      puts("\nImage traitee avec succes\n\n"); 
      }
           
   }while(men!='5');

return 0;
}

/*************************************************************************/
// ouverture d'un fichier

FILE * OpenFile(char *nom_fic, char *mode)
{
   FILE * p_file = fopen (nom_fic, mode);
   if(p_file==NULL)
      {
      printf("Impossible d'ouvrir ou d'ecrire sur le fichier: %s\n"
             "Programme arrete",nom_fic);
      exit(1);
      } 
   return p_file;
} 

/*************************************************************************/

char menu()
{
char men;

do
   {
   puts("*********************************************");
   puts("*                                           *");
   puts("*           TRAITEMENTS DE L'IMAGE          *");
   puts("*                                           *");
   puts("*********************************************\n\n");
   puts("Utilisation du filtre :\n");
   puts("1 => Negatif");
   puts("2 => Gradient de Roberts");
   puts("3 => Sobel");
   puts("4 => Prewitt");
   puts("5 => Quitter");
   men=getch();
   }while(men < '1' || men > '5');
return men;
}

/**************************************************************************/

void c38(FILE *fic, FILE *fic2) // traitement des 38 premiers caracteres
{
char carac[38];

fread(carac,1,38,fic);
fwrite(carac,1,38,fic2);
}

/**************************************************************************/

void negatif(FILE *fic, FILE *fic2)
{
unsigned char data[207][250];
int ligne,col;

for(ligne=0;ligne<207;ligne++)     // lecture de 99c.pgm
   {
    fread(&data[ligne][0],1,250,fic);

// ecriture dans negatif.pgm chaque pixel est remplace par son complement … 255

   for(col=0;col<250;col++)
      {
      data[ligne][col]=255-data[ligne][col];
      fwrite(&data[ligne][col],1,1,fic2);
      }
    }
}

/*************************************************************************/

void roberts(FILE *fic, FILE *fic2)
{
int gx,gy,ligne,col;
long g;
unsigned char ampl[250];
unsigned char data[207][250];

for(ligne=0;ligne<207;ligne++)
    fread(&data[ligne][0],1,250,fic);    // lecture de 99c.pgm

     // ecriture dans roberts.pgm  Gradient de Roberts

for(ligne=0;ligne<207;ligne++)
   {
   for(col=0;col<249;col++)
      {
       if (ligne==206)    // rempli la 207eme ligne de la photo
	 ampl[col]=0;
       else
	  { // derivee en x
	  gx=data[ligne][col+1]-data[ligne][col];

	  // derivee en y

	  gy=data[ligne+1][col]-data[ligne][col];

	   // amplitude

	  g=sqrt((float)gx*gx+(float)gy*gy);
	  if (g>255)  g=255;
	  ampl[col]=g;
	  }
      }
   ampl[249]=0;              // rempli la colonne 249 de la photo
   fwrite(&ampl[0],1,250,fic2);
   }
}

/**************************************************************************/

void sobel(FILE *fic, FILE *fic2)
{
int gx,gy,ligne,col;
long g;
unsigned char ampl[250];
unsigned char data[207][250];

for(ligne=0;ligne<207;ligne++)
    fread(&data[ligne][0],1,250,fic);    // lecture de 99c.pgm

     // ecriture dans sobel.pgm

for(ligne=0;ligne<207;ligne++)
   {
   for(col=0;col<250;col++)
      {
       if (ligne==206 || ligne==0)  // rempli la 1ere et la 207eme ligne de la photo
	 ampl[col]=0;
       else if (col==0 || col==249)    // rempli la 1ere et la 250eme colonne de la photo
	 ampl[ligne]=0;
       else
	  { // derivee en x

	  gx=(int)(
	  -  (data[ligne-1][col-1] + data[ligne][col-1] + data[ligne+1][col-1])
	  +  (data[ligne-1][col+1] + data[ligne][col+1] + data[ligne+1][col+1]));

		 // derivee en y

	  gy=(int)(
	  -  (data[ligne-1][col-1] + data[ligne-1][col] + data[ligne-1][col+1])
	  +  (data[ligne+1][col-1] + data[ligne+1][col] + data[ligne+1][col+1]));

		 // amplitude

	  g=sqrt((float)gx*gx+(float)gy*gy);               // amplitude
	  if (g>255)  g=255;
	  ampl[col]=g;
	  }
      }
   fwrite(&ampl[0],1,250,fic2);
   }
}

/**************************************************************************/

void prewitt(FILE *fic, FILE *fic2)
{
int gx,gy,ligne,col;
long g;
unsigned char ampl[250];
unsigned char data[207][250];

for(ligne=0;ligne<207;ligne++)
    fread(&data[ligne][0],1,250,fic);    // lecture de 99c.pgm

     // ecriture dans sobel.pgm

for(ligne=0;ligne<207;ligne++)
   {
   for(col=0;col<250;col++)
      {
       if (ligne==206 || ligne==0)  // rempli la 1ere et la 207eme ligne de la photo
	 ampl[col]=0;
       if (col==0 || col==249)    // rempli la 1ere et la 250eme colonne de la photo
	 ampl[ligne]=0;
       else
	  {      // derivee en x
	  gx=(int)(
	  -  (data[ligne-1][col-1] + 2*data[ligne][col-1] + data[ligne+1][col-1])
	  +  (data[ligne-1][col+1] + 2*data[ligne][col+1] + data[ligne+1][col+1]));

		 // derivee en y

	  gy=(int)(
	  -  (data[ligne-1][col-1] + 2*data[ligne-1][col] + data[ligne-1][col+1])
	  +  (data[ligne+1][col-1] + 2*data[ligne+1][col] + data[ligne+1][col+1]));

		 // amplitude

	  g=sqrt((float)gx*gx+(float)gy*gy);
	  if (g>255)  g=255;
	  ampl[col]=g;
	  }
      }
   fwrite(&ampl[0],1,250,fic2);
   }
}

/**************************************************************************/

void bin(FILE *fic2, FILE *fic3,char men)
{
int seuil,max,min,ligne,col;
unsigned char data[207][250];

c38(fic2,fic3);


for(ligne=0;ligne<207;ligne++)     // lecture du fichier.pgm
    fread(&data[ligne][0],1,250,fic2);

// definition du seuil


if (men=='2')  
   seuil=11;  // seuil pour roberts
else
   {   

   for(ligne=0;ligne<207;ligne++)
      {
      for(col=0;col<250;col++)
	 {
	 if (col==0 && ligne==0)    // initialise max et min et evite max>250 et min<5
	    { max=2;   min=250; }
	 if (data[ligne][col] < min && data[ligne][col] > 5)   min=data[ligne][col];
	 if (data[ligne][col] > max && data[ligne][col] < 250 )   max=data[ligne][col];
	 }
      }
   seuil=(max+min)/4;  //definition du niveau du seuil
   }

// ecriture de binarisation  dans fic3
for(ligne=0;ligne<207;ligne++)
   {
   for(col=0;col<250;col++)
      {
      if (data[ligne][col] < seuil)
	 data[ligne][col]=0;
      else
	 data[ligne][col]=255;
      fwrite(&data[ligne][col],1,1,fic3);
      }
    }
}
