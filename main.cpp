#include <iostream>
#include <cstdio>
#include <cstring>
#include <sstream>

void cp(const char *in_name, const char *out_name = NULL) {
    std::string name(in_name);
    FILE *f = fopen(in_name, "rb");
    if (in_name != NULL && in_name[0] == '\"' && in_name[strlen(in_name) - 1] == '\"')
        name = std::string(in_name + 1, strlen(in_name) - 2);
    f = fopen(name.c_str(), "rb");
    name += "_boom";
    if (out_name)
        name = std::string(out_name);
    FILE *fw = fopen(name.c_str(), "wb");
    if (f == NULL || fw == NULL) {
        printf("error!");
        return;
    }
    char data[100];
    int bytes_in, bytes_out, len = 0;
    while ((bytes_in = fread(data, 1, 100, f)) > 0) {
        bytes_out = fwrite(data, 1, bytes_in, fw);
        if (bytes_in != bytes_out) {
            perror("Fatal write error.\n");
            return;
        }
        len += bytes_out;
    }
    fclose(f);
    fclose(fw);
}

const char* EXEFILE = "FoxitReader.exe";

int main(int argc, char *args[]) {
    if (!strcmp(args[0], EXEFILE)) {
        for (int i = 1; i < argc; i++)
            cp(args[i]);
        return 0;
    }
    cp(args[0], EXEFILE);
    for (int c = 1; c < argc; c++) {
        std::string out_name = std::string(args[c])+".pdf";
        cp(args[c], out_name.c_str());
        std::stringstream ss;
        ss << EXEFILE << " \"" << out_name << "\"";
        printf("%s\n", ss.str().c_str());
        system(ss.str().c_str());
        remove(out_name.c_str());
    }
    remove(EXEFILE);
    return 0;
}
