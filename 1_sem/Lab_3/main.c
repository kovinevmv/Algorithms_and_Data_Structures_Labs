#include "stdio.h" //updated_version_3.0 (using_dinamic_arrays, minor_bug_fixes)

void print(char* a, int len)
{	for (int i = (a[0] == ' ') ? 1 : 0; i < len; i++)
		printf("%c", a[i]);
}
int main(){
	char c;
	char* buf=NULL;
	int m = 0, n = 0, len = 0;
	while ((c = getchar()) != '!')
	{
		switch (c)
		{
		case '.':  print(buf, len); printf("%c\n", c); len = 0; m++; n++; break;
		case ';':  print(buf, len); printf("%c\n", c); len = 0; m++; n++; break;
		case '?':  len = 0; m++;    break;
		case '\n': len = 0;    	    break;
		case '\t': len = 0;    	    break;
		default:
		{
			buf = (char*)realloc(buf, (++len) * sizeof(char));
			buf[len - 1] = c; 
		}
		}
	}
	printf("Количество предложений до %d и количество предложений после %d", m, n);
	return 0;
}
