
/*
sperate from c51
*/
#include "reg.h"

char hex_add_hex(char *exp,char s_pos,char e_pos);
char hex_minus_hex(char *exp,char s_pos,char e_pos);
void reverse_answer(char* exp);
char Integrity_exp[INTEGRITY_EXP] = {0,};
char Result[RESULT_EXP] = {0,};
int code Hex_map[4] = {1,16,256,4096};
struct Op_type
{
    char type;
    char s_pos;
    char e_pos;
}op_type;


/*
__fun__ find_operator() will catch first '+' or '-' operator and the '#' 
return value list : 
{
    '+' '-' or '$' while experssion invalid.
}
eg.
input "20+20" will set op_type {'+';'2';'5'}
*/
void find_operator()
{
    char i = 0,error=0,j=0;
    while(i != INTEGRITY_EXP)
    {
        if(Integrity_exp[i] == '+')
        {
            ++error;
            j = i;
            op_type.type = '+';
        }
        else if(Integrity_exp[i] == '-')
        {
           ++error;
            j = i;
            op_type.type = '-';
        }
        else if(Integrity_exp[i] == '#')
        {
            op_type.e_pos = i;break;
        }
        else if(Integrity_exp[i] <= 'F' && Integrity_exp[i] >= 'A')
            Integrity_exp[i] = (int)(Integrity_exp[i]- 55);
        else if(Integrity_exp[i] <= '9' && Integrity_exp[i] >= '0')
            Integrity_exp[i] = (int)(Integrity_exp[i] - 48);
        
        ++i;
    }
    op_type.s_pos = j;
    if(error > 1 || !op_type.type)
    {
        op_type.type = '$';
    }

}

char process_express()
{
    char op = op_type.type;
    switch(op)
    {
        case '+' :
            return hex_add_hex(Integrity_exp,op_type.s_pos,op_type.e_pos);
        case '-' :
            return hex_minus_hex(Integrity_exp,op_type.s_pos,op_type.e_pos);
        break;
        case '$' :
        break;
        default :
        break;
    }
    Result[RESULT_EXP-1] = '\0';
	return ERROR_INVALID; 
}

/*
pos may be 0 , like " 2020" "+2020"

*/
char hex_add_hex(char *exp,char s_pos,char e_pos)
{
    char carry = 0,i = s_pos-1,j = e_pos - 1;
    char x = 0,y = 0,res = 0,res_count = 0;
    while(i>=0 || j>s_pos)
    {
        x = i>=0?exp[i]:0;
        y = j>s_pos?exp[j]:0;
        res = x + y + carry;
        if(res >= 16)
        {
            carry = 1;
            res -= 16;
        }else carry = 0;
        // ascii convert
        Result[res_count] = res >= 10?res+55:res+48; 
        --i;--j;
        if(++res_count >= RESULT_EXP)
            return ERROR_OVERFLOW;
        }

		if(carry)
			Result[res_count] = 49;
        reverse_answer(Result);
    return VALID;
       
}

/*
* minus as postive numbers
* tip negative flag if <0
* process negative
*/
char hex_minus_hex(char *exp,char s_pos,char e_pos)
{    char carry = 0,i = 0,j = s_pos + 1,little = 0,big = 0;int mov;unsigned int t = 0;
    char lDelta = s_pos - i,rDelta = e_pos - s_pos - 1;
    char negative = 0;
    char x = 0,y = 0,res = 0,res_count = lDelta;
    if(lDelta < rDelta)
    {
        negative = 1;
        big = e_pos - 1;
        little = s_pos - 1;
        res_count = rDelta;
    }
    else if(lDelta > rDelta)
    {
        big = s_pos - 1;
        little = e_pos - 1;
    }
    else
    while(i != s_pos)
    {
        if(exp[i] > exp[j])
        {
            big = s_pos - 1;
            little = e_pos - 1;
            break;
        }else if (exp[i] < exp[j])
        {
            negative = 1;
            big = e_pos - 1;
            little = s_pos - 1;
            break;
        }
        ++i;++j;
    } i = 0;
    while(res_count)
    {
        x = big>=0&&exp[big]>=0&&exp[big]<=15?exp[big]:0;
        y = little>=0&&exp[little]>=0&&exp[little]<=15?exp[little]:0;
        res = x - y + carry;
        if(res < 0)
        {
            carry = -1;
			--res_count;
            res += 16;
        }else carry = 0;
        //Result[i++] = res >= 10?res+55:res+48; 
        Result[i++] = res;
        --big;--little;
        --lDelta;
        if(--res_count < 0)
            return ERROR_OVERFLOW;
    }
    
    if(negative)
    {
        int tmp = 0; j = 0;
        for(;j<i;++j)
        {
            tmp -= Hex_map[j]*Result[j];
        }
        tmp +=65536;
        /*
        c51 int - 16 bits

        */
        //res = tmp;
        mov = 0x000f;
        for(j=0;j<RESULT_EXP;++j)
        {
            t = mov & tmp;
            t = t >> (4*j);
            Result[j] = t >= 10?t+55:t+48;
            mov = mov<<4;
        }

    }
    else
    {
         for(j=0;j<i;++j)
        {
            Result[j] = Result[j] >= 10?Result[j]+55:Result[j]+48;//map from absolute number to ASCII
        }  
    }
	reverse_answer(Result);
    return VALID;
}


void reverse_answer(char* exp)
{
	char i = 0,tmp = 0,len=0;
	while(exp[len] != '\0')
	{
		++len;
	}
	for (; i < len / 2; ++i)
	{
		tmp = exp[i];
		exp[i] = exp[len - i-1];
		exp[len - i -1] = tmp;
	}
}

