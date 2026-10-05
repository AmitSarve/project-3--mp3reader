#include<stdio.h>
#include<string.h>
typedef enum
{
    e_view,
    e_edit,
    e_help,
    e_unsupported
} OperationType;
typedef enum
{
    e_success,
    e_failure
} Status;
struct File
{
    FILE*fptr_ip_file;
    char *fptr_file_name;

    char *operation;
    char *new_data;
} ;

Status read_and_validate_(char*argv[],struct File *file);
OperationType check_operation_type(char*opt);
Status open_file(struct File *file);
Status view_tag_execution(struct File*file);
Status validate_ip(char*argv[],struct File*file);
Status edit_part(struct File *file);


int main(int argc, char*argv[])
{
   struct File file;
    
    if(argc<=2 || argc>5)
    {
        printf("Invalid input !!! pass ip like : \n for display: ./a.out -v mp3filename \n for edit :./a.out -e -t/-a/-A/-m/-y/-c mp3filename\n");
        return 1;
    }
    if(check_operation_type(&argv[1][1])==e_view)
    {
        if(read_and_validate_(argv,&file)==e_success)
        {
            printf("file validation done\n");

            if(view_tag_execution(&file)==e_success)
            {
                printf("view tag done\n");
            }


        }
        else{
            printf("file validation failed\n");
        }

    }
   else if(check_operation_type(&argv[1][1])==e_edit)
    {
        
        if(argc!=5)
        {
            printf("Error !!Enter the valid operation:\n"
       "-t --> to edit song title\n"
       "-a --> to edit artist name\n"
       "-A --> to edit album name\n"
       "-m --> to edit file content\n"
       "-y --> to edit year\n"
       "-c --> to edit composer\n");
       return e_failure;
        }
        
       if(validate_ip(argv,&file)==e_success)
       {
        if(edit_part(&file)==e_success)
        {
            printf("Edit part is done\n");
        }
       }
       else
       {
       printf("Enter the valid operation:\n"
       "-t --> to edit song title\n"
       "-a --> to edit artist name\n"
       "-A --> to edit album name\n"
       "-m --> to edit file content\n"
       "-y --> to edit year\n"
       "-c --> to edit composer\n");
       }
    }
    else if(check_operation_type(&argv[1][1])==e_help)
    {
        printf(" pass ip like : \n for display: ./a.out -v mp3filename \n for edit :./a.out -e -t/-a/-A/-m/-y/-c newData mp3filename\n");
        return 1;
    }
    else if(check_operation_type(&argv[1][1])==e_unsupported)
    {
        printf("Invalid input !!! pass ip like : \n for display: ./a.out -v mp3filename \n for edit :./a.out -e -t/-a/-A/-m/-y/-c newData mp3filename\n");
        return 1;
        
    }
    
return 0;
}
OperationType check_operation_type(char*opt)
    {
        if(strcmp(opt,"v")==0)
        {
            return e_view;
        }
        else if(strcmp(opt,"e")==0)
        {
            return e_edit;
        }
        else if(strcmp(opt,"h")==0)
        {
            return e_help;
        }
        else{
            return e_unsupported;
        }

    }
Status read_and_validate_(char*argv[],struct File *file)
{
    char*extension;
    extension=strrchr(argv[2],'.');

    if(extension==NULL || strcmp(extension,".mp3")!=0)
    {
        printf("File should be .mp3 file\n");
        return e_failure;
    }
    file->fptr_file_name=argv[2];
    if(open_file(file)==e_failure)
    {
        return e_failure;
    } 
    
    return e_success;

}
Status open_file(struct File *file)
{
    file->fptr_ip_file=fopen(file->fptr_file_name,"rb");

    if(file->fptr_ip_file==NULL)
    {
        printf("file not opened\n");
        return e_failure;
    }

    char buff[3];
    fread(buff,1,3,file->fptr_ip_file);

        if(buff[0]=='I' &&buff[1]=='D' && buff[2]=='3')
        {
            fseek(file->fptr_ip_file,10,SEEK_SET);
            return e_success;
        }

    printf("ID3 header not found\n");
    
    fclose(file->fptr_ip_file);
    return e_failure;
}

