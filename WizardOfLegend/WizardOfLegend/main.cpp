#include <SFML/Graphics.hpp>
#include "Framework/Framework.h"

int main()
{
    FRAMEWORK->Init(1980, 1080);
    FRAMEWORK->Do();
    FRAMEWORK->~Framework();
    return 0;
}