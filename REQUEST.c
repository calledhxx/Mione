//
// Created by calle on 25-1-3.
//

#include "OBJECTS.h"
#include "ERR.h"
#include <stdio.h>
#include <stdlib.h>

VariableRequestUPObj REQUEST(MioneObj* PACK,int PACKSize){
  VariableRequestUPObj VarReq;
  VarReq.VariableUPs = malloc(0);
  VarReq.VariablesSize = 0;

  for (int PACKIndex = 0; PACKIndex < PACKSize;PACKIndex++) {

    switch (PACK[PACKIndex].ObjType) {
      case SYMBOL:
        switch (PACK[PACKIndex].Symbol.CurNumber){
          case 2:
            break;
          default:
            ErrCall("M123","Unkown Symbol","",PACK[PACKIndex].Line,PACK[PACKIndex].Column);
        }
        break;
      case VARIABLE:

       

        VarReq.VariablesSize++;
        VarReq.VariableUPs = realloc(VarReq.VariableUPs,VarReq.VariablesSize*sizeof(VariableObj*));
        VarReq.VariableUPs[VarReq.VariablesSize-1] = PACK[PACKIndex].VarUP;



        break;
      default:
        ErrCall("M12345","Object Type Error",NULL,PACK[PACKIndex].Line,PACK[PACKIndex].Column);

    }
  }

  return VarReq;
}