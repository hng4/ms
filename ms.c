/*
    H&A Industries
    MS Programming Language
    Known as H&A M
*/

#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "limits.h"

#define MS_ERROR_1 "Stack Overflow occured"
#define MS_ERROR_2 "Brackets not balanced"
#define MS_ERROR_3 "Invalid file"
#define MS_ERROR_4 "Can't write function"
#define MS_ERROR_5 "Invalid arguments"
#define MS_ERROR_6 "Invalid amount of arguments"
#define MS_ERROR_7 "Invalid string literal formed"
#define MS_ERROR_8 "Invalid variable"
#define MS_ERROR_9 "Invalid function environment"
#define MS_ERROR_10 "Function environment "
#define MS_ERROR_11 "Function not found in namespace"
#define MS_ERROR_12 "Denary number exceeds bit-wise maximum"
#define MS_CALL_EV1 "Called by function environment [v1]"
#define MS_CALL_DEF "Definition"
#define MS_CALL_CAL "Called by script"
#define MS_CALL_SYS "Called by system"
#define MS_CALL_MSY "Called from master system call"
#define MS_CALL_MFL "Called from main file loop"
#define MS_OPT_INT 0 // enables internet
#define MS_OPT_FILW 0 // enable file
#define MS_STACK_MS 1000
#define MS_STACK_FL 100000
#define MS_STACK_TO 10000
#define MS_STACK_MN 10000 // max number

struct __ms_functione {
    int id;
    char * name;
    char * call;
    int line;
    char * nspace;
};

struct __ms_vare {
    int id;
    int type;
    int line;
    int lock;
    int chan; // increment everytime it's changed
    char * name;
    char * contents;
};

struct __ms_udfun {
    int id;
    char * name;
    int argcount;
    char * rawarg;
//    char * rawarge;
    char rawarge[1000];
    int linestart;
    int lineend;
    int environment;
    int await;
};

struct __ms_functione Global_F[10000];
struct __ms_vare Global_V[10000];
struct __ms_udfun Global_UF[10000];

int __ms_fun[MS_STACK_MS];
int __ms_top = -1;

int __var_full(){
    int count;
    for(int ef = 0; ef < 10000; ef++){
        if(Global_V[ef].id != ef){
            count++;
        }
    }
    return 10000-count;
}

int __throw(char *enol, int line, int ne, int chare, char *fline){
    printf("\033[1;31m%s error occured on line %d\n\033[0m", enol, line);
    if(ne == 1)
    {
        printf("Error occured at:\n      %s\n", fline);
        chare += 6;
        int x;
        for(x=0; x < chare-1; x++)
        {
            printf(" ");
        }
        printf("^\n");
        printf("Variable table %d\n", __var_full());
        printf("Begin function trace (Most recent at the top)\n");
        int line = 0;
        int xe;
        while(__ms_top != -1){
            printf("%d ", __ms_fun[__ms_top]);
            for(xe = 0; xe < line; xe++){ printf(" "); }
            printf("-> '%s' %s on line %d\n", Global_F[__ms_fun[__ms_top]].name, Global_F[__ms_fun[__ms_top]].call, Global_F[__ms_fun[__ms_top]].line);
            __ms_top = __ms_top - 1;
            if(line > 60){ line = 0; }
            line = line + 2;
        }
    }
    exit(-1);
}

int __var_is(char *name){
    int e;
    for(int ef = 0; ef < 10000; ef++){
        if(Global_V[ef].name == name){
            e = ef;
        }
    }
    return e;
}

int __var_new(int line, char *name, char *contents, int type){
    int ef;
    for(ef = 0; ef < 10000; ef++){
        if(Global_V[ef].id != ef){
            break;
        }
    }
    Global_V[ef].id = ef;
    Global_V[ef].type = type;
    Global_V[ef].line = line;
    Global_V[ef].lock = 0;
    Global_V[ef].chan = 0;
    Global_V[ef].name = name;
    Global_V[ef].contents = contents;
    if(Global_V[ef].contents == contents && Global_V[ef].id == ef){
        return 0;
    } else {
        return 1;
    }
}

int __var_change(char *name, char *contents){
    int ef = __var_is(name);
    while(Global_V[ef].lock != 0){}
    Global_V[ef].lock = 1;
    Global_V[ef].chan = Global_V[ef].chan + 1;
    Global_V[ef].contents = contents;
    Global_V[ef].lock = 0;
}

