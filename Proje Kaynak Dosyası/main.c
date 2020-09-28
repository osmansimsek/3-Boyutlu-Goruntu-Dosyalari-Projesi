#include <stdint.h>
#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

struct baslikbilgileri
{
    char dosyaformat[25];
    char version[10];
    char alanlar[30];
    char noktalar[20];
    char dataturu[15];
    int noktasayisi;
};
struct sayisalbilgiler
{
    struct baslikbilgileri baslikdegeri;
    float *xdeger;
    float *ydeger;
    float *zdeger;
    int *rdeger;
    int *gdeger;
    int *bdeger;
};

// butun dosya bilgilerinin tutuldugu structir. (sayisalbilgiler).
struct rgbbinarycek
{
    float x;
    float y;
    float z;
    int r;
    int g;
    int b;
};
struct binarycek
{
    float cx;
    float cy;
    float cz;
};
// binary okumasi yaparken kullanilan structlardir.

const char kontroldosyaformat[25]="# Noktalar dosya format\n";
const char kontrolversion[10]="VERSION 1\n";
const char kontrolalanlar_v1[30]="ALANLAR x y z r g b\n";
const char kontrolalanlar_v2[30]="ALANLAR x y z\n";
const char kontrolnoktalar[20]="NOKTALAR";
const char kontroldataturu_v1[15]="DATA ascii\n";
const char kontroldataturu_v2[15]="DATA binary\n";

// ilk 5 satir karsilastirmasi yaparken kullanilan const charlar dir.

struct sayisalbilgiler *liste1;

char hatatut[200][200];
char saglamdosyaadi[100][30];
int dosyaadi[200][200];
int dosyasayac=0;
int saglamdosyasayac=0;
int hatatutsayaci=0;

// globalde tuttugum hem binary icin hemde ascii icin degiskenlerdir.

void dosyabul();
void ascii_binary();
void dosyaoku();
void menuisleri();

// toplam 5 tane ayri fonskiyon kullandim ve prototip olarak tanimladim main fonksiyonun ustune.

int main()
{
    printf("\n");
    printf("           3 BOYUTLU GORUNTU DOSYALARI PROJESI\n\n\n");
    printf("islem 1: Dosya Kontrolu\n");
    printf("islem 2: En Yakin/Uzak Noktalar\n");
    printf("islem 3: Kup\n");
    printf("islem 4: Kure\n");
    printf("islem 5: Nokta Uzakliklari\n");
    printf("islem 6: Cikis\n\n");

// menu islemlerin arayuz kisminin basladigi yerdir ve tum islemler dosyaoku fonksiyonu ile basliyor.

    dosyaoku();

    return 0;
}

void dosyabul ()
{
    // dosyaoku fonksiyonu ile caðirilan fonskiyondur ve dirent.h kutuphanesi kullanýlarak yazýldý.
    // fonskiyonda main klasorunun icinde ki .nkt uzantili dosyalari cekmeye yarar.
    char *ayrac =".";
    char *kelimeayir;
    char isimtut[20][20];
    int sayac2=0, sayac1=0;

    DIR *klasor;
    struct dirent *klasorStructi;

    klasor=opendir(".");
    if(opendir == NULL)
    {
        exit(1);
    }
    // klasorun acildigi yerdir.
    while((klasorStructi=readdir(klasor))!= NULL)
    {
        // output ismi varsa acmicak sekilde ayarlandi.
        if(strstr(klasorStructi->d_name,"output")!=NULL)
            continue;
        kelimeayir=strtok(klasorStructi->d_name,ayrac);
        while(kelimeayir!=NULL)
        {
            strcpy(isimtut[sayac1],kelimeayir);
            kelimeayir=strtok(NULL,ayrac);
        }

        if(strcmp(isimtut[sayac1],"nkt")==0)
        {
            // .nkt uzantisini bulan kisim
            strcpy(dosyaadi[sayac2],klasorStructi->d_name);
            sayac2++;

        }
        sayac1++;
    }

    dosyasayac=sayac2;
    closedir(klasor);
}

void dosyaoku()
{
    // ana fonksiyonumuzdur ve tum islemler burda gerceklesmektedir.
    dosyabul();
    ascii_binary();
    menuisleri();
}

