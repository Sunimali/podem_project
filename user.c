#include "input.h"

/***************************************************************************************************************************
Lookup tables for gates
****************************************************************************************************************************/
int andGate[5][5] = {
  {0, 0, 0, 0, 0},
  {0, 1, 2, 3, 4},
  {0, 2, 2, 2, 2},
  {0, 3, 2, 3, 0},
  {0, 4, 2, 0, 4}
};

int orGate[5][5] = {
  {0, 1, 2, 3, 4},
  {1, 1, 1, 1, 1},
  {2, 1, 2, 2, 2},
  {3, 1, 2, 3, 1},
  {4, 1, 2, 1, 4},
  
};

int notGate[5] = {1, 0 , 2, 4, 3};
int lineNumber = 0;
LIST* dFrontier = NULL;
/****************************************************************************************************************************/

/***************************************************************************************************/
/***************************************************************************************************
 Function to read the test patterns(.pattern files)
***************************************************************************************************/
int ReadPattern(FILE * pattern, int patternList[Mpt][Mpi], int nPi){

	char line[Mlin];
	char input[5] = "";

	while (fgets(line, sizeof(line), pattern) && lineNumber < Mpt) {
		int index = 0;

		while(index != nPi){
			sprintf(input, "%.*s\n", 1 , &line[index]);
			//printf("test%d ", atoi(input));
			patternList[lineNumber][index] = atoi(input);
			index ++;
		}
		printf("\n");
		lineNumber++;
	}
}


void printPatternList(int patternList[Mpt][Mpi], int nPi){
	int patternNum;
	for(patternNum = 0; patternNum < lineNumber; patternNum++ ){
		int inputNum;
		for(inputNum = 0; inputNum < nPi; inputNum++ ){
			printf("%d", patternList[patternNum][inputNum]);
		}
		printf("\n");
	}
}

//end of ReadPattern 
/****************************************************************************************************************************/

/***************************************************************************************************
 Function to simulate logic
***************************************************************************************************/
 void simulateLogic(GATE * Node, int patternList[Mpt][Mpi], int tGat, FILE * res){
	int testNum;
	//iterating through test cases
	for(testNum = 0; testNum < lineNumber; testNum++){
		int itr ;
		int curPatternIndex = 0;
		for(itr = 0; itr <= tGat; itr++){ 
			if(Node[itr].Type != 0){  //not an invalid gate
				switch(Node[itr].Type){
					case 1: // type input
						Node[itr].Val = patternList[testNum][curPatternIndex];
						curPatternIndex ++;
						break;
					case 2: // type  from
						Node[itr].Val = Node[Node[itr].Fin->Id].Val; 
						break;
					case 3: // type  buff
						Node[itr].Val = Node[Node[itr].Fin->Id].Val;
						break;
					case 4: // type not
						Node[itr].Val = notGate[Node[Node[itr].Fin->Id].Val];
						break;
					case 5: //type and
						Node[itr].Val = andOperation(Node, (Node[itr].Fin));
						break;
					case 6: //type Nand
						Node[itr].Val = notGate[andOperation(Node, (Node[itr].Fin))];
						break;
					case 7: // type or
						Node[itr].Val = orOperation(Node, (Node[itr].Fin));
						break;
					case 8: // type Nor
						Node[itr].Val = notGate[orOperation(Node, (Node[itr].Fin))];
						break;
					case 9: // type xor
						Node[itr].Val = xorOperation(Node, (Node[itr].Fin));
						break;
					case 10: // type xNor
						Node[itr].Val = notGate[xorOperation(Node, (Node[itr].Fin))];
						break;
				}
				if(Node[itr].Nfo == 0){
					fprintf(res, "%d" , Node[itr].Val);
				}
			}

		}
		fprintf(res, "\n");
	}
 }

//end of simulateLogic
/****************************************************************************************************************************/