int __var_get(char *name){
    int ef;
    for(ef = 0; ef < 10000; ef++){
        if(Global_V[ef].id > 0 && Global_V[ef].line > 0){
            if(strcmp(Global_V[ef].name, name) == 0){
                break;
            }
        }
    }
    return ef;
}

int __val_fun(int id, int mode){
    if(mode == 1){
        if(Global_F[id].id = id){ return 0; }
    }
}

int __reg_naf(){
    return __ms_fun[__ms_top]+1;
}

int __reg_pop(){
    if(__ms_top != -1){
        __ms_top = __ms_top - 1;
    }
}

int __reg_fun(int id, char *name, char *call, int line, int handle){
    if(__ms_top == MS_STACK_MS){ __throw(MS_ERROR_1, 0, 1, 4, "Stack push"); }
    __ms_top = __ms_top + 1;
    __ms_fun[__ms_top] = id;
    Global_F[id].name = name;
    Global_F[id].id = id;
    Global_F[id].call = call;
    Global_F[id].line = line;
    if(__val_fun(id, 1) == 0){
        return 0;

    } else {
        if(handle == 1)
            __throw(MS_ERROR_4, 0, 1, 4, "__reg_fun()");
        return 1;
    }
}

int __arglen(char *args){
    int count = 0;
    int ffd = 0;
    for(int eg = 0; eg < strlen(args); eg++){
        if(args[eg] == '$'){
            count = count + 1;
            ffd = 1;
        }
        if(args[eg] == ',' && ffd == 1){
            count = count + 1;
            ffd = 0;
        }

        if(eg+1 == strlen(args) && ffd == 1){
            count = count +1;
            ffd = 0;
        }

        if(args[eg] == '"' && ffd != 1){
            count = count + 1;
        }
    }
    if(count % 2 == 0){
        return count/2;
    } else {
        return -1;
    }
}

char *__slice(char * text, int end){
    char *ende = malloc(100);
    strncpy(ende, text, end);
    return ende;
    free(ende);
}

char * __arg(int mode, int no, char *args){
    if(mode == 1){
        int ef = 0;
        int count = 0;
        int times = 0;
        int isvar = 0;
        int start1 = 0;
        int start2 = 0;
        int ffd = 0;
        int ale = __arglen(args);
        for(ef = 0; ef < strlen(args); ef++){
            if(args[ef] == '$' && ffd == 0){
                count = count + 1;
                if(count == 1){ start1 = ef+1; }
                if(count == 2){ start2 = ef+1; count = 0; times = times + 1;}
                ffd = 1;
            }

            if(args[ef] == ',' && ffd == 1 && __arglen(args) > 1){
                count = count + 1;
                if(count == 1){ start1 = ef+1; }
                if(count == 2){ start2 = ef+1; count = 0; times = times + 1;}
                ffd = 0;
            }
            if(ef+1 == strlen(args) && ffd == 1){
                count = count + 1;
                if(count == 1){ start1 = ef+1; }
                if(count == 2){ start2 = ef+1; count = 0; times = times + 1;}
            }

            if(args[ef] == '"' && ffd == 0){
                count = count + 1;
                if(count == 1){ start1 = ef+1; }
                if(count == 2){ start2 = ef+1; count = 0; times = times + 1;}
            }
            if(times == no){
                if(args[start1-1] == '$'){ isvar = 1; }
                break;
            }
        }
        if(isvar == 1){
            args = __slice(args, start2-start1+1);
            char * oar = args;
            args = args + 1;
            if(ale > 1 &&  no != ale){
                args[strlen(args)-1] = '\0'; // -1
            }
//            printf("1 %s\n", args);
            if(__var_get(args) == 10000){ __throw(MS_ERROR_8, 0, 1, 1, oar); }
            char * ef = Global_V[__var_get(args)].contents;
            return ef;
        } else {
            args = args + start1;
            args = __slice(args, start2-start1-1);
            return args;
        }
    }
}

