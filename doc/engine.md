# Struct

| Argument       | Type                                              | Description                                                                     |
|----------------|---------------------------------------------------|---------------------------------------------------------------------------------|
| current_level  | struct Level*                                     | The active level/scene currently managed and displayed by the engine.           |
| width          | int                                               | Screen width for the engine.                                                    |
| height         | int                                               | Screen height for the engine.                                                   |
| is_running     | int                                               | `1` if the engine is running, `0` if stopped.                                   |
| on_update      | void (*)(struct Engine* engine, float delta_time) | Callback function pointer called every frame to update game logic.              |

# Functions

| Name                       | Type             | Arguments                                                                         | Description                                                                                                    |
|----------------------------|------------------|-----------------------------------------------------------------------------------|----------------------------------------------------------------------------------------------------------------|
| engine_new                 | struct Engine*   | struct Level *current_level, int width, int height                                | Dynamically allocates and initializes a new engine instance.                                                   |
| engine_build               | void             | struct Engine *engine, struct Level *current_level, int width, int height         | Initializes the fields of an existing engine instance.                                                         |
| engine_free                | void             | struct Engine *engine                                                             | Frees the engine, its current level via `level_free`, and all contained game objects.                           |
| engine_run                 | void             | struct Engine *engine, int fps                                                    | Starts the main loop at the specified target FPS. Blocks until `engine_stop`. Handles cursor visibility.       |
| engine_stop                | void             | struct Engine *engine                                                             | Stops the game loop, restores cursor visibility, and flushes keyboard input.                                    |
| engine_set_level           | void             | struct Engine *engine, struct Level *new_level                                    | Changes the active level/scene.                                                                                 |
| engine_set_update_callback | void             | struct Engine *engine, void (*update_func)(struct Engine*, float)                 | Sets the callback function invoked each frame to update game logic.                                             |