Status view_tag_execution(struct File *file)
{
    int sr=0;
    char frame_id[5];
    unsigned char size_bytes[4];
    char data[100];
    unsigned int size;
    printf("\n+------+------------+-----------------------------\n");
    printf("| Sr. | TAG   | DATA                              |\n"); 
    printf("+------+------------+-----------------------------\n");
    while(1)
    {
        // Read tag 
        if(fread(frame_id, 1, 4, file->fptr_ip_file) != 4)
        {
            break;
        }

        frame_id[4] = '\0';

        //Read size 
        if(fread(size_bytes, 1, 4, file->fptr_ip_file) != 4)
        {
            break;
        }

        size = 0;

        for(int i = 0; i < 4; i++)
        {
            size = size * 256 + size_bytes[i];
        }

        //Skip flags
        fseek(file->fptr_ip_file, 2, SEEK_CUR);

        // Read data 
        if(size >= sizeof(data))
        {
            fseek(file->fptr_ip_file, size, SEEK_CUR);
            continue;
        }

        if(fread(data, 1, size, file->fptr_ip_file) != size)
        {
            break;
        }

       

        //Print only required tags 
        if(strcmp(frame_id, "TIT2") == 0)
        {
            printf("| %d | TITLE   | %s | ",(sr++)+1,&data[1]);
        }
        else if(strcmp(frame_id, "TPE1") == 0)
        {
            printf("| %d | ARTIST  | %s | ",(sr++)+1,&data[1]);
        }
        else if(strcmp(frame_id, "TALB") == 0)
        {
            printf("| %d | ALBUM   | %s | ",(sr++)+1,&data[1]);
        }
        else if(strcmp(frame_id, "TYER") == 0)
        {
            printf("| %d | YEAR    | %s |",(sr++)+1,&data[1]);
        }
        else if(strcmp(frame_id, "TCON") == 0)
        {
            printf("| %d | GENRE   | %s | ",(sr++)+1,&data[1]);
        }
        else if(strcmp(frame_id, "TCOM") == 0)
        {
            printf("| %d | COMPOSER   | %s | ",(sr++)+1,&data[1]);
        }
        else
        {
            continue;
        }

        printf("\n");
    }
     printf("+------+------------+-----------------------------\n");

    return e_success;
}

// editing part

Status validate_ip(char*argv[],struct File*file)
{
    if(strcmp(argv[2],"-t")==0 || strcmp(argv[2],"-A")==0 || strcmp(argv[2],"-a")==0 || strcmp(argv[2],"-y")==0 ||strcmp( argv[2],"-c")==0 || strcmp(argv[2],"-m")==0)
    {
        char*extension;
        extension=strrchr(argv[4],'.');

    if(extension==NULL || strcmp(extension,".mp3")!=0)
    {
        printf("File should be .mp3 file\n");
        return e_failure;
    }
    file->fptr_file_name=argv[4];
    file->operation=argv[2];
    file->new_data=argv[3];

    if(open_file(file)==e_failure)
    {
        return e_failure;
    } 
    
    return e_success;

    }
   
    else 
    return e_failure;   
}
Status edit_part(struct File *file)
{
    FILE *src, *dest;

    char target[10];

    if(strcmp(file->operation, "-t") == 0)
        strcpy(target, "TIT2");

    else if(strcmp(file->operation, "-a") == 0)
        strcpy(target, "TPE1");

    else if(strcmp(file->operation, "-A") == 0)
        strcpy(target, "TALB");

    else if(strcmp(file->operation, "-y") == 0)
        strcpy(target, "TYER");

    else if(strcmp(file->operation, "-c") == 0)
        strcpy(target, "TCOM");

    else
        strcpy(target, "TCON");

    fclose(file->fptr_ip_file);

    src = fopen(file->fptr_file_name, "rb");
    dest = fopen("temp.mp3", "wb");

    if(src == NULL || dest == NULL)
    {
        printf("File not open\n");
        return e_failure;
    }


    /* Copy ID3 header */
    unsigned char header[10];

    fread(header, 1, 10, src);
    fwrite(header, 1, 10, dest);


    while(1)
    {
        char tag[5];
       unsigned char size[4];
        unsigned char flag[2];

        int old_size = 0;
        if(fread(tag, 1, 4, src) != 4)
            break;

        tag[4] = '\0';
        if(fread(size, 1, 4, src) != 4)
            break;
        for(int i = 0; i < 4; i++)
        {
            old_size = old_size * 256 + size[i];
        }
        if(old_size==0)
        break;

        if(fread(flag, 1, 2, src) != 2)
            break;

        if(strcmp(tag, target) == 0)
        {
            int new_size = strlen(file->new_data) + 1;

             char new_size_byte[4];

            int temp = new_size;

            /* Convert size to Big Endian */
            for(int i=3;i>=0;i--)
            {
                new_size_byte[i] = temp % 256;
                temp = temp / 256;
            }

            fwrite(tag, 1, 4, dest);

            fwrite(new_size_byte, 1, 4, dest);

            fwrite(flag, 1, 2, dest);
              fputc(0, dest);

            /* Write new data */
            fwrite(file->new_data, 1,strlen(file->new_data), dest);

            /* Skip old data */
            fseek(src, old_size, SEEK_CUR);
        }

        else
        {
            fwrite(tag, 1, 4, dest);

            fwrite(size, 1, 4, dest);

            fwrite(flag, 1, 2, dest);


            /* Copy old data */
            char data[100];

          int remaining = old_size;
          int read_size;

    while(remaining > 0)
    {
        if(remaining > sizeof(data))
            read_size = sizeof(data);
        else
            read_size = remaining;

        fread(data, 1, read_size, src);
        fwrite(data, 1, read_size, dest);

        remaining = remaining - read_size;
    }
    }
    }

    char buffer[4096];
    int bytes;

    while((bytes = fread(buffer, 1, sizeof(buffer), src)) > 0)
    {
        fwrite(buffer, 1, bytes, dest);
    }


    fclose(src);
    fclose(dest);
    return e_success;
}