char * _narg(int no, char * string){
    int __times = 0; // amount of times
    int __curloops = 0; // looped through string
    char *result = malloc(strlen(string)+1);
    for(__curloops = 0; __curloops < strlen(string); __curloops++){
        if(string[__curloops] == ','){ __times++;}
    }
    if(__times != 0){
        __times = __times + 1;
    } else {
        __times = 1;
    }
    if(__times > 1){
        char ef[10000];
        strcpy(ef, string);
        char *ch;
        ch = strtok(ef, ",");
        int loop2 = 0;
        while(ch != NULL){
            loop2++;
            if(loop2 == no){ strcpy(result, ch); }
            ch = strtok(NULL, ",");
        }
    } else {
        strcpy(result, string);
    }
//    if(result[0] == '$'){ strcpy(result, Global_V[__var_get(result)].contents); }
    if(result[0] == '$'){
        char *tres = result + 1;
//        printf("%d\n", __var_get(tres));
        result = Global_V[__var_get(tres)].contents;
    }
    if(result[0] == '"'){ result = result + 1; }
//    if(result[strlen(result)-1] == '"'){
//        printf("nl\n");
//        result[strlen(result)-1] = '\0';
//    }
    return result;
}

int __get_last_fun(char *file, int start){
    FILE *fp = fopen(file, "r");
    char * __ms_off_buffer = malloc(MS_STACK_FL);
    int __ms_off_line = 1;
    int __last;
    while(fgets(__ms_off_buffer, MS_STACK_FL - 1, fp)){
        if(__ms_off_line > start || __ms_off_line == start){
            if(strcmp(__ms_off_buffer, "}") == 0){
                __last = __ms_off_line;
            }
        }
        __ms_off_line = __ms_off_line + 1;
    }
    free(__ms_off_buffer);
    fclose(fp);
    return __last;
}

