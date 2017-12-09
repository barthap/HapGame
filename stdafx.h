/******************************************************************************/
#include <EsenthelEngine/EsenthelEngine.h>
/******************************************************************************/
//wszystkie definy do luabind
#undef T
#undef C
#undef T2
#undef T1
#include "Luabind/luabind.hpp"
using namespace luabind;
#define T1(a    )   template<typename a                        > // 1 type  template
#define T2(a,b  )   template<typename a, typename b            > // 2 types template
#define T        (*this)
#define C        const