void ascii_binary()
{
    // ascii dosyalarinin cekildigi yerdir.
    FILE *dosya;

    liste1 = malloc(dosyasayac*(sizeof(struct sayisalbilgiler )));
    int z;
    for(z=0; z<dosyasayac; z++)
    {
        liste1[z].xdeger=malloc(2*sizeof(float));
        liste1[z].ydeger=malloc(2*sizeof(float));
        liste1[z].zdeger=malloc(2*sizeof(float));
        liste1[z].rdeger=malloc(2*sizeof(int));
        liste1[z].gdeger=malloc(2*sizeof(int));
        liste1[z].bdeger=malloc(2*sizeof(int));
    }

    char noktalaryazi[50];
    int i;
    // for ile tum ascii dosyalarinin iclerinin dolasilip islem yapildigi yerdir.
    for(i=0; i<dosyasayac; i++)
    {
        int satirsayisi=0;
        int alanlarkontroldegiskeni1=0,alanlarkontroldegiskeni2=0;
        int datakontroldegiskeni1=0,datakontroldegiskeni2=0;
        int hatakontrolsayaci=0,noktasayaci=0;

        char kisim[70];
        char kisim1[20];
        char kisim2[20];

        const char *ayrac=" ";
        char *kelimeayir;
        char path[20]=".nkt";

        strcat(dosyaadi[i],path);
        dosya=fopen(dosyaadi[i],"r");

        if(dosya==NULL)
        {
            printf("%s dosyasi acilmadi.\n",dosyaadi[i]);
            hatakontrolsayaci=1;
        }
        // satir satir okundugu kisimdir. ilk 5 satir icin kontrol yapilir ve 5 satridan sonra sayisal degerler strtok yontemi ile alinmaya baslanir.
        while(fgets(kisim,70,dosya)!=NULL)
        {
            satirsayisi++;
            switch(satirsayisi)
            {

            case 1:

                if(strcmp(kisim,kontroldosyaformat)!= 0)
                {
                    sprintf(hatatut[hatatutsayaci],"%s dosyadaki %d. satirdaki noktalar dosya format kismi hatali yazilmistir.",dosyaadi[i],satirsayisi);
                    hatatutsayaci++;
                    hatakontrolsayaci=1;
                }

                else
                {
                    strcpy(liste1[i].baslikdegeri.dosyaformat,kisim);
                }

                break;

            case 2:

                if(strcmp(kisim,kontrolversion)!= 0)
                {
                    sprintf(hatatut[hatatutsayaci],"%s dosyadaki %d. satirdaki versiyon kismi hatali yazilmistir.",dosyaadi[i],satirsayisi);
                    hatatutsayaci++;
                    hatakontrolsayaci=1;

                }
                else
                {
                    strcpy(liste1[i].baslikdegeri.version,kisim);
                }
                break;

            case 3:

                if(strcmp(kisim,kontrolalanlar_v1)!= 0)
                {
                    if(strcmp(kisim,kontrolalanlar_v2)!= 0)
                    {
                        sprintf(hatatut[hatatutsayaci],"%s dosyadaki %d. satirdaki alanlar kismi hatali yazilmistir.",dosyaadi[i],satirsayisi);
                        hatakontrolsayaci=1;
                        hatatutsayaci++;
                    }
                    else
                    {
                        strcpy(liste1[i].baslikdegeri.alanlar,kisim);
                        alanlarkontroldegiskeni2=2;
                    }
                }
                else
                {
                    strcpy(liste1[i].baslikdegeri.alanlar,kisim);
                    alanlarkontroldegiskeni1=1;
                }
                break;

            case 4:

                sscanf(kisim,"%s %s",kisim1,kisim2);

                if(strcmp(kontrolnoktalar,kisim1)!=0)
                {
                    sprintf(hatatut[hatatutsayaci],"%s dosyadaki %d. satirdaki noktalar kismi hatali yazilmistir.",dosyaadi[i],satirsayisi);
                    hatakontrolsayaci=1;
                    hatatutsayaci++;
                }

                else
                {
                    strcpy(liste1[i].baslikdegeri.noktalar,kisim1);
                    liste1[i].baslikdegeri.noktasayisi=atoi(kisim2);

                    sprintf(noktalaryazi,"%d",liste1[i].baslikdegeri.noktasayisi);

                    if(strcmp(noktalaryazi,kisim2)!=0)
                    {
                        sprintf(hatatut[hatatutsayaci],"%s dosyadaki nokta sayisi yanlis verilmistir.",dosyaadi[i]);
                        hatatutsayaci++;
                        hatakontrolsayaci=1;
                    }
                    else
                        liste1[i].baslikdegeri.noktasayisi=atoi(kisim2);

                }
                break;

            case 5:

                if(strcmp(kontroldataturu_v1,kisim)!=0)
                {
                    if(strcmp(kontroldataturu_v2,kisim)!=0)
                    {

                        sprintf(hatatut[hatatutsayaci],"%s dosyadaki %d. satirdaki data türü kismi hatali yazilmistir",dosyaadi[i],satirsayisi);
                        hatatutsayaci++;
                        hatakontrolsayaci=1;

                    }
                    else
                    {
                        strcpy(liste1[i].baslikdegeri.dataturu,kisim);
                        datakontroldegiskeni2=2;
                    }

                }
                else
                {
                    strcpy(liste1[i].baslikdegeri.dataturu,kisim);
                    datakontroldegiskeni1=1;
                }

                break;

            default :
                break;

            }

            if(satirsayisi>5)
            {
                if(datakontroldegiskeni2==2)
                {

                    fclose(dosya);
                    break;
                }

                // data ascii
                if(datakontroldegiskeni1==1)
                {
                    int strtoksayac=0;
                    // x y z r g b
                    if(alanlarkontroldegiskeni1==1)
                    {
                        kelimeayir=strtok(kisim,ayrac);
                        while(kelimeayir!=NULL)
                        {
                            strtoksayac++;
                            float floatdeger=0;
                            int intdeger=0;
                            if(strtoksayac==1)
                            {
                                floatdeger=atof(kelimeayir);
                                liste1[i].xdeger[satirsayisi-6]=floatdeger;
                                liste1[i].xdeger=realloc(liste1[i].xdeger,(satirsayisi)*sizeof(float));
                            }
                            if(strtoksayac==2)
                            {
                                floatdeger=atof(kelimeayir);
                                liste1[i].ydeger[satirsayisi-6]=floatdeger;
                                liste1[i].ydeger=realloc(liste1[i].ydeger,(satirsayisi)*sizeof(float));
                            }
                            if(strtoksayac==3)
                            {
                                floatdeger=atof(kelimeayir);
                                liste1[i].zdeger[satirsayisi-6]=floatdeger;
                                liste1[i].zdeger=realloc(liste1[i].zdeger,(satirsayisi)*sizeof(float));
                            }
                            if(strtoksayac==4)
                            {
                                intdeger=atoi(kelimeayir);
                                liste1[i].rdeger[satirsayisi-6]=intdeger;
                                liste1[i].rdeger=realloc(liste1[i].rdeger,(satirsayisi)*sizeof(int));
                            }
                            if(strtoksayac==5)
                            {
                                intdeger=atoi(kelimeayir);
                                liste1[i].gdeger[satirsayisi-6]=intdeger;
                                liste1[i].gdeger=realloc(liste1[i].gdeger,(satirsayisi)*sizeof(int));
                            }
                            if(strtoksayac==6)
                            {
                                intdeger=atoi(kelimeayir);
                                liste1[i].bdeger[satirsayisi-6]=intdeger;
                                liste1[i].bdeger=realloc(liste1[i].bdeger,(satirsayisi)*sizeof(int));
                            }
                            kelimeayir=strtok(NULL,ayrac);
                        }
                        if(strtoksayac<6)
                        {
                            sprintf(hatatut[hatatutsayaci],"%s deki dosyadaki %d. noktad x y z r g b degerleri eksik girilmistir.",dosyaadi[i],satirsayisi-5);
                            hatatutsayaci++;
                        }
                        if(strtoksayac<6 && strtoksayac>3)
                        {
                            sprintf(hatatut[hatatutsayaci],"%s deki dosyadaki %d. noktada r g b degerleri eksik girilmistir.",dosyaadi[i],satirsayisi-5);
                            hatatutsayaci++;
                        }
                        if(strtoksayac>6)
                        {
                            sprintf(hatatut[hatatutsayaci],"%s deki dosyadaki %d. noktada x y z r g b degerlerinin disinda deger girilmistir.",dosyaadi[i],satirsayisi-5);
                            hatatutsayaci++;
                        }
                    }
                    // x y z
                    if(alanlarkontroldegiskeni2==2)
                    {
                        kelimeayir=strtok(kisim,ayrac);
                        while(kelimeayir!=NULL)
                        {
                            strtoksayac++;
                            float floatdeger=0;
                            int intdeger=0;
                            if(strtoksayac==1)
                            {
                                floatdeger=atof(kelimeayir);
                                liste1[i].xdeger[satirsayisi-6]=floatdeger;
                                liste1[i].xdeger=realloc(liste1[i].xdeger,(satirsayisi)*sizeof(float));
                            }
                            if(strtoksayac==2)
                            {
                                floatdeger=atof(kelimeayir);
                                liste1[i].ydeger[satirsayisi-6]=floatdeger;
                                liste1[i].ydeger=realloc(liste1[i].ydeger,(satirsayisi)*sizeof(float));
                            }
                            if(strtoksayac==3)
                            {
                                floatdeger=atof(kelimeayir);
                                liste1[i].zdeger[satirsayisi-6]=floatdeger;
                                liste1[i].zdeger=realloc(liste1[i].zdeger,(satirsayisi)*sizeof(float));
                            }
                            kelimeayir=strtok(NULL,ayrac);
                        }
                        if(strtoksayac>3)
                        {
                            sprintf(hatatut[hatatutsayaci],"%s deki dosyadaki %d. noktada x y z degelerinden baska deger girilmistir.\n",dosyaadi[i],satirsayisi-5);
                            hatatutsayaci++;
                        }
                        if(strtoksayac<3)
                        {
                            sprintf(hatatut[hatatutsayaci],"%s deki dosyadaki %d. noktada x y z degerleri eksik girilmistir.",dosyaadi[i],satirsayisi-5);
                            hatatutsayaci++;
                        }
                    }
                }
            }
        }
        if(datakontroldegiskeni2==2)
        {
            fopen(dosyaadi[i],"r");
            while(fgets(kisim,70,dosya))
            {

                noktasayaci++;

                if(noktasayaci==5)
                    break;

            }

            if(alanlarkontroldegiskeni1==1)
            {

                while(1)
                {
                    size_t sayi;
                    struct rgbbinarycek binoku;
                    sayi = fread(&binoku, sizeof(binoku), 1, dosya);
                    if(sayi < 1)
                    {
                        break;
                    }

                    liste1[i].xdeger[noktasayaci-5] = binoku.x;
                    liste1[i].ydeger[noktasayaci-5] = binoku.y;
                    liste1[i].zdeger[noktasayaci-5] = binoku.z;
                    liste1[i].rdeger[noktasayaci-5] = binoku.r;
                    liste1[i].gdeger[noktasayaci-5] = binoku.g;
                    liste1[i].bdeger[noktasayaci-5] = binoku.b;
                    noktasayaci++;
                    liste1[i].xdeger=realloc(liste1[i].xdeger,(10*(noktasayaci-4))*sizeof(float));
                    liste1[i].ydeger=realloc(liste1[i].ydeger,(10*(noktasayaci-4))*sizeof(float));
                    liste1[i].zdeger=realloc(liste1[i].zdeger,(10*(noktasayaci-4))*sizeof(float));
                    liste1[i].rdeger=realloc(liste1[i].rdeger,(10*(noktasayaci-4))*sizeof(int));
                    liste1[i].gdeger=realloc(liste1[i].gdeger,(10*(noktasayaci-4))*sizeof(int));
                    liste1[i].bdeger=realloc(liste1[i].bdeger,(10*(noktasayaci-4))*sizeof(int));

                }

            }
            if(alanlarkontroldegiskeni2==2)
            {
                while(1)
                {
                    size_t sayi;
                    struct binarycek binoku2;
                    sayi = fread(&binoku2, sizeof(binoku2), 1, dosya);

                    if(sayi < 1)
                    {
                        break;
                    }

                    liste1[i].xdeger[noktasayaci-5] = binoku2.cx;
                    liste1[i].ydeger[noktasayaci-5] = binoku2.cy;
                    liste1[i].zdeger[noktasayaci-5] = binoku2.cz;
                    noktasayaci++;
                    liste1[i].xdeger=realloc(liste1[i].xdeger,(10*(noktasayaci-4)*sizeof(float)));
                    liste1[i].ydeger=realloc(liste1[i].ydeger,(10*(noktasayaci-4)*sizeof(float)));
                    liste1[i].zdeger=realloc(liste1[i].zdeger,(10*(noktasayaci-4)*sizeof(float)));

                }
            }
            fclose(dosya);
        }

        if(datakontroldegiskeni1==1)
        {
            if(satirsayisi-5!=liste1[i].baslikdegeri.noktasayisi)
            {
                sprintf(hatatut[hatatutsayaci],"%s dosyasinda ki nokta sayisi verilen nokta sayisina esit degildir.\n",dosyaadi[i]);
                hatakontrolsayaci=1;
                hatatutsayaci++;
            }
        }

        if(datakontroldegiskeni2==2)
        {
            if(noktasayaci-5!=liste1[i].baslikdegeri.noktasayisi)
            {
                sprintf(hatatut[hatatutsayaci],"%s dosyasinda ki nokta sayisi verilen nokta sayisina esit degildir.\n",dosyaadi[i]);
                hatakontrolsayaci=1;
                hatatutsayaci++;
            }
        }
        if(hatakontrolsayaci==0)
        {
            strcpy(saglamdosyaadi[saglamdosyasayac],dosyaadi[i]);
            saglamdosyasayac++;
        }

        fclose(dosya);
    }
}