int main(int argc, char *argv[])
{
    if(__reg_fun(1, "main", MS_CALL_MSY, 0, 0) != 0){ __throw(MS_ERROR_4, 0, 1, 4, "__reg_fun()"); }
    if(argc < 2){
        printf("\033[1m\033[32mMS Version 2 (for more, run ./ms copy)\n\033[0m");
        printf("To use MS, use the schema ./ms [filename]\n");
        exit(0);
    } else if(strcmp(argv[1], "copy") == 0){
        printf("\033[1m\033[32mMS Version 2 by H&A Industries (ms.hng.su.mt)\n\033[0m");
        printf("Primarily developed by Hayden NG (hng.su.mt)\n");
        printf("Owned by H&A Industries (www.handa.su.mt)\n\n");
        printf("Supported by:\n- Shared Domain Services under su.mt\n- Nyaru DB team (server allocation & testing)\n");
        exit(0);
    }
    FILE * __ms_filed;
    __ms_filed = fopen(argv[1], "r");
    __reg_fun(__reg_naf(), "fopen", MS_CALL_MSY, 0, 1);
    if(__ms_filed == NULL)
    {
        __throw(MS_ERROR_3, 0, 1, 3, "Opening new file");
    }
    int __ms_te_lin;
    int __ms_line = 0;
    __reg_pop();
//    char __ms_buffer[MS_STACK_FL];
    char *__ms_buffer = malloc(MS_STACK_FL);
    Global_V[0].id = 0;
    Global_V[0].name = "ms";
    Global_V[0].contents = "0x0001";
    Global_V[0].lock = 100;
    __var_new(0, "ms-author", "H&A Industries (english)", 0);
    while(fgets(__ms_buffer, MS_STACK_FL - 1, __ms_filed)){
        if(__ms_buffer[0] != '#'){
        __reg_fun(__reg_naf(), "main file loop", MS_CALL_MFL, 0, 1);
        int __ms_te_oft = -1;
        int __ms_te_ofb[MS_STACK_MS];

        int __ms_inf_firstpara = 0;
        int __ms_inf_lastparag = 0;
        int __ms_inf_paracount = 0;
        int __ms_inf_lastpacou = 0;
        int __ms_inf_isfunc = 1;
        int __ms_inf_isretn = 1;
        int __ms_inf_wvar = 0;
        int __ms_inf_var_equal;
        char * __ms_inf_var_name;
        char * __ms_inf_var_cont;
        char * __ms_inf_var_retn;
        int __ms_inf_v_ns;
        char * __ms_inf_v_retn = malloc(strlen(__ms_buffer));
        char *__ms_buf = malloc(MS_STACK_FL);
        char __ms_name[MS_STACK_FL];
        int __ms_inf_found = 0;

        for(int efgh = 0; efgh < 10000; efgh++){
                if(__ms_line+1 > Global_UF[efgh].linestart && Global_UF[efgh].lineend == 0 && Global_UF[efgh].environment > 0){
                    __ms_inf_found = 1;
            }
        }

        if(__ms_inf_found == 1){
            __ms_inf_isretn = 0;
            __ms_inf_isfunc = 0;
        }

        if(__ms_buffer[0] == '$'){
//            int __ms_inf_var_equal;
//            char * __ms_inf_var_name;
//            char * __ms_inf_var_cont;

            __ms_inf_isfunc = 0;
            for(int ef = 0; ef < strlen(__ms_buffer); ef++){
                if(__ms_buffer[ef] == '='){
                    __ms_inf_var_equal = ef-1;
                    __ms_inf_var_name = __slice(__ms_buffer, __ms_inf_var_equal);
                    __ms_inf_var_name++;
                    __ms_inf_var_cont = __ms_buffer + __ms_inf_var_equal + 4;
                    char * __ms_inf_var_full = __ms_buffer + __ms_inf_var_equal+3;
                    if(__ms_inf_var_full[0] != '"'){
                        __ms_inf_isfunc = 1;
                        __ms_inf_isretn = 1;
                        strcpy(__ms_buffer, __ms_inf_var_full);
                    } else {
                        __ms_inf_var_cont = __slice(__ms_inf_var_cont, strlen(__ms_inf_var_cont)-2);
                        __var_new(__ms_line+1, __ms_inf_var_name, __ms_inf_var_cont, 1);
                        __ms_inf_isretn = 0;
                    }
                }
            }
        } else if(__ms_buffer[0] == '}'){
            int __ms_te_cur_lin;
            int __ms_te_cur_found;
            for(__ms_te_cur_lin = 0; __ms_te_cur_lin < 10000; __ms_te_cur_lin++){
                if(Global_UF[__ms_te_cur_lin].await == 1){
                    __ms_te_cur_found = __ms_te_cur_lin;
                }
            }
            Global_UF[__ms_te_cur_found].lineend = __ms_line+1;
            __ms_inf_isfunc = 0;
        } else {
            __ms_inf_isretn = 0;
        }

        char * __ms_inf_namespace;
        char * __ms_inf_namespdef;
        int __ms_inf_isspace = 0;

        if(__ms_inf_isfunc == 1){
            for(int eg = 0; eg < strlen(__ms_buffer); eg++){
                if(__ms_buffer[eg] == '('){
                    if(__ms_inf_paracount == 0){
                        __ms_inf_firstpara = eg;
                    }
                    __ms_te_oft = __ms_te_oft + 1;
                    __ms_te_ofb[__ms_te_oft] = '1';
                    __ms_inf_paracount = __ms_inf_paracount + 1;
                } else if(__ms_buffer[eg] == ')'){
                    if(__ms_inf_paracount == 1){
                        __ms_inf_lastparag = eg;
                    }
                    __ms_te_oft = __ms_te_oft - 1;
                    __ms_inf_paracount = __ms_inf_paracount - 1;
                } else if(__ms_buffer[eg] == '.' && __ms_inf_firstpara == 0 && __ms_inf_lastparag == 0){
                    __ms_inf_isspace = eg;
                }
            }

            char * __ms_inf_args = __ms_buffer + __ms_inf_firstpara + 1;
             __ms_inf_args[__ms_inf_lastparag-__ms_inf_firstpara-1] = '\0';
            int __ms_inf_alen = __arglen(__ms_inf_args);

            if(__ms_te_oft != -1){
                __throw(MS_ERROR_2, __ms_line+1, 1, strlen(__ms_buffer)-1, __ms_buffer);
            }
            __ms_te_lin = 0;

            for(int eg = 0; eg < strlen(__ms_buffer); eg++){
                if(__ms_buffer[eg] == '('){
                    __reg_fun(__reg_naf(), __slice(__ms_buffer, __ms_te_lin), MS_CALL_CAL, __ms_line+1, 1);
                    break;
                }
                __ms_te_lin = __ms_te_lin + 1;
            }

            char * __ms_te_fun = __slice(__ms_buffer, __ms_te_lin);
            if(__ms_inf_isspace > 0){
                __ms_inf_namespdef =__ms_te_fun + __ms_inf_isspace + 1;
                __ms_inf_namespace = __ms_te_fun;
                __ms_inf_namespace[__ms_inf_isspace] = '\0';
            }

            if(strcmp(__ms_te_fun, "ms_dump_tr") == 0){
                __throw(MS_ERROR_1, __ms_line+1, 1, 10, "Purposely thrown error");
            } else if(strcmp(__ms_te_fun, "print") == 0){
                if(__ms_inf_alen != -1){
                    if(__ms_inf_alen == 1){
                        printf("%s\n", __arg(1, 1, __ms_inf_args));
                    } else {
                        __throw(MS_ERROR_6, __ms_line+1, 1, strlen(__ms_inf_args)-1, __ms_inf_args);
                    }
                } else {
                    __throw(MS_ERROR_5, __ms_line+1, 1, strlen(__ms_buffer)-1, __ms_buffer);
                }
            } else if(strcmp(__ms_te_fun, "__throw") == 0){
                __ms_inf_args++;
                __ms_inf_args = __slice(__ms_inf_args, strlen(__ms_inf_args)-1);
                __throw(__ms_inf_args, __ms_line+1, 1, 3, __ms_buffer);
            } else if(strcmp(__ms_te_fun, "__dump_var") == 0){
                for(int ef = 0; ef < MS_STACK_TO; ef++){
                    if(Global_V[ef].id == ef){
                        printf("-> Variable ID %d\n", ef);
                        printf("   -> Name: %s\n", Global_V[ef].name);
                        printf("   -> Contents: %s\n", Global_V[ef].contents);
                        printf("   -> Lock Status: %d\n", Global_V[ef].lock);
                        printf("   -> Formed on line: %d\n", Global_V[ef].line);
                        if(Global_V[ef].lock != 0){
                            printf("    * Not writeable\n");
                        } else {
                            printf("    * writeable\n");
                        }
                    }
                }
                __ms_inf_var_retn = "hello";
            } else if(strcmp(__ms_te_fun, "__comp") == 0){
                printf("is %d\n", strcmp(Global_V[__var_get("$var")].contents, "hayden"));
            } else if(strcmp(__ms_te_fun, "math") == 0){
                int no1 = atoi(__arg(1, 1, __ms_inf_args));
                printf("%d\n", no1);
                int no2 = atoi(__arg(1, 3, __ms_inf_args));
                char * op = __arg(1, 2, __ms_inf_args);
                if(op[0] == '*'){
                    int ma = no1*no2;
                    int ma_1 = ma;
                    int ma_2 = 0;
                    while(ma_1 > 0){
                        ma_1 = ma_1/10;
                        ma_2++;
                    }
                    char res[MS_STACK_MN];
                    sprintf(res, "%d", ma);
                    __ms_inf_var_retn = res;
//                    __ms_inf_var_retn = itoa_simple(no1 * no2, INT_MAX);
                } else if(op[0] == '+'){
                    int ma = no1+no2;
                    char res[MS_STACK_MN];
                    sprintf(res, "%d", ma);
                    __ms_inf_var_retn = res;
                }
            // venv dummys
            } else if(strcmp(__ms_te_fun, "0x0001") == 0){
            // end venv dummy
            } else if(strcmp(__slice(__ms_te_fun, 3), "def") == 0){
                char * __ms_te_def_fun = __ms_te_fun + 4;
                char * __ms_te_def_arg = malloc(strlen(__ms_buffer)+strlen(__ms_te_fun)+1);
                __ms_te_def_arg = __ms_buffer + strlen(__ms_te_fun) + 1;
                int __ms_te_def_len = __arglen(__ms_te_def_arg);
                int efg;
                for(efg = 1; efg < 10000; efg++){
                    if(Global_UF[efg].id != efg){
                        break;
                    }
                }
                Global_UF[efg].id = efg;
                Global_UF[efg].name = __ms_te_def_fun;
                Global_UF[efg].argcount = __ms_te_def_len;
                strcpy(Global_UF[efg].rawarge, __ms_te_def_arg);
//                Global_UF[efg].rawarge = __ms_te_def_arg;
                Global_UF[efg].linestart = __ms_line+1;
                Global_UF[efg].lineend = 0;
                Global_UF[efg].environment = 1;
                Global_UF[efg].await = 1;
            } else if(strcmp(__ms_te_fun, "__dump_func") == 0){
                for(int efg = 0; efg < 10000; efg++){
                    if(Global_UF[efg].id == efg && efg != 0){
                        printf("+ Name: %s\n", Global_UF[efg].name);
                        printf("    * Argcount: %d\n", Global_UF[efg].argcount);
                        printf("    * Args: %s\n", Global_UF[efg].rawarge);
                        printf("    * Line: %d to %d\n", Global_UF[efg].linestart, Global_UF[efg].lineend);
                    }
                }
            } else if(__ms_inf_isspace > 0 && strcmp(__ms_inf_namespace, "nmath") == 0){
                if(strcmp(__ms_inf_namespdef, "add") == 0){
                    printf("add");
                } else {
                    __throw(MS_ERROR_11, __ms_line+1, 1, strlen(__ms_inf_namespdef)+4, __ms_buffer);
                }
            } else if(__ms_inf_isspace > 0 && strcmp(__ms_inf_namespace, "ms") == 0){
                if(strcmp(__ms_inf_namespdef, "compare") == 0){
                    int ccounts = 0;
                    int tcounts = 0;
                    int ecounts = 0;
                    char * one = _narg(1, __ms_inf_args);
                    char * two = _narg(2, __ms_inf_args);

                    if(strlen(one) != strlen(two)){
                        ccounts = 0;
                    } else {
                        for(int ef = 0; ef < strlen(one); ef++){
                            if(one[ef] == two[ef]){
                                ecounts++;
                            }
                            tcounts++;
                        }
                        if(tcounts == ecounts){ ccounts = 1; }
                    }
                    if(ccounts == 0){ __ms_inf_var_retn = "0"; } else { __ms_inf_var_retn = "1"; }
                } else if(strcmp(__ms_inf_namespdef, "about") == 0){
                    printf("H&A MS Version 2\n");
                    printf("|-> MS is created, managed and programmed by H&A Industries\n");
                    printf("|-> MS is an interperated programming language.\n|-> H&A operates a website and documentation for it at ms.hng.su.mt\n");
                } else if(strcmp(__ms_inf_namespdef, "denaryToBinary") == 0){
                    char * bits = _narg(2, __ms_inf_args);
                    char * no = _narg(1, __ms_inf_args);
                    if(bits[strlen(bits)-1] == '"'){ bits[strlen(bits)-1] = '\0'; }
                    if(no[strlen(no)-1] == '"'){ no[strlen(no)-1] = '\0'; }
                    if(strcmp(bits, "4") == 0){
                        /*
                        old demo code! use the x8 method
                        */
                        if(atoi(no) > 15){ __throw(MS_ERROR_12, __ms_line+1, 1, strlen(__ms_inf_namespdef)+7, __ms_buffer); }
                        char end[1024];
                        int no1 = atoi(no);
                        if(no1 - 8 > -1){ strcat(end, "1"); no1 = no1 - 8;
                            } else { strcat(end, "0"); }
                        if(no1 - 4 > -1){ strcat(end, "1"); no1 = no1 - 4;
                            } else { strcat(end, "0"); }
                        if(no1 - 2 > -1){ strcat(end, "1"); no1 = no1 - 2;
                            } else { strcat(end, "0"); }
                        if(no1 - 1 > -1){ strcat(end, "1"); no1 = no1 - 1;
                            } else { strcat(end, "0"); }
                        char * fi = end;
//                        printf("%s\n", fi);
//                        free(end);
                        __ms_inf_v_ns = 1;
                        strcpy(__ms_inf_v_retn, end);
//                        __ms_inf_v_retn = end;
                    } else if(strcmp(bits, "8") == 0){
                        if(atoi(no) > 255){ __throw(MS_ERROR_12, __ms_line+1, 1, strlen(__ms_inf_namespdef)+7, __ms_buffer); }
                        char end[9];
                        int arr[] = {128, 64, 32, 16, 8, 4, 2, 1};
                        int no1 = atoi(no);
                        for(int x = 0; x < 8; x++){
                            if(no1 - arr[x] > -1){
                                no1 = no1 - arr[x];
                                strcat(end, "1");
                            } else {
                                strcat(end, "0");
                            }
                        }
                        printf("%s\n", end);
                    }
                } else {
                    __throw(MS_ERROR_11, __ms_line+1, 1, strlen(__ms_inf_namespdef)+4, __ms_buffer);
                }

            } else {
                int __ms_inf_fv = 0;
                int __ms_inf_id = 0;
                for(int ef_g = 0; ef_g < 10000; ef_g++){
                    if(Global_UF[ef_g].environment > 0 && Global_UF[ef_g].environment != 0){
                        if(strcmp(Global_UF[ef_g].name, __ms_te_fun) ==0){
                            __ms_inf_fv = 1;
                            __ms_inf_id = ef_g;
                        }
                    }
                }

                if(__ms_inf_fv == 0){
                    __throw(MS_ERROR_5, __ms_line+1, 1, strlen(__ms_buffer)-1, __ms_buffer);
                } else {
                    /*
                     This is for when we create a nicer virtual function environment option.
                    */
                    if(Global_UF[__ms_inf_id].environment == 1){
                        __reg_fun(__reg_naf(), __ms_te_fun, MS_CALL_EV1, 0, 1);
                        FILE *__ms_ve_file = fopen(argv[1], "r");
                        FILE *__ms_ve_temp = fopen("/tmp/.mse", "a+");
                        if(__ms_ve_file == NULL || __ms_ve_temp == NULL){
                            __throw(MS_ERROR_10, __ms_line+1, 1, 4, "Opening of VirtualEnvironment");
                        }

                        char __ms_ve_buffer[MS_STACK_FL];
                        int __ms_ve_buffline = 0;

                        int __ms_ev_t_fp = 0;
                        int __ms_ev_t_lp = 0;

                        if(Global_UF[__ms_inf_id].argcount == 1){
                            if(__arg(1, 1, __ms_inf_args)[0] != '$'){
                                fprintf(__ms_ve_temp, "%s = \"%s\"\n", Global_UF[__ms_inf_id].rawarge, __arg(1, 1, __ms_inf_args));
                            }
                        }


                        while(fgets(__ms_ve_buffer,
                                    MS_STACK_FL,
                                    __ms_ve_file))
                        {
                            if(__ms_ve_buffline+1 == Global_UF[__ms_inf_id].linestart){
                                // rawarg doesn't seem to work
                                for(int eh = 0; eh < strlen(__ms_ve_buffer); eh++){
                                    if(__ms_ve_buffer[eh] == '('){
                                        __ms_ev_t_fp = eh+1;
                                    } else if(__ms_ve_buffer[eh] == ')'){
                                        __ms_ev_t_lp = eh+1;
                                    }
                                }
                            }

                            if(__ms_ve_buffline+1 > Global_UF[__ms_inf_id].linestart && __ms_ve_buffline+1 < Global_UF[__ms_inf_id].lineend){
                                    fputs(__ms_ve_buffer, __ms_ve_temp);
                            }
                            __ms_ve_buffline++;
                        }


                        FILE * __ms_ve_open = popen("./ms /tmp/.mse", "r");
                        char __ms_ve_buffer2[MS_STACK_FL];
                        if(__ms_ve_open == NULL){
                            __throw(MS_ERROR_10, __ms_line+1, 1, 4, "Opening MS Virtual (venv temp file)");
                        }
                        int __ms_times = 1;
                        fclose(__ms_ve_file);
                        fclose(__ms_ve_temp);

                        while(fgets(__ms_ve_buffer2,
                              MS_STACK_FL,
                              __ms_ve_open)){
                                printf("%s", __ms_ve_buffer2);
                        }
                        remove("/tmp/.mse");
                        int code = WEXITSTATUS(pclose(__ms_ve_open));
                        if(code != 0){ exit(-1); }

                        __reg_pop();
                    } else {
                        __throw(MS_ERROR_9, __ms_line+1, 1, 4, "Handling of virtual function environment");
                    }
                }
            }
            if(__ms_inf_isretn == 1 &&  __ms_inf_var_equal > 0){
                if(__ms_inf_v_ns == 1){
                    __var_new(__ms_line+1, __ms_inf_var_name, __ms_inf_v_retn, 1);
                } else {
                    __var_new(__ms_line+1, __ms_inf_var_name, __ms_inf_var_retn, 1);
                }
            }
        }
        __reg_pop();
        __reg_pop();
        __ms_line = __ms_line + 1;
        }
    }
    free(__ms_buffer);
    __reg_pop();
}
