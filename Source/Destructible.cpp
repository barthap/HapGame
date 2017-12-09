/******************************************************************************/
#include "stdafx.h"
#include "Main.h"
/******************************************************************************/
Bool DestructibleEC::update()
{
	switch(mode)
    {
      case PIECE:
      {
         if(!destruct_mesh || !InRange(piece_index, destruct_mesh->parts())) return false;
      } break;
    }

    return true;
}
/******************************************************************************/