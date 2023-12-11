///////////////////////////////////////////////////////////////////////////////////////////////////
// if constexpr branch removal
///////////////////////////////////////////////////////////////////////////////////////////////////

#include <cstddef>

template <std::size_t N>
bool branch_removal() {
    if constexpr (N > 10) {
        return true;
    } else {
        return false;
    }
}

//    branch_removal<11>();
//    branch_removal<9>();
// resolves to:

template<>
bool branch_removal<11>()
{
  if constexpr (true) {
    return true;
  }
}

template<>
bool branch_removal<9>()
{
  if constexpr (false) {
  } else /* constexpr */ {
    return false;
  }
}

///////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////
// likely/unlikely
///////////////////////////////////////////////////////////////////////////////////////////////////

enum class YesNo {
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

///////////////////////////////////////////////////////////////////////////////////////////////////
// do {} while (condition)
//
// If we know that we are always going to run the first iteration of a loop, opting for a do while
// can provide minimal improvement. This is very much a micro optimisation and is likely to make
// little difference even if the conditional statement for looping is complex. It's worth stating
// that for known sizes the compiler can optimise this away.
///////////////////////////////////////////////////////////////////////////////////////////////////

void branch_while_1(std::vector<int> &in) {
    int i = 0;
    while (i < in.size()) {
        in[i] += 1;
        i++;
    }
}

void branch_while_2(std::vector<int> &in) {
    int i = 0;
    do {
        in[i] += 1;
        i++;
    } while (i < in.size());
}

// example_1(std::vector<int, std::allocator<int> >&):      //
//         mov     rcx, QWORD PTR [rdi]                     //
//         cmp     QWORD PTR [rdi+8], rcx                   //
//         je      .L1                                      // while loop has an initial check which can jump to return
//         mov     edx, 0                                   //
// .L3:                                                     //
//         add     DWORD PTR [rcx+rdx*4], 1                 //
//         mov     rcx, QWORD PTR [rdi]                     //
//         add     rdx, 1                                   //
//         mov     rax, QWORD PTR [rdi+8]                   //
//         sub     rax, rcx                                 //
//         sar     rax, 2                                   //
//         cmp     rdx, rax                                 //
//         jb      .L3                                      //
// .L1:                                                     //
//         ret                                              //
// example_2(std::vector<int, std::allocator<int> >&):      //
//         mov     edx, 0                                   //
// .L6:                                                     //
//         mov     rax, QWORD PTR [rdi]                     // do while loop goes straight to work
//         add     DWORD PTR [rax+rdx*4], 1                 //
//         add     rdx, 1                                   //
//         mov     rax, QWORD PTR [rdi+8]                   //
//         sub     rax, QWORD PTR [rdi]                     //
//         sar     rax, 2                                   //
//         cmp     rdx, rax                                 //
//         jb      .L6                                      // only jump is within the while to loop on the condition
//         ret                                              //

///////////////////////////////////////////////////////////////////////////////////////////////////
