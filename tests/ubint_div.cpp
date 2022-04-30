#include <iostream>
#include <vector>

#ifndef _MAKE_LIB
#include "../ubint.hpp"
#else
#include <ubint.hpp>
#endif

#include "mini-test.hpp"

int main() { START_TEST;

        // test variables
        apa::ubint ONE = 1, ZERO = 0;

        apa::ubint NUM1(
            "0x19cf0546f6a3fc1e93d8dbda5ea2889551cb7248d21125fbf60f3c622a4ab01456703c3"
            "39c96f18bed4ce4fa268983f97dcb83ae847f8ecf19f81870578c41ede22ccf76553d1c38"
            "deb692820ac53f361c2a2e0b0dc6e6b77810b15d656775b37fc2afb2200632008419c0555"
            "ccffd2f9377262ef48b6a096629b8af1048f07dc1a8152eebd3d209041dfccb9df9e5ef94"
            "f4b55bae8fb825d7dcfb68e89e0ab027f27bbd2d0b226a82e0fbcc85eeb8b1df2c6fd6ee3"
            "8e4b43e78649af542686ba5a385d34ed7c51c6b77c5ab1ff868deca9b0158edc44de55c44"
        );

        apa::ubint NUM2(
            "0xa4027a170efee729e543877bc6d8cb88c00810d0b3f18c77f9755907f470a1a94f4ab26"
            "825e763265c216490f5a93c0426485cc2bfc99ed1fd883aa983759e4662f26cdd96cf3289"
        );

        apa::ubint NUM3 = 2;

        apa::ubint NUM1_DIV_NUM2(
            "0x2848ca2401c26ea27bfc2689ff8ce447b6092adcab3c610c5d646521e7a4358f0aa93a39"
            "d8ec904a02856cf99e4456452e00a219a64b823b59edadaa23841fa573d848bc586e2840ca"
            "c74808ed76bff1a3168be007b1f2270efbbb42156794955629291df55af4ae1f3933e5dc77"
            "9f5c0b38f66e4a48da1d91a4461c682f0bdd4d688f6b70b338b7895a4ce0fb3236c7be0e"
        );

        apa::ubint NUM1_DIV_NUM3(
            "0xce782a37b51fe0f49ec6ded2f51444aa8e5b924690892fdfb079e311525580a2b381e19ce"
            "4b78c5f6a6727d1344c1fcbee5c1d7423fc7678cfc0c382bc620f6f11667bb2a9e8e1c6f5b4"
            "94105629f9b0e15170586e3735bbc0858aeb2b3bad9bfe157d910031900420ce02aae67fe97"
            "c9bb93177a45b504b314dc578824783ee0d40a9775e9e904820efe65cefcf2f7ca7a5aadd74"
            "7dc12ebee7db4744f055813f93dde9685913541707de642f75c58ef9637eb771c725a1f3c32"
            "4d7aa13435d2d1c2e9a76be28e35bbe2d58ffc346f654d80ac76e226f2ae22"  
        );

        apa::ubint NUM2_DIV_NUM3(
            "0x52013d0b877f7394f2a1c3bde36c65c46004086859f8c63bfcbaac83fa3850d4a7a559341"
            "2f3b1932e10b2487ad49e0213242e615fe4cf68fec41d54c1bacf233179366ecb679944"
        );

        apa::ubint NUM1_MOD_NUM2(
            "0x79b744150065c5ae9c0a197c73841d9a27735226bdd3e6177c1956e524095dad2dd6f9799"
            "fb95a944dc7b49a9bc204b1b81eb48bd25ecf6d7eef5f8afdcfc44cce56623d188feac6"
        );

        apa::ubint NUM1_MOD_NUM3 = 0;

        apa::ubint NUM2_MOD_NUM3 = 1;

        apa::ubint
            ANSQ1 = NUM1.bit_division(NUM2),
            ANSQ2 = NUM1.bit_division(NUM3),
            ANSQ3 = NUM2.bit_division(NUM3),
            ANSR1 = NUM1.bit_modulo(NUM2),
            ANSR2 = NUM1.bit_modulo(NUM3),
            ANSR3 = NUM2.bit_modulo(NUM3);
            
        ASSERT_EQUALITY(NUM1.bit_division(NUM1),ONE,"NUM1.bit_division(NUM1)");
        ASSERT_EQUALITY(NUM2.bit_division(NUM2),ONE,"NUM2.bit_division(NUM2)");
        ASSERT_EQUALITY(NUM3.bit_division(NUM3),ONE,"NUM3.bit_division(NUM3)");
        ASSERT_EQUALITY(ANSQ1,NUM1_DIV_NUM2,"NUM1.bit_division(NUM2)");
        ASSERT_EQUALITY(ANSQ2,NUM1_DIV_NUM3,"NUM1.bit_division(NUM3)");
        ASSERT_EQUALITY(ANSQ3,NUM2_DIV_NUM3,"NUM2.bit_division(NUM3)");

        ASSERT_EQUALITY(NUM1.bit_modulo(NUM1),ZERO,"NUM1.bit_modulo(NUM1)");
        ASSERT_EQUALITY(NUM2.bit_modulo(NUM2),ZERO,"NUM2.bit_modulo(NUM2)");
        ASSERT_EQUALITY(NUM3.bit_modulo(NUM3),ZERO,"NUM3.bit_modulo(NUM13)");
        ASSERT_EQUALITY(ANSR1,NUM1_MOD_NUM2,"NUM1.bit_modulo(NUM2)");
        ASSERT_EQUALITY(ANSR2,NUM1_MOD_NUM3,"NUM1.bit_modulo(NUM3)");
        ASSERT_EQUALITY(ANSR3,NUM2_MOD_NUM3,"NUM2.bit_modulo(NUM3)");

    #if defined(_BASE2_16)
        RESULT("UBINT BASE 2^16 DIVISION");
    #elif defined(_BASE2_32)
        RESULT("UBINT BASE 2^32 DIVISION");
    #elif defined(_BASE2_64)
        RESULT("UBINT BASE 2^64 DIVISION");
    #endif
}