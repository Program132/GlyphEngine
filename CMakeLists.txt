cmake_minimum_required(VERSION 4.0)
project(GlyphEngine C)

set(CMAKE_C_STANDARD 23)

add_executable(GlyphEngine
        main.c
        src/vec2/vec2.h
        src/vec2/vec2.c
        src/level/level.c
        src/level/level.h
        src/utils/utils.h
        src/utils/utils.c
        src/color/color.h
        src/color/color.c
        src/collision/collision.h
        src/collision/collision.c
        src/texture/texture.h
        src/texture/texture.c
        src/gameobject/point/point.h
        src/gameobject/point/point.c
        src/gameobject/rectangle/rectangle.h
        src/gameobject/rectangle/rectangle.c
        src/gameobject/ellipse/ellipse.h
        src/gameobject/ellipse/ellipse.c
        src/gameobject/square/square.h
        src/gameobject/square/square.c
        src/gameobject/circle/circle.h
        src/gameobject/circle/circle.c
        src/gameobject/player/player.h
        src/gameobject/player/player.c
        src/gameobject/text/text.h
        src/gameobject/text/text.c
        src/ui/button/button.h
        src/ui/button/button.c
        src/ui/panel/panel.h
        src/ui/panel/panel.c
        src/levelui/levelui.h
        src/levelui/levelui.c
        src/levelworld/levelworld.h
        src/levelworld/levelworld.c
        src/projectile/projectile.h
        src/projectile/projectile.c
        src/particle/particle.h
        src/particle/particle.c
        src/physics/physics.h
        src/physics/physics.c
        src/engine/engine.h
        src/engine/engine.c
        src/inputs/inputs.h
        src/inputs/inputs.c
)
