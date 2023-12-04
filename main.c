#include "input.h"
/***************************************************************************************************
Command Instructions
***************************************************************************************************/
//To Compile: make
//To Run: ./project c17.isc 
/***************************************************************************************************
 Main Function
***************************************************************************************************/
int main(int argc,char **argv)
{
FILE *Isc,*Pat,*Res;                  //File pointers used for .isc, .pattern, and .res files
int Npi,Npo,Tgat;                     //Tot no of PIs,Pos,Maxid,Tot no of patterns in.vec,.fau
GATE *Node;                           //Structure to store the ckt given in .isc file 
clock_t Start,End;                    //Clock variables to calculate the Cputime
double Cpu;                           //Total cpu time
int i,j;                              //Temporary variables




/****************PART 1.-Read the .isc file and store the information in Node structure***********/
Npi=Npo=Tgat=0;                                //Intialize values of all variables
Isc=fopen(argv[1],"r");                        //File pointer to open .isc file 
Node=(GATE *) malloc(Mnod * sizeof(GATE));     //Dynamic memory allocation for Node structure
Tgat=ReadIsc(Isc,Node);                        //Read .isc file and return index of last node
fclose(Isc);                                   //Close file pointer for .isc file
PrintGats(Node,Tgat);                          //Print the information of each active gate in Node structure after reading .isc file
CountPri(Node,Tgat,&Npi,&Npo);                 //Count the No of Pis and Pos
int patternList[Mpt][Mpi];
// Pat = fopen(argv[2],"r");
// ReadPattern(Pat, patternList, Npi);
// fclose(Pat);
// printPatternList(patternList, Npi);
// Res = fopen(argv[3],"w");
// simulateLogic(Node, patternList, Tgat, Res);
//fclose(Res);

printf("\n\nNpi: %d Npo: %d\n",Npi,Npo);       //Print the no of primary inputs and outputs

int faultCount = 0 ;//number of all faults
int successFCount = 0;// number of success fault count
int failedFCount = 0; // number of fail fault count
int timeOutFCount =0; // number of time out fault count

GV* gv = malloc(sizeof(GV));
//483 - 487 st1
for(i = 0;i<=Tgat;i++){
    if(Node[i].Type != 0){
        for(j=0;j<2;j++){
            gv->g = i;
            gv->v = j;
            faultCount++;
            int state  = podem(Node,gv, Tgat);
            if(state == SUCCESS){
                successFCount++;
                //printPI(Node);
            }else if(state == FAILURE){
                failedFCount++;
            }else{
                timeOutFCount++;
            }
        }
    }
}

checkFaultCoverage(faultCount, successFCount, failedFCount, timeOutFCount);

printf("st\n");

free(gv);
/***************************************************************************************************/

/***************************************************************************************************/
ClearGat(Node,Tgat);  free(Node);                                      //Clear memeory for all members of Node
return 0;
}//end of main
/****************************************************************************************************************************/

