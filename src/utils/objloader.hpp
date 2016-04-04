#ifndef OBJLOADER_H
#define OBJLOADER_H

#include <memory>

#include "Model.h"

std::shared_ptr<Model> loadOBJ(const char* path);

#endif