/***************************************************************************************************
 Function to andOperation
***************************************************************************************************/
int andOperation(GATE * Node, LIST *Cur){
	LIST *tmp=Cur;
	int output=1;

	while(tmp!=NULL){   
		output = andGate[output][Node[tmp->Id].Val]; // check output by two by two
		if(output == 0){ // if one pair give out as 0 then stop
			break;
		}
		tmp = tmp->Next; 
	} 
	return output;
}
//end of andOperation
/****************************************************************************************************************************/

/***************************************************************************************************
 Function to orOperation
***************************************************************************************************/
int orOperation(GATE * Node, LIST *Cur){
	LIST *tmp=Cur;
	int output=0;


	while(tmp!=NULL){   
		output = orGate[output][Node[tmp->Id].Val]; // check output by two by two
		if(output == 1){ // if one pair give out as 1 then stop
			break;
		}
		tmp = tmp->Next; 
	} 
	return output;
}
//end of orOperation
/****************************************************************************************************************************/


/***************************************************************************************************
 Function to xorOperation
***************************************************************************************************/
int xorOperation(GATE * Node, LIST *Cur){
	LIST *tmp=Cur;
	int output=1;
 
	if (Node[tmp->Next->Id].Val == Node[tmp->Id].Val){ // check input value is equal or not
		output = 0;
	}
		
	return output;
}
//end of xorOperation
/****************************************************************************************************************************/

/***************************************************************************************************
 Function to podem
***************************************************************************************************/
int podem(GATE * Node){
	setDontcares(Node);

	int result = podemRecursion(Node);

	if(result== SUCCESS){
		//PRINT VALUES AT PI
	}else{
		//print fault f in untestable
	}

}
//end of podem
/****************************************************************************************************************************/
/***************************************************************************************************
 Function to podemRecursion
***************************************************************************************************/
int podemRecursion(GATE * Node, GV *gv){

}
//end of podemRecursion
/****************************************************************************************************************************/

/***************************************************************************************************
 Function to getObjective
***************************************************************************************************/
*GV getObjective(GATE * Node, GV *gv){
//todo
	int d = //gate in d frontier
	gv->g = findXFanIn(Node, d);
	gv->v = !(Node[d]->Val);

	return gv;

	
}
//end of getObjective
/****************************************************************************************************************************/

/***************************************************************************************************
 Function to backtrace
***************************************************************************************************/
*GV backtrace(GATE * Node, int g, int v, GV *gv){
	int numInvertions = 0;
	int i = g;

	while(Node[i].Nfi !=0){
		i = findXFanIn(Node, i);
		if(Node[i].Type == NOR || Node[i].Type == NOT || Node[i].Type == NAND){// inverting type gate
			numInvertions ++;
		}
	}
	if(numInvertions % 2 == 1){ // inverting gates count is odd
		v = !v;
	}

	gv->g= i;
	gv->v = v;
setDontcares
}
//end of backtrace
/****************************************************************************************************************************/

/***************************************************************************************************
 Function to findXPath
***************************************************************************************************/
int findXPath(GATE * Node, LIST *Cur){
	
}
//end of findXPath
/****************************************************************************************************************************/

/***************************************************************************************************
 Function to findXFanIn
***************************************************************************************************/
int findXFanIn(GATE *Node, int i){
	LIST *tmp = Node[i]->Fin;
	i = 0;
	while(tmp!=NULL){   
		if([Node[tmp->Id].Val == X ){ // checl value is x 
			i = tmp->Id;
			break;
		}
		tmp = tmp->Next; 
	} 
	return i;
}
//end of findXFanIn
/****************************************************************************************************************************/

/***************************************************************************************************
 Function to setDontcares
***************************************************************************************************/
void setDontcares(GATE *Node){
	int i;
	for(i=1;i<=Tgat;i++){ 
		Node[i].Val = X;			
	}
}
//end of findXFanIn
/****************************************************************************************************************************/