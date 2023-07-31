#pragma once

#include <Application.hpp>

class NativeApplicationX : public Application {
protected:
    void beginRendering(float delta) override;
public:
    void prerender(float delta) override;

    NativeApplicationX(DWM *owner);
    ~NativeApplicationX();
};