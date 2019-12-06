#ifndef KEYBOARD_STATE_H
#define KEYBOARD_STATE_H

#include "PeripheralState.h"
#include <vector>
#include "KeyBindings/InputKey.h"

using namespace std;
using namespace Framework::Input::KeyBindings;


namespace Framework {
namespace Input {

	

	class KeyboardState : public PeripheralState<KeyboardState> {

		vector<pair<InputKey, int>> presses;

		vector<InputKey> ups;

	public:

		int AddPress(pair<InputKey, int> press);

		int AddUp(InputKey up);

		vector<pair<InputKey, int>>* GetPresses();

		vector<InputKey>* GetUps();

		virtual KeyboardState* Clone();

		virtual bool Contain(InputKey key);

		virtual bool CheckIsEmpty();

	};


	enum class Key {
		// major2
        C2,             // 0
        D2b,            //
        D2,             //
        E2b,            //
        E2,             //
        F2,             //
        G2b,            //
        G2,             //
        A2b,            //
        A2,             //
        B2b,            //
        B2,             //
        // major1
        C1,             // 12
        D1b,            //
        D1,             //
        E1b,            //
        E1,             //
        F1,             //
        G1b,            //
        G1,             //
        A1b,            //
        A1,             //
        B1b,            //
        B1,             //
        // major
        C,              // 24
        Db,             //
        D,              //
        Eb,             //
        E,              //
        F,              //
        Gb,             //
        G,              //
        Ab,             //
        A,              //
        Bb,             //
        B,              //
        // minor
        c,              // 36
        db,             //
        d,              //
        eb,             //
        e,              //
        f,              //
        gb,             //
        g,              //
        ab,             //
        a,              //
        bb,             //
        b,              //
        // minor1
        c1,             // 48
        d1b,            //
        d1,             //
        e1b,            //
        e1,             //
        f1,             //
        g1b,            //
        g1,             // 55
        a1b,            //
        a1,             // 57
        b1b,            //
        b1,             //
        // minor2
        c2,             // 60
        d2b,            //
        d2,             //
        e2b,            //
        e2,             // 64
        f2,             //
        g2b,            //
        g2,             // 67
        a2b,            //
        a2,             // 69
        b2b,            //
        b2,             // 71
        // minor3
        c3,             // 72
        d3b,            //
        d3,             // 74
        e3b,            //
        e3,             // 76
        f3,             // 77
        g3b,            //
        g3,             //
        a3b,            // 80
        a3,             //
        b3b,            //
        b3,             // 83
        // minor4
        c4,             // 84
        d4b,            //
        d4,             //
        e4b,            //
        e4,             //
        f4,             //
        g4b,            //
        g4,             //
        a4b,            //
        a4,             //
        b4b,            //
        b4,             //
        // minor5
        c5              // 96
	};

}}





#endif