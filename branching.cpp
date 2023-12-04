


///////////////////////////////////////////////////////////////////////////////////////////////////
// likely/unlikely
///////////////////////////////////////////////////////////////////////////////////////////////////

enum class YesNo : bool {
    Yes,
    No
};

static int count = 0;

bool branch_ex_1(YesNo yesno) {
    if (yesno == YesNo::Yes) [[likely]] {
        ++count;
        return true;
    }
    return false;
}

bool branch_ex_2(YesNo yesno, int a) {
    if (yesno == YesNo::Yes) [[unlikely]] {
        ++count;
        return true;
    }
    return false;
}

// branch_ex_1(YesNo):                                      // bool branch_ex_1(YesNo yesno)
//         test    dil, dil                                 //
//         jne     .L3                                      // preference for "likely" path
//         add     DWORD PTR _ZL5count[rip], 1              //
//         mov     eax, 1                                   //
//         ret                                              //
// .L3:                                                     //
//         xor     eax, eax                                 //
//         ret                                              //
// branch_ex_2(YesNo, int):                                 // bool branch_ex_2(YesNo yesno)
//         xor     eax, eax                                 //
//         test    dil, dil                                 //
//         je      .L8                                      // preference for "unlikely" path
//         ret                                              //
// .L8:                                                     //
//         add     DWORD PTR _ZL5count[rip], 1              //
//         mov     eax, 1                                   //
//         ret                                              //

///////////////////////////////////////////////////////////////////////////////////////////////////

