#include "input.h"
#include <string.h>
#include <stdio.h>
/***************************************************************************************************
Insert an element "X" at end of LIST "Cur", if "X" is not already in "Cur". 
If the "Cur" is Null, it creates a single element List using "X"
***************************************************************************************************/
void InsertEle(LIST **Cur,int X)
{
LIST *tl=NULL;
LIST *nl=NULL;

if ((tl=(LIST *) malloc(sizeof(LIST)))==NULL){ 
  printf("LIST: Out of memory\n");  
  exit(1);  }   
else{
  tl->Id=X;  tl->Next=NULL;
  if(*Cur==NULL){  
    *Cur=tl; 
    return;  }
  nl=*Cur; 
  while(nl!=NULL){
    if(nl->Id==X){       break;       }
    if(nl->Next==NULL){  nl->Next=tl; } 
    nl=nl->Next; }  
 }
return;
}//end of InsertEle
/***************************************************************************************************
Delete an element "X" from LIST "Cur",
***************************************************************************************************/	
void DeleteEle(LIST **Cur,int X)
{
LIST *fir = (*Cur);
LIST *lst = NULL;

if (fir==NULL) return;  
while((fir->Id!=X)&&(fir!= NULL)){
  lst=fir;
  fir=fir->Next; }   
if(fir!=NULL){
   if(lst==NULL){  (*Cur)=(*Cur)->Next;       }
   else{           lst->Next = fir->Next; }
   free(fir); }
else{ 
   return; }
return;
}//end of DeleteEle
/***************************************************************************************************************************
Return 1 if the element "x" is present in LIST "Cur"; Otherwise return  0  
*****************************************************************************************************************************/
int FindEle(LIST *Cur,int X)
{
LIST *temp=NULL;
	
temp=Cur;
while(temp!=NULL){
  if(temp->Id==X) return 1;
  temp=temp->Next; }
return 0;
}//end of FindEle
/***************************************************************************************************
Print the elements in LIST "Cur"          
***************************************************************************************************/
void PrintList(LIST *Cur)
{
LIST *tmp=Cur;

while(tmp!=NULL){   
  printf("%d  ", tmp->Id);
  tmp = tmp->Next; } 
return;
}//end of PrintList
/***************************************************************************************************
Count the total number  of elements in LIST "Cur"          
***************************************************************************************************/
int CountList(LIST *Cur)
{
LIST *tmp=Cur;
int size=0;

while(tmp!=NULL){   
  size++;
  tmp = tmp->Next; } 
return size;
}//end of PrintList
/***************************************************************************************************
Free all elements in  LIST "Cur"  
***************************************************************************************************/
void FreeList(LIST **Cur)
{
LIST *tmp=NULL;

if(*Cur==NULL){   return;  }
tmp=(*Cur);
while((*Cur) != NULL){
  tmp=tmp->Next;
  free(*Cur); 
  (*Cur)=tmp; }   
(*Cur)=NULL;
return;
}//end of FreeList 
/***************************************************************************************************
Initialize the paricular member of GATE structure
***************************************************************************************************/        
void InitiGat(GATE *Node,int Num)
{
Node[Num].Name=(char *) malloc(Mnam *sizeof(char));           //Dynamic memory allocation for an array
bzero(Node[Num].Name,Mnam);                                   //Clearing the string
Node[Num].Type=Node[Num].Nfi=Node[Num].Nfo=Node[Num].Mark=0;
Node[Num].Val=6;  
Node[Num].Fin=Node[Num].Fot=NULL;
//Node[Num].Rpath=Node[Num].Fpath=NULL;   
return;
}//end of InitiGat
/***************************************************************************************************
Print all contents(attribute) of all active member of GATE structure(DdNodes are not printed)
***************************************************************************************************/
void PrintGats(GATE *Node,int Tgat)
{
int i;

printf("tgat%d", Tgat);

printf("\nId\tName\tType\t#In\t#Out\tMark\tValue\t\tFanin\t\tFanout");
for(i=0;i<=Tgat;i++){
  if(Node[i].Type!=0){
    printf("\n%d\t%s\t%d\t%d\t%d\t%d\t%d\t\t",i,Node[i].Name,Node[i].Type,Node[i].Nfi,Node[i].Nfo,Node[i].Mark,Node[i].Val);
    PrintList(Node[i].Fin);  printf("\t\t");
    PrintList(Node[i].Fot); }}
return;
}//end of PrintGats
/***************************************************************************************************
Free the memory of all contents of all members of GATE structure(DdNodes are already cleared)
***************************************************************************************************/
void ClearGat(GATE *Node,int Tgat)
{
int i;

for(i=1;i<=Tgat;i++){ 
  free(Node[i].Name);
  Node[i].Type=Node[i].Nfi=Node[i].Nfo=Node[i].Mark=Node[i].Val=0;
  FreeList(&Node[i].Fin);  FreeList(&Node[i].Fot); }
return;
}//end of ClearGat
/***************************************************************************************************
Count the Total Number of Primary inputs and outputs
***************************************************************************************************/
void CountPri(GATE *Node,int Tgat,int *Npi,int *Npo)
{
int i,j,k;

i=j=k=0;
for(i=1;i<=Tgat;i++){ 
  if(Node[i].Type!=0){
    if(Node[i].Nfi==0){      j++; }
    if(Node[i].Nfo==0){      k++; } }}
*Npi=j; *Npo=k;
return;
}//end of CountPri
/***************************************************************************************************
Convert (char *) type read to (int)     
***************************************************************************************************/
int AssignType(char *Gtyp)
{
if      ((strcmp(Gtyp,"inpt")==0) || (strcmp(Gtyp,"INPT")==0))       return 1;
else if ((strcmp(Gtyp,"from")==0) || (strcmp(Gtyp,"FROM")==0))       return 2;
else if ((strcmp(Gtyp,"buff")==0) || (strcmp(Gtyp,"BUFF")==0))       return 3;
else if ((strcmp(Gtyp,"not")==0)  || (strcmp(Gtyp,"NOT")==0))        return 4;
else if ((strcmp(Gtyp,"and")==0)  || (strcmp(Gtyp,"AND")==0))        return 5;
else if ((strcmp(Gtyp,"nand")==0) || (strcmp(Gtyp,"NAND")==0))       return 6;
else if ((strcmp(Gtyp,"or")==0)   || (strcmp(Gtyp,"OR")==0))         return 7;
else if ((strcmp(Gtyp,"nor")==0)  || (strcmp(Gtyp,"NOR")==0))        return 8;
else if ((strcmp(Gtyp,"xor")==0)  || (strcmp(Gtyp,"XOR")==0))        return 9;
else if ((strcmp(Gtyp,"xnor")==0) || (strcmp(Gtyp,"XNOR")==0))       return 10;
else                          			                     return 0;
}//end of AssignType
/***************************************************************************************************************************
Return id if the element "x" is present in LIST "Cur"; Otherwise return  0  
*****************************************************************************************************************************/
int FindGatByName(GATE *Node,int currentTgat, char* name)
{
int i;

for(i=1;i<=currentTgat;i++){ 
	if(!Node[i].Name){
		continue;
	}

  if(strcmp(Node[i].Name , name) == 0) return i;
}

return 0;
}//end of FindEle
/***************************************************************************************************

/***************************************************************************************************
 Function to read the Bench Mark(.isc files)
***************************************************************************************************/
int ReadIsc(FILE *Isc,GATE *Node)
{

//INSERT CODE HERE
int lineLen = 1000; // max length of a line
char line[lineLen];
char token[8] = "";
int tot = 0;
int type;
int tokenIndex = 1;
int gateId = 0;
int isInitGateNeeded = 1;
int fanInId = 0;
int isLastTokenLine = 0;
int isEndLine = 0;
int isProcessing = 0;

while (fgets(line, sizeof(line), Isc)) {

	char* start = line;
	char* end;
	if(line[0] == '*'){
		continue;
	}
	
	tokenIndex = 1;
	isEndLine = 0;

	while ((end = strchr( start, ' ' )) != NULL || start != NULL){
		isLastTokenLine = 0;
		isProcessing = 0;
		if(end != NULL){
			if(end - start > 0 ){
				isProcessing = 1;
				
				sprintf(token,"%.*s\n", end - start, start);
				token[strcspn(token, "\n")] = 0;  // remove unwanted /n spaces				
			}
		}else{
			isProcessing = 1;
			sprintf(token,"%.*s\n", &line[lineLen] - start, start);
			token[strcspn(token, "\n")] = 0;
			isLastTokenLine = 1;
		}

		if(isProcessing){
			
			switch(tokenIndex){
					case 1:	//id
						if(isInitGateNeeded == 0){
							tokenIndex = 10;							
							
						}else{
						  	gateId = atoi(token);
							InitiGat(Node, gateId);
							if(gateId > tot){
								tot = gateId;
							}
							break;
						}
					case 10: //fanInId
						fanInId = atoi(token);
						InsertEle(&(Node[gateId].Fin), fanInId);

						//add fanOut
						InsertEle(&(Node[fanInId].Fot), gateId);
						if(isEndLine == 0){
							tokenIndex --;
						}else{
							tokenIndex = 0;
						}
						
						isInitGateNeeded = 1;
						break;  	                               
					case 2:	//gatename
					  	strcpy(Node[gateId].Name , token);
						break;

					case 3:	//GateType 
						type = AssignType(token);
					  	Node[gateId].Type = type ;

					  	break;
					case 4: //Nfo or Fin 
							
						if(Node[gateId].Type == FROM){
						  //find fanInId using name
						  	fanInId =  FindGatByName(Node,tot,token);
						  	InsertEle(&(Node[gateId].Fin), fanInId);
							InsertEle(&(Node[fanInId].Fot), gateId);

							Node[gateId ].Nfo =  1;
							Node[gateId ].Nfi =  1;
							
						
						}else{
							Node[gateId ].Nfo = atoi(token);
						}

						break;
			
					case 5://Nfi 
						if(Node[gateId].Type != FROM){  
							Node[gateId].Nfi = atoi(token);
						}
						if(Node[gateId].Type != FROM && Node[gateId].Type != INPT ){
							isInitGateNeeded = 0;
						}                                 
						break;
					
				}
				tokenIndex++;
		}

        start = end + 1;
		if(isLastTokenLine){
			isEndLine = 1;
			break;
		}
    }
}

//Return the Maximum node of the Isc file

return tot;
}//end of ReadIsc 
/****************************************************************************************************************************/

