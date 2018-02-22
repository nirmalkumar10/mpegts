#include <stdio.h>
#include <stdlib.h>
int main(int argc,char *argv[])
{
    if(argc < 3 ){
        printf("Usage : [executable name] [filename containing TS packet dump] [PID <19B1> ]\n Example ./a.out mbr.ts 19B1\n\n");
        exit(0);
    }
    char c,d,e,h,i,j,k;
    char f = '0';
    char g = '0';
    char old,new;
    int count;
    int temp,temp2;
    int firstpass = 1;

    FILE *fp = fopen(argv[1],"rb");
    if(fp != NULL) {
        while(!feof(fp)){

            if(f == '4' && g == '7'){
                fseek(fp,-2,SEEK_CUR);
            }
            c = fgetc(fp);
            if(c == '4'){           // Sync
                d = fgetc(fp);
                if(d == '7'){
                    e = fgetc(fp);
                    f = fgetc(fp);
                    g = getc(fp);

                    temp = f > '9' ? (f - 55) : (f - '0');
                    temp2 = argv[2][0] > '9' ? (argv[2][0] - 55) : (argv[2][0] - '0');
                    if((temp&1) == (temp2&1)&& g == argv[2][1]) {
                    //if(((f == '0' || f == '4') && g == '1')){  //PID
                        h = fgetc(fp);
                        i = fgetc(fp);
                        j = getc(fp);
                 //       if(i == '0' && j == '0'){          //PID
                          if((i == argv[2][2]) && (j == argv[2][3])){
                            printf("--------------------------------------------------------------------------------------\n");
                            printf("SYNC(8) %c%c",c,d);
                            temp = f > '9' ? (f - 55) : (f - '0');
                            printf("| TEI(1) %d",temp&0x8);
                            printf("| PUSI(1) %d",temp&0x4);
                            printf("| TP(1) %d",temp&0x2);
                            printf("| PID(13) %x%c ",(temp&1),g);
                            printf("%c%c ",i,j);
                            fgetc(fp);
                            k = fgetc(fp);
                            temp = k > '9' ? (k - 55) : (k - '0');
                            printf("| TSC(2) %x",temp&0xC);
                            printf("| AFC(2) %x",temp&3);

                        }
                        new = fgetc(fp);
                        if(!firstpass){
                            if(new == 'A') {
                                if( new != old+8){
                                    printf("CC Discontinuity\n");
                                }
                            }
                            else if(new == '0'){
                                if(new != old-22){
                                    printf("CC Discontinuity\n");
                                }
                            }
                            else if( new != old+1) {
                                printf("CC Discontinuity\n");
                            }
                        }
                        firstpass =0;
                        old = new;
                        printf("| CC(4) %c|",old);
                        printf(" %c",fgetc(fp));
                        printf("%c",fgetc(fp));
                        printf("%c",fgetc(fp));
                        printf("\n");
                        fseek(fp,-3,SEEK_CUR);
                    }
                }
            }
        }
        fclose(fp);
    }
    else{
        perror(argv[1]);
    }
}
