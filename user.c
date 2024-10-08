#include "input.h"

/***************************************************************************************************************************
Lookup tables for gates
****************************************************************************************************************************/

int andGate[5][5] = {
//o   1 xv  d  db	
  {0, 0, 0, 0, 0},
  {0, 1, 2, 3, 4},
  {0, 2, 2, 2, 2},
  {0, 3, 2, 3, 0},
  {0, 4, 2, 0, 4}
};

int orGate[5][5] = {
//o   1 xv  d  db	
  {0, 1, 2, 3, 4},
  {1, 1, 1, 1, 1},
  {2, 1, 2, 2, 2},
  {3, 1, 2, 3, 1},
  {4, 1, 2, 1, 4}
};

int notGate[5] = {1, 0 , 2, 4, 3};
int nonControllingValues[11] = {
	-1, //invalid
	-1, //input
	-1, //from
	-1, //buff
	1, //Not
	1, //and
	1, //Nand
	0, //Or
	0, //Nor
	1, //XOR
	1  //XNOR
};
int lineNumber = 0;
LIST* dFrontier = NULL;
int state = NEUTRAL;
int tGat = 0;
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
 Function to simulate logic Impl
***************************************************************************************************/
 int logicSimulateImpl(GATE * Node, GV gv, GV * fault){

	int mask = 0;
	//if(dFrontier != NULL){
	FreeList(&dFrontier); //release the dfrontier list
	//}
	int state = NEUTRAL;
	int itr ;

	Node[gv.g].Val = gv.v ; // assign PI

	for(itr = 0; itr <= tGat; itr++){ 
		if(Node[itr].Type != 0){  //not an invalid gate
			switch(Node[itr].Type){
				case 1: // type input
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

			if(itr == fault->g){ // check whether ID is fault gate //fault activation
				if(Node[itr].Val == 0 && fault->v == 1 ){
					Node[itr].Val = D;
				}else if(Node[itr].Val == 1 && fault->v == 0 ){
					Node[itr].Val = DB;
				}

				if((Node[itr].Val == 0 && fault->v == 0) || (Node[itr].Val == 1 && fault->v == 1) ){ //fault masking
					mask = 1;
				}
			}

			//creating dfrontier
			if(checkDFrontier(itr, Node)){
				InsertEle(&(dFrontier), itr);//Add to dfrontier
			}

			if(Node[itr].Nfo == 0){   //find Fault Effect of PO
				if(Node[itr].Val == D || Node[itr].Val == DB ){ 
					state =  SUCCESS;
					return state;
				}
			}	
		}

	}
	if(mask){ //fault masking
		state = FAILURE;
	}
	if(dFrontier == NULL && (Node[fault->g].Val == D || Node[fault->g].Val == DB )){ // check dfrontier is empty && fault is activated
		state = FAILURE;
	}	
	return state;
 }

//end of simulateLogicImpl
/****************************************************************************************************************************/

/***************************************************************************************************
 Function to podem
***************************************************************************************************/
int podem(GATE * Node, GV* fault, int tGatNum){
	tGat = tGatNum;

	setDontcares(Node);
	int start = clock();
	int result = podemRecursion(Node, fault, start);
	return result;
}
//end of podem
/****************************************************************************************************************************/
/***************************************************************************************************
 Function to podemRecursion
***************************************************************************************************/
int podemRecursion(GATE * Node, GV *fault, clock_t clockStart){

	int mask = 0;
	int result;
	clock_t end = clock();
	double duration = (double)(end - clockStart)/CLOCKS_PER_SEC;
	if(duration> 0.2){
		state = TIMEOUT;
		return state;
	}
	GV pi;
	GV obj; 
	
	obj = getObjective(Node, fault, obj);
	if(obj.g == 0){
		state = FAILURE;
		return state;
	} 	

	
	pi = backtrace(Node, obj); 
	if(pi.g == 0){
		state = FAILURE;
		return state;
	} 	
	state = logicSimulateImpl(Node,pi,fault);
	
	if(state == SUCCESS){
		return state;
	}

	if(state != FAILURE){
		result = podemRecursion(Node, fault, clockStart);		
		if(result == SUCCESS){
			state = SUCCESS;			
			return state;
		}else if(result == TIMEOUT){
			state = TIMEOUT;
			return state;
		}
	}
		
	pi.v = !(pi.v) ;
	state = logicSimulateImpl(Node, pi, fault);
	

	if(state == SUCCESS){
		return state;
	}
	if(state != FAILURE){
		result = podemRecursion(Node, fault, clockStart);
		if(result == SUCCESS){
			state = SUCCESS;
			return state;
		}else if(result == TIMEOUT){
			state = TIMEOUT;
			return state;
		}
	}
		
	//reset PI - BAD decision made ealier
	pi.v = XV;
	state = logicSimulateImpl(Node, pi, fault);
	if(state == SUCCESS){
		return state;
	}else if(state == FAILURE){
		return state;
	}
	state = FAILURE;

	return state;
	
}
//end of podemRecursion
/****************************************************************************************************************************/

/***************************************************************************************************
 Function to getObjective
***************************************************************************************************/
GV getObjective(GATE * Node, GV *fault, GV obj){ //fault GV

	if(Node[fault->g].Val != D && Node[fault->g].Val != DB ){ // if fault is not exicted
		obj.v = !(fault->v);
		obj.g = fault->g;
		return obj;
	}
	if(dFrontier == NULL){
		obj.g = -1;
		obj.v = -1;
		return obj;
	}
	int d = dFrontier->Id;//gate in d frontier
	obj.g = findXFanIn(Node, d);
	obj.v = findNonControlVal(Node, d);//find non controlling value

	return obj;
	
}
//end of getObjective
/****************************************************************************************************************************/

/***************************************************************************************************
 Function to backtrace
***************************************************************************************************/
GV backtrace(GATE * Node, GV gv){
	int g = gv.g;
	int v = gv.v;
	int numInvertions = 0;
	int i = g;

	if(Node[i].Type == NOR || Node[i].Type == NOT || Node[i].Type == NAND){// inverting type gate
		numInvertions ++;
	}

	while(Node[i].Nfi !=0){
		i = findXFanIn(Node, i);
		if(Node[i].Type == NOR || Node[i].Type == NOT || Node[i].Type == NAND){// inverting type gate
			numInvertions ++;
		}
	}
	if(numInvertions % 2 == 1){ // inverting gates count is odd
		v = !v;
	}

	gv.g= i;
	gv.v = v;
	return gv;
}
//end of backtrace
/****************************************************************************************************************************/

/***************************************************************************************************
 Function to findXFanIn
***************************************************************************************************/
int findXFanIn(GATE *Node, int i){
	LIST *tmp = Node[i].Fin;
	i = 0;
	while(tmp!=NULL){   
		if(Node[tmp->Id].Val == XV ){ // check value is x 
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
	for(i=1;i<=tGat;i++){ 
		Node[i].Val = XV;			
	}
}
//end of findXFanIn
/****************************************************************************************************************************/

/***************************************************************************************************
 Function to findNonControlVal
***************************************************************************************************/
int findNonControlVal(GATE *Node, int gId){
	return nonControllingValues[Node[gId].Type];
}
//end of findNonControlVal
/****************************************************************************************************************************/

/***************************************************************************************************
 Function to check DFrontier
***************************************************************************************************/
int checkDFrontier(int id, GATE *Node){
	if(Node[id].Val != XV){ // if output is not x 
		return 0;
	}
	LIST *Cur = Node[id].Fin;
	LIST *temp=NULL;

	temp=Cur;
	while(temp!=NULL){
	if(Node[temp->Id].Val==D || Node[temp->Id].Val==DB){
		return 1;
	}
	temp=temp->Next; }
	return 0;
}
//end of checkDFrontier
/****************************************************************************************************************************/
/***************************************************************************************************
 Function to print PI
***************************************************************************************************/
void printPI(GATE* Node){
	int i;
	for(i=1;i<=tGat;i++){ 
		if(Node[i].Type !=0 && Node[i].Nfi == 0){
			printf("%d", Node[i].Val);
		}			
	}
	printf("\n");
}

//end of printPI
/****************************************************************************************************************************/

/***************************************************************************************************
 Function to check fault coverage
***************************************************************************************************/
void checkFaultCoverage(FILE* res, int totalf, int sf, int ff, int tf){
	printf("Total Fault count: %d\n", totalf);
	printf("Total Sucess Fault count: %d\n", sf);
	printf("Total Failed Fault count: %d\n", ff);
	printf("Total Time Out Fault count: %d\n", tf);
	printf("Total Sucess Fault coverage: %.2f%% \n", (double)sf*100/totalf );

	fprintf(res, "Total Failed Fault coverage: %.2f%% \n", (double)ff*100/totalf);
	fprintf(res, "Total Time Out Fault coverage: %.2f%% \n", (double)tf*100/totalf);
	fprintf(res, "Total Sucess Fault coverage: %.2f%% \n", (double)sf*100/totalf);
}

//end of checkFaultCoverage
/****************************************************************************************************************************/
