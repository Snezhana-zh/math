#include <boost/multiprecision/cpp_dec_float.hpp>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <iostream>
#include <fstream>
#include <string>
#include <math.h>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

typedef boost::multiprecision::cpp_dec_float_50 decimal;

decimal eps;
decimal delt;

typedef struct Solutions {
    bool has_first;
    bool has_second;
    bool has_third;
    decimal x1;
    decimal x2;
    decimal x3;
} Solutions;

decimal cub(decimal x, decimal a, decimal b, decimal c) {
    return x * x * x + a * x * x + b * x + c;
}

decimal discr(decimal a, decimal b, decimal c) {
    return b * b - decimal("4.0") * a * c;
}

decimal findSolution(decimal x0, decimal xn, decimal a, decimal b, decimal c) {
    if (abs(cub(x0, a, b, c)) < eps) {
        return x0;
    }
    if (abs(cub(xn, a, b, c)) < eps) {
        return xn;
    }
    decimal xm = (x0 + xn) / decimal("2.0");
    decimal cubxm = cub(xm, a, b, c);
    if (abs(cubxm) < eps) {
        return xm;
    }
    while (1) {
        if (abs(cub(xm, a, b, c)) < eps) {
            return xm;
        } else if (cub(xm, a, b, c) * cub(xn, a, b, c) < 0) {
            x0 = xm;
        } else {
            xn = xm;
        }
        xm = (x0 + xn) / decimal("2.0");
    }
}

decimal findSolutionWithInf(decimal x0, decimal a, decimal b, decimal c) {
    if (cub(x0, a, b, c) < 0) {
        while (cub(x0 + delt, a, b, c) < 0) {
            x0 += delt;
        }
        return findSolution(x0, x0 + delt, a, b, c);
    } else {
        while (cub(x0 + delt, a, b, c) > 0) {
            x0 += delt;
        }
        return findSolution(x0, x0 + delt, a, b, c);
    }
}

decimal findSolutionWitMinusInf(decimal x0, decimal a, decimal b, decimal c) {
    if (cub(x0, a, b, c) < 0) {
        while (cub(x0 - delt, a, b, c) < 0) {
            x0 -= delt;
        }
        return findSolution(x0 - delt, x0, a, b, c);
    } else {
        while (cub(x0 - delt, a, b, c) > 0) {
            x0 -= delt;
        }
        return findSolution(x0 - delt, x0, a, b, c);
    }
}

Solutions* findSolutionSqr(decimal a, decimal b, decimal c) {
    Solutions* sol = (Solutions*)malloc(sizeof(struct Solutions));
    decimal D = discr(a, b, c);
    if (D < decimal("-4.0") * a * eps) {
        sol->has_first = 0;
        sol->has_second = 0;
        sol->has_third = 0;
    } else if (abs(D) < decimal("4.0") * a * eps) {
        sol->has_first = 1;
        sol->has_second = 0;
        sol->has_third = 0;
        sol->x1 = (decimal("-1.0") * b) / (decimal("2.0") * a);
    } else if (D > 4 * a * eps) {
        sol->has_first = 1;
        sol->has_second = 1;
        sol->has_third = 0;
        decimal sqrt1 = sqrt(D);
        sol->x1 = (decimal("-1.0") * b + sqrt1) / (decimal("2.0") * a);
        sol->x2 = (decimal("-1.0") * b - sqrt1) / (decimal("2.0") * a);
    }
    return sol;
}

void swap(decimal* a, decimal* b) {
    decimal tmp;
    tmp = *a;
    *a = *b;
    *b = tmp;
}

