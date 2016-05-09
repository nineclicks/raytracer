//Nicholas Gardner
//cs371
//Project - Ray Tracer

//#include <cmath>
#include <fstream>
#include <vector>
#include <string>
//#include <sstream>
#include <cstdlib>

#include <cstring>

#include "scene.h"
#include "vector.h"
#include "object.h"
#include "parser.h"
#include "fimage.h"

#define uint unsigned int

using namespace std;

class fimage;

int main(int argc, char *argv[])
{
    if (argc < 2) {
        printf("Usage: './project <scene file name> <options>'\n");
        return 1;
    }        

    Scene sc;
    Parser Parse;
    Parse.parse(&sc, argv[1]);

    for (int i = 2; i < argc; i++) {
        if (argv[i][0] == '-' && argv[i][1] == 'p'){
            sc.superSample = 1;
            sc.softShadows = 0;
            sc.recursion = 0;
            sc.occlusion = 0;
        } else if (argv[i][0] == '-' && argv[i][1] == 'w'){
            sc.superSample = 2;
            sc.softShadows = 0;
            sc.recursion = 6;
            sc.occlusion = 0;
            sc.setRes(600,400,256);
        } else if (argv[i][0] == '-' && argv[i][1] == 'o'){
			strcpy(sc.image.fileName,argv[i+1]);
		}
    }

    sc.drawScene();

    sc.image.save();
    return 0;
}

