
#ifndef __TestStateMachine02_H__
#define __TestStateMachine02_H__

#include "mixr/base/StateMachine.hpp"

//------------------------------------------------------------------------------
// Class: TestStateMachine02
// Description: State Machine Test #2 --
//
//    Same as Test #1 except that we're using separate state specific functions.
//
//------------------------------------------------------------------------------
class TestStateMachine02 final: public mixr::base::StateMachine
{
   DECLARE_SUBCLASS(TestStateMachine02, mixr::base::StateMachine)

public:
   enum { CALL_01 = 11, CALL_02 = 22 };

public:
   TestStateMachine02();

protected:
   void preStateProc(const double dt) final;
   void postStateProc(const double dt) final;
   unsigned short stateTable(
         const unsigned short cstate,
         const StateTableCode code,
         const double dt=0
      ) final;

private:
   // State specific functions
   void stateFunc00(const double dt);
   void stateFunc01(const double dt);
   void stateFunc02(const double dt);
   void stateFunc03(const double dt);
   void stateFunc04(const double dt);
   void stateFunc05(const double dt);
   void stateFunc11(const double dt);
   void stateFunc12(const double dt);
   void stateFunc13(const double dt);
   void stateFunc14(const double dt);
   void stateFunc15(const double dt);
   void stateFunc21(const double dt);
   void stateFunc22(const double dt);
   void stateFunc23(const double dt);
   void stateFunc99(const double dt);
};

#endif