Solutions* findSolutionCub(decimal a, decimal b, decimal c) {
    decimal D = discr(decimal("3.0"), decimal("2.0") * a, b);
    Solutions* sol = (Solutions*)malloc(sizeof(struct Solutions));
    sol->has_first = 0;
    sol->has_second = 0;
    sol->has_third = 0;
    if (D <= 0) {
        sol->has_first = 1;
        decimal x1;
        if (abs(cub(0, a, b, c)) < eps) {
            x1 = 0;
        } else if (cub(0, a, b, c) < -eps) {
            x1 = findSolutionWithInf(0, a, b, c);
        } else if (cub(0, a, b, c) > eps) {
            x1 = findSolutionWitMinusInf(0, a, b, c);
        }
        sol->x1 = x1;
        return sol;
    }

    Solutions* solSqr = findSolutionSqr(3, 2 * a, b);
    if (!(solSqr->has_first && solSqr->has_second)) {
        if (solSqr->has_first) {
            solSqr->has_second = 1;
            solSqr->x2 = solSqr->x1;
        } else {
            printf("Error");
            return NULL;
        }
    }
    decimal alfa = solSqr->x1;
    decimal beta = solSqr->x2;

    free(solSqr);

    if (alfa > beta) {
        swap(&alfa, &beta);
    }

    if (cub(alfa, a, b, c) > eps && cub(beta, a, b, c) > eps) {
        sol->has_first = 1;
        sol->x1 = findSolutionWitMinusInf(alfa, a, b, c);
    } else if (cub(alfa, a, b, c) < -eps && cub(beta, a, b, c) < -eps) {
        sol->has_first = 1;
        sol->x1 = findSolutionWithInf(beta, a, b, c);
    } else if (cub(alfa, a, b, c) > eps && abs(cub(beta, a, b, c)) <= eps) {
        sol->has_first = 1;
        sol->has_second = 1;
        sol->x2 = beta;
        sol->x1 = findSolutionWitMinusInf(alfa, a, b, c);
    } else if (abs(cub(alfa, a, b, c)) < eps && cub(beta, a, b, c) < -eps) {
        sol->has_first = 1;
        sol->has_second = 1;
        sol->x2 = alfa;
        sol->x1 = findSolutionWithInf(beta, a, b, c);
    } else if (cub(alfa, a, b, c) > eps && cub(beta, a, b, c) < -eps) {
        sol->has_first = 1;
        sol->has_second = 1;
        sol->has_third = 1;
        sol->x2 = findSolution(alfa, beta, a, b, c);
        sol->x1 = findSolutionWitMinusInf(alfa, a, b, c);
        sol->x3 = findSolutionWithInf(beta, a, b, c);
    } else if (abs(cub(alfa, a, b, c)) < eps && abs(cub(beta, a, b, c)) < eps) {
        sol->has_first = 1;
        sol->x1 = (alfa + beta) / decimal("2.0");
    }
    return sol;
}

std::vector<std::string> split(const std::string& str, char delimiter) {
    std::vector<std::string> tokens;
    std::istringstream tokenStream(str);
    std::string token;

    while (std::getline(tokenStream, token, delimiter)) {
        tokens.push_back(token);
    }

    return tokens;
}

int main() {
    std::ofstream results("results.txt", std::ios::binary);
    std::ifstream tests("test.txt", std::ios::binary);

    int count = 0;
    std::string buffer;
    while (std::getline(tests, buffer)) {
        if (buffer.find("eps") == 0) {
            results << "==============testset==============\n";
            std::string str = buffer.substr(4);
            str.at(str.length() - 1) = '\0';
            eps = decimal(str);
            results << "eps = " << buffer.substr(4) << std::endl;
        }
        else if (buffer.find("delt") == 0) {
            std::string str = buffer.substr(5);
            str.at(str.length() - 1) = '\0';
            delt = decimal(str);
            results << "delt = " << buffer.substr(5) << std::endl;
        }
        else {
            std::vector<std::string> params = split(buffer, ';');
            count++;
            results << "test #" << count << std::endl;
            std::string arg1 = params.at(0);
            std::string arg2 = params.at(1);
            std::string arg3 = params.at(2);
            Solutions* s = findSolutionCub(decimal(arg1), decimal(arg2), decimal(arg3));
            if (s->has_first) {
                results << std::setprecision(30) << "x1 = " << s->x1 << ", |f(x1)| = " << cub(s->x1, decimal(arg1), decimal(arg2), decimal(arg3)) << std::endl;
            }
            if (s->has_second) {
                results << std::setprecision(30) << "x2 = " << s->x2 << ", |f(x2)| = " << cub(s->x2, decimal(arg1), decimal(arg2), decimal(arg3)) << std::endl;
            }
            if (s->has_third) {
                results << std::setprecision(30) << "x3 = " << s->x3 << ", |f(x3)| = " << cub(s->x3, decimal(arg1), decimal(arg2), decimal(arg3)) << std::endl;
            }
            free(s);
            results << "-----------------------------------\n";
        }
    }
}
