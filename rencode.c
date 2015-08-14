#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <limits.h>
#include <string.h>

bool file_exists(char* filepath)
{
    return access(filepath,F_OK)!=-1;
}
void pause()
{
//    getchar();
    system("pause");
}
char target_file[FILENAME_MAX+1]= {0};
char temporary_file[FILENAME_MAX+1]= {0};
unsigned int encodings;
int main()
{
    printf("enter the file to re-encode (or just drag it in the black window) - and press Enter\n");
    if(1<scanf("%s",target_file))
    {
        fprintf(stderr,"Error: could not read string from input.. (i dont know why) - errno: %i",errno);
        exit(EXIT_FAILURE);
    }
    if(!file_exists(target_file))
    {
        fprintf(stderr,"Error: file does not exist (or does not have access to read): %s - errno: %i",target_file,errno);
        exit(EXIT_FAILURE);
    }
    {
        size_t len=strlen(target_file);
        memcpy(temporary_file,target_file,len);
        strcpy(&temporary_file[len],"_temp.mp4");
        unlink(temporary_file);

    }
    printf("how many times to re-encode the file? enter a number between 1 and %u\n",UINT_MAX);
    if(1<scanf("%u",&encodings))
    {
        fprintf(stderr,"Error: could not read number from input.. (i dont know why) - errno: %i",errno);
        exit(EXIT_FAILURE);
    }
    printf("\nOK. will encode the file \"%s\", %u times. using temporary file: \"%s\"\n",target_file,encodings,temporary_file);

    char* command_1_format="ffmpeg -y -i \"%s\" \"%s\"";
    size_t command_1_size=snprintf(NULL,0,command_1_format,target_file,temporary_file);
    char command_1[command_1_size+1];
    snprintf(command_1,command_1_size+1,command_1_format,target_file,temporary_file);
    printf("Command 1 will be: \n\n%s\n",command_1);
    char* command_2_format="ffmpeg -y -i \"%s\" \"%s\"";
    size_t command_2_size=snprintf(NULL,0,command_2_format,temporary_file,target_file);
    char command_2[command_2_size+1];
    snprintf(command_2,command_2_size+1,command_2_format,temporary_file,target_file);
    printf("Command 2 will be: \n\n%s\n\n",command_2);
    printf("if everything looks right, press enter to start...\n");
    pause();
    {
        size_t i;
        int ret;
        char buf[1337];
        for(i=0; i<encodings; ++i)
        {
            snprintf(buf,1337,"title Encoding Pass %u",i+1);
            system(buf);
            ret=system(command_1);
            if(ret!=0)
            {
                fprintf(stderr,"\n\nWARNING: on command1 \"%s\", ffmpeg did not return 0 (as was expected of it), but returned %i. check error output from ffmepg above..\n\n",command_1,ret);
            }
            ret=system(command_2);
            if(ret!=0)
            {
                fprintf(stderr,"\n\nWARNING: on command2 \"%s\", ffmpeg did not return 0 (as was expected of it), but returned %i. check error output from ffmepg above..\n\n",command_2,ret);
            }
        }
    }
    return 0;
}
