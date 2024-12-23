#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define IDX_SIZE 8
#define LINE_SIZE 16

char* hexdump(const char* data)
{
    if(data == NULL)
        return NULL;

    size_t len = strlen(data);
    const size_t buffLen = 128;

    size_t byteCount = (len / LINE_SIZE) > 0 ? (len / LINE_SIZE + 1) : 1;
    if(byteCount > 1 && len%LINE_SIZE == 0) byteCount--;
    
    char* hexDumped = (char*)malloc(buffLen * byteCount * sizeof(char));
    size_t currentPos = 0;
    size_t strAt = 0;

    for(size_t i = 0; i < byteCount; ++i)
    {
        strAt += sprintf(hexDumped + strAt, "%08zx: ", i*16);
        char complement[LINE_SIZE + 1];
        for(size_t j = 0; j < LINE_SIZE; ++j)
        {
            if(currentPos < len)
            {
                if((size_t)data[currentPos] >= 32 && (size_t)data[currentPos] <= 126)
                    complement[j] = data[currentPos];
                else
                    complement[j] = '.';
            }
            else 
                complement[j] = ' ';

            if(currentPos < len)
                strAt += sprintf(hexDumped + strAt, "%02zx ", (size_t)(data[currentPos]));
            else
                strAt += sprintf(hexDumped + strAt, "   ");

            currentPos++;
        }
        complement[LINE_SIZE] = '\0';
        strAt += sprintf(hexDumped + strAt, " %s\n", complement);
    }
    hexDumped[strAt] = '\0';
    return hexDumped;
}

char* dehex(const char* data)
{
    if(data == NULL)
        return NULL;

    const size_t width = 48;
    size_t lines = 1;
    size_t len = strlen(data);

    for(size_t i = 0; i < len; ++i)
    {
        if(data[i] == '\n')
            lines++;
    }

    char buffer[width * lines * 2];

    for(size_t i = 0, j = 0; i < len; ++i)
    {
        if(data[i] == ':')
        {
            size_t tmp = j* width;
            snprintf(buffer + tmp, width + 1, "%s ", data + i + 2);
            j++;
        }
    }

    char* result = (char*)malloc(lines * lines * 2 * sizeof(char));
    char* p = strtok(buffer, " ");
    size_t position = 0;

    while (p != NULL)
    {
        size_t tmp;
        sscanf(p, "%zx", &tmp);
        result[position++] = (unsigned char)tmp;
        p = strtok(NULL, " ");
    }
    result[position] = '\0';
    return result;
}

int main()
{
    char testDataShort[] = "C Hexdump test for both hexdump and dehex!! _&*";
    char* hex = hexdump(testDataShort);
    char* dhex = dehex(hex);
    printf("%s\n", hex);
    printf("%s\n", dhex);
    free(hex);
    free(dhex);
    return 0;
}
