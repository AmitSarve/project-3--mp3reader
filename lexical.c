#include<stdio.h>
#include<ctype.h>
#include<string.h>
int main()
{
     char keywords[][32]={
                "auto", "break", "case", "char", 
                "const", "continue", "default", 
                "do", "double", "else", "enum", 
                "extern", "float", "for", "goto", 
                "if", "int", "long", "register",
                "return", "short", "signed", 
                "sizeof", "static", "struct", 
                "switch", "typedef", "union", 
                "unsigned", "void", "volatile", "while" 
            };
        char operator[17][3]=
        {
            "==", "!=", "<=", ">=", "++", "--", "+=", "-=", "*=", "/=", "+", "-", "*", "/", "=", "<", ">"
        };
    FILE *fp;
    char str[1000];

    fp = fopen("file.txt", "r");

    if(fp == NULL)
    {
    printf("File opening failed\n");
    return 1;
    }

while(fgets(str, sizeof(str), fp))
{
    int i = 0;
    while(str[i] !='\0')
    {
        //to ignore spaces
        if(isspace(str[i]))
        {
            i++;
            continue;
            
        }
       
      // avoid preprocessor
        if(str[i]=='#')
        {
            int found1=0;
            int found2=0;
            char word[50];
            int j=0;
            while(str[i]!='\n' && str[i]!='\0')
            {
                
                if(str[i]=='<')
                {
                    found1++;
                    
                }
                if(str[i]=='>')
                {
                    found2++;
                    
                }

                word[j++]=str[i++];
            }
            word[j]='\0';
            continue;
        }
        

        //check digit
        if(isdigit(str[i]))
{
    char word[50];
    int j=0;
    int dot=0;

    while(isdigit(str[i]) || str[i]=='.')
    {
        if(str[i]=='.')
        {
            dot++;
        }

        word[j++]=str[i++];
    }

    // Check if number is followed by alphabet or _
    if(isalpha(str[i]) || str[i]=='_')
    {
        while(isalnum(str[i]) || str[i]=='_')
        {
            word[j++]=str[i++];
        }

        word[j]='\0';

        printf("%s --> error\n",word);
    }
    else
    {
        word[j]='\0';

        if(dot==0)
        {
            printf("%s --> int\n",word);
        }
        else if(dot==1)
        {
            printf("%s --> float\n",word);
        }
        else
        {
            printf("%s --> error\n",word);
        }
    }
}

        //check keywords
            
        else if (isalpha(str[i]) || str[i] == '_') 
        {
             char word[50]; 
             int j = 0;
              while (isalnum(str[i]) || (str[i] == '_'))
            { 
                word[j++] = str[i++];
            }
              word[j] = '\0'; 
              int flag = 0;
               for (int k = 0; k < 32; k++)
                {
                     if (strcmp(word, keywords[k]) == 0)
                    {
                         flag = 1;
                          break;
                    }
                }
                 if (flag)
                {
                     printf("%s --> keyword\n", word); 
                }
                else if(word[1]>='0' && word[1]<='9')
                {
                    printf("error\n");
                }
                 else
                {
                     printf("%s --> identifier\n", word);
                }
        } 
        else if (str[i] == '+' || str[i] == '-' || str[i] == '*' || str[i] == '/' || str[i] == '=') 
        {
            char word[5];
            int j=0;
            word[j++]=str[i++];
            if(word[0]=='='&&str[i]=='='||word[0]=='+'&&str[i]=='+'||word[0]=='-'&&str[i]=='-'||word[0]=='>'&&str[i]=='>'||word[0]=='<'&&str[i]=='<'||word[0]=='>'&&str[i]=='='||word[0]=='<'&&str[i]=='=' )
            {
                word[j++]=str[i++];
            }
            word[j]=0;

            for(int k=0;k<17;k++)
            {
            
                if(strcmp(word,operator[k])==0)
                {
                    printf("%s  --->operator\n",word);
                }
            }
        
         }
         else if(str[i]==';'||str[i]=='('||str[i]==')'||str[i]=='['||str[i]==']'||str[i]=='{'||str[i]=='}'||str[i]==','||str[i]=='.')
         {
            char word[10];
            int j=0;
            word[j++]=str[i++];
            word[j]=0;
            printf("%s  --->seperator\n",word);
         
         }
         
         else if(str[i]=='"')
         {
            char word[1000];
            int j=0;
            i++;

            while(str[i]!='"'&&str[i]!=0)
            {
                word[j++]=str[i++];
            }
            word[j]=0;
            if(str[i]=='"')
            {
                i++;
            }
            printf("%s--->literals\n",word);
         }
         else if(str[i] == '\'')
        {
            char ch;
            i++;

            ch = str[i++];

            if(str[i] == '\'')
            {
                i++;
                printf("%c --> character\n", ch);
            }
            else
            {
                printf("Error\n");
            }
        } 
                
    }
}

}