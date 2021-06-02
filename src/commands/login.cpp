#include "login.h"

std::vector<std::string> split(std::string s, std::string delim) {
    char *p = &s[0];
    char *d = &delim[0];
    std::vector<std::string> res = {""};

    do {
        bool is_delim = true;
        char *pp = p;
        char *dd = d;
        while (*dd && is_delim == true) {
            if (*pp++ != *dd++)
                is_delim = false;
        }

        if (is_delim) {
            p = pp - 1;
            res.push_back("");
        } else {
            *(res.rbegin()) += *p;
        }
    } while (*p++);
    return res;
}

static std::vector<const char *> HELP_MSGS = {
    "login <username> [password]",
    "Usage login",
    "",
    "  -h, --help    get help message.",
    "",
    "login user."
};

int main (
    int argc, char* *args
) {
    if (argc <= 1) {
        std::cout << "Params is empty." << std::endl;
        return 1;
    }
    for (int i = 0;i < argc; i++) {
        std::string arg = args[i];
        if (
            arg == "-h" || arg == "--help"
        ) {
            for (const auto &HELP_MSG : HELP_MSGS) {
                std::cout << HELP_MSG << std::endl;
            }
            return 0;
        }
    }
    std::string username = args[1];

    std::string line;
    std::ifstream file;
    file.open(
        std::string(getpwuid(getuid())->pw_dir) + "/.sfs.users"
    );
    if(!file.is_open()) {
        std::cout << "ERROR: FILE IS NOT OPEN" << std::endl;
        return -1;
    }
    while(!file.eof()) {
        getline(file, line);
        if (!line.empty()) {
            auto lineItems = split(line, ":");
            std::string lineUsername = lineItems[0];
            std::string linePassword = lineItems[1].substr(0, lineItems[1].length() - 1);
            if (username == lineUsername) {
                if (linePassword.empty()) {
                    break;
                } else {
                    std::string password;
                    if (argc >= 3) {
                        password = args[2];
                    } else {
                        int i = 0;
                        for (; i < 3; ++i) {
                            std::cout << "please input password> ";
                            std::getline(std::cin, password);
                            if (Cryptor::simple(
                                password, "12"
                            ) == linePassword) {
                                break;
                            } else {
                                std::cout << "password is wrong." << std::endl;
                            }
                        }
                        if (i == 3) {
                            std::cout << "ERROR: Too many errors." << std::endl;
                            return -1;
                        }
                    }
                }
            }
        }
    }
    std::cout << "login success." << std::endl;
    return 0;
}
