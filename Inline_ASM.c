#include <stdio.h>
#include <stdlib.h>
#define N 100

int main()
{
    int a, i = 0;
    char arr[N] = "";
    for (a = getchar(); a != '\n'; )
	{
		arr[i] = a;
		i++;
		a = getchar();
	}
	
	arr[i] = '\0';
	
	for(int j = 0; j < i; j++)
    {
        printf("%d, ", arr[j]);
    }
    printf("\n");
    for(int j = 0; j < i; j++)
    {
        printf("%c", arr[j]);
    }
    
    long long N_count = i;
    long long res = 0;
    long long diff = 'a' - 'A';
    char * p_arr = arr;
    
	asm volatile(
		".intel_syntax noprefix\n"
        
        "mov r8, 96;\n"
        "L1:;\n"
        "xor rbx, rbx;\n"
        "mov bl, [rax + (rcx-1)];\n"
        "cmp rbx, r8;\n"
        "jle L_le;\n"
		"jg L_g;\n"
		
		"L_le:;\n"
		"add bl, dl;\n"
		"mov [rax + (rcx-1)], bl;\n"
		"jmp L_end;\n"
		
		"L_g:;\n"\
		"sub bl, dl;\n"
		"mov [rax + (rcx-1)], bl;\n"
		"jmp L_end;\n"
		
		"L_end:;\n"
		"loop L1;\n"

		".att_syntax noprefix"
		: "=d" (res)
		: "a" (p_arr), "c" (i), "d" (diff)
	);
	
	printf("\narr = %p, diff = %llu\n\n", p_arr, res);
	
    for(int j = 0; j < i; j++)
    {
        printf("%d, ", arr[j]);
    }
    printf("\n");
    for(int j = 0; j < i; j++)
    {
        printf("%c", arr[j]);
    }

    return 0;
}
