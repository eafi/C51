
#include <reg52.h>

#define LCD1602_DB  P0
sbit LCD1602_RS = P1^0;
sbit LCD1602_RW = P1^1;
sbit LCD1602_E  = P1^5;
#define INTEGRITY_EXP   11      //0x0000 0x0 0x0000 $ \0
#define VALUE_EXP       5
#define RESULT_EXP      (VALUE_EXP+1)		   




char Integrity_exp[INTEGRITY_EXP] = {"3E-1234#"};
char Left_value[VALUE_EXP] = {0,};
char Right_value[VALUE_EXP] = {0,};
char Result[RESULT_EXP] = {0,};
char Output[RESULT_EXP] = {0,};

struct Op_type
{
    char type;
    char s_pos;
    char e_pos;
}op_type;

char error_overflow[] = {"overflow!"};
char error_invalid[] = {"exp invalid!"};


void find_operator();
char* process_express();
char* hex_add_hex(char *exp,char s_pos,char e_pos);
char* hex_minus_hex(char *exp,char s_pos,char e_pos);
char* reverse_negative(char* exp,char len);
void reverse_answer();
void InitLcd1602();
void LcdShowStr(unsigned char x, unsigned char y, unsigned char *str);
int main()
{
    unsigned char str[] = "aaaa";

    InitLcd1602();
    LcdShowStr(0, 0, Integrity_exp);
	find_operator();
	process_express();
   	reverse_answer();
    LcdShowStr(0, 1, Result+1);
    while (1);
}

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
    if(error > 1)
    {
        op_type.type = '$';
    }
}

char* process_express()
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
	return error_invalid; 
}

/*
pos may be 0 , like " 2020" "+2020"

*/
char* hex_add_hex(char *exp,char s_pos,char e_pos)
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
            return error_overflow;
    }
    return Result;
       
}

char* hex_minus_hex(char *exp,char s_pos,char e_pos)
{    char carry = 0,i = 0,j = s_pos + 1,little = 0,big = 0;
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
            res += 16;
        }else carry = 0;
        Result[i++] = res >= 10?res+55:res+48; 
        --big;--little;
        --lDelta;
        if(--res_count < 0)
            return error_overflow;
        }
        
        if(negative)
        {
            return reverse_negative(Result,i);
        }
    return Result;
}

char* reverse_negative(char* exp,char len)
{
	int i = len;
	return exp;
}

void reverse_answer()
{
	char i = 0,tmp = 0,len=0;
	while(Result[len] != 0)
	{
		++len;
	}
	for (; i < len / 2; ++i)
	{
		tmp = Result[i];
		Result[i] = Result[len - i];
		Result[len - i] = tmp;
	}
}

void LcdWaitReady()
{
    unsigned char sta;
    
    LCD1602_DB = 0xFF;
    LCD1602_RS = 0;
    LCD1602_RW = 1;
    do {
        LCD1602_E = 1;
        sta = LCD1602_DB; //读取状态字
        LCD1602_E = 0;
    } while (sta & 0x80); //bit7等于1表示液晶正忙，重复检测直到其等于0为止
}

void LcdWriteCmd(unsigned char cmd)
{
    LcdWaitReady();
    LCD1602_RS = 0;
    LCD1602_RW = 0;
    LCD1602_DB = cmd;
    LCD1602_E  = 1;
    LCD1602_E  = 0;
}

void LcdWriteDat(unsigned char dat)
{
    LcdWaitReady();
    LCD1602_RS = 1;
    LCD1602_RW = 0;
    LCD1602_DB = dat;
    LCD1602_E  = 1;
    LCD1602_E  = 0;
}

void LcdSetCursor(unsigned char x, unsigned char y)
{
    unsigned char addr;
    
    if (y == 0)  //由输入的屏幕坐标计算显示RAM的地址
        addr = 0x00 + x;  //第一行字符地址从0x00起始
    else
        addr = 0x40 + x;  //第二行字符地址从0x40起始
    LcdWriteCmd(addr | 0x80);  //设置RAM地址
}

void LcdShowStr(unsigned char x, unsigned char y, unsigned char *str)
{
    LcdSetCursor(x, y);   //设置起始地址
    while (*str != '\0')  //连续写入字符串数据，直到检测到结束符
    {
        LcdWriteDat(*str++);  //先取str指向的数据，然后str自加1
    }
}

void InitLcd1602()
{
    LcdWriteCmd(0x38);  //16*2显示，5*7点阵，8位数据接口
    LcdWriteCmd(0x0C);  //显示器开，光标关闭
    LcdWriteCmd(0x06);  //文字不动，地址自动+1
    LcdWriteCmd(0x01);  //清屏
}