void menuisleri()
{
    int i,j,k,a,x;
    float cx,cy,cz,cr;

    FILE *outputdosya;
    outputdosya = fopen("output.nkt","w");
    printf("1' den 6'ya kadar giris yapiniz\n\n");

// islem menusune giris kismi

    int islemkontorl=0;
    while(1)
    {
        int hatakontrolsayaci;
        int secim;
        printf("Yapicaginiz islemi seciniz: \n");
        scanf("%d",&secim);

        fflush(stdin);
        switch(secim)
        {

        case 1:

            islemkontorl=1;
            outputdosya=fopen("output.nkt","a");
            fprintf(outputdosya,"Islem 1: \n");
            for(i=0; i<hatatutsayaci; i++)
            {
                fprintf(outputdosya,"%s \n",hatatut[i]);
            }
            for(k=0; k<hatatutsayaci; k++)
            {
                printf("%s \n",hatatut[k]);
            }

            fclose(outputdosya);

            break;

        case 2:

            if(islemkontorl!=1)
            {

                printf("ilk islem olarak islem 1 yapilmasi gerekmektedir tekrar seciniz.\n");
                continue;

            }

            outputdosya = fopen("output.nkt","a");
            fprintf(outputdosya,"Islem 2: \n");
            for(i=0; i<dosyasayac; i++)
            {
                for(x=0; x<saglamdosyasayac; x++)
                {
                    if(strcmp(saglamdosyaadi[x],dosyaadi[i])==0)
                    {
                        int jtut=0,ktut=0;
                        int jtut2=0,ktut2=0;
                        double enbuyuk=0,enkucuk=3000;
                        double deger=0;
                        double sonuc=0;
                        unsigned long sayac=0;

                        for(j=0; j<liste1[i].baslikdegeri.noktasayisi; j++)
                        {
                            for(k=j+1; k<liste1[i].baslikdegeri.noktasayisi; k++)
                            {
                                deger = pow((liste1[i].xdeger[j]-liste1[i].xdeger[k]),2) + pow((liste1[i].ydeger[j]-liste1[i].ydeger[k]),2) + pow((liste1[i].zdeger[j]-liste1[i].zdeger[k]),2);
                                deger = sqrt(fabs(deger));

                                if(deger>enbuyuk)
                                {
                                    enbuyuk=deger;
                                    jtut=j;
                                    ktut=k;
                                }
                                if(deger<enkucuk)
                                {
                                    enkucuk=deger;
                                    jtut2=j;
                                    ktut2=k;
                                }
                            }
                        }

                        if(strcmp(liste1[i].baslikdegeri.alanlar,"ALANLAR x y z r g b\n")==0)
                        {
                            fprintf(outputdosya,"%s dosyasi: \n",saglamdosyaadi[x]);
                            fprintf(outputdosya,"En Uzak Noktalar: \n");
                            fprintf(outputdosya,"%f %f %f %d %d %d\n",liste1[i].xdeger[jtut],liste1[i].ydeger[jtut],liste1[i].zdeger[jtut],liste1[i].rdeger[jtut],liste1[i].gdeger[jtut],liste1[i].bdeger[jtut]);
                            fprintf(outputdosya,"%f %f %f %d %d %d\n",liste1[i].xdeger[ktut],liste1[i].ydeger[ktut],liste1[i].zdeger[ktut],liste1[i].rdeger[ktut],liste1[i].gdeger[ktut],liste1[i].bdeger[ktut]);
                            fprintf(outputdosya,"En Yakin Noktalar: \n");
                            fprintf(outputdosya,"%f %f %f %d %d %d\n",liste1[i].xdeger[jtut2],liste1[i].ydeger[jtut2],liste1[i].zdeger[jtut2],liste1[i].rdeger[jtut2],liste1[i].gdeger[jtut2],liste1[i].bdeger[jtut2]);
                            fprintf(outputdosya,"%f %f %f %d %d %d\n\n",liste1[i].xdeger[ktut2],liste1[i].ydeger[ktut2],liste1[i].zdeger[ktut2],liste1[i].rdeger[ktut2],liste1[i].gdeger[ktut2],liste1[i].bdeger[ktut2]);

                        }
                        if(strcmp(liste1[i].baslikdegeri.alanlar,"ALANLAR x y z\n")==0)
                        {
                            fprintf(outputdosya,"%s dosyasi: \n",saglamdosyaadi[x]);
                            fprintf(outputdosya,"En Uzak Noktalar: \n");
                            fprintf(outputdosya,"%f %f %f\n",liste1[i].xdeger[jtut],liste1[i].ydeger[jtut],liste1[i].zdeger[jtut]);
                            fprintf(outputdosya,"%f %f %f\n",liste1[i].xdeger[ktut],liste1[i].ydeger[ktut],liste1[i].zdeger[ktut]);
                            fprintf(outputdosya,"En Yakin Noktalar: \n");
                            fprintf(outputdosya,"%f %f %f\n",liste1[i].xdeger[jtut2],liste1[i].ydeger[jtut2],liste1[i].zdeger[jtut2]);
                            fprintf(outputdosya,"%f %f %f\n\n",liste1[i].xdeger[ktut2],liste1[i].ydeger[ktut2],liste1[i].zdeger[ktut2]);
                        }
                    }
                }
            }

            fclose(outputdosya);

            break;

        case 3:

            if(islemkontorl!=1)
            {

                printf("ilk islem olarak islem 1 yapilmasi gerekmektedir tekrar seciniz.\n");
                continue;

            }

            outputdosya = fopen("output.nkt","a");
            fprintf(outputdosya,"Islem 3: \n");
            for(i=0; i<dosyasayac; i++)
            {
                for(x=0; x<saglamdosyasayac; x++)
                {
                    if(strcmp(saglamdosyaadi[x],dosyaadi[i])==0)
                    {
                        if(strcmp(liste1[i].baslikdegeri.alanlar,"ALANLAR x y z r g b\n")==0)
                        {

                            double minx=1000, miny=1000, minz=1000, maxx=0, maxy=0, maxz=0, uzunluk;
                            int indis1,indis2,indis3,indis4,indis5,indis6;

                            for(k=0; k<liste1[i].baslikdegeri.noktasayisi; k++)
                            {
                                if(liste1[i].xdeger[k]<minx)
                                {
                                    minx=liste1[i].xdeger[k];
                                    indis1=k;
                                }
                                if(liste1[i].ydeger[k]<miny)
                                {
                                    miny=liste1[i].ydeger[k];
                                    indis2=k;
                                }
                                if(liste1[i].zdeger[k]<minz)
                                {
                                    minz=liste1[i].zdeger[k];
                                    indis3=k;
                                }
                            }
                            for(k=0; k<liste1[i].baslikdegeri.noktasayisi; k++)
                            {
                                if(liste1[i].xdeger[k]>maxx)
                                {
                                    maxx=liste1[i].xdeger[k];
                                    indis4=k;
                                }
                                if(liste1[i].ydeger[k]>maxy)
                                {
                                    maxy=liste1[i].ydeger[k];
                                    indis5=k;
                                }
                                if(liste1[i].zdeger[k]>maxz)
                                {
                                    maxz=liste1[i].zdeger[k];
                                    indis6=k;
                                }
                            }

                            uzunluk=maxx-minx;
                            if( (maxy-miny) > uzunluk )
                            {
                                uzunluk= maxy-miny;
                            }
                            if( (maxz-minz) > uzunluk )
                            {
                                uzunluk= maxz-minz;
                            }

                            fprintf(outputdosya,"%s dosyasi: \n",saglamdosyaadi[x]);
                            fprintf(outputdosya,"%f %f %f %d %d %d\n", maxx-uzunluk, maxy, maxz,liste1[i].rdeger[indis1],liste1[i].rdeger[indis5],liste1[i].rdeger[indis6] );
                            fprintf(outputdosya,"%f %f %f %d %d %d\n", maxx, maxy, maxz,liste1[i].rdeger[indis4],liste1[i].rdeger[indis5],liste1[i].rdeger[indis6] );
                            fprintf(outputdosya,"%f %f %f %d %d %d\n", maxx, maxy-uzunluk, maxz,liste1[i].rdeger[indis4],liste1[i].rdeger[indis2],liste1[i].rdeger[indis6] );
                            fprintf(outputdosya,"%f %f %f %d %d %d\n", maxx-uzunluk, maxy-uzunluk, maxz,liste1[i].rdeger[indis1],liste1[i].rdeger[indis2],liste1[i].rdeger[indis6] );
                            fprintf(outputdosya,"%f %f %f %d %d %d\n", maxx-uzunluk, maxy, maxz-uzunluk,liste1[i].rdeger[indis1],liste1[i].rdeger[indis5],liste1[i].rdeger[indis3] );
                            fprintf(outputdosya,"%f %f %f %d %d %d\n", maxx, maxy, maxz-uzunluk,liste1[i].rdeger[indis4],liste1[i].rdeger[indis5],liste1[i].rdeger[indis3] );
                            fprintf(outputdosya,"%f %f %f %d %d %d\n", maxx, maxy-uzunluk, maxz-uzunluk,liste1[i].rdeger[indis4],liste1[i].rdeger[indis2],liste1[i].rdeger[indis3] );
                            fprintf(outputdosya,"%f %f %f\n\n", maxx-uzunluk, maxy-uzunluk, maxz-uzunluk );


                        }
                        if(strcmp(liste1[i].baslikdegeri.alanlar,"ALANLAR x y z\n")==0)
                        {

                            double minx=1000, miny=1000, minz=1000, maxx=0, maxy=0, maxz=0, uzunluk;

                            for(k=0; k<liste1[i].baslikdegeri.noktasayisi; k++)
                            {
                                if(liste1[i].xdeger[k]<minx)
                                {
                                    minx=liste1[i].xdeger[k];
                                }
                                if(liste1[i].ydeger[k]<miny)
                                {
                                    miny=liste1[i].ydeger[k];
                                }
                                if(liste1[i].zdeger[k]<minz)
                                {
                                    minz=liste1[i].zdeger[k];
                                }
                            }
                            for(k=0; k<liste1[i].baslikdegeri.noktasayisi; k++)
                            {
                                if(liste1[i].xdeger[k]>maxx)
                                {
                                    maxx=liste1[i].xdeger[k];
                                }
                                if(liste1[i].ydeger[k]>maxy)
                                {
                                    maxy=liste1[i].ydeger[k];
                                }
                                if(liste1[i].zdeger[k]>maxz)
                                {
                                    maxz=liste1[i].zdeger[k];
                                }
                            }

                            uzunluk=maxx-minx;
                            if( (maxy-miny) > uzunluk )
                            {
                                uzunluk= maxy-miny;
                            }
                            if( (maxz-minz) > uzunluk )
                            {
                                uzunluk= maxz-minz;
                            }

                            fprintf(outputdosya,"%s dosyasi: \n",saglamdosyaadi[x]);
                            fprintf(outputdosya,"%f %f %f\n", maxx-uzunluk, maxy, maxz );
                            fprintf(outputdosya,"%f %f %f\n", maxx, maxy, maxz );
                            fprintf(outputdosya,"%f %f %f\n", maxx, maxy-uzunluk, maxz );
                            fprintf(outputdosya,"%f %f %f\n", maxx-uzunluk, maxy-uzunluk, maxz );
                            fprintf(outputdosya,"%f %f %f\n", maxx-uzunluk, maxy, maxz-uzunluk );
                            fprintf(outputdosya,"%f %f %f\n", maxx, maxy, maxz-uzunluk );
                            fprintf(outputdosya,"%f %f %f\n", maxx, maxy-uzunluk, maxz-uzunluk );
                            fprintf(outputdosya,"%f %f %f\n\n", maxx-uzunluk, maxy-uzunluk, maxz-uzunluk );


                        }
                    }
                }
            }

            fclose(outputdosya);

            break;

        case 4:

            if(islemkontorl!=1)
            {

                printf("ilk islem olarak islem 1 yapilmasi gerekmektedir tekrar seciniz.\n");
                continue;

            }
            printf("Kupun x y z kordinatlarini ve yaricapini giriniz:\n");
            printf("cx giriniz : ");
            scanf("%f",&cx);
            fflush(stdin);
            printf("cy giriniz : ");
            scanf("%f",&cy);
            fflush(stdin);
            printf("cz giriniz : ");
            scanf("%f",&cz);
            fflush(stdin);
            printf("cr giriniz : ");
            scanf("%f",&cr);
            fflush(stdin);

            outputdosya = fopen("output.nkt","a");
            fprintf(outputdosya,"Islem 4: \n");
            for(i=0; i<dosyasayac; i++)
            {
                for(k=0; k<saglamdosyasayac; k++)
                {
                    if(strcmp(saglamdosyaadi[k],dosyaadi[i])==0)
                    {
                        fprintf(outputdosya,"%s dosyasi: \n",saglamdosyaadi[x]);
                        fprintf(outputdosya,"%f = (kullanicidan alinan kürenin x degeri)\n",cx);
                        fprintf(outputdosya,"%f = (kullanicidan alinan kürenin y degeri)\n",cy);
                        fprintf(outputdosya,"%f = (kullanicidan alinan kürenin z degeri)\n",cz);
                        fprintf(outputdosya,"%f = (kullanicidan alinan kürenin r degeri)\n",cr);
                        fprintf(outputdosya,liste1[i].baslikdegeri.alanlar);
                        fprintf(outputdosya,liste1[i].baslikdegeri.noktalar);
                        fprintf(outputdosya," %d \n",liste1[i].baslikdegeri.noktasayisi);
                        fprintf(outputdosya,liste1[i].baslikdegeri.dataturu);

                        for(x=0; x<liste1[i].baslikdegeri.noktasayisi; x++)
                        {
                            double crkare=pow(cr,2);
                            double yaricapsonucukaresi=0;
                            yaricapsonucukaresi= pow((liste1[i].xdeger[x]-cx),2)+pow((liste1[i].ydeger[x]-cy),2)+pow((liste1[i].zdeger[x]-cz),2);

                            if(yaricapsonucukaresi<=crkare)
                            {
                                if(strcmp(liste1[i].baslikdegeri.alanlar,"ALANLAR x y z\n")==0)
                                {
                                    fprintf(outputdosya,"%f %f %f\n",liste1[i].xdeger[x],liste1[i].ydeger[x],liste1[i].zdeger[x]);
                                }
                                if(strcmp(liste1[i].baslikdegeri.alanlar,"ALANLAR x y z r g b\n")==0)
                                {
                                    fprintf(outputdosya,"%f %f %f %d %d %d\n",liste1[i].xdeger[x],liste1[i].ydeger[x],liste1[i].zdeger[x],liste1[i].rdeger[x],liste1[i].gdeger[x],liste1[i].bdeger[x]);
                                }
                            }
                        }
                    }
                }
            }

            fclose(outputdosya);

            break;

        case 5:

            if(islemkontorl!=1)
            {

                printf("ilk islem olarak islem 1 yapilmasi gerekmektedir tekrar seciniz.\n");
                continue;

            }

            outputdosya = fopen("output.nkt","a");
            fprintf(outputdosya,"Islem 5: \n");
            for(i=0; i<dosyasayac; i++)
            {
                for(x=0; x<saglamdosyasayac; x++)
                {
                    if(strcmp(saglamdosyaadi[x],dosyaadi[i])==0)
                    {
                        float ortalamadeger=0;
                        double deger=0;
                        double sonuc=0;
                        unsigned long sayac=0;
                        for(j=0; j<liste1[i].baslikdegeri.noktasayisi; j++)
                        {
                            for(k=j+1; k<liste1[i].baslikdegeri.noktasayisi; k++)
                            {
                                deger = pow((liste1[i].xdeger[j]-liste1[i].xdeger[k]),2) +
                                        pow((liste1[i].ydeger[j]-liste1[i].ydeger[k]),2) +
                                        pow((liste1[i].zdeger[j]-liste1[i].zdeger[k]),2);
                                deger = sqrt(fabs(deger));
                                sonuc += deger;
                                sayac++;
                            }
                        }
                        ortalamadeger = sonuc / sayac;
                        fprintf(outputdosya,"%s dosyasi: \n",saglamdosyaadi[x]);
                        fprintf(outputdosya,"%f\n\n",ortalamadeger);
                    }
                }
            }

            fclose(outputdosya);

            break;

        case 6:

            exit(1);

            break;

        default:

            printf("hatali giris yaptiniz \n");
            printf("tekrar giriniz \n");
            break;
        }
    }
}

