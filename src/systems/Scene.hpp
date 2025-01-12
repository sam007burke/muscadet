#ifndef SCENE_HPP
#define SCENE_HPP

#include <vector>

class Scene {
    public:
        void loadScene(const char* filepath);
    private:
        std::vector<int> entities;
};

#endif
